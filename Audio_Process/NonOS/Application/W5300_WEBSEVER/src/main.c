/*************************************************************
		File name: main.c
*************************************************************/
#include "dsp28x_project.h"
#include "task.h"
#include "heart.h"
#include "socket.h"
#include "ethernet.h"

#define INT_N  GpioDataRegs.GPADAT.bit.W5300_INT_Pin

// These are defined by the linker (see F28335.cmd)
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;

void     loopback_tcps(SOCKET s, uint16 port, uint8* buf, uint16 mode);
void     loopback_tcpc(SOCKET s, uint8* addr, uint16 port, uint8* buf,uint16 mode);
void     loopback_udp(SOCKET s, uint16 port, uint8* buf, uint16 mode);

int flag=0;

uint8 temp[]={
		"<html>"
		"<head>"
		"<meta http-equiv='content-type' content='text/html; charset=ksc5601' />"
		"<TITLE>HTTP</TITLE>"
		"</head>"
		"<body >"
		"<DIV>"
			"<p align='center'><b><font color='#999999' face='Times New Roman' size='5'>"
			"<br>"
			"<font color=\"#0000FF\">TL28335-W5300-WEBSEVER DEMO </font></font></b></DIV>"
			"<p align='center'><b><font color='#FF0000' face='Times New Roman' size='5'>"
			"<br> "
			"www.tronlong.com/ </font></b>"
			"<DIV>"
		" <form name='form2' method='get' action='LEDCTL.CGI'> "
		"<HR align=\"center\" style='COLOR: #EB9243; BACKGROUND-COLOR: #EB9243'>"
		"<p align='center'><br>"
		"<p align=\"left\">"
			"<input type=\"radio\" name=\"led\" value=\"1\">"
			"LED1</label>"
			"<input type=\"radio\" name=\"led\" value=\"2\">"
			"LED2</label>"
			"<input type=\"radio\" name=\"led\" value=\"3\">"
			"LED3</label>"
			"<input type=\"radio\" name=\"led\" value=\"4\">"
			"LED4</label>"
		"<p align='center'>"
			"<input name=\"submit\" type='submit' style='float: left' value='LED Control'>"
			"</DIV>"
				"<HR style='COLOR: #EB9243; BACKGROUND-COLOR: #EB9243'>"
		"</html>"
};
uint8 temp1[sizeof(temp)];

void main(void)
{
	uint8 data_buf[500]={0};

	uint16 p,q;
	/* Step 1. Initialize System Control ------------------------------------*/
  	InitSysCtrl();

	/* Step 2. Clear all interrupts and initialize PIE vector table ---------*/	
   	DINT;	/* Disable CPU interrupts */	   	
   	
	/* Initialize the PIE control registers to their default state. */
	InitPieCtrl();
	/* Disable CPU interrupts and clear all CPU interrupt flags: */
   	IER = 0x0000;
   	IFR = 0x0000;
	/* Initialize the PIE vector table with pointers to the shell Interrupt */	
	InitPieVectTable();	/* Service Routines (ISR). */
	
	// Copy time critical code and Flash setup code to RAM
	// The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
	// symbols are created by the linker. Refer to the F28335.cmd file.
   	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

	// Call Flash Initialization to setup flash waitstates
	// This function must reside in RAM
   	InitFlash();
   	
	/* Step 3. Initialize the Device Peripheral. -----------------------------*/ 
	taskInit();		/* init time-tick tasks */
	
	initHeart();	
	initW5300();
	q=sizeof(temp);
	for(p=0;p<(q/2);p++)
	{
		temp1[p*2]=temp[p*2]*256+temp[p*2+1];
	}
	if((q%2)==1)
	{
		temp1[q-1]=temp[q-1]*256;
	}

	/* This function can be found in DSP2833x_CpuTimers.c */
   	InitCpuTimers();   
   	/* 
   	 * For this example, only initialize the Cpu Timers	 
   	 * Configure CPU-Timer 0 to interrupt every 1 milliseconds:	 
   	 * 150MHz CPU Freq, 1 millisecond Period (in uSeconds)
   	 */
   	ConfigCpuTimer(&CpuTimer0, 150, 1000);
	/* 
	 * To ensure precise timing, use write-only instructions 
	 * to write to the entire register. Therefore, if any of the configuration 
	 * bits are changed in ConfigCpuTimer 
	 * and InitCpuTimers (in DSP2833x_CpuTimers.h), the below settings must 
	 * also be updated.
	 */
	/* Use write-only instruction to set TSS bit = 0 */
   	CpuTimer0Regs.TCR.all = 0x4001;
   	/* Interrupts that are used in this example are re-mapped to
   	 * ISR functions found within this file.
   	 */
   	/* This is needed to write to EALLOW protected registers */
   	EALLOW;  
   	PieVectTable.TINT0 = &taskUpdate;
   	/* This is needed to disable write to EALLOW protected registers */
   	EDIS;       	
	/* Step 4. User specific code, enable interrupts: -----------------------*/
	addTask(&heartTask,0,500,0);
	dispatchTask();
//	iinchip_init();
	addTask(&w5300Task,3,10,0);
	/* Enable CPU INT1 which is connected to CPU-Timer 0: */
   	IER |= M_INT1;	
   	/* Enable TINT0 in the PIE: Group 1 interrupt 7 */
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	/* Enable global Interrupts and higher priority real-time debug events: */
   	EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM

	EALLOW;
	GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;  // GPIO50
	GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0;  // GPIO51
	GpioCtrlRegs.GPBMUX2.bit.GPIO52 = 0;  // GPIO52
	GpioCtrlRegs.GPBMUX2.bit.GPIO53 = 0;  // GPIO53

    GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;   // output
    GpioCtrlRegs.GPBDIR.bit.GPIO51 = 1;   // output
    GpioCtrlRegs.GPBDIR.bit.GPIO52 = 1;   // output
    GpioCtrlRegs.GPBDIR.bit.GPIO53 = 1;   // output
    EDIS;

	/* Step 5. IDLE loop. Just sit and loop forever ------------------------*/
    dispatchTask();
	while(1)
   	{
		loopback_tcps(1,80,data_buf,0);					//OK

   	}
}/* end of main() */

/**
 * "TCP SERVER" loopback program.
 */
uint8 da[500];
void     loopback_tcps(SOCKET s, uint16 port, uint8* buf, uint16 mode)
{
   uint32 len,i;
   switch(getSn_SSR(s))                // check SOCKET status
   {                                   // ------------
      case SOCK_ESTABLISHED:           // ESTABLISHED?
         if(getSn_IR(s) & Sn_IR_CON)   // check Sn_IR_CON bit
         {
        	 	flag=1;
   //       printf("%d : Connect OK\r\n",s);
            setSn_IR(s,Sn_IR_CON);     // clear Sn_IR_CON
         }
         if((len=getSn_RX_RSR(s)) > 0) // check the size of received data
         {
            len = recv(s,buf,len);     // recv
            for(i=0;i<len;)
            {
            	da[i] = (buf[i]>>8)&0xff;
            	i++;
            	da[i] = buf[i-1] &0xff;
            	i++;
            }

            if(da[20] == '1')
            {
				GpioDataRegs.GPBSET.bit.GPIO50 = 1;
				GpioDataRegs.GPBCLEAR.bit.GPIO51 = 1;
				GpioDataRegs.GPBCLEAR.bit.GPIO52 = 1;
				GpioDataRegs.GPBCLEAR.bit.GPIO53 = 1;
            }
            else if(da[20] == '2')
            {
				GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1;
				GpioDataRegs.GPBSET.bit.GPIO51 = 1;
				GpioDataRegs.GPBCLEAR.bit.GPIO52 = 1;
				GpioDataRegs.GPBCLEAR.bit.GPIO53 = 1;
            }
            else if(da[20] == '3')
            {
          	   GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1;
          	   GpioDataRegs.GPBCLEAR.bit.GPIO51 = 1;
          	   GpioDataRegs.GPBSET.bit.GPIO52 = 1;
          	   GpioDataRegs.GPBCLEAR.bit.GPIO53 = 1;
            }
            else if(da[20] == '4')
            {
          	   GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1;
          	   GpioDataRegs.GPBCLEAR.bit.GPIO51 = 1;
          	   GpioDataRegs.GPBCLEAR.bit.GPIO52 = 1;
          	   GpioDataRegs.GPBSET.bit.GPIO53 = 1;
            }
            else
            {
            	GpioDataRegs.GPBDAT.bit.GPIO50 = 0;
            	GpioDataRegs.GPBDAT.bit.GPIO51 = 0;
            	GpioDataRegs.GPBDAT.bit.GPIO52 = 0;
            	GpioDataRegs.GPBDAT.bit.GPIO53 = 0;
            	send(s,temp1, sizeof(temp));
            }
         }
         break;
                                       // ---------------
   case SOCK_CLOSE_WAIT:               // PASSIVE CLOSED
         disconnect(s);                // disconnect
         break;
                                       // --------------
   case SOCK_CLOSED:                   // CLOSED
      close(s);                        // close the SOCKET
      socket(s,Sn_MR_TCP,port,mode);   // open the SOCKET
      break;
                                       // ------------------------------
   case SOCK_INIT:                     // The SOCKET opened with TCP mode
      listen(s);                       // listen to any connection request from "TCP CLIENT"
 //     printf("%d : LOOPBACK_TCPS(%d) Started.\r\n",s,port);
      break;
   default:
      break;
   }
}
/**
 * "TCP CLIENT" loopback program.
 */
void     loopback_tcpc(SOCKET s, uint8* addr, uint16 port, uint8* buf, uint16 mode)
{
   uint32 len;
   static uint16 any_port = 1000;

   switch(getSn_SSR(s))                   // check SOCKET status
   {                                      // ------------
      case SOCK_ESTABLISHED:              // ESTABLISHED?
         if(getSn_IR(s) & Sn_IR_CON)      // check Sn_IR_CON bit
         {

 //           printf("%d : Connect OK\r\n",s);
            setSn_IR(s,Sn_IR_CON);        // clear Sn_IR_CON
         }
         if((len=getSn_RX_RSR(s)) > 0)    // check the size of received data
         {
            len = recv(s,buf,len);        // recv
            if(len !=send(s,buf,len))     // send
            {

   //            printf("%d : Send Fail.len=%d\r\n",s,len);
            }
         }
         break;
                                          // ---------------
   case SOCK_CLOSE_WAIT:                  // PASSIVE CLOSED
         disconnect(s);                   // disconnect
         break;
                                          // --------------
   case SOCK_CLOSED:                      // CLOSED
      close(s);                           // close the SOCKET
      socket(s,Sn_MR_TCP,any_port++,mode);// open the SOCKET with TCP mode and any source port number
      break;
                                          // ------------------------------
   case SOCK_INIT:                        // The SOCKET opened with TCP mode
      connect(s, addr, port);             // Try to connect to "TCP SERVER"
 //     printf("%d : LOOPBACK_TCPC(%d.%d.%d.%d:%d) Started.\r\n",s,addr[0],addr[1],addr[2],addr[3],port);
      break;
   default:
      break;
   }
}

/**
 * UDP loopback program.
 */
void     loopback_udp(SOCKET s, uint16 port, uint8* buf, uint16 mode)
{
   uint32 len;
   uint8 destip[4];
   uint16 destport;

   switch(getSn_SSR(s))
   {
                                                         // -------------------------------
      case SOCK_UDP:                                     //
         if((len=getSn_RX_RSR(s)) > 0)                   // check the size of received data
         {
            len = recvfrom(s,buf,len,destip,&destport);  // receive data from a destination
            if(len !=sendto(s,buf,len,destip,destport))  // send the data to the destination
            {
 //              printf("%d : Sendto Fail.len=%d,",s,len);
 //              printf("%d.%d.%d.%d(%d)\r\n",destip[0],destip[1],destip[2],destip[3],destport);
            }
         }
         break;
                                                         // -----------------
      case SOCK_CLOSED:                                  // CLOSED
         close(s);                                       // close the SOCKET
         socket(s,Sn_MR_UDP,port,mode);                  // open the SOCKET with UDP mode
         break;
      default:
         break;
   }
}
