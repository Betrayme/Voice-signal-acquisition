/*************************************************************
		File name: ethernet.c
*************************************************************/
#include "dsp28x_project.h"
#include "types.h"
#include "socket.h"
#include "ethernet.h"



/* Global variables */
uint8 tx_mem_conf[8] = {8,8,8,8,8,8,8,8};	// for setting TMSR regsiter
uint8 rx_mem_conf[8] = {8,8,8,8,8,8,8,8};   // for setting RMSR regsiter   
uint8 data[5000];
uint8 *data_buf = &data[0];    				// buffer for loopack data   
uint8 ip[4] = {192,168,1,40};            // for setting SIP register
uint8 gw[4] = {192,168,1,1 };              // for setting GAR register
uint8 sn[4] = {255,255,255,0};              // for setting SUBR register
uint8 mac[6] = {0x00,0x08,0xDC,0x01,0x02,0x03};   // for setting SHAR register

void w5300Task(void)
{
	static Uint16 w5300Ticks = 0,cnt = 0;
	
	switch(w5300Ticks){
	case 0:			
		W5300_RST_ON;	
		w5300Ticks = 1;
		break;
		
	case 1:	/* initiate W5300 */
		W5300_RST_OFF;
		//iinchip_init();			
			
		w5300Ticks = 2;
		break;
				
	case 2:			
		cnt++;
		if(cnt > 9) w5300Ticks = 3;
		break;
		
	case 3:	/* allocate internal TX/RX memory of w5300 */
		if(sysinit(tx_mem_conf,rx_mem_conf)) w5300Ticks = 4;			
		break;			
	
	case 4:	/* configure network information */
		setMR(getMR() | MR_RDH | MR_WDF(2));
		
		cnt = 0;
		w5300Ticks = 5;
		break;
		
	case 5:
		cnt++;
		if(cnt > 9) w5300Ticks = 6;
		break;
		
	case 6:
   		setSHAR(&mac[0]);	/* set source hardware address */   			
      	setGAR(&gw[0]);     /* set gateway IP address */		 
      	setSUBR(&sn[0]);	/* set subnet mask address */		
      	setSIPR(&ip[0]);    /* set source IP address */ 
		   
   		w5300Ticks = 7;
		break;	
			
	case 7:
		/* maybe set breakpoint at HERE */
		
		break;	
	}
}/* end of w5300Task() */

void initW5300(void)
{
	initW5300Gpio();
	initW5300Xintf();
}/* end of initW5300() */

void initW5300Gpio(void)
{
	/* config extend interface -------------------------------------------*/
  	EALLOW;
    GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 3;  // XD15
    GpioCtrlRegs.GPCMUX1.bit.GPIO65 = 3;  // XD14
    GpioCtrlRegs.GPCMUX1.bit.GPIO66 = 3;  // XD13
    GpioCtrlRegs.GPCMUX1.bit.GPIO67 = 3;  // XD12
    GpioCtrlRegs.GPCMUX1.bit.GPIO68 = 3;  // XD11
    GpioCtrlRegs.GPCMUX1.bit.GPIO69 = 3;  // XD10
    GpioCtrlRegs.GPCMUX1.bit.GPIO70 = 3;  // XD19
    GpioCtrlRegs.GPCMUX1.bit.GPIO71 = 3;  // XD8
    GpioCtrlRegs.GPCMUX1.bit.GPIO72 = 3;  // XD7
    GpioCtrlRegs.GPCMUX1.bit.GPIO73 = 3;  // XD6
    GpioCtrlRegs.GPCMUX1.bit.GPIO74 = 3;  // XD5
    GpioCtrlRegs.GPCMUX1.bit.GPIO75 = 3;  // XD4
    GpioCtrlRegs.GPCMUX1.bit.GPIO76 = 3;  // XD3
    GpioCtrlRegs.GPCMUX1.bit.GPIO77 = 3;  // XD2
    GpioCtrlRegs.GPCMUX1.bit.GPIO78 = 3;  // XD1
    GpioCtrlRegs.GPCMUX1.bit.GPIO79 = 3;  // XD0
    	
    GpioCtrlRegs.GPBMUX1.bit.GPIO40 = 3;  // XA0/XWE1n
    GpioCtrlRegs.GPBMUX1.bit.GPIO41 = 3;  // XA1
    GpioCtrlRegs.GPBMUX1.bit.GPIO42 = 3;  // XA2
    GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 3;  // XA3
    GpioCtrlRegs.GPBMUX1.bit.GPIO44 = 3;  // XA4
    GpioCtrlRegs.GPBMUX1.bit.GPIO45 = 3;  // XA5
    GpioCtrlRegs.GPBMUX1.bit.GPIO46 = 3;  // XA6
    GpioCtrlRegs.GPBMUX1.bit.GPIO47 = 3;  // XA7

    GpioCtrlRegs.GPCMUX2.bit.GPIO80 = 3;  // XA8
   	GpioCtrlRegs.GPCMUX2.bit.GPIO81 = 3;  // XA9
    GpioCtrlRegs.GPCMUX2.bit.GPIO82 = 3;  // XA10
    GpioCtrlRegs.GPCMUX2.bit.GPIO83 = 3;  // XA11
    GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 3;  // XA12
    GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 3;  // XA13
         	
    GpioCtrlRegs.GPBMUX1.bit.GPIO38 = 3;  // XWE0
    GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 3;  // XZCS0     	
    EDIS;     	    	
     	
    /* W5300 reset pin -> GPIO63 as output pin  ----------------------------*/
    EALLOW;
    GpioCtrlRegs.GPBPUD.bit.W5300_RST_Pin = 0;
    GpioCtrlRegs.GPBMUX2.bit.W5300_RST_Pin = 0; /* set as gpio */    
    GpioCtrlRegs.GPBDIR.bit.W5300_RST_Pin = 1;	/* output */
    EDIS;    	
     	
    W5300_RST_OFF;
     	
    /* w5300 interrupt Pin -> GPIO35 ----------------------------------------------*/
    EALLOW;
    GpioCtrlRegs.GPBPUD.bit.W5300_INT_Pin = 0;
	GpioCtrlRegs.GPBMUX1.bit.W5300_INT_Pin = 0;	/* set as gpio */
	GpioCtrlRegs.GPBDIR.bit.W5300_INT_Pin = 0;	/* input */
	EDIS;
}/* end of initW5300Gpio() */

void initW5300Xintf(void)
{
  	// All Zones---------------------------------
    // Timing for all zones based on XTIMCLK = 1/2 SYSCLKOUT
    EALLOW;
    XintfRegs.XINTCNF2.bit.XTIMCLK = 1;
    // No write buffering
    XintfRegs.XINTCNF2.bit.WRBUFF = 0;
    // XCLKOUT is diabled
    XintfRegs.XINTCNF2.bit.CLKOFF = 1;
    // XCLKOUT = XTIMCLK/2
    XintfRegs.XINTCNF2.bit.CLKMODE = 1;
    
  	/* Zone 0 ---------------------------------------------------------------*/
    // When using ready, ACTIVE must be 1 or greater
    // Lead must always be 1 or greater
    // Zone write timing
    XintfRegs.XTIMING0.bit.XWRLEAD = 3;
    XintfRegs.XTIMING0.bit.XWRACTIVE = 7;
    XintfRegs.XTIMING0.bit.XWRTRAIL = 3;
    // Zone read timing
    XintfRegs.XTIMING0.bit.XRDLEAD = 3;
    XintfRegs.XTIMING0.bit.XRDACTIVE = 7;
    XintfRegs.XTIMING0.bit.XRDTRAIL = 3;

    // double all Zone read/write lead/active/trail timing
    XintfRegs.XTIMING0.bit.X2TIMING = 0;

    // Zone will NOT sample XREADY signal
    XintfRegs.XTIMING0.bit.USEREADY = 0;
   	XintfRegs.XTIMING0.bit.READYMODE = 0;  // sample asynchronous

    // Size must be either:
    // 0,1 = x32 or
    // 1,1 = x16 other values are reserved
    XintfRegs.XTIMING0.bit.XSIZE = 3;    
    
    // Bank switching
    // Assume Zone 0 is slow, so add additional BCYC cycles
    // when ever switching from Zone 0 to another Zone.
    // This will help avoid bus contention.
    XintfRegs.XBANK.bit.BANK = 0;
    XintfRegs.XBANK.bit.BCYC = 3;
    
    EDIS;
    
    asm(" RPT #7 || NOP");
}/* end of initW5300Xintf() */
