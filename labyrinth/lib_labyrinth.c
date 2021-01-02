#include "lpc17xx.h"
#include "labyrinth.h"
#include "../timer/timer.h"
#include "../led/led.h"

// Constants
//NORTH , EAST, SOUTH, WEST};
const int y_ver[4] = {	-1,	0,	1,	 0};
const int x_ver[4] = {	 0,	1,	0, 	-1};
const	uint32_t time[6] = {_8HZ_TIMER, _4HZ_TIMER, _4HZ_TIMER, _2HZ_TIMER, _2HZ_TIMER, _2HZ_TIMER};


// Local variables
unsigned int x = START_X;
unsigned int y = START_Y;

unsigned int distance = 0xFFFFFFFF;
unsigned int current_direction = NORTH;
unsigned int next_direction = NORTH;

void rotate (void){
	int i, j;
	uint8_t obstacle_found = 0;
	uint8_t exit_found = 0;
	
	next_direction = (next_direction + 1) % 4;	
	distance = 0;
	
	i = y;
	j = x;
	
	while(obstacle_found == 0 && exit_found == 0){
		distance++;
		i = i + y_ver[next_direction];
		j = j + x_ver[next_direction];

		if(map[i][j] == 1 || i < 0 || j < 0 || i >= LENGTH || j >= WIDTH){
			obstacle_found = 1;
		} else if(map[i][j] == 2){
			exit_found = 1;
			distance = 0xFFFFFFFF;
		}
	}
	
	distance--;
	
	if(distance > 5){
		NVIC_EnableIRQ(EINT2_IRQn);
		NVIC_DisableIRQ(TIMER0_IRQn);
		NVIC_EnableIRQ(TIMER1_IRQn);
		disable_timer(0);

		LED_On(3 - next_direction);
	} else {
		init_timer(0,time[distance]);
		enable_timer(0);
		if (distance == 0){
			NVIC_DisableIRQ(EINT2_IRQn);
			init_timer(2,_5HZ_TIMER);
			enable_timer(2);
		} else {
			NVIC_EnableIRQ(EINT2_IRQn);
			NVIC_EnableIRQ(TIMER1_IRQn);
		}
	}
}

void run (void) {
	current_direction = next_direction;
	
	x = x + x_ver[current_direction];
	y = y + y_ver[current_direction];
	
	if(map[y][x] == 2) {
		game_end();
	} else {
		distance--;

		if(distance > 5){
			NVIC_DisableIRQ(TIMER0_IRQn);
			disable_timer(0);
			LED_On(3 - next_direction);
		} else {
			init_timer(0,time[distance]);
			enable_timer(0);
			if (distance == 0){
				//NVIC_DisableIRQ(TIMER1_IRQn);
				//disable_timer(1);

				NVIC_DisableIRQ(EINT1_IRQn);
				//init_timer(2,_5HZ_TIMER);
				enable_timer(2);
			}
		}	
	}
	
}

/**
 *	This function disables the INT0 button interrupt and enables interrupts
 *	for buttons KEY1 and KEY2.
 *		-	KEY1 has priority 1 (rotation stops any running action)
 *		-	KEY2 has priority 2
 */
void game_start(void) {
	game_started = 1;
	
  LPC_PINCON->PINSEL4    |= (1 << 22);    /* External interrupt 0 pin selection */
  LPC_PINCON->PINSEL4    |= (1 << 24);    /* External interrupt 0 pin selection */

  LPC_SC->EXTMODE = 0x6;									/* Only EINT0 is disabled							*/
	
	// INT0 button is not needed any more
	NVIC_DisableIRQ(EINT0_IRQn);						/* Disable IRQ for EINT0 in nvic			*/

	// KEY1 button handles rotation
  NVIC_EnableIRQ(EINT1_IRQn);             /* enable irq in nvic                 */
	NVIC_SetPriority(EINT1_IRQn, 1);				 		

	// KEY2 button handles running
  NVIC_EnableIRQ(EINT2_IRQn);             /* enable irq in nvic                 */
	NVIC_SetPriority(EINT2_IRQn, 1);				/* priority, the lower the better     */
	
	// Timers initialization
	init_timer(1,STEP_NCYCLES);							/* Initialization of TIMER1 (1 sec)		*/
	init_timer(2,_5HZ_TIMER);						/* Initialization of TIMER2 (10 Hz)		*/
	disable_timer(0);
	disable_timer(1);
	disable_timer(2);
	
	// Reset and initialize LEDs
	LED_Out(0);
	rotate();
}


void game_end (void){
	game_started = 2;

	// Disable all interrupts
	NVIC_DisableIRQ(TIMER0_IRQn);
	NVIC_DisableIRQ(TIMER1_IRQn);
	NVIC_DisableIRQ(TIMER2_IRQn);
	NVIC_DisableIRQ(EINT0_IRQn);
	NVIC_DisableIRQ(EINT1_IRQn);
	NVIC_DisableIRQ(EINT2_IRQn);
	NVIC_DisableIRQ(RIT_IRQn);
	
	// All LEDs blink endlessly at 1 Hz
	init_timer(3,_1HZ_TIMER);
	enable_timer(3);	
}
