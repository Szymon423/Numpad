// Copyright (c) 2021 Michael Stoops. All rights reserved.
// Portions copyright (c) 2021 Raspberry Pi (Trading) Ltd.
// 
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
// following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
//    disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//    following disclaimer in the documentation and/or other materials provided with the distribution.
// 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
//    products derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// SPDX-License-Identifier: BSD-3-Clause
//
// Example of an SPI bus slave using the PL022 SPI interface

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"
#include "utilities.hpp"

#define BUF_LEN                      0x100
#define spi_default                  spi0
#define PICO_DEFAULT_SPI_SCK_PIN     2
#define PICO_DEFAULT_SPI_TX_PIN      3
#define PICO_DEFAULT_SPI_RX_PIN      0
#define PICO_DEFAULT_SPI_CSN_PIN     1

void printbuf(uint8_t buf[], size_t len) {
    int i;
    for (i = 0; i < len; ++i) {
        if (i % 16 == 15)
            printf("%02x\n", buf[i]);
        else
            printf("%02x ", buf[i]);
    }

    // append trailing newline if there isn't one
    if (i % 16) {
        putchar('\n');
    }
}


int main() {
    // Enable UART so we can print
    stdio_init_all();
#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(PICO_DEFAULT_SPI_CSN_PIN)
#warning spi/spi_slave example requires a board with SPI pins
    puts("Default SPI pins were not defined");
#else

    printf("SPI slave example\n");

    // Enable SPI 0 at 1 MHz and connect to GPIOs
    spi_init(spi_default, 1000 * 1000);
    spi_set_slave(spi_default, true);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);
    // Make the SPI pins available to picotool
    bi_decl(bi_4pins_with_func(PICO_DEFAULT_SPI_RX_PIN, PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI));

    const size_t struct_size = sizeof(SPI_data);
    uint8_t out_buf[struct_size], in_buf[struct_size];
    SPI_data tx_SPI_data;
    SPI_data rx_SPI_data;

    for (int i = 0; i < ANALOG_INPUT_SIGNALS_NUMBER; i++)
    {
        tx_SPI_data.uc_input_analogs[i].value = (float)i;
        tx_SPI_data.uc_input_analogs[i].valid = true;
    }
    
    for (int i = 0; i < BINARY_INPUT_SIGNALS_NUMBER; i++)
    {
        tx_SPI_data.uc_input_binaries[i].value = i % 2;
        tx_SPI_data.uc_input_binaries[i].valid = true;
    }

    memcpy(out_buf, &tx_SPI_data, struct_size);
    sleep_ms(5000);

    for (size_t i = 0; ; ++i) {
        // Write the output buffer to MISO, and at the same time read from MOSI.
        spi_write_read_blocking(spi_default, out_buf, in_buf, struct_size);

        printf("SPI slave\n", i);
        // printbuf(in_buf, struct_size);
        
        memcpy(&rx_SPI_data, in_buf, struct_size);

        printf("Recieved rx_buffer\n");
        printf("BINARIES\n");
        for (int i = 0; i < BINARY_OUTPUT_SIGNALS_NUMBER; i++)
        {
            printf("uc_output_binaries[%d] = %s | %s \n", i, rx_SPI_data.uc_output_binaries[i].value ? "true" : "false", rx_SPI_data.uc_output_binaries[i].valid ? "true" : "false");
        }

        printf("Send tx_buffer\n");
        printf("ANALOGS\n");
        for (int i = 0; i < ANALOG_INPUT_SIGNALS_NUMBER; i++)
        {
            printf("uc_input_analogs[%d] = %.2f | %s \n", i, tx_SPI_data.uc_input_analogs[i].value, tx_SPI_data.uc_input_analogs[i].valid ? "true" : "false");
        }

        printf("BINARIES\n");
        for (int i = 0; i < BINARY_INPUT_SIGNALS_NUMBER; i++)
        {
            printf("uc_input_binaries[%d] = %s | %s \n", i, tx_SPI_data.uc_input_binaries[i].value ? "true" : "false", tx_SPI_data.uc_input_binaries[i].valid) ? "true" : "false";
        }
        printf("\n");
    }
#endif
}