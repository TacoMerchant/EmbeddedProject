/*
 * Timer.c
 *
 *  Created on: May 25, 2018
 *      Author: Jacob Priddy
 */

#include "Timer.h"

uint32_t volatile totalElapsed30Seconds = 0;

void TIMER0A_IRQHandler(void)
{
    TIMER0->ICR  |= BIT0;
    totalElapsed30Seconds++;
    return;
}

void Timer0Init(void)
{
    // Enable Timer0
    SYSCTL->RCGCTIMER |= SYSCTL_RCGCTIMER_R0;
    while(!(SYSCTL->PRTIMER & SYSCTL_RCGCTIMER_R0));

    /* Initialize Timer0A */

    // Turn on clock for Timer0A
    TIMER0->CTL = 0;

    // 32 bit mode
    TIMER0->CFG = TIMER_CFG_32_BIT_TIMER;

    // Periodic timer
    TIMER0->TAMR = TIMER_TAMR_TAMR_PERIOD;

    TIMER0->TAILR = TIMER_COUNT_FROM_VALUE;

    // Don't want a prescaler
    TIMER0->TAPR = 0;

    TIMER0->IMR = TIMER_IMR_TATOIM;

    // Enable timer interrupts
    NVIC_EN0 |= 0x00080000;

    // Enable timer
    TIMER0->CTL |= TIMER_CTL_TAEN;
}


void WaitMs(uint32_t ms)
{
    uint32_t timerCount = TIMER0->TAR;
    uint32_t doneVal;
    uint32_t numToCount;
    uint32_t doneElapsedTime = totalElapsed30Seconds;

    // greater than a 30 second wait
    if (ms > 30000)
    {
        doneElapsedTime += ms / 30000;
        ms = ms % 30000;
    }

    numToCount = ms * CYCLES_IN_ONE_MS;

    if (numToCount > timerCount)
    {
        doneElapsedTime++;
        doneVal = TIMER_COUNT_FROM_VALUE - numToCount + timerCount;
    }
    else {
        doneVal = timerCount - numToCount;
    }

    while(TIMER0->TAR > doneVal || (doneElapsedTime < totalElapsed30Seconds));

    return;
}

void WaitUs(uint32_t us)
{
    uint32_t timerCount = TIMER0->TAR;
    uint32_t doneVal;
    uint32_t numToCount;
    uint32_t doneElapsedTime = totalElapsed30Seconds;

    // greater than a 30 second wait
    if (us > 30000000)
    {
        doneElapsedTime += us / 30000000;
        us = us % 30000000;
    }

    numToCount = us * CYCLES_IN_ONE_US;

    if (numToCount > timerCount)
    {
        doneElapsedTime++;
        doneVal = TIMER_COUNT_FROM_VALUE - numToCount + timerCount;
    }
    else {
        doneVal = timerCount - numToCount;
    }

    while(TIMER0->TAR > doneVal || (doneElapsedTime < totalElapsed30Seconds));

    return;
}

uint32_t getElapsedTimeInS(void)
{
    return totalElapsed30Seconds * 30 + TIMER_COUNT_FROM_VALUE / CYCLES_IN_ONE_S - TIMER0->TAR / CYCLES_IN_ONE_S;
}

uint32_t getElapsedTimeInMs(void)
{
    return totalElapsed30Seconds * 30000 + TIMER_COUNT_FROM_VALUE / CYCLES_IN_ONE_MS - TIMER0->TAR / CYCLES_IN_ONE_MS;
}

uint32_t getElapsedTimeInUs(void)
{
    return totalElapsed30Seconds * 30000000 + TIMER_COUNT_FROM_VALUE / CYCLES_IN_ONE_US - TIMER0->TAR / CYCLES_IN_ONE_US;
}
