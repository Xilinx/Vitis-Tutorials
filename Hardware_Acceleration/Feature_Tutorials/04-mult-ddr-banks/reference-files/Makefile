VPP := $(XILINX_VITIS)/bin/v++
EMCONFIGUTIL := $(XILINX_VITIS)/bin/emconfigutil
TARGET := hw_emu
LAB := run1
PLATFORM ?= xilinx_u250_gen3x16_xdma_4_1_202210_1

# sources
KERNEL_SRC := src/vadd.cpp
HOST_SRC := src/host.cpp

# targets
HOST_EXE := host.exe

XOS := vadd.$(TARGET).xo
XCLBIN := vadd.$(TARGET).xclbin
EMCONFIG_FILE := emconfig.json

VPP_LINK_OPTS := --profile.data all:all:all
# Linker options to map kernel ports to DDR banks
ifneq ($(LAB),$(filter $(LAB),run1))
   VPP_LINK_OPTS += --config connectivity.cfg
endif

VPP_COMMON_OPTS := -s -t $(TARGET) --platform $(PLATFORM)
CXXFLAGS += -g -std=c++17 -I$(XILINX_XRT)/include
LFLAGS := -L$(XILINX_XRT)/lib -lxilinxopencl -pthread -lrt -lxrt_coreutil
NUMDEVICES := 1

# run time args
EXE_OPT := vadd.$(TARGET).xclbin

# primary build targets
.PHONY: xclbin host all

xclbin:  $(XCLBIN)
host: $(HOST_EXE)

all: xclbin host

clean:
	-$(RM) $(EMCONFIG_FILE) $(HOST_EXE) $(XCLBIN) *.xclbin *.xo $(XOS)

# kernel rules
$(XOS): $(KERNEL_SRC)
	$(RM) $@
	$(VPP) $(VPP_COMMON_OPTS) -c -k vadd -o $@ $+

$(XCLBIN): $(XOS)
	$(VPP) $(VPP_COMMON_OPTS) -l -o $@ $+ $(VPP_LINK_OPTS)

# host rules
$(HOST_EXE): $(HOST_SRC)
	g++ $(CXXFLAGS) -o $@ $+ $(LFLAGS)
	@echo 'Compiled Host Executable: $(HOST_EXE)'

$(EMCONFIG_FILE):
	$(EMCONFIGUTIL) --nd $(NUMDEVICES) --od . --platform $(PLATFORM)

run: $(XCLBIN) $(HOST_EXE) $(EMCONFIG_FILE)
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
	export XCL_EMULATION_MODE=${TARGET}; ./$(HOST_EXE) $(EXE_OPT)
else
	./$(HOST_EXE) $(EXE_OPT)
endif

.PHONY: help

help::
        $(ECHO) "Makefile Usage:"
	$(ECHO) "  make all TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform> LAB=<run1/run2>"
	$(ECHO) "      Command to generate the design for specified Target and Device for LAB step 1 or 2."
	$(ECHO) ""
	$(ECHO) "  make xclbin TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform> LAB=<run1/run2>"
	$(ECHO) "      Command compile just the kernel of the design for specified Target and Device for LAB step 1 or 2."
	$(ECHO) ""
	$(ECHO) "  make host TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform>"
	$(ECHO) "      Command to generate the host application for specified Target and Device."
	$(ECHO) ""
	$(ECHO) "  make run TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform> LAB=<run1/run2>"
	$(ECHO) "      Command compile just the kernel of the design for specified Target and Device for LAB step 1 or 2."
	$(ECHO) ""
	$(ECHO) "  make clean "
	$(ECHO) "      Command to remove the generated non-hardware files."
	$(ECHO) ""
	$(ECHO) "  make cleanall"
	$(ECHO) "      Command to remove all the generated files."
	$(ECHO) ""
