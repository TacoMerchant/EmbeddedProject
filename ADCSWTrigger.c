//****************************************************************************
// ADCSWTrigger.c
// Runs on MSP432E401Y
// Provides functions that sample ADC0 inputs PE4, PE5 using SS2
// to be triggered by software and trigger two conversions,
// wait for them to finish, and return the two results.
//
// Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
//    You may use, edit, run or distribute this file
//    as long as the above copyright notice remains
//
//****************************************************************************
#include <stdint.h>
#include "ADCSWTrigger.h"
#include "msp.h"

// There are many choices to make when using the ADC, and many
// different combinations of settings will all do basically the
// same thing.  For simplicity, this function makes some choices
// for you.  When calling this function, be sure that it does
// not conflict with any other software that may be running on
// the microcontroller.  Particularly, ADC0 sample sequencer 2
// is used here because it takes up to four samples, and two
// samples are needed.  Sample sequencer 2 generates a raw
// interrupt when the second conversion is complete, but it is
// not promoted to a controller interrupt.  Software triggers
// the ADC0 conversion and waits for the conversion to finish.
// If somewhat precise periodic measurements are required, the
// software trigger can occur in a periodic interrupt.  This
// approach has the advantage of being simple.  However, it does
// not guarantee real-time.
//
// A better approach would be to use a hardware timer to trigger
// the ADC0 conversion independently from software and generate
// an interrupt when the conversion is finished.  Then, the
// software can transfer the conversion result to memory and
// process it after all measurements are complete.

// This initialization function sets up the ADC according to the
// following parameters.  Any parameters not explicitly listed
// below are not modified:
// ADC clock source: 480 MHz PLL VCO / 15 = 32 MHz
//  (max sample rate: <=2,000,000 samples/second)
//  (assumes PLL_Init() has been called and not changed)
// Sequencer 0 priority: 4th (lowest)
// Sequencer 1 priority: 3rd
// Sequencer 2 priority: 2nd
// Sequencer 3 priority: 1st (highest)
// SS2 triggering event: software trigger
// Hardware oversampling: none
// Voltage reference: internal VDDA and GNDA
// ADC conversion sent to: FIFO (not digital comparator)
// SS2 sample and hold time: 4 ADC clock periods
// SS2 1st sample source: Ain9 (PE4)
// SS2 2nd sample source: Ain8 (PE5)
// SS2 interrupts: enabled but not promoted to controller



void ADC0_InitSWTriggerSeq0(void){
                                  // 1) activate clock for Port E
  SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R4;
                                  // allow time for clock to stabilize
  while((SYSCTL->PRGPIO&SYSCTL_PRGPIO_R4) == 0){};
  GPIOE->AFSEL |= 0x30;     // 2) enable alternate function on PE5, PE4
  GPIOE->DEN &= ~0x30;      // 3) disable digital I/O on PE5, PE4
  GPIOE->AMSEL |= 0x30;     // 4) enable analog functionality on PE5, PE4
                                  // 5) activate clock for ADC0
  SYSCTL->RCGCADC |= SYSCTL_RCGCADC_R0;
                                  // allow time for clock to stabilize
  while((SYSCTL->PRADC&SYSCTL_PRADC_R0) == 0){};
                                  // 6) configure ADC clock source as PLL VCO / 15 = 32 MHz
  ADC0->CC = ((ADC0->CC&~ADC_CC_CLKDIV_M)+(14<<ADC_CC_CLKDIV_S)) |
              ((ADC0->CC&~ADC_CC_CS_M)+ADC_CC_CS_SYSPLL);
  ADC0->SSPRI = 0x0123;          // 7) sequencer 0 is highest priority
                                  // 8) disable sample sequencer 0
  ADC0->ACTSS &= ~ADC_ACTSS_ASEN0;
                                  // 9) configure seq0 for software trigger (default)
  ADC0->EMUX = (ADC0->EMUX&~ADC_EMUX_EM0_M)+ADC_EMUX_EM0_PROCESSOR;
                                  // 10) configure for no hardware oversampling (default)
  ADC0->SAC = (ADC0->SAC&~ADC_SAC_AVG_M)+ADC_SAC_AVG_OFF;
                                  // 11) configure for internal reference (default)
  ADC0->CTL = (ADC0->CTL&~ADC_CTL_VREF_M)+ADC_CTL_VREF_INTERNAL;
                                  // 12) configure for ADC results saved to FIFO (default)
  ADC0->SSOP0 &= ~(ADC_SSOP0_S1DCOP|ADC_SSOP0_S0DCOP);
                                  // 13) configure for 16 ADC clock period S&H (default)
  ADC0->SSTSH0 = ((ADC0->SSTSH0&~ADC_SSTSH0_TSH1_M)+(0<<ADC_SSTSH0_TSH1_S)) |
                  ((ADC0->SSTSH0&~ADC_SSTSH0_TSH0_M)+(0<<ADC_SSTSH0_TSH0_S)) |
                  ((ADC0->SSTSH0&~ADC_SSTSH0_TSH2_M)+(0<<ADC_SSTSH0_TSH2_S)) |
                  ((ADC0->SSTSH0&~ADC_SSTSH0_TSH3_M)+(0<<ADC_SSTSH0_TSH3_S)) |
                  ((ADC0->SSTSH0&~ADC_SSTSH0_TSH4_M)+(0<<ADC_SSTSH0_TSH4_S)) |
                  ((ADC0->SSTSH0&~ADC_SSTSH0_TSH5_M)+(0<<ADC_SSTSH0_TSH5_S)) |
                  ((ADC0->SSTSH0&~ADC_SSTSH0_TSH6_M)+(0<<ADC_SSTSH0_TSH6_S)) |
                  ((ADC0->SSTSH0&~ADC_SSTSH0_TSH7_M)+(0<<ADC_SSTSH0_TSH7_S));
                                  // 14) set channels
  ADC0->SSMUX0 = ((ADC0->SSMUX0&~ADC_SSMUX0_MUX1_M)+(10<<ADC_SSMUX0_MUX1_S)) |
                  ((ADC0->SSMUX0&~ADC_SSMUX0_MUX0_M)+(11<<ADC_SSMUX0_MUX0_S)) |
                  ((ADC0->SSMUX0&~ADC_SSMUX0_MUX0_M)+(16<<ADC_SSMUX0_MUX0_S)) |
                  ((ADC0->SSMUX0&~ADC_SSMUX0_MUX0_M)+(17<<ADC_SSMUX0_MUX0_S)) |
                  ((ADC0->SSMUX0&~ADC_SSMUX0_MUX0_M)+(18<<ADC_SSMUX0_MUX0_S)) |
                  ((ADC0->SSMUX0&~ADC_SSMUX0_MUX0_M)+(19<<ADC_SSMUX0_MUX0_S)) |
                  ((ADC0->SSMUX0&~ADC_SSMUX0_MUX0_M)+(6<<ADC_SSMUX0_MUX0_S)) |
                  ((ADC0->SSMUX0&~ADC_SSMUX0_MUX0_M)+(13<<ADC_SSMUX0_MUX0_S));
  ADC0->SSEMUX0 &= ~0x0;      // 15) SS2 1st and 2nd sample channels in range 0:15
  ADC0->SSCTL0 = 0x0060;         // 16) no TS1 D1 TS0 IE0 END0 D0, yes IE1 END1
  ADC0->IM &= ~ADC_IM_MASK0;     // 17) disable SS0 interrupts
  ADC0->ACTSS |= ADC_ACTSS_ASEN0;// 18) enable sample sequencer 0
}

//------------ADC0_InSeq0------------
// Busy-wait analog to digital conversion
// Input: none
// Output: two 12-bit result of ADC conversions
// Sample channels and clock rate set in initialization function
// software trigger, busy-wait sampling
// data returned by reference
// data[0] is second result (default: ADC8 (PE5)) 0 to 4095
// data[1] is first result  (default: ADC9 (PE4)) 0 to 4095
void ADC0_InSeq0(uint32_t data[2]){
  ADC0->PSSI = 0x0001;            // 1) initiate SS0
  while((ADC0->RIS&0x04)==0){};   // 2) wait for conversion done
  data[7] = ADC0->SSFIFO0&0xFFF;  // 3A) read first result
  data[6] = ADC0->SSFIFO0&0xFFF;  // 3A) read first result
  data[5] = ADC0->SSFIFO0&0xFFF;  // 3A) read first result
  data[4] = ADC0->SSFIFO0&0xFFF;  // 3A) read first result
  data[3] = ADC0->SSFIFO0&0xFFF;  // 3A) read first result
  data[2] = ADC0->SSFIFO0&0xFFF;  // 3A) read first result
  data[1] = ADC0->SSFIFO0&0xFFF;  // 3A) read first result
  data[0] = ADC0->SSFIFO0&0xFFF;  // 3B) read second result
  ADC0->ISC = 0x0001;             // 4) acknowledge completion
}
