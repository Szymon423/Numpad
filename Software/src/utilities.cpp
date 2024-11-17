#include "utilities.hpp"
#include "pixels.hpp"

void board_init() {
    stdio_init_all();
    LED::Pixels::Init();
    for (int i = 2; i <= 21; i++) {
        gpio_init(i);
        gpio_set_dir(i, GPIO_IN);
    }
}