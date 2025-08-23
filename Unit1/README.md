# Unit 1 – Embedded Systems Basics + GPIO Concepts

This is the following structure of this Unit:
   - **Key Concepts**
      - What is an embedded system? 
      - Microcontrollers vs. Microprocessors. 
      - Pico W architecture
   - **Projects**
      -  Blink an LED (basic GPIO output)
      -  Read a push button (basic GPIO input)

## Key Concepts

1.**What is an embedded system?**
   - **Definition**: An embedded system is a specialized computer system designed to perform dedicated functions within a larger mechanical, electrical, or electronic system. Unlike general-purpose computers, embedded systems are optimized for specific tasks with real-time constraints.
   - **Characteristics**:

      - **Single-purpose functionality:** Designed for specific applications
      - **Real-time operation:** Must respond to inputs within defined time constraints
      - **Resource constraints:** Limited memory, processing power, and energy consumption
      - **Reliability requirements:** Often operate in harsh environments with minimal maintenance
   - **Examples**
      ```
      Consumer Electronics: Smart thermostats, washing machines, automotive ECUs
      Industrial Systems: PLCs, robotic controllers, sensor networks
      Medical Devices: Pacemakers, insulin pumps, monitoring equipment
      Communication: Routers, IoT devices, wireless sensors
      ```

2. **Microcontrollers vs. Microprocessors**

| Aspect               | Microcontroller (MCU)            | Microprocessor (MPU)             |
|-----------------------|----------------------------------|----------------------------------|
| **Architecture**      | System-on-Chip (SoC)            | CPU-centric design               |
| **Integration**       | CPU + RAM + ROM + I/O on single chip | CPU only, external components required |
| **Memory**            | Built-in Flash/SRAM (KB to MB)  | External RAM/storage (GB range)  |
| **Power Consumption** | Ultra-low power (µA to mA)      | Higher power consumption (hundreds of mA to watts) |
| **Cost**              | Low cost ($1–$50)               | Higher cost ($20–$500+)          |
| **Real-time Perf.**   | Deterministic, low-latency      | Variable latency, OS overhead    |
| **Dev. Complexity**   | Simpler, bare-metal programming | Complex, requires OS and drivers |


3. **Raspberry Pi Pico W Architecture**

   - **RP2040 Microcontroller Specifications**:
      - **CPU Configuration**: Dual-core ARM Cortex-M0+ @ 133MHz
      - **Memory Architecture**:
         - 264KB SRAM (6 independent banks for concurrent access)
         - 2MB Flash memory (external QSPI)
         - 16KB boot ROM with USB bootloader
      - **Peripheral Integration**:
         - 30 GPIO pins (26 user-accessible)
         - 2x UART, 2x SPI, 2x I2C interfaces
         - 16x PWM channels
         - 4x ADC inputs (12-bit resolution)


   -  **Wireless Module (CYW43439)**:
      - **Standards**: IEEE 802.11 b/g/n (2.4GHz)
      - **Interface**: SDIO connection to RP2040
      - **Additional Features**: Bluetooth 5.2 capability (requires separate stack)
      - **Power Management**: Deep sleep modes with wake-on-wireless

## Key functions: 
1. **Blink an LED (basic GPIO output)**
-  Let's understand some key functions used in [blink_led.c]()
   ```c
   #include "pico/stdlib.h"
   ```
   - This is the header file to be included as it contains standard library headers for hardware abstraction
   ```c
   stdio_init_all(); 
   ```
   - This function initializes the standard I/O 
   - It is usefull when we want to use serial communication interface for debugging output
   - It is a good habit to call this function even if you we don’t use the serial communication
   ```c
   gpio_init(pin);
   ```
   - It sets up the hardware registers for that pin, clearing any alternate function and making it a “plain GPIO.
   ```c
   gpio_set_dir(pin, GPIO_OUT);
   ```
   - It sets up a bit in the **GPIO control register** that determines direction:
      - `GPIO_OUT` : Output (1)
      - `GPIO_IN` : Input (0)
   ```c
   gpio_put(pin, value);
   ```
   - It sets the GPIO pin to HIGH(1) or LOW(0) state.
   ```c
   sleep_ms(milliseconds);
   ```
   - It pauses the program for a certain amount of time in milliseconds.

So with the understanding of these function we could write the program [blink_led.c]() that blinks the LED at GPIO15 at a frequency of 1 Hz.

2. **Read a push button (basic GPIO input):**
   - Let's understand some key functions used in [read_push_button.c]()
      ```c
      gpio_pull_up(pin);
      ```
      - This function consigures the input pin with the internal **pul up** resistor.
      - A pull up resistor sets the state of input pin to HIGH when button is not pressed (makes a default state for it)
      - A pull down resistor sets the state of input pin to LOW when button is not pressed
      //add image
      ```c
      gpio_get(pin);
      ```
      - This function returns the current state of the input pin.
      ```c
      printf();
      ```
      - Unlike the `printf()` function from the standard I/O C library this `printf()` function from the  `pico/stdlib.h` prints the output on the serial monitor.
So with the understanding of these and previous functions we can write the program [read_push_button.c]() that turns the LED on when the button is pressed and prints on the serial monitor that the `button is pressed` or else prints `button not pressed`.

## CMakeLists.txt file
   - A CMakeLists.txt file is the primary configuration file used by CMake that build, test, and package programs. This plain text file contains a set of directives and instructions that describe how to build a project, including its source files, libraries, and executables.

   - We don't need to memorize this file but we should understand the content of these files.
   - Let's understand some elements used in [CMakeLists.txt]() file.
      ```txt
      cmake_minimum_required(VERSION 3.13)
      ```
      - It make sure that CMake uses the required version.
      ```txt
      include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
      ```
      - Imports functions/macros like `pico_sdk_init()` and `pico_add_extra_outputs()` from Pico SDK.
      ```txt
      pico_sdk_init()
      ```
      - Initialize Pico SDK to run setup macros to make it available for building.
      ```txt
      project(<PROJECT_NAME> C CXX ASM)
      set(CMAKE_C_STANDARD 11)
      set(CMAKE_CXX_STANDARD 17)
      ```
      - Names your project.
      - Specifies C and C++ standards (C11 / C++17).
      ```txt
      add_executable(<TARGET_NAME> <SOURCE_FILE.c>)
      target_link_libraries(<TARGET_NAME> pico_stdlib)
      pico_add_extra_outputs(<TARGET_NAME>)
      ```
      - `add_executable` : tells CMake to compile this .c into a program.

      - `target_link_libraries` : links Pico standard library (GPIO, sleep, stdio).

      - `pico_add_extra_outputs` : generates .uf2 for flashing.

   - **If you want to use the serial monitor**
      ```txt
      add_executable(<TARGET_NAME> <SOURCE_FILE.c>)
      target_link_libraries(<TARGET_NAME> pico_stdlib)

      pico_enable_stdio_usb(<TARGET_NAME> 1)   # enable USB serial
      pico_enable_stdio_uart(<TARGET_NAME> 0)  # disable UART serial

      pico_add_extra_outputs(<TARGET_NAME>)
      ```

After understanding this with the use of [blink_led.c](), [read_push_button.c]() and [CMakeLists.txt]() we can generate the `.uf2` files to be flash into our Pi Pico using these command in the directory with these files:
```bash
mkdir build
cd build
cmake ..
make
````
## Building and running the Circuit
1. **Blink an LED**
- **Components used:**
   - Red LED.
   - 270 ohm resistor.
- **The Circuit:**
   - //add image
- **Running the Circuit:**
//add video

2. **Read a push button**
- **Components used:**
   - Red LED.
   - 270 ohm resistor.
   - Push button.
   - Jumper wires.
- **The Circuit:**
   - //add image
- **Running the Circuit:**
//add video
- **Observing the serial monitor:**
   - To observe the serial monitor we need to download the utility `screen`
   ```bash
   sudo apt install screen
   ```
   - Now we can open the serial monitor by:
   ```bash
   sudo screen /dev/ttyACM0 115200
   ```
   - Note: We used `sudo` here as sometimes this command is restricted for normal users.

