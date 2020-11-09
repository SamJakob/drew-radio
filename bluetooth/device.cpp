#ifndef DR_BLUETOOTH_DEVICE
#define DR_BLUETOOTH_DEVICE

using namespace std;

namespace DrewRadio {

    struct BluetoothDevice {
        char address[19];
        char name[248];
    };

}

#endif
