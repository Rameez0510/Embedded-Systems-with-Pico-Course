#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Define GPIO pin numbers for button and LED
#define button_pin 14
#define led_pin 15

// Volatile variable to store LED state, modified in ISR
volatile bool led_state = false;

// Interrupt Service Routine for button press
// Toggles the LED state each time the button is pressed
void button_isr(uint gpio, uint32_t events){
    led_state = !led_state;           // Toggle LED state
    gpio_put(led_pin, led_state);     // Update LED output
}

int main(){
    stdio_init_all(); // Initialize all standard IO

    // Initialize button GPIO as input with pull-up resistor
    gpio_init(button_pin);
    gpio_set_dir(button_pin, GPIO_IN);
    gpio_pull_up(button_pin);

    // Initialize LED GPIO as output and turn it off
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
    gpio_put(led_pin, 0);

    // Enable falling edge interrupt on button pin and set callback
    gpio_set_irq_enabled_with_callback(
        button_pin,
        GPIO_IRQ_EDGE_FALL, // Trigger on falling edge (button press)
        true,
        &button_isr         // ISR to call on interrupt
    );

    // Main loop does nothing, waits for interrupts
    while (true)
    {
        tight_loop_contents();    
    }
    
    
}