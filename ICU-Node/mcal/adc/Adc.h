/*****************************************************************************************
 *
 * Module: Adc.h
 * File Name: Adc.h
 * Description: Header file for TM4C123GH6PM Microcontroller - ADC Driver
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


#ifndef ADC_H
#define ADC_H

/* Id for the company in the AUTOSAR */
#define ADC_VENDOR_ID    (1000U)

/* ADC Module Id */
#define ADC_MODULE_ID    (123U)

/*
 * Module Version 1.0.0
 */
#define ADC_SW_MAJOR_VERSION           	(1U)
#define ADC_SW_MINOR_VERSION           	(0U)
#define ADC_SW_PATCH_VERSION           	(0U)

/*	
 * AUTOSAR Version 4.0.4	
 */	
#define ADC_AR_RELEASE_MAJOR_VERSION   	(4U)
#define ADC_AR_RELEASE_MINOR_VERSION   	(0U)
#define ADC_AR_RELEASE_PATCH_VERSION   	(3U)

/*	
 * Macros for Adc_GroupInitStatus	
 */	
#define ADC_INITIALIZED                	(1U)
#define ADC_NOT_INITIALIZED            	(0U)

/*	
 * Macros for Adc_BufferInitStatus	
 */	
#define BUFFER_INITIALIZED             	(1U)
#define BUFFER_NOT_INITIALIZED         	(0U)

/*
 * Macros for Adc_GroupContinousMode
 */
#define MODE_CONTINOUS         			(1U)
#define MODE_ONE_SHOT             		(0U)

/*****************************************************************************************/
/*                                                                     					 */
/*****************************************************************************************/



/*****************************************************************************************
                                        INCLUDES
 *****************************************************************************************/

/* Include Standard Types abstraction */
#include "../../library/Std_Types.h"
#include "../../library/Common_Macros.h"


/* AUTOSAR checking between Std Types and ADC Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != ADC_AR_RELEASE_MAJOR_VERSION)\
||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != ADC_AR_RELEASE_MINOR_VERSION)\
||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != ADC_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Std_Types.h does not match the expected version"
#endif

/* 
* @ADC Pre-Compile Configuration Header file 
*/
#include "Adc_Cfg.h"

/* AUTOSAR Version checking between Adc_Cfg.h and Adc.h files */

#if ((ADC_CFG_AR_RELEASE_MAJOR_VERSION != ADC_AR_RELEASE_MAJOR_VERSION)\
||  (ADC_CFG_AR_RELEASE_MINOR_VERSION != ADC_AR_RELEASE_MINOR_VERSION)\
||  (ADC_CFG_AR_RELEASE_PATCH_VERSION != ADC_AR_RELEASE_PATCH_VERSION))
#error "The AR version of ADC_Cfg.h does not match the expected version"
#endif

/* Software Version checking between Adc_Cfg.h and Adc.h files */

#if ((ADC_CFG_SW_MAJOR_VERSION != ADC_SW_MAJOR_VERSION)\
||  (ADC_CFG_SW_MINOR_VERSION != ADC_SW_MINOR_VERSION)\
||  (ADC_CFG_SW_PATCH_VERSION != ADC_SW_PATCH_VERSION))
#error "The SW version of ADC_Cfg.h does not match the expected version"
#endif


/*****************************************************************************************
                                        CONSTANTS
 *****************************************************************************************/

/* Error: Adc_Init has not been called prior to another function call */
#define ADC_E_UNINIT                			0x0A

/* Error: Adc_StartGroupConversion was called while another conversion is already running or a HW trigger
 		  is already enabled or a request is already stored in the queue 								*/
/* Error: Adc_EnableHardwareTrigger was called while a conversion is ongoing or a HW trigger is already 
		  enabled or the maximum number of HW triggers is already enabled 								*/
/* Error: Adc_DeInit was called while a conversion is still ongoing */       
#define ADC_E_BUSY                              0x0B

/* Error: Adc_StopGroupConversion was called while no conversion was running */ 
/* Error: Adc_DisableHardwareTrigger was called while group is not enabled   */
#define ADC_E_IDLE                              0x0C

/* Error: Adc_Init has been called while ADC is already initialized */
#define ADC_E_ALREADY_INITIALIZED               0x0D

/* Error: Adc_Init has been called with incorrect configuration parameter */
#define ADC_E_PARAM_POINTER                     0x14

/* Error: Invalid group ID requested */
#define ADC_E_PARAM_GROUP                       0x15

/* Error: Adc_EnableHardwareTrigger called on a group with conversion mode configured as continuous */
/* Error: Adc_DisableHardwareTrigger called on a group with conversion mode configured as continuous */
#define ADC_E_WRONG_CONV_MODE                   0x16

/* Adc_StartGroupConversion called on a group with trigger source configured as hardware */
/* Adc_StopGroupConversion called on a group with trigger source configured as hardware */
/* Adc_EnableHardwareTrigger called on a group with trigger source configured as software API */
/* Adc_DisableHardwareTrigger called on a group with trigger source configured as software API */
#define ADC_E_WRONG_TRIGG_SRC                   0x17

/* Error: Enable/disable notification function for a group whose configuration set has no notification available */
#define ADC_E_NOTIF_CAPABILITY                  0x18

/* Error: Conversion started and result buffer pointer is not initialized */
#define  ADC_E_BUFFER_UNINIT                    0x19

/* Error: One or more ADC group/channel not in IDLE state */
#define ADC_E_NOT_DISENGAGED                    0x1A

/* Error: Unsupported power state request */
#define ADC_E_POWER_STATE_NOT_SUPPORTED         0x1B 

/* Error: Requested power state can not be reached directly */
#define ADC_E_TRANSITION_NOT_POSSIBLE           0x1C

/* Error: ADC not prepared for target power state */
#define  ADC_E_PERIPHERAL_NOT_PREPARED          0x1D

/*****************************************************************************************
                                STRUCTURES AND OTHER TYPE DEFS
 *****************************************************************************************/

/* Numeric ID of an ADC channel*/
typedef uint8 		Adc_ChannelType;

/* Numeric ID of an ADC channel group */
typedef uint8 		Adc_GroupType;

/* Type for reading the converted values of a channel group */
typedef uint16 		Adc_ValueGroupType;

/*Type of clock prescaler factor*/															
typedef uint8 		Adc_PrescaleType;

/* the time during which the sampled analogue value is converted into digital representation */
typedef uint32 		Adc_ConversionTimeType;													

/* the time during which the value is sampled in clock cycles */
typedef enum{
	KS_125 = 1U , KS_250 = 3U , KS_500 = 5U, MS_1 = 7U
	}Adc_SamplingTimeType;													

/* Type of channel resolution in number of bits */
typedef uint8 		Adc_ResolutionType;														

/* Current status of the conversion of the requested ADC Channel group */
/* ADC_IDLE: The conversion has not been started*/
/* ADC_BUSY: The conversion has been started and is still going on*/
/* ADC_COMPLETED: A result is available for all channels of the group*/
/* ADC_STREAM_COMPLETED: The result buffer is completely filled*/
typedef enum{
	ADC_IDLE,				
	ADC_BUSY,				
	ADC_COMPLETED,			
	ADC_STREAM_COMPLETED	
}Adc_StatusType;

/* Type for configuring the trigger source for an ADC Channel group */
typedef enum{
	ADC_TRIGG_SRC_SW,
	ADC_TRIGG_SRC_HW	
}Adc_TriggerSourceType;

/* Type for configuring the conversion mode of an ADC Channel group */
typedef enum{
	ADC_CONV_MODE_ONESHOT,
	ADC_CONV_MODE_CONTINUOUS	
}Adc_GroupConvModeType;

/* Priority level of the channel. Highest priority is 0 */
typedef uint8	Adc_GroupPriorityType;

/* Type for configuring the access mode to group conversion results */
typedef enum{
	ADC_ACCESS_MODE_SINGLE,
	ADC_ACCESS_MODE_STREAMING
}Adc_GroupAccessModeType;

/* Type for configuring the number of group conversions in streaming access mode */
typedef uint8	Adc_StreamNumSampleType;

/* Type for configuring the streaming access mode buffer type */
/* Linear : The ADC Driver stops the conversion as soon as the stream buffer is full*/
/* Circular : The ADC Driver continues the conversion even if the stream buffer is full by wrapping around the stream buffer itself*/
typedef enum{
	ADC_STREAM_BUFFER_LINEAR,
	ADC_STREAM_BUFFER_CIRCULAR
}Adc_StreamBufferModeType;

/* Type for configuring on which edge of the hardware trigger signal the driver should react */
/* React on the rising edge of the hardware trigger signal */
/* React on the falling edge of the hardware trigger signal */
/* React on both edges of the hardware trigger signalv */
typedef enum{
	ADC_HW_TRIG_RISING_EDGE,		
	ADC_HW_TRIG_FALLING_EDGE,		
	ADC_HW_TRIG_BOTH_EDGES,			
}Adc_HwTriggerSignalType;


/* Type for the reload value of the ADC module embedded timer (only if supported by the ADC hardware) */
typedef uint32	Adc_HwTriggerTimerType;																			

/* Type for configuring the prioritization mechanism */
/* priority mechanism is not available.*/
/* Hardware priority mechanism is available only.*/
/* Hardware and software priority mechanism is available.*/
typedef enum{
	ADC_PRIORITY_NONE,		
	ADC_PRIORITY_HW,		
	ADC_PRIORITY_HW_SW,		
}Adc_PriorityImplementationType;

/*Replacement mechanism, which is used on ADC group level */  
/*if a group conversion is interrupted by a group which has a higher priority */
typedef enum{
	ADC_GROUP_REPL_ABORT_RESTART,		
	ADC_GROUP_REPL_SUSPEND_RESUME		
}Adc_GroupReplacementType;

/* In case of active limit checking: defines which conversion values are taken into account related to the
	boardes defined with AdcChannelLowLimit and AdcChannelHighLimit. */
typedef enum{
	ADC_RANGE_UNDER_LOW,				/* Range below low limit - low limit value included.*/
	ADC_RANGE_BETWEEN,					/* Range between low limit and high limit - high limit value included.*/
	ADC_RANGE_OVER_HIGH,				/* Range above high limit.*/
	ADC_RANGE_ALWAYS,					/* Complete range - independent from channel limit settings.*/
	ADC_RANGE_NOT_UNDER_LOW,			/* Range above low limit.*/
	ADC_RANGE_NOT_BETWEEN,				/* Range above high limit or below low limit - low limit value included.*/
	ADC_RANGE_NOT_OVER_HIGH,			/* Range below high limit - high limit value included.*/
}Adc_ChannelRangeSelectType;

/* Type for alignment of ADC raw results in ADC result buffer (left/right alignment)*/
typedef enum{
	ADC_ALIGN_LEFT,						/* left alignment.*/
	ADC_ALIGN_RIGHT						/* right alignmen.*/
}Adc_ResultAlignmentType;

/* Power state currently active or set as target power state */
typedef enum{
	ADC_FULL_POWER,						/* Full Power.*/
	ADC_POWER_1,						/* power modes with decreasing power consumptions.*/
	ADC_POWER_2							/* power modes with decreasing power consumptions.*/
}Adc_PowerStateType;

/* Result of the requests related to power state transitions */
typedef enum{
	ADC_SERVICE_ACCEPTED,				/* Power state change executed.*/
	ADC_NOT_INIT,						/* ADC Module not initialized.*/
	ADC_SEQUENCE_ERROR,					/* Wrong API call sequence.*/
	ADC_HW_FAILURE,						/* The HW module has a failure which prevents it to enter the required power state.*/
	ADC_POWER_STATE_NOT_SUPP,			/* ADC Module does not support the requested power state.*/
	ADC_TRANS_NOT_POSSIBLE,				/* ADC Module cannot transition directly from the current powerstate 
										   to the requested power state or the HW peripheral is still busy.*/
}Adc_PowerStateRequestResultType;

/*	Type for assignment of channels to a channel group	*/
typedef Adc_GroupType* Adc_GroupDefType;

/* Data structure containing parameters for initializing the ADC Driver and ADC HW Unit */										

typedef struct{
	Adc_GroupAccessModeType AdcGroupAccessMode;
	Adc_GroupConvModeType AdcGroupConversionMode;							// ADC_CONV_MODE_ONESHOT / ADC_CONV_MODE_CONTINUOUS													//NOT IMPLEMENTED YET
	Adc_GroupType AdcGroupId;												// ADC_GROUP_0 / .. / ADC_GROUP_3
	Adc_GroupPriorityType AdcGroupPriority;									// ADC_GroupPriority_0/../ADC_GroupPriority_3 (Highest priority is 0)
	Adc_GroupReplacementType AdcGroupReplacement;
	Adc_TriggerSourceType AdcGroupTriggSrc;									// ADC_TRIGG_SRC_SW / ADC_TRIGG_SRC_HW
	Adc_HwTriggerSignalType AdcHwTrigSignal;								// ADC_HW_TRIG_RISING_EDGE / ADC_HW_TRIG_FALLING_EDGE / ADC_HW_TRIG_BOTH_EDGES
	Adc_HwTriggerTimerType AdcHwTrigTimer;
	uint8 AdcNotification;
	Adc_StreamBufferModeType AdcStreamingBufferMode;						// ADC_STREAM_BUFFER_LINEAR / ADC_STREAM_BUFFER_CIRCULAR
	Adc_StreamNumSampleType AdcStreamingNumSamples;
	Adc_GroupDefType AdcGroupDefinition;									// pointer to array identifying channels used in group
}AdcGroup_t;

typedef struct{
	uint8	AdcClockSource;
	uint8 	AdcHwUnitId;
	Adc_PrescaleType AdcPrescale;											// 125 KSPS, 250 KSPS, 500 KSPS, 1 MSPS
	AdcGroup_t	AdcGroup[4];
}AdcHwUnit_t;

typedef struct{
	AdcHwUnit_t AdcHwUnit[2];
}AdcConfigSet_t;

typedef struct{
	boolean ADC_instance;													// ADC_0_INSTANCE_ID / ADC_1_INSTANCE_ID
}Adc_ConfigType;



/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/

/* Service ID for Initializes the ADC hardware units and driver */
#define ADC_INIT_SID                      (uint8)0x00

/* Service ID for DeInitialize the ADC hardware units */
#define ADC_DEINIT_SID                    (uint8)0x01

/* Service ID for Starts the conversion of all channels of the requested ADC Channel group */
#define ADC_START_GROUP_CONVERSION_SID        (uint8)0x02

/* Service ID for Stops the conversion of all channels of the requested ADC Channel group */
#define ADC_STOP_GROUP_CONVERSION_SID         (uint8)0x03

/* Service ID for Reads the group conversion result of the last completed conversion round */
#define ADC_READ_GROUP_SID                    (uint8)0x04

/* Service ID for Enables the hardware trigger for the requested ADC Channel group. */
#define ADC_ENABLE_HARDWARE_TRIGGER_SID   (uint8)0x05

/* Service ID for Disables the hardware trigger for the requested ADC Channel group */
#define ADC_DISABLE_HARDWARE_TRIGGER_SID   (uint8)0x06

/* Service ID for Enables the notification mechanism for the requested ADC Channel group */
#define ADC_ENABLE_GROUP_NOTIFICATION_SID   (uint8)0x07

/* Service ID for Disables the notification mechanism for the requested ADC Channel group */
#define ADC_DISABLE_GROUP_NOTIFICATION_SID   (uint8)0x08

/* Service ID for Returns the conversion status of the requested ADC Channel group */
#define ADC_GET_GROUP_STATUS_SID             (uint8)0x09

/* Service ID for Returns the version information of this module */
#define ADC__GET_VERSION_INFO_SID            (uint8)0x0A

/* Service ID for Returns the number of valid samples per channel, stored in the result buffer */
#define ADC_GET_STRAM_LAST_POINTER_SID       (uint8)0x0B

/* Service ID for Initializes ADC driver with the group specific result buffer 
start address where the conversion results will be stored. */
#define ADC_SETUP_RESULT_BUFFER_SID          (uint8)0x0C

/* Service ID for configures the Adc module so that it enters the already prepared power state */
#define ADC_SET_POWER_STATE_SID              (uint8)0x10

/* Service ID for returns the current power state of the ADC HW unit */
#define ADC_GET_CURRENT_POWER_STATE_SID      (uint8)0x11

/* Service ID for returns the Target power state of the ADC HW unit */
#define ADC_GET_TARGET_POWER_STATE_SID       (uint8)0x12

/* Service ID for starts the needed process to allow the ADC HW module to enter the requested power state */
#define ADC_PREPARE_POWER_STATE_SID          (uint8)0x13

/* Service ID for cyclically called and supervises the power state transitions, checking
for the readiness of the module and issuing the callbacks */
#define ADC_MAIN_POWER_TRANSITION_MANAGER_SID (uint8)0x14

/*****************************************************************************************
                                    FUNCTION PROTOTYPES
 *****************************************************************************************/

/************************************************************************
*  Service name: Adc_Init
*  Sync/Async: Synchronous                                                                                  
*  Reentrancy: Non Reentrant                                                                                
*  Parameters (in): ConfigPtr Pointer to configuration set in Variant PB (Variant PC requires a NULL_PTR).  
*  Parameters (inout): None                                                                                 
*  Parameters (out): None                                                                                   
*  Return value: None                                                                                       
*  Description: Initializes the ADC hardware units and driver.                                              
************************************************************************/
void Adc_Init(const Adc_ConfigType* ConfigPtr);




/************************************************************************
*  Service name: Adc_DeInit
*  Sync/Async: Synchronous
*  Reentrancy: Non Reentrant
*  Parameters (in): None
*  Parameters (inout): None
*  Parameters (out): None
*  Return value: None
*  Description: Returns all ADC HW Units to a state comparable to their power on reset state.
 ************************************************************************/
void Adc_DeInit(void);





/************************************************************************
*  Service name: Adc_SetupResultBuffer
*  Sync/Async: Asynchronous
*  Reentrancy: Reentrant
*  Parameters (in): Group Numeric ID of requested ADC channel group.
*  Parameters (in):	DataBufferPtr pointer to result data buffer
*  Parameters (inout): None
*  Parameters (out): None
*  Return value: E_OK: result buffer pointer initialized correctly
*  Return value: E_NOT_OK: operation failed or development error occured
*  Description: Returns all ADC HW Units to a state comparable to their power on reset state.
 ************************************************************************/
Std_ReturnType Adc_SetupResultBuffer( Adc_GroupType Group, const Adc_ValueGroupType* DataBufferPtr);





/************************************************************************
*  Service name: Adc_StartGroupConversion
*  Sync/Async: Asynchronous
*  Reentrancy: Reentrant
*  Parameters (in): Group Numeric ID of requested ADC Channel group.
*  Parameters (inout): None
*  Parameters (out): None
*  Return value: None
*  Description: Starts the conversion of all channels of the requested ADC Channel group.                                        
************************************************************************/
void Adc_StartGroupConversion(Adc_GroupType Group);





/************************************************************************
*  Service name: Adc_StopGroupConversion
*  Sync/Async: Synchronous
*  Reentrancy: Reentrant
*  Parameters (in): Group Numeric ID of requested ADC Channel group.
*  Parameters (inout): None
*  Parameters (out): None
*  Return value: None
*  Description: Stops the conversion of the requested ADC Channel group.                                                                    
************************************************************************/
void Adc_StopGroupConversion(Adc_GroupType Group);




/************************************************************************
*  Service name: Adc_ReadGroup
*  Sync/Async: Synchronous
*  Reentrancy: Reentrant
*  Parameters (in): Group Numeric ID of requested ADC channel group.
*  Parameters (inout): None
*  Parameters (out): DataBufferPtr ADC results of all channels of the selected group 
*                    are stored in the data buffer addressed with the pointer.
*  Return value: Std_ReturnType E_OK: results are available and written to the data buffer
*                                     E_NOT_OK: no results are available or development error occured
*  Description: Reads the group conversion result of the last completed conversion round of the
*               requested group and stores the channel values starting at the DataBufferPtr
*               address. The group channel values are stored in ascending channel number order
*               ( in contrast to the storage layout of the result buffer if streaming access is configured).                                                                     
************************************************************************/
Std_ReturnType Adc_ReadGroup(Adc_GroupType Group,Adc_ValueGroupType* DataBufferPtr);

/************************************************************************
*  Service name: Adc_EnableHardwareTrigger
*  Sync/Async: Asynchronous
*  Reentrancy: Reentrant
*  Parameters (in): Group Numeric ID of requested ADC Channel group.
*  Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Enables the hardware trigger for the requested ADC Channel group.                                                                    
************************************************************************/
void Adc_EnableHardwareTrigger(Adc_GroupType Group);



/************************************************************************
*  Service name: Adc_DisableHardwareTrigger
*  Sync/Async: Asynchronous
*  Reentrancy: Reentrant
*  Parameters (in): Group Numeric ID of requested ADC Channel group.
*  Parameters (inout): None
*  Parameters (out): None
*  Return value: None
*  Description: Disables the hardware trigger for the requested ADC Channel group                                                                    
************************************************************************/
void Adc_DisableHardwareTrigger(Adc_GroupType Group);




#if (ADC_GRP_NOTIF_CABABILITY == STD_ON)

/************************************************************************
*  Service name: Adc_EnableGroupNotification
*  Sync/Async: Asynchronous
*  Reentrancy: Reentrant
*  Parameters (in): Group Numeric ID of requested ADC Channel group.
*  Parameters (inout): None
*  Parameters (out): None
*  Return value: None
*  Description: Enables the notification mechanism for the requested ADC Channel group.                                                                    
************************************************************************/
void Adc_EnableGroupNotification(Adc_GroupType Group);


/************************************************************************
*  Service name: Adc_DisableGroupNotification
*  Sync/Async: Asynchronous
*  Reentrancy: Reentrant
*  Parameters (in): Group Numeric ID of requested ADC Channel group.
*  Parameters (inout): None
*  Parameters (out): None
*  Return value: None
*  Description: Disables the notification mechanism for the requested ADC Channel group.                                                                    
************************************************************************/

void Adc_DisableGroupNotification(Adc_GroupType Group);

#endif


/************************************************************************
*  Service name: Adc_GetGroupStatus
*  Sync/Async: Synchronous
*  Reentrancy: Reentrant
*  Parameters (in): Group Numeric ID of requested ADC Channel group.
*  Parameters (inout): None
*  Parameters (out): None
*  Return value: Adc_StatusType Conversion status for the requested group.
*  Description: Returns the conversion status of the requested ADC Channel group.                                                                    
************************************************************************/
Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group);




#if (ADC_VERSION_INFO_API == STD_ON)
/************************************************************************
*  Service name: Adc_GetVersionInfo
*  Sync/Async: Synchronous
*  Reentrancy: Reentrant
*  Parameters (in): None
*  Parameters (inout): None
*  Parameters (out): versioninfo Pointer to where to store the version information of this module.
*  Return value: None
*  Description: Returns the version information of this module.                                                                    
************************************************************************/
void Adc_GetVersionInfo(Std_VersionInfoType* versioninfo);

#endif



/************************************************************************
*  Service name: Adc_GetStreamLastPointer
*  Sync/Async: Synchronous
*  Reentrancy: Reentrant
*  Parameters (in): Group Numeric ID of requested ADC Channel group.
*  Parameters (inout): None
*  Parameters (out): PtrToSamplePtr Pointer to result buffer pointer.
*  Return value: Adc_StreamNumSampleType Number of valid samples per channel.
*  Description: Returns the number of valid samples per channel, stored in the result buffer.
*               Reads a pointer, pointing to a position in the group result buffer. With the pointer
*               position, the results of all group channels of the last completed conversion round
*               can be accessed.
*               With the pointer and the return value, all valid group conversion results can be
*               accessed (the user has to take the layout of the result buffer into account). 
************************************************************************/
Adc_StreamNumSampleType Adc_GetStreamLastPointer(Adc_GroupType Group,Adc_ValueGroupType** PtrToSamplePtr);




/***********************************************************************
*  Service name: Adc_SetupResultBuffer
*  Sync/Async: Asynchronous
*  Reentrancy: Reentrant
*  Parameters (in): Group Numeric ID of requested ADC channel group.
*  DataBufferPtr pointer to result data buffer
*  Parameters (inout): None
*  Parameters (out): None
*  Return value: Std_ReturnType E_OK: result buffer pointer initialized correctly
*                               E_NOT_OK: operation failed or development error occured
*  Description: Initializes ADC driver with the group specific result buffer start address where the
*               conversion results will be stored.                                                                     
************************************************************************/
Std_ReturnType Adc_SetupResultBuffer(Adc_GroupType Group, const Adc_ValueGroupType* DataBufferPtr);




/************************************************************************
*  Service name: Adc_SetPowerState
*  Sync/Async: Synchronous
*  Reentrancy: Non Reentrant
*  Parameters (in): None
*  Parameters (inout): None
*  Parameters (out): Result If the API returns E_OK:
*                    ADC_SERVICE_ACCEPTED: Power state change executed.
*
*                    If the API returns E_NOT_OK:
*                    ADC_NOT_INIT: ADC Module not initialized.
*                    ADC_SEQUENCE_ERROR: wrong API call sequence.
*                    ADC_HW_FAILURE: the HW module has a failure which prevents it to enter the required power state.
*  Return value: Std_ReturnType E_OK: Power Mode changed
*                               E_NOT_OK: request rejected
*  Description: This API configures the Adc module so that it enters the already prepared power
*               state, chosen between a predefined set of configured ones.                                                                    
************************************************************************/
Std_ReturnType Adc_SetPowerState(Adc_PowerStateRequestResultType* Result);



/************************************************************************
*  Service name: Adc_GetCurrentPowerState
*  Sync/Async: Synchronous
*  Reentrancy: Non Reentrant
*  Parameters (in): None
*  Parameters (inout): None
*  Parameters (out): CurrentPowerState The current power mode of the ADC HW Unit is returned in this parameter
*                    Result If the API returns E_OK:
*                    ADC_SERVICE_ACCEPTED: Current power mode was returned.
*
*                    If the API returns E_NOT_OK:
*                    ADC_NOT_INIT: ADC Module not initialized.
*  Return value: Std_ReturnType E_OK: Mode could be read
*                               E_NOT_OK: Service is rejected
* Description: This API returns the current power state of the ADC HW unit.                                                                    
************************************************************************/
Std_ReturnType Adc_GetCurrentPowerState(Adc_PowerStateType* CurrentPowerState,Adc_PowerStateRequestResultType* Result);



/************************************************************************
*  Service name: Adc_GetTargetPowerState
*  Sync/Async: Synchronous
*  Reentrancy: Non Reentrant
*  Parameters (in): None
*  Parameters (inout): None
*  Parameters (out): TargetPowerState The Target power mode of the ADC HW Unit is returned in this parameter
*                    Result If the API returns E_OK:
*                    ADC_SERVICE_ACCEPTED:Target power mode was returned.
*
*                    If the API returns E_NOT_OK:
*                    ADC_NOT_INIT: ADC Module not initialized.
*  Return value: Std_ReturnType E_OK: Mode could be read
*                               E_NOT_OK: Service is rejected
*  Description: This API returns the Target power state of the ADC HW unit.                                                                    
************************************************************************/
Std_ReturnType Adc_GetTargetPowerState(Adc_PowerStateType* TargetPowerState,Adc_PowerStateRequestResultType* Result);


/************************************************************************
*  Service name: Adc_PreparePowerState
*  Sync/Async: Synchronous
*  Reentrancy: Non Reentrant
*  Parameters (in): PowerState The target power state intended to be attained
*  Parameters (inout): None
*  Parameters (out): Result If the API returns E_OK:
*                    ADC_SERVICE_ACCEPTED: ADC Module power state preparation was started.
*
*                    If the API returns E_NOT_OK:
*                    ADC_NOT_INIT: ADC Module not initialized.
*                    ADC_SEQUENCE_ERROR: wrong API call sequence (Current Power State = Target Power State).
*                    ADC_POWER_STATE_NOT_SUPP: ADC Module does not support the requested power state.
*                    ADC_TRANS_NOT_POSSIBLE: ADC Module cannot transition directly from the current 
*                                            power state to the requested power state or the HW peripheral is still busy.
*  Return value: Std_ReturnType E_OK: Preparation process started
*                               E_NOT_OK: Service is rejected
*  Description: This API starts the needed process to allow the ADC HW module to enter the requested power state.                                                                    
************************************************************************/
Std_ReturnType Adc_PreparePowerState(Adc_PowerStateType PowerState,Adc_PowerStateRequestResultType* Result);


/************************************************************************
*  Service name: Adc_Main_PowerTransitionManager
*  Description: This API is cyclically called and supervises the power state transitions, checking
*               for the readiness of the module and issuing the callbacks
*               IoHwAb_Adc_NotifyReadyForPowerState<Mode> (see AdcPowerStateReadyCbkRef configuration parameter).                                                                    
************************************************************************/
void Adc_Main_PowerTransitionManager(void);



/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

/* Extern PB structures to be used by Adc and other modules */
extern const AdcConfigSet_t AdcConfigSet;
extern const Adc_ConfigType Adc_Configuration;
extern Adc_StatusType Adc_GroupStatus[ADC_CONFIGURED_GROUP];
extern Adc_ConfigType adc_config;

#endif /* ADC_H */