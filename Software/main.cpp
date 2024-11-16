#include "pico/stdio.h"
#include "pico/time.h"
#include <tusb.h>
#include "Adafruit_NeoPixel.hpp"
#include "definitions.hpp"

#define DELAYVAL 200

void example() {
	Adafruit_NeoPixel pixels(Constants::PixelNumber, Constants::PixelPin, NEO_GRB + NEO_KHZ800);
	pixels.begin();

	for (int l; l < 4; l++) {
		pixels.clear();
		for (int i = 0; i < NUMPIXELS; i++) { 
			pixels.setPixelColor(i, pixels.Color((i % 3) * 150, ((i + 1) % 3) * 150, ((i + 2) % 3) * 150));
			pixels.show(); 
			sleep_ms(DELAYVAL);
		}
	}
};

int main() {
	stdio_init_all();
	while (true) {
		example();
	}
};