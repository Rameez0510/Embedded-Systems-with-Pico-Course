# Unit 4: Displays & Communication with Peripherals (I²C)

Structure of this Unit:
   - **Key Concepts**
      - 
   - **Experiments & Projects**
      -  

## Key Concepts

1. **I²C (Inter-Integrated Circuit):**
   - I²C (Inter-Integrated Circuit) is a two-wire conversation. It's like a teacher (master) calling on students (slaves) one at a time to answer questions.
      - The teacher controls the timing (SCL clock).
      - The students listen and reply only when called (via their unique addresses).
2. **How with the help of only two wires (SDA + SCL) multiple devices can share the same bus without any chaos?**

   - On I²C, all devices share **SDA (data)** and **SCL (clock)**.
   - But only the **master** drives the clock (SCL).
   - When the master wants to talk to a device, it sends an **address** byte first.
   - Every device on the bus listens to that address:
      - If the address matches, that device responds.
      - If not, they stay quiet.

3. **Transaction flow:**

   - START condition (SDA ↓ while SCL ↑).
   - Master sends [7-bit address + R/W bit].
   - Slave responds with ACK (0) or NACK (1).
   - Data transfer begins, 1 byte at a time.
   - Receiver ACKs each byte to continue, or NACKs the last byte to stop.
   - Master issues STOP (SDA ↑ while SCL ↑).

**7-bit address + 1 R/W bit (total 8 bits).**
   - `R/W = 0` → Write (Master → Slave).
   - `R/W = 1` → Read (Slave → Master).
      - Example: `0x3C (0111100)` + Write → `01111000`.

**ACK/NACK meaning:**
   - After address: 
      - `ACK` = device exists, 
      - `NACK` = no device found.
   - After data: 
      - `ACK` = “keep going,” 
      - `NACK` = “done, stop sending.”

**Repeated START:**
   - Master can skip STOP and issue a START again to switch device or direction (efficient + keeps bus reserved).

**Error handling:**
   - If no slave ACKs, master retries another address or terminates with “device not found.”

4. **Example: Master wants to read 2 bytes from Slave at address `0x42`**
   - The slave address is `0x42` (7-bit → 1000010).
   - When Master wants to write: it sends `1000010 + 0` → `10000100` (0x84).
   - When Master wants to read: it sends `1000010 + 1` → `10000101` (0x85).

So, even though the base address is `0x42`, the final transmitted byte depends on R/W, here `0x85`.

Now, here's the sequence operations 👇
   - **START** (SDA ↓ while SCL ↑).
   - Master sends `0x85` (address + R/W=1).
   - Slave pulls SDA low → ACK.
   - Slave puts first data byte on SDA.
   - Master ACKs (pulls SDA low at 9th clock).
   - Slave puts second data byte.
   - Master sends **NACK this time** (to say “I’m done”).
   - Master sends **STOP** (SDA ↑ while SCL ↑).

5. **Why pull-up resistors are used on SDA and SCL lines, instead of letting devices drive them high and low directly?**
   - SDA/SCL lines are open-drain; external pull-up resistors keep them HIGH by default. Any device can pull them LOW without conflicts.
   - **Mixed-voltage compatibility:** Using pull-ups (e.g., 3.3 V) ensures both 5 V and 3.3 V devices can share the bus safely.
## Experiments & Projects

#### Wiring Setup:
- Pico GP4 (SDA) → SSD1306 SDA
- Pico GP5 (SCL) → SSD1306 SCL
- Pico 3.3 V → SSD1306 VCC
- Pico GND → SSD1306 GND
- <img width="480" height="251" alt="Screenshot from 2025-10-02 17-51-54" src="https://github.com/user-attachments/assets/6e1994a5-6096-4725-9b68-d3e5b2de7718" />


1. **Initial Setup:**

   - The [01_initial_set_up.c](i2c_with_ssd1306/01_initial_set_up.c) file demonstarte how we can initialize the I2C Communication 
   - Here we are going to import a new library `hardware/i2c.h`, which is helps in using I2C functions.
      ```c
      #include "hardware/i2c.h"
      ```
   - We are going to use the `i2c_init()` function to initialize the I2C communication at 100kHz frequency.
      ```c
      i2c_init(I2C_PORT, 100 * 1000);
      ```
   - We also need to set the GPIO pins to I2C mode using the `gpio_set_function()` function.
      ```c
      gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
      gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
      ```
   - We also need to enable the internal pull-up resistors on the SDA and SCL pins.
      ```c
      gpio_pull_up(SDA_PIN);
      gpio_pull_up(SCL_PIN);
      ```
   - This we had done our initial setup for I2C communication. To verify the code is working on Pico we just add an LED blinking block to check the code is running fine.

   - Now as we are now using a new library `hardware/i2c.h`, we need to mention this library to our `CMakeLists.txt` file.
      ```cmake
      target_link_libraries(your_project_name pico_stdlib hardware_i2c) # here after pico_stdlib add hardware_i2c
      ```

2. **Scanning I2C Devices:**
   - The [02_i2c_scanner.c](i2c_with_ssd1306/02_i2c_scanner.c) file demonstarte how we can scan the I2C bus to find the connected devices.
   - We are going to use the `i2c_write_blocking()` function to send a write request to each address from 1 (`0x01`) to 127 (`0x7F`).
      ```c
      uint8_t dummy = 0x00;
      int result = i2c_write_blocking(I2C_PORT, adrr, &dummy, 1, true);

      ```
   - The `i2c_write_blocking()` function returns the following values:
      - `>= 0`: Number of bytes written (success).
      - `PICO_ERROR_GENERIC (-1)`: No device responded (NACK).
      - `PICO_ERROR_TIMEOUT (-2)`: Timeout occurred.
   - If the return value is greater than or equal to `0`, it means a device acknowledged the address, indicating a device is present at that address.
      - We can print the address of the device found using:
      ```c
      if (result >= 0){
         printf("I2C device found at 0x%02X\n", adrr);
         }
      ```
   - This way we can find all the devices connected to the I2C bus.
   - **Uploading the code to pico and observing the serial monitor:**
      - <img width="296" height="59" alt="Screenshot from 2025-10-02 17-55-30" src="https://github.com/user-attachments/assets/a58b839d-6203-4099-b309-8b9a7e27fa15" />
      - From the above image we can see that the I2C device is found at address `0x3C`, which is the address of our SSD1306 OLED display.

3. **Turning ON and OFF the Display:**
   - The [03_turn_display_on_off.c](i2c_with_ssd1306/03_turn_display_on_off.c) file demonstarte how we can turn ON and OFF the SSD1306 OLED display using I2C communication.
   - The `SSD1306` expects to kinds of bytes:
      - **Command bytes:** 
         - These bytes are used to configure the display settings.
         - We send `0x00` as the first byte to indicate that the following byte(s) are command(s).
      - **Data bytes:** 
         - These bytes are used to send pixel data to be displayed on the screen.
         - We send `0x40` as the first byte to indicate that the following byte(s) are data.
         - Example: `0xAE` (turn OFF display), `0xAF` (turn ON display).
         - See the command table (page-28) of [SSD1306 datasheet](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf) for more commands.
   - We are to use the `i2c_write_blocking()` function to send the command bytes to the display.
   - It expects the following parameters:
      - `i2c_write_blocking(i2c_inst_t *i2c, uint8_t addr, const uint8_t *src, size_t len, bool nostop)`
         - `i2c`: The I2C instance to use (e.g., `i2c0` or `i2c1`).
         - `addr`: The 7-bit address of the I2C device (e.g., `0x3C` for SSD1306).
         - `src`: Pointer to the data buffer to send.
         - `len`: Number of bytes to send.
         - `nostop`: If true, no STOP condition is sent after the transfer (useful for repeated starts).
   - To turn ON the display, we send the following command sequence:
      ```c
      uint8_t off_cmd[2] = {0x00, 0xAE};
      i2c_write_blocking(I2C_PORT, SSD1306_ADDR, off_cmd, 2, false);
      ```
   - To turn OFF the display, we send the following command sequence:
      ```c
      uint8_t on_cmd[2] = {0x00, 0xAF};
      i2c_write_blocking(I2C_PORT, SSD1306_ADDR, on_cmd, 2, false);
      ```

4. **Initializing the Display:**
   - The [04_initialize_ssd1306.c](i2c_with_ssd1306/04_initialize_ssd1306.c) file demonstarte how we can initialize the SSD1306 OLED display using I2C communication.
   - After this initialization, the display is ready to receive pixel data and show images or text.
   - We are going to create a helper function `ssd1306_cmd()` to send a single command byte to the display.
      ```c
      void ssd1306_cmd(uint8_t cmd) {
          uint8_t buf[2] = {0x00, cmd}; // 0x00 = control byte (command mode)
          i2c_write_blocking(I2C_PORT, SSD1306_ADDR, buf, 2, false);
      }
      ```
   - Now we will use this function to send some sequence of commands to initialize the display.
      ```c
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
      ```
   - This sequence of commands configures various settings of the display, such as addressing mode, multiplex ratio, clock settings, and more.
   - After sending this initialization sequence, the display is ready to receive pixel data and show images or text.

5. **Drawing a line on the Display:**
   - The [05_draw_line.c](i2c_with_ssd1306/05_draw_line.c) file demonstarte how we can draw a line on the SSD1306 OLED display using I2C communication.
   - Before we start drawing, we are defining some functions to help us in drawing.
      1. `ssd1306_cmd(uint8_t cmd)`: Sends a command byte to the display.
      2. `ssd1306_data(uint8_t *data, size_t len)`: Sends data bytes to the display.
         ```c
         void ssd1306_data(uint8_t *data, size_t len){
            uint8_t buf[len + 1]; // first byte is control byte, rest is data
            buf[0] = 0x40;
            for (size_t i = 0; i < len; i++)
            {
               buf[i+1] = data[i]; // copy data into buf array after control byte
            }
            i2c_write_blocking(I2C_PORT, SSD1306_ADDR, buf, len + 1, false); //sending data to display
         }
         ```
      3. `ssd1306_clear()`: Clears the display by writing zeros to all pixels.
         ```c
         void ssd1306_clear() { // clear display by writing 0 to all pixels
            for (uint8_t page = 0; page < 8; page++) {
               ssd1306_cmd(0xB0 + page); // set page
               ssd1306_cmd(0x00);        // lower col = 0 
               ssd1306_cmd(0x10);        // higher col = 0
               uint8_t zeros[128] = {0};     // one full row of empty pixels
               ssd1306_data(zeros, 128); // write empty row
            }
         }
         ```
      - Here, lower column indicates the lower nibble (4 bits) `0x0{lower bits}` of the column address, and higher column indicates the higher nibble (4 bits) `0x1{higher bits}` of the column address. By setting both to `0`, we are effectively setting the column address to `0x00`, which is the starting point for writing pixel data on that page.

      4. `ssd1306_init()`: Initializes the display by sending a sequence of commands.
         - This contains the same initialization sequence we discussed in the previous section.
   - In the `main()` function, we first initialize the I2C communication and the display.
   - Then we clear the display to start with a blank screen.
   - Next, we set the page to `7` (the last page) and the column to `0` to start drawing from the beginning of the last row.
      ```c
      ssd1306_cmd(0xB7); //page 7
      ssd1306_cmd(0x00); //lower column 0
      ssd1306_cmd(0x10); //higher column 0
      ```
   - We create an array `line[128]` to hold the pixel data for the line.
   - We set each byte in the `line` array to `0x01`, which means only the first pixel in each byte is turned ON (1).
      ```c
      for (uint8_t i = 0; i < 128; i++)
      {
         line[i] = 0x01; // setting only one bit high in page to draw line
      }
      ```
   - Finally, we send the `line` array to the display using the `ssd1306_data()` function.
      ```c
      ssd1306_data(line, 128); //sending data to display
      ```
   - This will draw a horizontal line on the last row of the display.
   - **Uploading the code to pico and observing the display:**
      - <img width="256" height="170" alt="horizontalline" src="https://github.com/user-attachments/assets/a6df56b2-e1fd-4e05-b3c9-6e009182a305" />
      - From the above image we can see that a horizontal line is drawn on the last row of the display.

6. **Drawing anything on x-y coordinates system:**
   - Now that we have learned how to draw a line on the display, we are moving a step further to draw anything on the display using x-y coordinates system.
   - The [06_draw_x_y.c](i2c_with_ssd1306/06_draw_x_y.c) file demonstarte how we can draw on x-y coordinates system on the SSD1306 OLED display using I2C communication.
   - We are going to define a 2D array `screen[8][128]` to represent the pixel data of the display.
      - The display has `8` pages (rows) and `128` columns.
      - Each byte in the array represents `8` vertical pixels in a column.
      - For example, `screen[2][5] = 0x04` (00000100) means the 3rd pixel in the 6th column of the 3rd page is ON (1).
   - We are going to create a function `ssd1306_set_pixel()` to set a pixel at given x-y coordinates.
      ```c
      void ssd1306_set_pixel(uint8_t x, uint8_t y){
         screen[y/8][x] = screen[y/8][x] | (1 << (y%8));
      }
      ```
      - Here, `y/8` gives the page number (row), and `x` gives the column number.
      - `y%8` gives the bit position within the byte.
      - We use the bitwise OR operation to set the specific bit to `1` without affecting other bits in the byte.
   - We are also going to create a function `ssd1306_clear_pixel()` to clear a pixel at given x-y coordinates.
      ```c
      void ssd1306_clear_pixel(uint8_t x, uint8_t y){ 
         screen[y/8][x] = screen[y/8][x] & ~(1 << (y%8));
      }
      ```
      - Here, we use the bitwise AND operation with the negated bit mask to clear the specific bit to `0` without affecting other bits in the byte.
   - As these two above functions only modify the `screen` buffer, we need to create another function `ssd1306_update` to send the entire `screen` buffer to the display.
      ```c
      void ssd1306_update(){
         for (uint8_t page = 0; page < 8; page++)
         {
         ssd1306_cmd(0xB0 + page); //page
         ssd1306_cmd(0x00); //lower column
         ssd1306_cmd(0x10); //higher column

         ssd1306_data(screen[page], 128);
         }
    
      }
      ```
   - Also to clear the `screen` buffer, we are going to define a function `ssd1306_clear_buffer()`.
      ```c
      void ssd1306_clear_buffer() {
         for (uint8_t page = 0; page < 8; page++) {
            for (uint8_t x = 0; x < 128; x++)
            {
               screen[page][x] = 0x00;
            }
        
         }
      }
      ```
   - Now that we have defined all the necessary functions, in `main()` function, we are going to draw a diagonal line.
   ```c
   for (uint8_t i = 0; i < 64; i++)
   {
      ssd1306_set_pixel(i, i);
   }
   ```
   - Here, we are setting pixels at coordinates (0,0), (1,1), (2,2), ..., (63,63) to draw a diagonal line from the top-left corner to the middle of the display.
   - Uploading the code to pico and observing the display:
      - <img width="256" height="208" alt="diagonalline" src="https://github.com/user-attachments/assets/ba1f2a67-862e-4299-87d6-e589c55e7b22" />
      - From the above image we can see that a diagonal line is drawn from the top-left corner to the middle of the display.

7. **Drawing figures on display:**
   - The [07_draw_figures.c](i2c_with_ssd1306/07_draw_figures.c) file demonstarte an example of drawing filled and unfilled rectangles on the SSD1306 OLED display using I2C communication.
   - So, we defined a function `ssd1306_draw_rectangle()` to draw a rectangle on the display.
      ```c
      void ssd1306_draw_rectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool fill){ 
         if (!fill){ //creates an unfilled rectangle
            for (uint8_t i = 0; i <= w; i++)
            {   
                  if (x+i < 128){ //avoiding creating beyond screen
                     ssd1306_set_pixel(x+i, y);
                     if (y+h < 64)
                     {
                        ssd1306_set_pixel(x+i, y+h);
                     }
                     
                  }

            }
            for (uint8_t i = 0; i <= h; i++)
            {
                  if (y+i < 64)
                  {
                     ssd1306_set_pixel(x, y+i);
                     if (x+w < 128)
                     {
                        ssd1306_set_pixel(x+w, y+i);
                     }
                     
                  }

                  
            }
         }
         else{ //creates a filled rectangle
            for (uint8_t i = 0; i < w; i++)
            {
                  if (x+i < 128)
                  {
                     for (uint8_t j = 0; j < h; j++)
                     {
                        if (y+j < 64)
                        {
                              ssd1306_set_pixel(x+i, y+j);
                        }
                        
                     }
                     
                  }
                  
            }
         }
      }
      ```
      - Here, `x` and `y` are the top-left corner coordinates of the rectangle, `w` is the width, `h` is the height, and `fill` is a boolean to indicate whether to fill the rectangle or not.
   - In the `main()` function, we are drawing 2 figures:
      1. A face using unfilled rectangles:
         ```c
         ssd1306_draw_rectangle(20, 10, 20, 20, false);
         ssd1306_draw_rectangle(80, 10, 20, 20, false);
         ssd1306_draw_rectangle(20, 50, 80, 5, false);
         ssd1306_draw_rectangle(30, 20, 1, 1, false);
         ssd1306_draw_rectangle(90, 20, 1, 1, false);
         ```
         - Uploading the code to pico and observing the display:
            - <img width="256" height="230" alt="face" src="https://github.com/user-attachments/assets/9a8caf91-4475-49d9-8a41-a2b7e2b1c7b9" />
            - From the above image we can see that a face is drawn using unfilled rectangles.
      2. A filled rectangle:
         ```c
         ssd1306_draw_rectangle(20, 10, 40, 40, true);
         ```
         - Uploading the code to pico and observing the display:
            - <img width="256" height="244" alt="rectangle" src="https://github.com/user-attachments/assets/748e0a27-fef9-436b-a2b2-9e15f9baa785" />
            - From the above image we can see that a filled rectangle is drawn on the display.

            
8. **Displaying text on the Display:**
   - The [08_display_text.c](i2c_with_ssd1306/08_display_text.c) file demonstarte how we can display text on the SSD1306 OLED display using I2C communication.
   - We are going to make an array `font5x7[95][5]` to store the 5x7 font data for ASCII characters from space (32) to tilde (126).
   - Each character is represented by 5 bytes, where each byte represents a column of 8 pixels.
   - For example, the character 'A' (ASCII 65) is represented by the following 5 bytes:
      ```c
      {0x7E, 0x11, 0x11, 0x11, 0x7E} // 'A'
      ```
   - We are going to a function `ssd1306_draw_char()` to draw a single character on the display at given x-y coordinates.
      ```c
      void ssd1306_draw_char(uint8_t x, uint8_t y, char c) { // draws a character at (x, y)
         if (c < 32 || c > 126) return;  // only printable chars
         for (int i = 0; i < 5; i++) {
            uint8_t line = font5x7[c - 32][i]; // pick the column data
            for (int j = 0; j < 7; j++) {
                  if (line & (1 << j)) {  
                     ssd1306_set_pixel(x+i, y+j);
                  }
            }
         }
      } 
      ```
   - Additionally, we are going to create a function `ssd1306_draw_string()` to draw a string of characters on the display at given x-y coordinates.
      ```c
      void ssd1306_draw_string(uint8_t x, uint8_t y, const char *str) { 
         while (*str) {
            ssd1306_draw_char(x, y, *str++);
            x += 6; // 5 pixels for char + 1 space
         }
      }
      ```
   - Now, in the `main()` function, we are drawing a string "Hello, World!" at coordinates (0, 0).
      ```c
      ssd1306_draw_string(0, 0, "HELLO, WORLD!");
      ```
   - Uploading the code to pico and observing the display:
      - <img width="256" height="223" alt="hello_world" src="https://github.com/user-attachments/assets/05aaa290-32cc-4046-bec9-a715697c2405" />
      - From the above image we can see that the text "HELLO, WORLD!" is displayed on the display.

9. **Playing animation on Display:**
   - The [09_animation.c](i2c_with_ssd1306/09_animation.c) file demonstarte how we can play a simple animation on the SSD1306 OLED display using I2C communication.
   - We are going to create a simple animation of a pixel bouncing around the screen.
   - We are going to define the following variables to control the animation:
      ```c
      int8_t dx = -1; //will become +1 further in the code
      int8_t dy = -1;
      uint8_t x = 0; //initial position
      uint8_t y = 0;
      ```
      - Here, `dx` and `dy` are the change in x and y coordinates respectively.
   - Now, in the `while (true)` loop, we are going to update the position of the pixel and draw it on the display.
      ```c
      while (true)
      {
         
         ssd1306_set_pixel(x, y); //set pixel at (x, y)
         ssd1306_update(); //update display
         sleep_ms(50); //wait
         if (x >= 127 || x <= 0) { dx *= -1; } //reverse direction if hitting edge
         if (y >= 63 || y <= 0) { dy *= -1; } 
         ssd1306_clear_pixel(x, y); //clear pixel at (x, y)
         x += dx; //update position
         y += dy;
         
      }
      ```
      - Here, we first set the pixel at the current position (x, y) using `ssd1306_set_pixel()`.
      - Then we update the display using `ssd1306_update()`.
      - We wait for 50 milliseconds using `sleep_ms(50)` to control the speed of the animation.
      - We check if the pixel has hit the edge of the display (x >= 127 or x <= 0 for horizontal edges, y >= 63 or y <= 0 for vertical edges). If it has, we reverse the direction by multiplying `dx` or `dy` by -1.
      - We then clear the pixel at the current position (x, y) using `ssd1306_clear_pixel()`.
      - Finally, we update the position of the pixel by adding `dx` to `x` and `dy` to `y`.
   - Uploading the code to pico and observing the display:
      - ![ball_bounce](https://github.com/user-attachments/assets/483918f9-3a17-4188-ac54-5bd6e516fcbb)
      - From the above animation we can see that a pixel is bouncing around the screen.

10. **Displaying images on Display:**
   - The [10_display_images.c](i2c_with_ssd1306/10_display_images.c) file demonstarte how we can display an image on SSD1306 OLED display.
   - We are going to generate the `screen[8][128]` buffer using the python program [10_image_converter.py](i2c_with_ssd1306/10_image_converter.py).
   - This program takes an image file, converts it to monochrome, resizes it to 128x64 pixels, and generates a C array that can be used in our C program.
   - We our going to generate the `screen` buffer for the image [luffy.jpg](i2c_with_ssd1306/images/luffy.jpg) using the python program.
   - After generating the `screen` buffer, we are going to copy it to our C program.
   - After this we will update the display using the `ssd1306_update()` function to show the image on the display. (Note that we had modified the `ssd1306_update()` function to take the `screen` buffer as a parameter) instead of using the global `screen` buffer.
   - Uploading the code to pico and observing the display:
      - <img width="256" height="226" alt="luffyface" src="https://github.com/user-attachments/assets/47655902-876b-42a6-919e-078e29b919ad" />
      - From the above image we can see that the image is displayed on the display.

---

### **Using Standard Library for SSD1306**
- Now that we have learned how to use I2C communication and control the SSD1306 OLED display using low-level commands, we can also use a standard library to simplify our code.
- We are going to use the [pico-ssd1306](http://github.com/daschr/pico-ssd1306) library, which provides a high-level API to control the SSD1306 display.
- So, to use this library in our code, we need to follow these steps:
   1. Download the library from the [GitHub repository](http://github.com/daschr/pico-ssd1306) as a ZIP file.
   2. Extract the ZIP file and copy the `pico-ssd1306` folder to `~/pico/libraries/` directory.
   3. Now change the directory to the `pico-ssd1306` folder:
      ```bash
      cd ~/pico/libraries/pico-ssd1306
      ```
   4. Now, inside the `pico-ssd1306` folder, create a `CMakeLists.txt` file with the following content:
      ```cmake
      add_library(ssd1306 ssd1306.c)

      target_include_directories(ssd1306 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

      target_link_libraries(ssd1306 PUBLIC pico_stdlib hardware_i2c)
      ```
   5. Now, we need to make an environment variable for our library path, so that it can be accessed from anywhere in future programs without specifying the full path.
      ```bash
      echo "export PICO_LIB_PATH=$HOME/pico/libraries" >> ~/.bashrc
      source ~/.bashrc
      ```
   6. Now, with this setup, we can include the `ssd1306` library or any other external library by mentioning in `CMakeLists.txt` and easily use in our C programs.
   7. To include the `ssd1306` library in our C program, we need to add the following in our [CMakeLists.txt](i2c_with_ssd1306/CMakeLists.txt) file:
      ```cmake
      add_subdirectory($ENV{PICO_LIB_PATH}/ssd1306 ${CMAKE_BINARY_DIR}/ssd1306_build)
      ```
      - add this line after project settings.
   - Also, we need to link the `ssd1306` library to our project by adding the following line in the `target_link_libraries()` function:
      ```cmake
      target_link_libraries(11_using_ssd1306_library pico_stdlib hardware_i2c ssd1306)
      ```

- Now that everything is set up, let's do a quick demonstration of using this library in [11_using_ssd1306_library.c](i2c_with_ssd1306/11_using_ssd1306_library.c) file.
   - We are going to include the `ssd1306.h` header file in our C program.
      ```c
      #include "ssd1306.h"
      ```
   - We need define an instance of the `ssd1306_t` structure to hold the display configuration.
      ```c
      ssd1306_t display;
      ```
   - Now, we are not using any external source to power `SSD1306`(for some display it requires external power source 8-9V), so we to define that we are not using any external power source by setting the `external_vcc` member of the `ssd1306_t` structure to `false`.
      ```c
      display.external_vcc = false; 
      ```
   - Now, we are going to initialize the SSD1306 display using the `ssd1306_init()` function.
      ```c
      ssd1306_init(&display, 128, 64, SSD1306_ADDR, I2C_PORT);
      ```
      - Here, we are passing the following parameters to the `ssd1306_init()` function:
         - `&display`: Pointer to the `ssd1306_t` structure.
         - `128`: Width of the display in pixels.
         - `64`: Height of the display in pixels.
         - `SSD1306_ADDR`: I2C address of the display (0x3C for most displays).
         - `I2C_PORT`: I2C port to use.
 
   - To clear display, we are going to use `ssd1306_clear()` function.
      ```c
      ssd1306_clear(&display);
      ```
   - To update the display, we are going to use `ssd1306_show()`
      ```c
      ssd1306_show(&display);
      ```
   - To draw unfilled square, we are going to use `ssd1306_draw_empty_square()`.
      ```c
      ssd1306_draw_empty_square(&display, 20, 20, 40, 20);
      ```
   - To draw a character, we are going to use `ssd1306_draw_char()` function.
      ```c
      ssd1306_draw_char(&display, 12*5, 0, 1, '_');
      ```
   - Using these all function [11_using_ssd1306_library.c](i2c_with_ssd1306/11_using_ssd1306_library.c) displays an animation of "Hello World" and show a unfilled square.

   - Uploading the code to pico and observing the display:
      - ![helloWorldAnimation](https://github.com/user-attachments/assets/e4b2ed24-7269-4c56-aca9-180b2dcca7d8)
      - From the above animation we can see every character of "Hello World" is displayed one by one and it also show an unfilled rectangle in bottom.

---
