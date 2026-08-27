set(PROJECT sdcardtest)

cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico2 CACHE STRING "Board type")

set(FREERTOS_KERNEL_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/FreeRTOS-Kernel" CACHE STRING "Path to the FreeRTOS Kernel")

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

#include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)
if ("${PICO_PLATFORM}" STREQUAL "rp2040")
    include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)
elseif("${PICO_PLATFORM}" STREQUAL "rp2350-arm-s")
    include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/Community-Supported-Ports/GCC/RP2350_ARM_NTZ/FreeRTOS_Kernel_import.cmake)
elseif("${PICO_PLATFORM}" STREQUAL "rp2350-riscv")
    include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2350_RISC-V/FreeRTOS_Kernel_import.cmake)
endif()

project(${PROJECT} C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)

pico_sdk_init()

add_subdirectory(
  ${CMAKE_CURRENT_LIST_DIR}/lib/FreeRTOS-FAT-CLI-for-RPi-Pico/src/FreeRTOS+FAT+CLI build
)

add_executable(${PROJECT}
  src/sdcardtest.cpp
  src/cbsd/cbsd.h
  src/cbsd/cbsd.cpp
  src/cbsd/hw_config.c
)

target_link_libraries(${PROJECT}
  pico_stdlib
  hardware_spi
  FreeRTOS-Kernel
  FreeRTOS-Kernel-Heap4
  FreeRTOS+FAT+CLI
)

# FreeRTOSConfig.h needs to be in the include path
target_include_directories(${PROJECT} PUBLIC
  include/
  src/cbsd
)

pico_enable_stdio_usb(${PROJECT} 1)
pico_enable_stdio_uart(${PROJECT} 1)

pico_add_extra_outputs(${PROJECT})
