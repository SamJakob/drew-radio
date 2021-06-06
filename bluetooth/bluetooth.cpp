#include <cstdlib>
#include <cerrno>
#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include "../config.hpp"
#include "../exceptions.hpp"
#include "bluetooth.hpp"

using namespace DrewRadio;

Bluetooth::Bluetooth(const bool doLogging) {

    // Ensure we have a valid device ID (NULL => first available)
    this->deviceId = hci_get_route(nullptr);
    if (this->deviceId < 0) throw DrewRadio::InterfaceNotFoundException();

    // Reset the device state.
    this->resetDeviceState();

    // Set up the bluetooth interface details (name and class).
    int socket = hci_open_dev(this->deviceId);
    if (socket < 0) throw DrewRadio::InterfaceFailureException();

    hci_write_local_name(socket, DR_CONFIG_BLUETOOTH_NAME, strlen(DR_CONFIG_BLUETOOTH_NAME));
    hci_write_class_of_dev(socket, DR_CONFIG_BLUETOOTH_CLASS, 0);

    if (doLogging) std::cout << "Initialized bluetooth device: \""
                << DR_CONFIG_BLUETOOTH_NAME
                << "\" (0x" << std::hex << DR_CONFIG_BLUETOOTH_CLASS << std::dec << ")!"
                << std::endl;

    close(socket);

    // Initialize a list to contain the devices.
    this->devices = new std::list<BluetoothDevice>;

}

Bluetooth::~Bluetooth() {

    hci_close_dev(this->deviceId);

    if (this->devices) {
        delete this->devices;
        this->devices = nullptr;
    }

}

int Bluetooth::scanForDevices(const unsigned int searchDuration) {

    // Clear previously discovered devices.
    this->devices->clear();

    // Get device and socket ID.
    int maxResponseCount = 255;

    // Allocate memory to store the bluetooth inquiry results.
    inquiry_info* ii = (inquiry_info*) malloc(maxResponseCount * sizeof(inquiry_info));

    // Perform a bluetooth inquiry and collect the number of responses.
    int numberOfResponses = hci_inquiry(
        this->deviceId,				// The local bluetooth device ID to scan with.
        searchDuration,			    // The length of time to search for.
        maxResponseCount,		    // The maximum number of devices to search for.
        NULL,
        &ii,				        // The memory location to return the results.
        IREQ_CACHE_FLUSH		    // Flush the cache of previously discovered devices.
    );

    // Open a socket to the bluetooth adaptor and start reading device information.
    int socket = hci_open_dev(this->deviceId);
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

            // Copy the device class.
            if (hci_read_class_of_dev(socket, device.deviceClass, 0) < 0) {
                *device.deviceClass = { 0 };
            }
        }

        (*devices).push_back(device);

    }

    // Clean up.
    close(socket);
    free(ii);
    ii = nullptr;

    // Return the detected data.
    this->devicesCount = numberOfResponses;
    return numberOfResponses;

}

void Bluetooth::setHciOptions(uint32_t hciOptions) {

    struct hci_dev_req deviceRequest;
    deviceRequest.dev_id = this->deviceId;

    int socketId = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);
    if (socketId < 0) throw DrewRadio::InterfaceFailureException();

    hci_write_local_name(socketId, DR_CONFIG_BLUETOOTH_NAME, strlen(DR_CONFIG_BLUETOOTH_NAME));
    hci_write_class_of_dev(socketId, DR_CONFIG_BLUETOOTH_CLASS, 0);

    deviceRequest.dev_opt = AUTH_ENABLED;
    if (ioctl(socketId, HCISETAUTH, (unsigned long) &deviceRequest) < 0) {
        std::cerr << "ERROR: Failed to set bluetooth auth mode: ioctl(): " << strerror(errno) << std::endl;
    }

    deviceRequest.dev_opt = ENCRYPT_BOTH;
    if (ioctl(socketId, HCISETENCRYPT, (unsigned long) &deviceRequest) < 0) {
        std::cerr << "ERROR: Failed to set bluetooth encryption mode: ioctl(): " << strerror(errno) << std::endl;
    }

    /*deviceRequest.dev_opt = HCI_LM_MASTER | HCI_LM_RELIABLE | HCI_LM_ENCRYPT | HCI_LM_AUTH;
    if (ioctl(socketId, HCISETLINKMODE, (unsigned long) &deviceRequest) < 0) {
        std::cerr << "ERROR: Failed to set bluetooth link mode: ioctl(): " << strerror(errno) << std::endl;
    }*/

    deviceRequest.dev_opt = hciOptions;
    if (ioctl(socketId, HCISETSCAN, (unsigned long) &deviceRequest) < 0) {
        std::cerr << "ERROR: Failed to set bluetooth scan mode: ioctl(): " << strerror(errno) << std::endl;
    }

    close(socketId);

}

void Bluetooth::resetDeviceState() { this->setHciOptions(SCAN_DISABLED); }

void Bluetooth::waitForNextDevice() {

    this->setHciOptions(SCAN_PAGE | SCAN_INQUIRY);

}
