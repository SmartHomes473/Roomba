#include "msp430g2553.h"
#include "msp430g2553_utils.h"

int main(void) {

	processor_init();
	UART_init_9600();

	UARTSendArray("Hello", 5);

	while (1)
	{
		/* Trap CPU */
	}
}
