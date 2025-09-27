# Unit 3: Displays & Communication with Peripherals (I²C)

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
