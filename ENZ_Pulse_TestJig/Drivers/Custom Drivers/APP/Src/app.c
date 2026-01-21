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
 *   - To test ENZs and determine if they comply SANS/IEC 60335-2-76 standards                              *
 *                                                                                                          *
 * Dependencies:                                                                                            *
 *   - app.h header file for function prototypes and data structures.                                       *
 *   - stdlib.h, dma.h and tim.h                                                                            *
 *                                                                                                          *
 * Notes:                                                                                                   *
 *   - GPIO and TIMx must be initialized on startup                                                         *
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
		.PULSE_CNT = 0,
		.PULSE_Width = 0,
		.PULSE_T = 0,
		.PulseActive = false
};

ENZ_TST_JIG EVENT = SCAN;

static uint16_t ADCSampleCnt;



/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           HIGH-LEVEL FUNCTIONS                                           */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/* ────────────────────────────────────────────────────────────── /
 * Function : ENZ_PULSE_EVENTS()
 * Purpose  : SCANs & PROCESS the ENZ PULSES
 * Details  : Returns a PASS/FAIL for an ENZ
 * Runtime  : ~X.Xxx
 * ────────────────────────────────────────────────────────────── */

void ENZ_PULSE_EVENTS(void) {
	switch(EVENT)
	{
	case SCAN:
		if(ADC1->DR >= 80  && !ENZ.PulseActive)
		{
			if(!ENZ.PULSE_CNT)
			{
				ADC_Start_DMA(ADC1, DMA1_Channel1, (uint32_t*)ENZ.PULSE, ENZ_MAX_PULSE_LENGHT);
				TIMx_Start(TIM14);                                   //Start Tim14 for ENZ.PULSE_T
				ENZ_PULSE_Counting();                                //Detect the 1st PULSE
				ENZ.PulseActive = true;
				EVENT= FIRST_PULSE;
			}
			else
			{
				ENZ_PULSE_Counting();                                //Detect the 2nd PULSE and poll TIMx->CNT for ENZ.PULSE_T
				EVENT= SECOND_PULSE;
			}
		}
		break;

	case FIRST_PULSE:
		if(ADC_Cmplt) {
			if(ENZ.PULSE[ENZ_MAX_PULSE_LENGHT - 1 ] > 50 && ENZ.PULSE[ENZ_MAX_PULSE_LENGHT - 2] > 50 && ENZ.PULSE[ENZ_MAX_PULSE_LENGHT - 3] > 50)
			{
				ENZ_FAILED();                                        //PULSE is Longer than average ENZ has FAILED!
				TIMx_Stop(TIM14);
				ADC_Cmplt = false;
				EVENT = SCAN;
			}
			for (int var = 0; var < ENZ_MAX_PULSE_LENGHT; ++var)
			{
				if(ENZ.PULSE[var] <= 50)
				{
					break;                                           //Break For Loop once ADC values starts dropping below the THR
				}

				if(ENZ.PULSE[var] > ENZ.ADC_Peak)
				{
					ENZ.ADC_Peak = ENZ.PULSE[var];                    //Peak Detection
				}
				ADCSampleCnt++;                                       //Num of valid PULSE Samples
			}

			ENZ.PULSE_Width = ADCSampleCnt*ENZ_PULSE_SAMPLE_TIME_s;  //ENZ Pulse Duration
			ADC_Cmplt = false;
			EVENT = SCAN;                                            //SCAN for the 2nd PULSE to get ENZ.PULSE_T
		}
		break;

	case SECOND_PULSE:
		if(ENZ.PULSE_CNT==2)
		{
			ENZ.V_Peak = ENZ.ADC_Peak*ADC_TO_VOLTAGE_SCALE;          //ENZ Peak Voltage (V)
			ENZ.I_Peak = ENZ.V_Peak / ENZ_LOAD_RES;                  //ENZ Peak Current (A)
			for (int var = 0; var < ADCSampleCnt; ++var)
			{
				ENZ.Energy_J +=((float)ENZ.PULSE[var]*ADC_TO_VOLTAGE_SCALE *  (float)ENZ.PULSE[var]*ADC_TO_VOLTAGE_SCALE)*ENZ_ENERGY_K;
			}

			ENZ.PULSE_Width = ADCSampleCnt*ENZ_PULSE_SAMPLE_TIME_s;
			ADCSampleCnt = 0;
			ENZ.PULSE_CNT = 0;
			ADC1_Stop();                                             //Stop ADC1 and TIMx b4 DATA_PROCESSING
			TIMx_Stop(TIM14);
			EVENT = DATA_PROCESSING;
		}
		break;

	case DATA_PROCESSING:
		if((ENZ.PULSE_T > ENZ_PULSE_PERIOD_THR ) && (ENZ.PULSE_Width < ENZ_PULSE_WIDTH_THR) && (ENZ.Energy_J < ENZ_PULSE_ENERGY_THR ))
		{
			ENZ_PASSED();
		}
		else
		{
			ENZ_FAILED();
		}
		/*
		 * RESET ENZ STRUCT
		 */
		ENZ.V_Peak = 0;
		ENZ.I_Peak = 0;
		ENZ.Energy_J = 0;
		ENZ.ADC_Peak = 0;
		ENZ.PULSE_CNT = 0;
		ENZ.PULSE_Width = 0;
		ENZ.PULSE_T = 0;
		ADC1_Start();
		EVENT = SCAN;
		break;
	}
}

