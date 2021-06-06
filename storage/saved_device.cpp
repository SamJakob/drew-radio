#include <cstring>

#include "saved_device.hpp"
#include "magic.hpp"
#include "../exceptions.hpp"

using namespace DrewRadio;

SavedDevice::SavedDevice(
    BluetoothDevice* bluetoothDevice,
    char* customName,
    uint8_t priority
) {
    this->bluetoothDevice = bluetoothDevice;
    this->customName = customName;
    this->priority = priority;
}

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
    outputStream.write(reinterpret_cast<const char*>(DR_MAGIC_SAVED_DEVICE), sizeof(DR_MAGIC_SAVED_DEVICE));

    // bluetoothDevice
    outputStream.write(reinterpret_cast<const char*>(&(this->bluetoothDevice)), sizeof(BluetoothDevice));

    // customName (preceded by length).
    int customNameLength = this->customName != nullptr ? strlen(this->customName) : 0;
    outputStream.write(reinterpret_cast<const char*>(&customNameLength), sizeof(int));
    outputStream.write(this->customName, strlen(this->customName));

    // priority
    outputStream.write(reinterpret_cast<const char*>(this->priority), sizeof(uint8_t));
}

SavedDevice* SavedDevice::load(ifstream& inputStream) {
    int readMagicNumber = 0;

    // Validate the magic number.
    inputStream.read(reinterpret_cast<char*>(&readMagicNumber), sizeof(DR_MAGIC_SAVED_DEVICE));
    if (readMagicNumber != DR_MAGIC_SAVED_DEVICE) {
        throw DrewRadio::FileFormatException();
    }

    // Read the bluetooth device information.
    BluetoothDevice* device = (BluetoothDevice*) malloc(sizeof(BluetoothDevice));
    inputStream.read(reinterpret_cast<char*>(device), sizeof(BluetoothDevice));

    // Read the customName (based on the length it is preceded by).
    int customNameLength = 0;
    inputStream.read(reinterpret_cast<char*>(&customNameLength), sizeof(int));

    char* customName = nullptr;

    if (customNameLength > 0) {
        customName = (char*) malloc(customNameLength);
        inputStream.read(customName, customNameLength);
    }

    // Read the device priority
    int priority = 0;
    inputStream.read(reinterpret_cast<char*>(&priority), sizeof(int));

    return new SavedDevice(device, customName, priority);
}
