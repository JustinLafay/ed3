/*
 * Estudiante: Justin Lafay
 * Carrera: Electronica
 *
 */

#include "LPC17xx.h"

void retardo(uint32_t contador);

int main(void){

	uint32_t relojCpu = SystemCoreClock;

	LPC_GPIO0->FIODIR |= (0b1111111111<<0); // select 9 first pin as out (1)

	int seq [2] = {0b1100100101, 0b001110001}; // table of 2 sequences of 10 LED

	while(1){
		for (int i = 0; i<2; i++){
			LPC_GPIO0->FIOSET &= (seq[i]<<0);
			LPC_GPIO0->FIOSET |= (seq[i]<<0);
			retardo(relojCpu);
		}
	}

	return 0;
}

void retardo(uint32_t contador){

	for(contador =0 ; contador<6000000; contador++){};

}