BUILD_ROOT=../../..
OBJS = main.o


LED_DIR = ../lib

LIB_DIRS += $(LED_DIR)
INCLUDE_DIRS += -I$(LED_DIR)
LDLIBS += -lleds

BINARY_NAME = main

include $(BUILD_ROOT)/common_defs.mk

.PHONY: $(BINARY_NAME)

all: $(BINARY_NAME)

$(BINARY_NAME): $(BINARY_NAME).bin

clean:
	@rm -f $(OBJS) main.elf main.bin main.elf.map
flash: $(BINARY_NAME)
	$(FLASH_BINARY)
