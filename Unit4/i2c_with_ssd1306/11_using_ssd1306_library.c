#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h" // Including the standard ssd1306 library

#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5
   
#define SSD1306_ADDR 0x3C

int main(){

    stdio_init_all();
    i2c_init(I2C_PORT, 400*1000);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SCL_PIN);
    gpio_pull_up(SDA_PIN);

    ssd1306_t display; // Creating an instance of the ssd1306 structure
    display.external_vcc=false; // We are not using external vcc
    ssd1306_init(&display, 128, 64, SSD1306_ADDR, I2C_PORT); // Initializing the display
    ssd1306_clear(&display);// Clearing the display

    ssd1306_draw_empty_square(&display, 20, 20, 40, 20); // Drawing an unfilled rectangle
    char s[] = "Hello World"; // Text to display

    // Displaying animation of the text
    for (uint8_t i = 0; i < (uint8_t)sizeof(s); i++)
    {
        ssd1306_draw_char(&display, i*5, 0, 1, s[i]); // Drawing each character
        ssd1306_show(&display); // Updating the display
        sleep_ms(200); // waiting for 200 ms
    }
    



    while (true)
    {
        // Animation of an underscore moving after the text
        ssd1306_draw_char(&display, 12*5, 0, 1, '_');
        ssd1306_show(&display);
        sleep_ms(200);
        ssd1306_clear_square(&display, 12*5, 0, 5, 8); // Clearing the underscore
        ssd1306_show(&display);
        sleep_ms(200);
    }
    
}