/******************************************************************************
 *
 * Module: Led
 *
 * File Name: Led.c
 *
 * Description: Source file for Led Module.
 *
 * Author:
 ******************************************************************************/
#include "../../mcal/dio/Dio.h"
#include "Led.h"

void LED_setOn(void)
{
    Dio_WriteChannel(DioConf_LED1_CHANNEL_ID_INDEX,LED_ON);  /* LED ON */
}

/*********************************************************************************************/
void LED_setOff(void)
{
    Dio_WriteChannel(DioConf_LED1_CHANNEL_ID_INDEX,LED_OFF); /* LED OFF */
}

/*********************************************************************************************/
void LED_refreshOutput(void)
{
    Dio_LevelType state = Dio_ReadChannel(DioConf_LED1_CHANNEL_ID_INDEX);
    Dio_WriteChannel(DioConf_LED1_CHANNEL_ID_INDEX,state); /* re-write the same value */
}

/*********************************************************************************************/
void LED_toggle(void)
{
    Dio_LevelType state = Dio_FlipChannel(DioConf_LED1_CHANNEL_ID_INDEX);
}

void LED_G_turnOn(void)
{
    Dio_WriteChannel(DioConf_GREEN_LED_INDEX,LED_ON);  /* LED ON */
}

void LED_B_turnOn(void)
{
    Dio_WriteChannel(DioConf_BLUE_LED_INDEX,LED_ON);  /* LED ON */
}

void LED_R_turnOn(void)
{
    Dio_WriteChannel(DioConf_RED_LED_INDEX,LED_ON);  /* LED ON */
}

void LED_turnOff(void)
{
    Dio_WriteChannel(DioConf_GREEN_LED_INDEX,LED_OFF); /* LED OFF */
    Dio_WriteChannel(DioConf_BLUE_LED_INDEX,LED_OFF); /* LED OFF */
    Dio_WriteChannel(DioConf_RED_LED_INDEX,LED_OFF); /* LED OFF */
}

/*********************************************************************************************/
