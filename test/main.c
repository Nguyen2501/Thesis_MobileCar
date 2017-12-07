#include "include.h"

extern void SetPWM(uint32_t ulFrequency, int32_t ucDutyCycle);
extern void GetValue(uint32_t *adc_raw);

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
	switch (system_GetState()) {
		case SYSTEM_INITIALIZE:
			statecount = 1;
			LED2_ON();
			SysCtlDelay(SysCtlClockGet() / 3);
			LED2_OFF();
			StopPWM(DEFAULT);
			system_SetState(SYSTEM_GET_MOTOR_MODEL);
			break;
		case SYSTEM_GET_MOTOR_MODEL:
			statecount = 2;
			LED1_ON();
			SysCtlDelay(SysCtlClockGet() / 3);
			LED1_OFF();
			system_SetState(SYSTEM_ESTIMATE_MOTOR_MODEL);
//			speed_set(10);
			break;
		case SYSTEM_ESTIMATE_MOTOR_MODEL:
			statecount = 3;
			LED3_ON();
			SysCtlDelay(SysCtlClockGet() / 3);
			LED3_OFF();
			StopPWM(DEFAULT);
			system_SetState(SYSTEM_SAVE_MOTOR_MODEL);
			break;
		case SYSTEM_SAVE_MOTOR_MODEL:
			statecount = 4;
			saveMotorModel();
			system_SetState(SYSTEM_WAIT_TO_RUN);
			break;
		case SYSTEM_WAIT_TO_RUN:
			statecount = 5;
			SysCtlDelay(SysCtlClockGet() / 3);
			system_SetState(SYSTEM_RUN_IMAGE_PROCESSING);
			qei_setPos(0);
			break;
		case SYSTEM_RUN_IMAGE_PROCESSING:
			statecount = 6;
//			speed_set(10);
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
	system_SetState(SYSTEM_INITIALIZE);
	Config_System();
	LEDDisplayInit();
	Timer_Init();
	EEPROMConfig();
	speed_control_init();
    qei_init(20);
    Button_init();

	ButtonRegisterCallback(BUTTON_LEFT, &ButtonLeftHandler);
//	ButtonRegisterCallback(BUTTON_RIGHT, &ButtonRightHandler);
    while(1)
    {
    	system_Process_System_State();
    }

}

