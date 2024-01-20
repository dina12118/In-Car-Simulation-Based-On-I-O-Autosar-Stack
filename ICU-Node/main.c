	/*
 * 
 *
 * Created: 11/20/2023 11:24:52 PM
 * Author : Mina
 */ 

//*****************************************************************************
//
// INCLUDES SECTION
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "mcal/icu/icu.h"
#include "mcal/dio/Dio.h"
#include "mcal/gpt/gpt.h"
#include "mcal/adc/Adc.h"
#include "ecual/led/Led.h"
#include "ecual/lm393/lm393.h"
#include "ecual/CanDriver/CanDriver.h"
#include "ecual/ultrasonic/ultrasonic.h"
#include "driverlib/sysctl.h"

//*****************************************************************************
//
// VARIABLES SECTION
//
//*****************************************************************************
uint32 US_CrashDistance = 0, US_ParkDistance = 0, ENC_Count = 0, ENC_Flag = 0;

//*****************************************************************************
//
// FUNCTION PROTOTYPES SECTION
//
//*****************************************************************************
uint16_t GetTemp(void);
void UltrasonicTakeAction(void);

void Delay(void)
{
    volatile uint32_t ui32Loop;
    for(ui32Loop = 0; ui32Loop<400000; ui32Loop++)
    {
    }
}

void Reset_Speed(void){
	// update encoder reading
	ENC_Flag = 1;
}

uint32_t clockFreq = 0;
uint16_t TempReading = 0;



int main(void)
{	
    clockFreq = SysCtlClockGet();
	Adc_Init(&adc_config);
    InitCAN0();
	ULTRASONIC_Init();
	SysTick_SetCallBack(Reset_Speed);
	SysTick_Start(1000);
	
	for(uint8_t counter = 0; counter < 5; counter++)
	{
		Delay();
	}
	
	while(1)
	{ 
		// update ultrasonic reading
		ULTRASONIC_ReadDistance(&US_CrashDistance, 1);
		ULTRASONIC_ReadDistance(&US_ParkDistance, 2);
		
		if(ENC_Flag)
		{
			ENC_Count = Icu_GetEdgeNumbers(2);
			Icu_ResetEdgeCount(2);
			ENC_Flag = 0;
		}
		
		// update temperature reading
		TempReading = GetTemp();
		
		// update and send CAN data
		g_ui8RX_AcButt_Motor[0] = US_CrashDistance;
		g_ui8RX_AcButt_Motor[1] = US_ParkDistance;
		g_ui8RX_AcButt_Motor[2] = ENC_Count;
		g_ui8RX_AcButt_Motor[3] = TempReading;
		CANMessageSet(CAN0_BASE,ObjID_AcButt_Motor, &g_sCAN0R_MsgID_AcButt_Motor,	MSG_OBJ_TYPE_TX);
		
		// take action based on ultrasonic reading
		UltrasonicTakeAction();
		
		//take action based on CAN error
		if(g_ui32ErrFlag != 0)
		{
			CANErrorHandler();
		}
		
	}	  
}

uint16_t GetTemp(void)
{
	uint16_t temp;
	Adc_StartGroupConversion( AdcConfigSet.AdcHwUnit[0].AdcGroup[3].AdcGroupId  );
	Adc_ReadGroup( AdcConfigSet.AdcHwUnit[0].AdcGroup[3].AdcGroupId , &temp);
	temp = (temp * 5000UL)/4096;
    temp /= 10;
    temp *= 0.66;
	return temp;
}


void UltrasonicTakeAction(void)
{
	if(US_ParkDistance > 50){
	  Dio_WriteChannel(BUZZER_CHANNEL_ID,STD_LOW);
	}
	else if(US_ParkDistance < 12){
	  Dio_WriteChannel(BUZZER_CHANNEL_ID,STD_HIGH);
	  for(uint32 i = 0; i < 250; i++){
		for(uint32 j = 0; j < 3180; j++);
	  }
	  Dio_WriteChannel(BUZZER_CHANNEL_ID,STD_LOW);
	  for(uint32 i = 0; i < 250; i++){
		for(uint32 j = 0; j < 3180; j++);
	  }
	}
	else if(US_ParkDistance < 25){
	  Dio_WriteChannel(BUZZER_CHANNEL_ID,STD_HIGH);
	  for(uint32 i = 0; i < 500; i++){
		for(uint32 j = 0; j < 3180; j++);
	  }
	  Dio_WriteChannel(BUZZER_CHANNEL_ID,STD_LOW);
	  for(uint32 i = 0; i < 500; i++){
		for(uint32 j = 0; j < 3180; j++);
	  }
	}
	else if(US_ParkDistance < 50){
	  Dio_WriteChannel(BUZZER_CHANNEL_ID,STD_HIGH);
	  for(uint32 i = 0; i < 1000; i++){
		for(uint32 j = 0; j < 3180; j++);
	  }
	  Dio_WriteChannel(BUZZER_CHANNEL_ID,STD_LOW);
	  for(uint32 i = 0; i < 1000; i++){
		for(uint32 j = 0; j < 3180; j++);
	  }
	}
}