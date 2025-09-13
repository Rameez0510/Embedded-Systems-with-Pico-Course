
# Unit 2: Timing Delays in Embedded Systems

Structure of this Unit:
   - **Key Concepts**
      - Why Timing Matters
      - System Clock Sources
      - Delay Mechanisms
   - **Experiments & Projects**
      -  Software-based Delay 
      -  SDK Delay Using `sleep_ms()`
      -  Non-blocking Delays Using Repeating Hardware Timer


## Key Concepts

1. **Why Timing Matters**

    -  Precise timing is fundamental in embedded systems for tasks like blinking LEDs, generating waveforms, refreshing displays, polling sensors, and scheduling communications. 
    -  Inaccurate timing can cause malfunction and unpredictable behavior, especially where synchronization is crucial.

2. **System Clock Sources**

    - **External 12 MHz Crystal Oscillator:** Provides precise reference for the Pico W.
    - **PLL (Phase-Locked Loop):** Multiplies 12 MHz to 133 MHz as the ARM Cortex-M0+ core clock.
    - **Internal Ring Oscillator (ROSC):** Used as fallback, but less accurate.


3. **Delay Mechanisms**

    - **Software Loops:** Naive; burn CPU cycles; not accurate or efficient.
    - **Hardware Timers:** Use counters and alarms inside the MCU; accurate and power-efficient.
    - **Non-blocking Scheduling:** Avoids CPU idle periods; allows concurrent multitasking.
## Experiments & Projects

1. **Software-based Delay (Busy Wait):**
      - The [SoftwareDelayBlink.c](SoftwareDelay/SoftwareDelayBlink.c) file demonstrates the delay made using a  simple 'for' or 'while' loop to create an approximate delay by iterating it a calculated number of times.
      - Some of the new **Key Functions** used:
         - `software_delay_ms` burns the CPU cycles to create create a desired delay.
         - `count = ms * 133000` stores the the number of CPU cycles to make a delay in milliseconds.
      - Setting up the circuit and flashing `.uf2` file:
         - **Components**: LED, resistor, wires.
         - <img width="448" height="284" alt="Screenshot from 2025-09-13 13-33-48" src="https://github.com/user-attachments/assets/cb5b48a8-2abb-4d80-b94c-80cc3eb40d84" />
         - Upload the .uf2 to pico!
      - Observations after running the circuit:

         - Instead of toggling at 500ms it takes much more time ~5000ms (10 times) 
         - **Limitation of using software delays:** The actual timing is unreliable—it depends on compiler optimizations, instruction timings, and may vary significantly.

2. **SDK Delay Using `sleep_ms()`:**
      - The [SDKDelayBlink.c,](SDKDelay/SDKDelayBlink.c) file demonstrates the delay made using the function `sleep_ms()` that uses the internal hardware timers for accurate and portable delays.
      - It physically sleeps the CPU that reduces the power usage.
      - Setting up the circuit and flashing `.uf2` file:
         - **Components**: LED, resistor, wires.
         - <img width="448" height="284" alt="Screenshot from 2025-09-13 13-33-48" src="https://github.com/user-attachments/assets/cb5b48a8-2abb-4d80-b94c-80cc3eb40d84" />
         - Upload the .uf2 to pico!
      - Observations after running the circuit:

         - It now toggles at proper rate of 500ms.

3. **Non-blocking Delays Using Repeating Hardware Timer**
      - The [HardwareDelayBlink.c](HardwareDelay/HardwareDelayBlink.c) file demonstrates the delay made using `hardware timer` that triggers a callback function at regular intervals, toggling the LED without blocking the main code flow.
      - Some of the new **Key Functions** used:
         - `add_repeating_timer_ms(delay, callback function, user data, repeating_timer pointer)`: This function schedules recurring timer callback (non-blocking).
      - Setting up the circuit and flashing `.uf2` file:
         - **Components**: LED, resistor, wires.
         - <img width="448" height="284" alt="Screenshot from 2025-09-13 13-33-48" src="https://github.com/user-attachments/assets/cb5b48a8-2abb-4d80-b94c-80cc3eb40d84" />
         - Upload the .uf2 to pico!
      - Observations after running the circuit:
        - It now toggles at proper rate of 500ms.

    - In a similar fashion the [Blink_2_LEDs.c](HardwareDelay/Blink_2_LEDs.c) file demonstrates Hardware timer by toggling two LEDs at different rates by passing `user_data` through `add_repeating_timer_ms` function. 
   - Setting up the circuit and flashing `.uf2` file:
      - **Components**: 2 LEDs, resistors, wires.
      - <img width="458" height="225" alt="Screenshot from 2025-09-13 19-10-53" src="https://github.com/user-attachments/assets/74548c0d-458c-4bc2-8a9a-21fb1869ad24" />
      - Upload the .uf2 to pico!
   - Running the circuit:
      - ![blink2](https://github.com/user-attachments/assets/902a6c54-c6db-499f-a793-0f220c6e46eb)
