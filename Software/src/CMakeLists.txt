file(GLOB SOURCES
     "*.cpp"
     "*.hpp"
     "*.h"
)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

add_library(test_lib ${SOURCES})

target_link_libraries(test_lib 
    pico_stdlib
    hardware_spi
)

target_include_directories(test_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})