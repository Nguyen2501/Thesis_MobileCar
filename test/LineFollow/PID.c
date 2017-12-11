/*
 * PID.c
 *
 *  Created on: Nov 14, 2017
 *      Author: Nguyen
 */

#include "../include.h"
#include "PID.h"

void PIDSetKParameters(PID_PARAMETERS* pid_parameters, float Kp, float Ki, float Kd){
	pid_parameters->Kp = Kp;
	pid_parameters->Kd = Kd;
	pid_parameters->Ki = Ki;
}

float PIDProcess(PID_PARAMETERS* pid_parameters, float error){
	pid_parameters->e__ = pid_parameters->e_;
	pid_parameters->e_ = pid_parameters->e;
	pid_parameters->e = error;
	pid_parameters->u_ = pid_parameters->u;
	pid_parameters->u = pid_parameters->u_  + pid_parameters->Kp * (pid_parameters->e - pid_parameters->e_)
											+ pid_parameters->Ki * pid_parameters->Ts * pid_parameters->e
											+ (pid_parameters->Kd / pid_parameters->Ts) * (pid_parameters->e - (2 * pid_parameters->e_) + pid_parameters->e__);
	if (pid_parameters->u > pid_parameters->Saturation) {
		pid_parameters->u = pid_parameters->Saturation;
	}
	if (pid_parameters->u < -pid_parameters->Saturation) {
		pid_parameters->u = -pid_parameters->Saturation;
	}

	return pid_parameters->u;
}

void PIDReset(PID_PARAMETERS* pid_parameters){
	pid_parameters->e = 0;
	pid_parameters->e_ = 0;
	pid_parameters->e__ = 0;
	pid_parameters->u = 0;
	pid_parameters->u_ = 0;
}
