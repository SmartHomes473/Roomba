#include "msp430g2553.h"
#include "msp430g2553_utils.h"
#include "roomba.h"
#include "string.h"
#include "software_uart.h"
#include "hardware_uart.h"
#include "smrtcontrol_comms.h"

typedef enum
{
	NONE,
	STATUS,
	DEVICE_NUM,
	SIZE,
	SIZE_2,
	DATA, 
	END
} parse_state_t;

int main(void) {
	/* Initializations */
	processor_init();
	UART_init_9600();
	softwareUART_init();
	initialize_roomba();

	/* Register Device */
	uint8_t* device_num_addr = (uint8_t*)FLASH_START_ADDR;
	register_device(device_num_addr);

	/* Parsing Information */
	uint8_t new_UART_RX;
	parse_state_t state = NONE;
	uint16_t packet_size = 1;
	uint8_t data[10];
	uint8_t data_idx = 0;
	uint8_t device_num;
	uint8_t status;

	while (1)
	{
		if (UART_data_available() == 1) {
			new_UART_RX = UART_get_data();
			switch(state)
			{
			case NONE:
				if (new_UART_RX == PACKET_START_BYTE) {
					state = DEVICE_NUM;
				}
				break;
			case DEVICE_NUM:
				device_num = new_UART_RX;
				state = STATUS;
				break;
			case STATUS:
				status = new_UART_RX;
				if (status == 0)
				{
					/* Store device number */
					flash_erase(device_num_addr);
					flash_write(device_num_addr, device_num);
					state = NONE;
				}
				else
				{
					state = SIZE;
				}
				break;
			case SIZE:
				packet_size = (new_UART_RX << 8);
				state = SIZE_2;
				break;
			case SIZE_2:
				packet_size |= (new_UART_RX);

				if (packet_size == 0 && device_num == 0)
				{
					/* Device being deleted */
					flash_erase(device_num_addr);
					state = NONE;
				}
				else
				{
					state = DATA;
				}
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

		if (data_idx >= packet_size && state == DATA)
		{
			/* Packet done */
			deserialize(data);
			data_idx = 0;
			state = END;
		}
	}
}
