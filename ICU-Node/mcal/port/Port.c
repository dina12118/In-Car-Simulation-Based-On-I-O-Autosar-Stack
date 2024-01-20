 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Paula Emad
 ******************************************************************************/

#include "Port.h"
#include "../../library/tm4c123gh6pm_registers.h"

#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "../../service/det/Det.h"
/* AUTOSAR Version checking between Det and Port Modules */
#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 || (DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 || (DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Det.h does not match the expected version"
#endif

#endif

/* Holds the status of the Port*/
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;

/* Holds the pointer of the Port_PinConfig */
STATIC const Port_ConfigType* Port_ConfigPtr = NULL_PTR;
   
/************************************************************************************
* Service Name: Port_Init
* Sync/Async: Synchronous
* Reentrancy: reentrant
* Parameters (in): ConfigPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Setup the pin configuration:
*              - Setup the pin as Digital GPIO pin
*              - Setup the direction of the GPIO pin
*              - Provide initial value for o/p pin
*              - Setup the internal resistor for i/p pin
************************************************************************************/
void Port_Init(const Port_ConfigType * ConfigPtr )
{
    volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
    volatile uint32 delay = 0;
    Port_ConfigPtr = ConfigPtr;
    uint8 i=0;
   #if (PORT_DEV_ERROR_DETECT == STD_ON)
		/* check if the input configuration pointer is not a NULL_PTR */
		if(ConfigPtr == NULL_PTR)
		{
			Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID, PORT_E_PARAM_CONFIG);
		}
		else
		{	/* Do Nothing */	}
	#endif

	for(i = 0; i < PORT_CONFIGURED_PINS; i++)
	{
		switch(Port_ConfigPtr->Pin[i].port_num)
		{
			case  PORT_A: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
			 break;
			case  PORT_B: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
			 break;
			case  PORT_C: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
			 break;
			case  PORT_D: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
			 break;
			case  PORT_E: 
				continue; 
			 break;//PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
			 
			case  PORT_F: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
			 break;
		}

		/* Enable clock for PORT and allow time for clock to start*/
		SYSCTL_REGCGC2_REG |= (1<<(Port_ConfigPtr->Pin[i].port_num));
		delay = SYSCTL_REGCGC2_REG;

		if( ((Port_ConfigPtr->Pin[i].port_num == 3) && (Port_ConfigPtr->Pin[i].pin_num == 7)) || ((Port_ConfigPtr->Pin[i].port_num == 5) && (Port_ConfigPtr->Pin[i].pin_num == 0)) ) /* PD7 or PF0 */
		{
			/* Unlock the GPIOCR register */
			*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;

			/* Set the corresponding bit in GPIOCR register to allow changes on this pin */
			SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_COMMIT_REG_OFFSET) , Port_ConfigPtr->Pin[i].pin_num);
		}
		else if( (Port_ConfigPtr->Pin[i].port_num == 2) && (Port_ConfigPtr->Pin[i].pin_num <= 3)) /* PC0 to PC3 */
		{
			/* Do Nothing ...  this is the JTAG pins */
			continue;
		}
		else
		{
			/* Do Nothing ... No need to unlock the commit register for this pin */
		}

		if (Port_ConfigPtr->Pin[i].mode == PORT_PIN_MODE_0)
		{
			/* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
			CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_ConfigPtr->Pin[i].pin_num);

			/* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
			CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ConfigPtr->Pin[i].pin_num);

			/* Clear the PMCx bits for this pin */
			*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_ConfigPtr->Pin[i].pin_num * 4));

			/* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
			SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ConfigPtr->Pin[i].pin_num);
		}
		else if (Port_ConfigPtr->Pin[i].mode == PORT_PIN_MODE_AIN)
		{
			/* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
			CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ConfigPtr->Pin[i].pin_num);

			/* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
			CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ConfigPtr->Pin[i].pin_num);

			/* Clear the PMCx bits for this pin */
			*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_ConfigPtr->Pin[i].pin_num * 4));

			/* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
			SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_ConfigPtr->Pin[i].pin_num);
		}
		else /* Another mode */
		{
			/* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
			CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_ConfigPtr->Pin[i].pin_num);

			/* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
			SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ConfigPtr->Pin[i].pin_num);

			/* Set the PMCx bits for this pin */
			*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_ConfigPtr->Pin[i].pin_num * 4));
			*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (Port_ConfigPtr->Pin[i].mode << (Port_ConfigPtr->Pin[i].pin_num * 4));

			/* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
			SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ConfigPtr->Pin[i].pin_num);
		}
                
                
		if(Port_ConfigPtr->Pin[i].direction == PORT_PIN_OUT)
		{
				/* Set the corresponding bit in the GPIODIR register to configure it as output pin */
				SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_ConfigPtr->Pin[i].pin_num);

				if(Port_ConfigPtr->Pin[i].initial_value == PORT_PIN_LEVEL_HIGH)
				{
						/* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
						SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , Port_ConfigPtr->Pin[i].pin_num);
				}
				else
				{
						/* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
						CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , Port_ConfigPtr->Pin[i].pin_num);
				}
		}
		else if(Port_ConfigPtr->Pin[i].direction == PORT_PIN_IN)
		{
				/* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
				CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_ConfigPtr->Pin[i].pin_num);

				if(Port_ConfigPtr->Pin[i].resistor == PULL_UP)
				{
						/* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
						SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_ConfigPtr->Pin[i].pin_num);
				}
				else if(Port_ConfigPtr->Pin[i].resistor == PULL_DOWN)
				{
						/* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
						SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_ConfigPtr->Pin[i].pin_num);
				}
				else
				{
						/* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
						CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_ConfigPtr->Pin[i].pin_num);

						/* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
						CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_ConfigPtr->Pin[i].pin_num);
				}
		}
		else
		{
				/* Do Nothing */
		}
	}
}


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
                          Port_PinDirectionType Direction)
{
  volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
  #if (PORT_DEV_ERROR_DETECT == STD_ON)
		/* Check if the Driver is initialized before using this function */
		if(Port_Status == PORT_NOT_INITIALIZED)
		{
			Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIR_SID, PORT_E_UNINIT);
		}
		else
		{	/* Do Nothing */	}
                if(Pin >= PORT_CONFIGURED_PINS)
		{
			Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIR_SID, PORT_E_PARAM_PIN);
		}
		else
		{	/* Do Nothing */	}
                if(Port_ConfigPtr->Pin[Pin].pin_dir_changeable == STD_OFF)
                {
                        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIR_SID, PORT_E_DIRECTION_UNCHANGEABLE);
                }
                else
		{	/* Do Nothing */	}
  #endif
                 switch(Port_ConfigPtr->Pin[Pin].port_num)
                 {
               case  PORT_A : PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                        break;
               case  PORT_B : PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                        break;
               case  PORT_C : PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                        break;
               case  PORT_D : PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                        break;
               case  PORT_E : PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                        break;
               case  PORT_F : PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                        break;
          }
                if(Direction == PORT_PIN_IN)
                {
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_ConfigPtr->Pin[Pin].pin_num);
                }
                 else
                  {	/* Do Nothing */	}
                if(Direction == PORT_PIN_OUT)
                {
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_ConfigPtr->Pin[Pin].pin_num);
                }
                 else
                  {	/* Do Nothing */	}
}
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
void Port_RefresheportDirection(void )
{
  volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
  uint8 i=0;
  #if (PORT_DEV_ERROR_DETECT == STD_ON)
		/* Check if the Driver is initialized before using this function */
		if(Port_Status == PORT_NOT_INITIALIZED)
		{
			Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIR_SID, PORT_E_UNINIT);
		}
		else
		{	/* Do Nothing */	}
  #endif
  for(i=0;i<=PORT_CONFIGURED_PINS;i++)
  {
    switch(Port_ConfigPtr->Pin[i].port_num)
			{
				case  PORT_A : PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
				 break;
				case  PORT_B : PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
				 break;
				case  PORT_C : PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
				 break;
				case  PORT_D : PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
				 break;
				case  PORT_E : PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
				 break;
				case  PORT_F : PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
				 break;
			}
    if( (Port_ConfigPtr->Pin[i].port_num == 2) && (Port_ConfigPtr->Pin[i].pin_num <= 3)) /* PC0 to PC3 */
			{
				/* Do Nothing ...  this is the JTAG pins */
				continue;
			}
    else
			{	/* Do Nothing */	}
    if (Port_ConfigPtr->Pin[i].pin_dir_changeable == STD_OFF)
			{
				if(Port_ConfigPtr->Pin[i].direction == PORT_PIN_OUT)
				{
					/* Set the corresponding bit in the GPIODIR register to configure it as output pin */
					SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_ConfigPtr->Pin[i].pin_num);
				}
				else
				{	/* Do Nothing */	} 
                                if(Port_ConfigPtr->Pin[i].direction == PORT_PIN_IN)
				{
					/* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
					CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_ConfigPtr->Pin[i].pin_num);
				}
				else
				{	/* Do Nothing */	}
  }    
}
}

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
void Port_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
  #if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* Check if input pointer is not Null pointer */
	if(NULL_PTR == versioninfo)
	{
		/* Report to DET  */
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_POINTER);
	}
	else
#endif /* (PORT_DEV_ERROR_DETECT == STD_ON) */
	{
		/* Copy the vendor Id */
		versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
		/* Copy the module Id */
		versioninfo->moduleID = (uint16)PORT_MODULE_ID;
		/* Copy Software Major Version */
		versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
		/* Copy Software Minor Version */
		versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
		/* Copy Software Patch Version */
		versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
	}
}
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
                     Port_PinModeType Mode)
{
  volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
  #if (PORT_DEV_ERROR_DETECT == STD_ON)
		/* Check if the Driver is initialized before using this function */
		if(Port_Status == PORT_NOT_INITIALIZED)
		{
			Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIR_SID, PORT_E_UNINIT);
		}
		else
		{	/* Do Nothing */	}
                if(Pin >= PORT_CONFIGURED_PINS)
		{
			Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIR_SID, PORT_E_PARAM_PIN);
		}
		else
		{	/* Do Nothing */	}
                if(Mode > PORT_PIN_MODE_14)
		{
			Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_INVALID_MODE);
		}
		else
		{	/* Do Nothing */	}
  #endif
   switch(Port_ConfigPtr->Pin[Pin].port_num)
	{
		case  PORT_A : PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break;
		case  PORT_B : PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
		case  PORT_C : PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
		case  PORT_D : PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
		case  PORT_E : PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
		case  PORT_F : PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
	}

	if( (Port_ConfigPtr->Pin[Pin].port_num == 2) && (Port_ConfigPtr->Pin[Pin].pin_num <= 3) ) /* PC0 to PC3 */
	{
		/* Do Nothing ...  this is the JTAG pins */
		return;
	}
                if(Mode == PORT_PIN_MODE_AIN)
                {
                 /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
		CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,Port_ConfigPtr->Pin[Pin].pin_num);

		/* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
		CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ConfigPtr->Pin[Pin].pin_num);

		/* Clear the PMCx bits for this pin */
		*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_ConfigPtr->Pin[Pin].pin_num * 4));

		/* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
		SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_ConfigPtr->Pin[Pin].pin_num); 
                }
                else
		{	/* Do Nothing */	}
                if(Mode == PORT_PIN_MODE_0)
                {
                  /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
		CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_ConfigPtr->Pin[Pin].pin_num);

		/* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
		CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ConfigPtr->Pin[Pin].pin_num);

		/* Clear the PMCx bits for this pin */
		*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_ConfigPtr->Pin[Pin].pin_num * 4));

		/* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
		SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ConfigPtr->Pin[Pin].pin_num);
                }
                 else
		{	/* Do Nothing */	}
                 if((Mode >= PORT_PIN_MODE_1)&&(Mode <= PORT_PIN_MODE_14))
                 {
                   /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
		CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_ConfigPtr->Pin[Pin].pin_num);

		/* Enable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
		SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_ConfigPtr->Pin[Pin].pin_num);

		/* Set the PMCx bits for this pin */
		*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (Mode & 0x0000000F << (Port_ConfigPtr->Pin[Pin].pin_num * 4));

		/* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
		SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_ConfigPtr->Pin[Pin].pin_num);
                 }
                else
		{	/* Do Nothing */	}
}
#endif