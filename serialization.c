#include "serialization.h"
#include "roomba.h"


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
	}
}


