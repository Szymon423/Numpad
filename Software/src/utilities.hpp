#pragma once

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

#define ANALOG_INPUT_SIGNALS_NUMBER 20
#define BINARY_INPUT_SIGNALS_NUMBER 10
#define BINARY_OUTPUT_SIGNALS_NUMBER 10

struct ShortAnalogSignal
{
    bool valid;
    float value;
};


struct ShortBinarySignal
{
    bool valid;
    bool value;
};


struct SPI_data
{
    ShortAnalogSignal uc_input_analogs[ANALOG_INPUT_SIGNALS_NUMBER];
    ShortBinarySignal uc_input_binaries[BINARY_INPUT_SIGNALS_NUMBER];
    ShortBinarySignal uc_output_binaries[BINARY_OUTPUT_SIGNALS_NUMBER];
};


extern const uint LED_PIN;  

extern const uint SPI0_RX_PIN;
extern const uint SPI0_CS_PIN;
extern const uint SPI0_SCK_PIN;
extern const uint SPI0_TX_PIN;

extern const uint SEL_A0_PIN;
extern const uint SEL_A1_PIN;
extern const uint SEL_A2_PIN;

extern const uint UART_TX_PIN;
extern const uint UART_RX_PIN;

extern const uint DE_1;
extern const uint nRE_1;


/// @brief fucntion to init all GPIO and their functionality
void init_all();


/// @brief function to properly configure SEL_A0..A2 pins for channel selection
/// @param channel channel 0..7
void select_channel(uint channel);