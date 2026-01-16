
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */
/*                                                                                                          *
 *              ╔═════════════════════════════════════════════════════════════════════════╗                 *
 *               ║                            FILE: adc.h                                ║                  *
 *               ║                            AUTHOR: Lovejoy Mhishi                     ║                  *
 *               ║                            DATE: December 8, 2025                     ║                  *
 *              ╠═════════════════════════════════════════════════════════════════════════╣                 *
 *               ║       STM32 ADC Driver - Function Prototypes and Definitions          ║                  *
 *              ╚═════════════════════════════════════════════════════════════════════════╝                 *
 * This header file provides function prototypes, type definitions, and external variables                  *
 * for the STM32 ADC driver.                                                                                *
 *                                                                                                          *
 * Contents:                                                                                                *
 *   - Function prototypes for ADC initialization and data read.                                            *
 *   - Definitions for ADC channels and configuration constants.                                            *
 *   - External variables for ADC results or state tracking.                                                *
 *                                                                                                          *
 * Intended Use:                                                                                            *
 *   - Include in application files (e.g., app.c, main.c) to access ADC driver APIs.                        *
 *                                                                                                          *
 * Dependencies:                                                                                            *
 *   - CMSIS device headers (e.g., stm32f4xx.h) for register definitions.                                   *
 *   - adc.c implementation file containing the function bodies.                                            *
 *                                                                                                          */
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */

#ifndef CUSTOM_DRIVERS_ADC_INC_ADC_H_
#define CUSTOM_DRIVERS_ADC_INC_ADC_H_

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/* 																											*/
/*                                           INCLUDES                                                       */
/*                                                                                  						*/
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */

#include <stdint.h>
#include <stdbool.h>

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           DEFINES                                                        */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */

#define t_ADCVREG_SETUP                      2		// 2ms
#define V_REF_plus                           3.3f   // Internal Voltage Reference
#define ADC_MAX                              4095

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                          DATA PROCESSING                                                 */
/*																						 				    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */


extern bool ADC_Cmplt;

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           LOW-LEVEL FUNCTIONS                                            */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */

void ADC1_Init(void);
void ADC1_Start(void);
void ADC1_Stop(void);

void ADC_AWD_Thrds_Config(void);
uint16_t ADC1_Read(void);
#endif /* CUSTOM_DRIVERS_ADC_INC_ADC_H_ */



