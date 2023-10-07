Escribir un programa para que por cada presión de un pulsador, la frecuencia de parpadeo disminuya a la mitad debido a la modificación del registro del Match 0. El pulsador debe producir una interrupción por EINT2 con flanco descendente. Adjuntar el código en C.

```C
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
	LPC_PINCON->PINSEL4 |= (0b01<<24); //25:24 P2.12 (EINT2) OR
	LPC_PINCON->PINSEL4 &=~ (0b10<<24); //25:24 P2.12 (EINT2) AND
	LPC_SC->EXTINT |= 0xF; //limpiar bandera de interupcion
	LPC_SC->EXTMODE |= (1<<2); //edge-sensitive mode
	LPC_SC->EXTPOLAR &=~ (1<<2); //falling-edge sensitive
	NVIC_EnableIRQ(EINT2_IRQn); //activar la interupcion externa
	return;

}

void confTimer(void){
	LPC_SC->PCONP |= (1<<1); //activar el power en timer 0 p.65
	LPC_SC->PCLKSEL0 |= (0b01<<2); //CCLK = PCLK
	LPC_SC->PCLKSEL0 &=~ (0b10<<2); //CCLK = PCLK
	LPC_PINCON->PINSEL3 |= (3<<24); //P1.18 = MAT0.0
	LPC_TIM0->EMR |= (3<<4); //Toggle the corresponding External Match bit/output.
	LPC_TIM0->MR0 = 70000000; //Match value
	LPC_TIM0->MCR |= 3; // Interrupt, Reset on MR0, Stop on MR0 -> TCR = 0
	LPC_TIM0->TCR = 3; // Reset timer AND enable timer
	LPC_TIM0->TCR&=~ (1<<1);// remove reset timer
	return;

}

void EINT2_IRQHandler(void){
	LPC_TIM0->MR0 >>= 1;

}

void TIMER0_IRQHandler(void){
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