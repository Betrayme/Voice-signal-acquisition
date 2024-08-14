;;#############################################################################
;;! \file source/sel_q.asm
;;!
;;! \brief  Selects the Q format for the twiddle factors
;;!
;;! \date   Feb 25, 2002
;;! 
;;
;;  Group:            C2000
;;  Target Family:    C28x
;;
;; (C)Copyright 2014, Texas Instruments, Inc.
;;#############################################################################
;;$TI Release: C28x Fixed Point DSP Library V1.20.00.00 $
;;$Release Date: Oct 23, 2014 $
;;#############################################################################
;;
;;*****************************************************************************
;; defines
;;*****************************************************************************
Q30             .set    30
Q31             .set    31

; Select Twiddle factor Q format
TF_QFMAT        .set    Q30    
        
;;#############################################################################
;;  End of File
;;#############################################################################
