#include "LPC17xx.h"

void confIntExt(void); //Prototipo de la funci�n de conf. de interrupciones externas
void confTimer(void);
void confGPIO(void);
void EINT1_IRQHandler(void); //Funci�n de interrupci�n EINT1

int main(void) {
	confGPIO();
	confIntExt();
	confTimer();
	while(1){}
	return 0;
}

void confIntExt(void){
	LPC_PINCON->PINSEL3 |= (0b01<<22); //P2.11
	LPC_PINCON->PINMODE4 &=~ (0b11<22);
	LPC_SC->EXTINT      |= 1;   //Limpia bandera de interrupci�n
	LPC_SC->EXTMODE     |= 1; //Selecciona interrupcion por flanco
	LPC_SC->EXTPOLAR    &= 0<<1; //Interrumpe EINT1 cuando el flanco es de bajada
	NVIC_EnableIRQ(EINT1_IRQn);    // Habilita de interrupciones externas EINT1_IRQn.
	return;
}

void confGPIO(void){
	LPC_GPIO0->FIODIR |= (1<<1);
}

void confTimer(void){
	LPC_SC->PCONP       |= (1<<22);  // pag. 65 Power Control for Peripherals register (TIMER 2)
	LPC_SC->PCLKSEL0    |= (1<<11);  // pag. 59  Peripheral Clock Selection register TIMER1
	LPC_PINCON->PINSEL3 |= (3<<11); // pag. 120 When 11 => MAT1.0
	LPC_TIM2->EMR       |=(3<<4);   // pag. 509 External Match Register EMC0 toggle
	LPC_TIM2->MR0        = 70000000;// match time
	LPC_TIM2->MCR       |= (3<<0);  // pag. 507 the TC will be reset if MR0 matches it
	LPC_TIM2->TCR        = 3;       // pag. 505 Reset timer AND enable timer
	LPC_TIM2->TCR       &= ~(1<<1); // remove reset timer
	return;
}

void EINT1_IRQHandler(void){
	LPC_TIM2->MR0 >>= 1;
}

void TIMER2_IRQHandler(void) //ISR del timer2
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
	LPC_TIM2->IR|=1; //Limpia bandera de interrupci�n
	return;
}

