/*
 * PID.h
 *
 *  Created on: Nov 14, 2017
 *      Author: Nguyen
 */

#ifndef LINEFOLLOW_PID_H_
#define LINEFOLLOW_PID_H_

typedef struct
{
	float Kp;
	float Kd;
	float Ki;
	float u;
	float u_;
	float e;
	float e_;
	float e__;
	float Ts;
	float Saturation;
} PID_PARAMETERS;

extern void PIDInit(void);
extern void PIDSetParameters(PID_PARAMETERS pid_param);
float PIDProcess(PID_PARAMETERS* pid_parameters, float error);
extern float PIDGetError();
extern void PIDReset(PID_PARAMETERS* pid_parameters);
extern void PIDSetKParameters(PID_PARAMETERS* pid_parameters, float Kp, float Ki, float Kd);
extern void PIDSGetParameters(PID_PARAMETERS *pid_parameters);

#endif /* LINEFOLLOW_PID_H_ */
