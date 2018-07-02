COMMON_DIR = common
LIB = lib
TESTAPI = testapi

CROSS = ./tools/armv7a-unknown-eabi_gcc_4.7.2/bin/armv7a-unknown-eabi-
BIN = bin
TARGET = rom
LD_FILE = ./script/rom.ld
LDFLAGS = -T $(LD_FILE)
LDFLAGS_COM := -L $(shell dirname `$(CROSS)gcc -print-libgcc-file-name`) -L $(shell dirname `$(CROSS)gcc -print-file-name=libc.a`) -lgcc -lc

CFLAGS += -fno-builtin
CFLAGS += -nodefaultlibs
CFALGS += -ffunction-sections -fdata-sections -flto
CFLAGS += -Wall -march=armv5te -Wno-unused-function -Wno-unused-variable -Wno-implicit-function-declaration
CFLAGS += -Iinclude -Iinclude/util -g

#-mthumb -mthumb-interwork

SPI_ALL := romter.bin
SPI_HEX := spi_all.hex

# Boot up
ASOURCES = start.S
CSOURCES = main.c

# common

CSOURCES += $(COMMON_DIR)/diag.c $(COMMON_DIR)/common.c $(COMMON_DIR)/sio.c $(COMMON_DIR)/cpu_util.c $(wildcard ./testapi/util/*.c)
CSOURCES += $(COMMON_DIR)/uart_printf.c

# mon
CSOURCES += mon/monitor.c

# interrupt
CSOURCES += $(TESTAPI)/interrupt/sp_interrupt.c
ASOURCES += $(TESTAPI)/interrupt/vectors.S


# NOC_TEST = ENABLE
ifeq "$(NOC_TEST)" "ENABLE"
	CFLAGS += -DNOC_TEST
	# noc test
	TEST_NOC = $(TESTAPI)/noc
	CSOURCES += $(wildcard $(TEST_NOC)/*.c)
endif

# QCH_TEST = ENABLE
ifeq "$(QCH_TEST)" "ENABLE"
	CFLAGS += -DQCH_TEST
	# noc test
	TEST_QCH = $(TESTAPI)/qch
	CSOURCES += $(wildcard $(TEST_QCH)/*.c)
endif


TEST_AXI_MON = $(TESTAPI)/axi_monitor
CSOURCES += $(wildcard $(TEST_AXI_MON)/*.c)


OBJS = $(ASOURCES:.S=.o) $(CSOURCES:.c=.o)


.PHONY: clean all

all: clean $(TARGET)


$(TARGET): $(OBJS)
	@-rm -Rf $(BIN)
	@mkdir -p $(BIN)
	$(CROSS)ld $(OBJS) -o $(BIN)/$@ -Map $(BIN)/$@.map $(LDFLAGS) $(LDFLAGS_COM)
	$(CROSS)objcopy -O binary -S $(BIN)/$@ $(BIN)/$@.bin
	$(CROSS)objdump -d -S $(BIN)/$@ > $(BIN)/$@.dis
	@# Add image header
	@echo "Wrap code image..."
	@bash ./script/add_uhdr.sh uboot $(BIN)/$@.bin $(BIN)/$@.img 0x200000 0x200000
	@sz=`du -sb bin/$@.img|cut -f1`;	printf "rom size = %d (hex %x)\n" $$sz $$sz
	bash ./script/update_all.sh $(SPI_ALL)

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
	@-rm -Rf $(BIN)
	@-rm -f pack.conf >/dev/null


p-%:
	@echo "$* is '$($*)'"
