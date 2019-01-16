BUILD_ROOT = ../../..

OBJS = leds.o
DEPS = leds.d

include $(BUILD_ROOT)/common_defs.mk

LIB = leds.a


all: $(LIB)
	@echo "Done"

$(LIB): $(OBJS)
	@echo "Linking library $@"
	$(MAKE_LIBRARY)

clean:
	@echo "Cleaning library directory"
	@rm -f $(OBJS) $(DEPS) $(LIB)