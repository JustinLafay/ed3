a.- Modificar el valor del Match del ejemplo visto en clases para que la frecuencia de muestreo del ADC sea de 20 Kmuestras/seg. El resultado de la conversión deben mostrarse por 12 pines de salida del GPIO0.  Adjuntar el código en C.

Ejemplo en classe :

```C
#include "LPC17xx.h"

void confGPIO(void); // Prototipo de la funci�n de conf. de puertos
void confADC(void); //Prototipo de la funci�n de conf. de interrupciones externas
void confTimer(void);
void retardo(void);

int main(void) {
	confGPIO();
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

void confGPIO(void){
	LPC_GPIO0->FIODIR |= (1<<22)|(1<<9);
	return;
}

void confADC(void){
	LPC_SC->PCONP |= (1 << 12);
	LPC_ADC->ADCR |= (1 << 21);    //enable ADC
	LPC_SC->PCLKSEL0 |= (3<<24);  //CCLK/8
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
	LPC_TIM0->MR1 = 100000000;
	LPC_TIM0->MCR = (1<<4); //reset on match
	LPC_TIM0->TCR = 3;      // hab y reset
	LPC_TIM0->TCR &= ~(1<<1);
	return;
}


void ADC_IRQHandler(void)
{
	static uint16_t ADC0Value = 0;
	float volt = 0;
	ADC0Value = ((LPC_ADC->ADDR0)>>4) & 0xFFF; //Variable auxiliar para observar el valor del registro de captura
	if(ADC0Value>2055)
		LPC_GPIO0->FIOSET |= (1<<9);
	else
		LPC_GPIO0->FIOCLR |= (1<<9);
	volt=(ADC0Value/4096)*3.3;
	return;
}
```

Calculo :
$$
T = \frac{1}{CCLK} \cdot (PR + 1) \cdot (TMR + 1) ≡ TMR = (T \cdot CCLK) - 1 ≡ TMR = (\frac{100 \cdot 10^6}{20000}) - 1 ≡ TMR = 4999
$$
Cambiamos :
```C
PC_TIM0->MR1 = 100000000;
```
Por :
```C
PC_TIM0->MR1 = 4999;
```
Codigo final : 

```C
#include "LPC17xx.h"

void confGPIO(void); // Prototipo de la funci�n de conf. de puertos
void confADC(void); //Prototipo de la funci�n de conf. de interrupciones externas
void confTimer(void);
void retardo(void);

int main(void) {
	confGPIO();
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

void confGPIO(void){
	LPC_GPIO0->FIODIR |= 0xFFF;
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
	LPC_TIM0->MR1 = 4999;
	LPC_TIM0->MCR = (1<<4); //reset on match
	LPC_TIM0->TCR = 3;      // hab y reset
	LPC_TIM0->TCR &= ~(1<<1);
	return;
}


void ADC_IRQHandler(void)
{
	static uint16_t ADC0Value = 0;
	ADC0Value = ((LPC_ADC->ADDR0)>>4) & 0xFFF; //Variable auxiliar para observar el valor del registro de captura
	LPC_GPIO0->FIOSET |= ADC0Value;
	LPC_GPIO0->FIOCLR |=~ ADC0Value;
	return;
}
```

 b.- Modificar el código realizado en el punto "a" utilizando ahora el Driver provisto para este microcontrolador. 

```C
#include "../../CMSISv2p00_LPC17xx/Drivers/inc/lpc17xx_pinsel.h"
#include "../../CMSISv2p00_LPC17xx/Drivers/inc/lpc17xx_adc.h"
#include "../../CMSISv2p00_LPC17xx/Drivers/inc/lpc17xx_timer.h"
#include "../../CMSISv2p00_LPC17xx/Drivers/inc/lpc17xx_libcfg_default.h"

#define _ADC_INT		ADC_ADINTEN0
#define _ADC_CHANNEL	ADC_CHANNEL_0

__IO uint16_t adc_value = 0;

void confGPIO(void);
void confADC(void);
void confTimer(void);

int main(void){
	confGPIO();
	confADC();
	confTimer();
}

void confGPIO(void){
	LPC_GPIO0->FIODIR |= 0xFFF;
	return;
}

void confADC(void){
	ADC_Init(LPC_ADC, 200000);
	ADC_IntConfig(LPC_ADC, _ADC_INT, ENABLE);
	ADC_ChannelCmd(LPC_ADC, _ADC_CHANNEL, ENABLE);
	ADC_StartCmd(LPC_ADC, ADC_START_ON_MAT01);
}

void confTimer(void){
	TIM_TIMERCFG_Type	tmrConf;
	TIM_MATCHCFG_Type	matchConf;

	tmrConf.PrescaleOption	=	TIM_PRESCALE_USVAL;
	tmrConf.PrescaleValue	=	4999;

	matchConf.MatchChannel			=	1;
	matchConf.IntOnMatch			=	ENABLE;
	matchConf.ResetOnMatch			=	ENABLE;
	matchConf.StopOnMatch			=	DISABLE;
	matchConf.ExtMatchOutputType	=	TIM_EXTMATCH_NOTHING;
	matchConf.MatchValue			=	4999;

	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &tmrConf);
	TIM_ConfigMatch(LPC_TIM0, &matchConf);
	TIM_Cmd(LPC_TIM0, ENABLE);

	NVIC_EnableIRQ(TIMER0_IRQn);
}

void ADC_IRQHandler(void){
	adc_value = 0;
	if(ADC_ChannelGetStatus(LPC_ADC, _ADC_CHANNEL, ADC_DATA_DONE)){
		adc_value = ADC_ChannelGetData(LPC_ADC, _ADC_CHANNEL);
		adc_value = (adc_value>>4) & 0xFFF;
	}
	LPC_GPIO0->FIOSET |= adc_value;
	LPC_GPIO0->FIOCLR |=~ adc_value;
}

```