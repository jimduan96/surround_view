################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include A53_src/subdir.mk
-include A53_gen/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_SIZE += \
surround_proj_isp_linux_ov10635.siz \


# All Target
all:
	$(MAKE) --no-print-directory pre-build
	$(MAKE) --no-print-directory main-build

# Main-build Target
main-build: surround_proj_isp_linux_ov10635.elf secondary-outputs

dependents:
	-cd D:/workspaceS32DS.3.2/surround_proj_isp_linux_ov10635/ISP && $(MAKE) all

# Tool invocations
surround_proj_isp_linux_ov10635.elf: $(OBJS) $(USER_OBJS) D:/workspaceS32DS.3.2/surround_proj_isp_linux_ov10635/ISP/surround_proj_isp_linux_ov10635_ISP.elf
	@echo 'Building target: $@'
	@echo 'Invoking: Standard S32DS C++ Linker'
	aarch64-linux-gnu-g++ -o "surround_proj_isp_linux_ov10635.elf" "@surround_proj_isp_linux_ov10635.args"  $(USER_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

surround_proj_isp_linux_ov10635.siz: surround_proj_isp_linux_ov10635.elf
	@echo 'Invoking: Standard S32DS Print Size'
	aarch64-linux-gnu-size --format=berkeley surround_proj_isp_linux_ov10635.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ./*
	-@echo ' '

pre-build:
	-rm -f ../A53_gen/src/sequencer_srec.c; $(MAKE) -f ../Project_Settings/Scripts/seq.mk  TD="D:\NXP\S32DS.3.2\eclipse\../S32DS/build_tools/gcc_b1620/gcc-6.3-arm32-eabi"  VD="D:\NXP\S32DS.3.2\eclipse\../S32DS/software/VSDK_S32V2_RTM_1_5_0/s32v234_sdk"
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents main-build pre-build
D:/workspaceS32DS.3.2/surround_proj_isp_linux_ov10635/ISP/surround_proj_isp_linux_ov10635_ISP.elf:

-include ../makefile.targets
