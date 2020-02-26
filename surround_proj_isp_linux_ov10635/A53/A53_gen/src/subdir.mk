################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../A53_gen/src/isp_process.cpp 

C_SRCS += \
../A53_gen/src/kmem.c \
../A53_gen/src/ov10635_surround.c \
../A53_gen/src/sequencer_srec.c 

OBJS += \
./A53_gen/src/isp_process.o \
./A53_gen/src/kmem.o \
./A53_gen/src/ov10635_surround.o \
./A53_gen/src/sequencer_srec.o 

C_DEPS += \
./A53_gen/src/kmem.d \
./A53_gen/src/ov10635_surround.d \
./A53_gen/src/sequencer_srec.d 

CPP_DEPS += \
./A53_gen/src/isp_process.d 


# Each subdirectory must supply rules for building sources it contributes
A53_gen/src/%.o: ../A53_gen/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C++ Compiler'
	aarch64-linux-gnu-g++ "@A53_gen/src/isp_process.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

A53_gen/src/%.o: ../A53_gen/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	aarch64-linux-gnu-gcc "@A53_gen/src/kmem.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


