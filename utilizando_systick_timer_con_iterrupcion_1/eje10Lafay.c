#include "LPC17xx.h"

void confIntExt(void); //Prototipo de la funci�n de conf. de interrupciones externas
void confGPIO(void); // Prototipo de la funcion de conf. de puertos
void EINT1_IRQHandler(void); //Funci�n de interrupci�n EINT1
void EINT2_IRQHandler(void); //Funci�n de interrupci�n EINT2

uint8_t duty = 1;

int main(void) {
	confIntExt();
	confGPIO();
	if (SysTick_Config(SystemCoreClock/100)){
			while (1); // AVISO DE ERROR: El programa llega hasta aqui si la funci�n devuelve un 1
		}
	while(1){
	}
	return 0;
}

void confIntExt(void){
	LPC_PINCON->PINSEL4 |= (0b1<<20); //Selecciona funcionamiento del pin P2.11 y P2.12 como INT0 y INT2
	LPC_SC->EXTINT      |= 1;   //Limpia bandera de interrupci�n
	LPC_SC->EXTMODE     |= 0b1; //Selecciona interrupcion por flanco
	LPC_SC->EXTPOLAR    |= 0b1; //Interrumpe EINT0 cuando el flanco es de subida
	NVIC_EnableIRQ(EINT0_IRQn);    // Habilita de interrupciones externas EINT1_IRQn.
	NVIC_SetPriority(EINT0_IRQn, 1);
	NVIC_SetPriority(SysTick_IRQn, 0);
	return;
}

void confGPIO(void) {
	LPC_GPIO0->FIODIR |= (1<<22);
	return;
}

void EINT0_IRQHandler(void){

	if (duty < 10) {
		duty++;
	} else {
		duty = 1;
	}
	if (SysTick_Config(SystemCoreClock/(duty*100))){
			while (1); // AVISO DE ERROR: El programa llega hasta aqui si la funci�n devuelve un 1
		}
	LPC_SC->EXTINT |= 1;
	return;
}

void SysTick_Handler(void){
	SysTick->CTRL &= SysTick->CTRL;
	return;
}
