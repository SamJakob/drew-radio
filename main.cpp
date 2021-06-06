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

    /*************************************************************
    ********** STARTUP PROCEDURE
    ********** -----------------
    **********
    ********** Once the program starts up, the first task is to
    ********** scan for all bluetooth devices.
    **********
    ********** Once the program has 'taken inventory' of all the
    ********** bluetooth devices, it will first attempt to connect
    ********** to any previously saved devices in priority order,
    ********** failing that, it will enter pairing mode and wait
    ********** for new devices.
    **********
    **************************************************************
    */

    cout << "Initializing..." << endl;

    // The main bluetooth controller.
    Bluetooth* bluetooth = new Bluetooth();
    cout << endl;

    // The next device to attempt to connect to on startup.
    //
    // Once control has 'left' this scope, reconnectDevice will
    // either be populated with the currently active device or it
    // will be null and we will enter 'pairing mode' until we have
    // successfully paired and connected with a device.
    BluetoothDevice* reconnectDevice = nullptr;

    // Locate bluetooth devices and attempt to select the next
    // known device.
    cout << ">> Scanning for bluetooth devices..." << endl;

    int foundDevices = bluetooth->scanForDevices();
    if (foundDevices > 0) {
        cout << ">> Found " << foundDevices << " device(s)!" << endl;

        for (BluetoothDevice device : *bluetooth->devices) {
            cout << ">>>> " << device.address << " (" << device.name << ")" << endl;
        }

        // reconnectDevice = &bluetooth->devices->front();
    }

    cout << endl;

    // Break out into 'pairing mode' if necessary. If not, allow
    // execution to continue to the code responsible for playing
    // audio from the bluetooth device.
    if (reconnectDevice == nullptr) {
        cout << ">> Did not find any known bluetooth devices." << endl;
        cout << ">> Entering pairing mode..." << endl;
        bluetooth->waitForNextDevice();
    }

    delete bluetooth;
    return 0;

}
