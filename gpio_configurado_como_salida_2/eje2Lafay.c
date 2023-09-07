/*
 * Estudiante: Justin Lafay
 * Carrera: Electronica
 *
 */

#include "LPC17xx.h"

void retardo(uint32_t contador);

int main(void){

	uint32_t relojCpu = SystemCoreClock;
	int numDisplay [10] ={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};

	LPC_GPIO2->FIODIR0     |= (0b11111111<<0);

	while(1){
		for (int i=0; i<10; i++){
			LPC_GPIO2->FIOSET0 &= (numDisplay[i]);
			LPC_GPIO2->FIOSET0 |= (numDisplay[i]);
			retardo(relojCpu);
		}
	}

	return 0;
}

void retardo(uint32_t contador){

	for(contador =0 ; contador<6000000; contador++){};

}
