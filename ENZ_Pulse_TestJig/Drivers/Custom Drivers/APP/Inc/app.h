
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
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           DEFINES                                                        */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                          DATA PROCESSING                                                 */
/*																						 				    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */

typedef struct{
	uint16_t PULSE[400];
	uint16_t V_Peak;
	uint16_t I_Peak;
	uint16_t ADC_Peak;
	uint8_t  Buff_Num;
	uint8_t PULSE_CNT;
	bool PulseActive;
}Energizer;


typedef enum {
	SCAN = 0,
    FIRST_PULSE ,
    SECOND_PULSE
} ENZ_TST_JIG ;




extern volatile Energizer ENZ;                       // Declare ENZ

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           HIGH-LEVEL FUNCTIONS                                           */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */

void ENZ_PULSE_EVENTS(void);
void ENZ_PULSE_Detector(void);
void ENZ_PULSE_DataProc(void);

void ENZ_PULSE_Interval(void);

#endif /* CUSTOM_DRIVERS_APP_INC_APP_H_ */
