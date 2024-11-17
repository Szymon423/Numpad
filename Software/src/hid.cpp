#include "hid.hpp"
#include "pixels.hpp"
#include "definitions.hpp"

void tud_mount_cb(void) {
    LED::Pixels::TurnOn();
}

void tud_umount_cb(void) {
    LED::Pixels::TurnOff();
}

void tud_suspend_cb(bool remote_wakeup_en) {
    LED::Pixels::TurnOn();
}

void tud_resume_cb() {
    LED::Pixels::TurnOff();
}

/// @brief Structure for holding buttons state
struct ButtonState {
    bool current;
    bool previous;
    uint8_t keycode;
};

/// @brief Array of buttons
ButtonState buttons[19] = {
    {false, false, HID_KEY_KEYPAD_SUBTRACT},    // -
    {false, false, HID_KEY_KEYPAD_ADD},         // +
    {false, false, HID_KEY_KEYPAD_ENTER},       // Enter
    {false, false, HID_KEY_KEYPAD_DECIMAL},     // ,
    {false, false, HID_KEY_KEYPAD_3},           // 3
    {false, false, HID_KEY_KEYPAD_6},           // 6
    {false, false, HID_KEY_KEYPAD_9},           // 9
    {false, false, HID_KEY_KEYPAD_MULTIPLY},    // *
    {false, false, HID_KEY_KEYPAD_DIVIDE},      // /
    {false, false, HID_KEY_KEYPAD_8},           // 8
    {false, false, HID_KEY_KEYPAD_5},           // 5
    {false, false, HID_KEY_KEYPAD_2},           // 2
    {false, false, HID_KEY_KEYPAD_0},           // 0
    {false, false, HID_KEY_KEYPAD_1},           // 1
    {false, false, HID_KEY_KEYPAD_4},           // 4
    {false, false, HID_KEY_KEYPAD_7},           // 7
    {false, false, HID_KEY_KEYPAD_ENTER},       // LED - need to be omitted
    {false, false, HID_KEY_MUTE}                // Mic
};

void send_hid_report(uint8_t report_id) {
    // skip if hid is not ready yet
    if ( !tud_hid_ready() ) return;

    // use to avoid send multiple consecutive zero report for keyboard
    static bool has_keyboard_key = false;
    uint8_t keycode[6] = { 0 };
    int keycount = 0;

    // check each button state
    for(int i = 0; i < Constants::ButtonNumber; i++) {
        if (i == 18) continue; // do not check for LED button
        buttons[i].current = !gpio_get(i + 2); // GPIO są aktywne nisko (pull-up)
        
        if(buttons[i].current && keycount < 6) {
            keycode[keycount++] = buttons[i].keycode;
        }
        
        buttons[i].previous = buttons[i].current;
    }

    if(keycount > 0) {
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
        has_keyboard_key = true;
    }
    else if(has_keyboard_key) {
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        has_keyboard_key = false;
    }
}

void hid_task(void) {
    const uint32_t interval_ms = 10;
    static uint32_t start_ms = 0;

    if (board_millis() - start_ms < interval_ms) return;
    start_ms += interval_ms;

    bool any_button_pressed = false;
    for(int i = 2; i <= 20; i++) {
        if(!gpio_get(i)) { 
            // Check if any button is pressed
            any_button_pressed = true;
            break;
        }
    }

    if (tud_suspended() && any_button_pressed) {
        tud_remote_wakeup();
    }
    else {
        send_hid_report(REPORT_ID_KEYBOARD);
    }
}

void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len) {
    (void) instance;
    (void) len;
    uint8_t next_report_id = report[0] + 1u;

    if (next_report_id < REPORT_ID_COUNT) {
        send_hid_report(next_report_id);
    }
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
    // TODO not Implemented
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
    (void) instance;

    if (report_type == HID_REPORT_TYPE_OUTPUT)  {
        // Set keyboard LED e.g Capslock, Numlock etc...
        if (report_id == REPORT_ID_KEYBOARD) {
            // bufsize should be (at least) 1
            if ( bufsize < 1 ) return;

            uint8_t const kbd_leds = buffer[0];

            if (kbd_leds & KEYBOARD_LED_CAPSLOCK) {
                // Capslock On: disable blink, turn led on
                blink_interval_ms = 0;
                board_led_write(true);
            }
            else {
                // Caplocks Off: back to normal blink
                board_led_write(false);
                blink_interval_ms = BLINK_MOUNTED;
            }
        }
    }
}