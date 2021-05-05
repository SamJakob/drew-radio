#ifndef DR_BLUETOOTH_DEVICE
#define DR_BLUETOOTH_DEVICE

using namespace std;

#define UNKNOWN_BLUETOOTH_DEVICE_NAME "Unknown Device"

namespace DrewRadio {

    /**
     * Stores basic information about bluetooth devices, required for
     * connection and reference.
     */
    struct BluetoothDevice {
        /**
         * A null-terminated string containing the MAC address, formatted
         * as ASCII and colon-delimited (:).
         *
         * The length of this string is always 18 chars (with the null
         * termination byte.)
         */
        char address[18];

        /**
         * The human-readable display name of the device. If this was
         * unavailable, this will be set to UNKNOWN_BLUETOOTH_DEVICE_NAME
         * (which is "Unknown Device" by default).
         */
        char name[255];
    };

}

#endif
