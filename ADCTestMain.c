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
//	Timer0Init(); ////////////////////////
//	UART_Init();								// Added
//  ADC0_InitSWTriggerWWU();   // initialize ADC0 for software triggering
//	
//	//ADC0_InSeq9(ADCdata);      // Start conversions, read data, return 9 values

//	//Load_9registers(ADCdata);  // Debug using registers.  Will stop here 
//	
//	
//  while(1){                  // Never should get here.
//		ADC0_InSeq9(ADCdata);      // Start conversions, read data, return 9 values
//		
//		//Mask or strip bits of address
//		sw1 = GPIOJ->DATA & SW1_MASK;
//		
//		//SW1 logic;
//		if (!(sw1 == SW1_MASK)) {
//			OutCRLF();					
//			UART_OutUDec(ADCdata[2]);
//			UART_OutChar(' ');
//			UART_OutUDec(ADCdata[1]);
//			UART_OutChar(' ');
//			UART_OutUDec(ADCdata[0]);
//			OutCRLF();
//			
//			UART_OutUDec(ADCdata[4]);
//			UART_OutChar(' ');
//			UART_OutUDec(ADCdata[8]);
//			UART_OutChar(' ');
//			UART_OutUDec(ADCdata[3]);
//			OutCRLF();
//			
//			UART_OutUDec(ADCdata[7]);
//			UART_OutChar(' ');
//			UART_OutUDec(ADCdata[6]);
//			UART_OutChar(' ');
//			UART_OutUDec(ADCdata[5]);
//			OutCRLF();
//			OutCRLF();
//		}
//		// count++;
//		for (uint32_t i = 0; i < 5000000; i++); 			// Added
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
