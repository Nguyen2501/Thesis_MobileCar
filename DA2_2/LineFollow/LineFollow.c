/*
 * LineFollow.c
 *
 *  Created on: Nov 14, 2017
 *      Author: Nguyen
 */

#include "../include.h"
#include "LineFollow.h"

static void PIDProcessCallback(void);
static void PIDStopTimeout(void);
static TIMER_ID PIDRuntimeout(TIMER_CALLBACK_FUNC CallbackFcn, uint32_t msTime);
static bool TurnLeft(int forwardpulse, int averagespeedleft, int averagespeedright, int turnpulse, int forwardpulse2);
static bool TurnRight(int forwardpulse, int averagespeedleft, int averagespeedright, int turnpulse, int forwardpulse2);

static uint32_t ui32msLoop = 0;
static TIMER_ID pidTimerID = INVALID_TIMER_ID;

PID_PARAMETERS pidlinefollow = {.Kp = 0, .Kd = 0, .Ki = 0, .Ts = 0.020, .Saturation = 300, .e_ = 0, .e__ = 0, .u_ = 0};

static void PIDLineFollowInit(){
	ui32msLoop = pidlinefollow.Ts * 1000;
	PIDRuntimeout(&PIDProcessCallback, ui32msLoop);
}

//void LineFollowInit(){
//	PIDLineFollowInit();
//
//}

static bool PIDLineFollow();

static void PIDLineFollowProcess(){
	pidTimerID = INVALID_TIMER_ID;
	PIDRuntimeout(&PIDLineFollowProcess, ui32msLoop);
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

