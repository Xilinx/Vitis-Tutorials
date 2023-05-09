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
TARGET := hw
PLATFORM ?= xilinx_u250_gen3x16_xdma_4_1_202210_1
HOST_EXE := host
XO := apply_watermark.$(TARGET).$(PLATFORM).xo
XCLBIN := apply_watermark.$(TARGET).$(PLATFORM).xclbin

# Host building global settings
CXXFLAGS := -I$(XILINX_XRT)/include/ -I$(XILINX_VIVADO)/include/ -Wall -O0 -g -std=c++11 -L$(XILINX_XRT)/lib/ -pthread -lrt -lstdc++
CXXFLAGS2 := -lOpenCL 

# Kernel compiler & linker global settings
KRNL_COMPILE_OPTS := -t $(TARGET) --platform $(PLATFORM) --config design.cfg
KRNL_LINK_OPTS := -s -R2

#build: $(HOST_EXE) $(XO) $(XCLBIN) emconfig
all: $(HOST_EXE) $(XO) $(XCLBIN) emconfig

exe: $(HOST_EXE)

xclbin: $(XO) $(XCLBIN)
# Building kernel
$(XO): ./src/krnl_watermarking.cl
	v++ $(KRNL_COMPILE_OPTS) -c -k apply_watermark -o'$@' '$<'
$(XCLBIN): $(XO)
	v++ $(KRNL_COMPILE_OPTS) $(KRNL_LINK_OPTS) -l -o'$@' $(+)

# Building Host
$(HOST_EXE): ./src/host.cpp
	g++ $(CXXFLAGS) -o '$@' '$<' $(CXXFLAGS2)

.PHONY: emconfig
emconfig: 
	emconfigutil --platform $(PLATFORM)

# Cleaning stuff
.PHONY: clean
clean:
	rm -f $(HOST_EXE) *apply_watermark.*.$(PLATFORM).* *.log *.json *.xo output.bmp

# Please set XCL_EMULATION_MODE manually before running sw_emu/hw_emu mode
.PHONY: run
run: all
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
	XCL_EMULATION_MODE=$(TARGET) ./$(HOST_EXE) $(XCLBIN) ./src/inputImage.bmp ./src/golden.bmp 
else
	./$(HOST_EXE) $(XCLBIN) ./src/inputImage.bmp ./src/golden.bmp
endif

