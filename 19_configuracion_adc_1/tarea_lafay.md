A partir del código ejemplo visto en clase:

```C
#include "LPC17xx.h"
void confGPIO(void);
void confADC(void);

uint16_t ADC0Value = 0;

int main(void) {
confGPIO();
confADC();
while(1){}
return 0;
}

void confGPIO(void){
LPC_GPIO0->FIODIR |= (1<<22);
return;
}

void confADC(void){
LPC_SC->PCONP |= (1 << 12);
LPC_ADC->ADCR |= (1 << 21); //habilita el ADC
LPC_SC->PCLKSEL0 |= (3<<24);  //CCLK/8
LPC_ADC->ADCR &=~(255 << 8);  //[15:8] CLKDIV
//LPC_ADC->ADCR |= (1 << 0);   // channel
LPC_ADC->ADCR |= (1 << 16);   // burst
LPC_PINCON->PINMODE1 |= (1<<15); //neither pull-up nor pull-down.
LPC_PINCON->PINSEL1 |= (1<<14);
LPC_ADC->ADINTEN |= (1<<0);
LPC_ADC->ADINTEN &=~ (1<<8);
NVIC_EnableIRQ(ADC_IRQn);

return;
}
void ADC_IRQHandler(void)
{
ADC0Value = ((LPC_ADC->ADDR0)>>4) & 0xFFF; //Variable auxiliar para observar el valor del registro de captura

if (ADC0Value<2054){
LPC_GPIO0->FIOSET = (1<<22);
}
else{
LPC_GPIO0->FIOCLR = (1<<22);
}

return;
}
```

1.- Determine la frecuencia de trabajo y de muestreo del ADC suponiendo una frecuencia de cclk de 100 Mhz. ¿En que modo se encuentra trabajando el ADC? 

2.- Modifique el ejemplo para considerar la utilización de dos canales de conversión. Adjuntar el código en C. ¿A que frecuencia se encuentra muestreando cada canal?

```C
//Justin Lafay

/*
 * 1.- Determine la frecuencia de trabajo y de muestreo
 * del ADC suponiendo una frecuencia de cclk de 100 Mhz.
 * ¿En que modo se encuentra trabajando el ADC?
 *
 * Frecuencia de trabajo : CCLK/8 = 100*10^6/8 = 12,5Mhz
 * Frecuencia de muestreo : "12-bit conversion rate of 200 kHz."
 * Modo de trabajo : Burst mode
 *
 * 2.- Modifique el ejemplo para considerar la utilización de dos
 * canales de conversión. Adjuntar el código en C. ¿A que frecuencia
 * se encuentra muestreando cada canal?
 * 
 * Cada canal se encuentra muestreando a la mitad de 200kHz (100kHz)
 *
 * */


#include "LPC17xx.h"
void confGPIO(void);
void confADC(void);

uint16_t ADC0Value = 0;

int main(void) {
	confGPIO();
	confADC();
	while (1) {
	}
	return 0;
}

void confGPIO(void) {
	LPC_GPIO0->FIODIR |= (3 << 22);
	return;
}

void confADC(void) {
	LPC_SC->PCONP |= (1 << 12);
	LPC_ADC->ADCR |= (1 << 21); //habilita el ADC
	LPC_SC->PCLKSEL0 |= (3 << 24);  //CCLK/8
	LPC_ADC->ADCR &= ~(255 << 8);  //[15:8] CLKDIV
	LPC_ADC->ADCR |= (3 << 0);   // channel 0 and 1
	LPC_ADC->ADCR |= (1 << 16);   // burst
	LPC_PINCON->PINMODE1 |= (0b101 << 15); //neither pull-up nor pull-down.
	LPC_PINCON->PINSEL1 |= (3 << 14);
	LPC_ADC->ADINTEN |= (3 << 0);
	LPC_ADC->ADINTEN &= ~(1 << 8);
	NVIC_EnableIRQ(ADC_IRQn);

	return;
}
void ADC_IRQHandler(void) {
	ADC0ValueChannel0 = ((LPC_ADC->ADDR0) >> 4) & 0xFFF; //Variable auxiliar para observar el valor del registro de captura
	ADC0ValueChannel1 = ((LPC_ADC->ADDR1) >> 4) & 0xFFF;

	if (ADC0ValueChannel0 < 2054) {
		LPC_GPIO0->FIOSET = (1 << 22);
	} else {
		LPC_GPIO0->FIOCLR = (1 << 22);
	}

	if (ADC0ValueChannel1 < 3000) {
		LPC_GPIO0->FIOSET = (1 << 23);
	} else {
		LPC_GPIO0->FIOCLR = (1 << 23);
	}

	return;
}

```