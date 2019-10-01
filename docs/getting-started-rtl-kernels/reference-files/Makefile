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

# Points to Utility Directory
#COMMON_REPO = ../../../
#COMMON_REPO = /home/aayyagar/scratch/LABS_VITIS/SDAccel_Examples

ABS_COMMON_REPO = $(shell readlink -f $(COMMON_REPO))

TARGETS := hw
TARGET := $(TARGETS)
DEVICE := $(DEVICES)
XCLBIN := ./xclbin

#include ./utils.mk

DSA :=
#$(call device2sandsa, $(DEVICE))
BUILD_DIR := ./_x.$(TARGET).$(DSA)

BUILD_DIR_vadd = $(BUILD_DIR)/vadd

CXX := g++
VPP := $(XILINX_VITIS)/bin/v++

RM = rm -f
RMDIR = rm -rf

ECHO:= @echo


###################################################################
# XCPP COMPILER FLAGS
######################################################################
opencl_CXXFLAGS += -g -I./ -I$(XILINX_XRT)/include -I$(XILINX_VIVADO)/include -Wall -O0 -g -std=c++14
# The below are linking flags for C++ Comnpiler
opencl_LDFLAGS += -L$(XILINX_XRT)/lib -lOpenCL -lpthread



##########################################################################
# The below commands generate a XO file from a pre-exsisitng RTL kernel.
###########################################################################
VIVADO := $(XILINX_VIVADO)/bin/vivado
$(XCLBIN)/vadd.$(TARGET).xo: ./src/xml/kernel.xml ./scripts/package_kernel.tcl ./scripts/gen_xo.tcl ./src/IP/*.sv ./src/IP/*.v
	mkdir -p $(XCLBIN)
	$(VIVADO) -mode batch -source scripts/gen_xo.tcl -tclargs $(XCLBIN)/vadd.$(TARGET).xo vadd $(TARGET) $(DEVICE)
###########################################################################
#END OF GENERATION OF XO
##########################################################################

CXXFLAGS += $(opencl_CXXFLAGS)
LDFLAGS += $(opencl_LDFLAGS)

HOST_SRCS += ./src/host/host.cpp

# Host compiler global settings
CXXFLAGS += -fmessage-length=0
LDFLAGS += -lrt -lstdc++

# Kernel compiler global settings
CLFLAGS += -t $(TARGET) --platform $(DEVICE) --save-temps


EXECUTABLE = host
CMD_ARGS = $(XCLBIN)/vadd.$(TARGET).xclbin

EMCONFIG_DIR = $(XCLBIN)/

BINARY_CONTAINERS += $(XCLBIN)/vadd.$(TARGET).xclbin
BINARY_CONTAINER_vadd_OBJS += $(XCLBIN)/vadd.$(TARGET).xo

CP = cp -rf

.PHONY: all clean cleanall docs emconfig
all: check-devices $(EXECUTABLE) $(BINARY_CONTAINERS) emconfig

.PHONY: exe
exe: $(EXECUTABLE)

# Building kernel
$(XCLBIN)/vadd.$(TARGET).xclbin: $(BINARY_CONTAINER_vadd_OBJS)
	mkdir -p $(XCLBIN)
	$(VPP) $(CLFLAGS) $(LDCLFLAGS) -lo $(XCLBIN)/vadd.$(TARGET).xclbin $(XCLBIN)/vadd.$(TARGET).xo

# Building Host
$(EXECUTABLE): $(HOST_SRCS) $(HOST_HDRS)
	mkdir -p $(XCLBIN)
	$(CXX) $(CXXFLAGS) $(HOST_SRCS) $(HOST_HDRS) -o '$@' $(LDFLAGS)

emconfig:$(EMCONFIG_DIR)/emconfig.json
$(EMCONFIG_DIR)/emconfig.json:
	emconfigutil --platform $(DEVICE) --od $(EMCONFIG_DIR)

check: all
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
	$(CP) $(EMCONFIG_DIR)/emconfig.json .
	XCL_EMULATION_MODE=$(TARGET) ./$(EXECUTABLE) $(XCLBIN)/vadd.$(TARGET).xclbin $(DEVICE)
else
	 ./$(EXECUTABLE) $(XCLBIN)/vadd.$(TARGET).xclbin $(DEVICE)
endif

# Cleaning stuff
clean:
	-$(RMDIR) $(EXECUTABLE) $(XCLBIN)/{*sw_emu*,*hw_emu*}
	-$(RMDIR) TempConfig system_estimate.xtxt *.rpt
	-$(RMDIR) src/*.ll _v++_* .Xil emconfig.json dltmp* xmltmp* *.log *.jou

cleanall: clean
	-$(RMDIR) $(XCLBIN)
	-$(RMDIR) _x.*
	-$(RMDIR) ./tmp_kernel_pack* ./packaged_kernel* _x/

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
LDCLFLAGS += --profile_kernel data:all:all:all
endif

#Generates debug summary report
ifeq ($(DEBUG), yes)
CLFLAGS += --dk protocol:all:all:all
endif
