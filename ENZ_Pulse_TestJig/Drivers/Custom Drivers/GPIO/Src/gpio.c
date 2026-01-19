
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */
/*                                                                                                          *
 *              ╔═════════════════════════════════════════════════════════════════════════╗                 *
 *               ║                            FILE: gpio.c                               ║                  *
 *               ║                            AUTHOR: Lovejoy Mhishi                     ║                  *
 *               ║                            DATE: December 22, 2025                    ║                  *
 *              ╠═════════════════════════════════════════════════════════════════════════╣                 *
 *               ║       STM32 GPIO Driver - Function Implementations                    ║                  *
 *              ╚═════════════════════════════════════════════════════════════════════════╝                 *
 * This source file contains bare-metal implementations of GPIO functions for STM32G030xx.                  *
 *                                                                                                          *
 * Contents:                                                                                                *
 *   - GPIO initialization                                                                                  *
 *   - GPIO read/write operations                                                                           *
 *   - GPIO toggle function                                                                                 *
 *                                                                                                          *
 * Intended Use:                                                                                            *
 *   - Include gpio.h in application files (e.g., main.c, app.c) to access GPIO APIs.                       *
 *                                                                                                          *
 * Dependencies:                                                                                            *
 *   - gpio.h header file for function prototypes and data structures.                                      *
 *   - CMSIS device headers (e.g., stm32g030xx.h) for register definitions.                                 *
 *                                                                                                          *
 * Notes:                                                                                                   *
 *   - Enable GPIO peripheral clocks  (Ref. RM0444, p. 106-107).                                            *
 *   - GPIO modes, output type, pull-up/pull-down, and speed registers (Ref. RM0444, p. 221-225).           *
 *   - Alternate function configuration (AFR registers) (Ref. RM0444, p. 226-227).                          *
 *   - Polling or interrupt-driven GPIO can be implemented as needed.                                       *
 *                                                                                                          */
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */


#include "gpio.h"


void GPIO_Init(void) {


	/*
	 * GPIO CLK EN
	 */
	SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN);                      //I/O port A clock enable


	/*
	 * GPIO PA0: VI_SENSE
	 */

	/*
	 * GPIO PA1: +5V_EN
	 */

	/*
	 * GPIO PA2: UART_Pi_RX_ST_TX
	 */
	CLEAR_BIT(GPIOA->MODER, GPIO_MODER_MODE2);                     //Clear MODE6[1:0]
	SET_BIT(GPIOA->MODER, GPIO_MODER_MODE2_1);                     //10: Alternate function mode
	CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT2);                     //0: Output push-pull
	SET_BIT(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED2_1);               //10: High speed
	CLEAR_BIT(GPIOA->PUPDR, GPIO_PUPDR_PUPD2);                     //00: No pull-up, pull-down
	SET_BIT(GPIOA->AFR[0], GPIO_AFRL_AFSEL2_0);                    //PA2-->AFSEL2 & 0001: AF1--> USART2_TX Alternate Function


	/*
	 * GPIO PA3: UART_Pi_TX_ST_RX
	 */
	CLEAR_BIT(GPIOA->MODER, GPIO_MODER_MODE3);                     //Clear MODE6[1:0]
	SET_BIT(GPIOA->MODER, GPIO_MODER_MODE3_1);                     //10: Alternate function mode
	CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT3);                     //0: Output push-pull
	SET_BIT(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED3_1);               //10: High speed
	CLEAR_BIT(GPIOA->PUPDR, GPIO_PUPDR_PUPD3);                     //00: No pull-up, pull-down
	SET_BIT(GPIOA->AFR[0], GPIO_AFRL_AFSEL3_0);                    //PA3-->AFSEL2 & 0001: AF1--> USART2_RX Alternate Function


	/*
	 * GPIO PA4:
	 */

	/*
	 * GPIO PA5: BUTTON_0
	 */

	/*
	 * GPIO PA6: BUZZER
	 */
	CLEAR_BIT(GPIOA->MODER, GPIO_MODER_MODE6);                     //Clear MODE6[1:0]
	SET_BIT(GPIOA->MODER, GPIO_MODER_MODE6_1);                     //10: Alternate function mode
	CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT6);                     //0: Output push-pull
	SET_BIT(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED6_1);               //10: High speed
	CLEAR_BIT(GPIOA->PUPDR, GPIO_PUPDR_PUPD6);                     //00: No pull-up, pull-down
	SET_BIT(GPIOA->AFR[0], GPIO_AFRL_AFSEL6_0);                    //PA6-->AFSEL6 & 0001: AF1--> TIM3_CH1 Alternate Function
	/*
	 * GPIO PA7: LED_3
	 */

	CLEAR_BIT(GPIOA->MODER, GPIO_MODER_MODE7);                     //Clear MODE7[1:0]
	SET_BIT(GPIOA->MODER, GPIO_MODER_MODE7_0);                     //01: General purpose output mode
	CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT7);                     //0: Output push-pull
	CLEAR_BIT(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED7);               //00: Very low speed
	CLEAR_BIT(GPIOA->PUPDR, GPIO_PUPDR_PUPD7);                     //00: No pull-up, pull-down

	/*
	 * GPIO PA8: LED_2
	 */
	CLEAR_BIT(GPIOA->MODER, GPIO_MODER_MODE8);                     //Clear MODE7[1:0]
	SET_BIT(GPIOA->MODER, GPIO_MODER_MODE8_0);                     //01: General purpose output mode
	CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT8);                     //0: Output push-pull
	CLEAR_BIT(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED8);               //00: Very low speed
	CLEAR_BIT(GPIOA->PUPDR, GPIO_PUPDR_PUPD8);                     //00: No pull-up, pull-down
	GPIO_Writepin(GPIOA, LED_2, GPIO_PIN_SET);                     //Pin should be HIGH by Default

	/*
	 * GPIO PA9: LED_1
	 */
	CLEAR_BIT(GPIOA->MODER, GPIO_MODER_MODE9);                     //Clear MODE7[1:0]
	SET_BIT(GPIOA->MODER, GPIO_MODER_MODE9_0);                     //01: General purpose output mode
	CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT9);                     //0: Output push-pull
	CLEAR_BIT(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED9);               //00: Very low speed
	CLEAR_BIT(GPIOA->PUPDR, GPIO_PUPDR_PUPD9);                     //00: No pull-up, pull-down
	GPIO_Writepin(GPIOA, LED_1, GPIO_PIN_SET);                     //Pin should be HIGH by Default

	/*
	 * GPIO PA12[PA10]: LED_0
	 */
	CLEAR_BIT(GPIOA->MODER, GPIO_MODER_MODE12);                     //Clear MODE7[1:0]
	SET_BIT(GPIOA->MODER, GPIO_MODER_MODE12_0);                     //01: General purpose output mode
	CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT12);                     //0: Output push-pull
	CLEAR_BIT(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED12);               //00: Very low speed
	CLEAR_BIT(GPIOA->PUPDR, GPIO_PUPDR_PUPD12);                     //00: No pull-up, pull-down
	GPIO_Writepin(GPIOA, LED_0, GPIO_PIN_SET);                      //Pin should be HIGH by Default

	/*
	 * GPIO PB6: Prog_DRUIDx_TX
	 */

	/*
	 * GPIO PB7: Prog_DRUIDx_RX
	 */

}

void GPIO_Writepin(GPIO_TypeDef *GPIOx, uint16_t Pin, GPIO_PinState State)
{
	if(State == GPIO_PIN_SET)
	{
		WRITE_REG(GPIOx->BSRR, Pin);                                //Drive Pin HIGH
	}
	else
	{
		WRITE_REG(GPIOx->BSRR, (uint32_t)Pin << 16);                //Drive Pin LOW
	}
}
