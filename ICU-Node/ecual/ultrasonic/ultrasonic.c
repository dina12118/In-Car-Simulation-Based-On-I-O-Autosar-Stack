/*****************************************************************************************
 *
 * Module: Icu
 * File Name: ultrasonic.c
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

#include "../../mcal/port/port.h"
#include "ultrasonic.h"
#include "../../mcal/icu/Icu.h"
#include "../../mcal/dio/Dio.h"
#include "../../mcal/adc/Adc.h"

Icu_DutyCycleType DutyCycleValues;
Icu_ValueType BufferPtr[3];
Icu_ValueType BufferPtr2[3];

extern uint32 g_timeHigh;
extern uint32 g_distance1_cm;
extern uint32 g_distance2_cm;

extern uint32 ICU_VALUE;

void ULTRASONIC_DelayUS(uint32 us){
    uint32 clocks = (uint32)((us * 0.000001) / 0.0000000625);
    for (uint32 i = 0; i < clocks; i++)
    {
        asm("NOP");
    }   
}

void ULTRASONIC_Init(void){
    Port_Init(&Port_PinConfig);
    Dio_Init(&Dio_Configuration);
	
    Icu_Init(&Icu_Config);
    Icu_StartTimestamp(0, BufferPtr, 2, 2);
    Icu_StartTimestamp(4, BufferPtr2, 2, 2);
    Icu_EnableWakeup(0);
    Icu_EnableWakeup(4);
}

void ULTRASONIC_Trigger(uint8 id){
    switch (id)
    {
    case 1:
        Dio_WriteChannel(TRIGGER1_ID, STD_HIGH);
        /* delay 10 us*/
        ULTRASONIC_DelayUS(18000);
        Dio_WriteChannel(TRIGGER1_ID, STD_LOW);
        break;
    
    case 2:
        Dio_WriteChannel(TRIGGER2_ID, STD_HIGH);
        /* delay 10 us*/
        ULTRASONIC_DelayUS(18000);
        Dio_WriteChannel(TRIGGER2_ID, STD_LOW);
        break;
    }
}

void ULTRASONIC_ReadDistance(uint32 *distance, uint8 id){
    ULTRASONIC_Trigger(id);
    switch (id)
    {
    case 1:
        *distance = g_distance1_cm;
        break;
    
    case 2:
        *distance = g_distance2_cm;
        break;
    } 
}