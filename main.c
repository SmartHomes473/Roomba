#include "msp430g2553.h"
#include "msp430g2553_utils.h"
#include "roomba.h"
#include "software_uart.h"
#include "hardware_uart.h"

int main(void) {

	processor_init();
	UART_init_9600();
	softwareUART_init();

	while (1)
	{
		if (new_UART_RX != 0) {
			softwareUART_send_byte(new_UART_RX);
			new_UART_RX = 0;
		}
	}
}
