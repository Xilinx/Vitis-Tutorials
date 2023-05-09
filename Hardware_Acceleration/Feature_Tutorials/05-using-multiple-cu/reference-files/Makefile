VPP := v++
EMCONFIGUTIL := $(XILINX_VITIS)/bin/emconfigutil
TARGET := sw_emu
LAB := run1
DEVICE := xilinx_u250_gen3x16_xdma_4_1_202210_1

# sources
#HOST_SRC := src/host/*.cpp
KERNEL_SRC := src/kernel/*.cpp 
KERNEL_HEADER := src/kernel/*.h
KERNEL_XO := Filter2DKernel.$(TARGET).xo

# targets
HOST_EXE := filter2d.exe
XCLBIN := filter2d.$(TARGET).xclbin
EMCONFIG_FILE := emconfig.json

# v++ Linker options to configure number of compute units
#VPP_LINK_OPTS := --config link.cfg -I ./src/kernel

ifeq ($(LAB),$(filter $(LAB),run1))
   HOST_SRC := ./src/host/host.cpp ./src/host/cmdlineparser.cpp ./src/host/filter2d.cpp ./src/host/logger.cpp ./src/host/xclbin_helper.cpp
   VPP_LINK_OPTS := --config link.cfg -I ./src/kernel
else
   HOST_SRC := ./src/host/host-final.cpp ./src/host/cmdlineparser.cpp ./src/host/filter2d.cpp ./src/host/logger.cpp ./src/host/xclbin_helper.cpp
   VPP_LINK_OPTS := --config link3.cfg -I ./src/kernel
endif

# v++ Compiler options
VPP_COMMON_OPTS := -s -g -t $(TARGET) --platform $(DEVICE) 

CFLAGS := -g -std=c++11 -I$(XILINX_XRT)/include 
LFLAGS := -L$(XILINX_XRT)/lib -lxilinxopencl -lrt -fopenmp -Wl,--as-needed 

NUMDEVICES := 1

# run time args
EXE_OPT := -x filter2d.${TARGET}.xclbin -n 1

# primary build targets
.PHONY: xclbin host all

xclbin:  $(XCLBIN)
host: $(HOST_EXE)

all: xclbin host

clean:
	-$(RM) $(EMCONFIG_FILE) $(HOST_EXE) *.xclbin *.xo

$(XCLBIN): $(KERNEL_XO)
	$(VPP) $(VPP_COMMON_OPTS) -l -o $@ $+ $(VPP_LINK_OPTS)

$(KERNEL_XO) : $(KERNEL_SRC) $(KERNEL_HEADER)
	$(VPP) $(VPP_COMMON_OPTS) -c -k Filter2DKernel -o $@ $(KERNEL_SRC) -I ./src/kernel


# host rules
$(HOST_EXE): $(HOST_SRC)
	g++ $(CFLAGS) -o $@ $+ $(LFLAGS)
	@echo 'Compiled Host Executable: $(HOST_EXE)'

$(EMCONFIG_FILE):
	$(EMCONFIGUTIL) --nd $(NUMDEVICES) --od . --platform $(DEVICE)

run: $(XCLBIN) $(HOST_EXE) $(EMCONFIG_FILE)
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
	XCL_EMULATION_MODE=${TARGET} ./$(HOST_EXE) $(EXE_OPT)
else
	./$(HOST_EXE) $(EXE_OPT)
endif
