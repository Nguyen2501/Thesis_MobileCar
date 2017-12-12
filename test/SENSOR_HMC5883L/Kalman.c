/*
 * Kalman.c
 *
 *  Created on: Nov 30, 2017
 *      Author: Nguyen
 */

#include "Kalman.h"
void KalmanInit(Kalman *KalmanStruct){
	/*We will set the variables like so, these can also be tuned by the user*/

	(*KalmanStruct).p = 1.0f; //Estimation Error Covariance
	(*KalmanStruct).r = 1.0f; //Measurement Noise Covariance
	(*KalmanStruct).q = 0.001f; //Process Noise Covariance

	(*KalmanStruct).x = 0.0f; //Value
	(*KalmanStruct).k = 0.0f; //Kalman Gain
}

float GetAngle(Kalman *KalmanStruct, float newAngle){
	//Discrete Kalman filter time update equations - Time update ("Predict")
	//Prediction Update
	/*Step 1*/
	(*KalmanStruct).p = (*KalmanStruct).p + (*KalmanStruct).q;

	//Measurement Update
	/*Step 2*/
	//Calculate Kalman Gain
	(*KalmanStruct).k = (*KalmanStruct).p / ((*KalmanStruct).p + (*KalmanStruct).r);
	/*Step 3*/
	//Calculate new angle
	(*KalmanStruct).x = (*KalmanStruct).x + (*KalmanStruct).k * (newAngle - (*KalmanStruct).x);
	/*Step 4*/
	//Calculate Estimation Error Covariance
	(*KalmanStruct).p = (1 - (*KalmanStruct).k) * (*KalmanStruct).p;

	return (*KalmanStruct).x;
	}

//Used to set angle, this should be set as the starting angle
void SetAngle(Kalman *KalmanStruct, float angle){
	(*KalmanStruct).x = angle;
}

/*These are used to tune the Kalman filter*/
void SetQ(Kalman *KalmanStruct, float q){
	(*KalmanStruct).q = q;
}

void SetR(Kalman *KalmanStruct, float r){
	(*KalmanStruct).r = r;
}

float GetQ(Kalman *KalmanStruct){
	return (*KalmanStruct).q;
}

float GetR(Kalman *KalmanStruct){
	return (*KalmanStruct).r;
}

