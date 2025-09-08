
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

1. **Software-based Delay (Busy Wait)**
    -  A simple 'for' or 'while' loop is used to create an approximate delay by iterating a calculated number of times.
    -  In [SoftwareDelayBlink.c]() the function `software_delay_ms` burns the CPU cycles to create create a desired delay. `count = ms * 133000` stores the the number of CPU cycles to make a delay in milliseconds.
    - **Limitation of using software delays:** The actual timing is unreliable—it depends on compiler optimizations, instruction timings, and may vary significantly (may be off by x10).


2. **SDK Delay Using `sleep_ms()`**

    -  In [SDKDelayBlink.c](), the function `sleep_ms()` uses the internal hardware timers for accurate and portable delays.
    -  It is tied to the 12 MHz crystal, making delays very close to requested intervals.
    - It physically sleeps the CPU that reduces the power usage.
