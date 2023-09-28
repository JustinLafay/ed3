#include "LPC17xx.h"

void confIntGPIO(void); //Prototipo de la funci�n de conf. de interrupciones externas
void confGPIO(void); // Prototipo de la funcion de conf. de puertos
void EINT0_IRQHandler(void); //Funci�n de interrupci�n EINT1

uint8_t contador = 1;
uint8_t flag = 0;

int main(void) {
	confIntGPIO();
	confGPIO();
	SysTick->CTRL = 7;
	if (SysTick_Config(SystemCoreClock/200)){
			while (1); // AVISO DE ERROR: El programa llega hasta aqui si la funci�n devuelve un 1
		}
	while(1){
	}
	return 0;
}

void confIntGPIO(void){
	LPC_GPIOINT->IO0IntEnR |= (1<<0); //Selecciona Interrupci�n por flanco de subida.
	LPC_GPIOINT->IO0IntClr |= (1<<0); //Limpia la bandera de interrupci�n
	NVIC_EnableIRQ(EINT0_IRQn);    // Habilita de interrupciones externas EINT0_IRQn.
	return;
}

void confGPIO(void) {
	LPC_GPIO0->FIODIR &=~ (1<<0);
	LPC_GPIO2->FIODIR |= (0b11111111<<0);
	LPC_GPIO2->FIOMASK |= (0b111111111111111111111111<<8);
	return;
}

void EINT0_IRQHandler(void){
	contador++;
	NVIC_DisableIRQ(EINT0_IRQn);
	NVIC_EnableIRQ(SysTick_IRQn);
	return;
}

void SysTick_Handler(void){
	SysTick->CTRL &= SysTick->CTRL;
	if (LPC_GPIO0->FIOPIN0 &=~(1<<0)){
		flag++;
		if (flag == 3){
			LPC_GPIO2->FIOSET &= contador;
			LPC_GPIO2->FIOSET |= contador;
			flag = 0;
			NVIC_DisableIRQ(SysTick_IRQn);
			NVIC_EnableIRQ(EINT0_IRQn);
		}
	}
	return;
}
