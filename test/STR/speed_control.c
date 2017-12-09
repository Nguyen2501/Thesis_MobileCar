/**
 *	Raise your ARM 2015 sample code http://raiseyourarm.com/
 *	Author: Pay it forward club
 *	http://www.payitforward.edu.vn
 *	version 0.0.1
 */

/**
 * @file	speed control.c
 * @brief	speed control
 */

#include "../include.h"
#include "speed_control.h"

//* Private function prototype ----------------------------------------------*/
static void Config_PWM(void);
//void SetPWM(uint32_t ulFrequency, int32_t ucDutyCycle);
void SetPWMCCW_LeftMotor(uint32_t ulFrequency, int32_t ucDutyCycle);
void SetPWMCW_LeftMotor(uint32_t ulFrequency, int32_t ucDutyCycle);
void SetPWMCCW_RightMotor(uint32_t ulFrequency, int32_t ucDutyCycle);
void SetPWMCW_RightMotor(uint32_t ulFrequency, int32_t ucDutyCycle);
static void speed_update_setpoint(void);
static void speed_control_runtimeout(uint32_t ms);
static void speed_control_stoptimeout(void);

//* Private variables -------------------------------------------------------*/
static real_T Theta[4], Theta_[4] = {-1, 1, 1, 1};
static real_T Theta2[4], Theta2_[4] = {-1, 1, 1, 1};
static int32_t SetPoint[2] = {0, 0};
static int32_t RealSpeedSet[2] = {0, 0};
static float udk = 0;
static TIMER_ID speed_control_timID = INVALID_TIMER_ID;
float udkmain;
uint32_t Period;
int32_t Dutycycle;
void SpeedControlInit(void)
{
	Control_initialize();
	Config_PWM();
	StopPWM(DEFAULT);
}

/**
 * @brief Init battery sense
 * @note this function must call to calculate speed control
 */
void ProcessSpeedControl(void)
{
	int32_t Velocity[2];
//	SetPoint = 250;
	if (QeiGetVelocity(0, &Velocity[0]) == true)
	{
		udk = STR_Indirect(Theta, RealSpeedSet[0], Velocity[0]);
		udkmain = udk;
//		SetPWM(DEFAULT, udk);
		SetPWMCW_RightMotor(DEFAULT, udk);
		Uocluong(udk, Velocity[0], Theta, Theta_);
	}
	if (QeiGetVelocity(1, &Velocity[1]) == true) {
		udk = STR_Indirect2(Theta2, RealSpeedSet[1], Velocity[1]);
//		SetPWM(DEFAULT)
		SetPWMCW_LeftMotor(DEFAULT, udk);
		Uocluong2(udk, Velocity[1], Theta2, Theta2_);
	}
}

static void Config_PWM(void)
{
    // Enable Peripheral Clocks
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB2_T3CCP0);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinConfigure(GPIO_PB3_T3CCP1);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_3);

//    Configure timer
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);

    TimerConfigure(TIMER3_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM);
    TimerLoadSet(TIMER3_BASE, TIMER_A, DEFAULT);
    TimerMatchSet(TIMER3_BASE, TIMER_A, DEFAULT); // PWM
    TimerControlLevel(TIMER3_BASE, TIMER_A, true);
    TimerEnable(TIMER3_BASE, TIMER_A);

    TimerLoadSet(TIMER3_BASE, TIMER_B, DEFAULT);
    TimerMatchSet(TIMER3_BASE, TIMER_B, DEFAULT); // PWM
    TimerControlLevel(TIMER3_BASE, TIMER_B, true);
    TimerEnable(TIMER3_BASE, TIMER_B);

    // Enable Peripheral Clocks
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB6_T0CCP0);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_6);
    GPIOPinConfigure(GPIO_PB7_T0CCP1);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_7);
//    Configure timer
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM);
    TimerLoadSet(TIMER0_BASE, TIMER_A, DEFAULT);
    TimerMatchSet(TIMER0_BASE, TIMER_A, DEFAULT); // PWM
    TimerControlLevel(TIMER0_BASE, TIMER_A, true);
    TimerEnable(TIMER0_BASE, TIMER_A);

    TimerLoadSet(TIMER0_BASE, TIMER_B, DEFAULT);
    TimerMatchSet(TIMER0_BASE, TIMER_B, DEFAULT); // PWM
    TimerControlLevel(TIMER0_BASE, TIMER_B, true);
    TimerEnable(TIMER0_BASE, TIMER_B);
}

void SetPWMCW_RightMotor(uint32_t ulFrequency, int32_t ucDutyCycle)
{
	uint32_t ulPeriod;
	ulPeriod = SysCtlClockGet() / ulFrequency;
	Period = ulPeriod;
	TimerLoadSet(PWM_MOTOR_RIGHT_A, ulPeriod);
	TimerLoadSet(PWM_MOTOR_RIGHT_B, DEFAULT);
	if (ucDutyCycle > 90)
		ucDutyCycle = 90;
	else if (ucDutyCycle < -90)
		ucDutyCycle = -90;
	Dutycycle = (100 + ucDutyCycle) * ulPeriod / 200 - 1;
	TimerMatchSet(PWM_MOTOR_RIGHT_B, DEFAULT);
	TimerMatchSet(PWM_MOTOR_RIGHT_A, (100 + ucDutyCycle) * ulPeriod / 200 - 1);
}

void SetPWMCCW_RightMotor(uint32_t ulFrequency, int32_t ucDutyCycle)
{
	uint32_t ulPeriod;
	ulPeriod = SysCtlClockGet() / ulFrequency;
	Period = ulPeriod;
	TimerLoadSet(PWM_MOTOR_RIGHT_A, DEFAULT);
	TimerLoadSet(PWM_MOTOR_RIGHT_B, ulPeriod);
	if (ucDutyCycle > 90)
		ucDutyCycle = 90;
	else if (ucDutyCycle < -90)
		ucDutyCycle = -90;
	Dutycycle = (100 + ucDutyCycle) * ulPeriod / 200 - 1;
	TimerMatchSet(PWM_MOTOR_RIGHT_B, (100 + ucDutyCycle) * ulPeriod / 200 - 1);
	TimerMatchSet(PWM_MOTOR_RIGHT_A, DEFAULT);
}

void SetPWMCW_LeftMotor(uint32_t ulFrequency, int32_t ucDutyCycle)
{
	uint32_t ulPeriod;
	ulPeriod = SysCtlClockGet() / ulFrequency;
	Period = ulPeriod;
	TimerLoadSet(PWM_MOTOR_LEFT_A, DEFAULT);
	TimerLoadSet(PWM_MOTOR_LEFT_B, ulPeriod);
	if (ucDutyCycle > 90)
		ucDutyCycle = 90;
	else if (ucDutyCycle < -90)
		ucDutyCycle = -90;
	Dutycycle = (100 + ucDutyCycle) * ulPeriod / 200 - 1;
	TimerMatchSet(PWM_MOTOR_LEFT_A, DEFAULT);
	TimerMatchSet(PWM_MOTOR_LEFT_B, (100 + ucDutyCycle) * ulPeriod / 200 - 1);
}

void SetPWMCCW_LeftMotor(uint32_t ulFrequency, int32_t ucDutyCycle)
{
	uint32_t ulPeriod;
	ulPeriod = SysCtlClockGet() / ulFrequency;
	Period = ulPeriod;
	TimerLoadSet(PWM_MOTOR_LEFT_A, ulPeriod);
	TimerLoadSet(PWM_MOTOR_LEFT_B, DEFAULT);
	if (ucDutyCycle > 90)
		ucDutyCycle = 90;
	else if (ucDutyCycle < -90)
		ucDutyCycle = -90;
	Dutycycle = (100 + ucDutyCycle) * ulPeriod / 200 - 1;
	TimerMatchSet(PWM_MOTOR_LEFT_B, DEFAULT);
	TimerMatchSet(PWM_MOTOR_LEFT_A, (100 + ucDutyCycle) * ulPeriod / 200 - 1);
}

void StopPWM(uint32_t ulFrequency){
	uint32_t ulPeriod;
	ulPeriod = SysCtlClockGet() / ulFrequency;
	TimerLoadSet(PWM_MOTOR_LEFT_A, ulPeriod);
	TimerMatchSet(PWM_MOTOR_LEFT_A, ulPeriod);
	TimerLoadSet(PWM_MOTOR_LEFT_B, ulPeriod);
	TimerMatchSet(PWM_MOTOR_LEFT_B, ulPeriod);
	TimerLoadSet(PWM_MOTOR_RIGHT_A, ulPeriod);
	TimerMatchSet(PWM_MOTOR_RIGHT_A, ulPeriod);
	TimerLoadSet(PWM_MOTOR_RIGHT_B, ulPeriod);
	TimerMatchSet(PWM_MOTOR_RIGHT_B, ulPeriod);
}

/**
 * @brief Control speed
 * @param select motor select
 * @param speed motor speed (encoder pulse / 20ms)
 */
void speed_set(MOTOR_SELECT select, int32_t speed)
{
//	speed_Enable_Hbridge(true);
	if(select == MOTOR_RIGHT){
		if (SetPoint[0] != speed)
		{
			SetPoint[0] = speed;
			speed_control_runtimeout(20);
		}
	}
	else if(select == MOTOR_LEFT){
		if (SetPoint[1] != speed)
		{
			SetPoint[1] = speed;
			speed_control_runtimeout(20);
		}
	}
	if (SetPoint[0] == 0 && SetPoint[1] == 0)
	{
		StopPWM(DEFAULT);
	}
}


static void speed_update_setpoint(void)
{
	int i;
	speed_control_timID = INVALID_TIMER_ID;

	for (i = 0; i < 2; i++)
	{
		if (RealSpeedSet[i] + 20 < SetPoint[i])
			RealSpeedSet[i] += 20;
		else if (RealSpeedSet[i] > SetPoint[i] + 20)
			RealSpeedSet[i] -= 20;
		else
			RealSpeedSet[i] = SetPoint[i];
	}

	speed_control_runtimeout(20);
}

static void speed_control_runtimeout(uint32_t ms)
{
	speed_control_stoptimeout();
	speed_control_timID = TIMER_RegisterEvent(&speed_update_setpoint, ms);
}

static void speed_control_stoptimeout(void)
{
	if (speed_control_timID != INVALID_TIMER_ID)
		TIMER_UnregisterEvent(speed_control_timID);
	speed_control_timID = INVALID_TIMER_ID;
}

void speed_SetMotorModel(MOTOR_SELECT select, real_T Theta[4])
{
	int i;
	if (select == MOTOR_RIGHT) {
		for (i = 0; i < 4; i++)
		{
			Theta_[i] = Theta[i];
		}
	} else if (select == MOTOR_LEFT) {
		for (i = 0; i < 4; i++)
		{
			Theta2_[i] = Theta[i];
		}
	}
}

void speed_GetMotorModel(MOTOR_SELECT select, real_T Theta[4])
{
	int i;
	if (select == MOTOR_RIGHT) {
		for (i = 0; i < 4; i++)
		{
			Theta[i] = Theta_[i];
		}
	} else if (select == MOTOR_LEFT){
		for (i = 0; i < 4; i++)
		{
			Theta[i] = Theta2_[i];
		}
	}
}
