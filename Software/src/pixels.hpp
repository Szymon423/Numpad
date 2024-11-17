#pragma once

#include "pico/stdio.h"
#include "pico/time.h"
#include "bsp/board_api.h"
#include "Adafruit_NeoPixel.hpp"

namespace LED {
    /// @brief Light mode
    enum class Mode {
        Static      // just static color
        Pulse,      // pulsing color
        Rainbow,    // rainbow changing colors
        Random      // 
    };

    /// @brief Wrapper class for pixels handling
    class Pixels {
    private:
        static Adafruit_NeoPixel pixels;
        static Mode mode;
        static bool turnedOn;

    public:
        /// @brief Initializes the NeoPixel LED. Sets up the LED with default brightness and clears any previous states.
        static void Init();
        
        /// @brief Activates the LED. Sets the turnedOn flag to true, allowing LED operations.
        static void TurnOn();
        
        /// @brief Deactivates the LED. Turns off the LED and clears current display.
        static void TurnOff();
        
        /// @brief Cycles through available LED modes. Switches between Static, Pulse, Rainbow, and Random modes in sequence.
        static void ChangeMode();

        /// @brief Main processing function for LED operations. Handles the LED behavior based on current mode and timing
        static void Handler();
    };
}
