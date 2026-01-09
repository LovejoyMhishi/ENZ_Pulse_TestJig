
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */
/*                                                                                                          *
 *              ╔═════════════════════════════════════════════════════════════════════════╗                 *
 *               ║                            FILE: dma.h                                ║                  *
 *               ║                            AUTHOR: Lovejoy Mhishi                     ║                  *
 *               ║                            DATE: December 18, 2025                    ║                  *
 *              ╠═════════════════════════════════════════════════════════════════════════╣                 *
 *               ║       STM32 DMA Driver - Function Prototypes and Definitions         ║                   *
 *              ╚═════════════════════════════════════════════════════════════════════════╝                 *
 * This header file provides function prototypes, type definitions, and external variables                  *
 * for the STM32 DMA driver.                                                                                *
 *                                                                                                          *
 * Contents:                                                                                                *
 *   - Function prototypes for DMA initialization and transfer control.                                     *
 *   - Definitions for DMA Peripheral addresses.                                                            *
 *   - External variables for DMA status or callback tracking.                                              *
 *                                                                                                          *
 * Intended Use:                                                                                            *
 *   - Include in application files (e.g., app.c, main.c) to access DMA driver APIs.                        *
 *                                                                                                          *
 * Dependencies:                                                                                            *
 *   - CMSIS device headers (e.g., stm32g030xx.h) for register definitions.                                 *
 *   - dma.c implementation file containing the function bodies.                                            *
 *                                                                                                          */
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */


#ifndef CUSTOM_DRIVERS_DMA_INC_DMA_H_
#define CUSTOM_DRIVERS_DMA_INC_DMA_H_
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/* 																											*/
/*                                           INCLUDES                                                       */
/*                                                                                  						*/
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
#include "stm32g030xx.h"
#include "stm32g0xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/* 																											*/
/*                                           PERIPHERAL ADDRESSES                                           */
/*                                                                                  						*/
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
#define ADC1_DR_ADDRESS                       (ADC1_BASE + 0x40UL)  //(uint32_t)&ADC1->DR;
#define DMAMUX_ADC_DMA                        (5 << DMAMUX_CxCR_DMAREQ_ID_Pos)

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           INITIALIZATIONS                                                */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
void DMA1_Init(void);

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           LOW-LEVEL FUNCTIONS                                            */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
void ADC_Start_DMA(ADC_TypeDef *ADCx, DMA_Channel_TypeDef *DMA_Channelx, uint32_t *pData, uint32_t DataLength);
void ADC_Stop_DMA(ADC_TypeDef *ADCx);

#endif /* CUSTOM_DRIVERS_DMA_INC_DMA_H_ */
