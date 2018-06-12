///*******************************************
//*	Name: Julian Torres
//* CPTR 480
//* 6/12/2018
//*	File Name: StopTest.c
//*	Purpose: This is a test for the sensor functions when they reach an intersection.
//*******************************************/

////****************************************************************************
//// ADCTestMain.c                                                             *
//// Runs on MSP432E401Y                                                       *
////                                                                           *
//// Program to test the ADC0_SWTriggerWWU routines                            *
//// L.Aamodt  5/28/18                                                         *
////****************************************************************************

//#include "msp.h"
//#include <stdint.h>
//#include "PLL.h"
//#include "Timer.h"
//#include "UART.h"
//#include "PWM.h"
//#include "ADCSWTrigger_wwu.h"

//#define SW1_MASK (0x1)

//void Init_Switches(void);

//int32_t count;
//uint32_t ADCdata[9];

//int main(void){
//	uint32_t sw1;						// Added

//	Init_Switches();					// Added
//	
//  PLL_Init();                // 120 MHz system clock rate
//	Timer0Init(); //////////////////////////////
//	UART_Init();								// Added
//  ADC0_InitSWTriggerWWU();   // initialize ADC0 for software triggering
//	PWM0B_Init(37500, NEUTRAL);
//	PWM1A_Init(37500, NEUTRAL);
//	
//	//ADC0_InSeq9(ADCdata);      // Start conversions, read data, return 9 values

//	//Load_9registers(ADCdata);  // Debug using registers.  Will stop here 
//	
//	Stop();
//	MoveStraight();
//	
//	//TurnRight();
//	
//  while(1){                  // Never should get here.
//		ADC0_InSeq9(ADCdata);
//		
//		if ((ADCdata[2] > 1500) && (ADCdata[1] > 3000) && (ADCdata[0] > 3000)) {
//			WaitMs(850);
//			Stop();
//			TurnLeft();
//		}
//  }
//}

//void Init_Switches() {					////// Added
//	// Halt watchdog timer							// Added
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
