1- Dada una señal de 100khz de componente espectral máxima que ingresa por un pin del ADC, se necesita procesar.  El rango dinámico de amplitud es de 3.3v.
Proponer otra señal de frecuencia definida para que samplee a la señal conectada al ADC y poder reconstruirla. 300 samples.
Escriba el programa de la manera más conveniente. Definir si burst o software, tiempos de sampleo, tipo de variables.

```C
#include "LPC17xx.h"

void confADC(void); //Prototipo de la funci�n de conf. de interrupciones externas
void confTimer(void);
void retardo(void);

int main(void) {
	confTimer();
	confADC();
	while(1){
		LPC_GPIO0->FIOSET = (1<<22);
		retardo();
		LPC_GPIO0->FIOCLR = (1<<22);
		retardo();
	}
	return 0;
}

void retardo (void){
	uint32_t conta;
	for(conta = 0; conta<1000000;conta++){}
	return;
}

void confADC(void){
	LPC_SC->PCONP |= (1 << 12);
	LPC_ADC->ADCR |= (1 << 21);    //enable ADC
	LPC_SC->PCLKSEL0 |= (0b01<<24);  //CCLK
	LPC_ADC->ADCR &=~(255 << 8);  //[15:8] CLKDIV
	//LPC_ADC->ADCR |= (1 << 0);   // channel
	LPC_ADC->ADCR &= ~(1 << 16);   // no burst
	LPC_ADC->ADCR |= (1 << 26);    // match
	LPC_ADC->ADCR &= ~(3 << 24);   // match
	LPC_ADC->ADCR |= (1 << 27);    //edge
	//LPC_PINCON->PINSEL4 |= (1<<20);
	LPC_PINCON->PINMODE1 |= (1<<15); //neither pull-up nor pull-down.
	LPC_PINCON->PINSEL1 |= (1<<14);
	LPC_ADC->ADINTEN |= (1<<0);
	NVIC_EnableIRQ(ADC_IRQn);
	return;
}

void confTimer(void){
	LPC_SC->PCONP |= (1 << 1); //Por defecto timer 0 y 1 estan siempre prendidos
	LPC_SC->PCLKSEL0 |= (1<<2); // configuraci�n del clock del perif�rico
	//LPC_PINCON->PINSEL3 |= (3<<24);
	LPC_TIM0->EMR|=(3<<6); //toggle
	LPC_TIM0->MR1 = 666663;
	LPC_TIM0->MCR = (1<<4); //reset on match
	LPC_TIM0->TCR = 3;      // hab y reset
	LPC_TIM0->TCR &= ~(1<<1);
	return;
}


void ADC_IRQHandler(void)
{
	static uint16_t ADC0Value = 0;
	ADC0Value = ((LPC_ADC->ADDR0)>>4) & 0xFFF; //Variable auxiliar para observar el valor del registro de captura
	return;
}

```