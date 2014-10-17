#include "msp430g2553_utils.h"

#define BAUD_9600 104
#define BAUD_57600 1

/******************************************************************************
* Initialize Processor 
******************************************************************************/
void processor_init()
{
	/* Disable watchdog */
	WDTCTL = WDTPW | WDTHOLD;

	/* Set DCO to 1MHz */
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	/* GPIO for the device detect line */
	P1DIR |= BIT0;

	/* DD lines is active low */
	P1OUT |= BIT0;
}

		

