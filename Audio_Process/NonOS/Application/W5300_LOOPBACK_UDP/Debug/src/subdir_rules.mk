################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/ethernet.obj: ../src/ethernet.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ti-cgt-c2000_15.9.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ti-cgt-c2000_15.9.0.STS/include" --include_path="../inc" --include_path="../W5300_Drv_V1.2.1/include" --include_path="../../../Include/Headers" --include_path="../../../Include/Common" --include_path="../../../Include/Math/IQmath" --include_path="../../../Include/Math/FPUfastRTS" -g --display_error_number --diag_wrap=off --diag_warning=225 --preproc_with_compile --preproc_dependency="src/ethernet.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/heart.obj: ../src/heart.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ti-cgt-c2000_15.9.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ti-cgt-c2000_15.9.0.STS/include" --include_path="../inc" --include_path="../W5300_Drv_V1.2.1/include" --include_path="../../../Include/Headers" --include_path="../../../Include/Common" --include_path="../../../Include/Math/IQmath" --include_path="../../../Include/Math/FPUfastRTS" -g --display_error_number --diag_wrap=off --diag_warning=225 --preproc_with_compile --preproc_dependency="src/heart.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/main.obj: ../src/main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ti-cgt-c2000_15.9.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ti-cgt-c2000_15.9.0.STS/include" --include_path="../inc" --include_path="../W5300_Drv_V1.2.1/include" --include_path="../../../Include/Headers" --include_path="../../../Include/Common" --include_path="../../../Include/Math/IQmath" --include_path="../../../Include/Math/FPUfastRTS" -g --display_error_number --diag_wrap=off --diag_warning=225 --preproc_with_compile --preproc_dependency="src/main.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/task.obj: ../src/task.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ti-cgt-c2000_15.9.0.STS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="C:/ti/ti-cgt-c2000_15.9.0.STS/include" --include_path="../inc" --include_path="../W5300_Drv_V1.2.1/include" --include_path="../../../Include/Headers" --include_path="../../../Include/Common" --include_path="../../../Include/Math/IQmath" --include_path="../../../Include/Math/FPUfastRTS" -g --display_error_number --diag_wrap=off --diag_warning=225 --preproc_with_compile --preproc_dependency="src/task.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


