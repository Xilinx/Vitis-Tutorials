#######################################################################################
.PHONY: help
help:
	@echo "Makefile Usage:"
	@echo "  make all TARGET=<sw_emu/hw_emu/hw> DEVICE=<FPGA platform>"
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
TARGET := sw_emu
DEVICE := xilinx_u200_xdma_201830_2
EXECUTABLE := host
XO := krnl_vadd.$(TARGET).$(DEVICE).xo
XCLBIN := krnl_vadd.$(TARGET).$(DEVICE).xclbin

# Host building global settings
CXXFLAGS := -I$(XILINX_XRT)/include/ -I$(XILINX_VIVADO)/include/ -Wall -O0 -g -std=c++11 -L$(XILINX_XRT)/lib/ -lOpenCL -lpthread -lrt -lstdc++
CXXFLAGS2 := -lOpenCL

# Kernel compiler & linker global settings
CLFLAGS := -t $(TARGET) --config compile.cfg
LDCLFLAGS := -t $(TARGET) --config link.cfg
LDCLFLAGS += --profile_kernel data:all:all:all
LDCLFLAGS += --profile_kernel stall:all:all:all
LDCLFLAGS += --profile_kernel exec:all:all:all
 

all: $(EXECUTABLE) $(XO) $(XCLBIN) emconfig

exe: $(EXECUTABLE)

xclbin: $(XO) $(XCLBIN)

# Building kernel
$(XO): ../src/kernel_cpp/krnl_vadd.cpp
	v++ $(CLFLAGS) -c -k krnl_vadd -I'$(<D)' -o'$@' '$<'
$(XCLBIN): krnl_vadd.$(TARGET).$(DEVICE).xo ../../script/workplace/rtl_project/src/rtl_kernel/rtl_kernel_wizard_0/rtl_kernel_wizard_0.xo
	v++ $(LDCLFLAGS) -l -o'$@' $(+)

# Building Host
$(EXECUTABLE): ../src/host/host_step2.cpp
	g++ $(CXXFLAGS) -o '$@' '$<' $(CXXFLAGS2)

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
	-$(RMDIR) _x*

