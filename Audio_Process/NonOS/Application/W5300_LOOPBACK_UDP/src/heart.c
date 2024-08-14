/*************************************************************
		File name: heart.c
*************************************************************/
#include "dsp28x_project.h"
#include "heart.h"

void initHeart(void)
{
	EALLOW;
	GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
	EDIS;	
}/* end initHeart() */

void heartTask(void)
{	
	GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;
}/* end of heartTask() */

/*** END ****************************************************/
