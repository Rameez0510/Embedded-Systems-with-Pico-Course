#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5
#define LED_PIN 15

int main(){
   stdio_init_all();            // Initialize stdio 
   gpio_init(LED_PIN);               // Prepare GPIO 15
   gpio_set_dir(LED_PIN, GPIO_OUT);  // Configure GPIO 15 as output for LED

   // Initialize I2C at 100kHz
   i2c_init(I2C_PORT, 100 * 1000);
   // Set up GPIO pins 4 and 5 for I2C functionality
   gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
   gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
   // Enable pull-up resistors on the I2C pins
   gpio_pull_up(SDA_PIN);
   gpio_pull_up(SCL_PIN);

   // Blink LED just to see code is alive.
   while (1)
   {
       gpio_put(LED_PIN, 0);            // Turn LED off 
       sleep_ms(500);              // Sleep for 500 ms
       gpio_put(LED_PIN, 1);            // Turn LED on
       sleep_ms(500);              // Sleep for 500 ms
   }

}