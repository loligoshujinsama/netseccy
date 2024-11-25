//hi
#include <stdio.h>
#include "pico/stdlib.h"
#include "functions.h"

int main() {
    stdio_init_all();
    printf("Pico LED Controller Example\n");

    // Initialize system components
    init_gpio();
    init_pwm();
    printf("System initialized.\n");

    while (1) {
        tight_loop_contents(); // Keep running indefinitely
    }

    return 0;
}
