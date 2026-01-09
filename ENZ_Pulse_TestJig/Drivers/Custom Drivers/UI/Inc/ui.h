/*
 * ui.h
 *
 *  Created on: Dec 26, 2025
 *      Author: Lovejoy
 */

/* ******************* ──────────────────────────────────────────────────────────────── ******************* */
/*                                                                                                          *
 *              ╔═════════════════════════════════════════════════════════════════════════╗                 *
 *               ║                            FILE: ui.h                                 ║                  *
 *               ║                            AUTHOR: Lovejoy Mhishi                     ║                  *
 *               ║                            DATE: December 26, 2025                    ║                  *
 *              ╠═════════════════════════════════════════════════════════════════════════╣                 *
 *               ║      User Interface drive controlling LEDs and the Buzzer             ║                  *
 *              ╚═════════════════════════════════════════════════════════════════════════╝                 *
 * This source file contains function definations for the functions used to control the LEDs and Buzzer     *
 *                                                                                                          *
 * Contents:                                                                                                *
 *   - Function Prototypes                                                                                  *
 *   - Buzzer ON and OFF                                                                                    *
 *                                                                                                          *
 * Intended Use:                                                                                            *
 *   -  Include ui.h in application files (e.g., main.c, app.c) to access UI control APIs.                  *
 *                                                                                                          *
 * Dependencies:                                                                                            *                                              *
 *                                                                                                          *
 *                                                                                                          */
/* ******************* ──────────────────────────────────────────────────────────────── ******************* */

#ifndef CUSTOM_DRIVERS_UI_INC_UI_H_
#define CUSTOM_DRIVERS_UI_INC_UI_H_

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           HIGH-LEVEL FUNCTIONS                                           */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
void ENZ_PASSED(void);
void ENZ_FAILED(void);
void DEBUG_ENZ(void);
#endif /* CUSTOM_DRIVERS_UI_INC_UI_H_ */
