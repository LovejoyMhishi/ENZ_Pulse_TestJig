
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */
/*                                                                                                          *
 *              ╔═════════════════════════════════════════════════════════════════════════╗                 *
 *               ║                            FILE: ui.c                                 ║                  *
 *               ║                            AUTHOR: Lovejoy Mhishi                     ║                  *
 *               ║                            DATE: December 26, 2025                    ║                  *
 *              ╠═════════════════════════════════════════════════════════════════════════╣                 *
 *               ║      User Interface drive controlling LEDs and the Buzzer             ║                  *
 *              ╚═════════════════════════════════════════════════════════════════════════╝                 *
 * This source file contains function bodies for the functions used to control the LEDs and Buzzer          *
 *                                                                                                          *
 * Contents:                                                                                                *
 *   - LEDs ON and OFF                                                                                      *
 *   - Buzzer ON and OFF                                                                                    *
 *                                                                                                          *
 * Intended Use:                                                                                            *
 *   -  Include ui.h in application files (e.g., main.c, app.c) to access UI control APIs.                  *
 *                                                                                                          *
 * Dependencies:                                                                                            *
 *   - ui.h header file for function prototypes and data structures.                                        *
 *   - tim.c and gpio.c for timer- and GPIO-based functions                                                 *
 *                                                                                                          *
 * Notes:                                                                                                   *
 *   - GPIO and TIMx must be initialized before calling UI functions.                                       *
 *                                                                                                          */
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/* 																											*/
/*                                           INCLUDES                                                       */
/*                                                                                  						*/
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */

#include "ui.h"
#include "tim.h"
#include  "adc.h"
#include "app.h"
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/* 																											*/
/*                                           DEFINES                                                        */
/*                                                                                  						*/
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
volatile ButtonState Button = {
		.StateHistory = 0b0000000000000000,
		.StateCount = 0
};

volatile ST_BUTTON BUTTON_STATE = IDLE;
volatile bool PI_ON = false;
volatile uint8_t Rx_Pi_Status[3];
volatile uint8_t Pi_Status;
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           HIGH-LEVEL FUNCTIONS                                           */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/* ────────────────────────────────────────────────────────────── /
 * Function : BUTTON_STATES()
 * Purpose  : SCANs & PROCESS the BUTTON_STATEs
 * Details  : Returns a SHORT_/LONG_PRESS BUTTON_STATEs
 * Runtime  : ~X.Xxx
 * ────────────────────────────────────────────────────────────── */

void BUTTON_STATES(void)
{
	switch(BUTTON_STATE)
	{
	case IDLE:
		if(!READ_BIT(GPIOA->IDR, GPIO_IDR_ID5))
		{
			TIMx_Start(TIM16);
			BUTTON_STATE = PRESS_DETECTED;
		}
		break;
	case PRESS_DETECTED:
		if(Button.StateCount >= 16)
		{
			Button.StateHistory = ~Button.StateHistory;
			TIMx_Stop(TIM16);
			if(Button.StateHistory >= BUTTON_LONG_PRESS)
			{
				BUTTON_STATE = LONG_PRESS;

			}
			else if((Button.StateHistory & BUTTON_SHORT_PRESS) >= 0x1F)
			{
				BUTTON_STATE = SHORT_PRESS;
			}
			else
			{
				BUTTON_STATE = IDLE;
			}

			Button.StateHistory = 0;
			Button.StateCount = 0;
		}
		break;
	case SHORT_PRESS:
		if(PI_ON)
		{
			ENZ.V_Peak = 0;
			ENZ.I_Peak = 0;
			ENZ.Energy_J = 0;
			ENZ.ADC_Peak = 0;
			ENZ.PULSE_CNT = 0;
			ENZ.PULSE_Width = 0;
			ENZ.PULSE_T = 0;
			GPIO_Writepin(GPIOA, LED_0, GPIO_PIN_SET);
			EVENT = SCAN;                                    //Test the next Druid:
		}
		BUTTON_STATE = IDLE;
		break;

	case LONG_PRESS:
		if(PI_ON)
		{
			GPIO_Writepin(GPIOA, Plus_5V_EN , GPIO_PIN_RESET);
			GPIO_Writepin(GPIOA, LED_1, GPIO_PIN_RESET);
			TimeOut(1000);
			GPIO_Writepin(GPIOA, LED_1, GPIO_PIN_SET);
			PI_ON = false;
		}
		else
		{
			GPIO_Writepin(GPIOA, Plus_5V_EN , GPIO_PIN_SET);

			UART_Receive_DMA(USART2,  Rx_Pi_Status, 3);
			SET_BIT(TIM3->CR1, TIM_CR1_CEN);
			TimeOut(800);
			CLEAR_BIT(TIM3->CR1, TIM_CR1_CEN);

			while (!PI_ON)
			{
				GPIO_Writepin(GPIOA, LED_0, GPIO_PIN_RESET);
				TimeOut(500);            //Wait for the Pi to turn ON
				GPIO_Writepin(GPIOA, LED_0, GPIO_PIN_SET);
				TimeOut(500);
				GPIO_Writepin(GPIOA, LED_1, GPIO_PIN_RESET);
				TimeOut(500);            //Wait for the Pi to turn ON
				GPIO_Writepin(GPIOA, LED_1, GPIO_PIN_SET);
				TimeOut(500);
			}//Exits the moment the Pi turns ON

			GPIO_Writepin(GPIOA, LED_0, GPIO_PIN_SET);
			GPIO_Writepin(GPIOA, LED_1, GPIO_PIN_SET);

			GPIO_Writepin(GPIOA, LED_0, GPIO_PIN_RESET);
			SET_BIT(TIM3->CR1, TIM_CR1_CEN);
			TimeOut(100);
			CLEAR_BIT(TIM3->CR1, TIM_CR1_CEN);
			TimeOut(800);
			GPIO_Writepin(GPIOA, LED_0, GPIO_PIN_SET);

			GPIO_Writepin(GPIOA, LED_1, GPIO_PIN_RESET);
			SET_BIT(TIM3->CR1, TIM_CR1_CEN);
			TimeOut(100);
			CLEAR_BIT(TIM3->CR1, TIM_CR1_CEN);
			TimeOut(800);

			GPIO_Writepin(GPIOA, LED_0, GPIO_PIN_RESET);
			SET_BIT(TIM3->CR1, TIM_CR1_CEN);
			TimeOut(1000);
			CLEAR_BIT(TIM3->CR1, TIM_CR1_CEN);
			GPIO_Writepin(GPIOA, LED_0, GPIO_PIN_SET);
			GPIO_Writepin(GPIOA, LED_1, GPIO_PIN_SET);
			UART_Receive_DMA(USART2,  RxBuffer, 13);                                //Start EXpecting a Serial Num after PI_ON
		}
		BUTTON_STATE = IDLE;
		break;
	}
}

/* ────────────────────────────────────────────────────────────── /
 * Function : ENZ_PASSED()
 * Purpose  : UI for ENZ testing
 * Details  : Turns LED & beep once if ENZ passed the test
 * Runtime  : ~X.Xxx
 * ────────────────────────────────────────────────────────────── */
void ENZ_PASSED(void)
{
	GPIO_Writepin(GPIOA, LED_0, GPIO_PIN_RESET);
	SET_BIT(TIM3->CR1, TIM_CR1_CEN);
	TimeOut(100);
	CLEAR_BIT(TIM3->CR1, TIM_CR1_CEN);
	TimeOut(800);
	GPIO_Writepin(GPIOA, LED_0, GPIO_PIN_SET);
}

/* ────────────────────────────────────────────────────────────── /
 * Function : ENZ_FAILED()
 * Purpose  : UI for ENZ testing
 * Details  : Turns LED & beep thrice if ENZ failed the test
 * Runtime  : ~X.Xxx
 * ────────────────────────────────────────────────────────────── */
void ENZ_FAILED(void)
{
	GPIO_Writepin(GPIOA, LED_1, GPIO_PIN_RESET);
	SET_BIT(TIM3->CR1, TIM_CR1_CEN);
	TimeOut(200);
	CLEAR_BIT(TIM3->CR1, TIM_CR1_CEN);
	TimeOut(150);
	SET_BIT(TIM3->CR1, TIM_CR1_CEN);
	TimeOut(200);
	CLEAR_BIT(TIM3->CR1, TIM_CR1_CEN);
	TimeOut(150);
	SET_BIT(TIM3->CR1, TIM_CR1_CEN);
	TimeOut(1000);
	CLEAR_BIT(TIM3->CR1, TIM_CR1_CEN);
	GPIO_Writepin(GPIOA, LED_1, GPIO_PIN_SET);
}

void DEBUG_ENZ(void)
{
	if(ADC_Cmplt)
	{
		for (uint8_t var = 0; var < 200; ++var)
		{
			printf("%d \n", 100);
		}
	}
}
