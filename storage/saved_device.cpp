#include <cstring>

#include "saved_device.hpp"

using namespace DrewRadio;

const char* SavedDevice::getDisplayName() {

    // Return the device's custom name if it exists.
    if (this->customName != nullptr) return this->customName;

    // Otherwise, return the bluetooth device's name if it exists.
    if (strlen(this->bluetoothDevice->name) > 0) return this->bluetoothDevice->name;

    // Finally, if neither were set, return the unknown device name string.
    return UNKNOWN_BLUETOOTH_DEVICE_NAME;

}

void SavedDevice::save(ofstream& outputStream) {
    // Magic Number (DR_MAGIC_SAVED_DEVICE)
    outputStream.write(DR_MAGIC_SAVED_DEVICE, sizeof(DR_MAGIC_SAVED_DEVICE));
    // bluetoothDevice
    outputStream.write(&(this->bluetoothDevice), sizeof(BluetoothDevice));
    // customName

    // priority
}

void SavedDevice::load(ifstream& inputStream) {
}
