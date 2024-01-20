/******************************************************************************
*
* Module: Adc
*
* File Name: Adc_PBCfg.C
*
* Description: Post build configure source file for TM4C123GH6PM Microcontroller - Adc Driver.
*
* Author: 
******************************************************************************/

#include "Adc_Cfg.h"
#include "Adc.h"

/*
*Module version 1.0.0
*/
#define ADC_PBCFG_SW_MAJOR_VERSION (1U)
#define ADC_PBCFG_SW_MINOR_VERSION (0U)
#define ADC_PBCFG_SW_PACTH_VERSION (0U)

/*
*AUTOSAR version 4.0.4
*/
#define ADC_PBCFG_AR_RELEASE_MAJOR_VERSION (4U)
#define ADC_PBCFG_AR_RELEASE_MINOR_VERSION (0U)
#define ADC_PBCFG_AR_RELEASE_PATCH_VERSION (3U)

/* AUTOSAR Version checking between Adc_PBcfg.c and Adc.h files */
#if ((ADC_PBCFG_AR_RELEASE_MAJOR_VERSION != ADC_AR_RELEASE_MAJOR_VERSION)\
||  (ADC_PBCFG_AR_RELEASE_MINOR_VERSION != ADC_AR_RELEASE_MINOR_VERSION)\
||  (ADC_PBCFG_AR_RELEASE_PATCH_VERSION != ADC_AR_RELEASE_PATCH_VERSION))
#error "The AR version of PBcfg.c does not match the expected version"
#endif

/* Software Version checking between Adc_PBcfg.c and Adc.h files */
#if ((ADC_PBCFG_SW_MAJOR_VERSION != ADC_SW_MAJOR_VERSION)\
||  (ADC_PBCFG_SW_MINOR_VERSION != ADC_SW_MINOR_VERSION)\
||  (ADC_PBCFG_SW_PACTH_VERSION != ADC_SW_PATCH_VERSION))
#error "The SW version of PBcfg.c does not match the expected version"
#endif

Adc_ChannelType Adc_Group0_Channels[ADC_GROUP0_CHANNELS] = {ADC_CHANNEL_0 , ADC_CHANNEL_1, ADC_CHANNEL_2};
	
Adc_ValueGroupType Adc_Group0_Buffers[ADC_GROUP0_CHANNELS];

Adc_StatusType Adc_GroupStatus[ADC_CONFIGURED_GROUP]=
{
	/* ADC_IDLE */ 
	/* The conversion of the specified group has not been started. - 
	No result is available.*/
	ADC_IDLE,
};

Adc_GroupType	Group0[8] = {ADC_CHANNEL_0};
Adc_GroupType	Group1[4] = {ADC_CHANNEL_0};
Adc_GroupType	Group2[4] = {ADC_CHANNEL_0};
Adc_GroupType	Group3[1] = {ADC_CHANNEL_11};
Adc_GroupType	Group4[8] = {ADC_CHANNEL_0};
Adc_GroupType	Group5[4] = {ADC_CHANNEL_0};
Adc_GroupType	Group6[4] = {ADC_CHANNEL_0};
Adc_GroupType	Group7[1] = {ADC_CHANNEL_0};


const AdcConfigSet_t AdcConfigSet = {
	.AdcHwUnit[0].AdcPrescale = KS_125,
	.AdcHwUnit[0].AdcHwUnitId = 0,

	.AdcHwUnit[0].AdcGroup[0].AdcGroupPriority 	= ADC_GroupPriority_3,
	.AdcHwUnit[0].AdcGroup[0].AdcGroupTriggSrc 	= ADC_TRIGG_SRC_SW,
	.AdcHwUnit[0].AdcGroup[0].AdcGroupId 		= ADC_0_GROUP_0,
	.AdcHwUnit[0].AdcGroup[0].AdcGroupDefinition = Group0,

	.AdcHwUnit[0].AdcGroup[1].AdcGroupPriority 	= ADC_GroupPriority_2,
	.AdcHwUnit[0].AdcGroup[1].AdcGroupTriggSrc 	= ADC_TRIGG_SRC_SW,
	.AdcHwUnit[0].AdcGroup[1].AdcGroupId 		= ADC_0_GROUP_1,
	.AdcHwUnit[0].AdcGroup[1].AdcGroupDefinition = Group1,

	.AdcHwUnit[0].AdcGroup[2].AdcGroupPriority 	= ADC_GroupPriority_1,
	.AdcHwUnit[0].AdcGroup[2].AdcGroupTriggSrc 	= ADC_TRIGG_SRC_SW,
	.AdcHwUnit[0].AdcGroup[2].AdcGroupId 		= ADC_0_GROUP_2,
	.AdcHwUnit[0].AdcGroup[2].AdcGroupDefinition = Group2,

	.AdcHwUnit[0].AdcGroup[3].AdcGroupPriority 	= ADC_GroupPriority_0,
	.AdcHwUnit[0].AdcGroup[3].AdcGroupTriggSrc	= ADC_TRIGG_SRC_SW,
	.AdcHwUnit[0].AdcGroup[3].AdcGroupId 		= ADC_0_GROUP_3,
	.AdcHwUnit[0].AdcGroup[3].AdcGroupDefinition = Group3,


	.AdcHwUnit[1].AdcPrescale = KS_125,
	.AdcHwUnit[1].AdcHwUnitId = 1,

	.AdcHwUnit[1].AdcGroup[0].AdcGroupPriority 	= ADC_GroupPriority_3,
	.AdcHwUnit[1].AdcGroup[0].AdcGroupTriggSrc 	= ADC_TRIGG_SRC_SW,
	.AdcHwUnit[1].AdcGroup[0].AdcGroupId 		= ADC_1_GROUP_0,
	.AdcHwUnit[1].AdcGroup[0].AdcGroupDefinition = Group4,

	.AdcHwUnit[1].AdcGroup[1].AdcGroupPriority 	= ADC_GroupPriority_2,
	.AdcHwUnit[1].AdcGroup[1].AdcGroupTriggSrc 	= ADC_TRIGG_SRC_SW,
	.AdcHwUnit[1].AdcGroup[1].AdcGroupId 		= ADC_1_GROUP_1,
	.AdcHwUnit[1].AdcGroup[1].AdcGroupDefinition = Group5,

	.AdcHwUnit[1].AdcGroup[2].AdcGroupPriority 	= ADC_GroupPriority_1,
	.AdcHwUnit[1].AdcGroup[2].AdcGroupTriggSrc 	= ADC_TRIGG_SRC_SW,
	.AdcHwUnit[1].AdcGroup[2].AdcGroupId 		= ADC_1_GROUP_2,
	.AdcHwUnit[1].AdcGroup[2].AdcGroupDefinition = Group6,

	.AdcHwUnit[1].AdcGroup[3].AdcGroupPriority 	= ADC_GroupPriority_0,
	.AdcHwUnit[1].AdcGroup[3].AdcGroupTriggSrc	= ADC_TRIGG_SRC_SW,
	.AdcHwUnit[1].AdcGroup[3].AdcGroupId 		= ADC_1_GROUP_3,
	.AdcHwUnit[1].AdcGroup[3].AdcGroupDefinition = Group7,
};

Adc_ConfigType adc_config ={
	.ADC_instance = 0,
};