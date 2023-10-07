Utilizando el Timer 1, escribir un código en C para que por cada presión de un pulsador, la frecuencia de parpadeo de un led disminuya a la mitad debido a la modificación del reloj que llega al periférico. El pulsador debe producir una interrupción por GPIO0 con flanco descendente. Adjuntar el código en C.

```C
//Justin Lafay
#include "LPC17xx.h"

void confGPIO(void);
void confIntExt(void);
void confTimer(void);

int main(void){
	confGPIO();
	confIntExt();
	confTimer();
	while(1);
	return 0;

}

void confGPIO(void){
	LPC_GPIO0->FIODIR |= 1; //P0.0 como salida

}

void confIntExt(void){
	LPC_PINCON->PINSEL4 |= (0b01<<22); //23:22 P2.11 (EINT1) OR
	LPC_PINCON->PINSEL4 &=~ (0b10<<22); //23:22 P2.11 (EINT1) AND
	LPC_SC->EXTINT |= 0xF; //limpiar bandera de interupcion
	LPC_SC->EXTMODE |= (1<<1); //edge-sensitive mode
	LPC_SC->EXTPOLAR &=~ (1<<1); //falling-edge sensitive
	NVIC_EnableIRQ(EINT1_IRQn); //activar la interupcion externa
	return;

}

void confTimer(void){
	LPC_SC->PCONP |= (1<<22); //activar el power en timer 2 p.65
	LPC_SC->PCLKSEL1 |= (0b01<<12); //CCLK = PCLK TIMER 2
	LPC_SC->PCLKSEL1 &=~ (0b10<<12); //CCLK = PCLK TIMER 2
	LPC_PINCON->PINSEL0 |= (3<<12); //P0.6 = MAT2.0
	LPC_TIM2->EMR |= (3<<4); //Toggle the corresponding External Match bit/output.
	LPC_TIM2->MR0 = 70000000; //Match value
	LPC_TIM2->MCR |= 3; // Interrupt, Reset on MR0, Stop on MR0 -> TCR = 0
	LPC_TIM2->TCR = 3; // Reset timer AND enable timer
	LPC_TIM2->TCR&=~ (1<<1);// remove reset timer
	return;

}

void EINT1_IRQHandler(void){
	static uint8_t j = 0;
	LPC_SC->PCLKSEL1 &= (j<<12); //PCLK TIMER 2 AND
	if (j = 3) {
		j = 0;
	} else {
		j++;
	}

}

void TIMER2_IRQHandler(void){
	static uint8_t i = 1;
	if (i==1){
		i=0;
		LPC_GPIO0->FIOSET |= 1;
	} else {
		i=1;
		LPC_GPIO0->FIOCLR |= 1;
	}
	LPC_TIM0->IR |= 1;

}

```