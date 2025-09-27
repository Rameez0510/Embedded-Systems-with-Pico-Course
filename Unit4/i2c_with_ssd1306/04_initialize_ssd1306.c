#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0
#define SSD1306_ADDR 0x3C

// Function to send one command
void ssd1306_cmd(uint8_t cmd) {
    uint8_t buf[2] = {0x00, cmd}; // 0x00 = control byte (command mode)
    i2c_write_blocking(I2C_PORT, SSD1306_ADDR, buf, 2, false);
}

int main() {
    stdio_init_all();
    i2c_init(I2C_PORT, 100 * 1000);  
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);
    gpio_pull_up(5);

    // Initialize the SSD1306
    ssd1306_cmd(0xAE); // Display off
    ssd1306_cmd(0x20); ssd1306_cmd(0x00); // Horizontal addressing mode
    ssd1306_cmd(0x40); // Start line = 0

    ssd1306_cmd(0xA8); ssd1306_cmd(0x3F); // Multiplex ratio = 64
    ssd1306_cmd(0xD3); ssd1306_cmd(0x00); // Display offset
    ssd1306_cmd(0xDA); ssd1306_cmd(0x12); // COM pins hardware config
    ssd1306_cmd(0xD5); ssd1306_cmd(0x80); // Clock divide
    ssd1306_cmd(0xD9); ssd1306_cmd(0xF1); // Pre-charge period
    ssd1306_cmd(0xDB); ssd1306_cmd(0x40); // VCOMH level
    ssd1306_cmd(0x8D); ssd1306_cmd(0x14); // Enable charge pump ⚡

    ssd1306_cmd(0xA1); // Segment remap
    ssd1306_cmd(0xC8); // COM scan direction
    ssd1306_cmd(0x81); ssd1306_cmd(0x7F); // Contrast
    ssd1306_cmd(0xA6); // Normal display
    ssd1306_cmd(0xA4); // Display follows RAM
    ssd1306_cmd(0xAF); // Display on

    while (1) {
        tight_loop_contents(); 
    }
}
