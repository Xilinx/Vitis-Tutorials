.PHONY: help

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all TARGET=<sw_emu/hw_emu/hw> DEVICE=<FPGA platform>"
	$(ECHO) "      Command to generate the design for specified Target and Device."
	$(ECHO) ""
	$(ECHO) "  make clean "
	$(ECHO) "      Command to remove the generated non-hardware files."
	$(ECHO) ""
	$(ECHO) "  make cleanall"
	$(ECHO) "      Command to remove all the generated files."
	$(ECHO) ""
	$(ECHO) "  make check TARGET=<sw_emu/hw_emu/hw> DEVICE=<FPGA platform>"
	$(ECHO) "      Command to run application in emulation."
	$(ECHO) ""
	$(ECHO) "  make run_nimbix DEVICE=<FPGA platform>"
	$(ECHO) "      Command to run application on Nimbix Cloud."
	$(ECHO) ""
	$(ECHO) "  make aws_build DEVICE=<FPGA platform>"
	$(ECHO) "      Command to build AWS xclbin application on AWS Cloud."
	$(ECHO) ""

ABS_COMMON_REPO = $(shell readlink -f $(COMMON_REPO))

HOST := user
TARGET := hw_emu
#TARGET := $(TARGETS)
DEVICE := xilinx_u200_gen3x16_xdma_2_202110_1 
XCLBIN := ./xclbin
XO := ./xo

#include ./utils.mk

DSA := $(call device2sandsa, $(DEVICE))
BUILD_DIR := ./_x.$(TARGET).$(DSA)

BUILD_DIR_vadd = $(BUILD_DIR)/vadd

CXX := g++
VPP := v++

RM = rm -f
RMDIR = rm -rf

ECHO:= @echo


###################################################################
# G++ COMPILER FLAGS
######################################################################
host_CXXFLAGS += -g -I./ -I$(XILINX_XRT)/include -I$(XILINX_VIVADO)/include -Wall -O0 -g -std=c++1y
# The below are linking flags for C++ Comnpiler
opencl_LDFLAGS += -L$(XILINX_XRT)/lib -lOpenCL -pthread
xrt_LDFLAGS += -L$(XILINX_XRT)/lib -lxrt_coreutil -pthread

CXXFLAGS += $(host_CXXFLAGS)

ifeq ($(HOST),$(filter $(HOST),user))
   LDFLAGS += $(xrt_LDFLAGS)
   HOST_SRCS += ./src/host/user-host.cpp
   XO := ./user-xo
   XCLBIN := ./user-xclbin
else ifeq ($(HOST),$(filter $(HOST),xrt))
   LDFLAGS += $(xrt_LDFLAGS)
   HOST_SRCS += ./src/host/xrt-host.cpp
else
   LDFLAGS += $(opencl_LDFLAGS)
   HOST_SRCS += ./src/host/opencl_host.cpp
endif

# Host compiler global settings
CXXFLAGS += -fmessage-length=0
LDFLAGS += -lrt -lstdc++


##########################################################################
# The below commands generate a XO file from a pre-exsisitng RTL kernel.
###########################################################################
VIVADO := $(XILINX_VIVADO)/bin/vivado
$(XO)/vadd.xo: ./src/xml/kernel.xml ./scripts/package_kernel.tcl ./scripts/gen_xo.tcl ./src/IP/*.sv ./src/IP/*.v
	mkdir -p $(XO)
	$(VIVADO) -mode batch -source scripts/gen_xo.tcl -tclargs $(XO)/vadd.xo vadd $(TARGET) $(DEVICE)
###########################################################################
#END OF GENERATION OF XO
##########################################################################

# Kernel compiler global settings
CLFLAGS += -t $(TARGET) --platform $(DEVICE) --save-temps

EXECUTABLE = host
CMD_ARGS = $(XCLBIN)/vadd.$(TARGET).xclbin

EMCONFIG_DIR = $(XCLBIN)/

BINARY_CONTAINERS += $(XCLBIN)/vadd.$(TARGET).xclbin
BINARY_CONTAINER_vadd_OBJS += $(XO)/vadd.xo

CP = cp -rf

.PHONY: all clean cleanall docs emconfig
all: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS) emconfig

.PHONY: exe
exe: $(EXECUTABLE)

.PHONY: xclbin
xclbin: $(XCLBIN)/vadd.$(TARGET).xclbin

# Building kernel
$(XCLBIN)/vadd.$(TARGET).xclbin: $(BINARY_CONTAINER_vadd_OBJS)
	mkdir -p $(XCLBIN)
	$(VPP) $(CLFLAGS) $(LDCLFLAGS) -l -o $(XCLBIN)/vadd.$(TARGET).xclbin $(XO)/vadd.xo
	$(CP) $(XCLBIN)/vadd.$(TARGET).xclbin ./vadd.xclbin

$(EXECUTABLE): $(HOST_SRCS)
	$(CXX) $(CXXFLAGS) $(HOST_SRCS) -o '$@' $(LDFLAGS)

emconfig:$(EMCONFIG_DIR)/emconfig.json
$(EMCONFIG_DIR)/emconfig.json:
	emconfigutil --platform $(DEVICE) --od $(EMCONFIG_DIR)

run: all
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
	$(CP) $(EMCONFIG_DIR)/emconfig.json .
	XCL_EMULATION_MODE=$(TARGET) ./$(EXECUTABLE) ./vadd.xclbin $(DEVICE)
else
	 ./$(EXECUTABLE) ./vadd.xclbin $(DEVICE)
endif

# Cleaning stuff
clean:
	#-$(RMDIR) $(EXECUTABLE) $(XCLBIN)/{*sw_emu*,*hw_emu*}
	-$(RMDIR) $(EXECUTABLE) $(XCLBIN)
	-$(RMDIR) *.log emconfig.json vadd.xclbin vivado.jou
	#-$(RMDIR) TempConfig system_estimate.xtxt *.rpt
	#-$(RMDIR) src/*.ll _v++_* .Xil emconfig.json dltmp* xmltmp* *.log *.jou

cleanall: clean
	-$(RMDIR) $(XCLBIN) $(XO)
	-$(RMDIR) _x
	-$(RMDIR) ./tmp_kernel_pack* ./packaged_kernel*

#######################################################################
# RTL Kernel only supports Hardware and Hardware Emulation.
# THis line is to check that
#########################################################################
ifneq ($(TARGET),$(findstring $(TARGET), hw hw_emu))
$(warning WARNING:Application supports only hw hw_emu TARGET. Please use the target for running the application)
endif

###################################################################
#check the devices avaiable
########################################################################

check-devices:
ifndef DEVICE
	$(error DEVICE not set. Please set the DEVICE properly and rerun. Run "make help" for more details.)
endif

############################################################################
# check the VITIS environment
#############################################################################

ifndef XILINX_VITIS
$(error XILINX_VITIS variable is not set, please set correctly and rerun)
endif


#################################################################
# Enable profiling if needed
#####################################################################a

REPORT := no
PROFILE := no
DEBUG := no

#'estimate' for estimate report generation
#'system' for system report generation
ifneq ($(REPORT), no)
CLFLAGS += --report estimate
CLLDFLAGS += --report system
endif

#Generates profile summary report
ifeq ($(PROFILE), yes)
LDCLFLAGS += --profile.data all:all:all
endif

#Generates debug summary report
ifeq ($(DEBUG), yes)
CLFLAGS += --debug.protocol all
endif
