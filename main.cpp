#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include "bluetooth/device.cpp"
#include "bluetooth/bluetooth.cpp"

using namespace std;
using namespace DrewRadio;

int main(int argc, char** argv) {

    cout << ">> Scanning for bluetooth devices..." << endl;
    cout << ">> Found " << Bluetooth::scanForDevices() << " device(s)!" << endl;

    for (BluetoothDevice device : Bluetooth::devices) {
        cout << ">>>> " << device.address << " (" << device.name << ")" << endl;
    }

    return 0;

}