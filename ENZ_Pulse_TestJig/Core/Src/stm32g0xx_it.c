/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32g0xx_it.c
 * @brief   Interrupt Service Routines.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "app.h"
#include "tim.h"
#include "ui.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
	/* USER CODE BEGIN NonMaskableInt_IRQn 0 */

	/* USER CODE END NonMaskableInt_IRQn 0 */
	/* USER CODE BEGIN NonMaskableInt_IRQn 1 */
	while (1)
	{
	}
	/* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
	/* USER CODE BEGIN HardFault_IRQn 0 */

	/* USER CODE END HardFault_IRQn 0 */
	while (1)
	{
		/* USER CODE BEGIN W1_HardFault_IRQn 0 */
		/* USER CODE END W1_HardFault_IRQn 0 */
	}
}

/**
 * @brief This function handles System service call via SWI instruction.
 */
void SVC_Handler(void)
{
	/* USER CODE BEGIN SVC_IRQn 0 */

	/* USER CODE END SVC_IRQn 0 */
	/* USER CODE BEGIN SVC_IRQn 1 */

	/* USER CODE END SVC_IRQn 1 */
}

/**
 * @brief This function handles Pendable request for system service.
 */
void PendSV_Handler(void)
{
	/* USER CODE BEGIN PendSV_IRQn 0 */

	/* USER CODE END PendSV_IRQn 0 */
	/* USER CODE BEGIN PendSV_IRQn 1 */

	/* USER CODE END PendSV_IRQn 1 */
}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void)
{
	/* USER CODE BEGIN SysTick_IRQn 0 */

	/* USER CODE END SysTick_IRQn 0 */
	HAL_IncTick();
	/* USER CODE BEGIN SysTick_IRQn 1 */

	/* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/

/* USER CODE BEGIN 1 */
/* ────────────────────────────────────────────────────────────── /
 * Handler  : DMA1_Channel1_IRQHandler()
 * Purpose  : Gets triggered after ENZ.PULSE[500] gets full
 * Details  : Sets ADC flags used in the main FSM
 * Runtime  : ~X.Xxx
 * ────────────────────────────────────────────────────────────── */
void DMA1_Channel1_IRQHandler(void)
{
	if (DMA1->ISR & DMA_ISR_TCIF1)
	{
		ENZ.PulseActive = false;
		ADC_Cmplt = true;
		SET_BIT(DMA1->IFCR, DMA_IFCR_CGIF1);
		ADC1_Start();
	}
}
/* ────────────────────────────────────────────────────────────── /
 * Handler  : ADC1_IRQHandler()
 * Purpose  : Gets triggered after a pulse has been detected
 * Details  : ADC Flags and returns the ENZ.PULSE_T
 * Runtime  : ~X.Xxx
 * ────────────────────────────────────────────────────────────── */
void ADC1_IRQHandler(void)
{
	if (READ_BIT(ADC1->ISR, ADC_ISR_AWD1))
	{
		ENZ_PULSE_CNT();

		if(ENZ.PULSE_CNT  == 2)
		{
			ENZ_ReadPulse_T();
		}
		SET_BIT(ADC1->ISR, ADC_ISR_AWD1);
	}
}

/* ────────────────────────────────────────────────────────────── /
 * Handler  : TIM14_IRQHandler()
 * Purpose  : Gets triggered after 5s has elapsed
 * Details  : Used to Count the Time between pulses: TIM14->CNT
 * Runtime  : ~X.Xxx
 * ────────────────────────────────────────────────────────────── */
void TIM14_IRQHandler(void)
{
	if (READ_BIT(TIM14->SR, TIM_SR_UIF))
	{
		ENZ.V_Peak = 0;
		ENZ.I_Peak = 0;
		ENZ.Energy_J = 0;
		ENZ.ADC_Peak = 0;
		ENZ.PULSE_CNT = 0;
		ENZ.PULSE_Width = 0;
		ENZ.PULSE_T = 0;
		EVENT = SCAN;
		CLEAR_BIT(TIM14->SR,  TIM_SR_UIF);
	}
}


void TIM16_IRQHandler(void)
{
	if(READ_BIT(TIM16->SR, TIM_SR_UIF))
	{
		Button.StateHistory = (Button.StateHistory << 1) | (READ_BIT(GPIOA->IDR, GPIO_IDR_ID5));
		Button.StateCount++;
		CLEAR_BIT(TIM16->SR,  TIM_SR_UIF);
	}

}

//void SysTick_Handler(void){
//
//}
/* USER CODE END 1 */
