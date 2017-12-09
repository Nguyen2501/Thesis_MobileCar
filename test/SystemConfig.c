/*
 * SystemConfig.c
 *
 *  Created on: Oct 10, 2017
 *      Author: Nguyen
 */

#include "include.h"
#include "inc/hw_gpio.h"
#include "driverlib/systick.h"

//* Private function prototype ----------------------------------------------*/
static void SysTickIntHandle(void);
static void system_SystickConfig(uint32_t ui32_msInterval);
//* Private variables -------------------------------------------------------*/
static SYSTEM_STATE e_SystemState = SYSTEM_POWER_UP;
static uint32_t ms_Tickcount = 0;
static uint32_t systemClock = 80000000;
uint32_t u32_UsrSystemClockGet();

uint32_t u32_UsrSystemClockGet()
{
	return systemClock;
}

void ConfigSystem(void){
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
	FPUEnable();
	FPULazyStackingEnable();
	system_SystickConfig(1);
}

static void system_SystickConfig(uint32_t ui32_msInterval){
	SysTickPeriodSet(SysCtlClockGet() * ui32_msInterval / 1000);
	SysTickIntRegister(&SysTickIntHandle);
	SysTickIntEnable();
	SysTickEnable();
}

static void SysTickIntHandle(void)
{
	ms_Tickcount++;
}

void LEDDisplayInit(void){
	SysCtlPeripheralEnable(LED1_PERIPHERAL);
	SysCtlPeripheralEnable(LED2_PERIPHERAL);
	SysCtlPeripheralEnable(LED3_PERIPHERAL);
	GPIOPinTypeGPIOOutput(LED1_PORT, LED1_PIN);
	GPIOPinTypeGPIOOutput(LED2_PORT, LED2_PIN);
	GPIOPinTypeGPIOOutput(LED3_PORT, LED3_PIN);

	LED1_OFF();
	LED2_OFF();
	LED3_OFF();
}

SYSTEM_STATE SystemGetState(void)
{
	return e_SystemState;
}

void SystemSetState(SYSTEM_STATE SysState)
{
	e_SystemState = SysState;
}

void system_Process_System_State(void)
{
	switch (SystemGetState())
	{
		case SYSTEM_POWER_UP:
			break;
		case SYSTEM_INITIALIZE:
			break;
		case SYSTEM_CALIB_SENSOR:
			break;
		case SYSTEM_SAVE_CALIB_SENSOR:
			break;
		case SYSTEM_ESTIMATE_MOTOR_MODEL:
			ProcessSpeedControl();
			break;
		case SYSTEM_SAVE_MOTOR_MODEL:
			break;
		case SYSTEM_WAIT_TO_RUN:
			break;
		case SYSTEM_RUN_SOLVE_MAZE:
//			ProcessSpeedControl();
			break;
		case SYSTEM_RUN_IMAGE_PROCESSING:
			LED1_ON();
			PIDLineFollowProcess();
			ProcessSpeedControl();
			break;
		case SYSTEM_ERROR:
			IntMasterDisable();
			while (1)
			{
				LED1_ON();
				LED2_ON();
				LED3_ON();
				SysCtlDelay(SysCtlClockGet() / 3);
				LED1_OFF();
				LED2_OFF();
				LED3_OFF();
				SysCtlDelay(SysCtlClockGet() / 3);
			}
//			break;
	}
}
