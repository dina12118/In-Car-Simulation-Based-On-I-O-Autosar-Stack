#ifndef ICU_CFG_H
#define ICU_CFG_H

#define ON                                   1
#define OFF                                  0

/* APIs configurations */
#define IcuDeInitApi                         (STD_ON)
#define IcuSetModeApi                        (STD_ON)
#define IcuDisableWakeupApi                  (STD_ON)
#define IcuEnableWakeupApi                   (STD_ON)
#define ICU_TIMESTAMP_API                    (STD_ON)
#define IcuTimestampApi                      (STD_ON)
#define ICU_EDGE_COUNT_API                   (STD_ON)
#define IcuEdgeCountApi                      (STD_ON)
#define IcuEdgeDetectApi                     (STD_ON)
#define IcuSignalMeasurementApi              (STD_ON)
#define IcuGetTimeElapsedApi                 (STD_ON)
#define IcuGetInputStateApi                  (STD_ON)
#define IcuGetDutyCycleValuesApi             (STD_ON)

/*
 * Module Version 1.6.0
 */
#define ICU_CFG_SW_MAJOR_VERSION             (1U)
#define ICU_CFG_SW_MINOR_VERSION             (6U)
#define ICU_CFG_SW_PATCH_VERSION             (0U)

/*
 * AUTOSAR Version 4.4.0  
 */
#define ICU_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define ICU_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define ICU_CFG_AR_RELEASE_PATCH_VERSION     (3U)

/* Pre-compile option for Development Error Detect */
#define ICU_DEV_ERROR_DETECT                 (STD_ON)

/* Pre-compile option for Version Info API */
#define ICU_VERSION_INFO_API                 (STD_OFF)

/* Pre-compile option for presence of Dio_FlipChannel API */
#define ICU_FLIP_CHANNEL_API                 (STD_ON)

/* Number of configured channels */
#define ICU_CONFIGURED_CHANNLES              (3U)

#endif  /* ICU_CFG_H */