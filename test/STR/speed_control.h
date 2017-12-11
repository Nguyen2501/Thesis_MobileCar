/**
 *	Raise your ARM 2015 sample code http://raiseyourarm.com/
 *	Author: Pay it forward club
 *	http://www.payitforward.edu.vn
 *	version 0.0.1
 */

/**
 * @file	speed_control.h
 * @brief	speed control
 */


#ifndef STR_SPEED_CONTROL_H_
#define STR_SPEED_CONTROL_H_

#define PWM_MOTOR_LEFT_A			TIMER0_BASE, TIMER_A
#define PWM_MOTOR_LEFT_B			TIMER0_BASE, TIMER_B
#define PWM_MOTOR_RIGHT_A			TIMER3_BASE, TIMER_A
#define PWM_MOTOR_RIGHT_B			TIMER3_BASE, TIMER_B
typedef enum
{
	MOTOR_LEFT = 0,
	MOTOR_RIGHT
} MOTOR_SELECT;

extern void SpeedControlInit(void);
extern void ProcessSpeedControl(void);
void speed_set(MOTOR_SELECT select, int32_t speed);
extern void StopPWM(uint32_t ulFrequency);
extern void speed_SetMotorModel(MOTOR_SELECT select, real_T Theta[4]);
extern void speed_GetMotorModel(MOTOR_SELECT select, real_T Theta[4]);

#endif /* STR_SPEED_CONTROL_H_ */
