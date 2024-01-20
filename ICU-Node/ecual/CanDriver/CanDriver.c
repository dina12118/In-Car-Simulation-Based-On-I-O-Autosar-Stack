/******************************************************************************
 *
 * Module: CanDriver
 *
 * File Name: CanDriver.h
 *
 * Description: Header file for Can Driver
 *
 * Author:
 ******************************************************************************/

#include "CanDriver.h"

// A counter that keeps track of the number of times the TX & RX interrupt has
// occurred, which should match the number of messages that were transmitted /
// received.
//*****************************************************************************
volatile uint32_t g_ui32RXMsgCount = 0;
volatile uint32_t g_ui32TXMsgCount = 0;

volatile bool Flag_ParkUs_DB   		= 0x00;
volatile bool Flag_Lm35_DB			= 0x00;
volatile bool Flag_AcButt_Motor		= 0x00;
volatile bool Flag_SpdEnc_DB		= 0x00;
volatile bool Flag_FrontUs_DB   	= 0x00;

// A global to keep track of the error flags that have been thrown so they may
// be processed. This is necessary because reading the error register clears
// the flags, so it is necessary to save them somewhere for processing.
volatile uint32_t g_ui32ErrFlag = 0;
volatile uint32_t tec_counter = 0;

// CAN message Objects for data being sent / received
tCANMsgObject g_sCAN0R_MsgID_ParkUs_DB;
tCANMsgObject g_sCAN0R_MsgID_Lm35_DB;
tCANMsgObject g_sCAN0R_MsgID_AcButt_Motor;
tCANMsgObject g_sCAN0R_MsgID_SpdEnc_DB;
tCANMsgObject g_sCAN0R_MsgID_FrontUs_DB;

// Variables to hold character being sent / reveived
uint8_t g_ui8RX_AcButt_Motor[8] 		= {0xFF};


// CAN 0 Interrupt Handler. It checks for the cause of the interrupt, and
// maintains a count of all messages that have been transmitted / received
void CAN0IntHandler(void)
{
	tec_counter = ( (*((volatile uint32_t *)0x40040008)) & 0xFF);
	
    uint32_t ui32Status;

    //
    // Read the CAN interrupt status to find the cause of the interrupt
    //
    // CAN_INT_STS_CAUSE register values
    // 0x0000        = No Interrupt Pending
    // 0x0001-0x0020 = Number of message object that caused the interrupt
    // 0x8000        = Status interrupt
    // all other numbers are reserved and have no meaning in this system
    //
    ui32Status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

    //
    // If this was a status interrupt acknowledge it by reading the CAN
    // controller status register.
    //
    if(ui32Status == CAN_INT_INTID_STATUS)
    {
        //
        // Read the controller status.  This will return a field of status
        // error bits that can indicate various errors. Refer to the
        // API documentation for details about the error status bits.
        // The act of reading this status will clear the interrupt.
        //
        ui32Status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);

        //
        // Add ERROR flags to list of current errors. To be handled
        // later, because it would take too much time here in the
        // interrupt.
        //
        g_ui32ErrFlag |= ui32Status;
    }

    //
    // Check if the cause is message object TXOBJECT, which we are using
    // for transmitting messages.
    //
    else if(ui32Status == ObjID_AcButt_Motor)
    {
        //
        // Getting to this point means that the TX interrupt occurred on
        // message object TXOBJECT, and the message reception is complete.
        // Clear the message object interrupt.
        //
        CANIntClear(CAN0_BASE, ObjID_AcButt_Motor);

        //
        // Increment a counter to keep track of how many messages have been
        // transmitted. In a real application this could be used to set
        // flags to indicate when a message is transmitted.
        //
        g_ui32TXMsgCount++;

        //
        // Since a message was transmitted, clear any error flags.
        // This is done because before the message is transmitted it triggers
        // a Status Interrupt for TX complete. by clearing the flag here we
        // prevent unnecessary error handling from happeneing
        //
        g_ui32ErrFlag = 0;
    }

    //
    // Otherwise, something unexpected caused the interrupt.  This should
    // never happen.
    //
    else
    {
        //
        // Spurious interrupt handling can go here.
        //
		
    }
	CANErrorHandler();
}

void InitCAN0(void)
{
    //
    // For this example CAN0 is used with RX and TX pins on port E4 and E5.
    // GPIO port E needs to be enabled so these pins can be used.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    //
    // Configure the GPIO pin muxing to select CAN0 functions for these pins.
    // This step selects which alternate function is available for these pins.
    //
    GPIOPinConfigure(GPIO_PE4_CAN0RX);										
    GPIOPinConfigure(GPIO_PE5_CAN0TX);										

    //
    // Enable the alternate function on the GPIO pins.  The above step selects
    // which alternate function is available.  This step actually enables the
    // alternate function instead of GPIO for these pins.
    //
    GPIOPinTypeCAN(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    //
    // The GPIO port and pins have been set up for CAN.  The CAN peripheral
    // must be enabled.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);

    //
    // Initialize the CAN controller
    //
    CANInit(CAN0_BASE);

    //
    // Set up the bit rate for the CAN bus.  This function sets up the CAN
    // bus timing for a nominal configuration.  You can achieve more control
    // over the CAN bus timing by using the function CANBitTimingSet() instead
    // of this one, if needed.
    // In this example, the CAN bus is set to 500 kHz.
    //
    CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 650000);
	  
    //
    // Enable interrupts on the CAN peripheral.  This example uses static
    // allocation of interrupt handlers which means the name of the handler
    // is in the vector table of startup code.
    //
    CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);

    //
    // Enable the CAN interrupt on the processor (NVIC).
    //
    IntEnable(INT_CAN0);

    //
    // Enable the CAN for operation.
    //
    CANEnable(CAN0_BASE);
	
	
	//
    // Initialize a message object to for sending AC button state
    //
    g_sCAN0R_MsgID_AcButt_Motor.ui32MsgID = MsgID_AcButt_Motor;
    g_sCAN0R_MsgID_AcButt_Motor.ui32MsgIDMask = 0x1ff;
    g_sCAN0R_MsgID_AcButt_Motor.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    g_sCAN0R_MsgID_AcButt_Motor.ui32MsgLen = sizeof(g_ui8RX_AcButt_Motor);
	g_sCAN0R_MsgID_AcButt_Motor.pui8MsgData  = g_ui8RX_AcButt_Motor;
}

// Can ERROR handling. When a message is received if there is an erro it is
// saved to g_ui32ErrFlag, the Error Flag Set. Below the flags are checked
// and cleared. It is left up to the user to add handling fuctionality if so
// desiered.
//
// For more information on the error flags please see the CAN section of the
// microcontroller datasheet.
//
// NOTE: you may experience errors during setup when only one board is powered
// on. This is caused by one board sending signals and there not being another
// board there to acknoledge it. Dont worry about these errors, they can be
// disregarded.
void CANErrorHandler(void)
{
    //
    // CAN controller has entered a Bus Off state.
    //
    if(g_ui32ErrFlag & CAN_STATUS_BUS_OFF)
    {
        //
        // Handle Error Condition here
        //
        LED_R_turnOn();
        //
        // Clear CAN_STATUS_BUS_OFF Flag
        //
        g_ui32ErrFlag &= ~(CAN_STATUS_BUS_OFF);

    }

    //
    // CAN controller error level has reached warning level.
    //
    if(g_ui32ErrFlag & CAN_STATUS_EWARN)
    {
        //
        // Handle Error Condition here
        //
        LED_R_turnOn();

        //
        // Clear CAN_STATUS_EWARN Flag
        //
        g_ui32ErrFlag &= ~(CAN_STATUS_EWARN);
    }

    //
    // CAN controller error level has reached error passive level.
    //
    if(g_ui32ErrFlag & CAN_STATUS_EPASS)
    {
        //
        // Handle Error Condition here
        //
		LED_R_turnOn();
        //
        // Clear CAN_STATUS_EPASS Flag
        //
        g_ui32ErrFlag &= ~(CAN_STATUS_EPASS);
    }

    //
    // A message was received successfully since the last read of this status.
    //
    if(g_ui32ErrFlag & CAN_STATUS_RXOK)
    {
        //
        // Handle Error Condition here
        //
		LED_B_turnOn();
        //
        // Clear CAN_STATUS_RXOK Flag
        //
        g_ui32ErrFlag &= ~(CAN_STATUS_RXOK);
    }

    //
    // A message was transmitted successfully since the last read of this
    // status.
    //
    if(g_ui32ErrFlag & CAN_STATUS_TXOK)
    {
        //
        // Handle Error Condition here
        //
		
        LED_G_turnOn();
        //
        // Clear CAN_STATUS_TXOK Flag
        //
        g_ui32ErrFlag &= ~(CAN_STATUS_TXOK);
    }

    //
    // This is the mask for the last error code field.
    //
    if(g_ui32ErrFlag & CAN_STATUS_LEC_MSK)
    {
        //
        // Handle Error Condition here
        //

        //
        // Clear CAN_STATUS_LEC_MSK Flag
        //
        g_ui32ErrFlag &= ~(CAN_STATUS_LEC_MSK);
    }

    //
    // A bit stuffing error has occurred.
    //
    if(g_ui32ErrFlag & CAN_STATUS_LEC_STUFF)
    {
        //
        // Handle Error Condition here
        //
        LED_G_turnOn();
        //
        // Clear CAN_STATUS_LEC_STUFF Flag
        //
        g_ui32ErrFlag &= ~(CAN_STATUS_LEC_STUFF);
    }

    //
    // A formatting error has occurred.
    //
    if(g_ui32ErrFlag & CAN_STATUS_LEC_FORM)
    {
        //
        // Handle Error Condition here
        //
        LED_G_turnOn();
        //
        // Clear CAN_STATUS_LEC_FORM Flag
        //
        g_ui32ErrFlag &= ~(CAN_STATUS_LEC_FORM);
    }

    //
    // An acknowledge error has occurred.
    //
    if(g_ui32ErrFlag & CAN_STATUS_LEC_ACK)
    {
        //
        // Handle Error Condition here
        //
        LED_G_turnOn();
        //
        // Clear CAN_STATUS_LEC_ACK Flag
        //
        g_ui32ErrFlag &= ~(CAN_STATUS_LEC_ACK);
    }

    //
    // The bus remained a bit level of 1 for longer than is allowed.
    //
    if(g_ui32ErrFlag & CAN_STATUS_LEC_BIT1)
    {
        //
        // Handle Error Condition here
        //
        LED_G_turnOn();
        //
        // Clear CAN_STATUS_LEC_BIT1 Flag
        //
        g_ui32ErrFlag &= ~(CAN_STATUS_LEC_BIT1);
    }

    //
    // The bus remained a bit level of 0 for longer than is allowed.
    //
    if(g_ui32ErrFlag & CAN_STATUS_LEC_BIT0)
    {
        //
        // Handle Error Condition here
        //
        LED_G_turnOn();
        //
        // Clear CAN_STATUS_LEC_BIT0 Flag
        //
        g_ui32ErrFlag &= ~(CAN_STATUS_LEC_BIT0);
    }

    //
    // A CRC error has occurred.
    //
    if(g_ui32ErrFlag & CAN_STATUS_LEC_CRC)
    {
        //
        // Handle Error Condition here
        //
        LED_G_turnOn();
        //
        // Clear CAN_STATUS_LEC_CRC Flag
        //
        g_ui32ErrFlag &= ~(CAN_STATUS_LEC_CRC);
    }

    //
    // This is the mask for the CAN Last Error Code (LEC).
    //
    if(g_ui32ErrFlag & CAN_STATUS_LEC_MASK)
    {
        //
        // Handle Error Condition here
        //
	
        //
        // Clear CAN_STATUS_LEC_MASK Flag
        //
        g_ui32ErrFlag &= ~(CAN_STATUS_LEC_MASK);
    }

    //
    // If there are any bits still set in g_ui32ErrFlag then something unhandled
    // has happened. Print the value of g_ui32ErrFlag.
    //
    if(g_ui32ErrFlag !=0)
    {
		LED_G_turnOn();
    }
}
