
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */
/*                                                                                                          *
 *              ╔═════════════════════════════════════════════════════════════════════════╗                 *
 *               ║                            FILE: usart.c                              ║                  *
 *               ║                            AUTHOR: Lovejoy Mhishi                     ║                  *
 *               ║                            DATE: December 30, 2025                    ║                  *
 *              ╠═════════════════════════════════════════════════════════════════════════╣                 *
 *               ║       STM32 USART Driver - Function Implementations                   ║                  *
 *              ╚═════════════════════════════════════════════════════════════════════════╝                 *
 * This source file contains bare-metal implementations of USART functions for STM32G030xx.                 *
 *                                                                                                          *
 * Contents:                                                                                                *
 *   - USART initialization and data transmission/reception                                                 *
 *   - GPIO read/write operations                                                                           *
 *   - GPIO toggle function                                                                                 *
 *                                                                                                          *
 * Intended Use:                                                                                            *
 *   - Include gpio.h  and usart.h in application files (e.g., main.c, app.c) to access GPIO APIs.          *
 *                                                                                                          *
 * Dependencies:                                                                                            *
 *   - usart.h for USART function prototypes.                                                               *
 *   - CMSIS device headers (e.g., stm32g030xx.h) for register definitions.                                 *
 *                                                                                                          *
 * Notes:                                                                                                   *
 *   - Enable GPIO and USART peripheral clocks before configuring pins or registers.                        *
 *   - GPIO modes, output type, pull-up/pull-down, and speed registers (Ref. RM0444, p. 221-225).           *
 *   - Alternate function configuration (AFR registers) (Ref. RM0444, p. 226-227).                          *
 *   - USART BRR calculation (OVER8 = 0):                                                                   *
 *       BRR = round(USART_CLK / baudrate)                                                                  *
 *       Rounding via: (HSI_VALUE + (USART2_BAUDRATE/2)) / USART2_BAUDRATE                                  *
 *       Hardware splits BRR into Mantissa (BRR[15:4]) and Fraction (BRR[3:0]) for precise baud control.    *
 *                                                                                                          */
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/* 																											*/
/*                                           INCLUDES                                                       */
/*                                                                                  						*/
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
#include "usart.h"


/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           INITIALIZATION                                                 */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */

void USART_Init(void) {
	SET_BIT(RCC->APBENR1, RCC_APBENR1_USART2EN);                   //
	CLEAR_BIT(USART2->CR1, USART_CR1_M0 | USART_CR1_M1);           //M[1:0] = ‘00’: 1 start bit, 8 Data bits, n Stop bit
	WRITE_REG(USART2->BRR, USART_BRR(PLLCLK_VALUE, USART2_BAUDRATE)); //BRR[15:0]: USART baud rate
	CLEAR_BIT(USART2->CR2, USART_CR2_STOP );                       //00: 1 stop bit
	SET_BIT(USART2->CR1, USART_CR1_UE);                            //1: USART EN
	//SET_BIT(USART2->CR3, USART_CR3_DMAT);                        //1: DMA mode is enabled for transmission
	SET_BIT(USART2->CR1, USART_CR1_TE);                            //1: Transmitter is enabled
}

void UART_Transmit_DMA(USART_TypeDef *USARTx, const uint8_t *pData, uint16_t Size) {

	while(Size--) {
		while( !( USARTx->ISR & USART_ISR_TXE_TXFNF ) ) {}           //Wait for TXE=1:TDR is empty

		WRITE_REG(USARTx->TDR, *pData++);                            //Take the value pointed to by pData

	}

}


