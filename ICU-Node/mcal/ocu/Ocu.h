#ifndef OCU_H
#define OCU_H

#include "Std_Types.h"



/*-------------------OCU Channels-------------------*/

#define CHANNEL_0								0		
#define CHANNEL_1								1
#define CHANNEL_2								2
#define CHANNEL_3								3
#define CHANNEL_4								4
#define CHANNEL_5								5
#define CHANNEL_6								6
#define CHANNEL_7								7
#define CHANNEL_8								8	
#define CHANNEL_9								9
#define CHANNEL_10								10
#define CHANNEL_11								11	
#define CHANNEL_12								12
#define CHANNEL_13								13
#define CHANNEL_14								14
#define CHANNEL_15								15
#define CHANNEL_16								16
#define CHANNEL_17								17
#define CHANNEL_18								18
#define CHANNEL_19								19
#define CHANNEL_20								20
#define CHANNEL_21								21
#define CHANNEL_22								22
#define CHANNEL_23								23

//---------------------------------------------------------------------------------------

/*----------------Error Codes---------------------*/

#define OCU_E_UNINIT							0x02
#define OCU_E_PARAM_INVALID_CHANNEL				0x03
#define OCU_E_PARAM_INVALID_STATE				0x04
#define OCU_E_PARAM_INVALID_ACTION				0x05
#define OCU_E_NO_VALID_NOTIF 					0x06
#define OCU_E_ALREADY_INITIALIZED 				0x07
#define OCU_E_PARAM_POINTER 					0x08
#define OCU_E_PARAM_NO_PIN 						0x0A
#define OCU_E_INIT_FAILED						0x0B
#define OCU_E_BUSY								0x09 		//Runtime Error
//----------------------------------------------------------------------------------------

typedef uint8 Ocu_ChannelType;

typedef uint8 Ocu_ValueType;


/*Output state of the pin linked to an OCU channel*/
typedef enum{
	
	OCU_HIGH = 0x00,
	OCU_LOW
	
}Ocu_PinStateType;


/*Automatic action (by hardware) to be performed on a pin attached to an OCU channel*/
typedef enum {
	
	OCU_SET_HIGH = 0x00,
	OCU_SET_LOW,
	OCU_TOGGLE,
	OCU_DISABLE
	
}Ocu_PinActionType;

typedef struct{
	Ocu_ChannelType Channel_Num;
}Ocu_ConfigType;


/*Return information after setting a new threshold value*/
typedef enum{
	
	OCU_CM_IN_REF_INTERVAL = 0x00,
	OCU_CM_OUT_REF_INTERVAL
	
}Ocu_ReturnType;

typedef struct{
	/* true: detection and notification is enabled.
       false: detection and notification is disabled */
	boolean OcuDevErrorDetect   
	
}Ocu_General;

/* typedef struct{
	boolean OcuDeInitApi;
	boolean OcuGetCounterApi;
	boolean OcuNotificationSupported;
	boolean OcuSetAbsoluteThresholdApi;
	boolean OcuSetPinActionApi;
	boolean OcuSetPinStateApi;
	boolean OcuSetRelativeThresholdApi;
	boolean OcuVersionInfoApi;
	
}OcuConfigurationOfOptionalApis; */

typedef enum{
	
	OCU_DOWNCOUNTING,
	OCU_UPCOUNTING
}OcuCountdirection;

typedef enum{
	OCU_HIGH,
	OCU_LOW
}OcuOutputPinDefaultState;


typedef enum{
	
}OcuClockSource;

typedef enum{
	
}OcuPrescale;

typedef struct{
	
}OcuHWSpecificSettings;


typedef struct{
	uint8 OcuAssignedHardwareChannel;
	//uint32 OcuChannelId;
	OcuCountdirection CountDirection; //enum
	uint16 OcuChannelTickDuration; /*Specifies the number of input clock edges(rising or falling edges) 
	                                 required to increase the channel counter by one 
									 (i.e. one counter tick). The value range depends on the used HW*/
									 
	uint32 OcuDefaultThreshold; 
    /*uint8 OcuHardwareTriggeredAdc;*/  /*This parameter is used to allow the OCU channel to trigger an ADC 
	                                  channel upon compare match, if this is supported by hardware. 
									  The value of the parameter represents the ADC physical channel 
									  to trigger.*/
    //uint8 OcuHardwareTriggeredDMA;
    uint32 OcuMaxCounterValue;
    void (*OcuNotification) (void);
    OcuOutputPinDefaultState OutputPinDefaultState;       //enum	
	boolean OcuOutputPinUsed;
}OcuChannel;

/* typedef struct{
	OcuCountdirection CountDirection; //enum
	OcuChannel Channel;
}OcuConfigSet; */ 
//----------------------------------------------------------------------------------------

extern const Ocu_ConfigType Ocu_ChannelConfig;

extern const OcuChannel Channel_Config[24];

//----------------------------------------------------------------------------------------
#if (OCU_INIT_API == STD_ON)
void Ocu_Init (const Ocu_ConfigType*);
#endif


#if (OCU_DEINIT_API == STD_ON)		
void Ocu_DeInit (void);
#endif

#if (OCU_START_CHANNEL_API == STD_ON)
Std_ReturnType Ocu_StartChannel (Ocu_ChannelType);
#endif

#if (OCU_STOP_CHANNEL_API == STD_ON)
void Ocu_StopChannel (Ocu_ChannelType);
#endif

#if (OCU_SET_PIN_STATE_API == STD_ON)
void Ocu_SetPinState (Ocu_ChannelType,Ocu_PinStateType);
#endif

#if (OCU_SET_PIN_ACTION == STD_ON)
void Ocu_SetPinAction (Ocu_ChannelType,Ocu_PinActionType);
#endif

#if (OCU_GET_COUNTER_API == STD_ON)
Ocu_ValueType Ocu_GetCounter (Ocu_ChannelType); 
#endif

#if (OCU_SET_ABSOLUTE_THRESHOLD_API == STD_ON)
Ocu_ReturnType Ocu_SetAbsoluteThreshold (Ocu_ChannelType,Ocu_ValueType,Ocu_ValueType);
#endif

#if (OCU_SET_RELATIVE_THRESHOLD_API == STD_ON)
Ocu_ReturnType Ocu_SetRelativeThreshold (Ocu_ChannelType,Ocu_ValueType);
#endif

#if (OCU_DISABLE_NOTIFICATION_API == STD_ON)
void Ocu_DisableNotification (Ocu_ChannelType);
#endif

#if (OCU_ENABLE_NOTIFICATION_API == STD_ON)
void Ocu_EnableNotification (Ocu_ChannelType);
#endif

#if (OCU_GET_VERSION_INFO_API == STD_ON)
void Ocu_GetVersionInfo (Std_VersionInfoType*);
#endif


#endif //#ifndef