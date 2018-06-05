//****************************************************************************
// PWMTestMain.c                                                             *
// Runs on MSP432E401Y                                                       *
// Program for testing routines in PWM.c                                     *
//   First call one of the Init functions.                                   *
//   Call a Duty function to change the duty cycle                           *
//                                                                           *
// Use PWM0B/PF1 and PWM1A/PF2 to generate pulse-width modulated outputs.    *
//                                                                           *
//   period is a 16-bit number of PWM clock cycles in one period (3<=period) *
//   period for PF0 and PF1 must be the same,PF2 can differ from PF0&PF1     *
//   duty is number of PWM clock cycles output is high  (2<=duty<=period-1)  *
//   PWM clock rate = processor clock rate/SYSCTL_CC_PWMDIV_64               *
//                  = BusClock/64                                            *
//                  = 120 MHz/64 = 1875000 Hz                                *
// Created 5/28/18    L.Aamodt                                               *
//****************************************************************************
#include <stdint.h>
#include "PLL.h"


void PWM0A_Init(uint16_t period, uint16_t duty);
void PWM0B_Init(uint16_t period, uint16_t duty);
void PWM1A_Init(uint16_t period, uint16_t duty);
void PWM0A_Duty(uint16_t duty);
void PWM0B_Duty(uint16_t duty);
void PWM1A_Duty(uint16_t duty);

int main(void){
	uint32_t count;
	
  PLL_Init();                   // bus clock at 120 MHz
  PWM1A_Init(37500, 2812);      // initialize PWM1A/PF2, 50Hz, 1500us pulse width
  PWM0B_Init(37500, 20000);      // initialize PWM0B/PF1, 50Hz, 1500us pulse width

  while(1){
		count ++;                   // do nothing
  }
}
