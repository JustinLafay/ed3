/*
 * Estudiante: Justin Lafay
 * Carrera: Electronica
 * Descripci�n: Este programa pende y apaga un led
 *              conectado al pin P0.22 de forma intermitente
 *
 */

#include "LPC17xx.h"

void retardo(uint32_t contador);

int main(void){

	uint32_t relojCpu = SystemCoreClock;

	//LPC_PINCON->PINSEL1 &= ~(3<<12);   // 0B00111111111111 & 0B10010101010101 = 0B00_010101010101
	LPC_GPIO0->FIODIR     |= (0b1111111111<<0);     // 0B1000...000 | 0B010101..101 = 1_10101..101
	//LPC_GPIO0->FIOMASK =0xFFFFFFFF;  //~FIOMASK & REGISTRO =0000

	int seq1 = 0b1100100101;
	int seq2 = 0b0011100011;

	while(1){
		LPC_GPIO0->FIOCLR |= (seq1<<0);
		retardo(relojCpu);
		LPC_GPIO0->FIOSET |= (seq2<<0);
		retardo(relojCpu);
	}

	return 0;
}

void retardo(uint32_t contador){

	for(contador =0 ; contador<6000000; contador++){};

}
