/*
 * LineFollow.c
 *
 *  Created on: Nov 14, 2017
 *      Author: Nguyen
 */

#include "../include.h"
#include "LineFollow.h"

#define AVG_SPEED_FORWARD_FAST 30
#define AVG_SPEED_FORWARD 20
#define AVG_SPEED_FORWARD_SLOW 10
#define AVG_SPEED_FORWARD_BACKWARD 30

static void PIDProcessCallback(void);
static void PIDStopTimeout(void);
static TIMER_ID PIDRuntimeout(TIMER_CALLBACK_FUNC CallbackFcn, uint32_t msTime);
//static bool TurnLeft(int forwardpulse, int averagespeedleft, int averagespeedright, int turnpulse, int forwardpulse2);
//static bool TurnRight(int forwardpulse, int averagespeedleft, int averagespeedright, int turnpulse, int forwardpulse2);
//static bool Forward(int averagespeedleft, int averagespeedright);
static bool Forward();
static bool TurnBack(int forwardpulse, int averagespeedleft, int averagespeedright, int turnpulse);
static bool TurnLeft(int forwardpulse, int averagespeedleft, int averagespeedright, int turnpulse);
static bool TurnRight(int forwardpulse, int averagespeedleft, int averagespeedright, int turnpulse);

static int32_T AverageSpeed, AverageSpeedTemp;
static int32_t ControlStep = 1;
static uint32_t ui32msLoop = 0;
static TIMER_ID pidTimerID = INVALID_TIMER_ID;
static bool checkforward = false;
PID_PARAMETERS pidlinefollow = {.Kp = 0, .Kd = 0, .Ki = 0, .Ts = 0.020, .Saturation = 300, .e_ = 0, .e__ = 0, .u_ = 0};

static void PIDLineFollowInit(){
	ui32msLoop = pidlinefollow.Ts * 1000;
	PIDRuntimeout(&PIDProcessCallback, ui32msLoop);
}

void LineFollowInit(){
	PIDLineFollowInit();
	AverageSpeed = AVG_SPEED_FORWARD_SLOW;
}

static bool PIDLineFollow(float averagespeed){
	static uint8_t linestate;
	static int8_t error, u;
	int32_t set_speed[2];
	linestate = LineState();
	error = LineValuePID(linestate);
	u = PIDProcess(&pidlinefollow, error);
	set_speed[0] = averagespeed + (int32_t)(u / 2);
	set_speed[1] = averagespeed - (int32_t)(u / 2);

	speed_set(MOTOR_RIGHT, set_speed[0]);
	speed_set(MOTOR_LEFT, set_speed[1]);
	return true;
}

static void PIDProcessCallback(){
	pidTimerID = INVALID_TIMER_ID;
	PIDRuntimeout(&PIDProcessCallback, ui32msLoop);
}

static void PIDStopTimeout(void){
	if (pidTimerID != INVALID_TIMER_ID) {
		TIMER_UnregisterEvent(pidTimerID);
	}
	pidTimerID = INVALID_TIMER_ID;
}

static TIMER_ID PIDRuntimeout(TIMER_CALLBACK_FUNC CallbackFcn, uint32_t msTime){
	PIDStopTimeout();
	pidTimerID = TIMER_RegisterEvent(CallbackFcn, msTime);
	return pidTimerID;
}

static bool Forward(){
	LED1_OFF();
	LED2_ON();
	LED3_OFF();
	AverageSpeed = AVG_SPEED_FORWARD_SLOW;
	PIDLineFollow(AverageSpeed);
//	switch (ControlStep) {
//		case 1:
//			ControlStep++;
//			ControlStep = 1;
//		break;
//	}
	return true;
}
static bool TurnRight(int forwardpulse, int averagespeedleft, int averagespeedright, int turnpulse){
	switch (ControlStep) {
		case 1:
			ControlStep++;
			ControlStep = 1;
		break;
	}
	return false;
}
static bool TurnLeft(int forwardpulse, int averagespeedleft, int averagespeedright, int turnpulse){
	switch (ControlStep) {
		case 1:
			ControlStep++;
			ControlStep = 1;
		break;
	}
	return false;
}
static bool TurnBack(int forwardpulse, int averagespeedleft, int averagespeedright, int turnpulse){
	switch (ControlStep) {
		case 1:
			ControlStep++;
			ControlStep = 1;
		break;
	}
	return false;
}

void PIDLineFollowProcess(){
	PIDRuntimeout(&PIDProcessCallback, ui32msLoop);
	checkforward = Forward();
}
