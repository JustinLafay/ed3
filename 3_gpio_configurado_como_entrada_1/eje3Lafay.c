/*
 * Estudiante: Justin Lafay
 * Carrera: Electronica
 *
 */

#include "LPC17xx.h"


int main(void){

	int acumulador = 0;
	int temp = 0;

	LPC_GPIO0->FIODIR0     &= ~(0b11111<<0);
	LPC_PINCON->PINMODE0 	|= (0b11<<8);

	while(1){
		temp = 0;
		for (int i=0; i<3; i++){
			temp = temp + (LPC_GPIO0->FIOPIN0 &= (1<<i));
		}
		if ((LPC_GPIO0->FIOPIN0)&(1<<4)){
			acumulador = acumulador + temp;
		}
		else{
			acumulador = acumulador - temp;
		}
	}

	return 0;
}