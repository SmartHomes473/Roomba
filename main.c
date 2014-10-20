#include "msp430g2553.h"
#include "msp430g2553_utils.h"
#include "roomba.h"
#include "software_uart.h"
#include "hardware_uart.h"
#include "serialization.h"

#define PACKET_START_BYTE 0xFF

typedef enum
{
	NONE,
	SIZE,
	DATA
} parse_state_t;

int main(void) {

	processor_init();
	UART_init_9600();
	softwareUART_init();

	uint8_t new_UART_RX;
	parse_state_t state = NONE;
	uint8_t packet_size = 1;
	uint8_t data[UINT8_MAX];
	uint8_t data_idx = 0;

	while (1)
	{
		if (UART_data_available() == 1) {
			new_UART_RX = UART_get_data();
			switch(state)
			{
			case NONE:
				if (new_UART_RX == PACKET_START_BYTE) {
					state = SIZE;
				}
				break;
			case SIZE:
				packet_size = new_UART_RX;
				state = DATA;
				break;
			case DATA:
				data[data_idx] = new_UART_RX;
				data_idx++;
				break;
			}
			new_UART_RX = 0;
		}

		if (data_idx >= packet_size) {
			deserialize(data);
			data_idx = 0;
			state = NONE;
		}
	}
}
