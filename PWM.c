/*******************************************
*	Name: Julian Torres
* CPTR 480
* 6/12/2018
*	File Name: PWM.c
*	Purpose: Holds PWM functions including control over motors such as turning.
*******************************************/

//****************************************************************************
// PWM.c                                                                     *
// Runs on MSP432E401Y                                                       *
// Derived from J.W. Valvano's PWM.c file      L.Aamodt  5/27/18             *
//    Converted to standard CMSIS naming.                                    * 
// Initialization and duty cycle setting routines for the PWM0 module on the *
// MSP432E401Y which has one PWM module with four generators, 2 outputs each *
//   PWM generator 0 output A that drives port PF0                           *
//   PWM generator 0 output B that drives port PF1                           *
//   PWM generator 1 output A that drives port PF2                           *
//                                                                           *
// Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu            *
//    You may use, edit, run or distribute this file                         *
//    as long as the above copyright notice remains                          *
//                                                                           *
// The system clock (SYSCLK) is assumed to be running at 120Mhz              *
//   period is a 16-bit number of PWM clock cycles in one period (3<=period) *
//   duty is number of PWM clock cycles output is high  (2<=duty<=period-1)  *
// PWM CC register is set to prescale SYSCLK by 64 to give 1.875Mhz PWM clock*
//   Set period = 37500 to get 50Hz pulse period                             *
//   Set duty = 2812 to get 1500us pulse width                               *
// Note: Port PF0 on the MSP-EXP432E401Y board is connected to LED D4        *
//****************************************************************************

#include <stdint.h>
#include "msp432e401y.h"
#include "PWM.h"
#include "Timer.h"



//#define PWM_ENABLE_PWM1EN       0x00000002  // MnPWM1 Output Enable
//#define PWM_ENABLE_PWM0EN       0x00000001  // MnPWM0 Output Enable
//#define PWM_0_CTL_ENABLE        0x00000001  // PWM Block Enable
//#define PWM_0_GENA_ACTCMPAD_ONE 0x000000C0  // Set the output signal to 1
//#define PWM_0_GENA_ACTLOAD_ZERO 0x00000008  // Set the output signal to 0
//#define PWM_0_GENB_ACTCMPBD_ONE 0x00000C00  // Set the output signal to 1
//#define PWM_0_GENB_ACTLOAD_ZERO 0x00000008  // Set the output signal to 0
//#define PWM_CC_USEPWM           0x00000100  // Use PWM Clock Divisor
//#define PWM_CC_PWMDIV_M         0x00000007  // PWM Clock Divider
//#define PWM_CC_PWMDIV_2         0x00000000  // /2
//#define SYSCTL_RCGCGPIO_R5      0x00000020  // GPIO Port F Run Mode Clock
//#define SYSCTL_RCGCPWM_R0       0x00000001  // PWM Module 0 Run Mode Clock
//#define SYSCTL_PRGPIO_R5        0x00000020  // GPIO Port F Peripheral Ready
//#define SYSCTL_PRPWM_R0         0x00000001  // PWM Module 0 Peripheral Ready

//****************************************************************************

// Output on PF0/M0PWM0
void PWM0A_Init(uint16_t period, uint16_t duty){
  SYSCTL->RCGCPWM |= SYSCTL_RCGCPWM_R0; // 1) activate clock for PWM0
                                        // 2) activate clock for Port F
  SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R5;
                                        // allow time for clock to stabilize
  while((SYSCTL->PRGPIO&SYSCTL_PRGPIO_R5) == 0){};
  GPIOF->AFSEL |= 0x01;                 // 3) enable alt funct on PF0
  GPIOF->DEN |= 0x01;                   //    enable digital I/O on PF0
                                        // 4) configure PF0 as PWM0
  GPIOF->PCTL = (GPIOF->PCTL&0xFFFFFFF0)+0x00000006;
  GPIOF->AMSEL &= ~0x01;          //    disable analog functionality on PF0
                                        // allow time for clock to stabilize
  while((SYSCTL->PRPWM&SYSCTL_PRPWM_R0) == 0){};
  PWM0->CC |= PWM_CC_USEPWM;            // 5) use PWM divider
  PWM0->CC &= ~PWM_CC_PWMDIV_M;         //    clear PWM divider field
  PWM0->CC += PWM_CC_PWMDIV_64;         //    configure for /64 divider
  PWM0->_0_CTL = 0;                     // 6) re-loading down-counting mode
  PWM0->_0_GENA = (PWM_0_GENA_ACTCMPAD_ONE|PWM_0_GENA_ACTLOAD_ZERO);
  // PF0 goes low on LOAD
  // PF0 goes high on CMPA down
  PWM0->_0_LOAD = period - 1;           // 7) cycles needed to count down to 0
  PWM0->_0_CMPA = duty - 1;             // 8) count value when output rises
  PWM0->_0_CTL |= PWM_0_CTL_ENABLE;     // 9) start PWM0
  PWM0->ENABLE |= PWM_ENABLE_PWM0EN;   // 10) enable PWM0A/PF0 outputs
}

// change duty cycle of PWM0A/PF0
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
void PWM0A_Duty(uint16_t duty){
  PWM0->_0_CMPA = duty - 1;             // 8) count value when output rises
}
//***************************************************************************

// Output on PF1/M0PWM1
void PWM0B_Init(uint16_t period, uint16_t duty){
  SYSCTL->RCGCPWM |= SYSCTL_RCGCPWM_R0; // 1) activate clock for PWM0
                                        // 2) activate clock for Port F
  SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R5;
                                        // allow time for clock to stabilize
  while((SYSCTL->PRGPIO&SYSCTL_PRGPIO_R5) == 0){};
  GPIOF->AFSEL |= 0x02;                 // 3) enable alt funct on PF1
  GPIOF->DEN |= 0x02;                   //    enable digital I/O on PF1
                                        // 4) configure PF1 as PWM0
  GPIOF->PCTL = (GPIOF->PCTL&0xFFFFFF0F)+0x00000060;
  GPIOF->AMSEL &= ~0x02;          //    disable analog functionality on PF1
                                        // allow time for clock to stabilize
  while((SYSCTL->PRPWM&SYSCTL_PRPWM_R0) == 0){};
  PWM0->CC |= PWM_CC_USEPWM;            // 5) use PWM divider
  PWM0->CC &= ~PWM_CC_PWMDIV_M;         //    clear PWM divider field
  PWM0->CC += PWM_CC_PWMDIV_64;         //    configure for /2 divider
  PWM0->_0_CTL = 0;                     // 6) re-loading down-counting mode
  PWM0->_0_GENB = (PWM_0_GENB_ACTCMPBD_ONE|PWM_0_GENB_ACTLOAD_ZERO);
  // PF1 goes low on LOAD
  // PF1 goes high on CMPB down
  PWM0->_0_LOAD = period - 1;           // 7) cycles needed to count down to 0
  PWM0->_0_CMPB = duty - 1;             // 8) count value when output rises
  PWM0->_0_CTL |= PWM_0_CTL_ENABLE;     // 9) start PWM0
  PWM0->ENABLE |= PWM_ENABLE_PWM1EN;    // 10) enable PWM0B/PF1 outputs
}

// change duty cycle of PWM0B/PF1
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
void PWM0B_Duty(uint16_t duty){
  PWM0->_0_CMPB = duty - 1;             // 8) count value when output rises
}
//****************************************************************************

// Output on PF2/M0PWM2
void PWM1A_Init(uint16_t period, uint16_t duty){
  SYSCTL->RCGCPWM |= SYSCTL_RCGCPWM_R0; // 1) activate clock for PWM0
                                        // 2) activate clock for Port F
  SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R5;
                                        // allow time for clock to stabilize
  while((SYSCTL->PRGPIO&SYSCTL_PRGPIO_R5) == 0){};
  GPIOF->AFSEL |= 0x04;                 // 3) enable alt funct on PF2
  GPIOF->DEN |= 0x04;                   //    enable digital I/O on PF2
                                        // 4) configure PF2 as PWM0
  GPIOF->PCTL = (GPIOF->PCTL&0xFFFFF0FF)+0x00000600;
  GPIOF->AMSEL &= ~0x04;          //    disable analog functionality on PF2
                                        // allow time for clock to stabilize
  while((SYSCTL->PRPWM&SYSCTL_PRPWM_R0) == 0){};
  PWM0->CC |= PWM_CC_USEPWM;            // 5) use PWM divider
  PWM0->CC &= ~PWM_CC_PWMDIV_M;         //    clear PWM divider field
  PWM0->CC += PWM_CC_PWMDIV_64;         //    configure for /2 divider
  PWM0->_1_CTL = 0;                     // 6) re-loading down-counting mode
  PWM0->_1_GENA = (PWM_1_GENA_ACTCMPAD_ONE|PWM_1_GENA_ACTLOAD_ZERO);
  // PF2 goes low on LOAD
  // PF2 goes high on CMPA down
  PWM0->_1_LOAD = period - 1;           // 7) cycles needed to count down to 0
  PWM0->_1_CMPA = duty - 1;             // 8) count value when output rises
  PWM0->_1_CTL |= PWM_1_CTL_ENABLE;     // 9) start PWM1
  PWM0->ENABLE |= PWM_ENABLE_PWM2EN;   // 10) enable PWM1A/PF2 outputs
} 

// change duty cycle of PWM1A/PF2
// duty is number of PWM clock cycles output is high  (2<=duty<=period-1)
void PWM1A_Duty(uint16_t duty){
  PWM0->_1_CMPA = duty - 1;             // 8) count value when output rises
}


void TurnRight(uint32_t *middleSensor) {
	PWM0B_Duty(SLOW_LOW + 5);
	PWM1A_Duty(SLOW_LOW);
	WaitMs(500); 
	while (*middleSensor < 2500){};
	PWM0B_Duty(NEUTRAL);
	PWM1A_Duty(NEUTRAL);
}

void TurnLeft(uint32_t *middleSensor) {
	PWM0B_Duty(SLOW_HIGH - 20);
	PWM1A_Duty(SLOW_HIGH);
	WaitMs(500);
	while (*middleSensor < 2500){};
	PWM0B_Duty(NEUTRAL);
	PWM1A_Duty(NEUTRAL);
}

void MoveStraight(void) {
	// PF1 / Blue I want to be low
	PWM0B_Duty(SLOW_LOW);
	// PF2 / Yellow I want to be high
	PWM1A_Duty(SLOW_HIGH);
}

void Stop(void) {
	PWM0B_Duty(NEUTRAL);
	PWM1A_Duty(NEUTRAL);
}

void CorrectLeft(void) {	
	// High value PF1
	PWM0B_Duty(SLOW_LOW + 10);
	PWM1A_Duty(SLOW_HIGH);

	WaitMs(50);
	MoveStraight();
}

void CorrectRight(void) {
	// Low value for both
	PWM0B_Duty(SLOW_LOW);
	PWM1A_Duty(SLOW_HIGH - 10);
	
	WaitMs(50);
	MoveStraight();
}
