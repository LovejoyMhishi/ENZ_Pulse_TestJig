
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */
/*                                                                                                          *
 *              ╔═════════════════════════════════════════════════════════════════════════╗                 *
 *               ║                            FILE: timers.h                             ║                  *
 *               ║                            AUTHOR: Lovejoy Mhishi                     ║                  *
 *               ║                            DATE: December 23, 2025                    ║                  *
 *              ╠═════════════════════════════════════════════════════════════════════════╣                 *
 *               ║         STM32 Timer Driver - Function Prototypes and Definitions      ║                  *
 *              ╚═════════════════════════════════════════════════════════════════════════╝                 *
 * This header file provides function prototypes, type definitions, and macros for the STM32 timer driver.  *
 *                                                                                                          *
 * Contents:                                                                                                *
 *   - Timer defines                                                                                        *
 *   - Timer initialization and configuration function prototypes                                           *
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

#ifndef CUSTOM_DRIVERS_TIM_INC_TIM_H_
#define CUSTOM_DRIVERS_TIM_INC_TIM_H_
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/* 																											*/
/*                                           INCLUDES                                                       */
/*                                                                                  						*/
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
#include "stm32g030xx.h"
#include "stm32g0xx_hal.h"

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           DEFINES                                                        */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
#define TIM3_PSC                            15                        //PSC, timer clock divided by 16: 4 MHz
#define TIM3_ARR                            1999                      //ARR, counts 2K ticks: 4MHz/2K = 2 kHz
#define TIM3_CCR                            1000                      //Compare value: 50% of ARR

#define TIM14_PSC                           63999
#define TIM14_ARR                           4999




/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           LOW-LEVEL FUNCTIONS                                            */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
void TIM3_Init(void);
void TIM4_Init(void);



__STATIC_INLINE void TIMx_Start(TIM_TypeDef *TIMx)
{
	SET_BIT(TIMx->CR1, TIM_CR1_CEN);                                  //Start timer
}


__STATIC_INLINE void TIMx_Stop(TIM_TypeDef *TIMx)
{
	CLEAR_BIT(TIMx->CR1, TIM_CR1_CEN);                                // Stop timer
	CLEAR_REG(TIMx->CNT);
	CLEAR_BIT(TIMx->SR, TIM_SR_UIF);                                  // Clear update interrupt flag
}


#endif /* CUSTOM_DRIVERS_TIM_INC_TIMERS_H_ */
