#include "pico/stdlib.h"
#include "hardware/i2c.h"
   
#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5
   
#define SSD1306_ADDR 0x3C
   
uint8_t screen[8][128]; //global variable

void ssd1306_cmd(uint8_t cmd){
    uint8_t buf[2] = {0x00, cmd};
    i2c_write_blocking(I2C_PORT, SSD1306_ADDR, buf, 2, false);
}

void ssd1306_data(uint8_t *data, size_t len){
    uint8_t buf[len + 1];
    buf[0] = 0x40;
    for (size_t i = 0; i < len; i++)
    {
        buf[i+1] = data[i];
    }
    i2c_write_blocking(I2C_PORT, SSD1306_ADDR, buf, len + 1, false);
    
}

void ssd1306_init() {


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

}

void ssd1306_clear_buffer() {
    for (uint8_t page = 0; page < 8; page++) {
        for (uint8_t x = 0; x < 128; x++)
        {
            screen[page][x] = 0x00;
        }
        
    }
}


void ssd1306_set_pixel(uint8_t x, uint8_t y){
    screen[y/8][x] = screen[y/8][x] | (1 << (y%8));
}

void ssd1306_clear_pixel(uint8_t x, uint8_t y){
    screen[y/8][x] = screen[y/8][x] & ~(1 << (y%8));
}

void ssd1306_update(){
    for (uint8_t page = 0; page < 8; page++)
    {
        ssd1306_cmd(0xB0 + page); //page
        ssd1306_cmd(0x00); //lower column
        ssd1306_cmd(0x10); //higher column

        ssd1306_data(screen[page], 128);
    }
    
}

int main(){
    stdio_init_all();
    i2c_init(I2C_PORT, 100*1000);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SCL_PIN);
    gpio_pull_up(SDA_PIN);

    ssd1306_init();
    ssd1306_clear_buffer();

    //drawing a diagonal line
    for (uint8_t i = 0; i < 64; i++)
    {
        ssd1306_set_pixel(i, i);
    }

    ssd1306_update();
    

    while (true)
    {
        tight_loop_contents();
    }
    
    
}