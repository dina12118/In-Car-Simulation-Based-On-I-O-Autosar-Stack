 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Paula Emad
 ******************************************************************************/

#ifndef PORT_H
#define PORT_H

#include "../../library/Common_Macros.h"

/*******************************************************************************
 *                              Module Definitions                             *
 *******************************************************************************/

/* Id for the company in the AUTOSAR
 * for example ID = 1000 :) */
#define PORT_VENDOR_ID    (1000U)

/* Dio Module Id */
#define PORT_MODULE_ID    (124U)

/* Dio Instance Id */
#define PORT_INSTANCE_ID  (0U)

/*
 * Module Version 1.0.0
 */
#define PORT_SW_MAJOR_VERSION           (1U)
#define PORT_SW_MINOR_VERSION           (0U)
#define PORT_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_AR_RELEASE_MINOR_VERSION   (0U)
#define PORT_AR_RELEASE_PATCH_VERSION   (3U)

/*
 * Macros for Dio Status
 */
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)

/* Standard AUTOSAR types */
#include "../../library/Std_Types.h"

/* AUTOSAR checking between Std Types and Port Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

 
/* Port Pre-Compile Configuration Header file */
#include "Port_Cfg.h"

/* AUTOSAR Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Port_Cfg.h does not match the expected version"
#endif

/* Software Version checking between PORT_Cfg.h and PORT.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of Port_Cfg.h does not match the expected version"
#endif

   
/* Non AUTOSAR files */
#include "../../library/Common_Macros.h"
   
 /******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/
/* Service ID for Port_Init */
#define PORT_INIT_SID						(uint8)0x00

/* Service ID for Port_SetPinDirection */
#define PORT_SET_PIN_DIR_SID				(uint8)0x01

/* Service ID for Port_RefreshPortDirection */
#define PORT_REFRESH_PORT_DIR_SID			(uint8)0x02

/* Service ID for Port_GetVersionInfo */
#define PORT_GET_VERSION_INFO_SID			(uint8)0x03

/* Service ID for Port_SetPinMode */
#define PORT_SET_PIN_MODE_SID				(uint8)0x04
   
   /*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/
/* DET code to report Invalid Pin ID Request */
#define PORT_E_PARAM_PIN (uint8)0x0A

/* DET code to report Port Pin not configured as changeable */
#define PORT_E_DIRECTION_UNCHANGEABLE             (uint8)0x0B

/* DET code to report API Port_Init service called with wrong parameter. */
#define  PORT_E_PARAM_CONFIG  (uint8)0x0C

/* DET code to report Invalid API Port_SetPinMode */
#define PORT_E_PARAM_INVALID_MODE     (uint8)0x0D

/* DET code to report API Port_SetPinMode service called when mode is unchangeable.*/
#define PORT_E_MODE_UNCHANGEABLE    (uint8)0x0E
   
/* DET code to report API service called without module initialization */
#define PORT_E_UNINIT    (uint8)0x0F 
   
/* DET code to report a Null Pointer */
#define PORT_E_PARAM_POINTER    (uint8)0x10

   
/* GPIO Registers base addresses */
#define GPIO_PORTA_BASE_ADDRESS           0x40004000
#define GPIO_PORTB_BASE_ADDRESS           0x40005000
#define GPIO_PORTC_BASE_ADDRESS           0x40006000
#define GPIO_PORTD_BASE_ADDRESS           0x40007000
#define GPIO_PORTE_BASE_ADDRESS           0x40024000
#define GPIO_PORTF_BASE_ADDRESS           0x40025000

/* GPIO Registers offset addresses */
#define PORT_DATA_REG_OFFSET              0x3FC
#define PORT_DIR_REG_OFFSET               0x400
#define PORT_ALT_FUNC_REG_OFFSET          0x420
#define PORT_PULL_UP_REG_OFFSET           0x510
#define PORT_PULL_DOWN_REG_OFFSET         0x514
#define PORT_DIGITAL_ENABLE_REG_OFFSET    0x51C
#define PORT_LOCK_REG_OFFSET              0x520
#define PORT_COMMIT_REG_OFFSET            0x524
#define PORT_ANALOG_MODE_SEL_REG_OFFSET   0x528
#define PORT_CTL_REG_OFFSET               0x52C

/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/

   
typedef enum
{
    PA0,
    PA1,
    PA2,
    PA3,
    PA4,
    PA5,
    PA6,
    PA7,
    PB0,
    PB1,
    PB2,
    PB3,
    PB4,
    PB5,
    PB6,
    PB7,
    PC0,
    PC1,
    PC2,
    PC3,
    PC4,
    PC5,
    PC6,
    PC7,
    PD0,
    PD1,
    PD2,
    PD3,
    PD4,
    PD5,
    PD6,
    PD7,
    PE0,
    PE1,
    PE2,
    PE3,
    PE4,
    PE5,
    PF0,
    PF1,
    PF2,
    PF3,
    PF4
}Port_PinType;

/* Description: Enum to hold PIN direction */
typedef enum
{
    PORT_PIN_IN,
    PORT_PIN_OUT
}Port_PinDirectionType;

/* Description: Enum to hold internal resistor type for PIN */
typedef enum
{
    OFF,
    PULL_UP,
    PULL_DOWN
}Port_InternalResistor;

/* Description: Port Pin Level value from Port pin list */
typedef enum
{
	PORT_PIN_LEVEL_LOW,
	PORT_PIN_LEVEL_HIGH
}Port_PinLevelValue;

/* Port pin mode from mode list for use with Port_Init() function */
typedef enum
{
  PORT_PIN_MODE_AIN, /*Analog mode*/
  PORT_PIN_MODE_0, /*DIO mode*/
  PORT_PIN_MODE_1, /*UART and SSCLK modes, mode depends on pin and port nums*/
  PORT_PIN_MODE_2, /*UART and SSCLK modes, mode depends on pin and port nums*/
  PORT_PIN_MODE_3, /*I2C and CAN modes,mode depends on pin and port nums*/
  PORT_PIN_MODE_4, /*PWM mode*/
  PORT_PIN_MODE_5, /*PWM mode*/
  PORT_PIN_MODE_6, /*ID and Ph modes,mode depends on pin and port nums*/
  PORT_PIN_MODE_7, /*TCC, WTCC modes, mode depends on pin and port nums*/
  PORT_PIN_MODE_8, /*CAN, USB and URTS modes, mode depends on pin and port nums*/
  PORT_PIN_MODE_9, /*C0 and C1*/
  PORT_PIN_MODE_14, /*TRD and TRCLK modes*/
}
Port_PinModeType;
/* Description: Structure to configure each individual PIN:
 *	1. the PORT Which the pin belongs to. 0, 1, 2, 3, 4 or 5
 *	2. the number of the pin in the PORT.
 *      3. the direction of pin --> INPUT or OUTPUT
 *      4. the internal resistor --> Disable, Pull up or Pull down
 *      5. initial port pin value (high, low)
 *      6. initial port pin mode  (ADC, DIO, ..)
 *      7. pin direction changeable (true, false)
 *      8. pin mode changeable (true, false)
 */
typedef struct 
{
    uint8 port_num; 
    uint8 pin_num; 
    Port_PinDirectionType direction;
    Port_InternalResistor resistor;
    Port_PinLevelValue initial_value;
    Port_PinModeType mode;
    boolean pin_dir_changeable;
    boolean pin_mode_changeable;
}Port_ConfigPin;

/* Array of Port_ConfigPin */
typedef struct
{
	Port_ConfigPin Pin[PORT_CONFIGURED_PINS];
}Port_ConfigType;
/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/************************************************************************************
* Service Name: Port_Init
* Service ID[hex]: 0x00
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): ConfigPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Setup the pin configuration:
*              - Setup the pin as Digital GPIO pin
*              - Setup the direction of the GPIO pin
*              - Setup the internal resistor for i/p pin
************************************************************************************/
void Port_Init (const Port_ConfigType *ConfigPtr );

/************************************************************************************
* Service Name: Port_SetPinDirection
* Service ID[hex]: 0x01
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): Pin - Port Pin ID number , Direction - Port Pin Direction
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Sets the port pin direction.
************************************************************************************/
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
void Port_SetPinDirection(Port_PinType Pin,
                          Port_PinDirectionType Direction);
#endif

/************************************************************************************
* Service Name: Port_RefreshPortDirection
* Service ID[hex]: 0x02
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Refreshes port direction.
************************************************************************************/
void Port_RefresheportDirection(void );

/************************************************************************************
* Service Name: Port_GetVersionInfo
* Service ID[hex]: 0x03
* Sync/Async: Synchronous
* Reentrancy: non reentrant
* Parameters (in): versionInfo - Pointer to where to store the version information of this module.
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Returns the version information of this module.
************************************************************************************/
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

/************************************************************************************
* Service Name: Port_SetPinMode
* Service ID[hex]: 0x04
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Pin- Port Pin ID number
                  Mode- New Port Pin mode to be set on port pin. 
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin mode.
************************************************************************************/
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode(Port_PinType Pin,
                     Port_PinModeType Mode);
#endif

/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

/* Extern PB structures to be used by Port and other modules */
extern const Port_ConfigType Port_PinConfig;

#endif /* PORT_H */
