#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5

#define SSD1306_ADDR 0x3C //I2C address for the display

int main(){
    stdio_init_all();

    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SCL_PIN);
    gpio_pull_up(SDA_PIN);

    sleep_ms(2000);

    //Turn display off
    uint8_t off_cmd[2] = {0x00, 0xAE}; // 0x00 indicates next byte is a command, 0xAE is the command to turn off display
    i2c_write_blocking(I2C_PORT, SSD1306_ADDR, off_cmd, 2, false); //sending command to display

    sleep_ms(1000);

    //Turn display on
    uint8_t on_cmd[2] = {0x00, 0xAF}; //0xAF is the command to turn on display
    i2c_write_blocking(I2C_PORT, SSD1306_ADDR, on_cmd, 2, false); //sending command to display

    while (true)
    {
        tight_loop_contents();
    }
    
}