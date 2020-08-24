VPP := $(XILINX_VITIS)/bin/v++
EMCONFIGUTIL := $(XILINX_VITIS)/bin/emconfigutil
MODE := hw_emu
DSA := xilinx_u200_xdma_201830_2


# sources
HOST_SRC := src/host/*.cpp
KERNEL_SRC := src/kernel/*.cpp 
KERNEL_HEADER := src/kernel/*.h
KERNEL_XO := Filter2DKernel.$(MODE).xo

# targets
HOST_EXE := filter2d.exe
XCLBIN := filter2d.$(MODE).xclbin
EMCONFIG_FILE := emconfig.json

# v++ Linker options to configure number of compute units
VPP_LINK_OPTS := --config link.cfg

# v++ Compiler options
VPP_COMMON_OPTS := -s -t $(MODE) --config compile.cfg

CFLAGS := -g -std=c++11 -I$(XILINX_XRT)/include -I${XILINX_VIVADO}/include
CFLAGS += -I./opencv
LFLAGS := -L$(XILINX_XRT)/lib -lxilinxopencl -lrt -fopenmp -Wl,--as-needed -Wl,-rpath,./opencv/opencv_gcc -L./opencv/opencv_gcc -lopencv_core -lopencv_highgui

NUMDEVICES := 1

# run time args
EXE_OPT := -x filter2d.${MODE}.xclbin -i ./img/test.bmp -n 1

# primary build targets
.PHONY: xclbin app all

xclbin:  $(XCLBIN)
app: $(HOST_EXE)

all: xclbin app

clean:
	-$(RM) $(EMCONFIG_FILE) $(HOST_EXE) $(XCLBIN)

$(XCLBIN): $(KERNEL_XO)
	$(VPP) $(VPP_COMMON_OPTS) -l --profile_kernel data:all:all:all:all -o $@ $+ $(VPP_LINK_OPTS)

$(KERNEL_XO) : $(KERNEL_SRC) $(KERNEL_HEADER)
	$(VPP) $(VPP_COMMON_OPTS) -c -k Filter2DKernel -o $@ $(KERNEL_SRC)
	

# host rules
$(HOST_EXE): $(HOST_SRC)
	$(XILINX_VITIS)/bin/xcpp $(CFLAGS) -o $@ $+ $(LFLAGS)
	@echo 'Compiled Host Executable: $(HOST_EXE)'

$(EMCONFIG_FILE):
	$(EMCONFIGUTIL) --nd $(NUMDEVICES) --od . --platform $(DSA)

check: $(XCLBIN) $(HOST_EXE) $(EMCONFIG_FILE)
ifeq ($(MODE),$(filter $(MODE),sw_emu hw_emu))
	XCL_EMULATION_MODE=${MODE} ./$(HOST_EXE) $(EXE_OPT)
else
	./$(HOST_EXE) $(EXE_OPT)
endif
