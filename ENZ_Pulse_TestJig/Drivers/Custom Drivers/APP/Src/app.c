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
#include "usart.h"
#include "stdio.h"
#include "helpers.h"
#include "string.h"
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

volatile EnergizerThresholds THR = {                          //Defaut to Testing the Wizards
		.ENZ_PULSE_PERIOD = WIZARDx_PULSE_PERIOD_THR,
		.ENZ_PULSE_WIDTH = WIZARDx_PULSE_WIDTH_THR ,
		.ENZ_PULSE_ENERGY_MIN = WIZARD4_PULSE_ENERGY_MIN_THR,
		.ENZ_PULSE_ENERGY_MAX = WIZARD4_PULSE_ENERGY_MAX_THR
};

ENZ_TST_JIG EVENT = SCAN;

static uint16_t ADCSampleCnt;

volatile uint8_t RxBuffer[13];

volatile int64_t  DruidSerialNumber;
volatile bool SerNumRcvd;
volatile bool ST_to_Pi_Tx_Msg_Cmplt;
volatile bool ST_to_Druid_Prgmng_Cmplt;

char ST_to_Pi_TxMsg[100];
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

void ENZ_PULSE_EVENTS(void)
{
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
		ST_to_Pi_Tx_Msg_Cmplt = false;
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
		if((ENZ.PULSE_T > THR.ENZ_PULSE_PERIOD ) && (ENZ.PULSE_Width < THR.ENZ_PULSE_WIDTH) && (ENZ.Energy_J > THR.ENZ_PULSE_ENERGY_MIN ) && (ENZ.Energy_J < THR.ENZ_PULSE_ENERGY_MAX))
		{
			ENZ_PASSED();
			if(PI_ON)
			{
				GPIO_Writepin(GPIOA, LED_0, GPIO_PIN_RESET);
				char ST_to_Pi_TxMsg[100];
				char SerNum[13];

				while(!ST_to_Pi_Tx_Msg_Cmplt)                         //Block Until Serial Num is Rcvd and when Tx is Cmplt
				{
					if(SerNumRcvd)
					{
						SerialNumConvToStr(DruidSerialNumber, SerNum);
						snprintf(ST_to_Pi_TxMsg, sizeof(ST_to_Pi_TxMsg),
								"%s,%u,%u,%lu,%lu,%lu,%s\n",
								SerNum,
								ENZ.V_Peak,
								ENZ.I_Peak,
								(uint32_t)(ENZ.Energy_J * 100),
								(uint32_t)(ENZ.PULSE_Width * 1000000),
								ENZ.PULSE_T, "PASS");

						UART_Transmit_DMA(USART2, (uint8_t *)ST_to_Pi_TxMsg, strlen(ST_to_Pi_TxMsg));
						UART_Receive_DMA(USART2,  RxBuffer, 13);                                     //Start Expecting another Num
						DruidSerialNumber = 0;
						SerNumRcvd = false;
					}

				}
				GPIO_Writepin(GPIOA, LED_0, GPIO_PIN_SET);                 //Turn Off STTUS LED After Successfluu receiving the SreiualNum
			}
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

/* ────────────────────────────────────────────────────────────── /
 * Function : SerialNumConvToStr()
 * Purpose  : Converts int64 to str
 * Details  : Changes the DRUIDx serial numbers to string
 * Runtime  : ~X.Xxx
 * ────────────────────────────────────────────────────────────── */
void ENZ_TestJig_CONFIG(void)
{
	if(memcmp((const void*)Rx_Pi_Status, "W002",4) == 0)                    //https://www.w3schools.com/c/ref_string_memcmp.php
	{                                                                       //Rx_Pi_Status won't change when memcmp is running
		THR.ENZ_PULSE_PERIOD = DRUIDx_PULSE_PERIOD_THR;
		THR.ENZ_PULSE_WIDTH = DRUIDx_PULSE_WIDTH_THR ;
		THR.ENZ_PULSE_ENERGY_MIN = WIZARD2_PULSE_ENERGY_MIN_THR;
		THR.ENZ_PULSE_ENERGY_MAX = WIZARD2_PULSE_ENERGY_MAX_THR;
	}
	if(memcmp((const void*)Rx_Pi_Status, "W004",4) == 0)
	{
		THR.ENZ_PULSE_PERIOD = DRUIDx_PULSE_PERIOD_THR;
		THR.ENZ_PULSE_WIDTH = DRUIDx_PULSE_WIDTH_THR ;
		THR.ENZ_PULSE_ENERGY_MIN = WIZARD4_PULSE_ENERGY_MIN_THR;
		THR.ENZ_PULSE_ENERGY_MAX = WIZARD4_PULSE_ENERGY_MAX_THR;
	}

	if(memcmp((const void*)Rx_Pi_Status, "D012",4) == 0)                      //DRUIDx
	{
		THR.ENZ_PULSE_PERIOD = DRUIDx_PULSE_PERIOD_THR;
		THR.ENZ_PULSE_WIDTH = DRUIDx_PULSE_WIDTH_THR ;
		THR.ENZ_PULSE_ENERGY_MIN = DRUID12_PULSE_ENERGY_MIN_THR;
		THR.ENZ_PULSE_ENERGY_MAX = DRUID12_PULSE_ENERGY_MAX_THR;
		SWITCH_PI_OFF();
	}
	if(memcmp((const void*)Rx_Pi_Status, "D015",4) == 0)
	{
		THR.ENZ_PULSE_PERIOD = DRUIDx_PULSE_PERIOD_THR;
		THR.ENZ_PULSE_WIDTH = DRUIDx_PULSE_WIDTH_THR ;
		THR.ENZ_PULSE_ENERGY_MIN = DRUID15_PULSE_ENERGY_MIN_THR;
		THR.ENZ_PULSE_ENERGY_MAX = DRUID15_PULSE_ENERGY_MAX_THR;
		SWITCH_PI_OFF();
	}
	if(memcmp((const void*)Rx_Pi_Status, "D018",4) == 0)
	{
		THR.ENZ_PULSE_PERIOD = DRUIDx_PULSE_PERIOD_THR;
		THR.ENZ_PULSE_WIDTH = DRUIDx_PULSE_WIDTH_THR ;
		THR.ENZ_PULSE_ENERGY_MIN = DRUID18_PULSE_ENERGY_MIN_THR;
		THR.ENZ_PULSE_ENERGY_MAX = DRUID18_PULSE_ENERGY_MAX_THR;
		SWITCH_PI_OFF();
	}
	if(memcmp((const void*)Rx_Pi_Status, "D025",4) == 0)
	{
		THR.ENZ_PULSE_PERIOD = DRUIDx_PULSE_PERIOD_THR;
		THR.ENZ_PULSE_WIDTH = DRUIDx_PULSE_WIDTH_THR ;
		THR.ENZ_PULSE_ENERGY_MIN = DRUID25_PULSE_ENERGY_MIN_THR;
		THR.ENZ_PULSE_ENERGY_MAX = DRUID25_PULSE_ENERGY_MAX_THR;
		SWITCH_PI_OFF();
	}
	if(memcmp((const void*)Rx_Pi_Status, "D028",4) == 0)
	{
		THR.ENZ_PULSE_PERIOD = DRUIDx_PULSE_PERIOD_THR;
		THR.ENZ_PULSE_WIDTH = DRUIDx_PULSE_WIDTH_THR ;
		THR.ENZ_PULSE_ENERGY_MIN = DRUID28_PULSE_ENERGY_MIN_THR;
		THR.ENZ_PULSE_ENERGY_MAX = DRUID28_PULSE_ENERGY_MAX_THR;
	}
	if(memcmp((const void*)Rx_Pi_Status, "D114",4) == 0)
	{
		THR.ENZ_PULSE_PERIOD = DRUIDx_PULSE_PERIOD_THR;
		THR.ENZ_PULSE_WIDTH = DRUIDx_PULSE_WIDTH_THR ;
		THR.ENZ_PULSE_ENERGY_MIN = DRUID114_PULSE_ENERGY_MIN_THR;
		THR.ENZ_PULSE_ENERGY_MAX = DRUID114_PULSE_ENERGY_MAX_THR;
		SWITCH_PI_OFF();
	}

	if(memcmp((const void*)Rx_Pi_Status, "M002",4) == 0)                       //MERLINx
	{
		THR.ENZ_PULSE_PERIOD = DRUIDx_PULSE_PERIOD_THR;
		THR.ENZ_PULSE_WIDTH = DRUIDx_PULSE_WIDTH_THR ;
		THR.ENZ_PULSE_ENERGY_MIN = MERLIN2_PULSE_ENERGY_MIN_THR;
		THR.ENZ_PULSE_ENERGY_MAX = MERLIN2_PULSE_ENERGY_MAX_THR;
		SWITCH_PI_OFF();
	}
	if(memcmp((const void*)Rx_Pi_Status, "M004",4) == 0)
	{
		THR.ENZ_PULSE_PERIOD = DRUIDx_PULSE_PERIOD_THR;
		THR.ENZ_PULSE_WIDTH = DRUIDx_PULSE_WIDTH_THR ;
		THR.ENZ_PULSE_ENERGY_MIN = MERLIN4_PULSE_ENERGY_MIN_THR;
		THR.ENZ_PULSE_ENERGY_MAX = MERLIN4_PULSE_ENERGY_MAX_THR;
		SWITCH_PI_OFF();
	}
	if(memcmp((const void*)Rx_Pi_Status, "MS18",4) == 0)
	{
		THR.ENZ_PULSE_PERIOD = DRUIDx_PULSE_PERIOD_THR;
		THR.ENZ_PULSE_WIDTH = DRUIDx_PULSE_WIDTH_THR ;
		THR.ENZ_PULSE_ENERGY_MIN = MERLIN_STEALTH18_PULSE_ENERGY_MIN_THR;
		THR.ENZ_PULSE_ENERGY_MAX = MERLIN_STEALTH18_PULSE_ENERGY_MAX_THR;
		SWITCH_PI_OFF();
	}
	if(memcmp((const void*)Rx_Pi_Status, "MS28",4) == 0)
	{
		THR.ENZ_PULSE_PERIOD = DRUIDx_PULSE_PERIOD_THR;
		THR.ENZ_PULSE_WIDTH = DRUIDx_PULSE_WIDTH_THR ;
		THR.ENZ_PULSE_ENERGY_MIN = MERLIN_STEALTH28_PULSE_ENERGY_MIN_THR;
		THR.ENZ_PULSE_ENERGY_MAX = MERLIN_STEALTH28_PULSE_ENERGY_MAX_THR;
		SWITCH_PI_OFF();
	}
}

/* ────────────────────────────────────────────────────────────── /
 * Function : SerialNumConvToStr()
 * Purpose  : Converts int64 to str
 * Details  : Changes the DRUIDx serial numbers to string
 * Runtime  : ~X.Xxx
 * ────────────────────────────────────────────────────────────── */
void SerialNumConvToStr(int64_t N, char *str)
{
	int i = 0;
	int64_t sign = N;

	if (N < 0)
		N = -N;

	if (N == 0) {
		str[i++] = '0';
	}

	while (N > 0) {
		str[i++] = (N % 10) + '0';
		N /= 10;
	}

	if (sign < 0)
		str[i++] = '-';

	str[i] = '\0';

	for (int j = 0, k = i - 1; j < k; j++, k--) {
		char tmp = str[j];
		str[j] = str[k];
		str[k] = tmp;
	}
}
