################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
SENSOR_HMC5883L/Kalman.obj: ../SENSOR_HMC5883L/Kalman.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="E:/ti/TivaWare_C_Series-2.1.4.178" -g --gcc --define=ccs="ccs" --define=TARGET_IS_TM4C123_RB1 --define=PART_TM4C123GH6PM --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="SENSOR_HMC5883L/Kalman.pp" --obj_directory="SENSOR_HMC5883L" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

SENSOR_HMC5883L/SENSOR_HMC5883L.obj: ../SENSOR_HMC5883L/SENSOR_HMC5883L.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="E:/ti/TivaWare_C_Series-2.1.4.178" -g --gcc --define=ccs="ccs" --define=TARGET_IS_TM4C123_RB1 --define=PART_TM4C123GH6PM --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="SENSOR_HMC5883L/SENSOR_HMC5883L.pp" --obj_directory="SENSOR_HMC5883L" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


