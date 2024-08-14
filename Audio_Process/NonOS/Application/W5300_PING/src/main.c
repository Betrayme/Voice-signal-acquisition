/*************************************************************
		File name: main.c
*************************************************************/
#include "dsp28x_project.h"
#include "task.h"
#include "heart.h"
#include "socket.h"
#include "ethernet.h"

// These are defined by the linker (see F28335.cmd)
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;

void main(void)
{
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
	addTask(&w5300Task,3,10,0);
	
	/* Enable CPU INT1 which is connected to CPU-Timer 0: */
   	IER |= M_INT1;	
   	/* Enable TINT0 in the PIE: Group 1 interrupt 7 */
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	
	/* Enable global Interrupts and higher priority real-time debug events: */
   	EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM

	/* Step 5. IDLE loop. Just sit and loop forever ------------------------*/
	while(1)
   	{
    	dispatchTask();
   	}
}/* end of main() */
