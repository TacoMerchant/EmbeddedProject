//****************************************************************************
// Name: Julian Torres
// File: rtos_user.c
// Class: CPTR 480
// HW: Lab 5
// Date: 5/22/2018
//
// Requirements: Uses LCD screen connected to pin PC5. Pulse is transmitted through PF0.
//
// Purpose / Results: The purpose of this lab was to test the PWM module for controlling servo motors.
// 										This program generates a PWM signal whose pulse width can be changed up and down
//										with SW1 and SW2. The current pulse width is displayed on the LCD screen through 
//										UART7 on PC5, with units of counts. The max value that the program allows is roughly 
//										about 90% of the entire signal length (so it never reaches 100%).
//
// Issues:		I didn't run into a whole lot of issues with this one, it just took some time reading the
//						datasheet to figure out how the duty cycle is works with the functions given in PWM.c. I 
//						initially forgot to debounce the switches so it took quite a bit of time to understand why
//						my pulse width was going from 0% to about 90% with a single button press.

//****************************************************************************
// PWMTestMain.c
// Runs on MSP432E401Y
// Use PWM0A/PF0 and PWM0B/PF1 to generate pulse-width modulated outputs.
//
//  Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
//
//****************************************************************************
#include "msp.h"
#include <stdint.h>
#include "PLL.h"
#include "PWM.h"
#include "UART.h"

#define SW1_MASK (0x1)
#define SW2_MASK (0x2)

void Init_Switches(void);

int main(void){
	uint32_t sw1;
	uint32_t sw2;
	uint32_t period = 37500;
	uint32_t doody = 2500;
	
    // Halt watchdog timer
	SYSCTL->RCGCWD &= ~(SYSCTL_RCGCWD_R1 | SYSCTL_RCGCWD_R0);

	// Enable GPION, GPIOJ, GPIOF peripheral /////////////////only J needed
	SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R8;

	// Check if the peripheral access is enabled.
	while(!(SYSCTL->PRGPIO & SYSCTL_PRGPIO_R8));

	Init_Switches();
  PLL_Init();                      // bus clock at 120 MHz
	UART_Init();											//Initialize UART for screen.
  PWM0A_Init(period, doody); //7.5%       // initialize PWM0A/PF0, 1000 Hz, 75% duty 			PWM0A_Init(60000, 45000);
  PWM0B_Init(period, doody); //7.5%       // initialize PWM0B/PF1, 1000 Hz, 25% duty
	
	
	while(1) {
		//Mask or strip bits of address
		sw1 = GPIOJ->DATA & SW1_MASK;
		sw2 = GPIOJ->DATA & SW2_MASK;
		
		//SW1 logic;
		if (!(sw1 == SW1_MASK)) {
			if (doody <= 500)
				doody = 500;
			else
				doody = doody - 500;
		
		}
		//SW2 logic;
		if (!(sw2 == SW2_MASK)) {
			if (doody >= 33500)
				doody = 33500;
			else
				doody = doody + 500;
		}
		
		for (uint32_t i = 0; i < 5000000; i++);
		
		PWM0B_Duty(doody);
		
		UART_OutCommand(0xFE);		//Clear Screen
		UART_OutCommand(0x01);
		
		UART_OutString("Pulse Length");
		UART_OutCommand(0xFE);
		UART_OutCommand(0x80 + 64);
		UART_OutUDec(doody);			//Output current duty in units of count
		UART_OutChar('/');
		UART_OutUDec(37500);
	}
}

void Init_Switches() {
	/* Enable the GPIO pin for Switches (PJ0 & PJ1). (SW1 & SW2)*/
	
	//Enable pull up resistors
	GPIOJ->PUR |= BIT0;
	GPIOJ->PUR |= BIT1;
	//Disable pull down resistors
	GPIOJ->PDR &= ~(BIT0);
	GPIOJ->PDR &= ~(BIT1);
	
	//Set the pin direction and mode
	GPIOJ->DIR &= ~(BIT0);
	GPIOJ->DIR &= ~(BIT1);
	GPIOJ->AMSEL &= ~(BIT0);
	GPIOJ->AMSEL &= ~(BIT1);
	//Set pin type
	GPIOJ->DEN |= BIT0;
	GPIOJ->DEN |= BIT1;
}
