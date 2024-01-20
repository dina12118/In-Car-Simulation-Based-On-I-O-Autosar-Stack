
#include "Det.h"
#include "Ocu.h"
#include "Dio.h"

void (*Ocu_ISR) (void);

#if (OCU_INIT_API == STD_ON)
void Ocu_Init (const Ocu_ConfigType* ConfigPtr){
	
	if(ConfigPtr){
		
		/*diivide Timer into A&B*/
		Timer->CFG = 0x00000004;
	
		u8 Timer_Num;
		u8 Timer_A = 1; //use Timer A as default channel
		if(configptr->Channel_Num%2 == 1){
			Timer_Num = (configptr->Channel_Num - 1) / 2;
			Timer_A = 0;
		}
		else{
			Timer_Num = configptr->Channel_Num / 2;
			Timer_A = 1;
		}
		TimerRegisters Timer* = TimerBaseAddress[Timer_Num];
		
		if (Timer_Num < 6){
			SET_BIT(RCGCTimer,Timer_Num);
		}
		
		else{
			SET_BIT(RCGCWTimer,Timer_num-6);
		}
		
		if(Timer_A){
			/*Disable TimerA*/
			CLEAR_BIT(Timer->CTL,TAEN);
		
			/*Periodic mode 0x2*/
			CLEAR_BIT(Timer->TAMR,TAMR0);
			SET_BIT(Timer->TAMR,TAMR1);
		
			if(Channel_Config[configptr->Channel_Num].CountDirection == OCU_UPCOUNTING){
			
			/*up count direction*/
			SET_BIT(Timer->TAMR,TACDIR);
			}
		
			else if(Channel_Config[configptr->Channel_Num].CountDirection == OCU_DOWNCOUNTING){
			/*count down direction*/
			CLEAR_BIT(Timer->TAMR,TACDIR);
			}
		    /*Enable Timer A Capture Mode Match Interrupt Mask*/
			SET_BIT(Timer->MIMR,CAMIM);
	
			/*Enable Timer A Match Interrupt Mask*/
			SET_BIT(Timer->MIMR,TAMIM);
			
		}
		else{
			/*Disable TimerB*/
			CLEAR_BIT(Timer->CTL,TBEN);
		
			/*Periodic mode 0x2*/
			CLEAR_BIT(Timer->TBMR,TBMR0);
			SET_BIT(Timer->TBMR,TBMR1);
		    
			if(Channel_Config[configptr->Channel_Num].CountDirection == OCU_UPCOUNTING){
			
			/*up count direction*/
			SET_BIT(GPTMTBMR,TBCDIR);
			}
			else if(Channel_Config[configptr->Channel_Num].CountDirection == OCU_DOWNCOUNTING){
			/*count down direction*/
			CLEAR_BIT(GPTMTBMR,TBCDIR);
			}
			
			/*Enable Timer B Capture Mode Match Interrupt Mask*/
			SET_BIT(Timer->MIMR,CBMIM);
	
			/*Enable Timer B Match Interrupt Mask*/
			SET_BIT(Timer->MIMR,TBMIM);
		
		}
		
		
	}
	
	
}
#endif


#if (OCU_DEINIT_API == STD_ON)
void Ocu_DeInit (){
	
}
#endif

#if (OCU_START_CHANNEL_API == STD_ON)
Std_ReturnType Ocu_StartChannel (Ocu_ChannelType ChannelNumber){
	u8 Timer_Num;
	u8 Timer_A = 1; //use Timer A as default channel
	if(ChannelNumber%2 == 1){
		Timer_Num = (ChannelNumber - 1) / 2;
		Timer_A = 0;
	}
	else{
		Timer_Num = ChannelNumber / 2;
		Timer_A = 1;
	}
	TimerRegisters Timer* = TimerBaseAddress[Timer_Num];
	
	if(Timer_A){
	/*Enable TimerA*/
	SET_BIT(Timer->CTL,TAEN);
	}
	else{
	/*Enable TimerB*/
	SET_BIT(Timer->CTL,TBEN);
	}
}
#endif

#if (OCU_STOP_CHANNEL_API == STD_ON)
void Ocu_StopChannel (Ocu_ChannelType ChannelNumber){
	u8 Timer_Num;
	u8 Timer_A = 1; //use Timer A as default channel
	if(ChannelNumber%2 == 1){
		Timer_Num = (ChannelNumber - 1) / 2;
		Timer_A = 0;
	}
	else{
		Timer_Num = ChannelNumber / 2;
		Timer_A = 1;
	}
	TimerRegisters Timer* = TimerBaseAddress[Timer_Num];
	
	if(Timer_A){
	/*Disable TimerA*/
	CLEAR_BIT(Timer->CTL,TAEN);
	}
	else{
	/*Disable TimerB*/
	CLEAR_BIT(Timer->CTL,TBEN);
	}
}
#endif

#if (OCU_SET_PIN_STATE_API == STD_ON)
void Ocu_SetPinState(Ocu_ChannelType ChannelNumber,Ocu_PinStateType PinState){
	
}
#endif

#if (OCU_SET_PIN_ACTION == STD_ON)
void Ocu_SetPinAction(Ocu_ChannelType ChannelNumber,Ocu_PinActionType PinAction)
{
	
}
#endif

#if (OCU_GET_COUNTER_API == STD_ON)
Ocu_ValueType Ocu_GetCounter(Ocu_ChannelType ChannelNumber)
{
	Ocu_ValueType Timer_Value;
	u8 Timer_Num;
	u8 Timer_A = 1; //use Timer A as default channel
	if(ChannelNumber%2 == 1){
		Timer_Num = (ChannelNumber - 1) / 2;
		Timer_A = 0;
	}
	else{
		Timer_Num = ChannelNumber / 2;
		Timer_A = 1;
	}
	TimerRegisters Timer* = TimerBaseAddress[Timer_Num];
	
	if(Timer_A){
		Timer_Value = Timer->TAV;
	}
	else{
		Timer_Value = Timer->TBV;
	}
	return Timer_value;
} 
#endif

#if (OCU_SET_ABSOLUTE_THRESHOLD_API == STD_ON)
Ocu_ReturnType Ocu_SetAbsoluteThreshold(Ocu_ChannelType ChannelNumber,Ocu_ValueType ReferenceValue,
Ocu_ValueType AbsoluteValue)
{
	u8 Timer_Num;
	u8 Timer_A = 1; //use Timer A as default channel
	if(ChannelNumber%2 == 1){
		Timer_Num = (ChannelNumber - 1) / 2;
		Timer_A = 0;
	}
	else{
		Timer_Num = ChannelNumber / 2;
		Timer_A = 1;
	}
	TimerRegisters Timer* = TimerBaseAddress[Timer_Num];
	
	if(Timer_A){
		/*Set new threshold*/
		Timer->TAMR = AbsoluteValue; 
	}
	else{
		/*Set new threshold*/
		Timer->TBMR = AbsoluteValue;
	}
}
#endif

#if (OCU_SET_RELATIVE_THRESHOLD_API == STD_ON)
Ocu_ReturnType Ocu_SetRelativeThreshold(Ocu_ChannelType ChannelNumber,Ocu_ValueType RelativeValue){
	
	u8 Timer_Num;
	u8 Timer_A = 1; //use Timer A as default channel
	if(ChannelNumber%2 == 1){
		Timer_Num = (ChannelNumber - 1) / 2;
		Timer_A = 0;
	}
	else{
		Timer_Num = ChannelNumber / 2;
		Timer_A = 1;
	}
	TimerRegisters Timer* = TimerBaseAddress[Timer_Num];
	
	if(Timer_A){
		/*Set new threshold*/
		Timer->TAMR = Timer->TAV + RelativeValue; 
	}
	else{
		/*Set new threshold*/
		Timer->TBMR = Timer->TBV + RelativeValue;
	}
}
#endif

#if (OCU_DISABLE_NOTIFICATION_API == STD_ON)
void Ocu_DisableNotification (Ocu_ChannelType ChannelNumber){
	
}
#endif

#if (OCU_ENABLE_NOTIFICATION_API == STD_ON)
void Ocu_EnableNotification (Ocu_ChannelType ChannelNumber){
	
}
#endif

#if (OCU_GET_VERSION_INFO_API == STD_ON)
void Ocu_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	
}
#endif

void Ocu_Callback(void (*ptr) (void)){
	Ocu_ISR = ptr;
}