#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Define button and LED GPIO pins
#define button_pin 14
#define led_pin 15

// Volatile because these are accessed/modified in interrupt context
volatile bool led_state = false;    // Current state of the LED (on/off)
volatile uint32_t lastTime = 0;     // Timestamp of last accepted button event (ms since boot)

// Interrupt Service Routine called on button edge events.
// Implements a simple software debounce: ignore events that occur
// within 20 ms of the previous accepted event.
void button_isr(uint gpio, uint32_t events){
    // Get current time in milliseconds since boot
    uint32_t now = to_ms_since_boot(get_absolute_time());
    // If more than 20 ms have passed since last accepted press, accept it
    if (now - lastTime > 20)
    {
        // Toggle stored LED state and update the GPIO output
        led_state = !led_state;
        gpio_put(led_pin, led_state);
        // Record the time this event was accepted for debounce comparison
        lastTime = now;
    }
    
}

int main(){
    stdio_init_all(); // Initialize stdio

    // Configure button pin as input with internal pull-up, so the pin reads HIGH
    // when button is not pressed and goes LOW when pressed (active-low button)
    gpio_init(button_pin);
    gpio_set_dir(button_pin, GPIO_IN);
    gpio_pull_up(button_pin);

    // Configure LED pin as output and ensure it starts off
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
    gpio_put(led_pin, 0);

    // Enable falling-edge interrupt on the button pin (HIGH->LOW transition)
    // and register button_isr as the callback to handle the interrupt.
    gpio_set_irq_enabled_with_callback(
        button_pin,
        GPIO_IRQ_EDGE_FALL,
        true,
        &button_isr
    );

    // Main loop does nothing; the program reacts to interrupts.
    while (true)
    {
        tight_loop_contents();    
    }
    
    
}