#include "pico/stdlib.h"

int main(){
    stdio_init_all();            // Initialize standard IO 
    gpio_init(15);               // Initialize GPIO 15 for use
    gpio_set_dir(15, GPIO_OUT);  // Set GPIO 15 as an output

    while (1)
    {
        gpio_put(15, 1);    // Turn LED on 
        sleep_ms(500);      // Shutdown CPU for 500 milliseconds
        gpio_put(15, 0);    // Turn LED off
        sleep_ms(500);      // Shutdown CPU for 500 milliseconds
    }
    
}