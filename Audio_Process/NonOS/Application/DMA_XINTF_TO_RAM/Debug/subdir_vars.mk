################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../28335_RAM_lnk.cmd \
D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/TargetConfig/cmd/DSP2833x_Headers_nonBIOS.cmd 

ASM_SRCS += \
D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_ADC_cal.asm \
D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_CodeStartBranch.asm \
D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_usDelay.asm 

C_SRCS += \
D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_CpuTimers.c \
D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_DMA.c \
D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_DefaultIsr.c \
D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Headers/DSP2833x_GlobalVariableDefs.c \
D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_PieCtrl.c \
D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_PieVect.c \
D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_Sci.c \
D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_SciStdio.c \
D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_SysCtrl.c \
../DSP2833x_Xintf.c \
../main.c 

C_DEPS += \
./DSP2833x_CpuTimers.d \
./DSP2833x_DMA.d \
./DSP2833x_DefaultIsr.d \
./DSP2833x_GlobalVariableDefs.d \
./DSP2833x_PieCtrl.d \
./DSP2833x_PieVect.d \
./DSP2833x_Sci.d \
./DSP2833x_SciStdio.d \
./DSP2833x_SysCtrl.d \
./DSP2833x_Xintf.d \
./main.d 

OBJS += \
./DSP2833x_ADC_cal.obj \
./DSP2833x_CodeStartBranch.obj \
./DSP2833x_CpuTimers.obj \
./DSP2833x_DMA.obj \
./DSP2833x_DefaultIsr.obj \
./DSP2833x_GlobalVariableDefs.obj \
./DSP2833x_PieCtrl.obj \
./DSP2833x_PieVect.obj \
./DSP2833x_Sci.obj \
./DSP2833x_SciStdio.obj \
./DSP2833x_SysCtrl.obj \
./DSP2833x_Xintf.obj \
./DSP2833x_usDelay.obj \
./main.obj 

ASM_DEPS += \
./DSP2833x_ADC_cal.d \
./DSP2833x_CodeStartBranch.d \
./DSP2833x_usDelay.d 

OBJS__QUOTED += \
"DSP2833x_ADC_cal.obj" \
"DSP2833x_CodeStartBranch.obj" \
"DSP2833x_CpuTimers.obj" \
"DSP2833x_DMA.obj" \
"DSP2833x_DefaultIsr.obj" \
"DSP2833x_GlobalVariableDefs.obj" \
"DSP2833x_PieCtrl.obj" \
"DSP2833x_PieVect.obj" \
"DSP2833x_Sci.obj" \
"DSP2833x_SciStdio.obj" \
"DSP2833x_SysCtrl.obj" \
"DSP2833x_Xintf.obj" \
"DSP2833x_usDelay.obj" \
"main.obj" 

C_DEPS__QUOTED += \
"DSP2833x_CpuTimers.d" \
"DSP2833x_DMA.d" \
"DSP2833x_DefaultIsr.d" \
"DSP2833x_GlobalVariableDefs.d" \
"DSP2833x_PieCtrl.d" \
"DSP2833x_PieVect.d" \
"DSP2833x_Sci.d" \
"DSP2833x_SciStdio.d" \
"DSP2833x_SysCtrl.d" \
"DSP2833x_Xintf.d" \
"main.d" 

ASM_DEPS__QUOTED += \
"DSP2833x_ADC_cal.d" \
"DSP2833x_CodeStartBranch.d" \
"DSP2833x_usDelay.d" 

ASM_SRCS__QUOTED += \
"D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_ADC_cal.asm" \
"D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_CodeStartBranch.asm" \
"D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_usDelay.asm" 

C_SRCS__QUOTED += \
"D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_CpuTimers.c" \
"D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_DMA.c" \
"D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_DefaultIsr.c" \
"D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Headers/DSP2833x_GlobalVariableDefs.c" \
"D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_PieCtrl.c" \
"D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_PieVect.c" \
"D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_Sci.c" \
"D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_SciStdio.c" \
"D:/WORK_SPCAE/CCS_Project/Audio_Process/NonOS/Source/Common/DSP2833x_SysCtrl.c" \
"../DSP2833x_Xintf.c" \
"../main.c" 


