/*****************************************************************************************
 *
 * Module: Adc
 * File Name: Adc_Regs.h
 * Description: Header file for TM4C123GH6PM Microcontroller - ADC Driver
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

#ifndef ADC_REG_H
#define ADC_REG_H


/* ADC Registers Base Addresses */
#define ADC_BASE_ADDRESS       0x40038000U

#define ADC0_BASE_ADDRESS      0x40038000U
#define ADC1_BASE_ADDRESS      0x40039000U


/*	Analog-to-Digital Converter Run Mode Clock Gating Control */
#define RCGCADC 				(*((volatile uint32*)0x400FE638U))		

/*	General-Purpose Input/Output Peripheral Ready */
#define PRGPIO 					(*((volatile uint32*)0x400FEA08U))


/* ADC Registers offset addresses */

/* ADC Active Sample Sequencer */
#define ADCACTSS               0x000

/* ADC Raw Interrupt Status */
#define ADCRIS                 0x004

/* ADC Interrupt Mask */
#define ADCIM                  0x008

/* ADC Interrupt Status and Clear */
#define ADCISC                 0x00C

/* ADC Overflow Status */
#define ADCOSTAT               0x010

/* ADC Event Multiplexer Select */
#define ADCEMUX                0x014

/* ADC Underflow Status */
#define ADCUSTAT               0x018

/* ADC Trigger Source Select */
#define ADCTSSEL               0x01C

/* ADC Sample Sequencer Priority */
#define ADCSSPRI               0x020

/* ADC Sample Phase Control  */
#define  ADCSPC                0x024

/* ADC Processor Sample Sequence Initiate */
#define ADCPSSI                0x028

/* ADC Sample Averaging Control */
#define ADCSAC                 0x030

/* ADC Digital Comparator Interrupt Status and Clear */
#define  ADCDCISC              0x034

/* ADC Control */
#define   ADCCTL               0x038

/* ADC Sample Sequence Input Multiplexer Select 0 */
#define  ADCSSMUX0             0x040

/* ADC Sample Sequence Control 0 */
#define ADCSSCTL0              0x044

/* ADC Sample Sequence Result FIFO 0 */
#define  ADCSSFIFO0            0x048

/* ADC Sample Sequence FIFO 0 Status */
#define ADCSSFSTAT0            0x04C

/* ADC Sample Sequence 0 Operation */
#define ADCSSOP0               0x050

/* ADC Sample Sequence 0 Digital Comparator Select */
#define ADCSSDC0               0x054

/* ADC Sample Sequence Input Multiplexer Select 1 */
#define ADCSSMUX1              0x060

/* ADC Sample Sequence Control 1  */
#define ADCSSCTL1              0x064

/* ADC Sample Sequence Result FIFO 1 */
#define ADCSSFIFO1             0x068

/*  ADC Sample Sequence FIFO 1 Status */
#define ADCSSFSTAT1            0x06C

/* ADC Sample Sequence 1 Operation */
#define ADCSSOP1               0x070

/* ADC Sample Sequence 1 Digital Comparator Select */
#define ADCSSDC1               0x074

/* ADC Sample Sequence Input Multiplexer Select 2 */
#define ADCSSMUX2              0x080

/* ADC Sample Sequence Control 2  */
#define ADCSSCTL2              0x084

/* ADC Sample Sequence Result FIFO 2 */
#define ADCSSFIFO2             0x088

/*  ADC Sample Sequence FIFO 2 Status */
#define ADCSSFSTAT2            0x08C

/* ADC Sample Sequence 2 Operation */
#define ADCSSOP2               0x090

/* ADC Sample Sequence 2 Digital Comparator Select */
#define ADCSSDC2               0x094

/* ADC Sample Sequence Input Multiplexer Select 3 */
#define ADCSSMUX3              0x0A0

/* ADC Sample Sequence Control 3  */
#define ADCSSCTL3              0x0A4

/* ADC Sample Sequence Result FIFO 3 */
#define ADCSSFIFO3             0x0A8

/*  ADC Sample Sequence FIFO 3 Status */
#define ADCSSFSTAT3            0x0AC

/* ADC Sample Sequence 3 Operation */
#define ADCSSOP3               0x0B0

/* ADC Sample Sequence 3 Digital Comparator Select */
#define ADCSSDC3               0x0B4

/* ADC Digital Comparator Reset Initial Conditions */
#define ADCDCRIC               0xD00

/* ADC Digital Comparator Control 0 */
#define ADCDCCTL0              0xE00

/* ADC Digital Comparator Control 1 */
#define ADCDCCTL1              0xE04

/* ADC Digital Comparator Control 2 */
#define ADCDCCTL2              0xE08

/* ADC Digital Comparator Control 3 */
#define ADCDCCTL3              0xE0C

/* ADC Digital Comparator Control 4 */
#define ADCDCCTL4              0xE10

/* ADC Digital Comparator Control 5 */
#define ADCDCCTL5              0xE14

/* ADC Digital Comparator Control 6 */
#define ADCDCCTL6              0xE18

/* ADC Digital Comparator Control 7 */
#define ADCDCCTL7              0xE1C

/* ADC Digital Comparator Range 0 */
#define ADCDCCMP0              0xE40

/* ADC Digital Comparator Range 1 */
#define ADCDCCMP1              0xE44

/* ADC Digital Comparator Range 2 */
#define ADCDCCMP2              0xE48

/* ADC Digital Comparator Range 3 */
#define ADCDCCMP3              0xE4C

/* ADC Digital Comparator Range 4 */
#define ADCDCCMP4              0xE50

/* ADC Digital Comparator Range 5 */
#define ADCDCCMP5              0xE54

/* ADC Digital Comparator Range 6 */
#define ADCDCCMP6              0xE58

/* ADC Digital Comparator Range 7 */
#define ADCDCCMP7              0xE5C

/* ADC Peripheral Properties  */
#define ADCPP                  0xFC0

/* ADC Peripheral Configuration */
#define ADCPC                  0xFC4

/* ADC Clock Configuration */
#define ADCCC                  0xFC8




#endif 