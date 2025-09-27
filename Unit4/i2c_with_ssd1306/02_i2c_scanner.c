#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>

#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5

int main(){
    stdio_init_all();            // Initialize stdio

    // Initialize I2C at 100kHz
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    printf("Scanning I2C bus...\n");

    while (true)
    {
        for (uint8_t adrr = 1; adrr < 127; adrr++)
        {
            uint8_t dummy = 0x00;
            int result = i2c_write_blocking(I2C_PORT, adrr, &dummy, 1, true);

            if (result >= 0)
            {
                printf("I2C device found at 0x%02X\n", adrr);
            }
            sleep_ms(5);
        }

        sleep_ms(5000);
        
    }
    
}