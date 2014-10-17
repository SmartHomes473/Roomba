#include <stdint.h>
#include "msp430g2553.h"

#define delay_20ms() delay_cycles(20 * 1000)
#define delay_500ms() delay_cycles((uint32_t)500000)


/**
 * Initalize procesor clocks
 * Disable watchdog timer
 */
void processor_init();

/**
 * Delay processor for n * XXXX time
 */
void delay_cycles(register unsigned int n);
