
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */
/*                                                                                                          *
 *              ╔═════════════════════════════════════════════════════════════════════════╗                 *
 *               ║                            FILE: adc.c                                ║                  *
 *               ║                            AUTHOR: Lovejoy Mhishi                     ║                  *
 *               ║                            DATE: December 8, 2025                     ║                  *
 *              ╠═════════════════════════════════════════════════════════════════════════╣                 *
 *               ║       STM32 ADC Driver - Low-Level Initialization and Read API        ║                  *
 *              ╚═════════════════════════════════════════════════════════════════════════╝                 *
 * This driver provides low-level functions to configure and read from the Analog-to-Digital Converter      *
 * (ADC) peripheral of STM32 microcontrollers.                                                              *
 *                                                                                                          *
 * Key Features:                                                                                            *
 *   - Initializes ADC peripheral with user-defined channel and sample time.                                *
 *   - Starts single conversions and retrieves digital values from the ADC data register.                   *
 *   - Supports threshold-based logic for GPIO control or signal monitoring.                                *
 *                                                                                                          *
 * Intended Use:                                                                                            *
 *   - To read analog signals such as sensors, potentiometers, or voltage levels.                           *
 *   - To interface with GPIO outputs for simple decision-making based on ADC values.                       *
 *                                                                                                          *
 * Dependencies:                                                                                            *
 *   - CMSIS device headers (stm32g030xx.h) for register definitions.                                       *
 *   - RCC clock configuration (system_init.c) to enable ADC peripheral clock.                              *
 *                                                                                                          *
 * Notes:                                                                                                   *
 *   - Ensure the ADC clock is enabled via RCC before initialization.                                       *
 *   - Configure the GPIO pin connected to the ADC channel as analog input.                                 *
 *   - Polling mode is used in this driver; interrupt or DMA-based sampling can be added later.             *
 *   - ADC resolution and reference voltage depend on the specific STM32 family.                            *
 *                                                                                                          */
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */



/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/* 																											*/
/*                                           INCLUDES                                                       */
/*                                                                                  						*/
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
#include "adc.h"
#include "helpers.h"
#include "stm32g030xx.h"
#include "stdio.h"
#include "main.h"
#include <stdbool.h>
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                          DATA PROCESSING                                                 */
/*																						 				    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */

uint16_t ENZ_Out_V[200];
bool ADC_Cmplt = false;
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/* 																											*/
/*                                           CONFIGURATIONS:                                                */
/*                        CLOCK, RESOLUTION, DATA ALIGNMENT & NUMBER OF CONVERSION 							*/
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/* ────────────────────────────────────────────────────────────── /
 * Function : ADC1_Init()
 * Purpose  : Start an ADC1 conversion
 * Details  : EN ADC, sets RES, channel, sampling, and CONT mode
 * Runtime  : ~X.Xxx
 * ────────────────────────────────────────────────────────────── */
void ADC1_Init(void) {
	/*
	 * 14.3.5: ADC clock (CKMODE, PRESC[3:0])
	 */
	SET_BIT(RCC->APBENR2,RCC_APBENR2_ADCEN);                      //Enable ADC peripheral clock



	CLEAR_BIT(ADC1->CR, ADC_CR_ADEN);                             //ADC is disabled (OFF state)
	CLEAR_BIT(ADC1->CFGR2, ADC_CFGR2_CKMODE );                    //CKMODE = 00
	CLEAR_BIT(ADC->CCR, ADC_CCR_PRESC);                           //000: Clear Prescaller Bits
	SET_BIT(ADC->CCR, 0b0001<<ADC_CCR_PRESC_Pos);                 //0001: Input ADC clock divided by 2
	CLEAR_BIT(ADC1->CFGR1, ADC_CFGR1_RES);                        //00: 12 bits Resolution
	CLEAR_BIT(ADC1->CFGR1, ADC_CFGR1_ALIGN);                      //0: Right alignment

	/*
	 * 14.3.2: ADC voltage regulator Enabling(ADVREGEN)
	 */
	SET_BIT(ADC1->CR, ADC_CR_ADVREGEN);
	TimeOut(t_ADCVREG_SETUP);

	/*
	 * 14.3.3: Calibration (ADCAL)
	 */
	if(((ADC1->CR & ADC_CR_ADEN) == 0) && ((ADC1->CR & ADC_CR_ADVREGEN) !=0) && ((ADC1->CFGR1 & ADC_CFGR1_DMAEN) == 0)) {

		SET_BIT(ADC1->CR, ADC_CR_ADCAL);                          //Set ADCAL = 1 after ensuring that ADEN = 0, ADVREGEN = 1 and DMAEN = 0.

	}

	/*
	 * Wait until ADCAL = 0
	 */
	while(ADC1->CR & ADC_CR_ADCAL) {
		/*
		 * ⏳... After Waiting, The calibration factor can be read from bits 6:0 of ADC_DR or ADC_CALFACT registers.
		 */
	}

	/*
	 * 14.3.8: Channel selection (CHSEL, SCANDIR, CHSELRMOD)
	 */
	CLEAR_BIT(ADC1->CR, ADC_CR_ADSTART);                          //Ensures that no conversion is ongoing

	if(!(ADC1->ISR & ADC_ISR_CCRDY) ) {                           //Check ADC channel configuration ready flag
		TimeOut(1);
	}

	if(ADC1->ISR & ADC_ISR_CCRDY) {
		CLEAR_BIT(ADC1->CFGR1, ADC_CFGR1_CHSELRMOD);              //0: Each bit of the ADC_CHSELR register enables an input
	}


	//ADC1->CHSELR = 0;
	SET_BIT(ADC1->CHSELR, ADC_CHSELR_CHSEL0);                     //ADC channel selection : PA0 -> Channel 0

	/*
	 *  14.3.9: Programmable sampling time (SMPx[2:0])
	 */
	CLEAR_BIT(ADC1->SMPR, ADC_SMPR_SMP1);                         //000: 1.5 ADC clock cycles
	SET_BIT(ADC1->SMPR, (0b011 << ADC_SMPR_SMP1_Pos));            //011: 12.5 ADC clock cycles

	/*
	 * 14.3.10+11: Conversion modes (CONT = 0 & CONT = 1)
	 */
	SET_BIT(ADC1->CFGR1, ADC_CFGR1_CONT);                          //1: Continuous conversion mode*********


	/*
	 * 14.3.4: ADC on-off control (ADEN, ADDIS, ADRDY)
	 */
	CLEAR_BIT(ADC1->ISR, ADC_ISR_ADRDY);                          //Clear the ADRDY bit in ADC_ISR register by programming this bit to 1.
	SET_BIT(ADC1->CR, ADC_CR_ADEN);                               //Set ADEN = 1 in the ADC_CR register.

	while (!(ADC1->ISR & ADC_ISR_ADRDY)) {                        //Wait until ADRDY = 1 in the ADC_ISR register
		/*
		 * ⏳...
		 */
	}

}

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           RUNTIME DATA ACQUISITION                                       */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/* ────────────────────────────────────────────────────────────── /
 * Function : ADC1_Start()
 * Purpose  : Start an ADC1 conversion
 * Details  : Sets ADSTART to begin single/ continuous conversions
 * Runtime  : ~X.Xxx
 * ────────────────────────────────────────────────────────────── */
void ADC1_Start(void) {
	/*
	 *  14.3.12: Starting conversions (ADSTART)
	 */
	SET_BIT(ADC1->CFGR1, ADC_CFGR1_CONT);                          //1: Continuous conversion mode*********
	SET_BIT(ADC1->CR, ADC_CR_ADSTART);
}
void ADC1_Stop(void) {

	CLEAR_BIT(ADC1->CFGR1, ADC_CFGR1_CONT);                          //1: Continuous conversion mode*********

	SET_BIT(ADC1->CR, ADC_CR_ADSTP);
	while(ADC1->CR & ADC_CR_ADSTART) {

	}

}
/* ────────────────────────────────────────────────────────────── /
 * Function : ADC1_Read()
 * Purpose  : Read ADC1 conversion result
 * Details  : Returns ADC result after conversion
 * Runtime  : ~X.Xxx
 * ────────────────────────────────────────────────────────────── */
uint16_t ADC1_Read(void){

	while (!(ADC1->ISR & ADC_ISR_EOC));                           //Wait for end of conversion (EOC)

	return ADC1->DR;                                              //Read the value
}

void ADC_AWD_Thrds_Config(void) {
	SET_BIT(ADC1->CFGR1, ADC_CFGR1_AWD1EN);                       //1: Analog watchdog 1 EN
	SET_BIT(ADC1->CFGR1, ADC_CFGR1_AWD1SGL);                      //1: Analog watchdog 1 EN on a single channel
	CLEAR_BIT(ADC1->CFGR1, ADC_CFGR1_AWD1CH);                     //00000: ADC analog input Channel 0 monitored by AWD


	WRITE_REG(ADC1->AWD1TR, ADC_AWD1TR_HT1_10);                   //1024 HTx THR
	SET_BIT(ADC1->IER, ADC_IER_AWD1IE);                           //Analog watchdog 1 interrupt enable

	/*
	 * Enable NVIC IRQ
	 */
	NVIC_SetPriority(ADC1_IRQn, 3);

}

void  ADC_AWD_Thrds_Clear(void) {
	ADC1_Stop();
	CLEAR_BIT(ADC1->CFGR1, ADC_CFGR1_AWD1EN);                       //0: Analog watchdog 1 Disable
	CLEAR_BIT(ADC1->CFGR1, ADC_CFGR1_AWD1SGL);                      //0: Analog watchdog 1 EN on a single channel
	NVIC_DisableIRQ(ADC1_IRQn);
}

