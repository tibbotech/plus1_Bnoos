#set V=1 (eg, "make V=1") to print the full commands etc.
ifneq ($V,1)
 Pecho=@echo
 P=@
else
 Pecho=@:
 P=
endif
#set DEBUG_NONOS = y , xboot jump to nonos instead of uboot.
DEBUG_NONOS = 1
BOOT_NONOS_FROM_OPENAMP ?= 0

DD = dd status=none bs=1k of=$@ seek=

#CROSS = ../../../../crossgcc/gcc-arm-9.2-2019.12-x86_64-arm-none-eabi/bin/arm-none-eabi-
TOPDIR = $(abspath .)

ifneq ($(CROSS),)
CC = $(CROSS)gcc
CXX = $(CROSS)g++
LD = $(CROSS)ld
CPP = $(CROSS)cpp
OBJCOPY = $(CROSS)objcopy
OBJDUMP = $(CROSS)objdump
READELF = $(CROSS)readelf
SIZE = $(CROSS)size
endif

# Flags for C only
CFLAGS += -std=gnu11
# Flags for C++ only
CXXFLAGS += -std=gnu++14 -fno-use-cxa-atexit
# Flags in common for C and C++
CCFLAGS += -MMD -O1
CCFLAGS += -fno-pie -fno-PIE -fno-pic -fno-PIC
CCFLAGS += -fno-partial-inlining -fno-jump-tables
CCFLAGS += -ffunction-sections -fdata-sections
CCFLAGS += -Wall -march=armv5te -Wno-unused-function -Wno-unused-variable
CCFLAGS += -funsigned-char -funsigned-bitfields -fshort-enums -w
# OpenAMP libmetal log on
CCFLAGS += -DDEFAULT_LOGGER_ON -DMETAL_INTERNAL
# virt_uart
CCFLAGS += -DVIRTIOCON -DENABLE_SERIALVIRTIO 
CCFLAGS += -DSP7021
ifeq ($(BOOT_NONOS_FROM_OPENAMP),1)
CCFLAGS += -DBOOT_NONOS_FROM_OPENAMP
endif
ifeq ($(DEBUG_NONOS),1)
CCFLAGS += -DDEBUG_XBOOT_TO_NONOS
endif

# Include Path
# CMSIS header files
CCFLAGS += -I$(TOPDIR)/system/drivers/CMSIS/CMSIS/Core_ARMV5/Include
CCFLAGS += -I$(TOPDIR)/system/drivers/CMSIS/Device/sunplus/sp7021/Include
CCFLAGS += -I$(TOPDIR)/system/drivers/CMSIS/Device/sunplus/sp7021/Source/gcc

# HAL header files
CCFLAGS += -I$(TOPDIR)/system/drivers/sp7021_hal_driver/inc
CCFLAGS += -I$(TOPDIR)/system/sp7021

# libraries srcwarpper header files for Arduino framework
CCFLAGS += -I$(TOPDIR)/system/drivers/sp7021_hal_driver/src
CCFLAGS += -I$(TOPDIR)/libraries/DriverWrapper/src
CCFLAGS += -I$(TOPDIR)/libraries/DriverWrapper/src/sunplus
CCFLAGS += -I$(TOPDIR)/libraries/SPI/src
CCFLAGS += -I$(TOPDIR)/libraries/SPI/src/utility
CCFLAGS += -I$(TOPDIR)/libraries/Wire/src
CCFLAGS += -I$(TOPDIR)/libraries/Wire/src/utility

#Arduino header files for Arduino Framework
CCFLAGS += -I$(TOPDIR)/cores/arduino
CCFLAGS += -I$(TOPDIR)/cores/arduino/avr
CCFLAGS += -I$(TOPDIR)/cores/arduino/sunplus
CCFLAGS += -I$(TOPDIR)/cores/arduino/sunplus/hal
CCFLAGS += -I$(TOPDIR)/cores/arduino/sunplus/LL
CCFLAGS += -I$(TOPDIR)/cores/arduino/sunplus/LL
CCFLAGS += -I$(TOPDIR)/cores/arduino/sunplus/OpenAMP
CCFLAGS += -I$(TOPDIR)/variants/sp7021_evb
#OpenAMP headers files
CCFLAGS += -I$(TOPDIR)/system/Middlewares/OpenAMP
CCFLAGS += -I$(TOPDIR)/system/Middlewares/OpenAMP/libmetal/lib/include
CCFLAGS += -I$(TOPDIR)/system/Middlewares/OpenAMP/open-amp/lib/include
CCFLAGS += -I$(TOPDIR)/system/Middlewares/OpenAMP/open-amp/apps/machine/a926
CCFLAGS += -I$(TOPDIR)/system/Middlewares/OpenAMP/virt_driver

# C Sources Files Directory
#DIRS += $(TOPDIR)/system/sp7021
DIRS += $(TOPDIR)/libraries/DriverWrapper/src
DIRS += $(TOPDIR)/libraries/DriverWrapper/src/HAL
DIRS += $(TOPDIR)/libraries/DriverWrapper/src/sunplus
DIRS += $(TOPDIR)/libraries/SPI/src
DIRS += $(TOPDIR)/libraries/SPI/src/utility
DIRS += $(TOPDIR)/libraries/Wire/src
DIRS += $(TOPDIR)/libraries/Wire/src/utility
DIRS += $(TOPDIR)/cores/arduino
DIRS += $(TOPDIR)/cores/arduino/avr
DIRS += $(TOPDIR)/cores/arduino/sunplus
DIRS += $(TOPDIR)/cores/arduino/sunplus/OpenAMP
DIRS += $(TOPDIR)/cores/arduino/sunplus/OpenAMP/libmetal
DIRS += $(TOPDIR)/cores/arduino/sunplus/OpenAMP/libmetal/generic
DIRS += $(TOPDIR)/cores/arduino/sunplus/OpenAMP/libmetal/generic/a926
DIRS += $(TOPDIR)/cores/arduino/sunplus/OpenAMP/open-amp/machine
DIRS += $(TOPDIR)/cores/arduino/sunplus/OpenAMP/open-amp/proxy
DIRS += $(TOPDIR)/cores/arduino/sunplus/OpenAMP/open-amp/remoteproc
DIRS += $(TOPDIR)/cores/arduino/sunplus/OpenAMP/open-amp/rpmsg
DIRS += $(TOPDIR)/cores/arduino/sunplus/OpenAMP/open-amp/virtio
DIRS += $(TOPDIR)/system/Middlewares/OpenAMP/virt_driver

# Arduino C++ Example
#DIRS += $(TOPDIR)/libraries/examples
#DIRS += $(TOPDIR)/libraries/examples/timer
#DIRS += $(TOPDIR)/libraries/examples/i2c
#DIRS += $(TOPDIR)/libraries/examples/exti
#DIRS += $(TOPDIR)/libraries/examples/VirtIOSerial
DIRS += $(TOPDIR)/libraries/examples/uart

CSOURCES += $(wildcard $(patsubst %,%/*.c, $(DIRS) ))
CXXSOURCES += $(wildcard $(patsubst %,%/*.cpp, $(DIRS) ))
ASOURCES += $(wildcard $(patsubst %,%/*.S, $(DIRS) ))

BIN = $(TOPDIR)/bin
TARGET = $(BIN)/rom
LD_SRC = $(TOPDIR)/variants/sp7021_evb/script/ldscript.ldi
LD_FILE = $(TOPDIR)/ldscript.ld
LDFLAGS = -T $(LD_FILE)
LDFLAGS_COM = -L $(shell dirname `$(CC) -print-libgcc-file-name`) -L $(shell dirname `$(CC) -print-file-name=libc.a`) -lstdc++ -lm -lc -lgcc

SPI_ALL := $(BIN)/spi_all.bin
SPI_HEX := $(BIN)/spi_all.hex

OBJS = $(ASOURCES:.S=.o) $(CSOURCES:.c=.o) $(CXXSOURCES:.cpp=.o)
DEPS = $(OBJS:.o=.d)


.PHONY: clean all

all: check $(SPI_ALL) $(TARGET).dis

check:
	$P $(TOPDIR)/variants/sp7021_evb/script/check_dev.sh $(TOPDIR)

$(TARGET).bin: $(TARGET)
	$P $(OBJCOPY) -O binary -S $< $@

$(TARGET).dis: $(TARGET)
	$P $(OBJDUMP) -d -S $< > $@

$(TARGET): $(OBJS) $(LD_FILE)
	$(Pecho) "  LD   $@"
	$P mkdir -p $(BIN)
	$P $(LD) $(OBJS) -o $(TARGET) -Map $(TARGET).map $(LDFLAGS) $(LDFLAGS_COM)
	$P $(SIZE) -B $(TARGET)

$(LD_FILE): $(LD_SRC)
	$P $(CPP) -P $(CFLAGS) $(CCFLAGS) -x c $< -o $@

$(SPI_ALL): $(TARGET).bin Makefile
ifeq ($(DEBUG_NONOS),1)
	$(Pecho) "  PACK $@"
	$P bash $(TOPDIR)/variants/sp7021_evb/script/add_uhdr.sh uboot_B $< $(TARGET).img 0x200040 0x200040 $(TOPDIR)
	$P $(DD)64  if=$(TOPDIR)/variants/sp7021_evb/prebuilt/xboot_nor.img
	$P $(DD)256 if=$(TARGET).img
	$P ls -l $@
endif

%.o: %.S
	$(Pecho) "  CC   $<"
	$P $(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $<

%.o: %.c
	$(Pecho) "  CC   $<"
	$P $(CC) $(CFLAGS) $(CCFLAGS) -c -o $@ $<

%.o: %.cpp
	$(Pecho) "  CXX  $<"
	$P $(CXX) $(CXXFLAGS) $(CCFLAGS) -c -o $@ $<

clean:
	$P -rm -f $(OBJS) $(DEPS) rom.d ldscript.d >/dev/null
	$P -rm -f $(TARGET) $(TARGET).bin $(SPI_ALL) $(TARGET).map $(TARGET).dis $(TARGET).img >/dev/null

p-%:
	$(Pecho) "$* is '$($*)'"

# Automaticaly include the dependency files created by gcc
-include $(DEPS)
