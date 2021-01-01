/*----------------------------------------------------------------------------
	Here the methods and the functions that modify the status of the game
	are defined, with constants like the map matrix and its dimensions
 *----------------------------------------------------------------------------*/

#define	LENGTH				13
#define	WIDTH 				15
#define	START_X				2
#define	START_Y				2

// Timer cycles
#define _8HZ_TIMER			0x17D784
#define	_4HZ_TIMER			0x2FAF08
#define _2HZ_TIMER			0x5F5E10
#define	RLED_FREQUENCY	0x2625A0	// N = 2	500	000 cycles @ 25 MHz (10 Hz)
#define STEP_NCYCLES		0x17D7840	// N = 25	000	000 cycles @ 25 MHz
	
enum direction { NORTH = 0, EAST, SOUTH, WEST};

// Global variables
extern int game_started;
extern const char map[LENGTH][WIDTH];

// Game methods
void game_start(void);
void game_end(void);

void rotate(void);
void run(void);
