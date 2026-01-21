
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


#include "stm32g030xx.h"

/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           DEFINES                                                        */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
#define BUTTON_SHORT_PRESS                    0x001F         //0b0000000011111111
#define BUTTON_LONG_PRESS                     0x3FFF         //0b1111111111111111

typedef struct{
	uint16_t StateHistory;
	uint8_t StateCount;
}ButtonState;

typedef enum{
	IDLE = 0,
	PRESS_DETECTED,
	SHORT_PRESS,
	LONG_PRESS
}ST_BUTTON;

extern volatile ST_BUTTON BUTTON_STATE;
extern volatile ButtonState Button;
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
/*																											*/
/*                                           HIGH-LEVEL FUNCTIONS                                           */
/*																										    */
/* ──────────────────────────────────────────────────────────────────────────────────────────────────────── */
void ScanButtonPress(void);
void CheckButtonState(void);
void ENZ_PASSED(void);
void ENZ_FAILED(void);
void DEBUG_ENZ(void);
#endif /* CUSTOM_DRIVERS_UI_INC_UI_H_ */
