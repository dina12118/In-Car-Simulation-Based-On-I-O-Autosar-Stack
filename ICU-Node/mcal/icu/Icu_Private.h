/*****************************************************************************************
 *
 * Module: Icu
 * File Name: Icu_Private.h
 * Description: Header file for TM4C123GH6PM Microcontroller - ICU Driver
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

#ifndef ICU_PRIVATE_H_
#define ICU_PRIVATE_H_


#define TIMER_A                             0
#define TIMER_B                             1
        
#define Peripherals_Base                    0xE000E000UL
/* NVIC */
#define NVIC_EN0_OFFSET                     0x00000100UL
#define NVIC_DIS0_OFFSET                    0x00000180UL
#define NVIC_PEND0_OFFSET                   0x00000200UL
#define NVIC_UNPEND0_OFFSET                 0x00000280UL
#define NVIC_ACTIVE0_OFFSET                 0x00000300UL
#define NVIC_PRI0_OFFSET                    0x00000400UL
#define SW_TRIG_OFFSET                      0x00000F00UL
        
/* Accessing All Timers Registers */        
#define TIMER(TIMER_ID, REG_OFFSET)         (*((volatile uint32*) (0x40030000UL + (TIMER_ID > 7 ? (TIMER_ID + 4)<<12 : TIMER_ID<<12) + REG_OFFSET)))

/* Accessing All NVIC Registers */
#define NVIC_OP(INT_ID, NVIC_OP_OFFSET)     (*(volatile uint32*)(((INT_ID/32) * 4) + NVIC_OP_OFFSET + Peripherals_Base)) |= (1 << ((INT_ID) % 32))
/* Getting Active state or Pending state */
#define NVIC_GET_STATUS(INT_ID, NVIC_OP_OFFSET)     (*(volatile uint32*)(((INT_ID/32) * 4) + NVIC_OP_OFFSET + Peripherals_Base)) & (1 << ((INT_ID) % 32))

/* 16/32-Bit General-Purpose Timer Run Mode Clock Gating Control (RCGCTIMER), offset 0x604 */
#define     RCGCTIMER       (*((volatile uint32*) 0x400FE604))
/* 32/64-Bit Wide General-Purpose Timer Run Mode Clock Gating Control (RCGCWTIMER), offset 0x65C */
#define     RCGCWTIMER      (*((volatile uint32*) 0x400FE65C))
#define     RCGC1           (*((volatile uint32*) 0x400FE104))
/* */
#define     SCGC1           (*((volatile uint32*) 0x400FE114))
/* General Purpose Timer Config Register */
#define     GPTMCFG_OFFSET  0x000UL
/* General Purpose Timer Control Register */
#define     GPTMCTL_OFFSET  0x00CUL
#define     TAEN            0
#define     TASTALL         1
#define     TAEVENT_0       2
#define     TAEVENT_1       3
#define     RTCEN           4
#define     TAOTE           5
#define     TAPWML          6
#define     TBEN            8
#define     TBSTALL         9
#define     TBEVENT_0       10
#define     TBEVENT_1       11
#define     TBOTE           13
#define     TBPWML          14
/*  */
#define     GPTMTAMR_OFFSET 0x004UL
#define     TAPLO           11
#define     TAMRSU          10
#define     TAPWMIE         9
#define     TAILD           8
#define     TASNAPS         7
#define     TAWOT           6
#define     TAMIE           5
#define     TACDIR          4
#define     TAAMS           3
#define     TACMR           2
#define     TAMR_1          1
#define     TAMR_0          0     
/*  */
#define     GPTMTBMR_OFFSET 0x008UL
#define     TBPLO           11
#define     TBMRSU          10
#define     TBPWMIE         9
#define     TBILD           8
#define     TBSNAPS         7
#define     TBWOT           6
#define     TBMIE           5
#define     TBCDIR          4
#define     TBAMS           3
#define     TBCMR           2
#define     TBMR_1          1
#define     TBMR_0          0      
/*  */
#define     GPTMIMR_OFFSET  0x018UL
#define     WUEIM           16
#define     TBMIM           11
#define     CBEIM           10
#define     CBMIM           9
#define     TBTOIM          8
#define     TAMIM           4
#define     RTCIM           3
#define     CAEIM           2
#define     CAMIM           1
#define     TATOIM          0
/*  */
#define     GPTMRIS_OFFSET  0x01CUL 
#define     CBERIS          10
#define     CAERIS          2
/*  */                       
#define     GPTMMIS_OFFSET  0x020UL
/*  */                       
#define     GPTMICR_OFFSET  0x024UL
#define     CBECINT         10
#define     CAECINT         2
/*  */
#define     GPTMTAPR_OFFSET 0x038UL
/*  */
#define     GPTMTBPR_OFFSET 0x03CUL
/*  */
#define     GPTMTAR_OFFSET  0x048UL
/*  */
#define     GPTMTBR_OFFSET  0x04CUL
/*  */
#define     GPTMTAV_OFFSET  0x050UL
/*  */
#define     GPTMTBV_OFFSET  0x054UL
/* */
#define     GPTMTAMATCHR_OFFSET 0x030UL
/* */
#define     GPTMTBMATCHR_OFFSET 0x034UL
/* */
#define     GPTMTAPMR_OFFSET    0x040UL
/* */
#define     GPTMTAILR_OFFSET    0x028UL
/* */
#define     GPTMTBILR_OFFSET    0x02CUL

#endif /* ICU_PRIVATE_H_ */