# List of objects to link
OBJS=$(foreach dir,$(SUBDIRS),$(find_ofiles))

# List of additional include only directories
INCS = $(foreach dir,$(INCDIRS),$(find_hfiles))

# list all source files
SRC=$(foreach dir,$(SUBDIRS),$(find_sfiles))

# list all include directories
INCLUDEFLAGS=$(foreach dir,$(SUBDIRS),$(find_hdirs))


OS := $(shell uname -s)


ifeq ($(OS),Linux)
	MKIMAGE = mkimage
else
	MKIMAGE = ./common/BuildTools/mkimage
endif


# Used by most project's Build command
all: $(PROJECT).bin


# Build .elf and .map
$(PROJECT).elf:  $(OBJS)
	-$(RM) $(LDSCRIPT)
	$(CP) Board/$(LDSCRIPT) $(LDSCRIPT)
	$(CC) $(LDFLAGS) -o $(PROJECT).elf $(OBJS) -T $(LDSCRIPT) -Wl,-Map=$(PROJECT).map,--cref
	$(SIZE) $(PROJECT).elf
	$(RE) -a $(PROJECT).elf > $(PROJECT).map2
	$(RM) -f $(LDSCRIPT)

# Build .bin file
$(PROJECT).bin:   $(PROJECT).elf
	$(OBJCOPY) $(OBJCOPY_FLAGS)  --output-target=binary $< $@

# delete created binaries and object files
clean:
	-$(RM)  $(OBJS)
	-$(RM)  $(PROJECT).elf
	-$(RM)  $(PROJECT).bin
	-$(RM)  $(PROJECT).map
	-$(RM)	$(PROJECT).map2
	-$(RM)  $(PROJECT).lst
	-$(RM)	*.bin

# clean build
rebuild: clean all
