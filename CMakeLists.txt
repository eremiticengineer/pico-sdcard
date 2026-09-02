cmake_minimum_required(VERSION 3.13)

set(LIBRARY pico_sdcard)
set(PROJECT pico_sdcard_test)

# ---------------------------------------------------------
# Standalone board selection
# ---------------------------------------------------------

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)

    if(DEVICE STREQUAL "seeed_xiao_rp2040")
        set(PICO_BOARD pico CACHE STRING "Board type" FORCE)

    elseif(DEVICE STREQUAL "pico2")
        set(PICO_BOARD pico2 CACHE STRING "Board type" FORCE)

    else()
        message(FATAL_ERROR
            "Unknown DEVICE='${DEVICE}'. Use pico2 or seeed_xiao_rp2040."
        )
    endif()

endif()

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(${PROJECT} C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)

pico_sdk_init()

# ---------------------------------------------------------
# Standalone FreeRTOS configuration
# ---------------------------------------------------------

if(PROJECT_IS_TOP_LEVEL)

    set(FREERTOS_CONFIG_FILE_DIRECTORY
        ${CMAKE_CURRENT_LIST_DIR}/include
    )

    set(
        FREERTOS_KERNEL_PATH
        "${CMAKE_CURRENT_LIST_DIR}/lib/FreeRTOS-Kernel"
        CACHE STRING
        "Path to the FreeRTOS Kernel"
    )

    include(
        ${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake
    )

endif()

# ---------------------------------------------------------
# FreeRTOS+FAT dependency
# ---------------------------------------------------------

if(NOT TARGET FreeRTOS+FAT+CLI)

    if(PROJECT_IS_TOP_LEVEL)

        add_subdirectory(
            ${CMAKE_CURRENT_LIST_DIR}/lib/FreeRTOS-FAT-CLI-for-RPi-Pico/src/FreeRTOS+FAT+CLI
            ${CMAKE_CURRENT_BINARY_DIR}/freertos-fat
        )

    else()

        message(FATAL_ERROR
            "pico_sdcard requires the FreeRTOS+FAT+CLI target. "
            "The parent project must add FreeRTOS+FAT before pico_sdcard."
        )

    endif()

endif()

# FreeRTOS+FAT needs access to FreeRTOSFATConfig.h.
#
# In a standalone build this comes from pico-sdcard/include.
# In the weather station this also remains the SD-card-specific
# FAT configuration.
if(TARGET FreeRTOS+FAT+CLI)

    target_include_directories(
        FreeRTOS+FAT+CLI
        INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
    )

endif()

# ---------------------------------------------------------
# Library
# ---------------------------------------------------------

add_library(${LIBRARY}
    src/sdcard.cpp
    src/hw_config.c
)

target_include_directories(${LIBRARY} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src
)

target_link_libraries(${LIBRARY} PUBLIC
    pico_stdlib
    hardware_spi
    FreeRTOS-Kernel
    FreeRTOS+FAT+CLI
)

# ---------------------------------------------------------
# Standalone test
# ---------------------------------------------------------

if(PROJECT_IS_TOP_LEVEL)

    message(STATUS "DEVICE=${DEVICE}")
    message(STATUS "PICO_BOARD=${PICO_BOARD}")
    message(STATUS "PICO_PLATFORM=${PICO_PLATFORM}")

    add_executable(${PROJECT}
        examples/sdcardtest.cpp
    )

    target_include_directories(${PROJECT} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
    )

    target_compile_definitions(${PROJECT} PRIVATE
        DEVICE_${DEVICE}
    )

    target_link_libraries(${PROJECT}
        FreeRTOS-Kernel-Heap4
        ${LIBRARY}
    )

    pico_enable_stdio_usb(${PROJECT} 1)
    pico_enable_stdio_uart(${PROJECT} 1)

    pico_add_extra_outputs(${PROJECT})

endif()
