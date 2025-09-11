#include "pico/stdlib.h"

void software_delay_ms (uint32_t ms){
    volatile uint32_t count = 133000 * ms; // 1 ms has ~133000 clock counts
    while (count--)
    {
        __asm volatile("nop"); // no operation: consume a CPU cycle
    }
    
}

int main(){
   stdio_init_all();            // Initialize stdio 
   gpio_init(15);               // Prepare GPIO 15
   gpio_set_dir(15, GPIO_OUT);  // Configure GPIO 15 as output for LED

   // Blink loop using the software delay above.
   while (1)
   {
       gpio_put(15, 0);            // Turn LED off 
       software_delay_ms(500);     // Busy-wait ~500 ms
       gpio_put(15, 1);            // Turn LED on
       software_delay_ms(500);     // Busy-wait ~500 ms
   }
}