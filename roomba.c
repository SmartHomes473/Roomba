#include "roomba.h"
#include "msp430g2553_utils.h"


/* ROI Opcodes */
#define ROI_START 128
#define ROI_CONTROL 130
#define ROI_SAFE 131
#define ROI_FULL 132
#define ROI_POWER 133
#define ROI_SPOT 134
#define ROI_CLEAN 135
#define ROI_MAX 136
#define ROI_FORCE_DOCK 143

/* initiaize_roomba will start the mode as SAFE */
static roomba_mode_t current_mode = SAFE;

/******************************************************************************
* Initialize ROI on Roomba 
******************************************************************************/
void initialize_roomba()
{
	UART_send_byte(ROI_START);
	//delay 20milliseconds
	UART_send_byte(ROI_CONTROL);
}


/******************************************************************************
* Set Roomba's current mode
******************************************************************************/
void set_mode(roomba_mode_t mode)
{
	/* TODO: Account for DD line on waking from sleep */

	if (mode == current_mode) return;

	if (current_mode == PASSIVE) {
		if (mode == SAFE) {
			UART_send_byte(ROI_CONTROL);
		} else if (mode == FULL) {
			UART_send_byte(ROI_CONTROL);
			//delay 20 milliseconds
			UART_send_byte(ROI_FULL);
		}
	} else if (current_mode == SAFE) {
		if (mode == PASSIVE) {
			UART_send_byte(ROI_POWER);
		} else if (mode == FULL) {
			UART_send_byte(ROI_FULL);
		}
	} else if (current_mode == FULL) {
		if (mode == SAFE) {
			UART_send_byte(ROI_SAFE);
		} else if (mode == PASSIVE) {
			UART_send_byte(ROI_POWER);
		}
	}
	current_mode = mode;
}


/******************************************************************************
* Start the normal cleaning cycle
******************************************************************************/
void start_clean(clean_mode_t mode)
{
    if (current_mode != PASSIVE) {
        switch(mode) {
            case DEFAULT:
                UART_send_byte(ROI_CLEAN);
                break;
            case SPOT:
                UART_send_byte(ROI_START);
                break;
            case MAX:
                UART_send_byte(ROI_MAX);
                break;
        }
    }
}

/******************************************************************************
* Send Roomba back to the dock
******************************************************************************/
void send_dock()
{
    UART_send_byte(ROI_FORCE_DOCK);
}



