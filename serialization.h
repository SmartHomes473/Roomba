/*
 * This header provides the interface for serializing/deserializing the Roomba function calls
 */
#include <stdint.h>

typedef enum
{
	INITIALIZE_ROOMBA,
	SET_MODE,
	START_CLEAN,
	SEND_DOCK,
	SET_WHEEL_SPEEDS
} roomba_functions_t;

/*
 * This function takes the serialized function and calls the appropriate Roomba interface function
 */
void deserialize(uint8_t* function);
