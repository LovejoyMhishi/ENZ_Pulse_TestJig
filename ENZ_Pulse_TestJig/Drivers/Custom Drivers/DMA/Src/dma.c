
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */
/*                                                                                                          *
 *              ╔═════════════════════════════════════════════════════════════════════════╗                 *
 *               ║                            FILE: dma.c                                ║                  *
 *               ║                            AUTHOR: Lovejoy Mhishi                     ║                  *
 *               ║                            DATE: December 18, 2025                    ║                  *
 *              ╠═════════════════════════════════════════════════════════════════════════╣                 *
 *               ║         STM32 DMA Driver - Initialization & Transfer API              ║                  *
 *              ╚═════════════════════════════════════════════════════════════════════════╝                 *
 * This driver provides low-level functions to configure and manage Direct Memory Access                    *
 * (DMA) transfers on STM32 microcontrollers.                                                               *
 *                                                                                                          *
 * Key Features:                                                                                            *
 *   - Configure DMA channels, and priorities.                                                              *
 *   - Start and monitor memory-to-peripheral or peripheral-to-memory transfers.                            *
 *   - Interrupt callbacks for transfer complete or error events.                                           *
 *                                                                                                          *
 * Dependencies:                                                                                            *
 *   - CMSIS device headers for register definitions.                                                       *
 *   - RCC for enabling DMA peripheral clocks.                                                              *
 *                                                                                                          *
 * Notes:                                                                                                   *
 *   - Ensure DMA and associated peripheral clocks are enabled before use.                                  *
 *   - Polling or interrupt-driven transfers are supported.                                                 *
 *                                                                                                          */
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/* 																											*/
/*                                           INCLUDES                                                       */
/*                                                                                  						*/
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */

#include "dma.h"
#include "stm32g030xx.h"


/* ────────────────────────────────────────────────────────────── /
 * Function : DMA1_Init()
 * Purpose  : Initialize DMA, Enable clock, circular mode
 * Details  : Sets priority, CIRC mode, sizes, and interrupts
 * Runtime  : ~X.Xxx ms
 * ────────────────────────────────────────────────────────────── */
void DMA1_Init(void)
{
	CLEAR_REG(DMA1_Channel1->CCR);
	CLEAR_REG(DMAMUX1_Channel0->CCR);

	SET_BIT(RCC->AHBENR,RCC_AHBENR_DMA1EN);                       //DMA1 and DMAMUX clock enable

	SET_BIT(DMAMUX1_Channel0->CCR, DMAMUX_ADC_DMA );              //Set ADC as DMA trigger**********************
	/*
	 * Enable NVIC IRQ
	 */
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

	CLEAR_BIT(DMA1_Channel1->CCR, DMA_CCR_EN);                    // Disable channel first for configuration
	/*
	 *  Configure DMA_CCRx register parameters
	 */
	SET_BIT(DMA1_Channel1->CCR, DMA_CCR_PL_1);                    //10: Channel priority
	CLEAR_BIT(DMA1_Channel1->CCR, DMA_CCR_DIR);                   //0: Data transfer direction (Peripheral--->Memory)
	CLEAR_BIT(DMA1_Channel1->CCR, DMA_CCR_CIRC);                  //0: Circular mode disabled***********************
	CLEAR_BIT(DMA1_Channel1->CCR, DMA_CCR_PINC);                  //0: Peripheral incremented mode disabled
	SET_BIT(DMA1_Channel1->CCR, DMA_CCR_MINC);                    //1: Memory incremented mode EN
	SET_BIT(DMA1_Channel1->CCR, DMA_CCR_PSIZE_0);                 //01: Peripheral size 16Bits
	SET_BIT(DMA1_Channel1->CCR, DMA_CCR_MSIZE_0);                 //01: Memory size 16Bits
	SET_BIT(DMA1_Channel1->CCR, DMA_CCR_TCIE);                    //1: Transfer complete interrupt EN
}

/* ────────────────────────────────────────────────────────────── /
 * Function : ADC_Start_DMA()
 * Purpose  : Start ADC conversions with DMA
 * Details  : Configures DMA and starts ADC conversions
 * Runtime  : ~X.Xxx ms
 * ────────────────────────────────────────────────────────────── */
void ADC_Start_DMA(ADC_TypeDef *ADCx, DMA_Channel_TypeDef *DMA_Channelx, uint32_t *pData, uint32_t DataLength)
{
	SET_BIT(ADCx->CR, ADC_CR_ADSTP);
	while(ADCx->CR & ADC_CR_ADSTART)
	{
		//wait until fully stopped
	}

	//CLEAR_BIT(ADCx->CFGR1, ADC_CFGR1_CONT);

	CLEAR_BIT(DMA_Channelx->CCR, DMA_CCR_EN);                    // Disable channel first for configuration

	WRITE_REG(DMA_Channelx->CPAR, (uint32_t)&ADCx->DR);          //Set the peripheral register address in the DMA_CPARx register.
	WRITE_REG(DMA_Channelx->CMAR, (uint32_t)pData);              //Set the memory address in the DMA_CMARx register.
	WRITE_REG(DMA_Channelx->CNDTR, DataLength);                  //Configure the total number of data to transfer in the DMA_CNDTRx register.


	SET_BIT(DMA_Channelx->CCR, DMA_CCR_EN);                      //1: Channel EN

	/*
	 * ADC Conversions
	 */
	CLEAR_BIT(ADCx->CFGR1, ADC_CFGR1_DMACFG);                     //0: DMA one shot mode selected /****************************
	SET_BIT(ADCx->CFGR1, ADC_CFGR1_DMAEN);                        //1: Direct memory access EN
	SET_BIT(ADCx->CR, ADC_CR_ADSTART);                            //Starting conversions (ADSTART)

}

void ADC_Stop_DMA(ADC_TypeDef *ADCx)
{
	CLEAR_BIT(ADCx->CFGR1, ADC_CFGR1_DMAEN);                       //0: Direct memory access Disable
	SET_BIT(ADCx->CR, ADC_CR_ADSTP);
	while (ADCx->CR & ADC_CR_ADSTART);
	CLEAR_BIT(DMA1_Channel1->CCR, DMA_CCR_EN);                     //0: Channel Disable
}



