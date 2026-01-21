
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */
/*                                                                                                          *
 *              ╔═════════════════════════════════════════════════════════════════════════╗                 *
 *               ║                            FILE: gpio.h                               ║                  *
 *               ║                            AUTHOR: Lovejoy Mhishi                     ║                  *
 *               ║                            DATE: December 22, 2025                    ║                  *
 *              ╠═════════════════════════════════════════════════════════════════════════╣                 *
 *               ║       STM32 GPIO Driver - Function Prototypes and Definitions         ║                  *
 *              ╚═════════════════════════════════════════════════════════════════════════╝                 *
 * This header file provides function prototypes, type definitions, and external variables                  *
 * for the STM32 GPIO driver.                                                                               *
 *                                                                                                          *
 * Contents:                                                                                                *
 *   - GPIO pin modes: Input, Output, Alternate Function, Analog                                            *
 *   - Output types: Push-Pull, Open-Drain                                                                  *
 *   - Pull-up/Pull-down configurations                                                                     *
 *   - Function prototypes for GPIO initialization and control                                              *
 *                                                                                                          *
 * Intended Use:                                                                                            *
 *   - Include in application files (e.g., app.c, main.c) to access GPIO driver APIs.                       *
 *                                                                                                          *
 * Dependencies:                                                                                            *
 *   - CMSIS device headers (e.g., stm32g030xx.h) for register definitions.                                 *
 *   - gpio.c implementation file containing the function bodies.                                           *
 *                                                                                                          */
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */

#ifndef CUSTOM_DRIVERS_GPIO_INC_GPIO_H_
#define CUSTOM_DRIVERS_GPIO_INC_GPIO_H_
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
#define VI_SENSE                            GPIO_PIN_0   //GPIO PA0
#define Plus_5V_EN                          GPIO_PIN_1   //GPIO PA1
#define UART_Pi_RX_ST_TX                    GPIO_PIN_2   //GPIO PA2
#define UART_Pi_TX_ST_RX                    GPIO_PIN_3   //GPIO PA3

#define BUTTON_0                            GPIO_PIN_5   //GPIO PA5
#define BUZZER                              GPIO_PIN_6   //GPIO PA6
#define LED_3                               GPIO_PIN_7   //GPIO PA7
#define LED_2                               GPIO_PIN_8   //GPIO PA8
#define LED_1                               GPIO_PIN_11   //GPIO PA9
#define LED_0                               GPIO_PIN_12  //GPIO PA10
#define Prog_DRUIDx_TX                      GPIO_PIN_6   //GPIO PB6
#define Prog_DRUIDx_RX                      GPIO_PIN_7   //GPIO PB7

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           LOW-LEVEL FUNCTIONS                                            */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
void GPIO_Init(void);
void GPIO_Writepin(GPIO_TypeDef *GPIOx, uint16_t pin, GPIO_PinState State);

#endif /* CUSTOM_DRIVERS_GPIO_INC_GPIO_H_ */
