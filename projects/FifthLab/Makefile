BUILD_ROOT=../..
OBJS = main.o

BINARY_NAME = main

include $(BUILD_ROOT)/common_defs.mk

.PHONY: $(BINARY_NAME)

$(BINARY_NAME): $(BINARY_NAME).bin

library:
	@$(MAKE) -C lib all

clean:
	@rm -f $(OBJS) main.elf main.bin main.elf.map
	@$(MAKE) -C lib clean

flash: $(BINARY_NAME)
	$(FLASH_BINARY)
