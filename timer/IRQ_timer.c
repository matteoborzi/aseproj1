/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../led/led.h"
#include "../labyrinth/labyrinth.h"

extern unsigned int distance;
extern unsigned int next_direction;

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER0_IRQHandler (void) {
	count_cycles++;
	
	if( count_cycles % 2 != 0 ) {
		LED_Off(3 - next_direction);
	} else {
		LED_On(3 - next_direction);	
	}
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void) {
	if( (LPC_GPIO2->FIOPIN & (1<<12)) == 0){		/*	if KEY2 is still pressed	*/
		if(distance == 0){
			disable_timer(1);
			NVIC_DisableIRQ(EINT2_IRQn);
		} else {
			run();
		}
	}
	
	LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler (void) {
	run_cycles++;

	if(run_cycles % 2 != 0)
		LED_Off(5);
	else
		LED_On(5);
	
	LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER3_IRQHandler (void) {
	run_cycles++;

	if(run_cycles % 2 != 0)
			LED_Out(0x2F);
		else 
			LED_Out(0x00);
	
	LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
