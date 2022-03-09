# Multiple Retro Game Controllers with one RP2040
Quick hack to connect up to 4 retro 2-button joysticks to (hopefully) any USB-host.

Currently only tested with 1-button Amiga joysticks.

Default configuration:
Only Joystick 1

|              |  Up    | Down   | Left   | Right  | Fire 1 | Fire 2 |
| -------------|--------|--------|--------|--------|--------|--------|
| Joystick 1   | GPIO0  | GPIO1  | GPIO2  | GPIO03 | GPIO4  | GPIO5  |
| Joystick 2   | GPIO6  | GPIO7  | GPIO8  | GPIO09 | GPIO10 | GPIO11 |
| Joystick 3   | GPIO12 | GPIO13 | GPIO14 | GPIO15 | GPIO16 | GPIO17 |
| Joystick 4   | GPIO18 | GPIO19 | GPIO20 | GPIO21 | GPIO22 | GPIO26 |

Change ```CFG_TUD_HID``` to the desired number of joysticks.

Pin configuration can be changed in ```joygpio_cfg```.

All pins use the internal pullups so GND has to be connected to the other side of the switches (ie. Pin 8 on Amiga D-Sub-9).

Based on https://github.com/Drewol/rp2040-gamecon

## Build instructions
Instructions based on Ubuntu 21.10

1. Install CMake (at least version 3.13), and GCC cross compiler
```
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi
```

2. Download the pico-sdk and set the `PICO_SDK_PATH` environment variable. Also set the variable in your `.bashrc` for future use.
```
git clone https://github.com/raspberrypi/pico-sdk
cd pico-sdk
git submodule update --init
export PICO_SDK_PATH=$PWD
cd ..
```

3. Clone and build this repository
```
git clone https://github.com/JeremiasSpiegel/rp2040-retrodb9x4
cd rp2040-retrodb9x4
mkdir build
cd build
cmake ..
make
```

4. Upload the `gamecon.uf2` that was created in your build directory.
