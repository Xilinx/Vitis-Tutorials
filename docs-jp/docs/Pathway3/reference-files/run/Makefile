#######################################################################################
.PHONY: help
help:
	@echo "Makefile Usage:"
	@echo "  make build TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform>"
	@echo "      Command to generate the design for specified Target and Device."
	@echo ""
	@echo "  make run TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform>"
	@echo "      Command to run for specified Target."
	@echo ""
	@echo "  make exe "
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
PLATFORM := xilinx_u200_xdma_201830_2
HOST_EXE := host
XO := mmult.$(TARGET).$(PLATFORM).xo
XCLBIN := mmult.$(TARGET).$(PLATFORM).xclbin

# Host building global settings
CXXFLAGS := -I$(XILINX_XRT)/include/ -I$(XILINX_VIVADO)/include/ -Wall -O0 -g -std=c++11 -L$(XILINX_XRT)/lib/ -lpthread -lrt -lstdc++
CXXFLAGS2 := -lOpenCL

# Kernel compiler & linker global settings
KRNL_COMPILE_OPTS := -t $(TARGET) --config design.cfg
KRNL_LINK_OPTS := -t $(TARGET) --config design.cfg

build:  $(XO) $(XCLBIN) $(HOST_EXE) emconfig

exe: $(HOST_EXE)

xclbin: $(XO) $(XCLBIN)

# Building kernel
$(XO): ../src/mmult.cpp
	v++ $(KRNL_COMPILE_OPTS) -c -k mmult -I'$(<D)' -o'$@' '$<'
$(XCLBIN): $(XO)
	v++ $(KRNL_LINK_OPTS) -l -o'$@' $(+)

# Building Host
$(HOST_EXE): ../src/host.cpp
	g++ $(CXXFLAGS) -o '$@' '$<' $(CXXFLAGS2)

.PHONY: emconfig
emconfig:
	emconfigutil --platform $(PLATFORM)

.PHONY: run
run: build
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
	XCL_EMULATION_MODE=$(TARGET) ./$(HOST_EXE) $(XCLBIN)
else
	./$(HOST_EXE) $(XCLBIN)
endif

# Cleaning stuff
.PHONY: clean
clean:
	rm -f $(HOST_EXE) *mmult.$(TARGET).$(PLATFORM).* *.log *.json *.xo
