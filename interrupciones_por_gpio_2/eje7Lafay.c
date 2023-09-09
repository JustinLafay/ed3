#include "LPC17xx.h"

void confGPIO(void); // Prototipo de la funci�n de conf. de puertos
void confIntGPIO(void); //Prototipo de la funci�n de conf. de interrupciones por GPIO
void retardo(uint32_t tiempo);

uint16_t auxiliar = 0b0;

int main(void) {

	confGPIO();
	confIntGPIO();

	while(1){
	}
	return 0;
}
void retardo (uint32_t tiempo){
	for(uint32_t conta = 0; conta<tiempo;conta++){}
	return;
}
void confGPIO(void){
	LPC_GPIO0->FIODIR0 |= (0b11<<0);
	LPC_GPIO2->FIODIR0 &=~ (0b11<<0);
	return;
}
void confIntGPIO(void){
	LPC_GPIOINT->IO0IntEnR |= (0b1<<0); //Selecciona Interrupci�n por flanco de subida.
	LPC_GPIOINT->IO0IntClr |= (0b1<<0); //Limpia la bandera de interrupci�n
	LPC_GPIOINT->IO2IntEnR |= (0b1<<0); //Selecciona Interrupci�n por flanco de subida.
	LPC_GPIOINT->IO2IntClr |= (0b1<<0); //Limpia la bandera de interrupci�n
	NVIC_EnableIRQ(EINT3_IRQn);        // Habilita de interrupciones externas.
	return;
}
void EINT3_IRQHandler(uint32_t tiempo)
{
	if(LPC_GPIOINT->IO2IntStatR & (1<<0)){
		auxiliar = 0b111001011010110;
		LPC_GPIOINT->IO2IntClr |= (0b1<<0);
	}
	else if (LPC_GPIOINT->IO0IntStatR & (1<<0)){
		auxiliar = 0b100111;
		LPC_GPIOINT->IO0IntClr |= (0b1<<0);
	}
	return;
}
