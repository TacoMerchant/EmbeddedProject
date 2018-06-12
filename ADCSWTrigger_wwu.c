//****************************************************************************
// ADCSWTrigger_wwu.c                                                        *
// Runs on MSP432E401Y                                                       *
// Has a function that samples ADC0 inputs 10,11,16,17,18,19,13,6,4 using    *
// sequencers 1 & 0.  Conversion is triggered by software which then         *
// waits for conversion to complete. 9 values are returned.                  *
// Interrupts are not used.                                                  *
//                                                                           *
// Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu            *
//    You may use, edit, run or distribute this file                         *
//    as long as the above copyright notice remains                          *
//                                                                           *
// Based on ADCSWTrigger.c written by J.W. Valvano                           *
// Converted to standard CMSIS notation and the number of channels           *
//   expanded to nine.                                                       *
// The ADC0_InSeq9() function takes about 80us to run based on a 120Mhz      *
//   clock and the initialization shown below                                *
// L.Aamodt   5/28/18                                                        *
//****************************************************************************
#include <stdint.h>
#include "msp432e401y.h"
#include "ADCSWTrigger_wwu.h"

// ADC0 sample sequencers 0 and 1 are used.  Sequencer 0 is used
// because it can sample up to eight channels.  Sequencer 1 can sample
// up to 4 channels although it is only sampling one channel here.
// As programmed, sample sequencer 0 generates a raw interrupt when 
// the eighth conversion is complete, but the interrupt is not promoted 
// to system interrupt status.  Sequencer 1 is programmed to sample one
// channel and to also create a raw interrupt upon completion. Sequences
// 0 and 1 are started simultaneously by software.  Sequence 0 is programmed
// to have higher priority and runs first.  Sequence 1 runs at the completion
// of sequncer 0.  Software that initiated conversions waits for 
// A/D conversions to complete (no system interrupts are used). 
//
// The initialization function sets up the ADC according to the
// following parameters.  Any parameters not explicitly listed
// below are not modified:
// ADC clock source: 480 MHz PLL VCO / 15 = 32 MHz
//  (max sample rate: <=2,000,000 samples/second)
//  (assumes PLL_Init() has been called and not changed)
// SS0 & SS1 triggering event: software trigger
// Hardware oversampling: none
// Voltage reference: internal VDDA and GNDA
// ADC conversion sent to: FIFO (not digital comparator)
// SS0 sample and hold time: 128 ADC clock periods
// SS0 1st sample source: Ain10 (PB4)
// SS0 2nd sample source: Ain11 (PB5)
// SS0 3rd sample source: Ain16 (PK0)
// SS0 4th sample source: Ain17 (PK1)
// SS0 5th sample source: Ain18 (PK2)
// SS0 6th sample source: Ain19 (PK3)
// SS0 7th sample source: Ain13 (PD2)
// SS0 8th sample source: Ain6  (PD5)
// SS1 9th sample source: Ain4  (PD7)
// SS0 & SS1 interrupts: enabled but not promoted to controller


void ADC0_InitSWTriggerWWU(void){
                                  // 1) activate clock for Ports B, D, and K
	SYSCTL->RCGCGPIO |= (SYSCTL_RCGCGPIO_R1 | SYSCTL_RCGCGPIO_R3 | SYSCTL_RCGCGPIO_R9);
                                  // allow time for clock to stabilize
  while((SYSCTL->PRGPIO&(SYSCTL_PRGPIO_R9|SYSCTL_PRGPIO_R3|SYSCTL_PRGPIO_R1)) == 0){};
         // set up GPIO for Ports B, D, and K
	GPIOB->AFSEL |= 0x30;           // 2) Enable alternate function on portB pins 4 & 5
	GPIOB->DEN   &= ~0x30;          // 3) Disable digital I/O
	GPIOB->AMSEL |= 0x30;           // 4) enable alternate function
		
	GPIOD->AFSEL |= 0xA4;           //    Enable alternate funct on portD pins 7,5 & 2
	GPIOD->DEN   &= ~0xA4;
	GPIOD->AMSEL |=	0xA4;
 
	GPIOK->AFSEL |= 0x0F;           //    Enable alternate funct on port K pins 3,2,1,0
	GPIOK->DEN   &= ~0x0F;
	GPIOK->AMSEL |= 0x0F;
                               		// 5) activate clock for ADC0
	SYSCTL->RCGCADC |= SYSCTL_RCGCADC_R0;
                                  // allow time for clock to stabilize
	while((SYSCTL->PRADC&SYSCTL_PRADC_R0) == 0){};
                                  // 6) configure ADC clock source as PLL VCO / 15 = 32 MHz
  ADC0->CC = ((ADC0->CC&~ADC_CC_CLKDIV_M)+(14<<ADC_CC_CLKDIV_S)) |
              ((ADC0->CC&~ADC_CC_CS_M)+ADC_CC_CS_SYSPLL);		

  ADC0->SSPRI = 0x3210;           // 7) sequencer 0 has highest priority (default)
                                  // 8) disable sample sequencers 0 & 1 before setup
	ADC0->ACTSS &= ~ADC_ACTSS_ASEN0;
	ADC0->ACTSS &= ~ADC_ACTSS_ASEN1;
                                  // 9) configure seq0 & seq1 for software trigger
	ADC0->EMUX = (ADC0->EMUX&~ADC_EMUX_EM0_M)+ADC_EMUX_EM0_PROCESSOR;
	ADC0->EMUX = (ADC0->EMUX&~ADC_EMUX_EM1_M)+ADC_EMUX_EM1_PROCESSOR;
                                  // 10) configure for no hardware oversampling (default)
  ADC0->SAC = (ADC0->SAC & ~ADC_SAC_AVG_M)+ADC_SAC_AVG_OFF;
                                  // 11) configure for internal reference (default)
                                  //    thus VDDA and GNDA are the voltage references
  ADC0->CTL = (ADC0->CTL&~ADC_CTL_VREF_M)+ADC_CTL_VREF_INTERNAL;
  
  ADC0->SSOP0 = 0x00000000;       // 12) configure for ADC results saved to FIFO (default)
	ADC0->SSOP1 = 0x00000000;
  
  ADC0->SSTSH0 = 0xAAAAAAAA;      // 13) configure S&H interval for each channel
  ADC0->SSTSH1 = 0x0000000A;      //       sample for 128 A/D clocks

	ADC0->SSMUX0  = 0x6D3210BA;     // 14) set channels (hi to lo) 6,13,19,18,17,16,11,10
  ADC0->SSEMUX0 = 0x00111100;     // 15) EMUX0 bits set to one to enable channels 16 and up

  ADC0->SSMUX1  = 0x00000004;     // 14a) set channel 4 to be converted
	ADC0->SSEMUX0 = 0x00000000;     // 15a) high order bit of channel number set to zero

  ADC0->SSCTL0 = 0x60000000;      // 16) 8th sample on SEQ0 is last; interrupt allowed
	ADC0->SSCTL1 = 0x00000006;      //     First sample on SEQ1 is last; interrupt allowed
	
  ADC0->IM &= ~ADC_IM_MASK0;      // 17) disable raw SS0 & SS1 interrupts from
	ADC0->IM &= ~ADC_IM_MASK1;      //        creating a system interrupt
	
	ADC0->ACTSS |= ADC_ACTSS_ASEN0; // 18) re-enable sample sequencers 0 & 1
	ADC0->ACTSS |= ADC_ACTSS_ASEN1;
}

//------------ADC0_InSeq9------------
// Busy-wait analog to digital conversion of 9 analog signals
// Input: none
// Output: nine 12-bit results from ADC conversions
// Sample channels and clock rate set in initialization function
// Software trigger, busy-wait sampling
// Data returned by reference
// data[0] is the first result   (ADC 10 (PB4)) 0 to 4095
// data[1] is the second result  (ADC 11 (PB5)) 0 to 4095
// data[2] is the third result   (ADC 16 (Pk0)) 0 to 4095
// data[3] is the forth result   (ADC 17 (Pk1)) 0 to 4095
// data[4] is the fifth result   (ADC 18 (Pk2)) 0 to 4095
// data[5] is the sixth result   (ADC 19 (Pk3)) 0 to 4095
// data[6] is the seventh result (ADC 13 (PD2)) 0 to 4095
// data[7] is the eighth result  (ADC 6  (PD5)) 0 to 4095
// data[8] is the ninth result   (ADC 4  (PD7)) 0 to 4095

void ADC0_InSeq9(uint32_t data[]){
                                  // 1) Initiate SS0 and SS1 sequences
	ADC0->PSSI = ADC_PSSI_SS0 | ADC_PSSI_SS1;
                                  // 2) wait for conversion done
  while((ADC0->RIS & ADC_PSSI_SS1)==0){}; // wait on sequence 1
		
  data[0] = ADC0->SSFIFO0&0xFFF;  // 3a) read first result
  data[1] = ADC0->SSFIFO0&0xFFF;  // 3b) read second result
  data[2] = ADC0->SSFIFO0&0xFFF;  // 3c) read third result
  data[3] = ADC0->SSFIFO0&0xFFF;  // 3d) read forth result
  data[4] = ADC0->SSFIFO0&0xFFF;  // 3e) read fifth result
  data[5] = ADC0->SSFIFO0&0xFFF;  // 3f) read sixth result
  data[6] = ADC0->SSFIFO0&0xFFF;  // 3g) read seventh result
  data[7] = ADC0->SSFIFO0&0xFFF;  // 3h) read eighth result
  data[8] = ADC0->SSFIFO1&0xFFF;  // 3i) read nineth result		
		
                                  // 4) Acknowledge completion
  ADC0->ISC = ADC_ISC_IN0 | ADC_ISC_IN1; 
}
