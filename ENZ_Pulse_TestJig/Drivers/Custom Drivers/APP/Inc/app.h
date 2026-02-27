
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */
/*                                                                                                          *
 *              ╔═════════════════════════════════════════════════════════════════════════╗                 *
 *               ║                            FILE: app.h                                ║                  *
 *               ║                            AUTHOR: Lovejoy Mhishi                     ║                  *
 *               ║                            DATE: January 7, 2026                      ║                  *
 *              ╠═════════════════════════════════════════════════════════════════════════╣                 *
 *               ║                Main Application Logic for ENZ_PULSE_TestJig           ║                  *
 *              ╚═════════════════════════════════════════════════════════════════════════╝                 *
 * This header file provides function prototypes, type definitions, and external variables.                 *
 *                                                                                                          *
 * Contents:                                                                                                *
 *   - Function Prototypes                                                                                  *
 *   - Variables                                                                                            *
 *                                                                                                          *
 * Intended Use:                                                                                            *
 *   -  Include app.h in application files (e.g., main.c, app.c) to access app control logic.               *
 *                                                                                                          *
 * Dependencies:                                                                                            *                                              *
 *   -stm32g030xx.h & adc.h                                                                                 *
 *                                                                                                          */
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */
#ifndef CUSTOM_DRIVERS_APP_INC_APP_H_
#define CUSTOM_DRIVERS_APP_INC_APP_H_

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/* 																											*/
/*                                           INCLUDES                                                       */
/*                                                                                  						*/
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
#include "adc.h"
#include "gpio.h"
#include "ui.h"
#include "stm32g030xx.h"
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           DEFINES                                                        */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
#define ENZ_PULSE_SAMPLE_TIME_s              0.78125e-6f      //Total Time per sample:(12.5 + 12.5)(1/f_adc )
#define ENZ_MAX_PULSE_LENGHT                 500              //500×0.78125μs = 390.625μs Pulse Length
#define ENZ_LOAD_RES                         501              //501 Ohm
#define VOLTAGE_DIVIDER_GAIN                 3015
#define ENZ_ENERGY_K                        (ENZ_PULSE_SAMPLE_TIME_s / ENZ_LOAD_RES)
#define ADC_TO_VOLTAGE_SCALE                ((V_REF_plus*VOLTAGE_DIVIDER_GAIN) / ADC_MAX )


#define WIZARDx_PULSE_PERIOD_THR             1000000           //1,000,000 µs: ENZ_PULSE_FREQ = 1Hz
#define WIZARDx_PULSE_WIDTH_THR              0.000390625       //390.625μs: max(Wizord 2/4, Merlin 2/4 & Druid 25/28)
#define WIZRADx_PULSE_ENERGY_THR             5

#define DRUIDx_PULSE_PERIOD_THR              1000000           //1,000,000 µs: ENZ_PULSE_FREQ = 1Hz
#define DRUIDx_PULSE_WIDTH_THR               0.000390625       //390.625μs: max(Wizord 2/4, Merlin 2/4 & Druid 25/28)
#define DRUIDx_PULSE_ENERGY_THR              8

#define MERLINx_PULSE_PERIOD_THR             1000000           //1,000,000 µs: ENZ_PULSE_FREQ = 1Hz
#define MERLINx_PULSE_WIDTH_THR              0.000390625       //390.625μs: max(Wizord 2/4, Merlin 2/4 & Druid 25/28)
#define MERLINx_PULSE_ENERGY_THR             10



#define WIZARD2_PULSE_ENERGY_MIN_THR         1.65
#define WIZARD2_PULSE_ENERGY_MAX_THR         2.35

#define WIZARD4_PULSE_ENERGY_MIN_THR         3.60
#define WIZARD4_PULSE_ENERGY_MAX_THR         4.30

#define DRUID12_PULSE_ENERGY_MIN_THR         1.70
#define DRUID12_PULSE_ENERGY_MAX_THR         2.20

#define DRUID13_PULSE_ENERGY_MIN_THR         2.70
#define DRUID13_PULSE_ENERGY_MAX_THR         3.30

#define DRUID15_PULSE_ENERGY_MIN_THR         4.40
#define DRUID15_PULSE_ENERGY_MAX_THR         5.30

#define DRUID18_PULSE_ENERGY_MIN_THR         7.2
#define DRUID18_PULSE_ENERGY_MAX_THR         5.8

#define DRUID25_PULSE_ENERGY_MIN_THR         2
#define DRUID25_PULSE_ENERGY_MAX_THR         2.8

#define DRUID28_PULSE_ENERGY_MIN_THR         3.4
#define DRUID28_PULSE_ENERGY_MAX_THR         4.1

#define DRUID114_PULSE_ENERGY_MIN_THR        5.4
#define DRUID114_PULSE_ENERGY_MAX_THR        6.6

#define MERLIN2_PULSE_ENERGY_MIN_THR         1.7
#define MERLIN2_PULSE_ENERGY_MAX_THR         2.3

#define MERLIN4_PULSE_ENERGY_MIN_THR         3.6
#define MERLIN4_PULSE_ENERGY_MAX_THR         4.4

#define MERLIN_STEALTH18_PULSE_ENERGY_MIN_THR 7.2
#define MERLIN_STEALTH18_PULSE_ENERGY_MAX_THR 8.8

#define MERLIN_STEALTH28_PULSE_ENERGY_MIN_THR 7.2
#define MERLIN_STEALTH28_PULSE_ENERGY_MAX_THR 8.8
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                          DATA PROCESSING                                                 */
/*																						 				    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */

typedef struct{
	uint16_t PULSE[ENZ_MAX_PULSE_LENGHT];
	uint16_t V_Peak;
	uint16_t I_Peak;
	double Energy_J;
	uint16_t ADC_Peak;
	uint8_t PULSE_CNT;
	double PULSE_Width;
	uint32_t PULSE_T;
	bool PulseActive;
}Energizer;

typedef struct{
	uint32_t ENZ_PULSE_PERIOD;
	double ENZ_PULSE_WIDTH;
	double ENZ_PULSE_ENERGY_MIN;
	double ENZ_PULSE_ENERGY_MAX;
}EnergizerThresholds;

typedef enum {
	SCAN = 0,
	FIRST_PULSE ,
	SECOND_PULSE,
	DATA_PROCESSING
}ENZ_TST_JIG;


extern volatile Energizer ENZ;
extern volatile EnergizerThresholds THR;
extern ENZ_TST_JIG EVENT;


extern volatile uint8_t RxBuffer[13];
extern volatile int64_t  DruidSerialNumber;
extern volatile bool SerNumRcvd;
extern volatile bool ST_to_Pi_Tx_Msg_Cmplt;
extern volatile bool ST_to_Druid_Prgmng_Cmplt;
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           HIGH-LEVEL FUNCTIONS                                           */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */

void ENZ_PULSE_EVENTS(void);

void ENZ_TestJig_CONFIG(void);

__STATIC_INLINE void ENZ_PULSE_Counting(void)
{
	NVIC_EnableIRQ(ADC1_IRQn);                                       //Enable ADC1 IRQ************************
}

__STATIC_INLINE void ENZ_PULSE_CNT(void)
{
	ENZ.PULSE_CNT++;
	NVIC_DisableIRQ(ADC1_IRQn);                                      //Disable ADC1 IRQ***********************
}

__STATIC_INLINE void SWITCH_PI_ON(void)
{
	GPIO_Writepin(GPIOA, Plus_5V_EN , GPIO_PIN_SET);                 //SWITCH the Pi On for JIG Config
}

__STATIC_INLINE void SWITCH_PI_OFF(void)
{
	GPIO_Writepin(GPIOA, Plus_5V_EN , GPIO_PIN_RESET);               //SWITCH the Pi On OFF
	PI_ON = false;
}
void SerialNumConvToStr(int64_t N, char *str);

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           LOW-LEVEL FUNCTIONS                                            */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
__STATIC_INLINE void ENZ_ReadPulse_T(void)
{
	ENZ.PULSE_T = ((uint32_t)(TIM14->CNT)*(TIM14->PSC +1)/64);      //Read TIM14->CNT and calculate ENZ.PULSE_T
}

#endif /* CUSTOM_DRIVERS_APP_INC_APP_H_ */
