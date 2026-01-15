
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


void TIM3_Init(void) {


	CLEAR_BIT(TIM3->CR1, TIM_CR1_CEN);                            //Ensure that the Timer's Counter is OFF


	SET_BIT(RCC->APBENR1, RCC_APBENR1_TIM3EN);                    // TIM3 timer clock enable



	WRITE_REG(TIM3->PSC, 15);

	WRITE_REG(TIM3->ARR, 1999);
	WRITE_REG(TIM3->CCR1, 1000);                                   //Duty (%) = CCR / (ARR + 1)

	CLEAR_BIT(TIM3->CCMR1, TIM_CCMR1_OC1M);
	SET_BIT(TIM3->CCMR1, 0b110<<TIM_CCMR1_OC1M_Pos);
	SET_BIT(TIM3->CCMR1, TIM_CCMR1_OC1PE);

	SET_BIT(TIM3->CCER, TIM_CCER_CC1E);                           // Enable CH1 output6+

	SET_BIT(TIM3->CR1, TIM_CR1_ARPE);                             //ARPE: Auto-reload preload enable

	SET_BIT(TIM3->EGR, TIM_EGR_UG);                               // Force update event

}

void TIM14_Init(void) {


	CLEAR_BIT(TIM14->CR1, TIM_CR1_CEN);                            //Ensure that the Timer's Counter is OFF
	SET_BIT(RCC->APBENR2, RCC_APBENR2_TIM14EN);                    // TIM14 timer clock enable

	WRITE_REG(TIM14->PSC, 63999);
	WRITE_REG(TIM14->ARR, 4999);
	SET_BIT(TIM14->EGR, TIM_EGR_UG);                               //Force update event
	CLEAR_BIT(TIM14->SR,  TIM_SR_UIF);
	SET_BIT(TIM14->CR1, TIM_CR1_ARPE | TIM_CR1_OPM);               //ARPE & OPM EN
	SET_BIT(TIM14->DIER, TIM_DIER_UIE);                            //Update interrupt EN


	/*
	 * Enable NVIC IRQ
	 */
	NVIC_SetPriority(TIM14_IRQn, 2);
	NVIC_EnableIRQ(TIM14_IRQn);

}



