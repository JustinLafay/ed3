Utilizando los 4 registros match del Timer 0 y las salidas P0.0, P0.1, P0.2 y P0.3, realizar un programa en C que permita obtener las formas de ondas adjuntas, donde los pulsos en alto tienen una duración de 5 mseg. Un pulsador conectado a la entrada EINT3, permitirá elegir entre las dos secuencias mediante una rutina de servicio a la interrupción. La prioridad de la interrupción del Timer tiene que ser mayor que la del pulsador. Estas formas de ondas son muy útiles para controlar un motor paso a paso. Adjuntar el código en C .

<span style="font-size: 24px;">Secuencia 1</span>

![Secuencia 1](secuencia1.png)

<span style="font-size: 24px;">Secuencia 2</span>

![Secuencia 2](secuencia2.png)


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
	LPC_GPIO0->FIODIR |= 0b1111; //P0.0,1,2,3,4 como salida

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
	LPC_TIM2->PR += 1;

}

void TIMER2_IRQHandler(void){
	for (uint8_t i=0; i++; i<4){
		if ((LPC_TIM2->EMR >> i) & 1){
			LPC_GPIO0->FIOSET |= (LPC_GPIO0->FIOPIN & (1<<i));
			LPC_GPIO0->FIOCLR |=~ (LPC_GPIO0->FIOPIN & (1<<i));
		}
	}

}

```