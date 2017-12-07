/*
 * LineFollow.h
 *
 *  Created on: Nov 14, 2017
 *      Author: Nguyen
 */

#ifndef LINEFOLLOW_LINEFOLLOW_H_
#define LINEFOLLOW_LINEFOLLOW_H_

typedef enum{
	TURN_LEFT = 0,
	TURN_RIGHT,
	FORWARD,
	BACKWARD,
	NONE
} MOVE;

typedef enum{
	UP_DIR = 0,
	RIGHT_DIR = 1,
	DOWN_DIR = 2,
	LEFT_DIR = 3,
} DIRECTION;

extern void LineFollowInit();
extern void PIDLineFollowProcess();
bool move(int posleft, int posright, int velleftmax, int velrightmax);
#endif /* LINEFOLLOW_LINEFOLLOW_H_ */
