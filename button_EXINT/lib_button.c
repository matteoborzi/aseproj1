#include "button.h"
#include "lpc17xx.h"

/**
 * @brief  Function that initializes Buttons
 */
void BUTTON_init(void) {

  LPC_PINCON->PINSEL4    |= (1 << 20);		/* External interrupt 0 pin selection */
  LPC_PINCON->PINSEL4    &= ~(1 << 22);    /* External interrupt 0 pin selection */
	LPC_PINCON->PINSEL4    &= ~(1 << 24);    /* External interrupt 0 pin selection */

  LPC_GPIO2->FIODIR      &= ~(1 << 10);   /* PORT2.10 defined as input          */
  LPC_GPIO2->FIODIR      &= ~(1 << 11);   /* PORT2.11 defined as input          */
  LPC_GPIO2->FIODIR      &= ~(1 << 12);   /* PORT2.12 defined as input          */

  LPC_SC->EXTMODE = 0x1;									/* Only EINT0 is enabled							*/

  NVIC_EnableIRQ(EINT0_IRQn);             /* enable irq in nvic                 */
	NVIC_SetPriority(EINT0_IRQn, 1);				/* decreasing priority	from EINT2->0	*/
}
