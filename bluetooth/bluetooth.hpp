#include <vector>

#include "device.hpp"

#ifndef DR_BLUETOOTH
#define DR_BLUETOOTH

namespace DrewRadio {

    /**
     * Manages information about Bluetooth devices.
     */
    class Bluetooth {

        public:
            /**
             * A vector containing (n=devicesCount) bluetooth devices that were found
             * in the last scan for devices.
             */
            inline static vector<BluetoothDevice> devices;

            /**
             * The amount of devices that were found in the last scan.
             * This is the number of devices stored in the devices vector.
             */
            inline static int devicesCount = 0;

            /**
             * Scans for bluetooth devices. Returns the number of devices found if
             * successful, or throws an error if not.
             *
             * This data is then available statically via Bluetooth::devices and
             * Bluetooth::devicesCount.
             *
             * @return The number of devices found.
             */
            static int scanForDevices();

    };

}

#endif
