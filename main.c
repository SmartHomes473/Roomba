#include "msp430g2553.h"
#include "msp430g2553_utils.h"

int main(void) {

	processor_init();
	UART_init_9600();

	P1DIR = 0x40;
	P1OUT = 0;
	while (1)
	{
		P1OUT = 0x40;
		_delay_cycles(1000);
		P1OUT = 0;
		_delay_cycles(1000);
	}
}
