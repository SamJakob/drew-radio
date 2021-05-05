#include <iostream>
#include <cstdlib>
#include <unistd.h>

/**
 * @namespace DrewRadio
 * All classes supporting the definition and implementation of the
 * drew-radio bluetooth-to-aux kit project.
 */

#include "bluetooth/device.hpp"
#include "bluetooth/bluetooth.hpp"

using namespace std;
using namespace DrewRadio;

int main(int argc, char** argv) {

    // Locate bluetooth devices.
    cout << ">> Scanning for bluetooth devices..." << endl;

    int foundDevices = Bluetooth::scanForDevices();
    if (foundDevices > 0) {
        cout << ">> Found " << foundDevices << " device(s)!" << endl;

        for (BluetoothDevice device : Bluetooth::devices) {
            cout << ">>>> " << device.address << " (" << device.name << ")" << endl;
        }
    } else {
        cout << ">> Did not find any bluetooth devices." << endl;
    }



    return 0;

}
