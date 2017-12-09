#include "include.h"

extern void SetPWMCCW_LeftMotor(uint32_t ulFrequency, int32_t ucDutyCycle);
extern void SetPWMCW_LeftMotor(uint32_t ulFrequency, int32_t ucDutyCycle);
extern void SetPWMCCW_RightMotor(uint32_t ulFrequency, int32_t ucDutyCycle);
extern void SetPWMCW_RightMotor(uint32_t ulFrequency, int32_t ucDutyCycle);
extern void GetValue(uint32_t *adc_raw);

extern int32_t Vel[2];
extern float udkmain;
extern uint32_t Period;
extern int32_t Dutycycle;
extern uint32_t calib_white[7], calib_black[7];
extern volatile int8_t statecount = 0;

enum calib_color{
	CALIB_WHITE,
	CALIB_BLACK
}calib_color;

void ButtonLeftHandler(void){
	switch (SystemGetState()) {
		case SYSTEM_INITIALIZE:
			statecount = 1;
			LED2_ON();
			SysCtlDelay(SysCtlClockGet() / 3);
			LED2_OFF();
//			SetPWMCW_LeftMotor(DEFAULT, 30);
//			SysCtlDelay(SysCtlClockGet() / 3);
//			StopPWM(DEFAULT);
			SystemSetState(SYSTEM_GET_MOTOR_MODEL);
			break;
		case SYSTEM_GET_MOTOR_MODEL:
			statecount = 2;
			LED1_ON();
			SysCtlDelay(SysCtlClockGet() / 3);
			LED1_OFF();
			SystemSetState(SYSTEM_ESTIMATE_MOTOR_MODEL);
			speed_set(MOTOR_LEFT, 10);
			speed_set(MOTOR_RIGHT, 10);
			break;
		case SYSTEM_ESTIMATE_MOTOR_MODEL:
			statecount = 3;
			LED3_ON();
			SysCtlDelay(SysCtlClockGet() / 3);
			LED3_OFF();
			StopPWM(DEFAULT);
			SystemSetState(SYSTEM_SAVE_MOTOR_MODEL);
			break;
		case SYSTEM_SAVE_MOTOR_MODEL:
			statecount = 4;
//			saveMotorModel();
			SystemSetState(SYSTEM_WAIT_TO_RUN);
			break;
		case SYSTEM_WAIT_TO_RUN:
			statecount = 5;
			LED1_ON();
			SysCtlDelay(SysCtlClockGet() / 3);
			LED1_OFF();
			SystemSetState(SYSTEM_RUN_IMAGE_PROCESSING);
			QeiSetPositionLeft(0);
			QeiSetPositionRight(0);
			break;
		case SYSTEM_RUN_IMAGE_PROCESSING:
			statecount = 6;
//			speed_set(30);
			break;
		default:
			break;
	}
}

void ButtonRightHandler(void){
	if (calib_color == CALIB_BLACK) {
		GetValue(calib_black);
		calib_color = CALIB_WHITE;
	} else if(calib_color == CALIB_WHITE){
		GetValue(calib_white);
		calib_color = CALIB_BLACK;
	}
}

int main(void){
	SystemSetState(SYSTEM_INITIALIZE);
	ConfigSystem();
	EEPROMConfig();
	LEDDisplayInit();
	TimerInit();
	SpeedControlInit();
	QeiInit(20);
    ButtonInit();
    LineFollowInit();
    ADCInit();

	ButtonRegisterCallback(BUTTON_LEFT, &ButtonLeftHandler);
//	ButtonRegisterCallback(BUTTON_RIGHT, &ButtonRightHandler);
    while(1)
    {
    	system_Process_System_State();
    }

}

