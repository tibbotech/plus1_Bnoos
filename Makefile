# set V=1 (eg, "make V=1") to print the full commands etc.
ifneq ($V,1)
 Pecho=@echo
 P=@
else
 Pecho=@:
 P=
endif
DD = dd status=none bs=1k of=$@ seek=

COMMON_DIR = common
LIB = lib
TESTAPI = testapi

#CROSS = ../../crossgcc/armv5-eabi--glibc--stable/bin/armv5-glibc-linux-
CROSS = ../../gcc-arm-9.2-2019.12-mingw-w64-i686-arm-none-eabi/bin/arm-none-eabi-
ifneq ($(CROSS),)
CC = $(CROSS)gcc
CXX = $(CROSS)g++
LD = $(CROSS)ld
CPP = $(CROSS)cpp
OBJCOPY = $(CROSS)objcopy
OBJDUMP = $(CROSS)objdump
READELF = $(CROSS)readelf
endif

BIN = bin
TARGET = rom
LD_FILE = rom.ld
LD_SRC = script/rom.ldi
LDFLAGS = -T $(LD_FILE)
#LDFLAGS_COM  = -L $(shell dirname `$(CC) -print-libgcc-file-name`) -lgcc
LDFLAGS_COM = -L D:\SP7021\gcc-arm-9.2-2019.12-mingw-w64-i686-arm-none-eabi\lib\gcc\arm-none-eabi\9.2.1 -lstdc++ -lm -lc -lgcc

CFLAGS += -MMD -O1
CFLAGS += -nostdlib -fno-builtin
CFLAGS += -fno-pie -fno-PIE -fno-pic -fno-PIC
CFLAGS += -fno-partial-inlining -fno-jump-tables
CFLAGS += -static
CFLAGS += -nodefaultlibs
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wall -march=armv5te -Wno-unused-function -Wno-unused-variable
CFLAGS += -Iinclude -Iinclude/util -I$(TESTAPI)/qch -g

#-mthumb -mthumb-interwork

SPI_ALL := spi_all.bin
SPI_HEX := spi_all.hex

# Boot up
ASOURCES = start.S
CSOURCES = main.c

# common

CSOURCES += $(COMMON_DIR)/diag.c $(COMMON_DIR)/common.c $(COMMON_DIR)/sio.c $(COMMON_DIR)/cpu_util.c $(COMMON_DIR)/stc.c $(wildcard $(TESTAPI)/util/*.c)
CSOURCES += $(COMMON_DIR)/gpio_exp.c
CSOURCES += $(COMMON_DIR)/eabi_compat.c
#CSOURCES += $(COMMON_DIR)/uart_printf.c

# mon
CSOURCES += mon/monitor.c

# interrupt
CSOURCES += $(TESTAPI)/interrupt/sp_interrupt.c
ASOURCES += $(TESTAPI)/interrupt/vectors.S

# ardunio C source
CSOURCES += Marlin/arduino/wiring_analog.c Marlin/arduino/wiring_digital.c Marlin/arduino/wiring.c

# Marlin
CFLAGS += -fno-use-cxa-atexit -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -w
CFLAGS += -IMarlin/include -IMarlin/arduino
CFLAGS += -DF_CPU=202000000 -DARDUINO=105 -DMOTHERBOARD=33 -D__AVR_ATmega2560__ -DCPU_32_BIT
CXXSOURCES += Marlin/motion_control.cpp Marlin/MarlinSerial.cpp Marlin/stepper.cpp Marlin/planner.cpp 
CXXSOURCES += Marlin/temperature_ads1015.cpp Marlin/temperature.cpp
CXXSOURCES += Marlin/ConfigurationStore.cpp Marlin/Marlin_main.cpp Marlin/arduino/main.cpp


#I2C_TEST = ENABLE
ifeq "$(I2C_TEST)" "ENABLE"
	CFLAGS += -DI2C_TEST
	# i2c test
	I2C_PATH = drivers/i2c
	CSOURCES += $(wildcard $(I2C_PATH)/*.c)
endif

# NOC_TEST = ENABLE
ifeq "$(NOC_TEST)" "ENABLE"
	CFLAGS += -DNOC_TEST
	# noc test
	TEST_NOC = $(TESTAPI)/noc
	CSOURCES += $(wildcard $(TEST_NOC)/*.c)
endif

#QCH_TEST = ENABLE
ifeq "$(QCH_TEST)" "ENABLE"
	TEST_QCH = $(TESTAPI)/qch
	CFLAGS += -DQCH_TEST
	CSOURCES += $(wildcard $(TEST_QCH)/*.c)
	ASOURCES += $(wildcard $(TEST_QCH)/*.S)
endif

#AXI_MON = ENABLE
ifeq "$(AXI_MON)" "ENABLE"
	CFLAGS += -DAXI_MON
	TEST_AXI_MON = $(TESTAPI)/axi_monitor
	CSOURCES += $(wildcard $(TEST_AXI_MON)/*.c)
endif

#SPI_NOR_TEST = ENABLE
ifeq "$(SPI_NOR_TEST)" "ENABLE"
        CFLAGS += -DSPI_NOR_TEST
        # spi nor test
        SPINOR_PATH = drivers/spinor
        CSOURCES += $(wildcard $(SPINOR_PATH)/*.c)
endif

#RS485_TEST = ENABLE
ifeq "$(RS485_TEST)" "ENABLE"
	CFLAGS += -DRS485_TEST
	# rs485 test
	RS485_PATH = drivers/rs485
	CSOURCES += $(wildcard $(RS485_PATH)/*.c)
endif

#CFLAGS += -DIPC_TEST

OBJS = $(ASOURCES:.S=.o) $(CSOURCES:.c=.o) $(CXXSOURCES:.cpp=.o)
DEPS = $(OBJS:.o=.d)


.PHONY: clean all

all: $(BIN)/$(SPI_ALL) ISP
#	dd if=bin/rom.img of=bin/kernel
#	@cp bin/xboot.img xboot0
#	@cp bin/u-boot.img uboot0
#	@cp xboot0 xboot1
#	@cp uboot0 uboot1
#	@cp uboot0 uboot2
#	@isp pack_image out/ISPBOOOT.BIN xboot0 uboot0 xboot1 0x100000 uboot1 0x300000 uboot2 0x300000 bin/kernel 0xf00000
	@echo aaa
	cp bin/rom.img u-boot.img

$(BIN)/$(TARGET).bin: $(BIN)/$(TARGET)
	$P $(OBJCOPY) -O binary -S $< $@

$(BIN)/$(TARGET).dis: $(BIN)/$(TARGET)
	$P $(OBJDUMP) -d -S $< > $@

$(BIN)/$(TARGET): $(OBJS) $(LD_FILE)
	$(Pecho) "  LD   $@"
	$P $(LD) $(OBJS) -o $(BIN)/$(TARGET) -Map $(BIN)/$(TARGET).map $(LDFLAGS) $(LDFLAGS_COM)

$(LD_FILE): $(LD_SRC)
	$P $(CPP) -P $(CFLAGS) -x c $< -o $@

$(BIN)/$(SPI_ALL): $(BIN)/$(TARGET).bin Makefile
	$(Pecho) "  PACK $@"
#	$P bash ./script/add_uhdr.sh uboot_B $< $(BIN)/$(TARGET).img 0x200040 0x200040
	@mkimage -A arm -O linux -T quickboot -C none -a 0x200040 -e 0x200040 -n uboot_B -d $(BIN)/$(TARGET).bin $(BIN)/$(TARGET).img
#	$P $(DD)64  if=prebuilt/xboot_nor.img
#	$P $(DD)256 if=$(BIN)/$(TARGET).img
#	dd  bs=1k of=$@ if=bin/bootRom.bin
	dd  bs=1k of=$@ seek=64  if=bin/xboot.img
	dd  bs=1k of=$@ seek=256 if=$(BIN)/$(TARGET).img

#	$P ls -l $@
pack:
	@echo "Wrap code image..."
	@mkimage -A arm -O linux -T quickboot -C none -a 0x200040 -e 0x200040 -n uboot -d $(BIN)/$(TARGET).bin $(BIN)/$(TARGET).img

ISP:
	@echo "isp..."
	@rm -rf out
	@mkdir out
#testapi/qch/iop.o: testapi/qch/DQ8051.bin
%.o: %.S
	$(Pecho) "  CC   $<"
	$P $(CC) $(CFLAGS) -c -o $@ $<

%.o: %.c*
	$(Pecho) "  CC   $<"
	$P $(CC) $(CFLAGS) -c -o $@ $<

zmem: $(BIN)/zmem.hex
$(BIN)/zmem.hex: $(TARGET)
# 	args: input_elf output_hex tool_chain_location
	$P python ./script/tools/elf2mem.py $(BIN)/$< $@ $(READELF)
	$(Pecho) "Gen $@ finished."
	$P ${MAKE} up

up:
	$(Pecho) "Upload to vnc3"
	$P ~/util/ftp_upload.sh vnc3 $(BIN)/zmem.hex /home/nzlu/ftp
	$(Pecho) "Upload done"


clean:
	$P -rm -f $(OBJS)
#	$P -cd $(BIN); rm -f $(TARGET) $(TARGET).bin $(SPI_ALL) $(TARGET).map $(TARGET).dis $(TARGET).img >/dev/null
#	$P -rm -f $(LD_FILE) >/dev/null


p-%:
	$(Pecho) "$* is '$($*)'"

# Automaticaly include the dependency files created by gcc
-include $(DEPS)
