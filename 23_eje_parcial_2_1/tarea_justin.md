Programar el microcontrolador LPC1769 para que mediante su ADC digitalice  dos señales analógicas cuyos anchos de bandas son de 10 Khz cada una. Los canales utilizados deben ser el 2 y el 4 y los datos deben ser guardados en dos regiones de memorias distintas que permitan contar con los últimos 20 datos de cada canal. Suponer una frecuencia de core cclk de 100 Mhz. El código debe estar debidamente comentado.

```C
#include "../../CMSISv2p00_LPC17xx/Drivers/inc/lpc17xx_pinsel.h"
#include "../../CMSISv2p00_LPC17xx/Drivers/inc/lpc17xx_adc.h"
#include "../../CMSISv2p00_LPC17xx/Drivers/inc/lpc17xx_timer.h"
#include "../../CMSISv2p00_LPC17xx/Drivers/inc/lpc17xx_libcfg_default.h"

#define _ADC_INT_2			ADC_ADINTEN2
#define _ADC_CHANNEL_2		ADC_CHANNEL_2
#define _ADC_INT_4			ADC_ADINTEN4
#define _ADC_CHANNEL_4		ADC_CHANNEL_4
#define _ADC_SET_BURST		1
#define _ADC_RESET_BURST	0
#define ARRAY_SIZE			20

__IO uint16_t adc_value = 0;

void confADC(void);
void confPin(uint8_t funcnum,
		uint8_t opendrain,
		uint8_t pinmode,
		uint8_t pinnum,
		uint8_t portnum);

int main(void){
	confADC();
	//PIN ADC 1.30
	confPin(3, 0, 30, 1);
	//PIN ADC 0.25
	confPin(1, 0, 25, 0);
}

void confADC(void){
	// Set frequency to 20kHz
	ADC_Init(LPC_ADC, 20000);

	//INT + CHANNEL 2
	ADC_IntConfig(LPC_ADC, _ADC_INT_2, ENABLE);
	ADC_ChannelCmd(LPC_ADC, _ADC_CHANNEL_2, ENABLE);
	//INT + CHANNEL 4
	ADC_IntConfig(LPC_ADC, _ADC_INT_4, ENABLE);
	ADC_ChannelCmd(LPC_ADC, _ADC_CHANNEL_4, ENABLE);

	ADC_BurstCmd(LPC_ADC, _ADC_SET_BURST);
	ADC_StartCmd(LPC_ADC, ADC_START_CONTINUOUS);
}

void confPin(uint8_t funcnum,
				uint8_t opendrain,
				uint8_t pinmode,
				uint8_t pinnum,
				uint8_t portnum){
	PINSEL_CFG_Type confPin;
	confPin.Funcnum = funcnum;
	confPin.OpenDrain = opendrain;
	confPin.Pinmode = pinmode;
	confPin.Pinnum = pinnum;
	confPin.Portnum = portnum;
	PINSEL_ConfigPin(&confPin);
	return;
}

void ADC_IRQHandler(void){
	static uint16_t data_ch2[ARRAY_SIZE];
	static uint16_t data_ch4[ARRAY_SIZE];
	adc_value = 0;
	if(ADC_ChannelGetStatus(LPC_ADC, _ADC_CHANNEL_2, ADC_DATA_DONE)){
		adc_value = ADC_ChannelGetData(LPC_ADC, _ADC_CHANNEL_2);
		for(i = 0; i < ARRAY_SIZE - 1; i++){
			data_ch2[i] = data_ch2[i + 1];
		}
		data_ch2[0] = adc_value;

	} else if (ADC_ChannelGetStatus(LPC_ADC, _ADC_CHANNEL_4, ADC_DATA_DONE)){
		adc_value = ADC_ChannelGetData(LPC_ADC, _ADC_CHANNEL_4);
		for(i = 0; i < ARRAY_SIZE - 1; i++){
			data_ch4[i] = data_ch4[i + 1];
		}
		data_ch4[0] = adc_value;
	}
}

```