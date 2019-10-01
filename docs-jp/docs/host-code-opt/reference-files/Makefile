# Points to Utility Directory
COMMON_REPO = auxFiles
include $(COMMON_REPO)/utils.mk

# Run Target:
#   hw  - Compile for hardware
#   sw_emu/hw_emu - Compile for software/hardware emulation
# FPGA Board Platform (Default ~ vcu1525)

SIZE    := 14
TARGETS := hw
TARGET  := $(TARGETS)
DEVICES := xilinx_vcu1525_dynamic
DEVICE  := $(DEVICES)
XCLBIN  := ./xclbin
DSA := $(call device2sandsa, $(DEVICE))

CXX  := $(XILINX_SDX)/bin/xcpp
XOCC := $(XILINX_SDX)/bin/xocc

CXXFLAGS := $(opencl_CXXFLAGS) -Wall -O0 -g -std=c++14
LDFLAGS  := $(opencl_LDFLAGS)

HOST_SRCS = src/host.cpp

# Host compiler global settings
CXXFLAGS = -I$(XILINX_XRT)/include -I$(XILINX_VIVADO)/include/ -IsrcCommon/ -O0 -g -Wall -fmessage-length=0 -std=c++14
LDFLAGS = -lOpenCL -lpthread -lrt -lstdc++ -L$(XILINX_SDX)/runtime/lib/x86_64

# Kernel compiler global settings
CLFLAGS = -t $(TARGET) --platform $(DEVICE) --save-temps 

# Kernel linker flags
LDCLFLAGS =   --sp pass_1.m_axi_p0:bank0 --sp pass_1.m_axi_p1:bank1 

EXECUTABLE = pass

EMCONFIG_DIR = $(XCLBIN)/$(DSA)

BINARY_CONTAINERS += $(XCLBIN)/pass.$(TARGET).$(DSA).xclbin
BINARY_CONTAINER_pass_OBJS += $(XCLBIN)/pass.$(TARGET).$(DSA).xo

# Select Host code source based on target
.PHONY: pipeline
pipeline: HOST_SRCS= srcPipeline/host.cpp 
pipeline: BUILDDIR = runPipeline
pipeline: cleanExeBuildDir
pipeline: $(BUILDDIR)/$(EXECUTABLE)

.PHONY: runPipeline
pipelineRun:
	cp auxFiles/sdaccel.ini runPipeline
	cd runPipeline; \
	./$(EXECUTABLE) ../$(XCLBIN)/pass.$(TARGET).$(DSA).xclbin 
	cd runPipeline; \
	sdx_analyze profile -i sdaccel_profile_summary.csv -f html
	cd runPipeline; \
	sdx_analyze trace sdaccel_timeline_trace.csv -f wdb
	sdx -workspace workspace -report runPipeline/*.wdb
# sdx -workspace workspace -rundir runPipline

# Select Host code source based on target
.PHONY: sync
sync:	HOST_SRCS= srcSync/host.cpp
sync:	BUILDDIR = runSync
sync:   cleanExeBuildDir
sync:   $(BUILDDIR)/$(EXECUTABLE)

.PHONY: runSync
syncRun:
	cp auxFiles/sdaccel.ini runSync
	cd runSync; \
	./$(EXECUTABLE) ../$(XCLBIN)/pass.$(TARGET).$(DSA).xclbin 
	cd runSync; \
	sdx_analyze profile -i sdaccel_profile_summary.csv -f html
	cd runSync; \
	sdx_analyze trace sdaccel_timeline_trace.csv -f wdb
	sdx -workspace workspace -report runSync/*.wdb
# sdx -workspace workspace -rundir runPipline

# Select Host code source based on target
.PHONY: buf
buf:	HOST_SRCS= srcBuf/host.cpp
buf:	BUILDDIR = runBuf
buf:   cleanExeBuildDir
buf:   $(BUILDDIR)/$(EXECUTABLE)

.PHONY: runBuf
bufRun:
	cp auxFiles/sdaccel.ini runBuf
	cd runBuf; \
	./$(EXECUTABLE) ../$(XCLBIN)/pass.$(TARGET).$(DSA).xclbin $(SIZE)

# add code to check for gnuplot and if it exists try to pot otherwise just
# list data
bufRunSweep:
	cp auxFiles/sdaccel.ini runBuf
	cp auxFiles/run.py runBuf
	cd runBuf; \
	./run.py
	more runBuf/results.csv
	if hash gnuplot 2>/dev/null; then gnuplot -p -c auxFiles/plot.txt; fi;

#Include Libraries
include $(COMMON_REPO)/opencl.mk


CP = cp -rf

.PHONY: all clean cleanall docs emconfig
all: $(EXECUTABLE) $(BINARY_CONTAINERS) emconfig

.PHONY: exe
exe: $(EXECUTABLE)

.PHONY: kernel
kernel: $(BINARY_CONTAINERS)

# Building kernel
$(XCLBIN)/pass.$(TARGET).$(DSA).xo: ./srcKernel/pass.cpp
	mkdir -p $(XCLBIN)
	$(XOCC) $(CLFLAGS) -c -k pass -I'$(<D)' -o'$@' '$<'

$(XCLBIN)/pass.$(TARGET).$(DSA).xclbin: $(BINARY_CONTAINER_pass_OBJS)
	$(XOCC) $(CLFLAGS) -l $(LDCLFLAGS) --nk pass:1 -o'$@' $(+)

# Building Host
$(BUILDDIR)/$(EXECUTABLE): 
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(HOST_SRCS) $(HOST_HDRS) -o '$(BUILDDIR)/$(EXECUTABLE)' $(LDFLAGS)

emconfig:$(EMCONFIG_DIR)/emconfig.json
$(EMCONFIG_DIR)/emconfig.json:
	emconfigutil --platform $(DEVICE) --od $(EMCONFIG_DIR)


# Cleaning stuff
cleanExeBuildDir:
	-$(RMDIR) $(BUILDDIR)/$(EXECUTABLE)

clean:
	-$(RMDIR) $(XCLBIN)/{*sw_emu*,*hw_emu*}
	-$(RMDIR) workspace runBuf runSync runPipeline
	-$(RMDIR) $(XCLBIN)/*.xo $(XCLBIN)/*.ltx 

cleanall: clean
	-$(RMDIR) $(XCLBIN)

reset:	clean
	cp auxFiles/pipelineHost.cpp srcPipeline/host.cpp
	cp auxFiles/syncHost.cpp srcSync/host.cpp
	cp auxFiles/bufHost.cpp srcBuf/host.cpp

.PHONY: help

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all TARGETS=<sw_emu/hw_emu/hw> DEVICES=<FPGA platform>"
	$(ECHO) "      Command to generate the design for specified Target and Device."
	$(ECHO) "  make kernel TARGETS=<sw_emu/hw_emu/hw> DEVICES=<FPGA platform>"
	$(ECHO) "      Command compile just the kernel of the design for specified Target and Device."
	$(ECHO) ""
	$(ECHO) "  make clean "
	$(ECHO) "      Command to remove the generated non-hardware files."
	$(ECHO) ""
	$(ECHO) "  make cleanall"
	$(ECHO) "      Command to remove all the generated files."
	$(ECHO) ""

