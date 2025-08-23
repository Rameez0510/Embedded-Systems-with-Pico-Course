#include "pico/stdlib.h"

int main(){
    stdio_init_all();
    gpio_init(14);                 // Initialize button pin
    gpio_init(15);                 // Initialize LED pin
    gpio_set_dir(14, GPIO_IN);     // Configure button as input
    gpio_pull_up(14);              // Enable internal pull-up
    gpio_set_dir(15, GPIO_OUT);    // Configure LED as output
    gpio_put(15, 0);               // Initialize LED off
    
    while (1) {
        int value = gpio_get(14);   // Read button state
        
        if (value == 0) {           // Button pressed (active low)
            printf("Button Pressed!\n");
            gpio_put(15, 1);        // Turn LED on
        } else {
            printf("Button not Pressed\n");
            gpio_put(15, 0);        // Turn LED off
        }
        
        sleep_ms(200);              // Debounce delay
    }
}