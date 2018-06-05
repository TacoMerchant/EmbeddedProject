///*******************************************
//*	Name: Julian Torres
//*	File Name: rtos_user.c
//*	Purpose: to test sample RTOS by adding 4th thread
//* CPTR 480
//* HW6
//* 4/27/2018
//*	Purpose / Results: 	The purpose of this assignment is to modify a simple RTOS by adding a fourth thread
//						that turns on the 4th LED. After adding the fourth thread, the priorities of the four
//						tasks need to be modified so that they run in the order 3,1,4,2. Each LED needs to turn
//						on for a second and then turn off for a second. The interrupts need to occur 16 times a 
//						second so the reload value is given a value of 1,000,000 because the frequency is 16MHz.
//						A for loop delay is added to each task to spin for about a second, so the for loop counts
//						to 200,000 because each iteration takes about 5us. Everything works correctly now and it makes
//						me happy. 

//* Issues: 	I couldn't figure out why but trying to add in the fourth task initially made none of them work at all.
//						After giving up for awhile, I restarted from the beginning and got it to work. I found that the iteration 
//						of my for loop delay took about 5us rather than 10us, but that also has a variable increment instruction 
//						in it to keep the compiler from skipping the for loop.
//*******************************************/


////****************************************************************************
//// rtos_user.c
//// Runs on a TI MSP-EXP432E401Y board
//// An example user program that initializes the simple operating system
////   Schedules three independent threads using preemptive round robin  
////   Each thread rapidly toggles an LED 
////
////  Based on a program written by:
//// Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
////    You may use, edit, run or distribute this file
////    as long as the above copyright notice remains
//// Modified to use CMSIS definitions for registers
//// Uses the default internal system clock, 16Mhz, no PLL,
////    as input to the SysTick 24 bit counter.
////
//// Modified by L.Aamodt  4/25/18  Walla Walla University
////****************************************************************************

//#include <msp.h>

//void OS_Init(void);
//int OS_AddThreads(void(*task0)(void),
//	                void(*task1)(void),
//                  void(*task2)(void),
//									void(*task3)(void)); ///////////
//void OS_Launch(uint32_t theTimeSlice);


//#define SysCountReloadValue  0xF4240    // thread switch time in system time units

//uint32_t Count1;   // number of times thread1 loops
//uint32_t Count2;   // number of times thread2 loops
//uint32_t Count3;   // number of times thread3 loops
//uint32_t Count4;   ///////////////////////////////
//uint32_t i1;       // inner loop count, thread 1

//void GPIO_init() {
//	// Enable clock to PORTN and PORTF
//	SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R12 | SYSCTL_RCGCGPIO_R5;
//	while(!(SYSCTL->PRGPIO & SYSCTL_PRGPIO_R12));	

//	// Set drive strength
//	GPION->DR2R |= BIT1;	// D1
//	GPION->DR2R |= BIT0;	// D2
//	GPIOF->DR2R |= BIT4;	// D3
//	GPIOF->DR2R |= BIT0;  ///////////////D4
//	
//	// Enable digital I/O mode
//	GPION->DEN |= BIT1;
//	GPION->DEN |= BIT0;
//	GPIOF->DEN |= BIT4;
//	GPIOF->DEN |= BIT0; ////////////////

//	// Disable analog mode
//	GPION->AMSEL &= ~(BIT1);
//	GPION->AMSEL &= ~(BIT0);
//	GPIOF->AMSEL &= ~(BIT4);
//	GPIOF->AMSEL &= ~(BIT0); /////////////////

//	// Set the pin direction as output
//	GPION->DIR |= BIT1;
//	GPION->DIR |= BIT0;
//	GPIOF->DIR |= BIT4;
//	GPIOF->DIR |= BIT0; ///////////////////

//	GPION->AFSEL = 0x00;
//	GPIOF->AFSEL = 0x00;
//}

//void Task1(void){
//  Count1 = 0;
//	
//  for(;;){
//		for (uint32_t i = 0; i < 200000; i++) {
//			Count1++;
//		}
//		GPION->DATA ^= BIT1;				// toggle D1
//  }
//}
//void Task2(void){
//  Count2 = 0;
//  for(;;){
//		for (uint32_t i = 0; i < 200000; i++) {
//			Count2++;
//		}
//		GPION->DATA ^= BIT0;				// toggle D2
//  }
//}
//void Task3(void){
//  Count3 = 0;
//  for(;;){
//		for (uint32_t i = 0; i < 200000; i++) {
//			Count3++;
//		}
//		GPIOF->DATA ^= BIT4;				// toggle D3
//  }
//}

//void Task4(void){ ///////////////////////////////
//  Count4 = 0;
//  for(;;){
//    for (uint32_t i = 0; i < 200000; i++) {
//			Count4++;
//		}
//		GPIOF->DATA ^= BIT0;				// toggle D4
//  }
//}

//int main(void){
//  OS_Init();           // initialize, disable interrupts
//	GPIO_init();					// initialize GPIO ports for 3 LEDs
//  OS_AddThreads(&Task3, &Task1, &Task4, &Task2);
//  OS_Launch(SysCountReloadValue); // doesn't return, interrupts enabled in here
//  return 0;             // this never executes
//}
