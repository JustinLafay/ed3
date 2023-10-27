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

Utilizando el timer0, un dac, interrupciones y el driver del LPC1769 , escribir un código que permita generar una señal triangular periódica simétrica, que tenga el mínimo periodo posible, la máxima excursión de voltaje pico a pico posible y el mínimo incremento de señal posible por el dac. Suponer una frecuencia de core cclk de 100 Mhz. El código debe estar debidamente comentado.

```C
#include "../../CMSISv2p00_LPC17xx/Drivers/inc/lpc17xx_pinsel.h"
#include "../../CMSISv2p00_LPC17xx/Drivers/inc/lpc17xx_adc.h"
#include "../../CMSISv2p00_LPC17xx/Drivers/inc/lpc17xx_dac.h"
#include "../../CMSISv2p00_LPC17xx/Drivers/inc/lpc17xx_timer.h"
#include "../../CMSISv2p00_LPC17xx/Drivers/inc/lpc17xx_libcfg_default.h"

void confTimer(void);
void confDAC(void);
void confDMA(void);
void confPin(void);

uint16_t valores[2048];

int main(void) {
	confTimer();
	confDAC();
	confPin();
	confDMA();

	for (uint16_t i = 0; i < 2048; i++) {
		if (i < 1025) {
			valores[i] = i;
		} else if (i >= 1025) {
			valores[i] = 2048 - (i-1025);
		}
	}

	while (1) {
		GPDMA_ChannelCmd(0, ENABLE);
	}
	return 0;
}

void confTimer() {
	TIM_TIMERCFG_Type struct_config;
	TIM_MATCHCFG_Type struct_match;

	struct_config.PrescaleOption = TIM_PRESCALE_TICKVAL;
	struct_config.PrescaleValue = 0xFFFFFFFF;

	struct_match.MatchChannel = 0;
	struct_match.IntOnMatch = ENABLE;
	struct_match.ResetOnMatch = ENABLE;
	struct_match.StopOnMatch = DISABLE;
	struct_match.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
	struct_match.MatchValue = 0xFFFFFFFF;

	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &struct_config);
	TIM_ConfigMatch(LPC_TIM0, &struct_match);
	TIM_Cmd(LPC_TIM0, ENABLE);

	NVIC_EnableIRQ(TIMER0_IRQn);

	return;
}

void confDAC() {
	DAC_Init(LPC_DAC);
}

void confPin(void) {
	PINSEL_CFG_Type pinsel_cfg;
	pinsel_cfg.Portnum = 0;
	pinsel_cfg.Pinnum = 26;
	pinsel_cfg.Funcnum = 2;
	pinsel_cfg.Pinmode = 0;
	pinsel_cfg.OpenDrain = 0;
	PINSEL_ConfigPin(&pinsel_cfg);
	return;
}

void confDMA(void){
	GPDMA_LLI_Type LLI1;
	LLI1.SrcAddr = &valores;
	LLI1.DstAddr = LPC_DAC->DACR;
	LLI1.NextLLI = &LLI1;
	LLI1.Control = 2048 | (1<<18) | (1<<21) | (1<<26);
	GPDMA_Init();

	GPDMA_Channel_CFG_Type channel_DMA;
	channel_DMA.ChannelNum = 0;
	channel_DMA.SrcMemAddr = &valores;
	channel_DMA.DstMemAddr = 0;
	channel_DMA.TransferSize = 2048;
	channel_DMA.TransferWidth = 0;
	channel_DMA.TransferType = GPDMA_TRANSFERTYPE_M2P;
	channel_DMA.SrcConn = 0;
	channel_DMA.DstConn = GPDMA_CONN_DAC;
	channel_DMA.DMALLI = (uint16_t)&LLI1;
	GPDMA_Setup(&channel_DMA);

}

void TIMER0_IRQHandler(void) {
	static uint16_t dac_val;
	static uint8_t flag;
	if ((dac_val >= 0x3FF) | (dac_val <= 0)) {
		flag ^= flag;
	}
	if (flag == 0) {
		dac_val++;
	} else if (flag == 1) {
		dac_val--;
	}
	DAC_UpdateValue(LPC_DAC, dac_val);
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);

}

```