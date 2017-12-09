/*
 * SystemConfig.h
 *
 *  Created on: Oct 10, 2017
 *      Author: Doggy-Nguyen
 */

#ifndef SYSTEMCONFIG_H_
#define SYSTEMCONFIG_H_

#define LED1_PERIPHERAL						SYSCTL_PERIPH_GPIOF
#define LED1_PORT							GPIO_PORTF_BASE
#define LED1_PIN							GPIO_PIN_1
#define LED1_ON()							GPIOPinWrite(LED1_PORT, LED1_PIN, LED1_PIN)
#define LED1_OFF()							GPIOPinWrite(LED1_PORT, LED1_PIN, 0x00)
#define LED1_TOGGLE()						GPIOPinWrite(LED1_PORT, LED1_PIN, ~GPIOPinRead(LED1_PORT,LED1_PIN))

#define LED2_PERIPHERAL						SYSCTL_PERIPH_GPIOF
#define LED2_PORT							GPIO_PORTF_BASE
#define LED2_PIN							GPIO_PIN_2
#define LED2_ON()							GPIOPinWrite(LED2_PORT, LED2_PIN, LED2_PIN)
#define LED2_OFF()							GPIOPinWrite(LED2_PORT, LED2_PIN, 0x00)
#define LED2_TOGGLE()						GPIOPinWrite(LED2_PORT, LED2_PIN, ~GPIOPinRead(LED2_PORT,LED2_PIN))

#define LED3_PERIPHERAL						SYSCTL_PERIPH_GPIOF
#define LED3_PORT							GPIO_PORTF_BASE
#define LED3_PIN							GPIO_PIN_3
#define LED3_ON()							GPIOPinWrite(LED3_PORT, LED3_PIN, LED3_PIN)
#define LED3_OFF()							GPIOPinWrite(LED3_PORT, LED3_PIN, 0x00)
#define LED3_TOGGLE()						GPIOPinWrite(LED3_PORT, LED3_PIN, ~GPIOPinRead(LED3_PORT,LED3_PIN))

typedef enum{
	SYSTEM_POWER_UP = 0,
	SYSTEM_INITIALIZE,
	SYSTEM_CALIB_SENSOR,
	SYSTEM_SAVE_CALIB_SENSOR,
	SYSTEM_GET_MOTOR_MODEL,
	SYSTEM_ESTIMATE_MOTOR_MODEL,
	SYSTEM_SAVE_MOTOR_MODEL,
	SYSTEM_WAIT_TO_RUN,
	SYSTEM_RUN_SOLVE_MAZE,
	SYSTEM_RUN_IMAGE_PROCESSING,
	SYSTEM_ERROR
} SYSTEM_STATE;

extern void ConfigSystem(void);
extern void LEDDisplayInit(void);
extern SYSTEM_STATE SystemGetState(void);
extern void SystemSetState(SYSTEM_STATE SysState);
extern void system_Process_System_State(void);
extern uint32_t u32_UsrSystemClockGet();
#endif /* SYSTEMCONFIG_H_ */
