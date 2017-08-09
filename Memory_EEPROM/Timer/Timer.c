/*
 * Timer.c
 *
 *  Created on: Jul 31, 2017
 *      Author: Nguyen
 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "Timer.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#define TIMER_PERIOD_MS 1
#define MAX_TIMEOUT_EVT 10

typedef struct
{
  TIMER_CALLBACK_FUNC callback;
  unsigned long period_cnt;
}TIMEOUT_EVT;

//* Private function prototype ----------------------------------------------*/
void TIMER_ISR(void);
//* Private variables -------------------------------------------------------*/
static TIMEOUT_EVT timer_event_list[MAX_TIMEOUT_EVT];

void Timer_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4);

	TimerConfigure(TIMER4_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER4_BASE, TIMER_A, SysCtlClockGet() * TIMER_PERIOD_MS/ 1000);	//Interval: TIMER_PERIOD_MS(ms)

	TimerIntRegister(TIMER4_BASE, TIMER_A, &TIMER_ISR);
	IntEnable(INT_TIMER4A);
	TimerIntEnable(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
	TimerIntClear(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
	TimerControlStall(TIMER4_BASE, TIMER_A, false);
	TimerEnable(TIMER4_BASE, TIMER_A);
}

/**
 * @brief Register event
 * @param callback function name
 * @param ms event timeout
 * @return timer ID
 */
TIMER_ID TIMER_RegisterEvent(TIMER_CALLBACK_FUNC callback, unsigned long ms)
{
    int i;
    for(i=0; i< MAX_TIMEOUT_EVT; i++)
    {
      if((timer_event_list[i].period_cnt == 0) && (timer_event_list[i].callback == NULL)) break;
    }
    if(i == MAX_TIMEOUT_EVT)
    	return INVALID_TIMER_ID;

    timer_event_list[i].period_cnt = (unsigned long)(ms/TIMER_PERIOD_MS);
    timer_event_list[i].callback = callback;

    return (TIMER_ID)i;
}

bool TIMER_UnregisterEvent(TIMER_ID timer_id)
{
	bool ret = false;
    if(timer_id < MAX_TIMEOUT_EVT)
    {
        timer_event_list[timer_id].period_cnt = 0;
        timer_event_list[timer_id].callback = NULL;
        ret = true;
    }
    return ret;
}

void TIMER_ISR(void)
{
    int i;
	TimerIntClear(TIMER4_BASE, TIMER_TIMA_TIMEOUT);

	for(i=0; i<MAX_TIMEOUT_EVT; i++)
	{
		if(timer_event_list[i].period_cnt > 0)
		{
			timer_event_list[i].period_cnt--;
			if(timer_event_list[i].period_cnt == 0 && timer_event_list[i].callback != NULL)
			{
				(timer_event_list[i].callback)();
				/*
				 * Only clear timeout callback when period equal to 0
				 * Another callback could be register in current timeout callback
				 */
				if (timer_event_list[i].period_cnt == 0)
					timer_event_list[i].callback = NULL;
			}
		}
	}

}