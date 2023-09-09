/*
 * Estudiante: Justin Lafay
 * Carrera: Electronica
 *
 */

#include "LPC17xx.h"

int main(void){
	short buffer[8];
	short result = 0;
	short pin_out_l = 0;
	short pin_out_h = 0;
	int point = 0;

	LPC_GPIO1->FIODIR &= ~(0b1111111111111111<<0); // input mode from P1.0 to P1.15
	LPC_GPIO0->FIODIR |= (0b1111000111111111111<<0); // output mode

	while(1){
		result = 0;
		if (point == 7){
			point = 0;
		}
		buffer[point] = LPC_GPIO1->FIOPINL;

		for (int i; i<7; i++){
			result = result+(buffer[i]); // calculate the buffer
		}

		 pin_out_h = result & (0b1111<<15);
		 pin_out_l = result & (0b111111111111<<0);
		 result = pin_out_l | pin_out_h;

		LPC_GPIO0->FIOSET0 &= result;
		LPC_GPIO0->FIOSET0 |= result;

		point++;
	}
	return 0;
}
