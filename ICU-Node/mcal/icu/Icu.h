/*****************************************************************************************
 *
 * Module: Icu
 * File Name: Icu.h
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

#ifndef ICU_H_
#define ICU_H_

#include "../../library/Platform_Types.h"
#include "../../library/Std_Types.h"
#include "../../library/common_macros.h"
#include "Icu_cfg.h"
#include "Icu_Externals.h"
#include "Icu_Private.h"
#include "../../service/det/Det.h"


#define ICU_VENDOR_ID    (1000U)

/* Dio Module Id */
#define ICU_MODULE_ID    (122U)

/* Dio Instance Id */
#define ICU_INSTANCE_ID  (1U)

/*
 * Module Version 1.0.0
 */
#define ICU_SW_MAJOR_VERSION           (1U)
#define ICU_SW_MINOR_VERSION           (0U)
#define ICU_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define ICU_AR_RELEASE_MAJOR_VERSION   (4U)
#define ICU_AR_RELEASE_MINOR_VERSION   (0U)
#define ICU_AR_RELEASE_PATCH_VERSION   (3U)

/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/

/*API IS called with invalid pointer.*/
#define ICU_E_PARAM_POINTER               (uint8) 0x0A

/*API service used with an invalid channel identifier or channel was not configured for the functionality of the calling API.*/
#define ICU_E_PARAM_CHANNEL               (uint8) 0x0B

/*API service used with an invalid or not feasible activation.*/
#define ICU_E_PARAM_ACTIVATION            (uint8) 0x0C

/*Init function failed.*/
#define ICU_E_INIT_FAILED                 (uint8) 0x0D

/*API service used with an invalid buffer size.*/
#define ICU_E_PARAM_BUFFER_SIZE           (uint8) 0x0E

/*API service Icu_SetMode used with an invalid mode.*/
#define ICU_E_PARAM_MODE                  (uint8) 0x0F

/*API service used without module initialization.*/
#define ICU_E_UNINIT                      (uint8) 0x14

/*API service Icu_SetMode is called while a running operation.*/
#define ICU_E_BUSY_OPERATION              (uint8) 0x16

/*API Icu_Init service is called and when the ICU driver and the Hardware are already initialized.*/
#define ICU_E_ALREADY_INITIALIZED         (uint8) 0x17

/*API Icu_StartTimeStamp is called and the parameter NotifyInterval is invalid (e.g."0", NotifyInterval < 1)*/
#define ICU_E_PARAM_NOTIFY_INTERVAL       (uint8) 0x18

/*API Icu_GetVersionInfo is called and the parameter versioninfo is is invalid ( e.g. NULL )*/
#define ICU_E_PARAM_VINFO                 (uint8) 0x19


/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/

/*Service ID for Icu Init*/
#define Icu_Init_SID                      (uint8) 0x00

/*Service ID for Icu DeInit*/
#define Icu_DeInit_SID                    (uint8) 0x01

/*Service ID for Icu_SetMode*/
#define Icu_SetMode_SID                   (uint8) 0x02

/*Service ID for Icu_DisableWakeup*/
#define Icu_DisableWakeup_SID             (uint8) 0x03

/*Service ID for Icu_EnableWakeup*/
#define Icu_EnableWakeup_SID              (uint8) 0x04

/*Service ID for Icu_CheckWakeup*/
#define Icu_CheckWakeup_SID               (uint8) 0x15

/*Service ID for Icu_SetActivationCondition*/
#define Icu_SetActivationCondition_SID    (uint8) 0x05

/*Service ID for Icu_DisableNotification*/
#define Icu_DisableNotification_SID       (uint8) 0x06

/*Service ID for Icu_EnableNotification*/
#define Icu_EnableNotification_SID        (uint8) 0x07

/*Service ID for Icu_GetInputState*/
#define Icu_GetInputState_SID             (uint8) 0x08

/*Service ID for Icu_StartTimestamp*/
#define Icu_StartTimestamp_SID            (uint8) 0x09

/*Service ID for Icu_StopTimestamp*/
#define Icu_StopTimestamp_SID             (uint8) 0x0a

/*Service ID for Icu_GetTimestampIndex*/
#define Icu_GetTimestampIndex_SID         (uint8) 0x0b

/*Service ID for Icu_ResetEdgeCount*/
#define Icu_ResetEdgeCount_SID            (uint8) 0x0c

/*Service ID for Icu_EnableEdgeCount*/
#define Icu_EnableEdgeCount_SID           (uint8) 0x0d

/*Service ID for Icu_EnableEdgeDetection*/
#define Icu_EnableEdgeDetection_SID       (uint8) 0x16

/*Service ID for Icu_DisableEdgeDetection*/
#define Icu_DisableEdgeDetection_SID      (uint8) 0x17

/*Service ID for Icu_DisableEdgeCount*/
#define Icu_DisableEdgeCount_SID          (uint8) 0x0e

/*Service ID for Icu_GetEdgeNumbers*/
#define Icu_GetEdgeNumbers_SID            (uint8) 0x0f

/*Service ID for Icu_StartSignalMeasurement*/
#define Icu_StartSignalMeasurement_SID    (uint8) 0x13

/*Service ID for Icu_StopSignalMeasurement*/
#define Icu_StopSignalMeasurement_SID     (uint8) 0x14

/*Service ID for Icu_GetTimeElapsed*/
#define Icu_GetTimeElapsed_SID            (uint8) 0x10

/*Service ID for Icu_GetDutyCycleValues*/
#define Icu_GetDutyCycleValues_SID        (uint8) 0x11

/*Service ID for Icu_GetVersionInfo*/
#define Icu_GetVersionInfo_SID            (uint8) 0x12

/*Service ID for Icu_DisableNotificationAsync*/
#define Icu_DisableNotificationAsync_SID  (uint8) 0x18

/*Service ID for Icu_EnableNotificationAsync*/
#define Icu_EnableNotificationAsync_SID   (uint8) 0x19


/* Numeric identifier of an ICU channel. */
typedef uint32 Icu_ChannelType;

/* Width of the buffer for timestamp ticks and measured elapsed timeticks. */
typedef uint32 Icu_ValueType;

/* Type, to abstract the return value of the service Icu_GetTimestampIndex().Since 
circular buffer handling is supported and Icu_GetTimestampIndex can return '0' as a 
legally true value ( not as an error according to ICU107 and ICU135), Icu_IndexType 
may be implemented to have values 1..xyz. */
typedef uint32 Icu_IndexType;

/* Type, to abstract the return value of the service Icu_GetEdgeNumbers(). */
typedef uint32 Icu_EdgeNumberType;

/* Allow enabling / disabling of all interrupts which are not required for the ECU wakeup. */
typedef enum{
    ICU_MODE_NORMAL, ICU_MODE_SLEEP
}Icu_ModeType;

/* Input state of an ICU channel. */
typedef enum{
    ICU_ACTIVE, ICU_IDLE
}Icu_InputStateType;

/*  Definition of the type of activation of an ICU channel. */
typedef enum{
    ICU_RISING_EDGE, ICU_FALLING_EDGE, ICU_BOTH_EDGES
}Icu_ActivationType;

/* Definition of the measurement mode type */
typedef enum{
    ICU_MODE_SIGNAL_EDGE_DETECT, ICU_MODE_SIGNAL_MEASUREMENT, ICU_MODE_TIMESTAMP, ICU_MODE_EDGE_COUNTER
}Icu_MeasurementModeType;

/* Definition of the measurement property type */
typedef enum{
   ICU_LOW_TIME, ICU_HIGH_TIME, ICU_PERIOD_TIME, ICU_DUTY_CYCLE
}Icu_SignalMeasurementPropertyType;

/* Definition of the timestamp measurement property type */
typedef enum{
   ICU_LINEAR_BUFFER, ICU_CIRCULAR_BUFFER
}Icu_TimestampBufferType;

typedef struct{
    /*
    * Description: Type which shall contain the values, needed for calculating duty cycles.
    */
   Icu_ValueType ActiveTime; /* This shall be the coherent active-time measured on a channel */
   Icu_ValueType PeriodTime; /* This shall be the coherent period-time measured on a channel */
}Icu_DutyCycleType;

/*  This type contains initialization data. */
typedef struct{
    /*
    * Description: Hardware and implementation dependent structure. The contents of the initialization data structure are microcontroller specific.
    */
   Icu_ChannelType Icu_Channel;                     /* select timer_id ex: timer_0 */
   Icu_ActivationType Default_Start_Edge;           /* select the default edge to be detected */
   Icu_MeasurementModeType MeasurementMode;         /* select measurement mode */
   Icu_SignalMeasurementPropertyType SignalMeasurementProperty;    /* select Signal Measurement Property Type */
   Icu_TimestampBufferType BufferType;              /* select buffer type */
}Icu_ConfigChannel;

/* Data Structure required for initializing the ICU Driver */
typedef struct{
	Icu_ConfigChannel Channels[ICU_CONFIGURED_CHANNLES];
}Icu_ConfigType;

/* 
* Synchronous
* Non_Reentrant
* Description: This function initializes the driver.
*/
void Icu_Init (const Icu_ConfigType* ConfigPtr);

/* 
* Synchronous
* Non_Reentrant
* Description: This function de-initializes the ICU module.
*/
#if IcuDeInitApi == STD_ON
void Icu_DeInit (void);
#endif

/* 
* Synchronous
* Non_Reentrant
* Description: This function sets the ICU mode.
*/
#if IcuSetModeApi == STD_ON
void Icu_SetMode (Icu_ModeType Mode);
#endif

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function disables the wakeup capability of a single ICU channel.
*/
#if IcuDisableWakeupApi == STD_ON
void Icu_DisableWakeup (Icu_ChannelType Channel);
#endif

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function (re-)enables the wakeup capability of the given ICU channel.
*/
#if IcuEnableWakeupApi == STD_ON
void Icu_EnableWakeup (Icu_ChannelType Channel);
#endif

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: Checks if a wakeup capable ICU channel is the source for a wakeup event and calls 
the ECU state manager service EcuM_SetWakeupEvent in case of a valid ICU 
channel wakeup event.
*/
//void Icu_CheckWakeup (EcuM_WakeupSourceType WakeupSource);

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function sets the activation-edge for the given channel.
*/
void Icu_SetActivationCondition (Icu_ChannelType Channel,Icu_ActivationType Activation);

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function disables the notification of a channel.
*/
void Icu_DisableNotification (Icu_ChannelType Channel);

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function enables the notification on the given channel.
*/
void Icu_EnableNotification (Icu_ChannelType Channel);

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function returns the status of the ICU input.
*/
#if IcuGetInputStateApi == STD_ON
Icu_InputStateType Icu_GetInputState (Icu_ChannelType Channel);
#endif

/* 
* Asynchronous
* Reentrant (limited according to ICU050)
* Description: This function starts the capturing of timer values on the edges.
*/
#if ICU_TIMESTAMP_API == STD_ON
void Icu_StartTimestamp (Icu_ChannelType Channel,Icu_ValueType* BufferPtr,uint16 BufferSize,uint16 NotifyInterval);

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function stops the timestamp measurement of the given channel.
*/
void Icu_StopTimestamp (Icu_ChannelType Channel);

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function reads the timestamp index of the given channel.
*/
Icu_IndexType Icu_GetTimestampIndex (Icu_ChannelType Channel);
#endif

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function resets the value of the counted edges to zero.
*/
#if ICU_EDGE_COUNT_API == STD_ON
void Icu_ResetEdgeCount (Icu_ChannelType Channel);

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function enables the counting of edges of the given channel.
*/
void Icu_EnableEdgeCount (Icu_ChannelType Channel);
#endif

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function enables / re-enables the detection of edges of the given channel.
*/
#if IcuEdgeDetectApi == STD_ON
void Icu_EnableEdgeDetection (Icu_ChannelType Channel);

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function disables the detection of edges of the given channel.
*/
void Icu_DisableEdgeDetection (Icu_ChannelType Channel);
#endif

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function disables the counting of edges of the given channel.
*/
#if IcuEdgeCountApi == STD_ON
void Icu_DisableEdgeCount (Icu_ChannelType Channel);
#endif

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function reads the number of counted edges.
*/
#if ICU_EDGE_COUNT_API == STD_ON
Icu_EdgeNumberType Icu_GetEdgeNumbers (Icu_ChannelType Channel);
#endif

/* 
* Asynchronous
* Reentrant (limited according to ICU050)
* Description: This function starts the measurement of signals.
*/
#if IcuSignalMeasurementApi == STD_ON
void Icu_StartSignalMeasurement (Icu_ChannelType Channel);

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function stops the measurement of signals of the given channel.
*/
void Icu_StopSignalMeasurement (Icu_ChannelType Channel);
#endif

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function reads the elapsed Signal Low Time for the given channel.
*/
#if IcuGetTimeElapsedApi == STD_ON
Icu_ValueType Icu_GetTimeElapsed (Icu_ChannelType Channel);
#endif

/* 
* Synchronous
* Reentrant (limited according to ICU050)
* Description: This function reads the coherent active time and period time for the given ICU Channel.
*/
#if IcuGetDutyCycleValuesApi == STD_ON
void Icu_GetDutyCycleValues (Icu_ChannelType Channel,Icu_DutyCycleType* DutyCycleValues);
#endif

/* 
* Synchronous
* Reentrant
* Description: This function returns the version information of this module.
*/
#if (ICU_VERSION_INFO_API == STD_ON)
void Icu_GetVersionInfo (Std_VersionInfoType* versioninfo); 
#endif
/* 
* Asynchronous
* Reentrant (limited according to ICU050)
* Description: This function disables the notification of a channel.
*/
void Icu_DisableNotificationAsync (Icu_ChannelType Channel);

/* 
* Asynchronous
* Reentrant (limited according to ICU050)
* Description: This function enables the notification on the given channel.
*/
void Icu_EnableNotificationAsync (Icu_ChannelType Channel);

/* Extern PB structures to be used by ICU and other modules */
extern const Icu_ConfigType Icu_Config;

#endif /* ICU_H_ */