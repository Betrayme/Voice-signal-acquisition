################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ti-cgt-c2000_15.9.0.STS

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./W5300_Drv_V1.2.1/w5300.obj" \
"./W5300_Drv_V1.2.1/socket.obj" \
"./W5300_Drv_V1.2.1/md5.obj" \
"./W5300_Drv_V1.2.1/lstring.obj" \
"./src/task.obj" \
"./src/main.obj" \
"./src/heart.obj" \
"./src/ethernet.obj" \
"./DSP2833x_usDelay.obj" \
"./DSP2833x_Xintf.obj" \
"./DSP2833x_SysCtrl.obj" \
"./DSP2833x_SciStdio.obj" \
"./DSP2833x_Sci.obj" \
"./DSP2833x_PieVect.obj" \
"./DSP2833x_PieCtrl.obj" \
"./DSP2833x_MemCopy.obj" \
"./DSP2833x_GlobalVariableDefs.obj" \
"./DSP2833x_DefaultIsr.obj" \
"./DSP2833x_CpuTimers.obj" \
"./DSP2833x_CodeStartBranch.obj" \
"./DSP2833x_Adc.obj" \
"./DSP2833x_ADC_cal.obj" \
"../28335_RAM_lnk.cmd" \
"F:/ccsstore/F28335/TargetConfig/cmd/DSP2833x_Headers_nonBIOS.cmd" \
-l"libc.a" \
-l"rts2800_fpu32.lib" \
-l"IQmath_fpu32.lib" \
-l"rts2800_fpu32_fast_supplement.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include src/subdir_vars.mk
-include W5300_Drv_V1.2.1/subdir_vars.mk
-include subdir_rules.mk
-include src/subdir_rules.mk
-include W5300_Drv_V1.2.1/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: W5300_LOOPBACK_UDP.out

# Tool invocations
W5300_LOOPBACK_UDP.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ti-cgt-c2000_15.9.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -g --display_error_number --diag_wrap=off --diag_warning=225 -z -m"W5300_LOOPBACK_UDP.map" --stack_size=0x300 --warn_sections -i"C:/ti/ti-cgt-c2000_15.9.0.STS/lib" -i"C:/ti/ti-cgt-c2000_15.9.0.STS/include" -i"../" -i"../../../Library/Common" -i"../../../Library/Math/IQmath" -i"../../../Library/Math/FPUfastRTS" --reread_libs --display_error_number --diag_wrap=off --xml_link_info="W5300_LOOPBACK_UDP_linkInfo.xml" --rom_model -o "W5300_LOOPBACK_UDP.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C2000_EXECUTABLE_OUTPUTS__QUOTED) "W5300_LOOPBACK_UDP.out"
	-$(RM) "DSP2833x_Adc.pp" "DSP2833x_CpuTimers.pp" "DSP2833x_DefaultIsr.pp" "DSP2833x_GlobalVariableDefs.pp" "DSP2833x_MemCopy.pp" "DSP2833x_PieCtrl.pp" "DSP2833x_PieVect.pp" "DSP2833x_Sci.pp" "DSP2833x_SciStdio.pp" "DSP2833x_SysCtrl.pp" "DSP2833x_Xintf.pp" "src\ethernet.pp" "src\heart.pp" "src\main.pp" "src\task.pp" "W5300_Drv_V1.2.1\lstring.pp" "W5300_Drv_V1.2.1\md5.pp" "W5300_Drv_V1.2.1\socket.pp" "W5300_Drv_V1.2.1\w5300.pp" 
	-$(RM) "DSP2833x_ADC_cal.obj" "DSP2833x_Adc.obj" "DSP2833x_CodeStartBranch.obj" "DSP2833x_CpuTimers.obj" "DSP2833x_DefaultIsr.obj" "DSP2833x_GlobalVariableDefs.obj" "DSP2833x_MemCopy.obj" "DSP2833x_PieCtrl.obj" "DSP2833x_PieVect.obj" "DSP2833x_Sci.obj" "DSP2833x_SciStdio.obj" "DSP2833x_SysCtrl.obj" "DSP2833x_Xintf.obj" "DSP2833x_usDelay.obj" "src\ethernet.obj" "src\heart.obj" "src\main.obj" "src\task.obj" "W5300_Drv_V1.2.1\lstring.obj" "W5300_Drv_V1.2.1\md5.obj" "W5300_Drv_V1.2.1\socket.obj" "W5300_Drv_V1.2.1\w5300.obj" 
	-$(RM) "DSP2833x_ADC_cal.pp" "DSP2833x_CodeStartBranch.pp" "DSP2833x_usDelay.pp" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

