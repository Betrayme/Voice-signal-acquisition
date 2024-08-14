/*
 * touch.h
 *
 *  Created on: 2015-11-27
 *      Author: daogu
 */

#ifndef TOUCH_H_
#define TOUCH_H_

#define PEN GpioDataRegs.GPADAT.bit.GPIO13

extern void touch_setup(void);
extern void touch_isr(void);
extern void touch_init_xy(void);
extern Uint16 TouchDetect(void);
extern int Read_touch_ad(void);
extern void tsc2046ReadAxis(char mode, int*p1);
extern void TouchCoOrdGet(int *pX, int *pY);

#endif /* TOUCH_H_ */
