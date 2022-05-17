# Points to Utility Directory
COMMON_REPO = auxFiles
include $(COMMON_REPO)/utils.mk

# Run Target:
#   hw  - Compile for hardware
#   sw_emu/hw_emu - Compile for software/hardware emulation
# FPGA Board Platform (Default ~ vcu1525)

SIZE := 14
TARGET := hw_emu
#TARGETS := hw_emu
#TARGET  := $(TARGETS)
DEVICE := xilinx_u200_gen3x16_xdma_2_202110_1
LAB	:= pipeline
#BUILDIR	:= $(LAB)
XCLBIN  := ./xclbin
#DSA := $(call device2sandsa, $(DEVICE))

CXX  := g++
VPP := $(XILINX_VITIS)/bin/v++

#HOST_SRCS = src/host.cpp
HOST_SRCS = src/$(LAB)_host.cpp

# Host compiler global settings
CXXFLAGS = -I$(XILINX_XRT)/include -I$(XILINX_HLS)/include/ -IsrcCommon/ -O0 -g -Wall -fmessage-length=0 -std=c++11
LDFLAGS = -lOpenCL -pthread -lrt -lstdc++ -L$(XILINX_VITIS)/runtime/lib/x86_64

# Kernel compiler global settings
CLFLAGS = -t $(TARGET) --platform $(DEVICE) --save-temps

# Xclbin linker flags
LDCLFLAGS = --config design.cfg

EXECUTABLE = pass

#EMCONFIG_DIR = $(XCLBIN)

BINARY_CONTAINERS += $(XCLBIN)/pass.$(TARGET).$(DEVICE).xclbin
BINARY_CONTAINER_pass_OBJS += $(XCLBIN)/pass.$(TARGET).xo

# Select Host code source based on target
.PHONY: build
build: HOST_SRCS= src/$(LAB)_host.cpp
build: cleanExeBuildDir
build: $(LAB)/$(EXECUTABLE)

.PHONY: run
run: $(LAB)/$(EXECUTABLE) $(BINARY_CONTAINERS) emconfig
	cp auxFiles/xrt.ini $(LAB)
ifeq ($(LAB),$(filter $(LAB), pipeline sync))
   ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
	cd $(LAB); export XCL_EMULATION_MODE=${TARGET}; ./$(EXECUTABLE) ../$(BINARY_CONTAINERS)
   else
	cd $(LAB); ./$(EXECUTABLE) ../$(BINARY_CONTAINERS)
   endif
else
   ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
	cd $(LAB); export XCL_EMULATION_MODE=${TARGET}; ./$(EXECUTABLE) ../$(BINARY_CONTAINERS) $(SIZE)
   else
	cd $(LAB); ./$(EXECUTABLE) ../$(BINARY_CONTAINERS) $(SIZE)
   endif
endif


# add code to check for gnuplot and if it exists try to pot otherwise just list data
bufRunSweep:
	cp auxFiles/xrt.ini buf
	cp auxFiles/run.py buf
	cd buf; ./run.py $(DSA)
	more buf/results.csv
	if hash gnuplot 2>/dev/null; then gnuplot -p -c auxFiles/plot.txt; fi;


#.PHONY: all clean cleanall emconfig
#all: $(EXECUTABLE) $(BINARY_CONTAINERS) emconfig
#all: $(LAB)/$(EXECUTABLE) $(BINARY_CONTAINERS) emconfig

.PHONY: exe
exe: $(LAB)/$(EXECUTABLE)

# Building Host
#$(EXECUTABLE):
$(LAB)/$(EXECUTABLE):
	mkdir -p $(LAB)
	$(CXX) $(CXXFLAGS) $(HOST_SRCS) $(HOST_HDRS) -o '$(LAB)/$(EXECUTABLE)' $(LDFLAGS)

.PHONY: xclbin
xclbin: $(BINARY_CONTAINERS)

# Building xclbin
$(XCLBIN)/pass.$(TARGET).xo: ./src/pass.cpp
	mkdir -p $(XCLBIN)
	$(VPP) $(CLFLAGS) -c -k pass -I'$(<D)' -o'$@' '$<'

#$(XCLBIN)/pass.$(TARGET).xclbin: $(BINARY_CONTAINER_pass_OBJS)
$(BINARY_CONTAINERS): $(BINARY_CONTAINER_pass_OBJS)
	$(VPP) $(CLFLAGS) -l $(LDCLFLAGS) -o'$@' $(+)

# Building Host
#$(BUILDIR)/$(EXECUTABLE):
#	mkdir -p $(BUILDIR)
#	$(CXX) $(CXXFLAGS) $(HOST_SRCS) $(HOST_HDRS) -o '$(BUILDIR)/$(EXECUTABLE)' $(LDFLAGS)

emconfig:$(LAB)/emconfig.json
$(LAB)/emconfig.json:
	emconfigutil --platform $(DEVICE) --od $(LAB)


# Cleaning stuff
cleanExeBuildDir:
	-$(RMDIR) $(LAB)

clean:
	-$(RMDIR) $(XCLBIN)/{*sw_emu*,*hw_emu*}
	-$(RMDIR) workspace buf sync pipeline
	-$(RMDIR) $(XCLBIN)/*.xo $(XCLBIN)/*.ltx

cleanall: clean
	-$(RMDIR) $(XCLBIN)

reset:  clean
	cp auxFiles/*host.cpp src

.PHONY: help

help::
        $(ECHO) "Makefile Usage:"
	$(ECHO) "  make all TARGET=<sw_emu/hw_emu/hw> DEVICE=<FPGA platform> LAB=<pipeline/sync/buf>"
	$(ECHO) "      Command to generate the design for specified Target and Device."
	$(ECHO) "  make kernel TARGET=<sw_emu/hw_emu/hw> DEVICE=<FPGA platform>"
	$(ECHO) "      Command compile just the kernel of the design for specified Target and Device."
	$(ECHO) ""
	$(ECHO) "  make clean "
	$(ECHO) "      Command to remove the generated non-hardware files."
	$(ECHO) ""
	$(ECHO) "  make cleanall"
	$(ECHO) "      Command to remove all the generated files."
	$(ECHO) ""
