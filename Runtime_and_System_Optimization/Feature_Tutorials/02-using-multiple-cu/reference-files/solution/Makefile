VPP := $(XILINX_VITIS)/bin/v++
EMCONFIGUTIL := $(XILINX_VITIS)/bin/emconfigutil
MODE := hw_emu
DSA := xilinx_u200_xdma_201830_2

# sources
HOST_SRC := src/host/cmdlineparser.cpp src/host/filter2d.cpp src/host/logger.cpp src/host/xclbin_helper.cpp src/host/host.cpp
HOST_SRC_FINAL := src/host/cmdlineparser.cpp src/host/filter2d.cpp src/host/logger.cpp src/host/xclbin_helper.cpp src/host/host_ooo.cpp
KERNEL_XO := src/kernel/Filter2DKernel.xo

# targets
HOST_EXE := filter2d.exe
HOST_EXE_FINAL := filter2d_final.exe
XCLBIN := filter2d.$(MODE).xclbin
XCLBIN_FINAL := filter2d_3k.$(MODE).xclbin
EMCONFIG_FILE := emconfig.json

# V++ Linker options to configure number of compute units
VPP_LINK_OPTS := --config link.cfg
VPP_LINK_OPTS_FINAL := --nk Filter2DKernel:3

# V++ Compiler options
VPP_COMMON_OPTS := -s -t $(MODE) --config compile.cfg

CFLAGS := -g -std=c++11 -I$(XILINX_XRT)/include -I${XILINX_VIVADO}/include
LFLAGS := -L$(XILINX_XRT)/lib -lxilinxopencl -lrt -fopenmp -Wl,--as-needed -Wl,-rpath,${XILINX_VIVADO}/lnx64/tools/opencv/opencv_clang -L${XILINX_VIVADO}/lnx64/tools/opencv/opencv_clang -lopencv_core -lopencv_highgui


NUMDEVICES := 1

# run time args
EXE_OPT := -x filter2d.${MODE}.xclbin -i ./img/test.bmp -n 1
EXE_OPT_FINAL := -x filter2d_3k.${MODE}.xclbin -i ./img/test.bmp -n 1

# primary build targets
.PHONY: xclbin app all

xclbin:  $(XCLBIN)
app: $(HOST_EXE)

all: xclbin app

clean:
	-$(RM) $(EMCONFIG_FILE) $(HOST_EXE) $(XCLBIN) *.xclbin *.xo $(XOS)

$(XCLBIN): $(KERNEL_XO)
	$(VPP) $(VPP_COMMON_OPTS) -l -o $@ $+ $(VPP_LINK_OPTS)

$(XCLBIN_FINAL): $(KERNEL_XO)
	$(VPP) $(VPP_COMMON_OPTS) -l -o $@ $+ $(VPP_LINK_OPTS_FINAL)

# host rules
$(HOST_EXE): $(HOST_SRC)
	g++ $(CFLAGS) -o $@ $+ $(LFLAGS)
	@echo 'Compiled Host Executable: $(HOST_EXE)'

$(HOST_EXE_FINAL): $(HOST_SRC_FINAL)
	g++ $(CFLAGS) -o $@ $+ $(LFLAGS)
	@echo 'Compiled Host Executable: $(HOST_EXE_FINAL)'

$(EMCONFIG_FILE):
	$(EMCONFIGUTIL) --nd $(NUMDEVICES) --od . --platform $(DSA)

check: $(XCLBIN) $(HOST_EXE) $(EMCONFIG_FILE)
	XCL_EMULATION_MODE=${MODE} ./$(HOST_EXE) $(EXE_OPT)

check_final: $(XCLBIN_FINAL) $(HOST_EXE_FINAL) $(EMCONFIG_FILE)
	XCL_EMULATION_MODE=${MODE} ./$(HOST_EXE_FINAL) $(EXE_OPT_FINAL)
