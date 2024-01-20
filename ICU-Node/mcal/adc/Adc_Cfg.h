#ifndef ADC_CFG_H
#define ADC_CFG_H

/*
 * Module Version 1.0.0
 */
#define ADC_CFG_SW_MAJOR_VERSION              	(1U)
#define ADC_CFG_SW_MINOR_VERSION              	(0U)
#define ADC_CFG_SW_PATCH_VERSION              	(0U)

/*
 * AUTOSAR Version 4.0.4
 */
#define ADC_CFG_AR_RELEASE_MAJOR_VERSION     	(4U)
#define ADC_CFG_AR_RELEASE_MINOR_VERSION     	(0U)
#define ADC_CFG_AR_RELEASE_PATCH_VERSION     	(3U)


/*             ADC General API Configurations               */

/* Adds / removes the service Adc_DeInit() from the code */
#define ADC_DEINIT_API 						(STD_ON)	

/* Pre-compile option for Development Error Detect */
#define ADC_DEV_ERROR_DETECT                (STD_OFF)

/* Enables or disables limit checking feature in the ADC driver */
#define AdcEnableLimitCheck                (STD_ON)

/* is queuing mechanism active in case of disabled priority*/
#define AdcEnableQueuing                    (STD_ON)

/*  Pre-compile option for   Adc_EnableHardwareTrigger()    */
/*  Pre-compile option for   Adc_DisableHardwareTrigger()   */
#define AdcHwTriggerApi                    (STD_ON)

/* Pre-compile option for Version Info API */
#define ADC_VERSION_INFO_API                (STD_ON)

/*  Pre-compile option for  */
#define ADC_ENABLE_START_STOP_GROUP_API     (STD_ON)

/* Pre-compile option for Enable Notification API */
#define ADC_GRP_NOTIF_CABABILITY            (STD_ON)

#define AdcPriorityImplementation      (ADC_PRIORITY_NONE)



/* Number of Groups */
#define ADC_CONFIGURED_GROUP                (1U)

#define ADC_GROUP0_CHANNELS                 (3U)

#define ADC_GROUP1_CHANNELS

#define ADC_GROUP2_CHANNELS

#define ADC_GROUP3_CHANNELS

/* Adc_Channels Id for TM4C123GH6PM */
#define ADC_CHANNEL_0                           (0U)
#define ADC_CHANNEL_1							(1U)
#define ADC_CHANNEL_2							(2U)
#define ADC_CHANNEL_3 							(3U)
#define ADC_CHANNEL_4							(4U)
#define ADC_CHANNEL_5							(5U)
#define ADC_CHANNEL_6 							(6U)
#define ADC_CHANNEL_7							(7U)
#define ADC_CHANNEL_8							(8U)
#define ADC_CHANNEL_9							(9U)
#define ADC_CHANNEL_10							(10U)
#define ADC_CHANNEL_11	                        (11U)


/* Adc Modules for TM4C123GH6PM */
#define ADC_0_INSTANCE_ID  	(0U)	// ADC 0 instance
#define ADC_1_INSTANCE_ID  	(1U)	// ADC 1 instance

/* Adc Sequencers for TM4C123GH6PM */
#define ADC_SEQUENCER_0							(0U)
#define ADC_SEQUENCER_1							(1U)
#define ADC_SEQUENCER_2							(2U)
#define ADC_SEQUENCER_3                         (3U)
                                                  
#define ADC_MAX_GROUP                           (8U)
                                                  
/* Adc Groups for TM4C123GH6PM */                 
#define ADC_0_GROUP_0  	 						(0U)
#define ADC_0_GROUP_1	 						(1U)
#define ADC_0_GROUP_2	 						(2U)
#define ADC_0_GROUP_3	 						(3U)
#define ADC_1_GROUP_0	 						(4U)
#define ADC_1_GROUP_1	 						(5U)
#define ADC_1_GROUP_2	 						(6U)
#define ADC_1_GROUP_3	 						(7U)
                                                  
/* Adc Sample Averaging for TM4C123GH6PM */       
#define ADC_NoHardwareOverSampling				(0U)
#define ADC_2xHardwareOversampling				(1U)
#define ADC_4xHardwareOversampling				(2U)
#define ADC_8xHardwareOversampling              (3U)
#define ADC_16xHardwareOversampling				(4U)
#define ADC_32xHardwareOversampling				(5U)
#define ADC_64xHardwareOversampling             (6U)
                                                  
/*GROUP Priority for TM4C123GH6PM*/               
#define ADC_GroupPriority_0				        (0U)
#define ADC_GroupPriority_1				        (1U)
#define ADC_GroupPriority_2				        (2U)
#define ADC_GroupPriority_3				        (3U)

/* Trigger Events for TM4C123GH6PM */

#define TRIG_SW                                 (0x00U)
#define TRIG_AN_COMP_0                          (0x01U)
#define TRIG_AN_COMP_1                          (0x02U)
#define TRIG_EXT_GPIO                           (0x04U)
#define TRIG_TIMER                              (0x05U)
#define TRIG_PWM_0                              (0x06U)
#define TRIG_PWM_1                              (0x07U)
#define TRIG_PWM_2                              (0x08U)
#define TRIG_PWM_3                              (0x09U)
#define TRIG_CONTINOUS                          (0x0FU)


#endif /* ADC_CFG_H */