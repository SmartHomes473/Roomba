/**
 * This header describes the external API interface for the Roomba
 */

typedef enum 
{
    OFF,
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

typedef enum
{
    SUNDAY,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY
} day_t;

/*
 * Initalizes the ROI interface.
 * This causes the current mode to be changed to safe. 
 */
void initialize_roomba();

/* 
 * Sets the Roomba into Passive, Safe, or Full mode. 
 * The current mode restricts the operations available to the Roomba.
 * See ROI spec for details
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
 * This changes the current mode to passive.
 */
void send_dock();

/* 
 * Sets a schedule to run the Roomba. 
 * Takes an array of days, with corresponds hours/minutes to run.
 * Hours are in 24 hours format, from 0-23. 
 */
void set_schedule(day_t* day, int* hour, int* minute);

/*
 * Set the current day and time.
 */
void set_time(day_t day, int hour, int minute);

/*
 * Power down the Roomba
 */
void power_down();
