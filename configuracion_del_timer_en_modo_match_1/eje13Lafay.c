#include "LPC17xx.h"

void confTimer(void);
int main(void) {

	confTimer();
	while(1){}
	return 0;
}

void confTimer(void){
	LPC_SC->PCONP       |= (1<<1);  // pag. 65
	LPC_SC->PCLKSEL0    |= (2<<2);  // pag. 59
	LPC_PINCON->PINSEL3 |= (3<<24); // pag. 120
	LPC_TIM0->EMR       |=(3<<4);   // pag. 509
	LPC_TIM0->PR		= 2;        // Set PR to 2
	LPC_TIM0->MR0       = 6;        // Set MR0 to 6
	LPC_TIM0->MCR       |= (1<<1);  // pag. 507
	LPC_TIM0->MCR       &= ~(1<<0); // pag. 507
	LPC_TIM0->TCR        = 3;       // pag. 505
	LPC_TIM0->TCR       &= ~(1<<1);
	// NVIC_EnableIRQ(TIMER0_IRQn);
	return;
}
