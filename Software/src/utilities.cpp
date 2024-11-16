#include "utilities.hpp"


void init_all()
{
    // Inicjalizacja SPI w trybie slave
    spi_init(spi0, 1000 * 1000); // Ustawienie prędkości nie ma znaczenia w trybie slave
    gpio_set_function(SPI0_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI0_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI0_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI0_CS_PIN, GPIO_FUNC_SPI);
    bi_decl(bi_4pins_with_func(SPI0_RX_PIN, SPI0_TX_PIN, SPI0_SCK_PIN, SPI0_CS_PIN, GPIO_FUNC_SPI));
    spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    const uint Ax[3] = { SEL_A0_PIN, SEL_A1_PIN, SEL_A2_PIN };
    for (int i = 0; i < 3; i++)
    {
        gpio_init(Ax[i]);
        gpio_set_dir(Ax[i], GPIO_OUT);
        gpio_pull_up(Ax[i]);
    }

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}


void select_channel(uint channel)
{
    if (channel < 0 || channel > 7) return;

    bool A0 = (channel & 0b00000001) == 1;
    bool A1 = (channel & 0b00000010) == 2;
    bool A2 = (channel & 0b00000100) == 4;
    
    gpio_put(SEL_A0_PIN, A0);
    gpio_put(SEL_A1_PIN, A1);
    gpio_put(SEL_A2_PIN, A2);
}