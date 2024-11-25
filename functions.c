#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "functions.h"

#define NUM_LEDS 6
#define START_PIN 10
#define BUTTON_INCREASE 20
#define BUTTON_DECREASE 22
#define BUTTON_NEXT 21

static int speed = 1000; // Initial speed in ms
static int current_led = 0; // Current LED being adjusted
static uint brightness[NUM_LEDS] = {0, 0, 0, 0, 0, 0}; // Brightness levels (0 = off, 25 = dim, 255 = full)

// Initialize GPIO and PWM
void init_gpio() {
    // Initialize LEDs
    for (int i = 0; i < NUM_LEDS; i++) {
        gpio_set_function(START_PIN + i, GPIO_FUNC_PWM); // Set GPIO to PWM
    }

    // Initialize buttons
    gpio_set_irq_enabled_with_callback(BUTTON_INCREASE, GPIO_IRQ_EDGE_FALL, true, button_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_DECREASE, GPIO_IRQ_EDGE_FALL, true, button_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_NEXT, GPIO_IRQ_EDGE_FALL, true, button_callback);
}

// Initialize PWM for LEDs
void init_pwm() {
    for (int i = 0; i < NUM_LEDS; i++) {
        uint slice_num = pwm_gpio_to_slice_num(START_PIN + i);
        pwm_set_wrap(slice_num, 255); // 8-bit resolution
        pwm_set_chan_level(slice_num, pwm_gpio_to_channel(START_PIN + i), 0); // Start with off
        pwm_set_enabled(slice_num, true);
    }
}

// Button interrupt callback
void button_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_INCREASE) {
        speed = speed > 100 ? speed - 100 : 100; // Reduce delay to increase speed
        printf("Speed increased: %d ms\n", speed);
    } else if (gpio == BUTTON_DECREASE) {
        speed += 100; // Increase delay to decrease speed
        printf("Speed decreased: %d ms\n", speed);
    } else if (gpio == BUTTON_NEXT) {
        update_led_brightness();
    }
}

// Update the brightness of the current LED
void update_led_brightness() {
    if (brightness[current_led] == 0) {
        brightness[current_led] = 25; // Dim brightness
    } else if (brightness[current_led] == 25) {
        brightness[current_led] = 255; // Full brightness
    } else {
        // If all LEDs are full brightness, reset
        int all_full = 1;
        for (int i = 0; i < NUM_LEDS; i++) {
            if (brightness[i] != 255) {
                all_full = 0;
                break;
            }
        }
        if (all_full) {
            // Reset all LEDs
            for (int i = 0; i < NUM_LEDS; i++) {
                brightness[i] = 0;
                uint slice_num = pwm_gpio_to_slice_num(START_PIN + i);
                pwm_set_chan_level(slice_num, pwm_gpio_to_channel(START_PIN + i), 0);
            }
            current_led = 0;
            printf("All LEDs reset.\n");
            return;
        }
        // Move to the next LED
        current_led = (current_led + 1) % NUM_LEDS;
    }

    // Update the current LED's PWM brightness
    uint slice_num = pwm_gpio_to_slice_num(START_PIN + current_led);
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(START_PIN + current_led), brightness[current_led]);
    printf("LED %d brightness set to %d\n", START_PIN + current_led, brightness[current_led]);
}
