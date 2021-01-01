#include "button.h"
#include "lpc17xx.h"
#include "../RIT/RIT.h"

// This variable represents the button which has requested the RIT
// When the RIT handler is called, it will check the content of this variable
extern uint8_t active_buttons;

void EINT0_IRQHandler (void)	  						/* INT0														 */
{		
	active_buttons |= (1<<0);
	
	enable_RIT();															/* enable RIT to count 50ms				 */
	NVIC_DisableIRQ(EINT0_IRQn);							/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */

	LPC_SC->EXTINT &= (1 << 0);						    /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  						/* KEY1														 */
{
	active_buttons |= (1<<1);

	enable_RIT();															/* enable RIT to count 50ms				 */
	NVIC_DisableIRQ(EINT1_IRQn);							/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	
	LPC_SC->EXTINT &= (1 << 1);								/* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  						/* KEY2														 */
{
	active_buttons |= (1<<2);
	
	enable_RIT();															/* enable RIT to count 50ms				 */
	NVIC_DisableIRQ(EINT2_IRQn);							/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */

  LPC_SC->EXTINT &= (1 << 2);							  /* clear pending interrupt         */    
}
