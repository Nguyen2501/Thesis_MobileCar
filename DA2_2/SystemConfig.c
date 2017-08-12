/*
 * SystemConfig.c
 *
 *  Created on: Aug 12, 2017
 *      Author: Nguyen
 */

#include "include.h"
#include "driverlib/systick.h"
#include "inc/hw_gpio.h"

static void SysTickIntHandle(void);
static void SystemSystickConfig(uint32_t ui32msinterval);

static SYSTEM_STATE e_SystemState = SYSTEM_BLUE_LED;
static uint32_t ms_Tickcount = 0;
static uint32_t SystemClock = 80000000;
uint32_t ui32GetUserSystemClock();

uint32_t ui32GetUserSystemClock(){
	return SystemClock;
}

void ConfigSystem(void){
	FPUEnable();
	FPULazyStackingEnable();
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	SystemSystickConfig(1);

}

void SystemSystickConfig(uint32_t ui32msinterval){
	SysTickPeriodSet(SysCtlClockGet() * ui32msinterval / 1000);
	SysTickIntRegister(&SysTickIntHandle);
	SysTickIntEnable();
	SysTickEnable();
}

static void SysTickIntHandle(void){
	ms_Tickcount++;
}

void LedInit(void){
	ROM_SysCtlPeripheralEnable(LED_PERIPHERAL);
	ROM_GPIOPinTypeGPIOOutput(LED_PORT, RED_LED);
	ROM_GPIOPinTypeGPIOOutput(LED_PORT, BLUE_LED);
	ROM_GPIOPinTypeGPIOOutput(LED_PORT, GREEN_LED);

	RED_LED_OFF();
	BLUE_LED_OFF();
	GREEN_LED_OFF();
}

SYSTEM_STATE GetSystemState(void){
	return e_SystemState;
}

void SetSystemState(SYSTEM_STATE systemstate){
	e_SystemState = systemstate;
}
void ProcessSystemState(void){
	switch (GetSystemState()) {
		case SYSTEM_BLUE_LED:
			BLUE_LED_ON();
			SysCtlDelay(SysCtlClockGet() / 3);
//			break;
		case SYSTEM_RED_LED:
			RED_LED_ON();
			SysCtlDelay(SysCtlClockGet() / 3);
//		break;
		case SYSTEM_GREEN_LED:
			GREEN_LED_ON();
			SysCtlDelay(SysCtlClockGet() / 3);
//		break;
		default:
			BLUE_LED_OFF();
			RED_LED_OFF();
			GREEN_LED_OFF();
			SysCtlDelay(SysCtlClockGet() / 3);
		break;
	}
}
