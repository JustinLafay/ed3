# ED3 - 2023

## C Basics

Put 1 at "x" position in register "r" :

```C
r |= (1<<x);
```

Put 0 at "x" position in register "r" :

```C
r &= ~(1<<x);
```

## PIN SELECTION

### PINSEL

Exemple of selection

```C
LPC_PINCON->PINSEL0 |= (0b01<<14);  //ADC AD0.0 PIN 0.23 OR
LPC_PINCON->PINSEL0 &= ~(0b10<<14);  //ADC AD0.0 PIN 0.23 AND
```

<div>
    <details>
    <summary>PINSEL0</summary>
    <br>
    <img src="img_readme/pinsel0.png" width=700px>
    </details>
    <details>
    <summary>PINSEL1</summary>
    <br>
    <img src="img_readme/pinsel1.png" width=700px>
    </details>
    <details>
    <summary>PINSEL2</summary>
    <br>
    <img src="img_readme/pinsel2.png" width=700px>
    </details>
    <details>
    <summary>PINSEL3</summary>
    <br>
    <img src="img_readme/pinsel3.png" width=700px>
    </details>
    <details>
    <summary>PINSEL4</summary>
    <br>
    <img src="img_readme/pinsel4.png" width=700px>
    </details>
    <details>
    <summary>PINSEL7</summary>
    <br>
    <img src="img_readme/pinsel7.png" width=700px>
    </details>
    <details>
    <summary>PINSEL9</summary>
    <br>
    <img src="img_readme/pinsel9.png" width=700px>
    </details>
    <details>
    <summary>PINSEL10</summary>
    <br>
    <img src="img_readme/pinsel10.png" width=700px>
    </details>
</div>

### PINMODE

<img src="img_readme/pinmode.png" width=700px>

PINMODE use the same registers numbers as PINSEL, to change the targeted register, use the same position used in PINSEL.

Exemple of mode selection

```C
LPC_PINCON->PINMODE0 |= (0b10<<14);		//NEITHER PULL UP/DOWN OR
LPC_PINCON->PINMODE0 &= ~(0b01<<14);	//NEITHER PULL UP/DOWN AND
```

## GPIO

### Digital I/O ports

- Accelerated GPIO functions:
    - GPIO registers are located on a peripheral AHB bus for fast I/O timing.
    - Mask registers allow treating sets of port bits as a group, leaving other bits unchanged.
    - All GPIO registers are byte, half-word, and word addressable.
    - Entire port value can be written in one instruction.
    - GPIO registers are accessible by the GPDMA.
- Bit-level set and clear registers allow a single instruction set or clear of any number of bits in one port.
- All GPIO registers support Cortex-M3 bit-banding.
- GPIO registers are accessible by the GPDMA controller to allow DMA of data to or from GPIOs, synchronized to any DMA request.
- Direction control of individual port bits.
- All I/Os default to input with pullup after reset.

### How to setup GPIO ?

Exemple for output :

```C
LPC_GPIO0->FIODIR     |= (1<<22);

while(1){
    LPC_GPIO0->FIOCLR |= (1<<22);  // apaga el led
    LPC_GPIO0->FIOSET |= (1<<22);  // encende el led
}
```

Exemple for input :

```C
int main(void){
	uint32_t tiempo;

	LPC_GPIO0->FIODIR    |= (1<<22);
	LPC_GPIO2->FIODIR    &= ~(1<<10); //Configura el pin P2.10 como entrada
	LPC_PINCON->PINMODE4 |= (3<<20);  //Asociar resistencia de Pull-Down al pin P2.10

	while(1){
		if ((LPC_GPIO2->FIOPIN)&(1<<10)){
			tiempo = 1000000;
		}
		else{
			tiempo = 4000000;
		}
		LPC_GPIO0->FIOCLR |= (1<<22); //Apaga el led
		retardo(tiempo);
		LPC_GPIO0->FIOSET |= (1<<22); //Prende el led
		retardo(tiempo);
	}
	return(0);
}

```
### GPIO Direction :

<img src="img_readme/gpio_dir.png" width=700px>

<details>
<summary>GPIO DIR list</summary>
<br>
<img src="img_readme/gpio_dir_x.png" width=700px>
</details>

This list show starting bits and ending bits of each control register.

### GPIO Set :

<img src="img_readme/gpio_set.png" width=700px>

### GPIO Clear :

<img src="img_readme/gpio_clr.png" width=700px>

### GPIO Get value :

<img src="img_readme/gpio_val.png" width=700px>

### GPIO Mask :

<img src="img_readme/gpio_mask.png" width=700px>

### GPIO Interrupt :

Here is an example of GPIO interrupt config :
```C
void confGPIO(void){
	LPC_GPIO0->FIODIR |= (1<<22);
	LPC_GPIO0->FIODIR &=~ (1<<15);
	return;
}
void confIntGPIO(void){
	LPC_GPIOINT->IO0IntEnR |= (1<<15); //Selecciona Interrupci�n por flanco de subida.
	LPC_GPIOINT->IO0IntClr |= (1<<15); //Limpia la bandera de interrupci�n
	NVIC_EnableIRQ(EINT3_IRQn);        // Habilita de interrupciones externas.
	return;
}
void EINT3_IRQHandler(void)
{
	if(LPC_GPIOINT->IO0IntStatR & (1<<15)){ //Pregunta si el pin P0.15 produjo la interrupci�n
		inte++;
		LPC_GPIOINT->IO0IntClr |= (1<<15); //limpia la bandera de interrupci�n
	}
	return;
}
```
#### Interrupt status registers (all GPIO) :

<img src="img_readme/gpio_int.png" width=700px>

#### Interrupt set registers (rising edge) :

<img src="img_readme/gpio_int_set.png" width=700px>

<details>
<summary>GPIO Int port 0 rising edge</summary>
<br>
<img src="img_readme/gpio_int_port_0.png" width=700px>
</details>
<details>
<summary>GPIO Int port 2 rising edge</summary>
<br>
<img src="img_readme/gpio_int_port_2.png" width=700px>
</details>

#### Interrupt set registers (falling edge) :

<img src="img_readme/gpio_int_falling_edge.png">
Same bit numbers for falling edge than rising edge.

#### Interrupt clear registers :

<img src="img_readme/gpio_int_clear.png">
Same bit numbers for clear than rising edge.

## Timer

Exemple setup of timer (modo match) :

```C
void confTimer(void){
	LPC_SC->PCONP       |= (1<<1);  // pag. 65
	LPC_SC->PCLKSEL0    |= (1<<2);  // pag. 59
	LPC_PINCON->PINSEL3 |= (3<<24); // pag. 120
	//LPC_PINCON->PINSEL7 |= (2<<18);
	LPC_TIM0->EMR       |=(3<<4);   // pag. 509
	LPC_TIM0->MR0        = 70000000;//
	LPC_TIM0->MCR       |= (1<<1);       // pag. 507
	LPC_TIM0->MCR       &= ~(1<<0);       // pag. 507
	LPC_TIM0->TCR        = 3;       // pag. 505
	LPC_TIM0->TCR       &= ~(1<<1);
	NVIC_EnableIRQ(TIMER0_IRQn);
	return;
}

void TIMER0_IRQHandler(void) //ISR del timer0
{
static uint8_t i = 0;
if (i==0){
LPC_GPIO0->FIOSET = (1<<22);
i = 1;
}
else if (i==1){
LPC_GPIO0->FIOCLR = (1<<22);
i = 0;
}
LPC_TIM0->IR|=1; //Limpia bandera de interrupci�n
return;
}
```