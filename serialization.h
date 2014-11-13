/*
 * This header provides the interface for serializing/deserializing the Roomba function calls
 */
#include <stdint.h>

typedef enum
{
	INITIALIZE_ROOMBA	= '0',
	SET_MODE		  	= '1',
	START_CLEAN			= '2',
	SEND_DOCK			= '3',
	SET_WHEEL_SPEEDS	= '4',
	TEST_MOVE			= '5',
    PLAY_SONG           = '6'
} roomba_functions_t;

/* '0' is ASCII is 48 */
#define ASCII_OFFSET 48

/*
 * This function takes the serialized function and calls the appropriate Roomba interface function
 */
void deserialize(uint8_t* function);
