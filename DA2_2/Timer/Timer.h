/*
 * Timer.h
 *
 *  Created on: Aug 6, 2017
 *      Author: Nguyen
 */

#ifndef TIMER_TIMER_H_
#define TIMER_TIMER_H_

typedef void (*TIMER_CALLBACK_FUNC)();

typedef unsigned char TIMER_ID;

#define INVALID_TIMER_ID 0xff

void Timer_Init(void);
TIMER_ID TIMER_RegisterEvent(TIMER_CALLBACK_FUNC callback, unsigned long ms);
bool TIMER_UnregisterEvent(TIMER_ID timer_id);

#endif /* TIMER_TIMER_H_ */
