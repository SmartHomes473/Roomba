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
#define ROI_SPECIFY_SONG 140
#define ROI_PLAY_SONG 141
#define ROI_FORCE_DOCK 143
#define ROI_DRIVE_DIRECT 145

#define DD_PIN BIT0

/* initiaize_roomba will start the mode as SAFE */
static roomba_mode_t current_mode = SAFE;


/******************************************************************************
* Internal function to activate the Device Detect line
******************************************************************************/
static void device_detect_on()
{
	P1OUT = DD_PIN;
}

/******************************************************************************
* Internal function to deactivate the Device Detect line
******************************************************************************/
static void device_detect_off()
{
	P1OUT = 0;
}

/******************************************************************************
* Internal function to initialize several songs
******************************************************************************/
static void write_songs()
{

    /* Zelda Lost Woods */
    uint8_t song1[35] =
        {ROI_SPECIFY_SONG, 1, 16, 65, 13, 69, 13, 71, 26, 65, 13, 69, 13, 71, 26, 65, 13, 69, 13, 71, 13, 76, 13, 74, 26, 71, 13, 72, 13, 71, 13, 67, 13, 64, 26};
    softwareUART_send_array(song1, 35);

    /* Mario Game Over */
    uint8_t song2[27] =
        {ROI_SPECIFY_SONG, 2, 12, 72, 29, 67, 29, 64, 19, 69, 13, 71, 13, 69, 13, 68, 13, 70, 13, 68, 13, 67, 5, 65, 5, 67, 29};
    softwareUART_send_array(song2, 27);

    /* Hall of the Mountain King */
    uint8_t song3[32] =
        {ROI_SPECIFY_SONG, 3, 13, 35+24, 14, 37+24, 14, 38+24, 14, 40+24, 14, 42+24, 14, 38+24, 14, 42+24, 28, 41+24, 14, 37+24, 14, 41+24, 28, 40+24, 14, 36+24, 14, 40+24, 28};
    softwareUART_send_array(song3, 32);
}

/******************************************************************************
* Initialize ROI on Roomba 
******************************************************************************/
void initialize_roomba()
{
	uint8_t i;

	/* Set Roomba to 19200 Baud by pulsing DD pin 3 times */
	delay_1s();
	delay_1s();

	for (i = 0; i < 3; i++)
	{
		device_detect_on();
		_delay_cycles(250000);
		device_detect_off();
		_delay_cycles(250000);
	}
	delay_20ms();

	/* Send initialization bytes */
	softwareUART_send_byte(ROI_START);
	delay_1s();
	softwareUART_send_byte(ROI_SAFE);
	delay_1s();

    write_songs();
}

/******************************************************************************
* Move Roomba forward for half second
******************************************************************************/
void test_move()
{
	// Go forward
	softwareUART_send_byte(137);
	softwareUART_send_byte(0x00);
	softwareUART_send_byte(0xC8);
	softwareUART_send_byte(0x80);
	softwareUART_send_byte(0x00);

	_delay_cycles(500000);

	// Stop
	softwareUART_send_byte(137);
	softwareUART_send_byte(0x00);
	softwareUART_send_byte(0x00);
	softwareUART_send_byte(0x00);
	softwareUART_send_byte(0x00);
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


/******************************************************************************
* Play the specified song
******************************************************************************/
void play_song(uint8_t song_number)
{
    uint8_t play_song[2] = {ROI_PLAY_SONG, song_number};
    softwareUART_send_array(play_song, 2);
}
