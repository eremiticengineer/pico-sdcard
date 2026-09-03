cmake_minimum_required(VERSION 3.13)

set(LIBRARY pico_sdcard)

# ---------------------------------------------------------
# Are we being built directly or as a submodule?
# ---------------------------------------------------------
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(PICO_SDCARD_STANDALONE TRUE)
else()
    set(PICO_SDCARD_STANDALONE FALSE)
endif()

# ---------------------------------------------------------
# Standalone project setup
# ---------------------------------------------------------
if(PICO_SDCARD_STANDALONE)

    set(PROJECT pico_sdcard_test)

    if(DEVICE STREQUAL "seeed_xiao_rp2040")

        set(
            PICO_BOARD
            seeed_xiao_rp2040
            CACHE STRING "Board type"
            FORCE
        )

    elseif(DEVICE STREQUAL "pico")

        set(
            PICO_BOARD
            pico
            CACHE STRING "Board type"
            FORCE
        )

    elseif(DEVICE STREQUAL "pico2")

        set(
            PICO_BOARD
            pico2
            CACHE STRING "Board type"
            FORCE
        )

    else()

        message(
            FATAL_ERROR
            "Unknown DEVICE='${DEVICE}'. Use pico, pico2 or seeed_xiao_rp2040."
        )

    endif()

    include(
        $ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake
    )

    project(${PROJECT} C CXX ASM)

    set(CMAKE_C_STANDARD 11)
    set(CMAKE_CXX_STANDARD 23)

    pico_sdk_init()

    message(STATUS "DEVICE=${DEVICE}")
    message(STATUS "PICO_BOARD=${PICO_BOARD}")
    message(STATUS "PICO_PLATFORM=${PICO_PLATFORM}")

    # ---------------------------------------------------------
    # FreeRTOS dependency
    # ---------------------------------------------------------
    set(
        FREERTOS_KERNEL_PATH
        "${CMAKE_CURRENT_LIST_DIR}/lib/FreeRTOS-Kernel"
    )

    include(
        ${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake
    )

    # ---------------------------------------------------------
    # FreeRTOS+FAT dependency
    # ---------------------------------------------------------
    if(NOT TARGET FreeRTOS+FAT+CLI)

        add_subdirectory(
            ${CMAKE_CURRENT_LIST_DIR}/lib/FreeRTOS-FAT-CLI-for-RPi-Pico/src/FreeRTOS+FAT+CLI
            ${CMAKE_CURRENT_BINARY_DIR}/freertos-fat
        )

    endif()

endif()

# ---------------------------------------------------------
# FreeRTOS+FAT configuration
# ---------------------------------------------------------
#
# FreeRTOS+FAT needs access to FreeRTOSFATConfig.h.
#
# This configuration belongs to pico-sdcard, so expose the
# pico-sdcard include directory to the FreeRTOS+FAT target.
#
if(TARGET FreeRTOS+FAT+CLI)

    target_include_directories(
        FreeRTOS+FAT+CLI
        INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
    )

endif()

# ---------------------------------------------------------
# pico-sdcard library
# ---------------------------------------------------------
add_library(
    ${LIBRARY}
    src/sdcard.cpp
    src/hw_config.c
)

target_include_directories(
    ${LIBRARY}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(
    ${LIBRARY}
    PUBLIC
    pico_stdlib
    hardware_spi
    FreeRTOS-Kernel
    FreeRTOS+FAT+CLI
)

# ---------------------------------------------------------
# Standalone example
# ---------------------------------------------------------
if(PICO_SDCARD_STANDALONE)

    add_executable(
        ${PROJECT}
        examples/sdcardtest.cpp
    )

    target_compile_definitions(
        ${PROJECT}
        PRIVATE
        DEVICE_${DEVICE}
    )

    target_link_libraries(
        ${PROJECT}
        PRIVATE
        FreeRTOS-Kernel-Heap4
        ${LIBRARY}
    )

    pico_enable_stdio_usb(${PROJECT} 1)
    pico_enable_stdio_uart(${PROJECT} 1)

    pico_add_extra_outputs(${PROJECT})

endif()