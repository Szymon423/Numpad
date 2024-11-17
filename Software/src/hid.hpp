#pragma once

#include "pico/stdio.h"

/// @brief TinyUSB device callback when mounted
/// @param dev_addr device address
void tud_mount_cb();

/// @brief TinyUSB device callback when unmounted
/// @param dev_addr device address
void tud_umount_cb();

/// @brief Invoked when usb bus is suspended
/// @param remote_wakeup_en 
void tud_suspend_cb(bool remote_wakeup_en);

/// @brief Invoked when usb bus is resumed
void tud_resume_cb();

/// @brief Function to send HID report
/// @param report_id 
static void send_hid_report(uint8_t report_id);

/// @brief HID task
void hid_task();

/// @brief Invoked when sent REPORT successfully to host
/// @param instance 
/// @param report 
/// @param len 
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len);

/// @brief Invoked when received GET_REPORT control request
/// @param instance 
/// @param report_id 
/// @param report_type 
/// @param buffer 
/// @param reqlen 
/// @return 
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen);

/// @brief Invoked when received SET_REPORT control request or
/// @param instance 
/// @param report_id 
/// @param report_type 
/// @param buffer 
/// @param bufsize 
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize);