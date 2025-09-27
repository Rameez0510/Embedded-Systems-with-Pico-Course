#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5

#define SSD1306_ADDR 0x3C

void ssd1306_cmd(uint8_t cmd){
    uint8_t buf[2] = {0x00, cmd};
    i2c_write_blocking(I2C_PORT, SSD1306_ADDR, buf, 2, false);
}

void ssd1306_data(uint8_t *data, size_t len){
    uint8_t buf[len + 1]; // first byte is control byte, rest is data
    buf[0] = 0x40;
    for (size_t i = 0; i < len; i++)
    {
        buf[i+1] = data[i]; // copy data into buf array after control byte
    }
    i2c_write_blocking(I2C_PORT, SSD1306_ADDR, buf, len + 1, false); //sending data to display
    
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

void ssd1306_clear() { // clear display by writing 0 to all pixels
    for (uint8_t page = 0; page < 8; page++) {
        ssd1306_cmd(0xB0 + page); // set page
        ssd1306_cmd(0x00);        // lower col = 0 
        ssd1306_cmd(0x10);        // higher col = 0
        uint8_t zeros[128] = {0};     // one full row of empty pixels
        ssd1306_data(zeros, 128); // write empty row
    }
}
int main(){
    stdio_init_all();
    i2c_init(I2C_PORT, 100*1000);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SCL_PIN);
    gpio_pull_up(SDA_PIN);

    ssd1306_init(); // initialize the display
    ssd1306_clear(); // clear the display

    ssd1306_cmd(0xB7); //page 7 
    ssd1306_cmd(0x00); //lower column 0
    ssd1306_cmd(0x10); //higher column 0 --> column = 0

    //draw horizontal line on first row
    
    uint8_t line[128]; //array to hold data for line
    
    for (uint8_t i = 0; i < 128; i++)
    {
        line[i] = 0x01; // setting only one bit high in page to draw line
    }

    ssd1306_data(line, 128); //sending data to display

    while (true)
    {
        tight_loop_contents();
    }
    
    
}