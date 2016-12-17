

#update the project name and the source directories to build from
PROJECT = "SLAscanner"
SOURCE_DIR = \
	./Inc \
	./Src \
	./Drivers/CMSIS/Include \
	./Drivers/CMSIS/Device/ST/STM32F4xx/Include \
	./Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates \
	./Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc \
	./Drivers/STM32F4xx_HAL_Driver/Src \
	./Drivers/STM32F4xx_HAL_Driver/Inc 
	
LDSCRIPT=STM32F446RETx_FLASH.ld

#find the objects to use in the build
OBJS = $(foreach dir,$(SOURCE_DIR), $(patsubst $(dir)/%.c,$(dir)/%.o,$(wildcard $(dir)/*.c))) 
OBJS += $(foreach dir,$(SOURCE_DIR),$(patsubst $(dir)/%.cc,$(dir)/%.o,$(wildcard $(dir)/*.cc)))
OBJS += $(foreach dir,$(SOURCE_DIR),$(patsubst $(dir)/%.s,$(dir)/%.o,$(wildcard $(dir)/*.s)))

#find include
INCLUDEFLAGS  = $(foreach dir,$(SOURCE_DIR),-I $(dir))

#list of object to remove when cleaning
DELETEOBJS = $(foreach dir,$(SOURCE_DIR),$(wildcard $(dir)/*.o))
DELETEOBJS += $(foreach dir,$(SOURCE_DIR),$(wildcard $(dir)/*.d))
DELETEOBJS += $(foreach dir,$(SOURCE_DIR),$(wildcard $(dir)/*.lst))

CC=arm-none-eabi-gcc
AS=arm-none-eabi-gcc -x assembler-with-cpp
CXX=arm-none-eabi-g++
LD=arm-none-eabi-g++
OBJCOPY=arm-none-eabi-objcopy


CFLAGS = -O0 -fsingle-precision-constant -Wall -Wa,-adhlns="$@.lst" \
 -c -fmessage-length=0 -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g3 -gdwarf-2 -nostartfiles \
 $(INCLUDEFLAGS) -DSTM32F446xx

CXXFLAGS = -O0 -fsingle-precision-constant -Wall -std=gnu++0x \
 -Wa,-adhlns="$@.lst" -fno-exceptions -fno-rtti -c -fmessage-length=0 \
 -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g3 -gdwarf-2 -nostartfiles \
 $(INCLUDEFLAGS) -DSTM32F446xx

LDFLAGS =  -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g3 -gdwarf-2 -specs=nosys.specs

ASFLAGS = $(INCLUDEFLAGS) -T $(LDSCRIPT) -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 \
 -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g3 -gdwarf-2 -DSTM32F446xx
 
OBJCOPY_FLAGS=

all: $(PROJECT).bin

$(PROJECT).elf: $(OBJS) 
	$(LD) $(OBJS)  -o $(PROJECT).elf  $(LDFLAGS)  -T $(LDSCRIPT)

# Build .bin file
$(PROJECT).bin:   $(PROJECT).elf
	$(OBJCOPY) $(OBJCOPY_FLAGS)  --output-target=binary $< $@
	$(OBJCOPY)  -I binary -O srec $(PROJECT).bin $(PROJECT).s19

clean:
	-$(RM)  $(DELETEOBJS)
	-$(RM)  $(PROJECT)*.*
	-$(RM)  $(LIBLST)
