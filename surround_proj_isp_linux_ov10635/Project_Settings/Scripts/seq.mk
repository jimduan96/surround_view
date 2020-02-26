ECHO := @echo
DOS2UNIX := @dos2unix
AWK := @awk
RM := rm -rf
CD := cd
MKDIR := @mkdir -p

SRSECS ?= .text .data .rodata .vectable

# Sequencer sources folder name variable
SEQ_APP := sequencer
SEQ_SRC_DIR := ../SEQ_gen/src
SEQ_OUT_DIR := ../SEQ
SEQ_OUT_OBJ_DIR := $(SEQ_OUT_DIR)/src
SREC_OUT_DIR :=  ../A53_gen/src
LD_SRC_DIR := ../Project_Settings/Linker_Files

SEQ_CCSRC := $(notdir $(wildcard $(SEQ_SRC_DIR)/*.c))
SEQ_CXXSRC := $(notdir $(wildcard $(SEQ_SRC_DIR)/*.cpp))
SEQ_CCOBJS := $(foreach src, $(patsubst %.c, %.o, $(SEQ_CCSRC)), $(addprefix $(SEQ_OUT_OBJ_DIR)/,$(src)))
SEQ_CXXOBJS := $(foreach src, $(patsubst %.cpp, %.o, $(SEQ_CXXSRC)), $(addprefix $(SEQ_OUT_OBJ_DIR)/,$(src)))

TOOL_PATH := $(TD)/bin
TOOL_PREFIX := arm-none-eabi-
TOOL_CC := gcc
TOOL_CXX := g++
TOOL_LD := g++
TOOL_OBJCOPY := objcopy

CC := "$(TOOL_PATH)/$(TOOL_PREFIX)$(TOOL_CC)" -c
CXX := "$(TOOL_PATH)/$(TOOL_PREFIX)$(TOOL_CXX)" -c
LD := "$(TOOL_PATH)/$(TOOL_PREFIX)$(TOOL_LD)"
OBJCOPY := "$(TOOL_PATH)/$(TOOL_PREFIX)$(TOOL_OBJCOPY)"

CC_INCS := -I"$(VD)/isp/inc" -I"$(VD)/include" -I"$(VD)/isp/firmware/inc"
CXX_INCS := $(CC_INCS)
OPTS := -mfpu=vfp -mfloat-abi=soft -mcpu=cortex-m0 -march=armv6-m -mthumb -nostdlib -Wl,--build-id=none -g -DOSDIR=linux
CC_OPTS := $(OPTS) -mstructure-size-boundary=8 -mno-unaligned-access -O3 -fno-builtin
CXX_OPTS := $(CC_OPTS)
LD_OPTS := $(SEQ_CCOBJS) $(SEQ_CXXOBJS)
LD_OPTS += $(OPTS) -L"$(VD)/isp/firmware/build-v234ce-gnu-linux-d"
LD_OPTS += -T"$(LD_SRC_DIR)/sequencer.ld" -Wl,-Map,"$(SEQ_OUT_DIR)/$(SEQ_APP).map" -lsequencer

all: $(SREC_OUT_DIR)/$(SEQ_APP)_srec.c

$(SEQ_OUT_DIR)/$(SEQ_APP).elf: $(SEQ_CCOBJS) $(SEQ_CXXOBJS) $(LD_SRC_DIR)/sequencer.ld
	$(MKDIR) $(@D)
	$(ECHO) 'Building target: $@'
	$(ECHO) 'Invoking: ARM32 Standard S32DS C++ Linker'
	$(LD) $(LD_OPTS) -o $@
	$(ECHO) 'Finished building target: $@'
	$(ECHO)
 
$(SEQ_OUT_OBJ_DIR)/%.o: $(SEQ_SRC_DIR)/%.c
	$(MKDIR) $(@D)
	$(ECHO) 'Building file: $<'
	$(ECHO) 'Invoking: ARM32 Standard S32DS C Compiler'
	$(CC) $(CC_OPTS) $(CC_INCS) -o $@ "$<"
	$(ECHO) 'Finished building: $<'
	$(ECHO)

$(SEQ_OUT_OBJ_DIR)/%.o: $(SEQ_SRC_DIR)/%.cpp
	$(MKDIR) $(@D)
	$(ECHO) 'Building file: $<'
	$(ECHO) 'Invoking: ARM32 Standard S32DS C++ Compiler'
	$(CXX) $(CXX_OPTS) $(CXX_INCS) -o $@ "$<"
	$(ECHO) 'Finished building: $<'
	$(ECHO)

 clean:
	-$(RM) $(SEQ_CCOBJS) $(SEQ_CXXOBJS) $(SEQ_OUT_DIR)/$(SEQ_APP).*

.PHONY: all clean

# C array from srecord file
$(SREC_OUT_DIR)/$(SEQ_APP)_srec.c: $(SEQ_OUT_DIR)/$(SEQ_APP).run
	$(MKDIR) $(@D)
	$(RM) $(@F)
	$(DOS2UNIX) $<
	$(ECHO) "Generating: $(@F)"
	$(ECHO) "/* Sequencer Firmware Srecord */"> $@
	$(ECHO) "">> $@
	$(ECHO) "char sequencer_srec[] = {\\">> $@
	$(ECHO) "\"\\">> $@
	$(AWK) '{print $$0"\\"}' $< >> $@
	$(ECHO) "\"\\">> $@
	$(ECHO) "};">> $@
 
 # srecord file
$(SEQ_OUT_DIR)/$(SEQ_APP).run: $(SEQ_OUT_DIR)/$(SEQ_APP).elf
	$(MKDIR) $(@D)
	$(ECHO) "Generating: $@"
	$(OBJCOPY) -O srec --srec-forceS3 $(addprefix -j ,$(SRSECS)) $< $@
	$(ECHO)

$(V).SILENT:
