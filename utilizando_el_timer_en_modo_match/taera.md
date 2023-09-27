Utilizando el Timer 1, escribir un código en C para que por cada presión de un pulsador, la frecuencia de parpadeo de un led disminuya a la mitad debido a la modificación del reloj que llega al periférico. El pulsador debe producir una interrupción por GPIO0 con flanco descendente. Adjuntar el código en C.

```C
#include "LPC17xx.h"

void confGPIO(void); // Prototipo de la funcion de conf. de puertos
void confIntGPIO(void);
void confTimer(void);
int main(void) {

	confGPIO();
	confIntGPIO();
	confTimer();
	while(1){}
	return 0;
}

void confGPIO(void){
	LPC_GPIO0->FIODIR &=~ (1<<0);
	LPC_GPIO0->FIODIR |= (1<<1);
	return;
}

void confIntGPIO(void){
	LPC_GPIOINT->IO0IntEnF |= (1<<1);
	LPC_GPIOINT->IO0IntClr |= (1<<1);
	NVIC_EnableIRQ(EINT3_IRQn);
	return;
}


void confTimer(void){
	LPC_SC->PCONP       |= (1<<1);  // pag. 65
	LPC_SC->PCLKSEL0    |= (1<<2);  // pag. 59
	LPC_PINCON->PINSEL3 |= (3<<24); // pag. 120
	LPC_TIM0->EMR       |=(3<<4);   // pag. 509
	LPC_TIM0->MR0        = 70000000;//
	LPC_TIM0->MCR       |= (1<<1);       // pag. 507
	LPC_TIM0->MCR       &= (1<<0);       // pag. 507
	LPC_TIM0->TCR        = 3;       // pag. 505
	LPC_TIM0->TCR       &= ~(1<<1);
	// NVIC_EnableIRQ(TIMER0_IRQn);
	return;
}

void EINT3_IRQHandler(void){

}

void TIMER0_IRQHandler(void) //ISR del timer0
{
static uint8_t i = 0;
	if (i==0){
		LPC_GPIO0->FIOSET = (1<<1);
		i = 1;
	}
	else if (i==1){
		LPC_GPIO0->FIOCLR = (1<<1);
		i = 0;
	}
	LPC_TIM0->IR|=1; //Limpia bandera de interrupci�n
	return;
}


```