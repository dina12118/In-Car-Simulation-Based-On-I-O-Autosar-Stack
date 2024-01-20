
#define 	HWREG(x)   (*((volatile uint32_t *)(x)))


#define Timer_0_base						((volatile uint32*)0x40030000)
#define Timer_1_base						((volatile uint32*)0x40031000)
#define Timer_2_base						((volatile uint32*)0x40032000)
#define Timer_3_base						((volatile uint32*)0x40033000)
#define Timer_4_base						((volatile uint32*)0x40034000)
#define Timer_5_base						((volatile uint32*)0x40035000)
#define Wide_Timer_0_base                 	((volatile uint32*)0x40036000)
#define Wide_Timer_1_base                 	((volatile uint32*)0x40037000)
#define Wide_Timer_2_base                   ((volatile uint32*)0x4004C000)
#define Wide_Timer_3_base                   ((volatile uint32*)0x4004D000)
#define Wide_Timer_4_base                   ((volatile uint32*)0x4004E000)
#define Wide_Timer_5_base                   ((volatile uint32*)0x4004F000)


#define OCU_Timer							Timer_0_base


#define GPTMCFG                     *((volatile uint32*)(OCU_Timer+0x000) )

#define GPTMTAMR					*((volatile uint32*)(OCU_Timer+0x004) )
#define TAMR0						0
#define TAMR1						1
#define TAMCR						2
#define TACDIR						4	// CLR for counting down		
#define TAMIE						5   //set for enabling match interrupt
  
#define GPTMTBMR                    *((volatile uint32*)(OCU_Timer+0x008) )
#define TBMR0						0
#define TBMR1						1
#define TBMCR						2
#define TBCDIR						4	// CLR for counting down
#define TBMIE						5   //set for enabling match interrupt

/* Control Register */
#define GPTMCTL                     *((volatile uint32*)(OCU_Timer+0x00C) )
#define TAEN						0
#define TBEN						8
#define TAEVENT0					2
#define TAEVENT1					3
#define TBEVENT0					10
#define TBEVENT1					11


  
#define GPTMSYNC                    *((volatile uint32*)(OCU_Timer+0x010) )

/*Interrupt mask register*/    
#define GPTMIMR                     *((volatile uint32*)(OCU_Timer+0x018) )
#define CAMIM						1
#define CBMIM						9
#define TAMIM						4
#define TBMIM						11


#define GPTMRIS                     *((volatile uint32*)(OCU_Timer+0x01C) )  
#define GPTMMIS                     *((volatile uint32*)(OCU_Timer+0x020) )
#define GPTMICR                     *((volatile uint32*)(OCU_Timer+0x024) ) 
#define GPTMTAILR                   *((volatile uint32*)(OCU_Timer+0x028) )
#define GPTMTBILR                   *((volatile uint32*)(OCU_Timer+0x02C) ) 
#define GPTMTAMATCHR                *((volatile uint32*)(OCU_Timer+0x030) )     
#define GPTMTBMATCHR                *((volatile uint32*)(OCU_Timer+0x034) )        
#define GPTMTAPR                    *((volatile uint32*)(OCU_Timer+0x038) )            
#define GPTMTBPR                    *((volatile uint32*)(OCU_Timer+0x03C) )          
#define GPTMTAPMR                   *((volatile uint32*)(OCU_Timer+0x040) )          
#define GPTMTBPMR                   *((volatile uint32*)(OCU_Timer+0x044) )                 
#define GPTMTAR                     *((volatile uint32*)(OCU_Timer+0x048) )               
#define GPTMTBR                     *((volatile uint32*)(OCU_Timer+0x04C) )             
#define GPTMTAV                     *((volatile uint32*)(OCU_Timer+0x050) )               
#define GPTMTBV                     *((volatile uint32*)(OCU_Timer+0x054) )              
#define GPTMRTCPD                   *((volatile uint32*)(OCU_Timer+0x058) )                 
#define GPTMTAPS                    *((volatile uint32*)(OCU_Timer+0x05C) )                   
#define GPTMTBPS                    *((volatile uint32*)(OCU_Timer+0x060) )
#define GPTMTAPV                    *((volatile uint32*)(OCU_Timer+0x064) )
#define GPTMTBPV                    *((volatile uint32*)(OCU_Timer+0x068) )
#define GPTMPP                      *((volatile uint32*)(OCU_Timer+0xFC0) )

//------------------------------------------------------------------------------------------
/* 16/32-Bit General-Purpose Timer Run Mode Clock Gating Control (RCGCTIMER), offset 0x604 */
#define     RCGCTIMER       (*((volatile uint32*) 0x400FE604))
/* 32/64-Bit Wide General-Purpose Timer Run Mode Clock Gating Control (RCGCWTIMER), offset 0x65C */
#define     RCGCWTIMER      (*((volatile uint32*) 0x400FE65C))

//-------------------------------------------------------------------------------------------


typedef struct {
	uint32  CFG;
	uint32  TAMR;
	uint32  TBMR;
	uint32  CTL;
	uint32  SYNC;
	uint32  RESERVED;
	uint32  IMR;
	uint32  RIS;
	uint32  MIS;
	uint32  ICR;
	uint32  TAILR;
	uint32  TBILR;
	uint32  TAMATCHR;
	uint32  TBMATCHR;
	uint32  TAPR;
	uint32  TBPR;
	uint32  TAPMR;
	uint32  TBPMR;
	uint32  TAR;
	uint32  TBR;
	uint32  TAV;
	uint32  TBV;
	uint32  RTCPD;
	uint32  TAPS;
	uint32  TBPS;
	
//#if (halCONFIG_1294 == 1)
	uint32  RESERVED1[2];
	uint32  DMAEV;
	uint32  ADCEV;
	uint32  RESERVED2[979];
	uint32  PP;
	uint32  RESERVED3;
	uint32  CC;
//#else // TM4C123
	uint32  TAPV;
	uint32  TBPV;
	uint32  RESERVED1[981];
	uint32  PP;
//#endif

} TimerRegisters;



const volatile uint32 * TimerBaseAddress[TIMER_MAX_BLOCKS] = {
	Timer_0_base	
    Timer_1_base	
    Timer_2_base	
    Timer_3_base	
    Timer_4_base	
    Timer_5_base	
    Wide_Timer_0_base
    Wide_Timer_1_base
    Wide_Timer_2_base
    Wide_Timer_3_base
    Wide_Timer_4_base
    Wide_Timer_5_base  };
	
	