#######################################################################################
.PHONY: help
help:
	@echo "Makefile Usage:"
	@echo "  make all TARGET=<sw_emu/hw_emu/hw> DEVICE=<FPGA platform> LAB=<run1/run2>"
	@echo "      Command to generate the design for specified Target and Device."
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
TARGET := hw_emu
DEVICE := xilinx_u250_gen3x16_xdma_4_1_202210_1
LAB := run1

ifeq ($(TARGET),sw_emu)
	XO := krnl_vadd.sw_emu.xo
else
	XO := krnl_vadd.hw.xo
endif

XCLBIN := krnl_vadd.$(TARGET).xclbin

#Locate RTL Kernel
RTL_KRNL := ./src/rtl_kernel/rtl_kernel_wizard_0.xo

# Host building global settings
CXXFLAGS += -g -std=c++17 -Wall -O0

# Kernel compiler & linker global settings
CLFLAGS := -t $(TARGET) --platform $(DEVICE)

ifeq ($(LAB),$(filter $(LAB),run1))
   EXECUTABLE := host_1
   LDCLFLAGS := -t $(TARGET) --platform $(DEVICE) --config run1.cfg
else
   EXECUTABLE := host_2
   LDCLFLAGS := -t $(TARGET) --platform $(DEVICE) --config run2.cfg
endif

all: $(EXECUTABLE) $(XO) $(XCLBIN) emconfig

exe: $(EXECUTABLE)

xclbin: $(XO) $(XCLBIN)

# Building kernel
$(XO): ./src/kernel_cpp/krnl_vadd.cpp
ifeq ($(TARGET),sw_emu)
	v++ $(CLFLAGS) -c -k krnl_vadd -g -o'$@' '$<'
else
	v++ --platform $(DEVICE) -c --mode hls --config hls_config.cfg
endif

$(XCLBIN): $(XO) 
ifeq ($(LAB),$(filter $(LAB),run1))
	v++ $(LDCLFLAGS) -l -o'$@' $(+)
else
	v++ $(LDCLFLAGS) -l -o'$@' $(+) $(RTL_KRNL)
endif


# Building Executable
ifeq ($(LAB),$(filter $(LAB),run1))
$(EXECUTABLE): ./src/host/xrt-host_step1.cpp
	g++ $(CXXFLAGS) -o '$@' '$<'  \
		-I${XILINX_XRT}/include/ \
                -L$(XILINX_XRT)/lib -lxrt_coreutil -pthread
else
$(EXECUTABLE): ./src/host/xrt-host_step2.cpp
	g++ $(CXXFLAGS) -o '$@' '$<'  \
		-I${XILINX_XRT}/include/ \
                -L$(XILINX_XRT)/lib -lxrt_coreutil -pthread
endif


.PHONY: emconfig
emconfig:
	emconfigutil --platform $(DEVICE)

.PHONY: run
run: all
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
	XCL_EMULATION_MODE=$(TARGET) ./$(EXECUTABLE) $(XCLBIN)
else
	./$(EXECUTABLE) $(XCLBIN)
endif


# Cleaning stuff
.PHONY: clean

RMDIR = rm -rf

clean:
	-$(RMDIR) $(EXECUTABLE) $(XCLBIN)/{*sw_emu*,*hw_emu*}
	-$(RMDIR) TempConfig system_estimate.xtxt *.rpt
	-$(RMDIR) *.protoinst _v++_* .Xil emconfig.json dltmp* xmltmp* *.log *.jou 

cleanall: clean
	-$(RMDIR) $(XCLBIN) *.xo *.xclbin* *.wcfg *.wdb *.csv *.compile_summary *.run_summary
	-$(RMDIR) _x* .run/

