/* ******************* ──────────────────────────────────────────────────────────────── ******************* */
/*                                                                                                          *
 *              ╔═════════════════════════════════════════════════════════════════════════╗                 *
 *               ║                            FILE: app.c                                ║                  *
 *               ║                            AUTHOR: Lovejoy Mhishi                     ║                  *
 *               ║                            DATE: January 7, 2025                      ║                  *
 *              ╠═════════════════════════════════════════════════════════════════════════╣                 *
 *               ║                Main Application Logic for ENZ_PULSE_TestJig           ║                  *
 *              ╚═════════════════════════════════════════════════════════════════════════╝                 *
 * ENZ data processing and programming function bodies                                                      *
 *                                                                                                          *
 * Contents:                                                                                                *
 *   - Process ADC Data to determine a PASS/FAIL of an ENZ                                                  *
 *   - Program Serial Numbers to the DRUIDs                                                                 *
 *                                                                                                          *
 * Intended Use:                                                                                            *
 *   -  Include app.h in application files (e.g., main.c, app.c) to access app control logic.               *
 *                                                                                                          *
 * Dependencies:                                                                                            *
 *   - ui.h header file for function prototypes and data structures.                                        *
 *   - tim.c and gpio.c for timer- and GPIO-based functions                                                 *
 *                                                                                                          *
 * Notes:                                                                                                   *
 *   - GPIO and TIMx must be initialized before calling UI functions.                                       *
 *                                                                                                          */
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */


/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/* 																											*/
/*                                           INCLUDES                                                       */
/*                                                                                  						*/
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
#include "app.h"
#include "dma.h"
#include "tim.h"
#include "stdlib.h"
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           VARIABLES                                                      */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */

volatile Energizer ENZ = {
		.PULSE = {0},
		.V_Peak = 0,
		.I_Peak = 0,
		.Energy_J =0,
		.ADC_Peak = 0,
		.Buff_Num = 0,
		.PULSE_CNT = 0,
		.PULSE_Width = 0,
		.PULSE_T = 0,
		.PulseActive = false,
		.Buff_Full = false
};

ENZ_TST_JIG EVENT = SCAN;
bool One_Sec_Elapsed = false;

uint8_t Sec = 0;
static uint16_t ADCSampleCnt;
bool THIRD = false;
volatile uint16_t third = 0;
volatile uint16_t ENZ_THIRD_PULSE[400];


/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           HIGH-LEVEL FUNCTIONS                                           */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/* ────────────────────────────────────────────────────────────── /
 * Function : ENZ_PULSE_DataProc()
 * Purpose  : Process ADC1 data
 * Details  : max(ENZ_DATA[]), determines PULSE duration
 * Runtime  : ~X.Xxx
 * ────────────────────────────────────────────────────────────── */
void ENZ_PULSE_Detector(void){
	if(ADC1->DR >= 80  && !ENZ.PulseActive) {
		if(!ENZ.PULSE_CNT) {
			//ADC_Start_DMA(ADC1, DMA1_Channel1, (uint32_t*)ENZ.PULSE, 400);
			TIMx_Start(TIM14);
			ENZ_PULSE_Counting();
			ENZ.PulseActive = true;

		}
		else{
			ENZ_PULSE_Counting();
		}
	}
}
void ENZ_PULSE_EVENTS(void) {
	switch(EVENT) {

	case SCAN:
		if(ADC1->DR >= 80  && !ENZ.PulseActive) {
			if(!ENZ.PULSE_CNT) {
				ADC_Start_DMA(ADC1, DMA1_Channel1, (uint32_t*)ENZ.PULSE, 400);
				TIMx_Start(TIM14);
				ENZ_PULSE_Counting();
				ENZ.PulseActive = true;
				EVENT= FIRST_PULSE;
			}
			else{
				ENZ_PULSE_Counting();
				EVENT= SECOND_PULSE;
			}
		}
		break;

	case FIRST_PULSE:
		if(ADC_Cmplt) {
			if((ENZ.PULSE[399] > 124))  {
				/*
				 * If the buffer is full: Pulse is > 312.5 µs:FAIL the ENZ
				 */
			}
			for (int var = 0; var < 400; ++var) {

				if(ENZ.PULSE[var] <= 80) {
					break;
				}

				if(ENZ.PULSE[var] > ENZ.ADC_Peak) {
					ENZ.ADC_Peak = ENZ.PULSE[var];
				}

				ADCSampleCnt++;
			}

			ENZ.PULSE_Width = ADCSampleCnt*ENZ_PULSE_SAMPLE_TIME_s ;

//			DEBUG_ENZ();
			ADC_Cmplt = false;
			EVENT = SCAN;
		}
		break;
	case SECOND_PULSE:
		if(ENZ.PULSE_CNT==2) {
			ENZ.V_Peak = ENZ.ADC_Peak*ADC_TO_VOLTAGE_SCALE;
			ENZ.I_Peak = ENZ.V_Peak / ENZ_LOAD_RES;
			for (int var = 0; var < ADCSampleCnt; ++var) {

				float V_actual = (float)ENZ.PULSE[var]*ADC_TO_VOLTAGE_SCALE;
				float dE = (V_actual * V_actual)*ENZ_ENERGY_K;
				ENZ.Energy_J += dE;
			}
			ENZ.PULSE_Width = ADCSampleCnt*ENZ_PULSE_SAMPLE_TIME_s;
			ENZ.PULSE_CNT = 0;
			//ADC1_Stop();
			EVENT = DATA_PROCESSING;


		}
		break;

	case DATA_PROCESSING:
//		ADC1_Start();


		uint32_t TimeElapsed;
		uint32_t DATA;
		DATA = ADC1 -> DR ;
		TimeElapsed = ((uint32_t)(TIM14->CNT)*(TIM14->PSC +1)/64);
		if((TimeElapsed >= 2*ENZ.PULSE_T-100 ) && !THIRD && DATA >=80) {

//			ADC_Start_DMA(ADC1, DMA1_Channel1, (uint32_t*)ENZ_THIRD_PULSE, 400);
			for(uint32_t i=0; i<400; i++) {
				SET_BIT(ADC1->CR, ADC_CR_ADSTART);      // start single conversion
				while(!(ADC1->ISR & ADC_ISR_EOC));     // wait for end of conversion
				ENZ_THIRD_PULSE[i] = ADC1->DR;         // store sample manually
			}
			THIRD = true;
			EVENT =  SCAN;
		}

		break;
	}
}


