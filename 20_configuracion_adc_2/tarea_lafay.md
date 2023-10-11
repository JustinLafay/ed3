Configurar 4 canales del ADC para que funcionando en modo burst se obtenga una frecuencia de muestreo en cada uno de 50Kmuestras/seg. Suponer un cclk = 100 Mhz

```C
// Justin Lafay
#include "LPC17xx.h";


void confADC(void);

int main(void){
	confADC();
	while(1){}
}

void confADC(void){
	LPC_SC->PCONP |= (1 << 12); // power ADC
	LPC_ADC->ADCR |= (1 << 21); // enable ADC
	LPC_SC->PCLKSEL0 |= (3 << 24); // clock ADC CCLK/8
	LPC_ADC->ADCR &= ~(255 << 8);  //[15:8] CLKDIV
	LPC_ADC->ADCR |= (15 << 0);   // channel 0, 1, 2, 3 ,4
	LPC_ADC->ADCR |= (1 << 16);   // burst
	LPC_PINCON->PINMODE1 |= (0b10101010 << 14); //neither pull-up nor pull-down. OR
	LPC_PINCON->PINMODE1 &=~ (0b01010101 << 14); //neither pull-up nor pull-down. AND	LPC_PINCON->PINSEL1 |= (3 << 14);
	LPC_ADC->ADINTEN |= (0b1111 << 0); // activate all interrupts
	LPC_ADC->ADINTEN &= ~(1 << 8); // deactivate only one channel feature
	NVIC_EnableIRQ(ADC_IRQn); // enable interrupt

}

void ADC_IRQHandler(void){
	int16_t ADC0ValueChannel0 = ((LPC_ADC->ADDR0) >> 4) & 0xFFF; //Variable auxiliar para observar el valor del registro de captura
	int16_t ADC0ValueChannel1 = ((LPC_ADC->ADDR1) >> 4) & 0xFFF;
	int16_t ADC0ValueChannel2 = ((LPC_ADC->ADDR2) >> 4) & 0xFFF;
	int16_t ADC0ValueChannel3 = ((LPC_ADC->ADDR3) >> 4) & 0xFFF;

}

```