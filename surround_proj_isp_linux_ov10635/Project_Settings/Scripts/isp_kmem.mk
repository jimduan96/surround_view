KMEM := kmem
TOOL_PREFIX := arm-none-eabi-
OBJCOPY := $(TOOL_PREFIX)objcopy
NM :=  $(TOOL_PREFIX)nm
ECHO := @echo
AWK :=  awk
MKDIR := mkdir -p

KMEMH_OUT_DIR := ../A53_gen/inc
KMEMC_OUT_DIR := ../A53_gen/src

PROJECT_NAME = $(ProjName)_ISP
 
all: $(KMEM).srec $(KMEMC_OUT_DIR)/$(KMEM).c $(KMEMH_OUT_DIR)/$(KMEM)_sym.h

# srecord file
$(KMEM).srec: $(PROJECT_NAME).elf
	$(ECHO)
	$(ECHO) "Generating: $@"
	$(OBJCOPY) -O srec -j .data $< $@ 
	
# memory dump of srecord file
$(KMEMC_OUT_DIR)/$(KMEM).c: $(KMEM).srec
	$(MKDIR) $(@D) 
	$(ECHO) "Generating: $(@F)"
	$(ECHO) "/* KMEM Kernel Firmware Srecord */" > $@
	$(ECHO) "">> $@
	$(ECHO) "char kmem_srec[] = {\\" >> $@
	$(ECHO) "\"\\">> $@
	$(AWK) '{ gsub("\r", ""); print $$0"\\"}' $< >> $@
	$(ECHO) "\"\\" >> $@
	$(ECHO) "};" >> $@

# include file with symbols
$(KMEMH_OUT_DIR)/$(KMEM)_sym.h: $(PROJECT_NAME).elf
	$(MKDIR) $(@D)
	$(ECHO) "Generating: $(@F)"
	$(NM) -n $< | $(AWK) -v name=$(*F) > $@ \
	'BEGIN{gsub("/","_",name);\
	printf("#ifndef %s_h\n#define %s_h\n",name,name);}\
	{ if($$2=="T" || $$2=="D") \
	{gsub("[.]","_",$$3); printf("#define %s 0x%s\n",$$3,$$1);}}\
	END{printf("#endif\n");}'
	$(ECHO)

$(V).SILENT:
