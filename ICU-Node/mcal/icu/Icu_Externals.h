/*****************************************************************************************
 *
 * Module: Icu
 * File Name: Icu_Externals.h
 * Description: Header file for TM4C123GH6PM Microcontroller - ICU Driver
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

#ifndef ICU_EXTERNALS_H_
#define ICU_EXTERNALS_H_

/* 
* Synchronous
* Non Reentrant
* Description: According to the last call of Icu_EnableNotification, this notification function to be called if the requested signal edge (rising / falling / both edges) occurs (once per edge).
*/
void Icu_SignalNotification (void);

/* 
* Synchronous
* Non Reentrant
* Description: This notification to be called if the number of requested timestamps (Notification interval > 0) are acquired and if the notification has been enabled by the call of Icu_EnableNotification().
*/
#if IcuTimestampApi == STD_ON
void Icu_TimestampNotification (void);
#endif

#endif /* ICU_EXTERNALS_H_ */