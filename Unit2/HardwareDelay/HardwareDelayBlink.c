#include "pico/stdlib.h"
#include "pico/time.h"

#define led_pin 15

// Timer callback: toggles the LED. Called from the SDK timer system
bool toggle_led(struct repeating_timer *t){
    static bool led_state = false;
    led_state = !led_state;
    gpio_put(led_pin, led_state);
    return true; // return true to keep the timer repeating
}

int main(){
    stdio_init_all(); // Initialize stdio

    // Configure LED GPIO as output
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

    // Create and start a repeating timer that calls toggle_led every 500 ms
    struct repeating_timer timer;
    add_repeating_timer_ms(500, toggle_led, NULL, &timer);

    // Main loop does nothing; the repeating timer toggles the LED in the background.
    while (1)
    {
        tight_loop_contents();
    }
    
}