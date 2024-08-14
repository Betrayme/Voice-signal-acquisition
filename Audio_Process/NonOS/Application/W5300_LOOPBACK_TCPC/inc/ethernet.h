#ifndef _ETHERNET_H_
#define _ETHERNET_H_

#define	W5300_RST_Pin	GPIO63
#define	W5300_INT_Pin	GPIO35

#define W5300_RST_ON	GpioDataRegs.GPBCLEAR.bit.W5300_RST_Pin = 1
#define W5300_RST_OFF	GpioDataRegs.GPBSET.bit.W5300_RST_Pin = 1

#define IINCHIP_READ(addr) 			*((volatile uint16 *)(Uint32)addr)
#define IINCHIP_WRITE(addr,data)  	(*((volatile uint16 *)(Uint32)addr) = data)

extern void w5300Task(void);
extern void initW5300(void);
extern void initW5300Gpio(void);
extern void initW5300Xintf(void);

extern void processTCPS(SOCKET s, Uint16 port);
extern void loopback_tcps(SOCKET s, uint16 port, uint8* buf,uint16 mode);
extern void loopback_tcpc(SOCKET s, uint8* addr, uint16 port, uint8* buf,uint16 mode);
extern void loopback_udp(SOCKET s, uint16 port, uint8* buf, uint16 mode);

#endif
