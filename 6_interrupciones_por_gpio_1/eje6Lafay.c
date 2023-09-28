#include "LPC17xx.h"

void confGPIO(void); // Prototipo de la funci�n de conf. de puertos
void confIntGPIO(void); //Prototipo de la funci�n de conf. de interrupciones por GPIO
void retardo(uint32_t tiempo);

uint8_t inte = 0;
uint32_t tiempo = 100000;
int sequencias[2] = {0b010011010, 0b011100110};

int main(void) {

	confGPIO();
	confIntGPIO();

	while(1){
		LPC_GPIO0->FIOSET = (0b11<<0);
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
	LPC_GPIOINT->IO2IntEnR |= (0b11<<0); //Selecciona Interrupci�n por flanco de subida.
	LPC_GPIOINT->IO2IntClr |= (0b11<<0); //Limpia la bandera de interrupci�n
	NVIC_EnableIRQ(EINT3_IRQn);        // Habilita de interrupciones externas.
	return;
}
void EINT3_IRQHandler(uint32_t tiempo)
{
	if(LPC_GPIOINT->IO2IntStatR & (1<<0)){ //Pregunta si el pin P0.15 produjo la interrupci�n
		for (int i; i<9; i++){
			LPC_GPIO0->FIOSET = ((sequencias[0]&(0b1>>(9-i)))>>(9-i));
			retardo(tiempo);
			LPC_GPIO0->FIOCLR = (0b1<<0);
			retardo(tiempo);
		}
	}
	else if (LPC_GPIOINT->IO2IntStatR & (1<<1)){
		for (int i; i<9; i++){
			LPC_GPIO0->FIOSET = ((sequencias[1]&(0b1>>(9-i)))>>(9-i-1));
			retardo(tiempo);
			LPC_GPIO0->FIOCLR = (0b1<<1);
			retardo(tiempo);
		}
	}
	return;
}
