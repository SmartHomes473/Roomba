#include "msp430g2553_utils.h"


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
}

/******************************************************************************
* Delay for XXXX Amount of time
******************************************************************************/
//void __inline__ delay_cycles(register unsigned int n)
//{
//	__asm__ __volatile__ (
//			"1: \n"
//			" dec	%[n] \n"
//			" jne	1b \n"
//			: [n] "+r"(n));
//}

