VPP := v++
EMCONFIGUTIL := emconfigutil
TARGET := hw
LAB := run1
DEVICE := xilinx_u250_gen3x16_xdma_4_1_202210_1

KRNL_SRC := ./src/kernel.cpp

# kernel targets
XOS := vadd.$(TARGET).xo
XCLBIN := vadd.$(TARGET).$(LAB).xclbin

# host target
HOST_EXE := host.exe

EMCONFIG_FILE := emconfig.json

#Compiler flags
VPP_COMMON_OPTS := -s -t $(TARGET) --platform $(DEVICE)

ifeq ($(LAB),$(filter $(LAB),run1))
  HOST_SRC := ./src/host.cpp
  VPP_LINK_OPTS := --config ./src/link.cfg
else
  HOST_SRC := ./src/host_hm.cpp
  VPP_LINK_OPTS := --config ./src/link_hm.cfg
endif
 
CFLAGS := -g -std=c++14 -I$(XILINX_XRT)/include

LFLAGS := -L$(XILINX_XRT)/lib -lxilinxopencl -pthread -lrt

#Run time arguments
EXE_OPT := vadd.$(TARGET).$(LAB).xclbin

# primary build targets
.PHONY: xclbin exe all clean

xclbin:  $(XCLBIN)
exe: $(HOST_EXE)

all: xclbin exe

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
	$(EMCONFIGUTIL) --nd 1 --od . --platform $(DEVICE)

run: $(XCLBIN) $(HOST_EXE) $(EMCONFIG_FILE)
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
	XCL_EMULATION_MODE=${TARGET} ./$(HOST_EXE) $(EXE_OPT)
else
	./$(HOST_EXE) $(EXE_OPT)
endif

