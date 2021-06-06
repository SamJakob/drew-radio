Designed for use on the [Raspberry Pi Zero W](https://www.raspberrypi.org/products/raspberry-pi-zero-w/?resellerType=home).

## Dependencies
- Bluez's headers are required and can be installed with `sudo apt install libbluetooth-dev`.

## Usage
- Copy `config.hpp.example` to `config.hpp` and customize as desired.

```bash
# Prepare the directories for the first time
# (or clean on rebuild)
$ make clean

# Build the project in debug mode.
$ make debug

# Build for production
$ make
```
