#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include <vector>

#ifndef DR_BLUETOOTH
#define DR_BLUETOOTH

namespace DrewRadio {

    class Bluetooth {

        public:
            inline static vector<BluetoothDevice> devices;
            inline static int devicesCount;

            static int scanForDevices() {
                
                // Get device and socket ID.
                int len = 8, maxResponse = 255;
                int dev_id = hci_get_route(NULL);
                int sock = hci_open_dev(dev_id);

                // Perform a bluetooth inquiry and collect the number of responses.
                inquiry_info* ii = (inquiry_info*) malloc(maxResponse * sizeof(inquiry_info));
                int numberOfResponses = hci_inquiry(dev_id, len, maxResponse, NULL, &ii, IREQ_CACHE_FLUSH);
                
                char addr[19] = { 0 };
                char name[248] = { 0 };
                for (int i = 0; i < numberOfResponses; i++) {
                    // Convert the MAC address to a string and save it in addr.
                    ba2str(&(ii+i)->bdaddr, addr);
                    // Save the name to a string.
                    memset(name, 0, sizeof(name));

                    // If the name is invalid, set the name to "Unknown Device".
                    if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), name, 0) < 0) {
                        strcpy(name, "Unknown Device");
                    }

                    // Finally copy the data into a BluetoothDevice struct.
                    BluetoothDevice device = BluetoothDevice();
                    strncpy(device.address, addr, 19);
                    strncpy(device.name, name, 248);

                    devices.push_back(device);
                }

                Bluetooth::devicesCount = numberOfResponses;
                return numberOfResponses;

            }

    };

}

#endif
