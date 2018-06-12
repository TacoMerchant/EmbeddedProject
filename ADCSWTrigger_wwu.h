#ifndef ADCSWTRIGGER_WWU_H
#define ADCSWTRIGGER_WWU_H

#include "msp.h"
#include <stdint.h>
//#include "PLL.h"
//#include "UART.h"

void ADC0_InitSWTriggerWWU(void);
void ADC0_InSeq9(uint32_t data[]);
void Load_9registers(uint32_t data[]);
void Init_Switches(void);


#endif
