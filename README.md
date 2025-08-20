# Unit 0 – Environment Setup

This Unit was focused on setting up the development environment for Raspberry Pi Pico (C SDK) on Linux.

## Steps

1. **Install Ubuntu**
   - Initally I was using WSL but I was unable to build the uf2 file, so I switched to a proper linux environment (Ubuntu).

2. **Install Toolchain and Dependencies**
   - `gcc-arm-none-eabi`
   - `cmake`
   - `build-essential`
   - `git`
   - `libnewlib-arm-none-eabi`

      ```bash
      sudo apt update
      sudo apt install cmake gcc-arm-none-eabi build-essential git libnewlib-arm-none-eabi   
      ```

3. **Install Pico SDK**
   - Clone official [pico-sdk](https://github.com/raspberrypi/pico-sdk) repository.
      ```bash
      mkdir -p ~/pico
      cd ~/pico
      git clone -b master https://github.com/raspberrypi/pico-sdk.git
      cd pico-sdk
      git submodule update --init
      ```

   - Set environment variable `PICO_SDK_PATH`.
      ```bash
      echo "export PICO_SDK_PATH=$HOME/pico/pico-sdk" >> ~/.bashrc
      source ~/.bashrc
      ```


4. **Test Project – Blink**
   - Create `myblink` project.
   - Write minimal [C program](Unit0/blink_gp15/blink_gp15.c) to blink LED (external LED for Pico W) and [CMakeLists.txt](Unit0/blink_gp15/CMakeLists.txt) file.
   - Verify that build process works:
     ```bash
     mkdir build
     cd build
     cmake ..
     make
     ```
   - Successfully generate `.uf2` file.

5. **Building the Circuit**
   - Component Used:
      - Red LED
      - 220 ohm resistor
      - Jumper Wires
   - The Circuit:
   - <img width="483" height="286" alt="Unit0Circuit" src="https://github.com/user-attachments/assets/80bd6b01-f62a-40a2-9d79-d10a6fda0519" />

    

6. **Flash the Pico**
   - Connect Pico W in BOOTSEL mode.
      - Hold the BOOTSEL button.
         - ![Bootsel](https://github.com/user-attachments/assets/8c32a6d9-6711-4ce6-97ab-d5e08f124979)

      - Insert the USB
      - Release the button.
   - Copy `.uf2` file to mounted storage.
   - LED starts to Blink!
       - ![blinkVideo](https://github.com/user-attachments/assets/60f1f7ae-8698-4ff0-824d-f122ace8a648)



## Notes on Pico W LED
- **Original Pico**: Onboard LED = GPIO 25.
- **Pico W**: Onboard LED controlled by Wi-Fi chip, not via GPIO 25.
- For now, used **external LED + resistor** on a GPIO pin to test.

---

✅ Unit 0 completed – toolchain and environment are ready for development.


