


#ifndef _UARTSTDIO_H_
#define _UARTSTDIO_H_

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
**                    MACRO DEFINITIONS
****************************************************************************/

/****************************************************************************
**                    FUNCTION PROTOTYPES
****************************************************************************/

extern unsigned int SCIPuts(char *pTxBuffer, int numBytesToWrite);
extern unsigned int SCIGets(char *pRxBuffer, int numBytesToRead);
extern unsigned int SCIwrite(const char *pcBuf, unsigned int len);
extern void SCIprintf(const char *pcString, ...);
extern void SCIPutHexNum(unsigned long hexValue);
extern void SCIPutc(unsigned char byteTx);
extern unsigned int SCIGetHexNum(void);
extern unsigned char SCIGetc(void);
extern void SCIPutNum(long value);
extern void SCIStdioInit(void);
extern long SCIGetNum(void);

#ifdef __cplusplus
}
#endif
#endif
