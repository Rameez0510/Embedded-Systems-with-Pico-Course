# Unit 3 – Interrupts & Event-Driven Programming

This is the following structure of this Unit:
   - **Key Concepts**
        - Polling vs interrupts
        - Edge-triggered vs level-triggered
        - Why debouncing matters for mechanical buttons
   - **Experiments & Projects**
      - Button interrupt with no debouncing
      - Button interrupt with Software debouncing

## Key Concepts

1. **Polling vs Interrupts**
   - **Polling:** Polling is a process where the CPU repeatedly checks if a device needs attention like a button press.
   - **Interrupts:** Interrupt is a device-driven process where a hardware signal notifies the CPU that attention is required.

2. **Edge-triggered vs. Level-triggered interrupts**
   - **Edge-triggered:** The interrupt fires when a change happens (e.g., button goes from not pressed → pressed).
   - **Level-triggered:** The interrupt fires as long as the condition is true (e.g., button is held down).
   - On microcontrollers, we usually prefer edge-triggered for buttons, otherwise you’d get a flood of repeated interrupts if someone held the button down.


3. **Debouncing & Why it matters for mechanical buttons**
   - Real buttons are mechanical. When you press one, the contacts inside bounce for a few milliseconds — making the signal go press-release-press-release really fast.
   - <img width="521" height="276" alt="Switch-Bouncing-in-the-Circuit" src="https://github.com/user-attachments/assets/472a8d19-61c2-4597-a741-32005f78c334" />

   - If you don’t handle this, your program might think you pressed the button 5 times when you only pressed once.

-  **Solutions:**
      - **Software debouncing:** ignore changes for a short time (say 10 ms) after the first press.
      - **Hardware debouncing:** add a small RC circuit (resistor + capacitor) or use dedicated chips.


## Experiments & Projects 
1. **Button interrupt with no debouncing:**
      - The [button_Interrupt_no_debounce.c](button_Interrupt_no_debounce.c) file demonstrates what happens when we use edge triggered interrupts without any debouncing.
      - Some of the new **Key Functions** used:
         - `gpio_set_irq_enabled_with_callback(GPIO pin, type of trigger, bool enable, global ISR for GPIO IRQs)`: This function registers one global callback for GPIO interrupts and enables the specific edge trigger
         - `GPIO_IRQ_EDGE_FALL`: Falling edge trigger as our circuit will be **active low**
         - `button_isr(uint gpio, uint32_t events)`: This is the ISR function that toggles the LED when called. `uint gpio` holds the pin number of the button, and `uint32_t events` holds the value of the type of event like `GPIO_IRQ_EDGE_FALL`. These values are already passed through `gpio_set_irq_enabled_with_callback` so we don't send it manually.
      - Setting up the circuit and flashing `.uf2` file:
         - **Components**: LED, push button, resistor, wires.
         - <img width="471" height="241" alt="Screenshot from 2025-09-13 12-56-02" src="https://github.com/user-attachments/assets/195ba8bb-c405-433f-8b05-8cc32514981e" />

         - Upload the .uf2 to pico!
      - Running the circuit:
         - ![noDebounce](https://github.com/user-attachments/assets/3fbc6e8b-4d22-45f1-8104-0a100d5dfd7e)

         - **Observation:** Sometime it toggles once when button is pressed, sometime twice, 
         - It has problems of interpreting multiple triggers on a single press. **Let's fix it!**
2. **Button interrupt with Software debouncing:**
   - The [button_Interrupt_software_debounce.c](button_Interrupt_software_debounce.c) file demonstrates what happens when we use edge triggered interrupts with Software debouncing.
   - Some of the new **Key Functions** used:
      - `to_ms_since_boot(get_absolute_time())`: This function is used to get the system uptime in ms. 
   - Setting up the circuit and flashing `.uf2` file:
      - **Components**: LED, push button, resistor, wires.
      - <img width="471" height="241" alt="Screenshot from 2025-09-13 12-56-02" src="https://github.com/user-attachments/assets/195ba8bb-c405-433f-8b05-8cc32514981e" />
      - Upload the .uf2 to pico!
   - Running the circuit:
      - ![softwareDebounce](https://github.com/user-attachments/assets/3fe620c6-e463-42a0-a34d-50055d23dcbd)

      - **Observation:** Now it toggles correctly, one toggle for one press! 
