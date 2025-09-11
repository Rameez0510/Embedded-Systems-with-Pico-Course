#include "pico/stdlib.h"
#include "pico/time.h"


// Callback for repeating timer: toggles the LED assigned via user_data.
bool toggle_led(struct repeating_timer *t){
    
    // user_data was set to the pin number when the timer was created.
    // We cast through uintptr_t to avoid warnings about pointer/integer casts.
    uint pin = (uint)(uintptr_t)t->user_data;
    
    // Static array to remember the on/off state for up to 32 possible pins.
    // Indexed by the GPIO pin number. Using static keeps state across calls.
    static bool led_state[32] = {0};

    // Toggle and write the new level to the GPIO pin
    led_state[pin] = !led_state[pin];
    gpio_put(pin, led_state[pin]);
    
    return true; // keep the timer repeating
}

int main(){
    
    stdio_init_all(); // initialize stdio

    // Pins for two independent blinking LEDs
    const uint pin1 = 15;
    const uint pin2 = 14;

    // Initialize both GPIO pins and mark them as outputs
    gpio_init(pin1);
    gpio_init(pin2);
    gpio_set_dir(pin1, GPIO_OUT);
    gpio_set_dir(pin2, GPIO_OUT);

    // Timer objects for each blinking rate
    struct repeating_timer timer1, timer2;

    // Create repeating timers:
    // - timer1 toggles pin1 every 500 ms
    // - timer2 toggles pin2 every 1000 ms
    // The pin number is passed via user_data by casting it to a void*.
    add_repeating_timer_ms(500, toggle_led, (void *)(uintptr_t)pin1, &timer1);
    add_repeating_timer_ms(1000, toggle_led, (void *)(uintptr_t)pin2, &timer2);

    // Main loop: nothing to do here, timers run on the core's timer system.
    while (1)
    {
        tight_loop_contents();
    }
    
}