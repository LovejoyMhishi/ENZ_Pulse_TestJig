/*
 * usart.h
 *
 *  Created on: Dec 30, 2025
 *      Author: Lovejoy
 */

/* ******************* ──────────────────────────────────────────────────────────────── ******************* */
/*                                                                                                          *
 *              ╔═════════════════════════════════════════════════════════════════════════╗                 *
 *               ║                            FILE: usart.h                              ║                  *
 *               ║                            AUTHOR: Lovejoy Mhishi                     ║                  *
 *               ║                            DATE: December 30, 2025                    ║                  *
 *              ╠═════════════════════════════════════════════════════════════════════════╣                 *
 *               ║      STM32 USART Driver - Function Prototypes and Definitions         ║                  *
 *              ╚═════════════════════════════════════════════════════════════════════════╝                 *
 * This header file provides function prototypes, type definitions, and external variables.                 *
 *                                                                                                          *
 * Contents:                                                                                                *
 *   - Function Prototypes                                                                                  *
 *   - Variables                                                                                            *
 *                                                                                                          *
 * Intended Use:                                                                                            *
 *   -  Include usart.h in application files (e.g., main.c, app.c) to access usart control APIs.            *
 *                                                                                                          *
 * Dependencies:                                                                                            *                                              *
 *                                                                                                          *
 *                                                                                                          */
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */

#ifndef CUSTOM_DRIVERS_USART_INC_USART_H_
#define CUSTOM_DRIVERS_USART_INC_USART_H_


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
#define PLLCLK_VALUE                         64000000UL
#define USART2_BAUDRATE                      115200U
#define USART_BRR(CLK, BAUD)                 ((CLK + ((BAUD) / 2U)) / (BAUD))


/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           LOW-LEVEL FUNCTIONS                                            */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */

void USART_Init(void);
void UART_Transmit_DMA(USART_TypeDef *USARTx, const uint8_t *pData, uint16_t Size);
#endif /* CUSTOM_DRIVERS_USART_INC_USART_H_ */
