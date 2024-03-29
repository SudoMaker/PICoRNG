# PICoRNG

## Introduction

The PICoRNG is a USB random number generator making use of the well-known diode avalanche effect, affordable, with good software support.

With this device, you no longer need to depend on these black boxes in your CPU to provide you random data.

## Features
- Tiny in size
- Low power consumption
- Full featured software
- Built-in random quality check
- Unique anti-tamper measures
- Board schematics and all software are all open sourced

**You can read more about how it works & buy it in our [Official Shop](https://shop.sudomaker.com/products/picorng-random-number-generator).**

## Hardware
The schematic and gerber files are in the `Hardware` directory.

## Software

### Device firmware
The source code files are in the `Software/PICoRNG.X` directory.

You'll need to install the MPLAB X IDE and XC8 compiler to compile it. To flash it to the device, you need a PICKit.

### PC Utility
The source code files are in the `Software/PICoRNG_Utility` directory.

#### Build requirements
- CMake 3.14+
- GCC/Clang with C++17 support
- POSIX compliant operating system
- Working Internet connection

#### Usage
```
PICoRNG Utility
Usage:
  PICoRNG [OPTION...] COMMAND...

 Main options:
  -h, --help               Show this help
  -l, --list-devices       List all devices
  -n, --device-number arg  Specify device number (default: 0)
  -c, --config-dir arg     Specify configuration directory (default: 
                           ~/.picorng/)

 Commands:
  info                      Show device info
  pair                      Pair device
  verify                    Verify device
  cat [block count]         Read random data into stdout
  quality [block count]     Check random data quality
  rngd [noverify]           Feed random data to system
 ```

## Licensing

### Hardware
All hardware design files are CERN-OHL-S v2 licensed.

### Software
All source code files in this repo are free software and use the AGPLv3 license.

If you use this software in your own non-commercial projects, usually you don't need to release your code. See [this FAQ](https://www.gnu.org/licenses/gpl-faq.html#GPLRequireSourcePostedPublic).

If you see a possible license violation, don't hesitate to tell us.

#### Warning for GitHub Copilot (or any "Coding AI") users

"Fair use" is only valid in some countries, such as the United States.

This program is protected by copyright law and international treaties.

Unauthorized reproduction or distribution of this program (e.g. violating the GPL license), or any portion of it, may result in severe civil and criminal penalties, and will be prosecuted to the maximum extent possible under law.
