#include "pico/stdio.h"
#include "pico/time.h"
#include "bsp/board_api.h"
#include <tusb.h>
#include "hid.hpp"
#include "pixels.hpp"

int main() {
	// init board and peripherials
	board_init();

	// init device stack on configured roothub port
  	tud_init(BOARD_TUD_RHPORT);

	while (true) {
		// TinyUSB device task
		tud_task();
		
		// LEDs task
		LED::Pixels::Handler();
		
		// HID task
		hid_task();
	}
};