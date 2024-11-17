#include "pixels.hpp"
#include "definitions.hpp"

namespace LED {

    // Initialize static members
    Adafruit_NeoPixel Pixels::pixels(Constants::PixelNumber, Constants::Pin, NEO_GRB + NEO_KHZ800);
    Mode Pixels::mode = Mode::Static;
    bool Pixels::turnedOn = false;

    void Pixels::Init() {
        pixels.begin();           // Init NeoPixels
        pixels.setBrightness(50); // Set brightness (0-255)
        pixels.show();            // Apply
    }

    void Pixels::TurnOn() {
        turnedOn = true;
    }

    void Pixels::TurnOff() {
        turnedOn = false;
        pixels.clear();
        pixels.show();
    }

    void Pixels::ChangeMode() {
        switch(mode) {
            case Mode::Static:
                mode = Mode::Pulse;
                break;
            case Mode::Pulse:
                mode = Mode::Rainbow;
                break;
            case Mode::Rainbow:
                mode = Mode::Random;
                break;
            case Mode::Random:
                mode = Mode::Static;
                break;
        }
    }

    void Pixels::Handler() {
        if (!turnedOn) {
            return;
        }

        static uint32_t lastUpdate = 0;
        static uint8_t hue = 0;
        static uint8_t brightness = 0;
        static bool increasing = true;
        
        uint32_t currentTime = to_ms_since_boot(get_absolute_time());
        
        // Update every 50ms
        if (currentTime - lastUpdate < 50) {
            return;
        }
        
        lastUpdate = currentTime;

        switch(mode) {
            case Mode::Static:
                pixels.setPixelColor(0, pixels.Color(255, 255, 255)); // Static white
                break;
                
            case Mode::Pulse:
                if (increasing) {
                    brightness += 5;
                    if (brightness >= 255) {
                        increasing = false;
                    }
                } else {
                    brightness -= 5;
                    if (brightness <= 0) {
                        increasing = true;
                    }
                }
                pixels.setBrightness(brightness);
                pixels.setPixelColor(0, pixels.Color(255, 255, 255));
                break;
                
            case Mode::Rainbow:
                hue++;
                // HSV to RGB
                uint32_t color = pixels.ColorHSV(hue * 256);
                pixels.setPixelColor(0, color);
                break;
                
            case Mode::Random:
                if (currentTime % 1000 == 0) { 
                    // Color change each 1s
                    uint8_t r = rand() % 256;
                    uint8_t g = rand() % 256;
                    uint8_t b = rand() % 256;
                    pixels.setPixelColor(0, pixels.Color(r, g, b));
                }
                break;
        }
        
        pixels.show();
    }

} // namespace LED
