/*
 * SystemConfig.h
 *
 *  Created on: Aug 12, 2017
 *      Author: Nguyen
 */

#ifndef SYSTEMCONFIG_H_
#define SYSTEMCONFIG_H_

typedef enum
{
	SYSTEM_BLUE_LED,
	SYSTEM_RED_LED,
	SYSTEM_GREEN_LED
}SYSTEM_STATE;

extern void ConfigSystem(void);
extern SYSTEM_STATE GetSystemState(void);
extern void LedInit(void);
extern void SetSystemState(SYSTEM_STATE systemstate);
extern void ProcessSystemState(void);
extern uint32_t ui32GetUserSystemClock();

#endif /* SYSTEMCONFIG_H_ */
