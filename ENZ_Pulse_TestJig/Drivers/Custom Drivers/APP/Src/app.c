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
static uint8_t ADCSampleCnt;



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

void ENZ_PULSE_Detector(void) {
	if(ADC1->DR >= 50  && !ENZ.PulseActive) {
		ADC_Start_DMA(ADC1, DMA1_Channel1, (uint32_t*)ENZ.PULSE, 400);
		TIMx_Start(TIM14);
		ENZ_PULSE_Counting();
		ENZ.PulseActive = true;
	}
}


void ENZ_PULSE_DataProc(void) {
	if(ADC_Cmplt) {
		if((ENZ.PULSE[399] > 50))  {
			/*
			 * If the buffer is full and
			 */
			//ADC1_Start();

		}
		for (int var = 0; var < 400; ++var) {

			if(ENZ.PULSE[var] <= 50) {
				break;
			}

			if(ENZ.PULSE[var] > ENZ.ADC_Peak) {
				ENZ.ADC_Peak = ENZ.PULSE[var];
			}

			ADCSampleCnt++;
		}
		ENZ.PULSE_Width = ADCSampleCnt*ENZ_PULSE_SAMPLE_TIME_s ;

		//DEBUG_ENZ();

		ADC_Cmplt = false;
	}
}


void ENZ_PULSE_EVENTS(void) {
	switch(EVENT) {

	case SCAN:
		if(ADC1->DR >= 50  && !ENZ.PulseActive) {
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

				if(ENZ.PULSE[var] <= 124) {
					break;
				}

				if(ENZ.PULSE[var] > ENZ.ADC_Peak) {
					ENZ.ADC_Peak = ENZ.PULSE[var];
				}

				ADCSampleCnt++;
			}
			ENZ.PULSE_Width = ADCSampleCnt*ENZ_PULSE_SAMPLE_TIME_s ;

			//DEBUG_ENZ();
			ADC_Cmplt = false;
			EVENT = SCAN;
		}
		break;
	case SECOND_PULSE:
		if(ENZ.PULSE_CNT==2) {
			for (int var = 0; var < ADCSampleCnt; ++var) {
				ENZ.Energy_J +=(ENZ.PULSE[var] * (ENZ.PULSE[var]) * ENZ_PULSE_SAMPLE_TIME_s);
			}
			ENZ.PULSE_Width = ADCSampleCnt*ENZ_PULSE_SAMPLE_TIME_s;
		}
	}
}


