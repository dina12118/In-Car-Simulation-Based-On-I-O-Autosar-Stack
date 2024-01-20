/*****************************************************************************************
 *
 * Module: Adc
 * File Name: Adc.c
 * Description: Source file for TM4C123GH6PM Microcontroller - ADC Driver
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

#include "Adc.h"
#include "Adc_Regs.h"
#include "Adc_Cfg.h"
//#include "service/dem/dem.h"

#define REG(BASE,OFFSET) 		(*((volatile uint32 *)(BASE + OFFSET)))
#define R_ADC(ID,OFFSET) 	(*(( (volatile uint32 *)(ADC_BASE_ADDRESS + (ID * 0x1000U) + OFFSET) )))

#if (ADC_DEV_ERROR_DETECT == STD_ON)
#include "../../service/det/Det.h"
/* AUTOSAR Version checking between Det and ADC Modules */
#if ((DET_AR_MAJOR_VERSION != ADC_AR_RELEASE_MAJOR_VERSION)\
|| (DET_AR_MINOR_VERSION != ADC_AR_RELEASE_MINOR_VERSION)\
|| (DET_AR_PATCH_VERSION != ADC_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Det.h does not match the expected version"
#endif

#endif




/***********************************GLOBAL VARIABLES**************************************/
STATIC uint8 Adc_GroupInitStatus[2] = {ADC_NOT_INITIALIZED};
STATIC uint8 Adc_BufferInitStatus[ADC_MAX_GROUP] = {BUFFER_NOT_INITIALIZED};
STATIC uint8 Adc_GroupIdleStatus[ADC_MAX_GROUP] = {ADC_IDLE};
STATIC uint8 Adc_GroupContinousMode[ADC_MAX_GROUP] = {MODE_ONE_SHOT};


/*	pointers to buffers for each group	*/
static const Adc_ValueGroupType* GroupSeqBuffer[ADC_MAX_GROUP] = {NULL_PTR,NULL_PTR,NULL_PTR,NULL_PTR,
															NULL_PTR,NULL_PTR,NULL_PTR,NULL_PTR,
															};


/*	call back Notification pointers to function	*/
// static void(* Adc_INST0_Seq0_InterruptHandler)(void) = NULL_PTR;
// static void(* Adc_INST0_Seq1_InterruptHandler)(void) = NULL_PTR;
// static void(* Adc_INST0_Seq2_InterruptHandler)(void) = NULL_PTR;
// static void(* Adc_INST0_Seq3_InterruptHandler)(void) = NULL_PTR;
// static void(* Adc_INST1_Seq0_InterruptHandler)(void) = NULL_PTR;
// static void(* Adc_INST1_Seq1_InterruptHandler)(void) = NULL_PTR;
// static void(* Adc_INST1_Seq2_InterruptHandler)(void) = NULL_PTR;
// static void(* Adc_INST1_Seq3_InterruptHandler)(void) = NULL_PTR;

/******************STATIC HELPER FUNCTION PROTOTYPES**************************************/

static void Adc_EnableGroupSequencer( Adc_GroupType Group );
static void Adc_DisableGroupSequencer( Adc_GroupType Group );
static void Adc_SetGroupPriority( const Adc_ConfigType* ConfigPtr);

#if (ADC_DEV_ERROR_DETECT == STD_ON)
static void ADC_Report_DET_Error(Adc_GroupType Group, uint8 ApiId, uint8 ErrorId);
#endif

static void ADC_Report_DEM_Error(Adc_GroupType Group, uint8 ApiId, uint8 ErrorId);
static uint8 ADC_Return_Instance(Adc_GroupType Group);
static uint8 ADC_Return_Sequencer(Adc_GroupType Group);
static void ADC_InitClock(const Adc_ConfigType* ConfigPtr);
static void ADC_DeInitClock(const Adc_ConfigType* ConfigPtr);
static void ADC_SetTimeSample(const Adc_ConfigType* ConfigPtr);
static void Adc_SetGroupTrigger(const Adc_ConfigType* ConfigPtr);
static void ADC_SetupChannels(const Adc_ConfigType* ConfigPtr);
static void ADC_SetSampleControlBits(const Adc_ConfigType* ConfigPtr);
static uint8 Adc_ReturnTriggerSource(Adc_GroupType Group);

/*******************API IMPLEMENTATION****************************************************/

void Adc_Init(const Adc_ConfigType* ConfigPtr)
{					

	/* check if the input configuration pointer is not a NULL_PTR */
	if (NULL_PTR == ConfigPtr)
	{
		//Dem_ReportError(ADC_MODULE_ID, 0, ADC_INIT_SID,ADC_E_PARAM_POINTER);
	}
	else
	{
		/* Check if already initialzied */
		if(ADC_INITIALIZED  == Adc_GroupInitStatus[ConfigPtr->ADC_instance]){
			#if (ADC_DEV_ERROR_DETECT == STD_ON)
			Det_ReportError(ADC_MODULE_ID, ConfigPtr->ADC_instance, ADC_INIT_SID,ADC_E_ALREADY_INITIALIZED);
			#endif
		}
		else
		{
			
			//1. Initialize the Module
			ADC_InitClock(ConfigPtr);
			
			//2. Select the time Sampling
			ADC_SetTimeSample(ConfigPtr);
			
			//4. Set the Sequencer priority if exist
			Adc_SetGroupPriority(ConfigPtr);
		
			//5. Choose the Trigger Type
			Adc_SetGroupTrigger(ConfigPtr);
			
			//6. Choose the Channels in the Sequencer *Note: Seq0 can HOLD 8 CHANNELS
			ADC_SetupChannels(ConfigPtr);


			//7. Setup Sequencer End bit and channel at which interrupt flag is set
			ADC_SetSampleControlBits(ConfigPtr);
			
			//8. set group to initialized
			Adc_GroupInitStatus[ConfigPtr->ADC_instance] = ADC_INITIALIZED;

			//9. [SWS_Adc_00077] Adc_Init shall disable the notifications  
			#if (ADC_GRP_NOTIF_CABABILITY == STD_ON)
			Adc_DisableGroupNotification(AdcConfigSet.AdcHwUnit[ConfigPtr->ADC_instance].AdcGroup[0].AdcGroupId);
			Adc_DisableGroupNotification(AdcConfigSet.AdcHwUnit[ConfigPtr->ADC_instance].AdcGroup[1].AdcGroupId);
			Adc_DisableGroupNotification(AdcConfigSet.AdcHwUnit[ConfigPtr->ADC_instance].AdcGroup[2].AdcGroupId);
			Adc_DisableGroupNotification(AdcConfigSet.AdcHwUnit[ConfigPtr->ADC_instance].AdcGroup[3].AdcGroupId);
			#endif			
			
			//10. //[SWS_Adc_00307] Adc_Init shall set all groups to ADC_IDLE state
			Adc_GroupStatus[AdcConfigSet.AdcHwUnit[ConfigPtr->ADC_instance].AdcGroup[0].AdcGroupId] = ADC_IDLE;
			Adc_GroupStatus[AdcConfigSet.AdcHwUnit[ConfigPtr->ADC_instance].AdcGroup[1].AdcGroupId] = ADC_IDLE;
			Adc_GroupStatus[AdcConfigSet.AdcHwUnit[ConfigPtr->ADC_instance].AdcGroup[2].AdcGroupId] = ADC_IDLE;
			Adc_GroupStatus[AdcConfigSet.AdcHwUnit[ConfigPtr->ADC_instance].AdcGroup[3].AdcGroupId] = ADC_IDLE;
		}

		
	}
	return;
}

#if (ADC_GRP_NOTIF_CABABILITY == STD_ON)
void Adc_EnableGroupNotification(Adc_GroupType Group)																                                                                                                           
{
	uint8 Instance_ID = ADC_Return_Instance(Group);
	uint8 Sequencer_ID = ADC_Return_Sequencer(Group);
	
	//[SWS_Adc_00130] if the channel group ID is non-existing, the function Adc_EnableGroupNotification shall raise development error ADC_E_PARAM_GROUP and return without any action.
	#if (ADC_DEV_ERROR_DETECT == STD_ON)
	if(ADC_MAX_GROUP <= Group)
	{
		Det_ReportError(ADC_MODULE_ID, Instance_ID, ADC_ENABLE_GROUP_NOTIFICATION_SID, ADC_E_PARAM_GROUP);
		
		return;
	}
	else
	{
		/*	NOTHING	*/
	}
	
	//[SWS_Adc_00299] if called prior to initializing the ADC module, Adc_EnableGroupNotification shall raise development error ADC_E_UNINIT and return without any action.
	if(ADC_NOT_INITIALIZED == Adc_GroupInitStatus[Instance_ID])
	{
		Det_ReportError(ADC_MODULE_ID, Instance_ID, ADC_ENABLE_GROUP_NOTIFICATION_SID, ADC_E_UNINIT);
		
		return;
	}
	else
	{
		/*	NOTHING	*/
	}
	#endif

	R_ADC(Instance_ID, ADCIM) |= (1U << (Sequencer_ID));
	R_ADC(Instance_ID, ADCISC) |= (1U << (Sequencer_ID));
	return;
	
}

void Adc_DisableGroupNotification(Adc_GroupType Group)																                                                                                                           
{
	uint8 Instance_ID = ADC_Return_Instance(Group);
	uint8 Sequencer_ID = ADC_Return_Sequencer(Group);

	#if (ADC_DEV_ERROR_DETECT == STD_ON)
	if(ADC_MAX_GROUP <= Group)
	{	
		Det_ReportError(ADC_MODULE_ID, Instance_ID, ADC_DISABLE_GROUP_NOTIFICATION_SID, ADC_E_PARAM_GROUP);
		return;
	}
	else
	{
		/*	NOTHING	*/
	}

	//[SWS_Adc_00299] if called prior to initializing the ADC module, Adc_EnableGroupNotification shall raise development error ADC_E_UNINIT and return without any action.
	if(ADC_NOT_INITIALIZED == Adc_GroupInitStatus[Instance_ID])
	{
		Det_ReportError(ADC_MODULE_ID, Instance_ID, ADC_DISABLE_GROUP_NOTIFICATION_SID, ADC_E_UNINIT);
		return;
	}
	else
	{
		/*	NOTHING	*/
	}
	#endif
	
	R_ADC(Instance_ID, ADCIM) &= ~(1 << (Sequencer_ID));
	R_ADC(Instance_ID, ADCISC) &= ~(1 << (Sequencer_ID));
	return;
}
#endif
	
/* The application has to ensure that the application buffer, where DataBufferPtr points to can hold all the conversion results of the specified group */
Std_ReturnType Adc_SetupResultBuffer( Adc_GroupType Group, const Adc_ValueGroupType* DataBufferPtr)					                                                                                                           
{
	Std_ReturnType ret = E_OK;
	uint8 Instance_ID  = ADC_Return_Instance(Group);

	/*	if DataBufferPtr has a null pointer */
	#if (ADC_DEV_ERROR_DETECT == STD_ON)
	if( NULL_PTR == DataBufferPtr )
	{
		ret |= E_NOT_OK;	
		ADC_Report_DET_Error(Group, ADC_SETUP_RESULT_BUFFER_SID, ADC_E_PARAM_POINTER);
	}
	else
	{
		/*	NOTHING	*/
	}
		/*	if called while ADC is not initialized	*/
	if( ADC_NOT_INITIALIZED == Adc_GroupInitStatus[ Instance_ID ] )
	{
		ret |= E_NOT_OK;
		ADC_Report_DET_Error(Group, ADC_SETUP_RESULT_BUFFER_SID, ADC_E_UNINIT);
	}
	else
	{
		/*	NOTHING	*/
	}
		
	if(ADC_MAX_GROUP <= Group)
	{	
		Det_ReportError(ADC_MODULE_ID, Instance_ID, ADC_DISABLE_GROUP_NOTIFICATION_SID, ADC_E_PARAM_GROUP);
		ret |= E_NOT_OK;
		return ret;
	}
	else
	{
		/*	NOTHING	*/
	}
	#endif

		/*	if called while ADC is not IDLE return error ADC_E_BUSY	*/
	if( ADC_BUSY == Adc_GetGroupStatus( Group ) )
	{
		ret |= E_NOT_OK;
		ADC_Report_DEM_Error(Group, ADC_SETUP_RESULT_BUFFER_SID, ADC_E_BUSY);
	}
	else
	{
		
		/*	map group buffer pointer to memory address passed to argument */
		GroupSeqBuffer[Group] = DataBufferPtr;

		/* set buffer status of the group as initialized */
		Adc_BufferInitStatus[Group] = BUFFER_INITIALIZED;
		
		ret |= E_OK;
		
	}
	return ret;
}

Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group)																                                                                                                           
{
	if(Group <= ADC_0_GROUP_3){
		if ( 0 == BIT_IS_SET( REG(ADC0_BASE_ADDRESS, ADCACTSS) , 16) ){
			return ADC_IDLE;
		}
		else if( 1 == BIT_IS_SET( REG(ADC0_BASE_ADDRESS, ADCACTSS) , 16) ){
			return ADC_BUSY;
		}
		else{
			/* NOTHING*/
		}
	}
	else if(Group >= ADC_1_GROUP_0 && Group <= ADC_1_GROUP_3){
		if ( 0 == BIT_IS_SET( REG(ADC1_BASE_ADDRESS, ADCACTSS) , 16) ){
			return ADC_IDLE;
		}
		else if( 1 == BIT_IS_SET( REG(ADC1_BASE_ADDRESS, ADCACTSS) , 16) ){
			return ADC_BUSY;
		}
		else{
			/* NOTHING*/
		}
	}
	else{
		/*NOTHING*/
	}
}

Std_ReturnType Adc_ReadGroup(Adc_GroupType Group,Adc_ValueGroupType* DataBufferPtr)		// SWS are not yet implemented for this API                                                                                                            
{
    uint8 ADC_instance =  ADC_Return_Instance(Group) ;
	uint8 Sequencer_ID = ADC_Return_Sequencer(Group);

	Std_ReturnType ret = E_NOT_OK;
	if(NULL_PTR == DataBufferPtr)
	{
		ret = E_NOT_OK;

		#if (ADC_DEV_ERROR_DETECT == STD_ON)
		ADC_Report_DET_Error(Group, ADC_SETUP_RESULT_BUFFER_SID, ADC_E_PARAM_POINTER);
		#endif
	}
	else
	{	
		/*	Busy Wait for Flag	*/
		while(	0 == (R_ADC(ADC_instance,ADCRIS) & (1 << Sequencer_ID)) 	){};
		/*	map group buffer pointer to memory address passed to argument */
		switch(Group)
		{
			case ADC_0_GROUP_0: *DataBufferPtr = R_ADC(ADC_instance,ADCSSFIFO0) & 0xFFF ; ret = E_OK; break;
			case ADC_0_GROUP_1: *DataBufferPtr = R_ADC(ADC_instance,ADCSSFIFO1) & 0xFFF ; ret = E_OK; break;
			case ADC_0_GROUP_2: *DataBufferPtr = R_ADC(ADC_instance,ADCSSFIFO2) & 0xFFF ; ret = E_OK; break;
			case ADC_0_GROUP_3: *DataBufferPtr = R_ADC(ADC_instance,ADCSSFIFO3) & 0xFFF ; ret = E_OK; break;
			case ADC_1_GROUP_0: *DataBufferPtr = R_ADC(ADC_instance,ADCSSFIFO0) & 0xFFF ; ret = E_OK; break;
			case ADC_1_GROUP_1: *DataBufferPtr = R_ADC(ADC_instance,ADCSSFIFO1) & 0xFFF ; ret = E_OK; break;
			case ADC_1_GROUP_2: *DataBufferPtr = R_ADC(ADC_instance,ADCSSFIFO2) & 0xFFF ; ret = E_OK; break;
			case ADC_1_GROUP_3: *DataBufferPtr = R_ADC(ADC_instance,ADCSSFIFO3) & 0xFFF ; ret = E_OK; break;
			default: ret = E_NOT_OK;
			/*	report non existing group parameter	*/
			#if (ADC_DEV_ERROR_DETECT == STD_ON)
			Det_ReportError(ADC_MODULE_ID, ADC_instance, ADC_SETUP_RESULT_BUFFER_SID, ADC_E_PARAM_GROUP);
			#endif
		}
		
		/*	clear sequencer raw interrupt flag	*/
		R_ADC(ADC_instance,ADCISC) &= ~(1 << Sequencer_ID);
		
	}
	return ret;
}

#if (ADC_ENABLE_START_STOP_GROUP_API == STD_ON)
void Adc_StartGroupConversion(Adc_GroupType Group)																	                                                                                                           
{
	uint8 ADC_instance =  ADC_Return_Instance(Group) ;
	
#if (ADC_DEV_ERROR_DETECT == STD_ON)

	/* Check if called prior to initializing the driver */
	if(ADC_NOT_INITIALIZED == Adc_GroupInitStatus[ADC_instance]){
		Det_ReportError(ADC_MODULE_ID, ADC_instance, ADC_START_GROUP_CONVERSION_SID,  ADC_E_UNINIT);
		return;
	}
	else{
		
	}

	/* Check if called with a non-existing channel group ID */
	if(Group >= ADC_MAX_GROUP){
		Det_ReportError(ADC_MODULE_ID, ADC_instance, ADC_START_GROUP_CONVERSION_SID,  ADC_E_PARAM_GROUP);
		return;
	} 
	else{
		
	}
	
	/* Check if  called prior to initializing the result buffer pointer with function Adc_SetupResultBuffer */									//PLEASE FIX
	if(BUFFER_NOT_INITIALIZED == Adc_BufferInitStatus[Group]){
		Det_ReportError(ADC_MODULE_ID, ADC_instance, ADC_START_GROUP_CONVERSION_SID,   ADC_E_BUFFER_UNINIT);
		return;
	}
	else{
		
	}
	
	/* Check if called on a group with trigger source configured as hardware */
	uint8 ADC_TRIGGER_Source = Adc_ReturnTriggerSource(Group);
	
	if(TRIG_SW != ADC_TRIGGER_Source){
		Det_ReportError(ADC_MODULE_ID, ADC_instance, ADC_START_GROUP_CONVERSION_SID,  ADC_E_WRONG_TRIGG_SRC);
		return;
	}
	else{
		
	}

#endif
	//1. reset the internal result buffer pointer
	GroupSeqBuffer[Group] = 0;

	//2. start the conversion of all channels of the requested ADC Channel group.
	Adc_EnableGroupSequencer(Group);
	
	//3. when called while any of the groups is not in state ADC_IDLE 
	//   the function Adc_StartGroupConversion shall report a runtime error ADC_E_BUSY.
	if(ADC_0_INSTANCE_ID == ADC_instance){
		for(uint8 Group_ID = 0 ; Group_ID <= ADC_0_GROUP_3 ; Group_ID++){
			if((ADC_IDLE != Adc_GetGroupStatus(Group_ID)) || ( ADC_STREAM_COMPLETED != Adc_GetGroupStatus(Group_ID)) ){
				ADC_Report_DEM_Error(Group, ADC_START_GROUP_CONVERSION_SID,  ADC_E_BUSY);
			}
			else{
				/* Do nothing */
			}
		}
	}
	else if(ADC_1_INSTANCE_ID == ADC_instance){
		for(uint8 Group_ID = 4 ; Group_ID <= ADC_1_GROUP_3 ; Group_ID++){
			if((ADC_IDLE != Adc_GetGroupStatus(Group_ID)) || ( ADC_STREAM_COMPLETED != Adc_GetGroupStatus(Group_ID)) ){
				ADC_Report_DEM_Error(Group, ADC_START_GROUP_CONVERSION_SID,  ADC_E_BUSY);
			}
			else{
				/* Do nothing */
			}
		}
	}
	else{
		
	}


}

void Adc_StopGroupConversion(Adc_GroupType Group)																	                                                                                                           
{
	uint8 ADC_instance =  ADC_Return_Instance(Group) ;
	
#if (ADC_DEV_ERROR_DETECT == STD_ON)

	/* Check if called prior to initializing the driver */
	if(ADC_NOT_INITIALIZED == Adc_GroupInitStatus[ADC_instance]){
		Det_ReportError(ADC_MODULE_ID, ADC_instance, ADC_STOP_GROUP_CONVERSION_SID,  ADC_E_UNINIT);
		return;
	}
	else{
		
	}


	/* Check if called with a non-existing channel group ID */
	if(Group >= ADC_MAX_GROUP){
		Det_ReportError(ADC_MODULE_ID, ADC_instance, ADC_STOP_GROUP_CONVERSION_SID,  ADC_E_PARAM_GROUP);
		return;
	}
	else{
		
	}
	
	/* Check if called on a group with trigger source configured as hardware */
	uint8 ADC_TRIGGER_Source = Adc_ReturnTriggerSource(Group);
	
	if(TRIG_SW != ADC_TRIGGER_Source){
		Det_ReportError(ADC_MODULE_ID, ADC_instance, ADC_STOP_GROUP_CONVERSION_SID,  ADC_E_WRONG_TRIGG_SRC);
		return;
	}
	else{
		
	}
#endif

	//1. When called while the group is in state ADC_IDLE
	if(ADC_IDLE == Adc_GetGroupStatus(Group)){
		ADC_Report_DEM_Error(Group, ADC_START_GROUP_CONVERSION_SID,  ADC_E_BUSY);
	}
	else{
		
	}
	
	//2. The function Adc_StopGroupConversion shall automatically disable group notification
	Adc_DisableGroupNotification(Group);
	
	//3. The function Adc_StopGroupConversion shall set the group status to state ADC_IDLE.
	
}
#endif

/******************STATIC HELPER FUNCTION DEFINITION**************************************/

static void Adc_EnableGroupSequencer( Adc_GroupType Group )													                                                                                                           
{
	uint8 ADC_instance =  ADC_Return_Instance(Group) ;
	uint8 ADC_Sequencer = ADC_Return_Sequencer(Group);

	//Enable the sample sequencer in ADC
	R_ADC(ADC_instance,ADCACTSS)|= (1U << ADC_Sequencer);
	
	/*	start the conversion	*/
	R_ADC(ADC_instance,ADCPSSI) |= (1U << ADC_Sequencer);
}

static void Adc_DisableGroupSequencer( Adc_GroupType Group )													                                                                                                           
{
	uint8 ADC_instance =  ADC_Return_Instance(Group) ;
	uint8 ADC_Sequencer = ADC_Return_Sequencer(Group);

	//Disable the sample sequencer in ADC
	R_ADC(ADC_instance,ADCACTSS) &= ~(1 << ADC_Sequencer);
	
}

static void Adc_SetGroupPriority(const Adc_ConfigType* ConfigPtr)												                                                                                                           
{
	uint8 ADC_instance =  ConfigPtr->ADC_instance ;

	R_ADC(ADC_instance,ADCSSPRI) = 0x00000000;
	
	for( uint8 counter = 0; counter < 4; counter++){
		R_ADC(ADC_instance,ADCSSPRI) |= (  (AdcConfigSet.AdcHwUnit[ADC_instance].AdcGroup[counter].AdcGroupPriority) << ( counter * 4) );
	}
}

static void Adc_SetGroupTrigger(const Adc_ConfigType* ConfigPtr){												                                                                                                           
	
	uint8 ADC_instance =  ConfigPtr->ADC_instance ;

	R_ADC(ADC_instance,ADCEMUX)  = 0x00000000;

	for( uint8 counter = 0; counter < 4; counter++){
		R_ADC(ADC_instance,ADCEMUX) |= (  (AdcConfigSet.AdcHwUnit[ADC_instance].AdcGroup[counter].AdcGroupTriggSrc) << ( counter * 4) );
	}

	// switch( ConfigPtr->Adc_Group_Trigg_Src ){
	// 	case TRIG_SW:			Adc_GroupContinousMode[ConfigPtr->Adc_Group] = MODE_ONE_SHOT;	break;
	// 	case TRIG_AN_COMP_0:	Adc_GroupContinousMode[ConfigPtr->Adc_Group] = MODE_ONE_SHOT;	break;
	// 	case TRIG_AN_COMP_1:	Adc_GroupContinousMode[ConfigPtr->Adc_Group] = MODE_ONE_SHOT;	break;
	// 	case TRIG_EXT_GPIO:		Adc_GroupContinousMode[ConfigPtr->Adc_Group] = MODE_ONE_SHOT;	break;
	// 	case TRIG_TIMER:		Adc_GroupContinousMode[ConfigPtr->Adc_Group] = MODE_ONE_SHOT;	break;
	// 	case TRIG_PWM_0:		Adc_GroupContinousMode[ConfigPtr->Adc_Group] = MODE_ONE_SHOT;	break;
	// 	case TRIG_PWM_1:		Adc_GroupContinousMode[ConfigPtr->Adc_Group] = MODE_ONE_SHOT;	break;
	// 	case TRIG_PWM_2:		Adc_GroupContinousMode[ConfigPtr->Adc_Group] = MODE_ONE_SHOT;	break;
	// 	case TRIG_PWM_3:		Adc_GroupContinousMode[ConfigPtr->Adc_Group] = MODE_ONE_SHOT;	break;
	// 	case TRIG_CONTINOUS:	Adc_GroupContinousMode[ConfigPtr->Adc_Group] = MODE_CONTINOUS;	break;
	// 	default:				/* CODE */	break;
	// }
}

#if (ADC_DEV_ERROR_DETECT == STD_ON)
static void ADC_Report_DET_Error(Adc_GroupType Group, uint8 ApiId, uint8 ErrorId){

	if(Group >= ADC_0_GROUP_0 && Group <= ADC_0_GROUP_3){

		Det_ReportError(ADC_MODULE_ID, ADC_0_INSTANCE_ID, ApiId, ErrorId);

	}
	else if(Group >= ADC_1_GROUP_0 && Group <= ADC_1_GROUP_3){
		
		Det_ReportError(ADC_MODULE_ID, ADC_1_INSTANCE_ID, ApiId, ErrorId);

	}
	else{
		/*NOTHING*/
	}
}
#endif

static void ADC_Report_DEM_Error(Adc_GroupType Group, uint8 ApiId, uint8 ErrorId){

	if(Group <= ADC_0_GROUP_3){

		//Dem_ReportError(ADC_MODULE_ID, ADC_0_INSTANCE_ID, ApiId, ErrorId);

	}
	else if(Group >= ADC_1_GROUP_0 && Group <= ADC_1_GROUP_3){
		
		//Dem_ReportError(ADC_MODULE_ID, ADC_1_INSTANCE_ID, ApiId, ErrorId);

	}
	else{
		/*NOTHING*/
	}
}

static uint8 ADC_Return_Instance(Adc_GroupType Group){

	if(Group <= ADC_0_GROUP_3){

		return ADC_0_INSTANCE_ID;

	}
	else if(Group >= ADC_1_GROUP_0 && Group <= ADC_1_GROUP_3){
		
		return ADC_1_INSTANCE_ID;

	}
	else{
		/*NOTHING*/
	}
}

static uint8 ADC_Return_Sequencer(Adc_GroupType Group){

	if(Group > ADC_0_GROUP_3){
		return (Group - 4);
	}
	else{
		return (Group);
	}
}

static void ADC_SetTimeSample(const Adc_ConfigType* ConfigPtr)																						                                                                                                           
{																							

	R_ADC(ConfigPtr->ADC_instance,ADCPC) = AdcConfigSet.AdcHwUnit[ConfigPtr->ADC_instance].AdcPrescale ;
}

static void ADC_InitClock(const Adc_ConfigType* ConfigPtr)																							                                                                                                           	
{
	volatile uint32 delay = 0;

	/* Enable and provide clock to ADC module x in Run mode */

	if( ADC_0_INSTANCE_ID == ConfigPtr->ADC_instance ){

		 RCGCADC |= 0x01;

	}
	else if( ADC_1_INSTANCE_ID == ConfigPtr->ADC_instance ){

		 RCGCADC |= 0x02;

	}
	else{
		/*NOTHING*/
	}
	/*	extra time to stabilize	*/
	delay = RCGCADC;			
	delay = RCGCADC;			
	delay = RCGCADC;			
	delay = RCGCADC;			
}

#if ADC_DEINIT_API == STD_ON
static void ADC_DeInitClock(const Adc_ConfigType* ConfigPtr)	// continue by reading the busy before configuring the RCGCADC						                                                                                                           	
{								

	/* Disable clock of ADC module x in Run mode */

	if( ADC_0_INSTANCE_ID == ConfigPtr->ADC_instance ){

		R_ADC(ConfigPtr->ADC_instance,RCGCADC) &= ~(0x01);

		Adc_GroupInitStatus[ADC_0_INSTANCE_ID] = ADC_NOT_INITIALIZED;

	}
	else if( ADC_1_INSTANCE_ID == ConfigPtr->ADC_instance ){

		R_ADC(ConfigPtr->ADC_instance,RCGCADC) &= ~(0x02);

		Adc_GroupInitStatus[ADC_1_INSTANCE_ID] = ADC_NOT_INITIALIZED;

	}
	else{
		/*NOTHING*/	
	}

}
#endif

static void ADC_SetupChannels(const Adc_ConfigType* ConfigPtr)																						                                                                                                           
{
	uint8 ADC_instance =  ConfigPtr->ADC_instance ;
	uint8 channel_counter = 0;

	R_ADC( ADC_instance, ADCSSMUX0 ) = 0x00000000;
	R_ADC( ADC_instance, ADCSSMUX1 ) = 0x00000000;
	R_ADC( ADC_instance, ADCSSMUX2 ) = 0x00000000;
	R_ADC( ADC_instance, ADCSSMUX3 ) = 0x00000000;

	for(channel_counter = 0; channel_counter < 8; channel_counter++ ){
		R_ADC( ADC_instance, ADCSSMUX0 ) |= (AdcConfigSet.AdcHwUnit[ADC_instance].AdcGroup[0].AdcGroupDefinition[channel_counter])	<< (channel_counter * 4);	
	}
	for(channel_counter = 0; channel_counter < 4; channel_counter++ ){
		R_ADC( ADC_instance, ADCSSMUX1 ) |= (AdcConfigSet.AdcHwUnit[ADC_instance].AdcGroup[1].AdcGroupDefinition[channel_counter])	<< (channel_counter * 4);	
	}
	for(channel_counter = 0; channel_counter < 4; channel_counter++ ){
		R_ADC( ADC_instance, ADCSSMUX2 ) |= (AdcConfigSet.AdcHwUnit[ADC_instance].AdcGroup[2].AdcGroupDefinition[channel_counter])	<< (channel_counter * 4);	
	}
	for(channel_counter = 0; channel_counter < 1; channel_counter++ ){
		R_ADC( ADC_instance, ADCSSMUX3 ) |= (AdcConfigSet.AdcHwUnit[ADC_instance].AdcGroup[3].AdcGroupDefinition[channel_counter])	<< (channel_counter * 4);	
	}
	return;
}

static void ADC_SetSampleControlBits(const Adc_ConfigType* ConfigPtr)
{
	/*	Clear the Sample Sequence Control Register*/
	R_ADC( ConfigPtr->ADC_instance, ADCSSCTL0 ) = 0x00000000;
	R_ADC( ConfigPtr->ADC_instance, ADCSSCTL1 ) = 0x00000000;
	R_ADC( ConfigPtr->ADC_instance, ADCSSCTL2 ) = 0x00000000;
	R_ADC( ConfigPtr->ADC_instance, ADCSSCTL3 ) = 0x00000000;

	/*	Set End Bit according to no of channels*/
	R_ADC( ConfigPtr->ADC_instance, ADCSSCTL0 ) |= 1 << 29;
	R_ADC( ConfigPtr->ADC_instance, ADCSSCTL1 ) |= 1 << 13;
	R_ADC( ConfigPtr->ADC_instance, ADCSSCTL2 ) |= 1 << 13;
	R_ADC( ConfigPtr->ADC_instance, ADCSSCTL3 ) |= 1 << 1;

	/*	Set Interrupt Signal Allow at End of Group*/
	R_ADC( ConfigPtr->ADC_instance, ADCSSCTL0 ) |= 1 << 30;
	R_ADC( ConfigPtr->ADC_instance, ADCSSCTL1 ) |= 1 << 14;
	R_ADC( ConfigPtr->ADC_instance, ADCSSCTL2 ) |= 1 << 14;
	R_ADC( ConfigPtr->ADC_instance, ADCSSCTL3 ) |= 1 << 2;

	return;
}

static uint8 Adc_ReturnTriggerSource(Adc_GroupType Group)																			                                                                                                           
{
	uint8 ADC_instance =  ADC_Return_Instance(Group) ;
	uint8 ADC_Sequencer = ADC_Return_Sequencer(Group);
	
	uint16 Source = R_ADC(ADC_instance, ADCEMUX);
	Source = Source >> (ADC_Sequencer * 4);
	Source = Source & 0x000F;
	
	return Source;
}



