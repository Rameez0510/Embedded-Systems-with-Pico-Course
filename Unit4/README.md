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


#### Wiring Setup:
- Pico GP4 (SDA) → SSD1306 SDA
- Pico GP5 (SCL) → SSD1306 SCL
- Pico 3.3 V → SSD1306 VCC
- Pico GND → SSD1306 GND
- //add image

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
      - //add image
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
      - //add image
      - From the above image we can see that a horizontal line is drawn on the last row of the display.