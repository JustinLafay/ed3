#include "LPC17xx.h"

void confGPIO(void); // Prototipo de la funci�n de conf. de puertos
void confIntGPIO(void); //Prototipo de la funci�n de conf. de interrupciones por GPIO

int main(void) {

	confGPIO();
	confIntGPIO();

	while(1){
	}
	return 0;
}

void confGPIO(void){
	LPC_GPIO0->FIODIR |= (1<<1);
	LPC_GPIO2->FIODIR |= (1<<1);
	return;
}
void confIntGPIO(void){
	LPC_GPIOINT->IO0IntEnR |= (1<<0);
	LPC_GPIOINT->IO0IntClr |= (1<<0);
	LPC_GPIOINT->IO2IntEnR |= (1<<0);
	LPC_GPIOINT->IO2IntClr |= (1<<0);
	NVIC_EnableIRQ(EINT3_IRQn);
	return;
}
void EINT3_IRQHandler(void)
{
	if(LPC_GPIOINT->IO0IntStatR & (1<<0)){
		LPC_GPIO0->FIOPIN0 ^= (0b1<<0);
		LPC_GPIOINT->IO0IntClr |= (1<<0);
		LPC_GPIO0->FIOPIN0 |= (1<<22);
	}
	else if (LPC_GPIOINT->IO2IntStatR & (1<<0)){
		LPC_GPIO2->FIOPIN2 ^= (0b1<<0);
		LPC_GPIOINT->IO2IntClr |= (1<<0);
	}
	return;
}
