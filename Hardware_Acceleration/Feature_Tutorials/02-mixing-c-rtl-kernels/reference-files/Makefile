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
EXECUTABLE := host
XO := krnl_vadd.$(TARGET).$(DEVICE).xo
XCLBIN := krnl_vadd.$(TARGET).$(DEVICE).xclbin

RTL_KRNL := ./src/rtl_kernel/rtl_kernel_wizard_0.xo

# Host building global settings
CXXFLAGS += -I$(XILINX_XRT)/include/ -I$(XILINX_VIVADO)/include/ -Wall -O0 -g -std=c++11 -L$(XILINX_XRT)/lib/ -lOpenCL -pthread -lrt -lstdc++
CXXFLAGS2 := -lOpenCL

# Kernel compiler & linker global settings
#CLFLAGS := -t $(TARGET) --platform $(DEVICE) --config compile.cfg
#LDCLFLAGS := -t $(TARGET) --platform $(DEVICE) --config link.cfg
CLFLAGS := -t $(TARGET) --platform $(DEVICE) -g

ifeq ($(LAB),$(filter $(LAB),run1))
LDCLFLAGS := -t $(TARGET) --platform $(DEVICE) --config run1.cfg
else
LDCLFLAGS := -t $(TARGET) --platform $(DEVICE) --config run2.cfg
endif

#LDCLFLAGS += --profile.data:all:all:all
#LDCLFLAGS += --profile.stall:all:all:all
#LDCLFLAGS += --profile.exec:all:all:all

all: $(EXECUTABLE) $(XO) $(XCLBIN) emconfig

exe: $(EXECUTABLE)

xclbin: $(XO) $(XCLBIN)

# Building kernel
$(XO): ./src/kernel_cpp/krnl_vadd.cpp
	v++ $(CLFLAGS) -c -k krnl_vadd -I'$(<D)' -o'$@' '$<'
#$(XCLBIN): krnl_vadd.$(TARGET).$(DEVICE).xo ../../script/workplace/rtl_project/src/rtl_kernel/rtl_kernel_wizard_0/rtl_kernel_wizard_0.xo
#	v++ $(LDCLFLAGS) -l -o'$@' $(+)

ifeq ($(LAB),$(filter $(LAB),run1))
$(XCLBIN): krnl_vadd.$(TARGET).$(DEVICE).xo 
	v++ $(LDCLFLAGS) -l -o'$@' $(+)
else
#$(XCLBIN): krnl_vadd.$(TARGET).$(DEVICE).xo ../script/workplace/rtl_project/src/rtl_kernel/rtl_kernel_wizard_0/rtl_kernel_wizard_0.xo
#	v++ $(CLFLAGS) -l -o'$@' $(+)
$(XCLBIN): krnl_vadd.$(TARGET).$(DEVICE).xo
	v++ $(LDCLFLAGS) -l -o'$@' $(+) $(RTL_KRNL)
endif


# Building Host
#$(EXECUTABLE): ./src/host/host_step2.cpp
#	g++ $(CXXFLAGS) -o '$@' '$<' $(CXXFLAGS2)

ifeq ($(LAB),$(filter $(LAB),run1))
$(EXECUTABLE): ./src/host/host_step1.cpp
	g++ $(CXXFLAGS) -o '$@' '$<' $(CXXFLAGS2)
else
$(EXECUTABLE): ./src/host/host_step2.cpp
	g++ $(CXXFLAGS) -o '$@' '$<' $(CXXFLAGS2)
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

