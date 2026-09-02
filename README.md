# SDCard Test

This is a simple project to write to an sdcard from a pico. It shows how to include FreeRTOS and FreeRTOS-FAT-CLI-for-RPi-Pico into a new project and how to use the pico-examples FreeRTOS customisation.

## Cloning the project

Clone the project with FreeRTOS and FreeRTOS-FAT-CLI-for-RPi-Pico submodules:

```
git clone --recurse-submodules https://github.com/eremiticengineer/pico-sdcard
```

If you cloned without recursing submodules:

```
git submodule update --init --recursive
```

## FreeRTOS-Kernal setup for new projects

When creating a FreeRTOS and FreeRTOS-FAT-CLI-for-RPi-Pico project from scratch, clone the main branches into the project:

```
git init
git submodule add https://github.com/FreeRTOS/FreeRTOS-Kernel.git lib/FreeRTOS-Kernel
git submodule update --init --recursive
git add .gitmodules lib/FreeRTOS-Kernel
git submodule add https://github.com/carlk3/FreeRTOS-FAT-CLI-for-RPi-Pico lib/FreeRTOS-FAT-CLI-for-RPi-Pico
git submodule update --init --recursive
git add .gitmodules lib/FreeRTOS-FAT-CLI-for-RPi-Pico
```

## FreeRTOSConfig.h

This file customises FreeRTOS for your project. The file:

```
include/FreeRTOSConfig.h
```

is this one from the pico-examples:

```
pico-examples/freertos/FreeRTOSConfig_examples_common.h
```

## References

undefined reference to `sd_get_num'

[Writing data to SD card using the no-OS-FatFS-SD-SPI-RPi-Pico library](https://forums.raspberrypi.com/viewtopic.php?t=342315&sid=effaefd26682877749f8977bb96a1186)
