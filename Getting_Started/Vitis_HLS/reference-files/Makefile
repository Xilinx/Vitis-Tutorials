#######################################################################################
.PHONY: help
help:
	@echo "Makefile Usage:"
	@echo "  make all TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform>"
	@echo "      Command to generate the design for specified Target and Device."
	@echo ""
	@echo "  make run TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform>"
	@echo "      Command to run for specified Target."
	@echo ""
	@echo "  make host "
	@echo "      Command to generate host."
	@echo ""
	@echo "  make xclbin "
	@echo "      Command to generate hardware platform files(xo,xclbin)."
	@echo ""
	@echo "  make clean "
	@echo "      Command to remove the generated files."
	@echo ""
#######################################################################################
TARGET := sw_emu
PLATFORM ?= xilinx_u200_gen3x16_xdma_2_202110_1
HOST_EXE := dct_top
XO := dct.$(TARGET).$(PLATFORM).xo
XCLBIN := dct.$(TARGET).$(PLATFORM).xclbin

# Host building global settings
CXXFLAGS := -I$(XILINX_XRT)/include/ -I$(XILINX_VIVADO)/include/ -Wall -O0 -g -std=c++11 -L$(XILINX_XRT)/lib/ -pthread -lrt -lstdc++
CXXFLAGS2 := -lOpenCL

# Kernel compiler & linker global settings
KRNL_COMPILE_OPTS := -t $(TARGET) --config design.cfg
KRNL_LINK_OPTS := -t $(TARGET) --config design.cfg

all:  $(XO) $(XCLBIN) $(HOST_EXE) emconfig

host: $(HOST_EXE)

xclbin: $(XO) $(XCLBIN)

# Building kernel
$(XO): ./src/dct.cpp
	v++ $(KRNL_COMPILE_OPTS) -c --platform $(PLATFORM) -k dct -I'$(<D)' -o'$@' '$<'
$(XCLBIN): $(XO)
	v++ $(KRNL_LINK_OPTS) -l --platform $(PLATFORM) -o'$@' $(+)

# Building Host
$(HOST_EXE): ./src/dct_top.cpp
	g++ $(CXXFLAGS) -o '$@' '$<' $(CXXFLAGS2)

.PHONY: emconfig
emconfig:
	emconfigutil --platform $(PLATFORM)

.PHONY: run
run: all
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
	XCL_EMULATION_MODE=$(TARGET) ./$(HOST_EXE) $(XCLBIN)
else
	./$(HOST_EXE) $(XCLBIN)
endif

# Cleaning stuff
.PHONY: clean
clean:
	rm -f $(HOST_EXE) *dct.$(TARGET).$(PLATFORM).* *.log *.json *.xo
