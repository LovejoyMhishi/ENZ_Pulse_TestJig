
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


/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           DEFINES                                                        */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
#define TIM3_PSC                            15
#define TIM3_ARR                            499

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           LOW-LEVEL FUNCTIONS                                            */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
void TIM3_Init(void);
#endif /* CUSTOM_DRIVERS_TIM_INC_TIMERS_H_ */
