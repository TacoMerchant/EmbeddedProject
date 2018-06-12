////****************************************************************************
//// PWMTestMain.c                                                             *
//// Runs on MSP432E401Y                                                       *
//// Program for testing routines in PWM.c                                     *
////   First call one of the Init functions.                                   *
////   Call a Duty function to change the duty cycle                           *
////                                                                           *
//// Use PWM0B/PF1 and PWM1A/PF2 to generate pulse-width modulated outputs.    *
////                                                                           *
////   period is a 16-bit number of PWM clock cycles in one period (3<=period) *
////   period for PF0 and PF1 must be the same,PF2 can differ from PF0&PF1     *
////   duty is number of PWM clock cycles output is high  (2<=duty<=period-1)  *
////   PWM clock rate = processor clock rate/SYSCTL_CC_PWMDIV_64               *
////                  = BusClock/64                                            *
////                  = 120 MHz/64 = 1875000 Hz                                *
//// Created 5/28/18    L.Aamodt                                               *
////****************************************************************************
//#include "msp.h"
//#include <stdint.h>
//#include "PLL.h"
//#include "PWM.h"



//#define SW1_MASK (0x1)
//#define SW2_MASK (0x2)



//void Init_Switches(void);

//// Baseline for motors is about 1500us or 1.5ms duty cycle = 2812/ 37500
//// Max forward is about 1800us = 3375/37500
//// Max reverse is about 1300us = 2437/37500




//int main(void){
//	uint32_t count;
//	uint32_t sw1;
//	uint32_t sw2;
//	
//	
//	
//	Init_Switches();
//	
//	// ML goes forward with 3375, backward with 2437?   This is blue ima connect to PF1
//	// MR goes forward with 2437, backwards with 3375		This is yellow ima connect to PF2
//	
//  PLL_Init();                   // bus clock at 120 MHz
//  PWM0B_Init(37500, NEUTRAL);      // initialize PWM0B/PF1, 50Hz, 1500us pulse width
//	PWM1A_Init(37500, NEUTRAL);      // initialize PWM1A/PF2, 50Hz, 1500us pulse width
//	
//	//TurnLeft();

//  while(1){
//		sw1 = GPIOJ->DATA & SW1_MASK;
//		sw2 = GPIOJ->DATA & SW2_MASK;
//		
//		//SW1 logic;
//		if (!(sw1 == SW1_MASK)) {
//			TurnLeft();
//		}
//		//SW2 logic;
//		if (!(sw2 == SW2_MASK)) {
//			TurnRight();
//		}

//		
//		for (int i = 0; i < 5000000; i++) {
//			count++;
//		}
//		count = 0;
//		//count ++;                   // do nothing
//  }
//}



//void Init_Switches() {
//	// Halt watchdog timer
//	SYSCTL->RCGCWD &= ~(SYSCTL_RCGCWD_R1 | SYSCTL_RCGCWD_R0);

//	// Enable GPION, GPIOJ, GPIOF peripheral /////////////////only J needed
//	SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R8;

//	// Check if the peripheral access is enabled.
//	while(!(SYSCTL->PRGPIO & SYSCTL_PRGPIO_R8));
//	
//	/* Enable the GPIO pin for Switches (PJ0 & PJ1). (SW1 & SW2)*/
//	
//	//Enable pull up resistors
//	GPIOJ->PUR |= BIT0;
//	GPIOJ->PUR |= BIT1;
//	//Disable pull down resistors
//	GPIOJ->PDR &= ~(BIT0);
//	GPIOJ->PDR &= ~(BIT1);
//	
//	//Set the pin direction and mode
//	GPIOJ->DIR &= ~(BIT0);
//	GPIOJ->DIR &= ~(BIT1);
//	GPIOJ->AMSEL &= ~(BIT0);
//	GPIOJ->AMSEL &= ~(BIT1);
//	//Set pin type
//	GPIOJ->DEN |= BIT0;
//	GPIOJ->DEN |= BIT1;
//}
