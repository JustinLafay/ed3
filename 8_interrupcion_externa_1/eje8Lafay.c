#include "LPC17xx.h"

void confIntExt(void); //Prototipo de la funci�n de conf. de interrupciones externas
void EINT1_IRQHandler(void); //Funci�n de interrupci�n EINT1
void EINT2_IRQHandler(void); //Funci�n de interrupci�n EINT2

int main(void) {
	confIntExt();
	while(1){
	}
	return 0;
}

void confIntExt(void){
	LPC_PINCON->PINSEL4 |= (0b110<<20); //Selecciona funcionamiento del pin P2.11 y P2.12 como INT0 y INT2
	LPC_SC->EXTINT      |= 1;   //Limpia bandera de interrupci�n
	LPC_SC->EXTMODE     |= 0b110; //Selecciona interrupcion por flanco
	LPC_SC->EXTPOLAR    &= 0b100; //Interrumpe EINT1 cuando el flanco es de bajada
	LPC_SC->EXTPOLAR    |= 0b100; //Interrumpe EINT2 cuando el flanco es de subida
	NVIC_EnableIRQ(EINT1_IRQn);    // Habilita de interrupciones externas EINT1_IRQn.
	NVIC_EnableIRQ(EINT2_IRQn);    // Habilita de interrupciones externas EINT2_IRQn.
	NVIC_SetPriority(EINT1_IRQn, 0);
	NVIC_SetPriority(EINT2_IRQn, 1);
	return;
}
void EINT2_IRQHandler(void){
	if (SysTick_Config(SystemCoreClock/250)){
		while (1); // AVISO DE ERROR: El programa llega hasta aqui si la funci�n devuelve un 1
	}
	return;
}
void EINT1_IRQHandler(void){
	if (SysTick_Config(SystemCoreClock/600)){
		while (1); // AVISO DE ERROR: El programa llega hasta aqui si la funci�n devuelve un 1
	}
	return;
}
void SysTick_Handler(void){
	SysTick->CTRL &= SysTick->CTRL;
	return;
}
