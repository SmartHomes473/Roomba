#include "smrtcontrol_comms.h"
#include "roomba.h"
#include "hardware_uart.h"
#include "msp430g2553_utils.h"

#define DEVICE_URL "http://rwooster.com/roomba.tar.gz"

/******************************************************************************
* Calls the Roomba interface function given some serialized function call
******************************************************************************/
void deserialize(uint8_t* function)
{
	switch(function[0])
	{
	case INITIALIZE_ROOMBA:
		initialize_roomba();
		break;
	case SET_MODE:
		set_mode((roomba_mode_t)(function[1] - ASCII_OFFSET));
		break;
	case START_CLEAN:
		start_clean((clean_mode_t)(function[1] - ASCII_OFFSET));
		break;
	case SEND_DOCK:
		send_dock();
		break;
	case SET_WHEEL_SPEEDS:
	{
		/* high byte right, low byte right, high byte left, low byte left */
		int16_t left, right;
		right = (function[1] << 8) & function[2];
		left = (function[3] << 8) & function[4];
		set_wheel_speeds(right, left);
	}
    break;
	case TEST_MOVE:
		test_move();
		break;
    case PLAY_SONG:
        play_song(function[1] - ASCII_OFFSET);
        break;
	}
}


/******************************************************************************
* Register Roomba device on SMRTControl
******************************************************************************/
void register_device(uint8_t* device_num_addr)
{
	/* Uninitialized flash memory is all 1*/
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
}


