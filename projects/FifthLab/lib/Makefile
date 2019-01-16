BUILD_ROOT = ../../..

OBJS = library.o
DEPS = library.d

include $(BUILD_ROOT)/common_defs.mk

LIB = library.a


all: $(LIB)
	@echo "Done"

$(LIB): $(OBJS)
	@echo "Linking library $@"
	$(MAKE_LIBRARY)

clean:
	@echo "Cleaning library directory"
	@rm -f $(OBJS) $(DEPS) $(LIB)