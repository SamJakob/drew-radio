#include <list>

#include "device.hpp"

#ifndef DR_BLUETOOTH
#define DR_BLUETOOTH

namespace DrewRadio {

    /**
     * Manages information about Bluetooth devices.
     */
    class Bluetooth {

        private:
            /**
             * This can be used upon initialization and after pairing mode has
             * been entered to ensure the bluetooth hardware has been returned
             * to its ordinary state.
             *
             * This is merely an alias for setHciOptions with SCAN_DISABLED.
             */
            void resetDeviceState();

            /**
             * Used to set HCI protocol options on the bluetooth adaptor, such
             * as discoverability.
             */
            void setHciOptions(uint32_t hciOptions);

        public:
            Bluetooth(const bool doLogging = true);
            ~Bluetooth();

            /**
             * A vector containing (n=devicesCount) bluetooth devices that were found
             * in the last scan for devices.
             */
            std::list<BluetoothDevice>* devices;

            /**
             * The hardware device ID of the bluetooth adaptor. If this is -1, we know
             * that it is an invalid device ID. This is used to make calls to the
             * underlying system libraries to control the bluetooth hardware.
             */
            int deviceId = -1;

            /**
             * The amount of devices that were found in the last scan.
             * This is the number of devices stored in the devices vector.
             */
            int devicesCount = 0;

            /**
             * Scans for bluetooth devices. Returns the number of devices found if
             * successful, or throws an error if not.
             *
             * This data is then available statically via Bluetooth::devices and
             * Bluetooth::devicesCount.
             *
             * Total Search Time = 1.28 * searchDuration seconds.
             *
             * @param searchDuration The amount of time to search for devices.
             * @return The number of devices found.
             */
            int scanForDevices(const unsigned int searchDuration = 2);

            /**
             * Enables pairing mode and waits for the next bluetooth device to
             * either connect or pair with the system, returning the device.
             *
             * This method will block until the device is connected.
             */
            void waitForNextDevice();

    };

}

#endif
