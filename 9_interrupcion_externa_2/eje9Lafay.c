#include "LPC17xx.h"

void confIntExt(void); //Prototipo de la funci�n de conf. de interrupciones externas
void confGPIO(void); // Prototipo de la funcion de conf. de puertos
void EINT1_IRQHandler(void); //Funci�n de interrupci�n EINT1
void EINT2_IRQHandler(void); //Funci�n de interrupci�n EINT2
void retardo(uint32_t tiempo);

uint32_t tiempo = 100000;
uint8_t seq[2] = {0b1100001, 0b1000001};

int main(void) {
	confIntExt();
	confGPIO();
	while(1){
	}
	return 0;
}

void confIntExt(void){
	LPC_PINCON->PINSEL4 |= (0b11<<20); //Selecciona funcionamiento del pin P2.10 y P2.11 como INT0 y INT1
	LPC_SC->EXTINT      |= 1;   //Limpia bandera de interrupci�n
	LPC_SC->EXTMODE     |= 0b10; //INT0 de nivle y INT1 de flanco
	LPC_SC->EXTPOLAR    |= 0b11; //Interrumpe EINT2 cuando el flanco es de subida
	NVIC_EnableIRQ(EINT0_IRQn);    // Habilita de interrupciones externas EINT1_IRQn.
	NVIC_EnableIRQ(EINT1_IRQn);    // Habilita de interrupciones externas EINT2_IRQn.
	NVIC_SetPriority(EINT0_IRQn, 0);
	NVIC_SetPriority(EINT1_IRQn, 1);
	return;
}

void confGPIO(void){
	LPC_GPIO1->FIODIRH |= (0b1111111<<0);
	return;
}

void EINT0_IRQHandler(void){
	if (NVIC_GetActive(EINT1_IRQn)){
		LPC_GPIO1->FIOSETH &= (seq[1]<<0);
		LPC_GPIO1->FIOSETH |= (seq[1]<<0);
		retardo(tiempo);
		LPC_GPIO1->FIOCLRH |= (0b1111111<<0);
	}
	else {
		LPC_GPIO1->FIOSETH &= (seq[0]<<0);
		LPC_GPIO1->FIOSETH |= (seq[0]<<0);
		retardo(tiempo);
		LPC_GPIO1->FIOCLRH |= (0b1111111<<0);
	}
	LPC_SC->EXTINT |= 1;
	return;
}

void retardo (uint32_t tiempo){
	for(uint32_t conta = 0; conta<tiempo;conta++){}
	return;
}
