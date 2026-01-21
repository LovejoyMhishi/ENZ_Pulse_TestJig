
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
#include "gpio.h"
#include "tim.h"
#include  "adc.h"
#include "app.h"


volatile ButtonState Button = {
		.StateHistory = 0b0000000000000000,
		.StateCount = 0
};

volatile ST_BUTTON BUTTON_STATE = IDLE;

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           HIGH-LEVEL FUNCTIONS                                           */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
void ScanButtonPress(void)
{
	if(!READ_BIT(GPIOA->IDR, GPIO_IDR_ID5) && BUTTON_STATE == IDLE)
	{
		TIMx_Start(TIM16);
		BUTTON_STATE = PRESS_DETECTED;
	}
}

void CheckButtonState(void)
{

	if(BUTTON_STATE == PRESS_DETECTED)
	{

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
	}
}

void ENZ_PASSED(void)
{
	GPIO_Writepin(GPIOA, LED_0, GPIO_PIN_RESET);
	SET_BIT(TIM3->CR1, TIM_CR1_CEN);
	TimeOut(100);
	CLEAR_BIT(TIM3->CR1, TIM_CR1_CEN);
	TimeOut(800);
	GPIO_Writepin(GPIOA, LED_0, GPIO_PIN_SET);
}


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
