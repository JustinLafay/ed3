#include "LPC17xx.h"

void confGPIO(void); // Prototipo de la funcion de conf. de puertos
void confIntGPIO(void);
void confTimer(void);
int main(void) {

	confGPIO();
	confIntGPIO();
	confTimer();
	while(1){}
	return 0;
}

void confGPIO(void){
	LPC_GPIO0->FIODIR &=~ (1<<0);
	LPC_GPIO0->FIODIR |= (1<<1);
	return;
}

void confIntGPIO(void){
	LPC_GPIOINT->IO0IntEnF |= (1<<1);
	LPC_GPIOINT->IO0IntClr |= (1<<1);
	NVIC_EnableIRQ(EINT3_IRQn);
	return;
}


void confTimer(void){
	LPC_SC->PCONP       |= (1<<2);  // pag. 65 Power Control for Peripherals register (TIMER 1)
	LPC_SC->PCLKSEL0    |= (1<<4);  // pag. 59  Peripheral Clock Selection register GPIO
	LPC_PINCON->PINSEL3 |= (3<<11); // pag. 120 When 11 => MAT1.0
	LPC_TIM0->EMR       |=(3<<4);   // pag. 509 External Match Register EMC0 toggle
	LPC_TIM0->MR0        = 70000000;// match time
	LPC_TIM0->MCR       |= (1<<3);  // pag. 507 the TC will be reset if MR1 matches it
	LPC_TIM0->MCR       &= (1<<2);  // pag. 507 This interrupt is enabled (MR1)
	LPC_TIM0->TCR        = 3;       // pag. 505 Reset timer AND enable timer
	LPC_TIM0->TCR       &= ~(1<<1); // remove reset timer
	return;
}

void EINT3_IRQHandler(void){
	LPC_TIM0->MR0 >>= 1;
}

void TIMER1_IRQHandler(void) //ISR del timer0
{
static uint8_t i = 0;
	if (i==0){
		LPC_GPIO0->FIOSET = (1<<1);
		i = 1;
	}
	else if (i==1){
		LPC_GPIO0->FIOCLR = (1<<1);
		i = 0;
	}
	LPC_TIM0->IR|=1; //Limpia bandera de interrupci�n
	return;
}

