
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
 *                                                                                                          *
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
#include "stm32g030xx.h"
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           DEFINES                                                        */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
#define ENZ_PULSE_SAMPLE_TIME_s              0.78125e-6f      //Total Time per sample:(12.5 + 12.5)(1/f_adc )
#define ENZ_LOAD_RES                         501              //501 Ohm
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                          DATA PROCESSING                                                 */
/*																						 				    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */

typedef struct{
	uint16_t PULSE[400];
	uint16_t V_Peak;
	uint16_t I_Peak;
	uint16_t Energy_J;
	uint16_t ADC_Peak;
	uint8_t  Buff_Num;
	uint8_t PULSE_CNT;
	double PULSE_Width;
	uint32_t PULSE_T;
	bool PulseActive;
	bool Buff_Full;
}Energizer;


typedef enum {
	SCAN = 0,
    FIRST_PULSE ,
    SECOND_PULSE
} ENZ_TST_JIG ;



extern volatile Energizer ENZ;                       // Declare ENZ


extern bool One_Sec_Elapsed;
extern uint8_t Sec;
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           HIGH-LEVEL FUNCTIONS                                           */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */

void ENZ_PULSE_EVENTS(void);
void ENZ_PULSE_Detector(void);
void ENZ_PULSE_DataProc(void);

void ENZ_PULSE_Interval(void);

__STATIC_INLINE void ENZ_PULSE_Counting(void) {
	NVIC_EnableIRQ(ADC1_IRQn);                                      //Enabling IRQ************************
}

__STATIC_INLINE void ENZ_PULSE_CNT(void) {
	ENZ.PULSE_CNT++;
	NVIC_DisableIRQ(ADC1_IRQn);                                     //Enabling IRQ************************
}

__STATIC_INLINE void ENZ_ReadPulse_T(void) {

	ENZ.PULSE_T = ((uint32_t)(TIM14->CNT)*(TIM14->PSC +1)/64);
//	return ENZ.PULSE_T;
}


//uint32_t ENZ_ReadPulse_T(void);
#endif /* CUSTOM_DRIVERS_APP_INC_APP_H_ */
