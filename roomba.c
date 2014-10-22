#include "roomba.h"
#include "msp430g2553_utils.h"
#include "software_uart.h"


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
#define ROI_DRIVE_DIRECT 145

/* initiaize_roomba will start the mode as SAFE */
static roomba_mode_t current_mode = SAFE;


/******************************************************************************
* Internal function to activate the Device Detect line
******************************************************************************/
static void device_detect_on()
{
	P1OUT = 0x0;
}

/******************************************************************************
* Internal function to deactivate the Device Detect line
******************************************************************************/
static void device_detect_off()
{
	P1OUT = 0x1;
}

/******************************************************************************
* Initialize ROI on Roomba 
******************************************************************************/
void initialize_roomba()
{
	softwareUART_send_byte(ROI_START);
	delay_20ms();

	softwareUART_send_byte(ROI_CONTROL);
	delay_20ms();
}


/******************************************************************************
* Set Roomba's current mode
******************************************************************************/
void set_mode(roomba_mode_t mode)
{
	if (mode == current_mode) return;

	switch (mode) {
		case SLEEP:
			softwareUART_send_byte(ROI_POWER);
			break;
		case PASSIVE:
			if (current_mode == SLEEP) {
				device_detect_on();
				delay_500ms();
				device_detect_off();
			} else {
				softwareUART_send_byte(ROI_START);
			}
			break;
		case SAFE:
			if (current_mode == FULL) {
				softwareUART_send_byte(ROI_SAFE);
			} else if (current_mode == PASSIVE) {
				softwareUART_send_byte(ROI_CONTROL);
			}
			break;	
		case FULL:
			softwareUART_send_byte(ROI_FULL);
			break;
	}
	/* Required delay after changing modes */
	delay_20ms();	
	current_mode = mode;
}


/******************************************************************************
* Start the normal cleaning cycle
******************************************************************************/
void start_clean(clean_mode_t mode)
{
    if (current_mode != SLEEP) {
        switch(mode) {
            case DEFAULT:
                softwareUART_send_byte(ROI_CLEAN);
                break;
            case SPOT:
                softwareUART_send_byte(ROI_SPOT);
                break;
            case MAX:
                softwareUART_send_byte(ROI_MAX);
                break;
        }
    }
}

/******************************************************************************
* Send Roomba back to the dock
******************************************************************************/
void send_dock()
{
    softwareUART_send_byte(ROI_FORCE_DOCK);
}

/******************************************************************************
* Set the wheel speeds of the Roomba
******************************************************************************/
void set_wheel_speeds(int16_t left_wheel, int16_t right_wheel)
{
	if (current_mode == SAFE || current_mode == FULL) {
		uint8_t drive_direct[5];
		drive_direct[0] = ROI_DRIVE_DIRECT;

		/* Send high byte right, low byte right, high byte left, low byte left */
		drive_direct[1] = (right_wheel >> 8) & 0xFF;
		drive_direct[2] = right_wheel & 0xFF;
		drive_direct[3] = (left_wheel >> 8) & 0xFF;
		drive_direct[4] = left_wheel & 0xFF;

		softwareUART_send_array(drive_direct, 5);
	}
}


