/*
 * Timer.h
 *
 *  Created on: May 25, 2018
 *      Author: Jacob Priddy
 *
 * Simple timer library with wait functions that wait about the specified time +/- 120 ish clock cycles.
 * Assumes the system is clocked at 120MHz.
 *
 * This was made because it is a lot more accurate than waiting in a spinning counting loop
 * for the approximate amount of time.
 */

#ifndef TIMER_H_
#define TIMER_H_
#include "msp.h"

#define NVIC_EN0 (*((volatile uint32_t *)0xE000E100))

// Assume clock speed of 120 MHz
#define CYCLES_IN_ONE_US 120
#define CYCLES_IN_ONE_MS 120000
#define CYCLES_IN_ONE_S  120000000

// Assume 120 MHz clock
// 1200000/s * 30 s = 0xD693A400
// So the timer counter will increment every 30 seconds.
#define TIMER_COUNT_FROM_VALUE 0xD693A400

// Just increments the every 30 second counter
void TIMER0A_IRQHandler(void);

/*
 * Assumes Clock rate is 120MHz and uses Timer0A in 32 bit mode.
 */
void Timer0Init(void);

/*
 * Waits the specified time in milliseconds.
 * This function takes about 120 clock cycles worth of extra time than you put in
 */
void WaitMs(uint32_t ms);

/*
 * Waits the specified time in microseconds.
 * This function takes about 120 clock cycles worth of extra time than you put in
 */
void WaitUs(uint32_t us);

/*
 * Returns the time elapsed since Timer Initialization in seconds
 */
uint32_t getElapsedTimeInS(void);

/*
 * Returns the time elapsed since Timer Initialization in milliseconds
 */
uint32_t getElapsedTimeInMs(void);

/*
 * Returns the time elapsed since Timer Initialization in microseconds
 */
uint32_t getElapsedTimeInUs(void);

// Every 30 second counter. If this gets messed with, it may make getElapsedTime functions inaccurate.
extern uint32_t volatile totalElapsed30Seconds;

#endif /* TIMER_H_ */
