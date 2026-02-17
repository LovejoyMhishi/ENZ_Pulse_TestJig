
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */
/*                                                                                                          *
 *              ╔═════════════════════════════════════════════════════════════════════════╗                 *
 *               ║                            FILE: timer.h                              ║                  *
 *               ║                            AUTHOR: Lovejoy Mhishi                     ║                  *
 *               ║                            DATE: December 23, 2025                    ║                  *
 *              ╠═════════════════════════════════════════════════════════════════════════╣                 *
 *               ║         STM32 Timer Driver - Function Prototypes and Definitions      ║                  *
 *              ╚═════════════════════════════════════════════════════════════════════════╝                 *
 * This header file provides function prototypes, type definitions, and macros for the STM32 timer driver.  *
 *                                                                                                          *
 * Contents:                                                                                                *
 *   - Timer initialization and configuration function bodies                                               *
 *   - Start, stop, and reset functions                                                                     *
 *   - Interrupt handling function prototypes                                                               *
 *   - Timer prescaler and period settings                                                                  *
 *                                                                                                          *
 * Intended Use:                                                                                            *
 *   - Include in application files (e.g., main.c, app.c) to access timer driver APIs.                      *
 *                                                                                                          *
 * Dependencies:                                                                                            *
 *   - CMSIS device headers (e.g., stm32g030xx.h) for register definitions.                                 *
 *   - timer.c implementation file containing the function bodies.                                          *
 *                                                                                                          */
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/* 																											*/
/*                                           INCLUDES                                                       */
/*                                                                                  						*/
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
#include "tim.h"
#include "stm32g030xx.h"
#include "stm32g0xx_hal.h"

/* ────────────────────────────────────────────────────────────── /
 * Function : TIM3_Init()
 * Purpose  : Initialize TIM3
 * Details  : ----
 * Runtime  : ~X.Xxx
 * ────────────────────────────────────────────────────────────── */
void TIM3_Init(void)
{
	CLEAR_BIT(TIM3->CR1, TIM_CR1_CEN);                                //Ensure that the Timer's Counter is OFF

	SET_BIT(RCC->APBENR1, RCC_APBENR1_TIM3EN);                        // TIM3 timer clock enable

	WRITE_REG(TIM3->PSC, TIM3_PSC);
	WRITE_REG(TIM3->ARR, TIM3_ARR);
	WRITE_REG(TIM3->CCR1, TIM3_CCR);                                  //Duty (%) = CCR / (ARR + 1)

	CLEAR_BIT(TIM3->CCMR1, TIM_CCMR1_OC1M);
	SET_BIT(TIM3->CCMR1, 0b110<<TIM_CCMR1_OC1M_Pos);
	SET_BIT(TIM3->CCMR1, TIM_CCMR1_OC1PE);

	SET_BIT(TIM3->CCER, TIM_CCER_CC1E);                               // Enable CH1 output6+

	SET_BIT(TIM3->CR1, TIM_CR1_ARPE);                                 //ARPE: Auto-reload preload enable

	SET_BIT(TIM3->EGR, TIM_EGR_UG);                                   // Force update event
}

/* ────────────────────────────────────────────────────────────── /
 * Function : TIM14_Init()
 * Purpose  : Initialize TIM14
 * Details  : ----
 * Runtime  : ~X.Xxx
 * ────────────────────────────────────────────────────────────── */
void TIM14_Init(void)
{
	CLEAR_BIT(TIM14->CR1, TIM_CR1_CEN);                               //Ensure that the Timer's Counter is OFF
	SET_BIT(RCC->APBENR2, RCC_APBENR2_TIM14EN);                       //TIM14 timer clock enable

	WRITE_REG(TIM14->PSC, TIM14_PSC);
	WRITE_REG(TIM14->ARR, TIM14_ARR);
	SET_BIT(TIM14->EGR, TIM_EGR_UG);                                  //Force update event
	CLEAR_BIT(TIM14->SR,  TIM_SR_UIF);
	SET_BIT(TIM14->CR1, TIM_CR1_ARPE | TIM_CR1_OPM);                  //ARPE & OPM EN
	SET_BIT(TIM14->DIER, TIM_DIER_UIE);                               //Update interrupt EN
	/*
	 * Enable NVIC IRQ
	 */
	NVIC_SetPriority(TIM14_IRQn, 2);
	NVIC_EnableIRQ(TIM14_IRQn);
}
/* ────────────────────────────────────────────────────────────── /
 * Function : TIM16_Init()
 * Purpose  : Initialize TIM16
 * Details  : ----
 * Runtime  : ~X.Xxx
 * ────────────────────────────────────────────────────────────── */
void TIM16_Init(void)
{
	CLEAR_BIT(TIM16->CR1, TIM_CR1_CEN);                               //Ensure that the Timer's Counter is OFF
	SET_BIT(RCC->APBENR2, RCC_APBENR2_TIM16EN);                       //TIM14 timer clock enable

	WRITE_REG(TIM16->PSC, TIM16_PSC);
	WRITE_REG(TIM16->ARR, TIM16_ARR);
	SET_BIT(TIM16->EGR, TIM_EGR_UG);                                  //Force update event
	CLEAR_BIT(TIM16->SR,  TIM_SR_UIF);
	SET_BIT(TIM16->CR1, TIM_CR1_ARPE);                                //ARPE EN
	SET_BIT(TIM16->DIER, TIM_DIER_UIE);                               //Update interrupt EN
	/*
	 * Enable NVIC IRQ
	 */
	NVIC_SetPriority(TIM16_IRQn, 2);
	NVIC_EnableIRQ(TIM16_IRQn);
}
