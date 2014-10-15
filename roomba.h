/**
 * This header describes the external API interface for the Roomba
 */

typedef enum 
{
	SLEEP,
    PASSIVE,
    SAFE,
    FULL
} roomba_mode_t;

typedef enum
{
    DEFAULT,
    SPOT,
    MAX
} clean_mode_t;

/*
 * Initalizes the ROI interface.
 * This causes the current mode to be changed to safe. 
 */
void initialize_roomba();

/* 
 * Sets the Roomba into Passive, Safe, or Full mode. 
 * The current mode restricts the operations available to the Roomba.
 * See ROI spec for details.
 * Possible mode transitions: 
 *		PASSIVE -> SAFE
 *		FULL	-> SAFE
 *		SAFE	-> FULL
 *		SAFE	-> SLEEP
 *		FULL	-> SLEEP
 *		SLEEP	-> PASSIVE
 */
void set_mode(roomba_mode_t mode);

/* 
 * Starts the Roomba's clean cycle. The clean cycle can be one of Default 
 * Spot, or Max.
 * See ROI spec for details on the different cleaning modes.
 * This causes the current mode to be changed to passive.
 */
void start_clean(clean_mode_t mode);

/*
 * Starts the Roomba's return to docking station algorithm.
 * Only works if a cleaning mode currently active.
 * This changes the current mode to passive.
 */
void send_dock();

/*
 * Power down the Roomba
 */
void power_down();
