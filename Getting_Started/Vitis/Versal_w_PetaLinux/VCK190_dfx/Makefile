#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc.
#SPDX-License-Identifier: MIT
#*/

ROOT_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))
PLATFORM = $(PLATFORM_REPO_PATHS)/xilinx_vck190_base_202520_1/xilinx_vck190_base_202520_1.xpfm
VERSION  := 2025.2
COMMON_IMAGE_VERSAL ?= ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v$(VERSION)/
SYSROOT := $(COMMON_IMAGE_VERSAL)/sysroots/cortexa72-cortexa53-amd-linux/

# === Put ALL build artifacts under this temp root ===
TMP_ROOT ?= $(ROOT_DIR)/tmp

.PHONY: help
help:
	@echo "Makefile usage"
	@echo "  make build_xsa     ## build both dfx1.xsa and dfx2.xsa into tmp/"
	@echo "  make build_xclbin  ## build both dfx1.xclbin and dfx2.xclbin into tmp/"
	@echo "  make clean         ## remove the tmp/ directory"

TARGET ?= hw
ifeq ($(TARGET),sw_emu)
  DFX1_AIE_FLAGS += --target=x86sim -Xpreproc='-DNDEBUG'
  DFX2_AIE_FLAGS += --target=x86sim -Xpreproc='-DNDEBUG'
else
  DFX1_AIE_FLAGS += --target=hw
  DFX2_AIE_FLAGS += --target=hw
endif

# ----------------------------- Task 1 paths -----------------------------
DFX1_DIR           := $(ROOT_DIR)/dfx1
DFX1_BUILD_DIR     := $(TMP_ROOT)/dfx1
DFX1_GRAPH         := $(DFX1_DIR)/aie/graph.cpp
DFX1_AIE_WORK      := $(DFX1_BUILD_DIR)/Work
DFX1_GRAPH_O       := $(DFX1_BUILD_DIR)/libadf.a
DFX1_LINK_OUTPUT   := $(DFX1_BUILD_DIR)/dfx1.xsa
DFX1_XCLBIN        := $(DFX1_BUILD_DIR)/dfx1.xclbin
DFX1_LINK_CONFIG   := $(DFX1_DIR)/system.cfg
DFX1_KERNEL_SRCS   := $(DFX1_DIR)/hls/s2mm.cpp $(DFX1_DIR)/hls/mm2s.cpp $(DFX1_DIR)/hls/polar_clip.cpp
DFX1_KERNEL_STEMS  := $(basename $(notdir $(DFX1_KERNEL_SRCS)))
DFX1_KERNEL_XO     := $(addprefix $(DFX1_BUILD_DIR)/,$(addsuffix .xo,$(DFX1_KERNEL_STEMS)))

# ----------------------------- Task 2 paths -----------------------------
DFX2_DIR           := $(ROOT_DIR)/dfx2
DFX2_BUILD_DIR     := $(TMP_ROOT)/dfx2
DFX2_GRAPH         := $(DFX2_DIR)/aie/graph.cpp
DFX2_AIE_WORK      := $(DFX2_BUILD_DIR)/Work
DFX2_GRAPH_O       := $(DFX2_BUILD_DIR)/libadf.a
DFX2_LINK_OUTPUT   := $(DFX2_BUILD_DIR)/dfx2.xsa
DFX2_XCLBIN        := $(DFX2_BUILD_DIR)/dfx2.xclbin
DFX2_LINK_CONFIG   := $(DFX2_DIR)/system.cfg
DFX2_KERNEL_SRCS   := $(DFX2_DIR)/hls/s2mm.cpp $(DFX2_DIR)/hls/mm2s.cpp $(DFX2_DIR)/hls/polar_clip.cpp
DFX2_KERNEL_STEMS  := $(basename $(notdir $(DFX2_KERNEL_SRCS)))
DFX2_KERNEL_XO     := $(addprefix $(DFX2_BUILD_DIR)/,$(addsuffix .xo,$(DFX2_KERNEL_STEMS)))

# Tools
VPP    := v++
AIECC  := aiecompiler
AIESIM := aiesimulator

# Common include flags for AIE
DFX1_AIE_INCLUDE_FLAGS := -include="$(XILINX_VITIS)/aietools/include" \
                           -include="$(DFX1_DIR)/aie" \
                           -include="$(DFX1_DIR)/aie/kernels" \
                           -include="$(DFX1_DIR)/data" -include="./"

DFX2_AIE_INCLUDE_FLAGS := -include="$(XILINX_VITIS)/aietools/include" \
                           -include="$(DFX2_DIR)/aie" \
                           -include="$(DFX2_DIR)/aie/kernels" \
                           -include="$(DFX2_DIR)/data" -include="./"

DFX1_AIE_FLAGS := --platform $(PLATFORM) $(DFX1_AIE_INCLUDE_FLAGS) --pl-freq=100 -workdir=$(DFX1_AIE_WORK)
DFX2_AIE_FLAGS := --platform $(PLATFORM) $(DFX2_AIE_INCLUDE_FLAGS) --pl-freq=100 -workdir=$(DFX2_AIE_WORK)

#host code
HOST_SRCS := ./host/host.cpp
EXECUTABLE = ./app

GCC_FLAGS := -Wall -c \
			 -Wno-int-to-pointer-cast \
			 --sysroot=$(SYSROOT) -Wall -Wextra -O0 -g3 -std=c++17 -MD -MT

GCC_INCLUDES := -I${XILINX_VITIS}/include \
				-I$(SYSROOT)/usr/include/xrt \
				-I./ -I./src/ \
				-I${XILINX_VITIS}/aietools/include \
				-I${XILINX_VITIS}/include
ifeq ($(EMU_PS),X86)
CXX := g++
else
CXX := $(XILINX_VITIS)/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-g++
endif
GCC_LIB := -lxrt_core -lxrt_coreutil \
		   -L$(SYSROOT)/usr/lib \
		   --sysroot=$(SYSROOT) \
		   -L${XILINX_VITIS}/aietools/lib/aarch64.o

ifeq ($(TARGET),$(filter $(TARGET),sw_emu))
ifeq ($(EMU_PS), X86)
       GCC_LIB := -lxrt_coreutil -L${XILINX_VITIS}/aietools/lib/lnx64.o -L${XILINX_XRT}/lib
       GCC_FLAGS := -Wall -c -std=c++17 -Wno-int-to-pointer-cast -I${XILINX_XRT}/include
       GCC_INCLUDES := -I./src/ -I./ -I${XILINX_VITIS}/aietools/include
else
       GCC_LIB := -lxrt_coreutil -L$(SYSROOT)/usr/lib --sysroot=$(SYSROOT) -L${XILINX_VITIS}/aietools/lib/aarch64.o
endif
endif
# Helper to ensure per-task temp folders exist
$(DFX1_BUILD_DIR)/.dirs:
	@mkdir -p $(DFX1_BUILD_DIR)/_x $(DFX1_BUILD_DIR)/logs $(DFX1_BUILD_DIR)/reports
	@mkdir -p $(DFX1_AIE_WORK)
	@touch $@

$(DFX2_BUILD_DIR)/.dirs:
	@mkdir -p $(DFX2_BUILD_DIR)/_x $(DFX2_BUILD_DIR)/logs $(DFX2_BUILD_DIR)/reports
	@mkdir -p $(DFX2_AIE_WORK)
	@touch $@

# ============================ HLS COMPILE (XO) ============================
.PHONY: dfx1_xo dfx2_xo

dfx1_xo: $(DFX1_KERNEL_XO)
$(DFX1_BUILD_DIR)/%.xo: $(DFX1_DIR)/hls/%.cpp | $(DFX1_BUILD_DIR)/.dirs
	$(VPP) -c -t $(TARGET) --platform $(PLATFORM) -k $(basename $(notdir $<)) $< \
	  --temp_dir $(DFX1_BUILD_DIR)/_x --log_dir $(DFX1_BUILD_DIR)/logs --report_dir $(DFX1_BUILD_DIR)/reports \
	  --save-temps -g -o $@

dfx2_xo: $(DFX2_KERNEL_XO)
$(DFX2_BUILD_DIR)/%.xo: $(DFX2_DIR)/hls/%.cpp | $(DFX2_BUILD_DIR)/.dirs
	$(VPP) -c -t $(TARGET) --platform $(PLATFORM) -k $(basename $(notdir $<)) $< \
	  --temp_dir $(DFX2_BUILD_DIR)/_x --log_dir $(DFX2_BUILD_DIR)/logs --report_dir $(DFX2_BUILD_DIR)/reports \
	  --save-temps -g -o $@

# ============================ AIE BUILD (libadf.a) ============================
.PHONY: dfx1_graph dfx2_graph

dfx1_graph: $(DFX1_GRAPH_O)
$(DFX1_GRAPH_O): $(DFX1_GRAPH) | $(DFX1_BUILD_DIR)/.dirs
	cd $(DFX1_BUILD_DIR) && $(AIECC) $(DFX1_AIE_FLAGS) $(abspath $<)
	@echo "COMPLETE: $(DFX1_GRAPH_O)"

dfx2_graph: $(DFX2_GRAPH_O)
$(DFX2_GRAPH_O): $(DFX2_GRAPH) | $(DFX2_BUILD_DIR)/.dirs
	cd $(DFX2_BUILD_DIR) && $(AIECC) $(DFX2_AIE_FLAGS) $(abspath $<)
	@echo "COMPLETE: $(DFX2_GRAPH_O)"

# ============================ LINK (XSA) ============================
$(DFX1_LINK_OUTPUT): $(DFX1_KERNEL_XO) $(DFX1_GRAPH_O)
	$(VPP) -l --platform $(PLATFORM) $(DFX1_KERNEL_XO) $(DFX1_GRAPH_O) -t $(TARGET) \
	  --config $(DFX1_LINK_CONFIG) \
	  --temp_dir $(DFX1_BUILD_DIR)/_x --log_dir $(DFX1_BUILD_DIR)/logs --report_dir $(DFX1_BUILD_DIR)/reports \
	  --save-temps -g -o $@
	@echo "COMPLETE: $@"

$(DFX2_LINK_OUTPUT): $(DFX2_KERNEL_XO) $(DFX2_GRAPH_O)
	$(VPP) -l --platform $(PLATFORM) $(DFX2_KERNEL_XO) $(DFX2_GRAPH_O) -t $(TARGET) \
	  --config $(DFX2_LINK_CONFIG) \
	  --temp_dir $(DFX2_BUILD_DIR)/_x --log_dir $(DFX2_BUILD_DIR)/logs --report_dir $(DFX2_BUILD_DIR)/reports \
	  --save-temps -g -o $@
	@echo "COMPLETE: $@"

.PHONY: build_xsa
build_xsa: $(DFX1_LINK_OUTPUT) $(DFX2_LINK_OUTPUT)

# ============================ XCLBIN (link to xclbin) ============================
$(DFX1_XCLBIN): $(DFX1_LINK_OUTPUT)
	$(VPP) -p --platform $(PLATFORM) -t $(TARGET) $(DFX1_GRAPH_O) $(DFX1_LINK_OUTPUT) \
	  --temp_dir $(DFX1_BUILD_DIR)/_x --log_dir $(DFX1_BUILD_DIR)/logs --report_dir $(DFX1_BUILD_DIR)/reports \
	  --package.defer_aie_run \
	  --save-temps -g -o $@

$(DFX2_XCLBIN): $(DFX2_LINK_OUTPUT)
	$(VPP) -p --platform $(PLATFORM) -t $(TARGET) $(DFX2_GRAPH_O) $(DFX2_LINK_OUTPUT) \
	  --temp_dir $(DFX2_BUILD_DIR)/_x --log_dir $(DFX2_BUILD_DIR)/logs --report_dir $(DFX2_BUILD_DIR)/reports \
	  --package.defer_aie_run \
	  --save-temps -g -o $@

.PHONY: build_xclbin
build_xclbin: $(DFX1_XCLBIN) $(DFX2_XCLBIN)

#=======compile the host.
host: $(EXECUTABLE)

$(EXECUTABLE): $(HOST_SRCS) 
ifneq ($(EMU_PS), X86)
endif 
		$(CXX) $(HOST_SRCS) $(GCC_FLAGS) $(GCC_INCLUDES) -o main.o
		$(CXX) *.o $(GCC_LIB) -o $(EXECUTABLE)
	@echo "COMPLETE: Host application created."

# ============================ PACKAGING (optional) ============================
RUN_APP_SCRIPT=$(ROOT_DIR)/run_aie.sh
PACKAGE_OUT=$(TMP_ROOT)/package_$(TARGET)
SD_CARD =sd_card.img
sd_card: $(SD_CARD)
$(SD_CARD):$(EXECUTABLE) $(DFX2_XCLBIN) $(DFX1_XCLBIN)
	$(VPP) -p  --platform $(PLATFORM) -t $(TARGET) $(DFX1_XCLBIN) $(DFX1_GRAPH_O) \
	--package.out_dir $(PACKAGE_OUT) \
	--package.rootfs $(COMMON_IMAGE_VERSAL)/rootfs.ext4 \
	--package.image_format=ext4 \
	--package.boot_mode=sd \
	--package.kernel_image=$(COMMON_IMAGE_VERSAL)/Image \
	--package.sd_file $(RUN_APP_SCRIPT) \
	--package.sd_file $(DFX1_XCLBIN) \
	--package.sd_file $(DFX2_XCLBIN) \
	--package.sd_file $(EXECUTABLE)

# ============================ CLEAN ============================
.PHONY: clean ultraclean
clean:
	rm -rf $(TMP_ROOT)

ultraclean: clean
	@echo "Workspace cleaned (all outputs lived under tmp/)."
