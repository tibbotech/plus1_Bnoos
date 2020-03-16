COMMON_DIR = common
LIB = lib
TESTAPI = testapi

ifeq ($(CROSS),)
CROSS = ../../crossgcc/armv5-eabi--glibc--stable/bin/armv5-glibc-linux-
endif

BIN = bin
TARGET = rom
LD_FILE = rom.ld
LD_SRC = script/rom.ldi
LDFLAGS = -T $(LD_FILE)
LDFLAGS_COM  = -L $(shell dirname `$(CROSS)gcc -print-libgcc-file-name`) -lgcc
#LDFLAGS_COM := -L $(shell dirname `$(CROSS)gcc -print-libgcc-file-name`) -L $(shell dirname `$(CROSS)gcc -print-file-name=libc.a`) -lgcc -lc

CFLAGS += -fno-builtin -O1
CFLAGS += -nodefaultlibs
CFALGS += -ffunction-sections -fdata-sections -flto
CFLAGS += -Wall -march=armv5te -Wno-unused-function -Wno-unused-variable -Wno-implicit-function-declaration
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

RS485_TEST = ENABLE
ifeq "$(RS485_TEST)" "ENABLE"
	CFLAGS += -DRS485_TEST
	# rs485 test
	RS485_PATH = drivers/rs485
	CSOURCES += $(wildcard $(RS485_PATH)/*.c)
endif

CFLAGS += -DIPC_TEST

OBJS = $(ASOURCES:.S=.o) $(CSOURCES:.c=.o)


.PHONY: clean all

all: clean $(TARGET) pack
	

$(TARGET): $(OBJS)
	@$(CROSS)cpp -P $(CFLAGS) $(LD_SRC) $(LD_FILE)
	$(CROSS)ld $(OBJS) -o $(BIN)/$@ -Map $(BIN)/$@.map $(LDFLAGS) $(LDFLAGS_COM)
	$(CROSS)objcopy -O binary -S $(BIN)/$@ $(BIN)/$@.bin
	$(CROSS)objdump -d -S $(BIN)/$@ > $(BIN)/$@.dis

pack:
	@# Add image header
	@echo "Wrap code image..."
	@bash ./script/add_uhdr.sh nonos_B $(BIN)/$(TARGET).bin $(BIN)/$(TARGET).img 0x10040 0x10040
	@sz=`du -sb bin/$(TARGET).img|cut -f1`;	printf "rom size = %d (hex %x)\n" $$sz $$sz

#testapi/qch/iop.o: testapi/qch/DQ8051.bin
%.o: %.S
	@$(CROSS)gcc $(CFLAGS) -c -o $@ $<


%.o: %.c
	@$(CROSS)gcc $(CFLAGS) -c -o $@ $<


$(BIN)/$(TARGET): $(OBJS)
	@$(CROSS)ld $(OBJS) -o $(BIN)/$(TARGET) -Map $(BIN)/$(TARGET).map $(LDFLAGS) $(LDFLAGS_COM)

zmem: $(BIN)/zmem.hex
$(BIN)/zmem.hex: $(TARGET)
# 	args: input_elf output_hex tool_chain_location
	@python ./script/tools/elf2mem.py $(BIN)/$< $@ $(CROSS)
	@echo "Gen $@ finished."
	@${MAKE} up

up:
	@echo "Upload to vnc3"
	@~/util/ftp_upload.sh vnc3 $(BIN)/zmem.hex /home/nzlu/ftp
	@echo "Upload done"


clean:
	@-rm -f $(OBJS) >/dev/null
	@-cd $(BIN); rm -f $(TARGET) $(TARGET).bin $(SPI_ALL).bin $(TARGET).map $(TARGET).dis $(TARGET).img >/dev/null
	@-rm -f bin/out.bin $(LD_FILE) >/dev/null


p-%:
	@echo "$* is '$($*)'"
