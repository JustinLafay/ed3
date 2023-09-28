/*
 * Estudiante: Justin Lafay
 * Carrera: Electronica
 *
 */

#include "LPC17xx.h"
#include <stdio.h>
#include <string.h>

int main(void){

	char resulto[14];

	LPC_GPIO0->FIODIR0 &= ~(0b11<<0);
	LPC_PINCON->PINMODE0 |= (0b1111<<0);

	while(1){

		switch (LPC_GPIO0->FIOPIN0 &= (0b11<<0)){
		case 0b00:
			strcpy(resulto ,"ninguno");
			break;
		case 0b01:
			strcpy(resulto ,"pulsador 1");
			break;
		case 0b10:
			strcpy(resulto ,"pulsador 2");
			break;
		case 0b11:
			strcpy(resulto ,"pulsador 1 y 2");
			break;
		printf("%s",resulto);
		}
	}
	return 0;
}
