/*
 * Kalman.h
 *
 *  Created on: Nov 30, 2017
 *      Author: Nguyen
 */

#ifndef SENSOR_HMC5883L_KALMAN_H_
#define SENSOR_HMC5883L_KALMAN_H_

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

//Define Kalman struct
typedef struct{
	float q;  			  //Process Noise Covariance
//	float q_init;
	float r;			  //Measurement Noise Covariance
//	float r_init;

	float x;			  //Value
	float p;			  //Estimation Error Covariance
//	float p_init;

	float k;			  //Kalman Gain
} Kalman;

extern void KalmanInit(Kalman *KalmanStruct);

extern float GetAngle(Kalman *KalmanStruct, float newangle);
extern void SetAngle(Kalman *KalmanStruct, float angle);

extern void setQ(Kalman *KalmanStruct, float q);
extern void setR(Kalman *KalmanStruct, float r);

extern float getQ(Kalman *KalmanStruct);
extern float getR(Kalman *KalmanStruct);
#endif /* SENSOR_HMC5883L_KALMAN_H_ */
