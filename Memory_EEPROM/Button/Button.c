/*
 * Button.c
 *
 *  Created on: Aug 6, 2017
 *      Author: Nguyen
 */

#include "../include.h"
#include "Button.h"


static void ButtonsISR(void);
static void (*Button_right_callback)(), (*Button_left_callback)();
static void ButtonDebounceCallback(void);
static void button_Stoptimeout(void);
static void button_Runtimeout(TIMER_CALLBACK_FUNC TimeoutCallback, uint32_t msTime);

static uint8_t Button_pressed = 0;
static TIMER_ID button_TimerID = INVALID_TIMER_ID;

void Button_Init(void){
	SysCtlPeripheralEnable(BUTTON_PERIPHERAL);
	HWREG(BUTTON_PORT + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(BUTTON_PORT + GPIO_O_CR) = 0x01;
//	HWREG(BUTTON_PORT + GPIO_O_AFSEL) &= ~0x01;
	HWREG(BUTTON_PORT + GPIO_O_AFSEL) = 0;
//	HWREG(BUTTON_PORT + GPIO_O_LOCK) = 0;


	GPIOPinTypeGPIOInput(BUTTON_PORT, LEFT_BUTTON | RIGHT_BUTTON);
	GPIOPadConfigSet(BUTTON_PORT, LEFT_BUTTON | RIGHT_BUTTON, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD_WPU);

	GPIOIntTypeSet(BUTTON_PORT, LEFT_BUTTON | RIGHT_BUTTON, GPIO_FALLING_EDGE);
	GPIOIntRegister(BUTTON_PORT, &ButtonsISR);
	IntEnable(INT_GPIOF);
	GPIOIntEnable(BUTTON_PORT, LEFT_BUTTON | RIGHT_BUTTON);
	GPIOIntClear(BUTTON_PORT, LEFT_BUTTON | RIGHT_BUTTON);
}

/**
 * @brief Register button callback
 * @param ButtonSelect button
 * @param ButtonCallBack pointer to callback function
 * @return registry state
 */
bool ButtonRegisterCallback(BUTTON_TYPE ButtonSelect, void (*ButtonCallback)())
{
	if (ButtonSelect == BUTTON_RIGHT)
	{
		Button_right_callback = ButtonCallback;
		return true;
	}
	else if (ButtonSelect == BUTTON_LEFT)
	{
		Button_left_callback = ButtonCallback;
		return true;
	}
	return false;
}

/**
 * @brief Button isr
 */
static void ButtonsISR(void){
	uint32_t ui32_IntStatus;
	ui32_IntStatus = GPIOIntStatus(BUTTON_PORT, true);
	GPIOIntClear(BUTTON_PORT, ui32_IntStatus);
	if (ui32_IntStatus & RIGHT_BUTTON) {
		Button_pressed |= 0x01 << RIGHT_BUTTON;
	}
	if (ui32_IntStatus & LEFT_BUTTON) {
		Button_pressed |= 0x01 << LEFT_BUTTON;
	}

	button_Runtimeout(&ButtonDebounceCallback, BUTTON_DEBOUNCE_MS);
}

/**
 * @brief Button debounce
 */
static void ButtonDebounceCallback(void){
	button_TimerID = INVALID_TIMER_ID;
	if ((Button_pressed & (0x01 << RIGHT_BUTTON)) && (Button_right_callback != NULL)) {
		if (GPIOPinRead(BUTTON_PORT, RIGHT_BUTTON) == 0) {
			Button_right_callback();
//			RED_LED_TOGGLE();
//			SysCtlDelay(SysCtlClockGet() / 3);
		}
	}

	if ((Button_pressed & (0x01 << LEFT_BUTTON)) && (Button_left_callback != NULL)) {
		if (GPIOPinRead(BUTTON_PORT, LEFT_BUTTON) == 0) {
			Button_left_callback();
//			BLUE_LED_TOGGLE();
//			SysCtlDelay(SysCtlClockGet() / 3);
		}
	}
	Button_pressed = 0;
}

static void button_Stoptimeout(void)
{
	if (button_TimerID != INVALID_TIMER_ID)
		TIMER_UnregisterEvent(button_TimerID);
	button_TimerID = INVALID_TIMER_ID;
}

static void button_Runtimeout(TIMER_CALLBACK_FUNC TimeoutCallback, uint32_t msTime)
{
	button_Stoptimeout();
	button_TimerID = TIMER_RegisterEvent(TimeoutCallback, msTime);
}

void ButtonLeftHandler(void){
	BLUE_LED_ON();
	SysCtlDelay(SysCtlClockGet() / 3);
	BLUE_LED_OFF();
	SysCtlDelay(SysCtlClockGet() / 3);
}


void ButtonRightHandler(void){
//	GPIOPinWrite(LED_PORT, GPIO_PIN_5, 1);
	RED_LED_ON();
	SysCtlDelay(SysCtlClockGet() / 3);
	RED_LED_OFF();
	SysCtlDelay(SysCtlClockGet() / 3);
}
