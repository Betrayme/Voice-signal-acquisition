;;*****************************************************************************
;;! \file source/vcu2/vcu2_crc_24.asm
;;!
;;! \brief  24-bit CRC
;;
;;  \date   Nov 8, 2012
;;! 
;;
;;  Group:            C2000
;;  Target Family:    F2837x
;;
;; Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/ 
;; ALL RIGHTS RESERVED
;;*****************************************************************************
;;$TI Release: C28x VCU Library Version v2.10.00.00 $
;;$Release Date: Mar 3, 2015 $
;;*****************************************************************************
;;
;;*****************************************************************************
;; includes
;;*****************************************************************************
;;
;;*****************************************************************************
;; global defines
;;*****************************************************************************
;; CRC Routine defines


;; Argument structure defines
ARG_SEEDVAL         .set    0
ARG_NBYTES          .set    2
ARG_PARITY          .set    3
ARG_CRCRESULT       .set    4
ARG_MSGBUFFER       .set    6
ARG_CRCTABLE        .set    8


;;*****************************************************************************
;; macros
;;*****************************************************************************
;;
;; MACRO   : 'CRC_CONTEXT_SAVE'
;; SIZE    : Number of WORDS/Number of Cycles 3
;; USAGE   : Called on entry into CRC routine
;;
CRC_CONTEXT_SAVE    .macro
    PUSH      XAR1
    PUSH      XAR2
    PUSH      XAR3
    .endm
;;
;; MACRO   : 'CRC_CONTEXT_RESTORE'
;; SIZE    : Number of WORDS/Number of Cycles 3
;; USAGE   : Called on exit from CRC routine
;;
CRC_CONTEXT_RESTORE    .macro
    POP       XAR3
    POP       XAR2
    POP       XAR1
    .endm

;;*****************************************************************************
;; globals
;;*****************************************************************************
    .global _CRC_run24Bit
    .global _CRC_run24BitReflected
    .global _CRC_init24Bit
;;*****************************************************************************
;; function definitions
;;*****************************************************************************
    .text
;;
;; \brief Calculate the 24-bit CRC using polynomial 0x5d6dcb
;;
;; \param Handle to the structure, CRC_Obj(passed in XAR4)
;; - *+XAR4[0]:  uint32_t seedValue -> Initial value of the CRC calculation
;; - *+XAR4[2]:  uint16_t nMsgBytes -> the number of bytes in the message buffer
;; - *+XAR4[3]:  CRC_parity_e parity -> start the CRC from the low byte (CRC_parity_even) or high byte (CRC_parity_odd) of the first word
;; - *+XAR4[4]:  uint32_t crcResult -> the calculated CRC
;; - *+XAR4[6]:  void *pMsgBuffer -> Pointer to the message buffer
;; - *+XAR4[8]:  void *pCrcTable -> Pointer to the CRC lookup table
;;
;; \note
;; -
;;
;; \return CRC of the message(stored within the structure itself)
;;
_CRC_run24Bit:
    CRC_CONTEXT_SAVE
    ;MOVL       XAR2, XAR4 				         ;*+XAR4[3bit] addressing wont support a larger structure
                                                 ;might have to assign XAR2 to point to the structure and
                                                 ;iterate through using XAR2++
;; Register Usage:
;;   XAR0:         Number of bytes to process
;;   XAR1:         Repeat block counter
;;   XAR2:         Pointer to iterate through the CRC object(possible future use)
;;   XAR4:         Points to the CRC object
;;   XAR5:         Points to the message buffer
;;
    VCRCCLR                                      ; Clear out the CRC result register
    MOVL         XAR0, *+XAR4[ARG_NBYTES]        ; Load number of message bytes into AR0
    VMOV32       VCRC, *+XAR4[ARG_SEEDVAL]       ; Load seed value into the CRC result register
    MOVL         XAR5, *+XAR4[ARG_MSGBUFFER]     ; XAR5 points to the message buffer
    MOV           AL,  *+XAR4[ARG_PARITY]        ; Check the parity
    SBF            _CRC_run24Bit_Loop, EQ        ; If Parity = LOW_BYTE, skip to loop
    VCRC24H_1    *XAR5++                         ; Parity = HIGH_BYTE, calculate high byte of the first word,
                                                 ; ignore the low byte and proceed to next word
    DEC          AR0
    SBF          _CRC_run24Bit_End, EQ           ; Jump to end if no more bytes

_CRC_run24Bit_Loop:
    MOV          AL, AR0
    MOV          AH, AR0
    AND          AL, #0xFFF8                     ; Check to see if length greater than 8 bytes
                                                 ; if true, handle the <8 bytes in a loop
                                                 ; AL is now a multiple of 8
    SBF          _CRC_run24Bit_LT8BytesLeft, EQ
    LSR          AL, #3                          ; loop in 8 bytes at a time
    MOV          AR1, AL                         ; move count into AR1
    SUB          AR1, #1                         ; subtract 1, accounts for the RPTB instruction i.e. it loops
                                                 ; N + 1 times

    .align       2                               ; align at 32-bit boundary to remove penalty
                                                 ; loop through the message 8 bytes at a time
    RPTB         _CRC_run24Bit_RepeatBlock, AR1
    VCRC24L_1    *XAR5
    VCRC24H_1    *XAR5++
    VCRC24L_1    *XAR5
    VCRC24H_1    *XAR5++
    VCRC24L_1    *XAR5
    VCRC24H_1    *XAR5++
    VCRC24L_1    *XAR5
    VCRC24H_1    *XAR5++
_CRC_run24Bit_RepeatBlock:
    LSL          AL, #3                          ; multiply by 8 to get the pre RPTB count
    SUB          AH, AL                          ; AH holds the number of remaining bytes(<8)
    SBF          _CRC_run24Bit_End, EQ           ; if multiple of 8, AH is 0, done processing
    MOV          AR0, AH
_CRC_run24Bit_LT8BytesLeft:
    VCRC24L_1    *XAR5
    DEC          AR0
    SBF          _CRC_run24Bit_End, EQ
    VCRC24H_1    *XAR5++
    DEC          AR0
    SBF          _CRC_run24Bit_LT8BytesLeft, NEQ
_CRC_run24Bit_End:
    VMOV32       *+XAR4[ARG_CRCRESULT], VCRC     ; Save the result to the structure

    CRC_CONTEXT_RESTORE
    LRETR

;;*****************************************************************************
;;
;; \brief Calculate the 24-bit CRC using polynomial 0x5d6dcb but with input bits reversed
;;
;; \param Handle to the structure, CRC_Obj(passed in XAR4)
;; - *+XAR4[0]:  uint32_t seedValue -> Initial value of the CRC calculation
;; - *+XAR4[2]:  uint16_t nMsgBytes -> the number of bytes in the message buffer
;; - *+XAR4[3]:  CRC_parity_e parity -> start the CRC from the low byte (CRC_parity_even) or high byte (CRC_parity_odd) of the first word
;; - *+XAR4[4]:  uint32_t crcResult -> the calculated CRC
;; - *+XAR4[6]:  void *pMsgBuffer -> Pointer to the message buffer
;; - *+XAR4[8]:  void *pCrcTable -> Pointer to the CRC lookup table
;;
;; \note
;; - actively sets the CRCMSGFLIP bit, each word has all its bits reversed prior to the
;;   CRC being calculated
;;
;; \return CRC of the message(stored within the structure itself)
;;
_CRC_run24BitReflected:
    VSETCRCMSGFLIP
    LCR       _CRC_run24Bit
    VCLRCRCMSGFLIP
    LRETR

;
;;*****************************************************************************
;;
;; \brief Initialize the 24-bit CRC
;;
;; Ensures that the CRCMSGFLIP bit is cleared, this ensures that the input
;; is interpreted in normal bit-order
;;
;; \param Handle to the structure, CRC_Obj(passed in XAR4)
;; - *+XAR4[0]:  uint32_t seedValue -> Initial value of the CRC calculation
;; - *+XAR4[2]:  uint16_t nMsgBytes -> the number of bytes in the message buffer
;; - *+XAR4[3]:  CRC_parity_e parity -> start the CRC from the low byte (CRC_parity_even) or high byte (CRC_parity_odd) of the first word
;; - *+XAR4[4]:  uint32_t crcResult -> the calculated CRC
;; - *+XAR4[6]:  void *pMsgBuffer -> Pointer to the message buffer
;; - *+XAR4[8]:  void *pCrcTable -> Pointer to the CRC lookup table
;;
;; \note
;; -
;;
_CRC_init24Bit:
    VCLRCRCMSGFLIP
    LRETR

;; End of file

