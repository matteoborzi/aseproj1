/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "../timer/timer.h"
#include "../button_EXINT/button.h"
#include "../labyrinth/labyrinth.h"

// The RIT handler needs this variable to check which button has been pressed
int requested_button; 
uint8_t active_buttons = 0x0;

extern int game_started;
extern unsigned int distance;
extern unsigned int current_direction;
extern unsigned int next_direction;

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:	None
**
******************************************************************************/

void RIT_IRQHandler (void)
{			
	static int down0 = 0;
	static int down1 = 0;
	static int down2 = 0;
	int value = LPC_GPIO2->FIOPIN;
	
	// KEY1
	if( (active_buttons & 0x2) != 0){
		if((value & (1<<11)) == 0){
			down1 = (down1<2 ? down1+1 : 2);
			reset_RIT();
		}
		else {	/* button released */
			down1 = (down1>0 ? down1-1 : 0);
			if(down1==0){
				reset_RIT();
				disable_timer(2);
				disable_timer(1);
				reset_timer(2);
				reset_timer(1);
				LED_Off(5);
				LED_Off(3 - next_direction);	
				rotate();

				active_buttons &= ~(1<<1);
				NVIC_EnableIRQ(EINT1_IRQn);
				LPC_PINCON->PINSEL4    |= (1 << 22);		
			}
		}
	} 

	// KEY2
	if ( (active_buttons & 0x4) != 0) {
		if((value & (1<<12)) == 0){
			down2 = (down2<2 ? down2+1 : 2);
			reset_RIT();
			switch(down2){
				case 1:
					NVIC_EnableIRQ(TIMER1_IRQn);
					enable_timer(1);
					LED_On(5);
					enable_timer(2);
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down2 = (down2>0 ? down2-1 : 0);	
			if(down2==0){
				disable_timer(1);
				reset_timer(1);
				NVIC_DisableIRQ(TIMER1_IRQn);

				if(distance > 0){
					LED_Off(5);
					disable_timer(2);
					reset_timer(2);
				} else {
					NVIC_EnableIRQ(EINT1_IRQn);
				}
				
				reset_RIT();
				active_buttons &= ~(1<<2);
				NVIC_EnableIRQ(EINT2_IRQn);
				LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
			}
		}
	}
	
	// INT0
	if( (active_buttons & 0x1) != 0){
		if((value & (1<<10)) == 0){
			down0 = (down0<2 ? down0+1 : 2);
			reset_RIT();
			switch(down0){
				case 1:
					game_start();
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down0 = (down0>0 ? down0-1 : 0);
			if(down0 == 0){
				reset_RIT();
				if(game_started == 0) 
					NVIC_EnableIRQ(EINT0_IRQn);
				active_buttons &= ~(1<<0);
			}
		}
	}
	
	// When no button is pressed, RIT is disabled
	if(	active_buttons == 0){
		disable_RIT();
		reset_RIT();
	}
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
