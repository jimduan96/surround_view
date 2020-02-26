################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
IPUS_SRCS += \
D:/NXP/S32DS.3.2/S32DS/software/VSDK_S32V2_RTM_1_5_0/s32v234_sdk/isp/kernels/generic/src/yuv42210bit_to_8bit.ipus 

OIPUS += \
./ISP_kernels/yuv42210bit_to_8bit.oipus 


# Each subdirectory must supply rules for building sources it contributes
ISP_kernels/yuv42210bit_to_8bit.oipus: D:/NXP/S32DS.3.2/S32DS/software/VSDK_S32V2_RTM_1_5_0/s32v234_sdk/isp/kernels/generic/src/yuv42210bit_to_8bit.ipus
	@echo 'Building file: $<'
	@echo 'Invoking: S32V IPUS Assembler'
	as-IPUS "@ISP_kernels/yuv42210bit_to_8bit.args" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


