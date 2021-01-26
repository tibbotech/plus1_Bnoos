COMMON_DIR = common
LIB = lib
TESTAPI = testapi

#CROSS = ../../crossgcc/armv5-eabi--glibc--stable/bin/armv5-glibc-linux-
CROSS = ../../crossgcc/gcc-arm-9.2-2019.12-x86_64-arm-none-eabi/bin/arm-none-eabi-
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
LDFLAGS_COM = -L $(shell dirname `$(CC) -print-libgcc-file-name`) -L $(shell dirname `$(CC) -print-file-name=libc.a`) -lstdc++ -lm -lc -lgcc

CFLAGS += -O1
CFLAGS += -nostdlib -fno-builtin
CFLAGS += -fno-pie -fno-PIE -fno-pic -fno-PIC
CFLAGS += -fno-partial-inlining -fno-jump-tables
CFLAGS += -static
CFLAGS += -nodefaultlibs
CFALGS += -ffunction-sections -fdata-sections -flto
CFLAGS += -Wall -march=armv5te -Wno-unused-function -Wno-unused-variable
CFLAGS += -Iinclude -Iinclude/util -I$(TESTAPI)/qch -g

#-mthumb -mthumb-interwork

SPI_ALL := romter.bin
SPI_HEX := spi_all.hex

# Boot up
ASOURCES = start.S
CSOURCES = main.c

# common

CSOURCES += $(COMMON_DIR)/diag.c $(COMMON_DIR)/common.c $(COMMON_DIR)/sio.c $(COMMON_DIR)/cpu_util.c $(COMMON_DIR)/stc.c $(wildcard ./testapi/util/*.c)
CSOURCES += $(COMMON_DIR)/gpio_exp.c
CSOURCES += $(COMMON_DIR)/eabi_compat.c
#CSOURCES += $(COMMON_DIR)/uart_printf.c

# mon
CSOURCES += mon/monitor.c

# interrupt
CSOURCES += $(TESTAPI)/interrupt/sp_interrupt.c
ASOURCES += $(TESTAPI)/interrupt/vectors.S

# Marlin
CFLAGS += -fno-use-cxa-atexit -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -w
CXXFLAGS = $(CFLAGS) -MMD -DF_CPU=16000000 -DARDUINO=105 -DMOTHERBOARD=3 -D__AVR_ATmega2560__
CXXFLAGS += -IMarlin/include -IMarlin/arduino
CXXSOURCES += Marlin/motion_control.cpp Marlin/MarlinSerial.cpp

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

CFLAGS += -DIPC_TEST

OBJS = $(ASOURCES:.S=.o) $(CSOURCES:.c=.o) $(CXXSOURCES:.cpp=.o)


.PHONY: clean all

all: $(BIN)/$(TARGET).bin pack
	

$(BIN)/$(TARGET).bin: $(LD_FILE) $(BIN)/$(TARGET).dis
	@$(OBJCOPY) -O binary -S $(BIN)/$(TARGET) $@

$(BIN)/$(TARGET).dis: $(BIN)/$(TARGET)
	@$(OBJDUMP) -d -S $< > $@

$(BIN)/$(TARGET): $(OBJS) $(LD_FILE)
	@$(LD) $(OBJS) -o $(BIN)/$(TARGET) -Map $(BIN)/$(TARGET).map $(LDFLAGS) $(LDFLAGS_COM)

$(LD_FILE): $(LD_SRC) $(OBJS)
	@$(CPP) -P $(CFLAGS) -x c $(LD_SRC) -o $(LD_FILE)

pack:
	@# Add image header
	@echo "Wrap code image..."
	@bash ./script/add_uhdr.sh uboot_B $(BIN)/$(TARGET).bin $(BIN)/$(TARGET).img 0x200040 0x200040
	@sz=`du -sb bin/$(TARGET).img|cut -f1`;	printf "rom size = %d (hex %x)\n" $$sz $$sz
	dd if=prebuilt/xboot_nor.img of=bin/spi_all.bin bs=1k seek=64
	dd if=bin/rom.img of=bin/spi_all.bin bs=1k seek=256
	dd if=bin/rom.img of=bin/out.bin 

#testapi/qch/iop.o: testapi/qch/DQ8051.bin
%.o: %.S
	@$(CC) $(CFLAGS) -c -o $@ $<


%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<


zmem: $(BIN)/zmem.hex
$(BIN)/zmem.hex: $(TARGET)
# 	args: input_elf output_hex tool_chain_location
	@python ./script/tools/elf2mem.py $(BIN)/$< $@ $(READELF)
	@echo "Gen $@ finished."
	@${MAKE} up

up:
	@echo "Upload to vnc3"
	@~/util/ftp_upload.sh vnc3 $(BIN)/zmem.hex /home/nzlu/ftp
	@echo "Upload done"


clean:
	@-rm -f $(OBJS) >/dev/null
	@-cd $(BIN); rm -f $(TARGET) $(TARGET).bin $(SPI_ALL).bin $(TARGET).map $(TARGET).dis >/dev/null
	@-rm -f bin/out.bin $(LD_FILE) >/dev/null


p-%:
	@echo "$* is '$($*)'"
