#include "msp430g2553.h"
#include "msp430g2553_utils.h"
#include "roomba.h"
#include "string.h"
#include "software_uart.h"
#include "hardware_uart.h"
#include "serialization.h"

#define PACKET_START_BYTE 0x0F
#define PACKET_END_BYTE 0x04
#define DEVICE_URL "http://rwooster.com/roomba.tar.gz"

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
	processor_init();
	UART_init_9600();
	softwareUART_init();
	initialize_roomba();

	/*

	int* device_num = (int*)FLASH_START_ADDR;
	flash_erase(device_num);
	flash_write(device_num, 100);
	int test_value = *device_num;

	*/

	uint8_t new_UART_RX;
	parse_state_t state = NONE;
	uint16_t packet_size = 1;
	uint8_t data[10];
	uint8_t data_idx = 0;

	uint8_t* device_num_addr = (uint8_t*)FLASH_START_ADDR;
	uint8_t device_num;
	uint8_t status;

	while (1)
	{
		/* Uninitialized flash memory is all 1 */
		if (*device_num_addr == 255)
		{
			/* Register device */
			uint8_t register_array[39] = {PACKET_START_BYTE, 0, 0, 0, 33};
			memcpy(register_array+5, DEVICE_URL, 33);
			register_array[38] = PACKET_END_BYTE;

			UART_send_array(register_array, 39);

			flash_erase(device_num_addr);
			flash_write(device_num_addr, 0);
		}

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

		if (status == 0 && packet_size == 0 && device_num != 0)
		{
			flash_erase(device_num_addr);
			flash_write(device_num_addr, device_num);
		}
		else if (data_idx >= packet_size && state == DATA)
		{
			deserialize(data);
			data_idx = 0;
			state = END;
		}
	}
}

