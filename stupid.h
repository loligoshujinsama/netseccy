#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "pico/stdlib.h"

// Function declarations
void init_gpio();
void init_pwm();
void button_callback(uint gpio, uint32_t events);
void update_led_brightness();

#endif
