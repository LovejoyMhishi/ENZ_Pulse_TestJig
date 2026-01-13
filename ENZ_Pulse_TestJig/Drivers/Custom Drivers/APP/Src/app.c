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
#include "stdlib.h"
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           VARIABLES                                                      */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
volatile Energizer ENZ;
ENZ_TST_JIG EVENT = SCAN;

static uint8_t ADCSampleCnt;
static const double T_Sample = 0.78125e-6f;                  //Total Time per sample:(12.5 + 12.5)(1/f_adc )
static double Pulse_Duration;

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
	if(ADC1->DR >= 50 ) {

		ADC_Start_DMA(ADC1, DMA1_Channel1, (uint32_t*)ENZ.PULSE, 400);
		//ADC1_Stop();
		ADC_Cmplt = true;


	}

}


void ENZ_PULSE_Interval(void) {

	//	if(ADC1->DR <= 124 &&  ADC_Strtd ) {
	//		//if(PULSE CNT)
	//	}
}
void ENZ_PULSE_DataProc(void) {
	if(ADC_Cmplt) {

		ADCSampleCnt = 0;
		ENZ.ADC_Peak = 0;
		Pulse_Duration = 0;

		if((ENZ.PULSE[199] > 124))  {
			/*
			 * If the buffer is full and
			 */
			ADC1_Start();

		}
		for (int var = 0; var < 200; ++var) {

			if(ENZ.PULSE[var] <= 124) {
				break;
			}

			if(ENZ.PULSE[var] > ENZ.ADC_Peak) {
				ENZ.ADC_Peak = ENZ.PULSE[var];
			}

			ADCSampleCnt++;
		}
		Pulse_Duration = ADCSampleCnt*T_Sample;

		//DEBUG_ENZ();

		ADC_Cmplt = false;
		ADC1_Start();



	}

}


void ENZ_PULSE_EVENTS(void) {
	switch(EVENT) {

	case SCAN:
		if(ADC1->DR >= 50 ) {

			ADC_Start_DMA(ADC1, DMA1_Channel1, (uint32_t*)ENZ.PULSE, 400);
			//ADC1_Stop();
			//ADC_Cmplt = true;

			EVENT++;
		}

	case FIRST_PULSE:
		if(ADC_Cmplt) {

			ADCSampleCnt = 0;
			ENZ.ADC_Peak = 0;
			Pulse_Duration = 0;

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
			Pulse_Duration = ADCSampleCnt*T_Sample;

			//DEBUG_ENZ();
			ADC_Cmplt = false;
			ADC1_Start();
		}

	case SECOND_PULSE:

	}
}
