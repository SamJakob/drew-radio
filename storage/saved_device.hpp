#include <cstdint>
#include <fstream>

#include "../bluetooth/device.hpp"

#ifndef DR_STORAGE_SAVED_DEVICE
#define DR_STORAGE_SAVED_DEVICE

namespace DrewRadio {

    /**
     * Represents a device that has been paired with. This can be used to store additional properties
     * about the device, such as the connection priority and the name associated with the device.
     *
     * This is intended to be written to storage and in binary-based storage, the SavedDevice data will
     * be preceded by the magic number DR_MAGIC_SAVED_DEVICE.
     */
    class SavedDevice {

        private:
            BluetoothDevice* bluetoothDevice;

        public:
            /**
             * If specified, the display name that will be used to refer to the device (overriding
             * the device's display name.)
             */
            char* customName;

            /**
             * The device's priority for connection. When the software starts, it will attempt to
             * connect to the saved device with the highest priority and continue until it has
             * attempted to connect to all the saved devices, at which point it will enter 'pairing
             * mode' and wait for a new device.
             *
             * Highest value = highest priority.
             * The default priority is 0 which is 'lowest priority' (semantically 'unprioritized).
             */
            uint8_t priority;

            SavedDevice(
                BluetoothDevice* bluetoothDevice,
                char* customName = nullptr,
                uint8_t priority = 0
            );

            /**
             * Returns the display name based on customName and the bluetooth device's reported name.
             * (Will default to customName if specified, falling back to the bluetooth device's reported name if not.)
             *
             * @return The most appropriate display name.
             */
            const char* getDisplayName();

            /**
             * Writes the SavedDevice data to the specified outputStream, this data is preceded by the magic number
             * associated with this class (DR_MAGIC_SAVED_DEVICE).
             *
             * This is intended to be read by the SavedDevice::load function.
             */
            void save(ofstream& outputStream);

            /**
             * Loads the SavedDevice data from the specified inputStream. This will first validate that the next
             * item in the inputStream is the magic number equal to DR_MAGIC_SAVED_DEVICE.
             *
             * This is intended to read only data saved by the SavedDevice::save function.
             */
            static SavedDevice* load(ifstream& inputStream);

    };

}

#endif
