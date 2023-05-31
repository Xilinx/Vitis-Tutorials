#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11

%.PHONY: help
help::
	@echo  ""
	@echo  " Makefile Usage:"
	@echo  ""
	@echo  "  make sd_card"
	@echo  "  Command calls rules: kernels graph xclbin application package, in sequence"
	@echo  "  Default TARGET is hw_emu"
	@echo  ""
	@echo  "  make run"
	@echo  "  Command calls rules: sd_card run_emu, in sequence"
	@echo  "  Default TARGET is hw_emu"
	@echo  ""
	@echo  "  make all"
	@echo  "  Command calls rules: sd_card report_metrics run_emu, in sequence"
	@echo  "  Default TARGET is hw_emu"
	@echo  ""
	@echo  "  make kernels"
	@echo  "  Command to generate the PL kernels"
	@echo  "  Default TARGET is hw_emu"
	@echo  ""
	@echo  "  make xclbin"
	@echo  "  Command to generate the xclbin for specified target"
	@echo  "  Default TARGET is hw_emu"
	@echo  ""
	@echo  "  make application"
	@echo  "  Command to generate the A72 application"
	@echo  ""
	@echo  "  make package"
	@echo  "  Command to package design into an sd_card for specified target"
	@echo  "  Default TARGET is hw_emu"
	@echo  ""
	@echo  "  make run_emu"
	@echo  "  Command to run the hw emulator if LAUNCH_HW_EMU_EXEC variable is set to 1"
	@echo  "  Default TARGET is hw_emu"
	@echo  ""
	@echo  "  make report_metrics"
	@echo  "  Command to run and get vivado utilisation and power reports."
	@echo  "  Works only for TARGET=hw"
	@echo  ""
	@echo  "  make clean_target"
	@echo  "  Command to remove the generated files for a specific target"
	@echo  ""
	@echo  "  make clean_gemmsize"
	@echo  "  Command to remove the generated files for a specific GeMM Matrix Dimension"
	@echo  ""
	@echo  "  make cleanall_blds"
	@echo  "  Command to remove the generated files for all variations"
	@echo  ""
	@echo  "  make cleanall_vivado_reports"
	@echo  "  Command to remove the generated vivado reports files for all variations"
	@echo  ""
	@echo  "  make cleanall_reports"
	@echo  "  Command to remove all the generated reports for all variations"
	@echo  ""
	@echo  "  make cleanall"
	@echo  "  Command to remove the generated files and reports for all variations"
	@echo  ""
	@echo  " OPTIONS:"
	@echo  " Use the make recipes with required values for options mentioned below-"
	@echo  "    TARGET     -specifies target: Hardware or Emulation. Values: hw_emu(default), hw"
	@echo  "    GEMM_SIZE  -specifies GeMM Matrix Dimension. Values: 32(default), 64, 128, 256, 512, 1024"
	@echo  "    PL_FREQ    -specifies HLS kernel frequencies. Values: 250Mhz(default)"
	@echo  "    LAUNCH_HW_EMU_EXEC -specifies whether to launch hw emulator when running make run_emu rule. Values: 0(disabled, default) and 1(enabled)"
	@echo  ""

# Print all options passed to Makefile
print-%  : ; @echo $* = $($*)

# =========================================================
# TARGET can be set as:
#   hw_emu: Hardware Emulation
#   hw    : Hardware Run
# =========================================================
TARGET := hw

# =========================================================
# Gemm Output matrix size can be set as:
# 32(32x32x32)<default>, 64(64x64x64), 128(128x128x128),
# 256(256x256x256), 512(512x512x512) and 1024(1kx1kx1k)
# =========================================================
GEMM_SIZE := 32
MAT_DIMS  := $(GEMM_SIZE)x$(GEMM_SIZE)x$(GEMM_SIZE)

# =========================================================
# PL Frequency in Mhz:
#   500MHz(default)
# =========================================================
PL_FREQ := 800
VPP_CLOCK_FREQ := $(PL_FREQ)000000

######################################
# When TARGET=hw_emu, hw_emu launch...
# 0 = Disabled (Default).
# 1 = Enabled.
######################################
LAUNCH_HW_EMU_EXEC := 0

# =========================================================
# Source directories
# =========================================================
# Relative gemm directory
RELATIVE_PROJECT_DIR := ./

# Absolute gemm directory = <user path>/Tutorials/AI_Engine/gemm
PROJECT_REPO := $(shell readlink -f $(RELATIVE_PROJECT_DIR))

## Setting other folders relative to PROJECT_REPO
DESIGN_REPO := $(PROJECT_REPO)/design
PL_SRC_REPO := $(DESIGN_REPO)/pl_src
CONSTRAINTS_REPO := $(PL_SRC_REPO)/constraints
HOST_APP_SRC     := $(DESIGN_REPO)/host_app_src
SYSTEM_CONFIGS_REPO := $(DESIGN_REPO)/system_configs
EXEC_SCRIPTS_REPO   := $(DESIGN_REPO)/exec_scripts
VIVADO_METRICS_SCRIPTS_REPO := $(DESIGN_REPO)/vivado_metrics_scripts

#BASE_BLD_DIR     := $(PROJECT_REPO)/build
BASE_BLD_DIR     := $(PROJECT_REPO)/build_$(PL_FREQ)
GEMM_BLD_DIR     := $(BASE_BLD_DIR)/gemm_$(MAT_DIMS)
BUILD_TARGET_DIR := $(GEMM_BLD_DIR)/$(TARGET)

VIVADO_REPORTS_REPO := $(PROJECT_REPO)/vivado_reports_dir
VIVADO_BLD_REPORTS_DIR := $(VIVADO_REPORTS_REPO)/gemm_$(MAT_DIMS)

EMBEDDED_PACKAGE_OUT := $(BUILD_TARGET_DIR)/package
EMBEDDED_EXEC_SCRIPT := run_script.sh





# ==========================================================
# Below are the names for Application executable,
# kernel executables, and xclbin
# ==========================================================
APP_ELF := gemm_dsp_xrt.elf
#XCLBIN  := vck190_dsp_gemm.$(TARGET).xclbin
XSA  := vck190_dsp_gemm.$(TARGET).xsa

# =========================================================
# Application Source Files repository
# =========================================================
APP_SRC_CPP := $(HOST_APP_SRC)/main.cpp

# =========================================================
# Application Compiler and Linker Flags
# =========================================================
GCC_FLAGS := -O
GCC_FLAGS += -c
GCC_FLAGS += -std=c++14
GCC_FLAGS += -D__linux__
GCC_FLAGS += -DM_LARGE=$(GEMM_SIZE)
GCC_FLAGS += -DN_LARGE=$(GEMM_SIZE)
GCC_FLAGS += -DL_LARGE=$(GEMM_SIZE)

GCC_INC_FLAGS := -I$(SDKTARGETSYSROOT)/usr/include/xrt
GCC_INC_FLAGS += -I$(SDKTARGETSYSROOT)/usr/include
GCC_INC_FLAGS += -I$(SDKTARGETSYSROOT)/usr/lib
GCC_INC_FLAGS += -I$(HOST_APP_SRC)/$(MAT_DIMS)

GCC_INC_LIB += -L$(SDKTARGETSYSROOT)/usr/lib

GCC_LIB := -lxrt_coreutil

# =========================================================
# Kernel Compiler and Linker Flags
# ========================================================
VPP_FLAGS := --platform $(PLATFORM)
VPP_FLAGS += --save-temps
VPP_FLAGS += --temp_dir $(BUILD_TARGET_DIR)/_x
VPP_FLAGS += --verbose
VPP_FLAGS += -g

VPP_LINK_FLAGS := --clock.freqHz $(VPP_CLOCK_FREQ):gemm_large_ocm_0 
VPP_LINK_FLAGS += --clock.defaultTolerance 0.001
VPP_LINK_FLAGS += --config $(SYSTEM_CONFIGS_REPO)/gemm.cfg
VPP_LINK_FLAGS += --vivado.prop fileset.sim_1.xsim.simulate.log_all_signals=true
VPP_LINK_FLAGS += --vivado.prop run.synth_1.{STEPS.SYNTH_DESIGN.ARGS.CONTROL_SET_OPT_THRESHOLD}={16}
VPP_LINK_FLAGS += --vivado.prop run.synth_1.{STEPS.SYNTH_DESIGN.ARGS.KEEP_EQUIVALENT_REGISTERS}={true}
#VPP_LINK_FLAGS += --to_step vpl.synth
#VPP_LINK_FLAGS += --from_step vpl.synth
VPP_LINK_FLAGS += --xp vivado_prop:run.impl_1.STEPS.PLACE_DESIGN.TCL.PRE=$(CONSTRAINTS_REPO)/gemm_dsp58.tcl

# =========================================================
# Packaging Flags
# ========================================================
PKG_FLAGS := -t $(TARGET)
PKG_FLAGS += --save-temps
PKG_FLAGS += --temp_dir $(BUILD_TARGET_DIR)/_x
PKG_FLAGS += -f $(PLATFORM)
PKG_FLAGS += --package.rootfs $(COMMON_IMAGE_VERSAL)/rootfs.ext4
PKG_FLAGS += --package.kernel_image $(COMMON_IMAGE_VERSAL)/Image
PKG_FLAGS += --package.boot_mode=sd
PKG_FLAGS += --package.out_dir $(EMBEDDED_PACKAGE_OUT)
PKG_FLAGS += --package.image_format=ext4
#PKG_FLAGS += --package.sd_file $(BUILD_TARGET_DIR)/$(APP_ELF) $(BUILD_TARGET_DIR)/$(XCLBIN)
PKG_FLAGS += --package.sd_file $(BUILD_TARGET_DIR)/$(APP_ELF) $(BUILD_TARGET_DIR)/$(XSA)
PKG_FLAGS += --package.sd_file $(EXEC_SCRIPTS_REPO)/$(EMBEDDED_EXEC_SCRIPT)

ifdef XRT_ROOT
   PKG_FLAGS += --package.sd_dir $(XRT_ROOT)
endif

# =========================================================
# Step 1. Kernel XO File Generation
# ========================================================

KERNEL_XOS := $(BUILD_TARGET_DIR)/gemm_large_ocm.xo 

kernels: $(KERNEL_XOS)

$(BUILD_TARGET_DIR)/gemm_large_ocm.xo: $(PL_SRC_REPO)/run_gemm.tcl
	mkdir -p $(BUILD_TARGET_DIR); \
	cd $(BUILD_TARGET_DIR); \
	vivado -mode batch -source $(PL_SRC_REPO)/run_gemm.tcl -tclargs $(PL_SRC_REPO) $(GEMM_SIZE)

# =========================================================
# Step 2. XCLBIN File Generation
# ========================================================
#xclbin: $(BUILD_TARGET_DIR)/$(XCLBIN)
xsa: $(BUILD_TARGET_DIR)/$(XSA)

#$(BUILD_TARGET_DIR)/$(XCLBIN): kernels 
$(BUILD_TARGET_DIR)/$(XSA): kernels 
	cd $(BUILD_TARGET_DIR);	\
	v++ -l $(VPP_FLAGS) $(VPP_LINK_FLAGS) -t $(TARGET) -o $@ \
		    $(KERNEL_XOS)

# =========================================================
# Step 3. A72 Application Executable File Generation
# ========================================================
application:
	mkdir -p $(BUILD_TARGET_DIR)
	@rm -rf $(BUILD_TARGET_DIR)/gemm_top_app.o $(BUILD_TARGET_DIR)/$(APP_ELF)
	$(CXX) $(GCC_FLAGS) $(GCC_INC_FLAGS) $(APP_SRC_CPP) -o $(BUILD_TARGET_DIR)/gemm_top_app.o $(GCC_INC_LIB) $(GCC_LIB)
	$(CXX) $(BUILD_TARGET_DIR)/gemm_top_app.o $(GCC_INC_LIB) $(GCC_LIB) -o $(BUILD_TARGET_DIR)/$(APP_ELF)

# =========================================================
# Step 4. Package Generation 
# ========================================================
package:
	rm -rf $(EMBEDDED_PACKAGE_OUT)
	cd $(BUILD_TARGET_DIR);	\
	v++ -p $(PKG_FLAGS)
	cp -rf $(BUILD_TARGET_DIR)/package $(BUILD_TARGET_DIR)/package_backup

# =========================================================
# Step 5. Run Hardware Emulation 
# ========================================================
# If the target is for HW_EMU, launch the emulator
# If the target is for HW, you'll have to follow the
# instructions in the README.md
run_emu:
ifeq ($(TARGET),hw_emu)
	@echo ""
	@echo "###########################################################################"

ifeq ($(LAUNCH_HW_EMU_EXEC),0)
	@echo "To Run Hardware Emulation Manually Goto:"
	@echo "$(EMBEDDED_PACKAGE_OUT)"
	@echo ""
	@echo "and do:"
	@echo "./launch_hw_emu.sh or ./launch_hw_emu.sh -g (for waveform viewer)..."
	@echo ""

else
	cd $(EMBEDDED_PACKAGE_OUT); \
	./launch_hw_emu.sh -run-app $(EMBEDDED_EXEC_SCRIPT) | tee embedded_run.log

endif
else
	@echo ""
	@echo "###########################################################################"
	@echo "Hardware build, no emulation executed."
	@echo ""
	@echo "Use sd_card.img from below directory and, follow the steps in README.md for execution on board."
	@echo "$(EMBEDDED_PACKAGE_OUT)"
	@echo ""

endif

# =========================================================
# Step 6. Report Utilization Metrics
# =========================================================
# If the target is HW, this generates the power and resource
# utilization metrics from vivado.
report_metrics:
ifeq ($(TARGET),hw_emu)
	@echo "This build target (report-metrics) not valid when design target is hw_emu"

else
	rm -rf $(VIVADO_BLD_REPORTS_DIR)
	mkdir -p $(VIVADO_BLD_REPORTS_DIR)
	cd $(VIVADO_BLD_REPORTS_DIR); \
	vivado -mode batch -source $(VIVADO_METRICS_SCRIPTS_REPO)/report_metrics.tcl $(BUILD_TARGET_DIR)/_x/link/vivado/vpl/prj/prj.xpr

endif

###########################################################################

# =========================================================
# Primary Build Targets
# ==> sd_card
# ==> run
# ==> all
# ==> clean_target
# ==> clean_gemmsize
# ==> cleanall_blds
# ==> cleanall_vivado_reports
# ==> cleanall_reports
# ==> cleanall
# ========================================================

#Build the design without running the hardware emulation
sd_card: kernels xsa application package

#Build the design and then run hardware emulation
run: sd_card run_emu

#Same as "run" above, but include metrics generation
all: sd_card report_metrics run_emu

#Clean build target output directory...
clean_target:
	@echo "Cleaning $(TARGET) Target Build Dir..."
	rm -rf $(BUILD_TARGET_DIR)

#Clean GeMM matrix dimension output directory...
clean_gemmsize:
	@echo "Cleaning Gemm Size Builds..."
	rm -rf $(GEMM_BLD_DIR)

#Clean all builds...
cleanall_blds:
	rm -rf $(BASE_BLD_DIR)
	rm -rf vivado* .Xil *.log vitis* core.*

#Clean_all vivado reports...
cleanall_vivado_reports:
	rm -rf $(VIVADO_REPORTS_REPO)

#Clean_all reports...
cleanall_reports: cleanall_vivado_reports

#Clean_all builds and reports...
cleanall: cleanall_blds cleanall_reports
