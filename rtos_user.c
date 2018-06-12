/*******************************************
*	Name: Julian Torres
*	File Name: rtos_user.c
* CPTR 480
* 6/12/2018
*	Purpose: 	Set reload value so there is an interrupt once every millisecond. There are 2 threads, TurnSequence and 
*						PeriodicCorrection, that are run in a round robin fashion. PeriodicSensor is run in the Scheduler and samples
*						ADC data once every millisecond. Right now the turn sequence decides the path of the car: 
* 					Left, Straight, Straight, Right. So the car should start in the bottom right corner of the map and it will make
*						its way to the top left corner. A semaphore is used to maintain mutual exclusion of control over the motor between 
*						the two threads, TurnSequence and PeriodicCorrection. This is so that the TurnSequence is not interrupted by 
*						the straight path correction.
*******************************************/


//****************************************************************************
// rtos_user.c
// Runs on a TI MSP-EXP432E401Y board
// An example user program that initializes the simple operating system
//   Schedules three independent threads using preemptive round robin  
//   Each thread rapidly toggles an LED 
//
//  Based on a program written by:
// Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
//    You may use, edit, run or distribute this file
//    as long as the above copyright notice remains
// Modified to use CMSIS definitions for registers
// Uses the default internal system clock, 16Mhz, no PLL,
//    as input to the SysTick 24 bit counter.
//
// Modified by L.Aamodt  4/25/18  Walla Walla University
//****************************************************************************

#include <msp.h>
#include <stdint.h>
#include "PLL.h"
#include "Timer.h"
#include "UART.h"
#include "PWM.h"
#include "ADCSWTrigger_wwu.h"


#define SysCountReloadValue  0x1D4C0 // 1000Hz for 1ms 	   // thread switch time in system time units


void OS_Init(void);
int OS_AddThreads(void(*task0)(void),
	                void(*task1)(void));
void OS_Launch(uint32_t theTimeSlice);
void PeriodicSensor(void);
void PeriodicCorrection(void);
void OS_Wait(int32_t *s);
void OS_Signal(int32_t *s);
void TurnSequence(void);
void OutputADC(void);
																											

uint32_t ADCdata[9];						
int32_t drift;	// variable that tells the car how off center it is when going straight.
int32_t s = 1; 	// Semaphore
uint8_t turnCount = 0;


int main(void){
	PLL_Init();
	Timer0Init();
	UART_Init();
	ADC0_InitSWTriggerWWU();
	
	PWM0B_Init(37500, NEUTRAL);
	PWM1A_Init(37500, NEUTRAL);
	
  OS_Init();           // initialize, disable interrupts
	
  OS_AddThreads(&TurnSequence, &PeriodicCorrection);
	
  OS_Launch(SysCountReloadValue); // doesn't return, interrupts enabled in here
  return 0;             // this never executes
}


void PeriodicSensor(void) { // Periodic thread that is run in Scheduler
	ADC0_InSeq9(ADCdata);
}

// Thread 1
void TurnSequence(void){	
	while(1) {
		if ((ADCdata[2] > 2500) && (ADCdata[1] > 2500) && (ADCdata[0] > 2500)) {	// If the front sensors hit an intersection
			OS_Wait(&s);
			WaitMs(300); 	
			Stop();
			
			switch (turnCount) {					// Car path
				case 0: TurnLeft(&ADCdata[1]); break;
				case 1: MoveStraight(); break;
				case 2: MoveStraight(); break;
				case 3: TurnRight(&ADCdata[1]); break;
			}
			turnCount++;
			
			OS_Signal(&s);	
		}
	}
	
}

// Thread 2
void PeriodicCorrection(void) {		// This thread and TurnSequence each use a semaphore to control the motor.
	while(1) {											// Don't want it to try to correct path while turning.
		OS_Wait(&s);		
		drift = (ADCdata[2] + ADCdata[4]) - (ADCdata[0] + ADCdata[3]);
		
		if (drift > 50)
			CorrectLeft();
		else if (drift < -50) 
			CorrectRight();
		else 
			MoveStraight();
	
		OS_Signal(&s);
	}
	
}

// This was a 3rd thread that I ended up not implementing due to timing issues. I was going to use it for LCD.
void OutputADC(void) {		// Ended up not using. There was a timing issue with outputting these values in a thread.
	while (1) {
		OutCRLF();					
		UART_OutUDec(ADCdata[2]);
		UART_OutChar(' ');
		UART_OutUDec(ADCdata[1]);
		UART_OutChar(' ');
		UART_OutUDec(ADCdata[0]);
		OutCRLF();
			
		UART_OutUDec(ADCdata[4]);
		UART_OutChar(' ');
		UART_OutUDec(ADCdata[8]);
		UART_OutChar(' ');
		UART_OutUDec(ADCdata[3]);
		OutCRLF();
			
		UART_OutUDec(ADCdata[7]);
		UART_OutChar(' ');
		UART_OutUDec(ADCdata[6]);
		UART_OutChar(' ');
		UART_OutUDec(ADCdata[5]);
		OutCRLF();
		OutCRLF();
	}	

}

