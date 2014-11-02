#include "msp430g2553.h"
#include "msp430g2553_utils.h"
#include "roomba.h"
#include "software_uart.h"
#include "hardware_uart.h"
#include "serialization.h"

#define PACKET_START_BYTE 0x0F
#define PACKET_END_BYTE 0x04

typedef enum
{
	NONE,
	STATUS,
	SIZE,
	SIZE_2,
	DATA, 
	END
} parse_state_t;

int main(void) {

	processor_init();
	UART_init_9600();
	softwareUART_init();

	uint8_t new_UART_RX;
	parse_state_t state = NONE;
	uint16_t packet_size = 1;
	uint8_t data[10];
	uint8_t data_idx = 0;

	while (1)
	{
		if (UART_data_available() == 1) {
			new_UART_RX = UART_get_data();
			switch(state)
			{
			case NONE:
				if (new_UART_RX == PACKET_START_BYTE) {
					state = STATUS;
				}
				break;
			case STATUS:
				state = SIZE;
				break;
			case SIZE:
				packet_size = (new_UART_RX << 8);
				state = SIZE_2;
				break;
			case SIZE_2:
				packet_size |= (new_UART_RX);
				state = DATA;
				break;
			case DATA:
				data[data_idx] = new_UART_RX;
				data_idx++;
				break;
			case END:
				state = NONE;
				break;
			}
		}

		if (data_idx >= packet_size && state == DATA) {
			deserialize(data);
			data_idx = 0;
			state = END;
		}
	}
}
