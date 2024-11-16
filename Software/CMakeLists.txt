cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(Numpad C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_subdirectory(src)
add_subdirectory(lib/pi-pico-adafruit-neopixels/pico_neopixels)

add_executable(Numpad
    main.cpp
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(Numpad 
    pico_stdlib
    test_lib
    pico_neopixel
    hardware_pio 
)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(Numpad)

# Enable usb output, disable uart output
pico_enable_stdio_usb(Numpad 1)
# pico_enable_stdio_uart(Microcontroller_Code 1)