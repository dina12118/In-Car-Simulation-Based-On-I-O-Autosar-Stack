/*****************************************************************************************
 *
 * Module: Icu
 * File Name: ultrasonic.h
 * Description: Header file for TM4C123GH6PM Microcontroller - ULTRASONIC Driver
 * Author:
 *****************************************************************************************/

/*****************************************************************************************
 *
 *  Project                     : Project    
 *  Platform                    : ARM
 *  Peripheral                  : TM4C123GH6PM
 *  Autosar Version             : 4.4.0
 *  Autosar Revision            : ASR_REL_4_0_REV_0000
 *  SW Version                  : 1.0.0
 *****************************************************************************************/

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include "../../library/Platform_Types.h"
#include "../../library/Std_Types.h"
#include "../../library/common_macros.h"

/* Description: Initialize the gpio pins interfaced with ultrasonic */
void ULTRASONIC_Init(void);
/* Description: Trigger ultrasonic to start the echo */
void ULTRASONIC_Trigger(uint8 id);
/* Description: Measure the distance using ultrasonic */
void ULTRASONIC_ReadDistance(uint32 *distance, uint8 id);
/* Description: delay by microseconds */
void ULTRASONIC_DelayUS(uint32 us);

#endif /* ULTRASONIC_H_ */