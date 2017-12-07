/*
 * main.c
 */
#include "include.h"
int i;
int16_t XaxisRaw, YaxisRaw, ZaxisRaw;
static bool qei_velocity_timeout[2];
static int32_t qei_velocity[2] = {0, 0};
uint32_t Period;
int32_t Velocity[2] = {0, 0};
int32_t Dutycycle;
extern int headingAngle;
extern float headingOffset;
extern Kalman headingkalman;

static void QEI0_VelocityIsr(void);
static void QEI1_VelocityIsr(void);

void qei_init(uint16_t ms_Timebase)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    QEIConfigure(QEI1_BASE, QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET
    		| QEI_CONFIG_QUADRATURE | QEI_CONFIG_SWAP, 0xFFFFFFFF);
    GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6);
    GPIOPinConfigure(GPIO_PC5_PHA1);
    GPIOPinConfigure(GPIO_PC6_PHB1);
    QEIVelocityConfigure(QEI1_BASE, QEI_VELDIV_2, SysCtlClockGet() * ms_Timebase/ 1000);
    QEIVelocityEnable(QEI1_BASE);
    QEIEnable(QEI1_BASE);

    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD_WPU);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0x80;
    HWREG(GPIO_PORTD_BASE + GPIO_O_AFSEL) &= ~0x80;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    QEIConfigure(QEI0_BASE, QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET
        		| QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP, 0xFFFFFFFF);
    GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinConfigure(GPIO_PD6_PHA0);
    GPIOPinConfigure(GPIO_PD7_PHB0);
    QEIVelocityConfigure(QEI0_BASE, QEI_VELDIV_2, SysCtlClockGet() * ms_Timebase/ 1000);
    QEIVelocityEnable(QEI0_BASE);
    QEIEnable(QEI0_BASE);

    QEIIntEnable(QEI0_BASE, QEI_INTTIMER);
    QEIIntEnable(QEI1_BASE, QEI_INTTIMER);

    QEIIntRegister(QEI0_BASE, &QEI0_VelocityIsr);
    QEIIntRegister(QEI1_BASE, &QEI1_VelocityIsr);

    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD_WPU);
}

static void QEI0_VelocityIsr(void){
	QEIIntClear(QEI0_BASE, QEIIntStatus(QEI0_BASE, true));
	qei_velocity[0] = QEIVelocityGet(QEI0_BASE) * QEIDirectionGet(QEI0_BASE);
	Velocity[0] = qei_velocity[0];
	qei_velocity_timeout[0] = true;
}

static void QEI1_VelocityIsr(void){
	QEIIntClear(QEI1_BASE, QEIIntStatus(QEI1_BASE, true));
	qei_velocity[1] = QEIVelocityGet(QEI1_BASE) * QEIDirectionGet(QEI1_BASE);
	Velocity[1] = qei_velocity[1];
	qei_velocity_timeout[1] = true;
}

bool qei_getVelocity(int32_t *velocity){
	if (qei_velocity_timeout[0]) {
		*velocity = qei_velocity[0];
		qei_velocity_timeout[0] = false;
		return true;
	}
	else
		return false;
}

static void Config_PWM(void)
{
    // Enable Peripheral Clocks
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB2_T3CCP0);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinConfigure(GPIO_PB3_T3CCP1);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_3);


//    Configure timer
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);

    TimerConfigure(TIMER3_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM);
    TimerLoadSet(TIMER3_BASE, TIMER_A, DEFAULT);
    TimerMatchSet(TIMER3_BASE, TIMER_A, DEFAULT); // PWM
    TimerControlLevel(TIMER3_BASE, TIMER_A, true);
    TimerEnable(TIMER3_BASE, TIMER_A);

    TimerLoadSet(TIMER3_BASE, TIMER_B, DEFAULT);
    TimerMatchSet(TIMER3_BASE, TIMER_B, DEFAULT); // PWM
    TimerControlLevel(TIMER3_BASE, TIMER_B, true);
    TimerEnable(TIMER3_BASE, TIMER_B);


    // Enable Peripheral Clocks
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB6_T0CCP0);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_6);
    GPIOPinConfigure(GPIO_PB7_T0CCP1);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_7);
//    Configure timer
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM);
    TimerLoadSet(TIMER0_BASE, TIMER_A, DEFAULT);
    TimerMatchSet(TIMER0_BASE, TIMER_A, DEFAULT); // PWM
    TimerControlLevel(TIMER0_BASE, TIMER_A, true);
    TimerEnable(TIMER0_BASE, TIMER_A);

    TimerLoadSet(TIMER0_BASE, TIMER_B, DEFAULT);
    TimerMatchSet(TIMER0_BASE, TIMER_B, DEFAULT); // PWM
    TimerControlLevel(TIMER0_BASE, TIMER_B, true);
    TimerEnable(TIMER0_BASE, TIMER_B);
}

void SetPWMCW(uint32_t ulFrequency, int32_t ucDutyCycle)
{
	uint32_t ulPeriod;
	ulPeriod = SysCtlClockGet() / ulFrequency;
	Period = ulPeriod;
	TimerLoadSet(TIMER3_BASE, TIMER_A, ulPeriod);
	TimerLoadSet(TIMER3_BASE, TIMER_B, DEFAULT);
	TimerLoadSet(TIMER0_BASE, TIMER_A, DEFAULT);
	TimerLoadSet(TIMER0_BASE, TIMER_B, ulPeriod);
	if (ucDutyCycle > 90)
		ucDutyCycle = 90;
	else if (ucDutyCycle < -90)
		ucDutyCycle = -90;
	Dutycycle = (100 + ucDutyCycle) * ulPeriod / 200 - 1;
	TimerMatchSet(TIMER3_BASE, TIMER_B, DEFAULT);
	TimerMatchSet(TIMER3_BASE, TIMER_A, (100 + ucDutyCycle) * ulPeriod / 200 - 1);
	TimerMatchSet(TIMER0_BASE, TIMER_A, DEFAULT);
	TimerMatchSet(TIMER0_BASE, TIMER_B, (100 + ucDutyCycle) * ulPeriod / 200 - 1);
}

void SetPWMCCW(uint32_t ulFrequency, int32_t ucDutyCycle)
{
	uint32_t ulPeriod;
	ulPeriod = SysCtlClockGet() / ulFrequency;
	Period = ulPeriod;
	TimerLoadSet(TIMER3_BASE, TIMER_A, DEFAULT);
	TimerLoadSet(TIMER3_BASE, TIMER_B, ulPeriod);
	TimerLoadSet(TIMER0_BASE, TIMER_A, ulPeriod);
	TimerLoadSet(TIMER0_BASE, TIMER_B, DEFAULT);
	if (ucDutyCycle > 90)
		ucDutyCycle = 90;
	else if (ucDutyCycle < -90)
		ucDutyCycle = -90;
	Dutycycle = (100 + ucDutyCycle) * ulPeriod / 200 - 1;
	TimerMatchSet(TIMER3_BASE, TIMER_B, (100 + ucDutyCycle) * ulPeriod / 200 - 1);
	TimerMatchSet(TIMER3_BASE, TIMER_A, DEFAULT);
	TimerMatchSet(TIMER0_BASE, TIMER_B, DEFAULT);
	TimerMatchSet(TIMER0_BASE, TIMER_A, (100 + ucDutyCycle) * ulPeriod / 200 - 1);
}

void ConfigButton(void){
	SysCtlPeripheralEnable(GPIO_PORTF_BASE);
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = GPIO_PIN_0;
//	HWREG(GPIO_PORTF_BASE + GPIO_O_AFSEL) &= ~0x01;
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
}

void UART0_ISR(void){
	uint32_t ui32status;
	ui32status = UARTIntStatus(UART0_BASE, true);
	UARTIntClear(UART0_BASE, ui32status);
}

void ConfigUART(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
	UARTStdioConfig(0, 115200, 16000000);

	UARTIntRegister(UART0_BASE, UART0_ISR);
	UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_TX);
	UARTEnable(UART0_BASE);
	IntEnable(INT_UART0);
}


void main(void) {
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
	Timer_Init();
//	qei_init(20);
//	Config_PWM();
	LedInit();
	ConfigUART();
//	ConfigButton();
	Hmc5883lInit();
	SysCtlPeripheralEnable(LED_PERIPHERAL);
	GPIOPinTypeGPIOOutput(LED_PORT, BLUE_LED | RED_LED | GREEN_LED);
	GPIOPinWrite(LED_PORT, BLUE_LED | RED_LED | GREEN_LED, 0x00);
	SysCtlDelay(20000);
	UARTprintf("Hello\n");
	while(1){
//		headingAngle = Hmc5883lAzimuth();
		Hmc5883lMeasurement(&XaxisRaw, &YaxisRaw, &ZaxisRaw);
//		headingAngle = Hmc5883lAzimuth();
		i++;
//		UARTprintf("%d", headingAngle);
//		int i;
//		for (i = -20; i < 40; i+=5) {
//			SetPWMCW(DEFAULT, i);
//			SysCtlDelay(SysCtlClockGet() / 3);
//		}
	}
}

