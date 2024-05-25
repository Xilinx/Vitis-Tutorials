#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

TARGET      ?= hw
PF_STDNAME  ?= base_pfm_vck190

XSA          = $(PF_STDNAME).xsa
BUILD_DIR    = build_$(TARGET)
MAIN_ELF     = main.elf

HOST_GCC 	 ?= aarch64-none-elf-gcc

PLATFORM_SW  = $(PF_STDNAME)/export/$(PF_STDNAME)/sw/$(PF_STDNAME)/
SW_PLATFORM  = ../link/$(BUILD_DIR)/$(PLATFORM_SW)
SOURCES      = ../link/$(BUILD_DIR) ../link/$(BUILD_DIR)/$(XSA)

#------------------------------------
# Define Test Iterations
#------------------------------------
ifeq ($(TARGET),"hw")
	NUMITE_MIN_HW  = 10000
        NUMITE_STP_HW  = 10000
        NUMTST_HW      = 4
	IS_COSIM       = 0
else
	NUMITE_MIN_HW  = 100
        NUMITE_STP_HW  = 100
        NUMTST_HW      = 1
	IS_COSIM       = 1
endif

.PHONY:	all clean cleanall

all:	$(BUILD_DIR) $(BUILD_DIR)/$(MAIN_ELF)

$(BUILD_DIR):	$(SOURCES)
	/bin/rm -rf $(BUILD_DIR)
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/$(MAIN_ELF):	$(SOURCES)
	cd $(BUILD_DIR) && { \
	echo "#define NUM_ITE_MIN $(NUMITE_MIN_HW)" >  ./test_cfg.h; \
	echo "#define NUM_ITE_STP $(NUMITE_STP_HW)" >> ./test_cfg.h; \
	echo "#define NUM_TST     $(NUMTST_HW)" >> ./test_cfg.h; \
	echo "#define IS_COSIM    $(IS_COSIM)" >> ./test_cfg.h ; \
	$(HOST_GCC) -I../$(SW_PLATFORM)/standalone_domain/bspinclude/include \
	        -I$(XILINX_VITIS)/aietools/include -I./ -c -std=c++11 -o main.o ../main.cpp; \
	$(HOST_GCC) -I../$(SW_PLATFORM)/standalone_domain/bspinclude/include \
	        -I$(XILINX_VITIS)/aietools/include -I./ -c -std=c++11 -o platform.o ../platform.cpp; \
	$(HOST_GCC) platform.o main.o -mcpu=cortex-a72 -Wl,-T -Wl,../../host/lscript.ld \
		-L../$(SW_PLATFORM)standalone_domain/bsplib/lib -L$(XILINX_VITIS)/aietools/lib/aarchnone64.o \
	        -Wl,--start-group,-lxil,-lgcc,-lc,-lstdc++,--end-group -o $(MAIN_ELF); }

clean:
	/bin/rm -rf $(BUILD_DIR)

cleanall:	clean
