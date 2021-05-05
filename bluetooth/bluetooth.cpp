#include <cstdlib>
#include <unistd.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include "../exceptions.hpp"
#include "bluetooth.hpp"

using namespace DrewRadio;

int Bluetooth::scanForDevices() {

    // Get device and socket ID.
    int searchDuration = 2;			 // Search Time = 1.28 * searchLength seconds.
    int maxResponseCount = 255;

    // Get the first available device ID (NULL => first available)
    int adaptorDeviceId = hci_get_route(NULL);
    if (adaptorDeviceId < 0) throw DrewRadio::InterfaceNotFoundException();

    // Allocate memory to store the bluetooth inquiry results.
    inquiry_info* ii = (inquiry_info*) malloc(maxResponseCount * sizeof(inquiry_info));

    // Perform a bluetooth inquiry and collect the number of responses.
    int numberOfResponses = hci_inquiry(
        adaptorDeviceId,				        // The local bluetooth device ID to scan with.
        searchDuration,			    // The length of time to search for.
        maxResponseCount,		    // The maximum number of devices to search for.
        NULL,
        &ii,				        // The memory location to return the results.
        IREQ_CACHE_FLUSH		    // Flush the cache of previously discovered devices.
    );

    // Open a socket to the bluetooth adaptor and start reading device information.
    int socket = hci_open_dev(adaptorDeviceId);
    if (socket < 0) throw DrewRadio::InterfaceFailureException();

    for (int i = 0; i < numberOfResponses; i++) {
        // Allocate a BluetoothDevice struct for the device.
        BluetoothDevice device = BluetoothDevice();

        // And copy the inquiry info into it.
        inquiry_info* currentDevice = ii + i;

        {
            // Copy the hex device address.
            ba2str(&currentDevice->bdaddr, device.address);

            // Copy the device name.
            if (hci_read_remote_name(socket, &currentDevice->bdaddr, sizeof(device.name), device.name, 0) < 0) {
                // Make the device name a 'nullish' value, so we can tell the difference between a device whose name
                // is set to UNKNOWN_BLUETOOTH_DEVICE_NAME and a device with an actually unknown name.
                strcpy(device.name, "\0");
            }
        }

        devices.push_back(device);

    }

    // Clean up.
    close(socket);
    free(ii);

    // Return the detected data.
    Bluetooth::devicesCount = numberOfResponses;
    return numberOfResponses;

}
