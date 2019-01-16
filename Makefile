BUILD_ROOT := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))


SPL_PATH = $(BUILD_ROOT)/stm_spl/
COMMON_PATH = $(BUILD_ROOT)/common
LABS_DIR = $(BUILD_ROOT)/projects

.PHONY: labs spl common FirstLab SecondLab ThirdLab FourthLab FifthLab clean

all: labs

labs: FirstLab SecondLab ThirdLab FourthLab FifthLab

spl:
	make -C $(SPL_PATH)

common:
	make -C $(COMMON_PATH)

spl:
	make -C $(SPL_PATH)

common:
	make -C $(COMMON_PATH)

FirstLab: spl common
	make -C $(LABS_DIR)/FirstLab

SecondLab: spl common
	make -C $(LABS_DIR)/SecondLab

ThirdLab: spl common
	make -C $(LABS_DIR)/ThirdLab

FourthLab: spl common
	make -C $(LABS_DIR)/FourthLab

FifthLab: spl common
	make -C $(LABS_DIR)/FifthLab

clean:
	make -C $(SPL_PATH) clean
	make -C $(COMMON_PATH) clean
	make -C $(LABS_DIR)/FirstLab clean
	make -C $(LABS_DIR)/SecondLab clean
	make -C $(LABS_DIR)/ThirdLab clean
	make -C $(LABS_DIR)/FourthLab clean
	make -C $(LABS_DIR)/FifthLab clean
