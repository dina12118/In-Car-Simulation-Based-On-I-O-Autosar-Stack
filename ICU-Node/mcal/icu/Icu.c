/*****************************************************************************************
 *
 * Module: Icu
 * File Name: Icu.c
 * Description: Source file for TM4C123GH6PM Microcontroller - ICU Driver
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

#include "Icu.h"

uint8 NVIC_TIMER_INTERRUPTS_IDs[24] = {19,20,21,22,23,24,35,36,70,71,92,93,94,95,96,97,98,99,100,101,102,103,104,105};

/* struct for timestamps data*/
typedef struct{
   Icu_ValueType* buffer;
   Icu_IndexType index;
   uint16 buffer_size;
}Icu_TimestampData;

/* Initializing timestamps data */
Icu_TimestampData Icu_Timestampdata[24]; /* 24 channels */

uint8 Signal_Noification_Status = 0, Timestamp_Notification_Status = 0;

uint32 g_timeHigh = 0, g_distance1_cm = 0, g_distance2_cm = 0;

/* 
* Synchronous
* Non_Reentrant
* Description: This function initializes the ICU driver.
*/
void Icu_Init (const Icu_ConfigType* ConfigPtr){
    for(uint8 i = 0; i < ICU_CONFIGURED_CHANNLES; i++){
        uint8 timer_id = ConfigPtr->Channels[i].Icu_Channel / 2;
        uint8 timer_channel = ConfigPtr->Channels[i].Icu_Channel % 2;
        /* enable Timer Run Mode Clock Gating Control for 16/32 bits or 32/64 bits */
        SET_BIT(RCGCTIMER,(timer_id > 5 ? timer_id - 6 : timer_id));
        volatile uint32 delay = 0;
        /* Stop Timer */
        TIMER(timer_id, GPTMCTL_OFFSET) = 0;
        /* Choosing the individual timer registers */
        TIMER(timer_id, GPTMCFG_OFFSET) = 4;
        
        switch (timer_channel)
        {
        case TIMER_A:
            /* Enable capture mode in Timer A */
            SET_BIT(delay, TAMR_0);
            SET_BIT(delay, TAMR_1);
            SET_BIT(delay, TACDIR);
            TIMER(timer_id, GPTMTAMR_OFFSET) = delay;

            /* Start timer */
            delay = 0;
            SET_BIT(delay, TAEN);
            TIMER(timer_id,GPTMCTL_OFFSET) = delay;
            break;
        
        case TIMER_B:
            delay = 0;
            /* Enable capture mode in Timer B */
            SET_BIT(delay, TBMR_0);
            SET_BIT(delay, TBMR_1);
            SET_BIT(delay, TBCDIR);
            TIMER(timer_id, GPTMTBMR_OFFSET) = delay;
            /* Start timer */
            delay = 0;
            SET_BIT(delay, TBEN);
            TIMER(timer_id, GPTMCTL_OFFSET) = delay;
            break;
        }
        Icu_DisableNotification(ConfigPtr->Channels[i].Icu_Channel);
        Icu_DisableWakeup(ConfigPtr->Channels[i].Icu_Channel);

        switch (ConfigPtr->Channels[i].MeasurementMode)
        {
            case ICU_MODE_EDGE_COUNTER:
            Icu_EnableEdgeCount(ConfigPtr->Channels[i].Icu_Channel);
            break;

            case ICU_MODE_SIGNAL_EDGE_DETECT:
            Icu_EnableEdgeDetection(ConfigPtr->Channels[i].Icu_Channel);
            break;
        }

        Icu_SetActivationCondition(ConfigPtr->Channels[i].Icu_Channel, ConfigPtr->Channels[i].Default_Start_Edge);
    }
}

/* 
* Synchronous
* Non_Reentrant
* Description: This function de-initializes the ICU module.
*/
#if IcuDeInitApi == STD_ON
void Icu_DeInit (void){
    for(uint8 i = 0; i < 12; i++){
        /* Disable Capture Mode IN ALL TIMERS A */
        CLEAR_BIT(TIMER(i,GPTMTAMR_OFFSET), TAMR_0);
        CLEAR_BIT(TIMER(i,GPTMTAMR_OFFSET), TAMR_1);   
        /* Disable Capture Mode IN ALL TIMERS B */
        CLEAR_BIT(TIMER(i,GPTMTBMR_OFFSET), TBMR_0);
        CLEAR_BIT(TIMER(i,GPTMTBMR_OFFSET), TBMR_1); 
    }
}
#endif

/* 
* Synchronous
* Non_Reentrant
* Description: This function sets the ICU mode.
*/
#if IcuSetModeApi == STD_ON
void Icu_SetMode (Icu_ModeType Mode){
    switch(Mode)
	{
		case ICU_MODE_NORMAL:
		SET_BIT(SCGC1,16);
		SET_BIT(SCGC1,17);
		SET_BIT(SCGC1,18);
		SET_BIT(SCGC1,19);
		break;
		
		case ICU_MODE_SLEEP:
		CLEAR_BIT(SCGC1,16);
		CLEAR_BIT(SCGC1,17);
		CLEAR_BIT(SCGC1,18);
		CLEAR_BIT(SCGC1,19);
		break;
		
	}
}
#endif

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function disables the wakeup capability of a single ICU channel.
*/
#if IcuDisableWakeupApi == STD_ON
void Icu_DisableWakeup (Icu_ChannelType Channel){
    uint8 timer_id = Channel / 2;
    uint8 timer_channel = Channel % 2;

    /* Disable Wide GPTM Write Update Error Interrupt */
    CLEAR_BIT(TIMER(timer_id, GPTMIMR_OFFSET), WUEIM);
    /* Disable Interrupt in NVIC */
    NVIC_OP(NVIC_TIMER_INTERRUPTS_IDs[Channel], NVIC_DIS0_OFFSET);
    /* Determine channel */
    switch (timer_channel)
    {
    case TIMER_A: /* Disable interrupt for Timer A */
        CLEAR_BIT(TIMER(timer_id, GPTMIMR_OFFSET), TAMIM);
        /* Disable GPTM Timer A Capture Mode Event Interrupt Mask */
        CLEAR_BIT(TIMER(timer_id, GPTMIMR_OFFSET), CAEIM);
        /* Disable GPTM Timer A Capture Mode Match Interrupt Mask */
        CLEAR_BIT(TIMER(timer_id, GPTMIMR_OFFSET), CAMIM);
        break;

    case TIMER_B: /* Disable interrupt for Timer B */
        CLEAR_BIT(TIMER(timer_id, GPTMIMR_OFFSET), TBMIM);
        /* Disable GPTM Timer B Capture Mode Event Interrupt Mask */
        CLEAR_BIT(TIMER(timer_id, GPTMIMR_OFFSET), CBEIM);
        /* Disable GPTM Timer B Capture Mode Match Interrupt Mask */
        CLEAR_BIT(TIMER(timer_id, GPTMIMR_OFFSET), CBMIM);
        break;
    }
}
#endif

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function (re-)enables the wakeup capability of the given ICU channel.
*/
#if IcuEnableWakeupApi == STD_ON
void Icu_EnableWakeup (Icu_ChannelType Channel){
    uint8 timer_id = Channel / 2;
    uint8 timer_channel = Channel % 2;

    /* Enable Wide GPTM Write Update Error Interrupt */
    SET_BIT(TIMER(timer_id, GPTMIMR_OFFSET), WUEIM);
    /* Enable Interrupt in NVIC */
    NVIC_OP(NVIC_TIMER_INTERRUPTS_IDs[Channel], NVIC_EN0_OFFSET);
    /* Determine channel */
    switch (timer_channel)
    {
    case TIMER_A: /* Enable interrupt for Timer A */
        SET_BIT(TIMER(timer_id, GPTMIMR_OFFSET), TAMIM);
        /* Enable GPTM Timer A Capture Mode Event Interrupt Mask */
        SET_BIT(TIMER(timer_id, GPTMIMR_OFFSET), CAEIM);
        /* Enable GPTM Timer A Capture Mode Match Interrupt Mask */
        //SET_BIT(TIMER(timer_id, GPTMIMR_OFFSET), CAMIM);
        break;

    case TIMER_B: /* Enable interrupt for Timer B */
        SET_BIT(TIMER(timer_id, GPTMIMR_OFFSET), TBMIM);
        /* Enable GPTM Timer B Capture Mode Event Interrupt Mask */
        SET_BIT(TIMER(timer_id, GPTMIMR_OFFSET), CBEIM);
        /* Enable GPTM Timer B Capture Mode Match Interrupt Mask */
        //SET_BIT(TIMER(timer_id, GPTMIMR_OFFSET), CBMIM);
        break;
    }
}
#endif

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: Checks if a wakeup capable ICU channel is the source for a wakeup event and calls 
the ECU state manager service EcuM_SetWakeupEvent in case of a valid ICU 
channel wakeup event.
*/
//void Icu_CheckWakeup (EcuM_WakeupSourceType WakeupSource){}

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function sets the activation-edge for the given channel.
*/
void Icu_SetActivationCondition (Icu_ChannelType Channel,Icu_ActivationType Activation){
    uint8 timer_id = Channel / 2;
	uint8 timer_channel = Channel % 2;
	
	switch(Activation)
	{
		case ICU_RISING_EDGE:
		switch (timer_channel)
		{
			case TIMER_A:
            CLEAR_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TAEN);
			CLEAR_BIT (TIMER(timer_id, GPTMCTL_OFFSET),TAEVENT_0);
			CLEAR_BIT (TIMER(timer_id, GPTMCTL_OFFSET),TAEVENT_1);
            SET_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TAEN);
			break;
			
			case TIMER_B:
            CLEAR_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TBEN);
			CLEAR_BIT (TIMER(timer_id, GPTMCTL_OFFSET),TBEVENT_0);
			CLEAR_BIT (TIMER(timer_id, GPTMCTL_OFFSET),TBEVENT_1);
            SET_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TBEN);
			break;
		}
		break;
		
		case ICU_FALLING_EDGE :
		switch (timer_channel)
		{
			case TIMER_A:
            CLEAR_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TAEN);
			SET_BIT (TIMER(timer_id, GPTMCTL_OFFSET),TAEVENT_0);
			CLEAR_BIT (TIMER(timer_id, GPTMCTL_OFFSET),TAEVENT_1);
            SET_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TAEN);
			break;
			
			case TIMER_B:
            CLEAR_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TBEN);
			SET_BIT (TIMER(timer_id, GPTMCTL_OFFSET),TBEVENT_0);
			CLEAR_BIT (TIMER(timer_id, GPTMCTL_OFFSET),TBEVENT_1);
            SET_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TBEN);
			break;
		}
		break;
		
		case ICU_BOTH_EDGES :
		switch (timer_channel)
		{
			case TIMER_A:
            CLEAR_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TAEN);
			SET_BIT (TIMER(timer_id, GPTMCTL_OFFSET),TAEVENT_0);
			SET_BIT (TIMER(timer_id, GPTMCTL_OFFSET),TAEVENT_1);
            SET_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TAEN);
			break;
			
			case TIMER_B:
            CLEAR_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TBEN);
			SET_BIT (TIMER(timer_id, GPTMCTL_OFFSET),TBEVENT_0);
			SET_BIT (TIMER(timer_id, GPTMCTL_OFFSET),TBEVENT_1);
            SET_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TBEN);
			break;
		}
		break;
		
		default:
		switch (timer_channel)
		{
			case TIMER_A:
            CLEAR_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TAEN);
			CLEAR_BIT (TIMER(timer_id, GPTMCTL_OFFSET),TAEVENT_0);
			SET_BIT (TIMER(timer_id, GPTMCTL_OFFSET),TAEVENT_1);
            SET_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TAEN);
			break;
			
			case TIMER_B:
            CLEAR_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TBEN);
			CLEAR_BIT (TIMER(timer_id, GPTMCTL_OFFSET),TBEVENT_0);
			SET_BIT (TIMER(timer_id, GPTMCTL_OFFSET),TBEVENT_1);
            SET_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TBEN);
			break;
		}
	}
}

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function disables the notification of a channel.
*/
void Icu_DisableNotification (Icu_ChannelType Channel){
    Signal_Noification_Status = 0;
    Timestamp_Notification_Status = 0;
}

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function enables the notification on the given channel.
*/
void Icu_EnableNotification (Icu_ChannelType Channel){
    Signal_Noification_Status = 1;
    Timestamp_Notification_Status = 1;
}

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function returns the status of the ICU input.
*/
#if IcuGetInputStateApi == STD_ON
Icu_InputStateType Icu_GetInputState (Icu_ChannelType Channel){
    uint8 timer_id = Channel / 2;
    uint8 timer_channel = Channel % 2;
    
    Icu_InputStateType input_state;
    switch(timer_channel){
            case TIMER_A:
            if (GET_BIT(TIMER(timer_id, GPTMRIS_OFFSET),CAERIS) == 0)
            {
                    input_state = ICU_IDLE ;
            }
            else if (GET_BIT(TIMER(timer_id, GPTMRIS_OFFSET),CAERIS) == 1)
            {
                    SET_BIT(TIMER(timer_id, GPTMICR_OFFSET),CAECINT);
                    input_state = ICU_ACTIVE ;
            }
            break;

            case TIMER_B:
            if (GET_BIT(TIMER(timer_id, GPTMRIS_OFFSET),CBERIS ) == 0)
            {
                    input_state = ICU_IDLE ;
            }
            else if (GET_BIT(TIMER(timer_id, GPTMRIS_OFFSET),CBERIS ) == 1)
            {
                    SET_BIT(TIMER(timer_id, GPTMICR_OFFSET),CBECINT);
                    input_state = ICU_ACTIVE ;
            }
            break;
    }
    return input_state;
}
#endif

/* 
* Asynchronous
* Reentrant (limited according to ICU050)
* Description: This function starts the capturing of timer values on the edges.
*/
#if ICU_TIMESTAMP_API == STD_ON
void Icu_StartTimestamp (Icu_ChannelType Channel,Icu_ValueType* BufferPtr,uint16 BufferSize,uint16 NotifyInterval){
    uint8 timer_id = Channel / 2;
    uint8 timer_channel = Channel % 2;
    /* Initializing timestamps data */
    Icu_Timestampdata[Channel].buffer = BufferPtr;
    Icu_Timestampdata[Channel].index = 0;
    Icu_Timestampdata[Channel].buffer_size = BufferSize;

    switch(timer_channel){
        case TIMER_A:
            CLEAR_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TAEN);
            /* Enable Edge Time Mode FOR TIMER A */
            /* COUNT UP */
            SET_BIT(TIMER(timer_id, GPTMTAMR_OFFSET), TACDIR);
            /* ENABLE CAPTURE OR COMPARE MODE */
            CLEAR_BIT(TIMER(timer_id, GPTMTAMR_OFFSET), TAAMS);
            /* Enable Edge Time Mode */
            SET_BIT(TIMER(timer_id, GPTMTAMR_OFFSET), TACMR);
            /* ENABLE CAPTURE MODE */
            SET_BIT(TIMER(timer_id, GPTMTAMR_OFFSET), TAMR_0);
            SET_BIT(TIMER(timer_id, GPTMTAMR_OFFSET), TAMR_1);

            SET_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TAEN);
            break;

        case TIMER_B:
            CLEAR_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TBEN);
            /* Enable Edge Time Mode FOR TIMER B */
            /* COUNT UP */
            SET_BIT(TIMER(timer_id, GPTMTBMR_OFFSET), TBCDIR);
            /* ENABLE CAPTURE OR COMPARE MODE */
            CLEAR_BIT(TIMER(timer_id, GPTMTBMR_OFFSET), TBAMS);
            /* Enable Edge Time Mode */
            SET_BIT(TIMER(timer_id, GPTMTBMR_OFFSET), TBCMR);
            /* ENABLE CAPTURE MODE */
            SET_BIT(TIMER(timer_id, GPTMTBMR_OFFSET), TBMR_0);
            SET_BIT(TIMER(timer_id, GPTMTBMR_OFFSET), TBMR_1);

            SET_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TBEN);
            break;
    }
}
#endif

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function stops the timestamp measurement of the given channel.
*/
#if ICU_TIMESTAMP_API == STD_ON
void Icu_StopTimestamp (Icu_ChannelType Channel){
    uint8 timer_id = Channel / 2;
    uint8 timer_channel = Channel % 2;

    switch(timer_channel){
        case TIMER_A:
        /* DISABLE CAPTURE MODE */
        CLEAR_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TAEN);
        CLEAR_BIT(TIMER(timer_id, GPTMTAMR_OFFSET), TAMR_0);
        CLEAR_BIT(TIMER(timer_id, GPTMTAMR_OFFSET), TAMR_1);
        SET_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TAEN);
        break;

        case TIMER_B:
        /* DISABLE CAPTURE MODE */
        CLEAR_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TBEN);
        CLEAR_BIT(TIMER(timer_id, GPTMTBMR_OFFSET), TBMR_0);
        CLEAR_BIT(TIMER(timer_id, GPTMTBMR_OFFSET), TBMR_1);
        SET_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TBEN);
        break;
    }
}
#endif

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function reads the timestamp index of the given channel.
*/
#if ICU_TIMESTAMP_API == STD_ON
Icu_IndexType Icu_GetTimestampIndex (Icu_ChannelType Channel){
    return Icu_Timestampdata[Channel].index;
}
#endif

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function resets the value of the counted edges to zero.
*/
#if ICU_EDGE_COUNT_API == STD_ON
void Icu_ResetEdgeCount (Icu_ChannelType Channel){
    uint8 timer_id = Channel / 2;
	uint8 timer_channel = Channel % 2;
	uint32 TAMR_VALUE = TIMER(timer_id, GPTMTAMR_OFFSET);
    uint32 TBMR_VALUE = TIMER(timer_id, GPTMTBMR_OFFSET);
    uint32 CTL_VALUE = TIMER(timer_id, GPTMCTL_OFFSET);
    uint32 value = 0;

	switch(timer_channel)
	{
		case TIMER_A:
        /* CHANGE MODE TO BE PERIODIC WITH SNAPSHOT */
        SET_BIT(value, TASNAPS);
        SET_BIT(value, TAMR_1);
        CLEAR_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TAEN);
        TIMER(timer_id, GPTMTAMR_OFFSET) = value;
        SET_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TAEN);

        TIMER(timer_id, GPTMTAV_OFFSET) = 0;

        /* Enable capture mode in Timer A */
        CLEAR_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TAEN);
        TIMER(timer_id, GPTMTAMR_OFFSET) = TAMR_VALUE;

        /* Start timer */
        TIMER(timer_id,GPTMCTL_OFFSET) = CTL_VALUE;

        break;
		
		case TIMER_B:
        /* CHANGE MODE TO BE PERIODIC WITH SNAPSHOT */
        SET_BIT(value, TBSNAPS);
        SET_BIT(value, TBMR_1);
        CLEAR_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TBEN);
        TIMER(timer_id, GPTMTBMR_OFFSET) = value;
        SET_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TBEN);

        TIMER(timer_id, GPTMTBV_OFFSET) = 0;

        /* Enable capture mode in Timer B */
        CLEAR_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TBEN);
        TIMER(timer_id, GPTMTBMR_OFFSET) = TBMR_VALUE;

        /* Start timer */
        TIMER(timer_id,GPTMCTL_OFFSET) = CTL_VALUE;
        break;
	}
}

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function enables the counting of edges of the given channel.
*/
void Icu_EnableEdgeCount (Icu_ChannelType Channel){
    uint8 timer_id = Channel / 2;
	uint8 timer_channel = Channel % 2;
	
	switch(timer_channel)
	{
		case TIMER_A:
        CLEAR_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TAEN);
		SET_BIT(TIMER(timer_id, GPTMTAMR_OFFSET),TAMR_1);
		SET_BIT(TIMER(timer_id, GPTMTAMR_OFFSET),TAMR_0);
		CLEAR_BIT(TIMER(timer_id, GPTMTAMR_OFFSET),TACMR);
        SET_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TAEN);
		break;
		
		case TIMER_B:
        CLEAR_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TBEN);
		SET_BIT(TIMER(timer_id, GPTMTBMR_OFFSET),TBMR_1);
		SET_BIT(TIMER(timer_id, GPTMTBMR_OFFSET),TBMR_0);
		CLEAR_BIT(TIMER(timer_id, GPTMTBMR_OFFSET),TBCMR);
        SET_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TBEN);
		break;
	}
}
#endif

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function enables / re-enables the detection of edges of the given channel.
*/
#if IcuEdgeDetectApi == STD_ON
void Icu_EnableEdgeDetection (Icu_ChannelType Channel){
    uint8 timer_id = Channel / 2;
	uint8 timer_channel = Channel % 2;
	
	switch(timer_channel)
	{
		case TIMER_A:
		SET_BIT(TIMER(timer_id, GPTMIMR_OFFSET),CAEIM);
		break;
		
		case TIMER_B:
		SET_BIT(TIMER(timer_id, GPTMIMR_OFFSET),CBEIM);
		break;
	}
}

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function disables the detection of edges of the given channel.
*/
void Icu_DisableEdgeDetection (Icu_ChannelType Channel){
    uint8 timer_id = Channel / 2;
	uint8 timer_channel = Channel % 2;
	
	switch(timer_channel)
	{
		case TIMER_A:
		CLEAR_BIT(TIMER(timer_id, GPTMIMR_OFFSET),CAEIM);
		break;
		
		case TIMER_B:
		CLEAR_BIT(TIMER(timer_id, GPTMIMR_OFFSET),CBEIM);
		break;
	}
}
#endif

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function disables the counting of edges of the given channel.
*/
#if IcuEdgeCountApi == STD_ON
void Icu_DisableEdgeCount (Icu_ChannelType Channel){
    uint8 timer_id = Channel / 2;
	uint8 timer_channel = Channel % 2;
	
	switch(timer_channel)
	{
		case TIMER_A:
        CLEAR_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TAEN);
		CLEAR_BIT(TIMER(timer_id, GPTMTAMR_OFFSET),TAMR_1);
		CLEAR_BIT(TIMER(timer_id, GPTMTAMR_OFFSET),TAMR_0);
        SET_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TAEN);
		break;
		
		case TIMER_B:
        CLEAR_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TBEN);
		CLEAR_BIT(TIMER(timer_id, GPTMTBMR_OFFSET),TBMR_1);
		CLEAR_BIT(TIMER(timer_id, GPTMTBMR_OFFSET),TBMR_0);
		SET_BIT(TIMER(timer_id,GPTMCTL_OFFSET), TBEN);
        break;
	}
}
#endif

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function reads the number of counted edges.
*/
#if ICU_EDGE_COUNT_API == STD_ON
Icu_EdgeNumberType Icu_GetEdgeNumbers (Icu_ChannelType Channel){
    Icu_EdgeNumberType READ;
	uint8 timer_id = Channel / 2;
	uint8 timer_channel = Channel % 2;
	
	switch(timer_channel)
	{
		case TIMER_A:
		READ = TIMER(timer_id, GPTMTAR_OFFSET);
		break;
		
		case TIMER_B:
		READ = TIMER(timer_id, GPTMTBR_OFFSET);
	}

    return READ;
}
#endif

/* 
* Asynchronous
* Reentrant (limited according to ICU050)
* Description: This function starts the measurement of signals.
*/
#if IcuSignalMeasurementApi == STD_ON
void Icu_StartSignalMeasurement (Icu_ChannelType Channel){
    Icu_SetActivationCondition(Channel, ICU_RISING_EDGE);
    Icu_DisableEdgeCount(Channel);
}

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function stops the measurement of signals of the given channel.
*/
void Icu_StopSignalMeasurement (Icu_ChannelType Channel){
    /* Set Activation Mode to be None */
    Icu_SetActivationCondition(Channel, 5);
    Icu_StopTimestamp(Channel);
}
#endif

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function reads the elapsed Signal Low Time for the given channel.
*/
#if IcuGetTimeElapsedApi == STD_ON
Icu_ValueType Icu_GetTimeElapsed (Icu_ChannelType Channel){
    Icu_ValueType Time_Elapsed = 0, val1, val2;
    uint8 timer_id = Channel / 2;
    val1 = TIMER(timer_id, GPTMTAV_OFFSET);
    val2 = TIMER(timer_id, GPTMTAR_OFFSET);
    Time_Elapsed = val1 - val2;
    return Time_Elapsed;
}
#endif

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function reads the coherent active time and period time for the given ICU Channel.
*/
#if IcuGetDutyCycleValuesApi == STD_ON
void Icu_GetDutyCycleValues (Icu_ChannelType Channel,Icu_DutyCycleType* DutyCycleValues){
    DutyCycleValues->ActiveTime = (Icu_Timestampdata[Channel].buffer[1] - Icu_Timestampdata[Channel].buffer[0]);
    if(Icu_Timestampdata[Channel].buffer[2]){
        DutyCycleValues->PeriodTime = (Icu_Timestampdata[Channel].buffer[2] - Icu_Timestampdata[Channel].buffer[0]);
    }
}
#endif
/* 
* Synchronous
* Reentrant
* Description: This function returns the version information of this module.
*/
#if (ICU_VERSION_INFO_API == STD_ON)
void Icu_GetVersionInfo (Std_VersionInfoType* versioninfo){
#if (ICU_DEV_ERROR_DETECT == STD_ON)
    /* Check if input pointer is not Null pointer */
    if(NULL_PTR == versioninfo)
    {
        /* Report to DET  */
        Det_ReportError(ICU_MODULE_ID, ICU_INSTANCE_ID,
        Icu_GetVersionInfo_SID, ICU_E_PARAM_POINTER);
    }
    else
#endif /* (DIO_DEV_ERROR_DETECT == STD_ON) */
    {
        /* Copy the vendor Id */
        versioninfo->vendorID = (uint16)ICU_VENDOR_ID;
        /* Copy the module Id */
        versioninfo->moduleID = (uint16)ICU_MODULE_ID;
        /* Copy Software Major Version */
        versioninfo->sw_major_version = (uint8) ICU_SW_MAJOR_VERSION;
        /* Copy Software Minor Version */
        versioninfo->sw_minor_version = (uint8) ICU_SW_MINOR_VERSION;
        /* Copy Software Patch Version */
        versioninfo->sw_patch_version = (uint8) ICU_SW_PATCH_VERSION;
    }
}
#endif

/* 
* Synchronous
* Non Reentrant
* Description: According to the last call of Icu_EnableNotification, this notification function to be called if the requested signal edge (rising / falling / both edges) occurs (once per edge).
*/
void Icu_SignalNotification (void){
    // sprintf("An Edge Has Been Detected!!!\n");
}

/* 
* Synchronous
* Non Reentrant
* Description: This notification to be called if the number of requested timestamps (Notification interval > 0) are acquired and if the notification has been enabled by the call of Icu_EnableNotification().
*/
#if IcuTimestampApi == STD_ON
void Icu_TimestampNotification (void){
    // sprintf("A Timestamp Has Been Captured!!!\n");
}
#endif

// extern Icu_DutyCycleType DutyCycleValues;

// void TIMER0_EdgeTime_Handler(void){
//     uint8 channel = 0, timer_id = 0;
//     Icu_IndexType idx = Icu_Timestampdata[channel].index;
//     if (idx >= Icu_Timestampdata[channel].buffer_size){
//         if(Icu_Config.Channels[channel].BufferType == ICU_LINEAR_BUFFER){
//             Icu_StopTimestamp(channel);
//             Icu_GetDutyCycleValues(channel, &DutyCycleValues);
//             /* Clear Flag */
//             SET_BIT(TIMER(timer_id,GPTMICR_OFFSET), CAECINT);
//             return;
//         }
//         else{
//             Icu_GetDutyCycleValues(channel, &DutyCycleValues);
//             idx = 1;
//             Icu_Timestampdata[channel].buffer[0] = Icu_Timestampdata[channel].buffer[Icu_Timestampdata[channel].index - 1];
//             Icu_Timestampdata[channel].index = 1;
//             return;
//         }
//     }
    
//     Icu_Timestampdata[channel].buffer[idx] = TIMER(timer_id,GPTMTAR_OFFSET);
//     Icu_Timestampdata[channel].index++;
//     if (idx % 2 == 0)
//     {
//         Icu_SetActivationCondition(channel,ICU_FALLING_EDGE);
//     }
//     else{
//         Icu_SetActivationCondition(channel,ICU_RISING_EDGE);
//     }
//     /* Clear Flag */
//     SET_BIT(TIMER(timer_id,GPTMICR_OFFSET), CAECINT);
// }

void TIMER0A_Handler(void){
    uint8 channel = 0, timer_id = 0;
    Icu_IndexType idx = Icu_Timestampdata[channel].index;
    Icu_Timestampdata[channel].buffer[idx] = TIMER(timer_id,GPTMTAR_OFFSET);
    Icu_Timestampdata[channel].index++;
    if (idx % 2 == 0)
    {
        Icu_SetActivationCondition(channel,ICU_FALLING_EDGE);
    }
    else{
        Icu_Timestampdata[channel].index = 0;
        g_timeHigh = Icu_Timestampdata[channel].buffer[1] - Icu_Timestampdata[channel].buffer[0];
		g_distance1_cm = (g_timeHigh * 0.0000000625f) * 17000;
        Icu_SetActivationCondition(channel,ICU_RISING_EDGE);
    }
	
    /* Clear Flag */
    SET_BIT(TIMER(0,GPTMICR_OFFSET), CAECINT);
}

void TIMER2A_Handler(void){
    uint8 channel = 4, timer_id = 2;
    Icu_IndexType idx = Icu_Timestampdata[channel].index;
    Icu_Timestampdata[channel].buffer[idx] = TIMER(timer_id,GPTMTAR_OFFSET);
    Icu_Timestampdata[channel].index++;
    if (idx % 2 == 0)
    {
        Icu_SetActivationCondition(channel,ICU_FALLING_EDGE);
    }
    else{
        Icu_Timestampdata[channel].index = 0;
        g_timeHigh = Icu_Timestampdata[channel].buffer[1] - Icu_Timestampdata[channel].buffer[0];
		g_distance2_cm = (g_timeHigh * 0.0000000625f) * 17000;
        Icu_SetActivationCondition(channel,ICU_RISING_EDGE);
    }

    /* Clear Flag */
    SET_BIT(TIMER(timer_id,GPTMICR_OFFSET), CAECINT);
}