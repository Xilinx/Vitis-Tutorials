VPP := $(XILINX_VITIS)/bin/v++
EMCONFIGUTIL := $(XILINX_VITIS)/bin/emconfigutil
MODE := hw
PLATFORM := xilinx_u250_gen3x16_xdma_3_1_202020_1


HOST_SRC := ./host.cpp
KRNL_SRC := ./kernel.cpp


# kernel targets
XOS := vadd.$(MODE).xo
XCLBIN := vadd.$(MODE).xclbin

# host target
HOST_EXE := host.exe


EMCONFIG_FILE := emconfig.json

#Compiler flags
VPP_COMMON_OPTS := -s -t $(MODE) --platform $(PLATFORM)
VPP_LINK_OPTS := --config link.cfg
CFLAGS := -g -std=c++14 -I$(XILINX_XRT)/include

LFLAGS := -L$(XILINX_XRT)/lib -lxilinxopencl -lpthread -lrt

#Run time arguments
EXE_OPT := vadd.$(MODE).xclbin

# primary build targets
.PHONY: xclbin app all clean

xclbin:  $(XCLBIN)
app: $(HOST_EXE)

all: xclbin app

clean:
	rm -rf $(EMCONFIG_FILE) $(HOST_EXE) 

# kernel rules
$(XOS): $(KRNL_SRC)
	$(VPP) $(VPP_COMMON_OPTS) -c -k vadd -o $@ $+


$(XCLBIN): $(XOS)
	$(VPP) $(VPP_COMMON_OPTS) -l -o $@ $+ $(VPP_LINK_OPTS)

# host rules

$(HOST_EXE): $(HOST_SRC)
	g++ $(CFLAGS) -o $@ $+ $(LFLAGS)
	@echo 'Compiled Host Executable: $(HOST_EXE)'

$(EMCONFIG_FILE):
	$(EMCONFIGUTIL) --nd 1 --od . --platform $(DSA)

check: $(XCLBIN) $(HOST_EXE) $(EMCONFIG_FILE)
	XCL_EMULATION_MODE=${MODE} ./$(HOST_EXE) $(EXE_OPT)
