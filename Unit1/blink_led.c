#include "pico/stdlib.h"

int main(){
    stdio_init_all();              // Initialize standard I/O
    gpio_init(15);                 // Initialize GPIO pin 15
    gpio_set_dir(15, GPIO_OUT);    // Configure as output
    
    while (1) {
        gpio_put(15, 1);           // Set pin HIGH (LED on)
        sleep_ms(500);             // 500ms delay
        gpio_put(15, 0);           // Set pin LOW (LED off)
        sleep_ms(500);             // 500ms delay
    }
}