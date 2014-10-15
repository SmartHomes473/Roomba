#include "msp430g2553.h"
#include "msp430g2553_utils.h"
#include "roomba.h"
#include "software_uart.h"

int main(void) {

	processor_init();
	UART_init_57600();
	softwareUART_init();

	uint8_t array[5] = {5, 10, 15, 20, 25};
	softwareUART_send_array(array, 5);

	while (1)
	{

	}
}
