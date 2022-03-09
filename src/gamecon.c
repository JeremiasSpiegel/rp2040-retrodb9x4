/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

void hid_task(void);

typedef union joygpio_cfg {
    struct {
        uint8_t up;
        uint8_t down;
        uint8_t left;
        uint8_t right;
        uint8_t fire1;
        uint8_t fire2;
    };
    uint8_t gpio[6];
} joygpio_cfg;

joygpio_cfg cfg[] = {
    { 0, 1, 2, 3, 4, 5 },
    { 5, 6, 7, 8, 9, 10 },
    { 11, 12, 13, 14, 15, 16 },
    { 18, 19, 20, 21, 22, 26 }    
};


struct report
{
    uint8_t buttons;
    uint8_t joy0;
    uint8_t joy1;
};

struct report reports[4];

int main(void)
{
    for (int i = 0; i < CFG_TUD_HID; i++) {
        for (int j = 0; j < 6; j++) {
            int gpio = cfg[i].gpio[j];
            gpio_init(gpio);
            gpio_set_dir(gpio, GPIO_IN);
            gpio_pull_up(gpio);
        }
    }

    board_init();

    tusb_init();

    gpio_set_function(PICO_DEFAULT_LED_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PICO_DEFAULT_LED_PIN);

    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, false);
    // Get some sensible defaults for the slice configuration. By default, the
    // counter is allowed to wrap over its maximum range (0 to 2**16-1)
    pwm_config config = pwm_get_default_config();
    // Set divider, reduces counter clock to sysclock/this value
    pwm_config_set_clkdiv(&config, 1.f);
    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num, &config, true);

    while (1)
    {
        hid_task();
        tud_task(); // tinyusb device task
    }

    return 0;
}

void con_panic(uint16_t errcode)
{
    reports[0].buttons = errcode;
    while (1)
    {
        tud_task(); // tinyusb device task
        // Remote wakeup
        if (tud_suspended())
        {
            // Wake up host if we are in suspend mode
            // and REMOTE_WAKEUP feature is enabled by host
            tud_remote_wakeup();
        }

        if (tud_hid_ready())
        {
            for (int i = 0; i < CFG_TUD_HID; i++) {
                tud_hid_n_report(i, 0x01, &reports[i], sizeof(struct report));
            }
        }
    }
}

void hid_task(void)
{
    // Poll every 1ms
    const uint32_t interval_ms = 1;
    static uint32_t start_ms = 0;

    if (board_millis() - start_ms < interval_ms)
        return; // not enough time
    start_ms += interval_ms;
    
    for (int i = 0; i < CFG_TUD_HID; i++) {
        joygpio_cfg *c = &cfg[i];
        reports[i].buttons = gpio_get(c->fire1) ? 0 : (1 << 0);
        reports[i].buttons |= gpio_get(c->fire2) ? 0 : (1 << 1);
        reports[i].joy0 = !gpio_get(c->left) ? 0 : (!gpio_get(c->right) ? 255 : 128);
        reports[i].joy1 = !gpio_get(c->up) ? 0 : (!gpio_get(c->down) ? 255 : 128);
    }

    // Remote wakeup
    if (tud_suspended())
    {
        // Wake up host if we are in suspend mode
        // and REMOTE_WAKEUP feature is enabled by host
        tud_remote_wakeup();
    }

    if (tud_hid_ready())
    {
        for (int i = 0; i < CFG_TUD_HID; i++) {
            tud_hid_n_report(i, 0x01, &reports[i], sizeof(struct report));
        }
    }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
    (void)remote_wakeup_en;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
    // TODO not Implemented
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
}
