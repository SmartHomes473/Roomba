#include <stdint.h>
#include "msp430g2553.h"

#define delay_20ms() _delay_cycles(20 * 1000)
#define delay_500ms() _delay_cycles(500000)


/**
 * Initalize procesor clocks
 * Disable watchdog timer
 */
void processor_init();

