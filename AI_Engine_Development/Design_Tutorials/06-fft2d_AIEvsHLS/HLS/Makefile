# © Copyright 2021 Xilinx, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

%.PHONY: help
help::
	@echo  " Use the make recipes with required values for TARGET, FFT_2D_PT, FFT_2D_INSTS, ITER_CNT and EN_TRACE"
	@echo  " TARGET=<hw_emu(default)/hw>"
	@echo  " FFT_2D_PT=<64,128,256,512,2048(default)>"
	@echo  " ITER_CNT=<No. of Iterations>"
	@echo  " EN_TRACE=<0(default),1>"
	@echo  ""
	@echo  " Makefile Usage:"
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
	@echo  "  make clean_target"
	@echo  "  Command to remove the generated files for a specific target"
	@echo  ""
	@echo  "  make clean_insts"
	@echo  "  Command to remove the generated files for a specific instance design 1x,5x or 10x"
	@echo  ""
	@echo  "  make clean_fftpt"
	@echo  "  Command to remove the generated files for a specific fft point size"
	@echo  ""
	@echo  "  make cleanall"
	@echo  "  Command to remove the generated files for all instances and point sizes"
	@echo  ""

# =========================================================
# TARGET can be set as:
#   hw_emu: Hardware Emulation
#   hw    : Hardware Run
# =========================================================
TARGET := hw_emu

# =========================================================
# 2 Dimentional FFT Point: MAT_ROWS x MAT_COLS(Mat Dims):
#   FFT_2D_PT =   64:   32 x   64
#   FFT_2D_PT =  128:   64 x  128
#   FFT_2D_PT =  256:  128 x  256
#   FFT_2D_PT =  512:  256 x  512
#   FFT_2D_PT = 2048: 1024 x 2048 (default)
#
#   MAT_ROWS: No. of rows in the Inp Matrix (default 1024)
#   MAT_COLS: No. of cols in the Inp Matrix (default 2048)
# =========================================================
FFT_2D_PT := 2048

MAT_COLS := $(FFT_2D_PT)
MAT_ROWS := $(shell echo $$(( $(MAT_COLS) / 2 )))

# =========================================================
# No of FFT2D Instances can be set as:
#   1(default), 5 and 10.
# =========================================================
FFT_2D_INSTS := 1

# =========================================================
# PL Frequency in Mhz:
#   250Mhz(default)
# =========================================================
PL_FREQ := 250
VPP_CLOCK_FREQ := $(PL_FREQ)000000

FFT_2D_PL_FREQ := $(shell echo $$(( $(PL_FREQ) * 2 )))
FFT_2D_VPP_CLOCK_FREQ := $(FFT_2D_PL_FREQ)000000

# =========================================================
# Iteration Count:
# 1(default)
# =========================================================
ITER_CNT := 8

# =========================================================
# Profiling/Trace and Datamover type Switches...
# =========================================================
EN_TRACE	:= 0

######################################
# FFT 2D Datatype can be,
# cint16 (Default) or cfloat...
# 0=cint16(Default)
# 1=cfloat
######################################

FFT_2D_DT := 0

# =========================================================
# Source directory
# =========================================================
# Relative fft_2d directory
RELATIVE_PROJECT_DIR := ./

# Absolute fft_2d directory = <user path>/Tutorials/AI_Engine/fft_2d
PROJECT_REPO := $(shell readlink -f $(RELATIVE_PROJECT_DIR))

DESIGN_REPO  := $(PROJECT_REPO)/design
HOST_APP_SRC := $(DESIGN_REPO)/host_app_src
PL_SRC_REPO  := $(DESIGN_REPO)/pl_src
DIRECTIVES_REPO        := $(DESIGN_REPO)/directives
SYSTEM_CONFIGS_REPO    := $(DESIGN_REPO)/system_configs
PROFILING_CONFIGS_REPO := $(DESIGN_REPO)/profiling_configs
HW_EMU_RUN_FILES_REPO  := $(DESIGN_REPO)/hw_emu_files
BASE_BLD_DIR     := $(PROJECT_REPO)/build

ifeq ($(FFT_2D_DT),0)
   FFT_2D_DT_DIR    := $(BASE_BLD_DIR)/cint16_dsns

else
   FFT_2D_DT_DIR    := $(BASE_BLD_DIR)/cfloat_dsns

endif

FFTPT_BLD_DIR    := $(FFT_2D_DT_DIR)/fft2d_$(MAT_ROWS)x$(MAT_COLS)
INSTS_BLD_DIR    := $(FFTPT_BLD_DIR)/x$(FFT_2D_INSTS)
BUILD_TARGET_DIR := $(INSTS_BLD_DIR)/$(TARGET)

EMBEDDED_PACKAGE_OUT := $(BUILD_TARGET_DIR)/package
EMBEDDED_EXEC_SCRIPT := run_script.sh

######################################
# When TARGET=hw_emu, hw_emu launch...
# 0 = Disabled (Default).
# 1 = Enabled.
######################################
LAUNCH_HW_EMU_EXEC := 0

# Print all options passed to Makefile
print-%  : ; @echo $* = $($*)

# ==========================================================
# Below are the names for Application executable,
# kernel executables, and xclbin
# ==========================================================
DATAMOVER_KERNEL_TOP := dma_hls
DATAMOVER_KERNEL_XO  := $(DATAMOVER_KERNEL_TOP).$(TARGET)

FFT_2D_KERNEL_XO := fft_2d.$(TARGET)

APP_ELF := fft_2d_hls_xrt.elf
XCLBIN  := vck190_hls_fft_2d.$(TARGET).xclbin

# =========================================================
# Kernel Source Files repository
# =========================================================
DATAMOVER_KERNEL_SRC := $(PL_SRC_REPO)/$(DATAMOVER_KERNEL_TOP).cpp

FFT_2D_KERNEL_SRC := $(PL_SRC_REPO)/fft_2d.cpp

# =========================================================
# Application Source Files repository
# =========================================================
APP_SRC_CPP := $(HOST_APP_SRC)/fft_2d_hls_app.cpp

# =========================================================
# Application Compiler and Linker Flags
# =========================================================
GCC_FLAGS := -O
GCC_FLAGS += -c
GCC_FLAGS += -std=c++14
GCC_FLAGS += -D__linux__
GCC_FLAGS += -DFFT2D_INSTS=$(FFT_2D_INSTS)
GCC_FLAGS += -DITER_CNT=$(ITER_CNT)
GCC_FLAGS += -DMAT_ROWS=$(MAT_ROWS)
GCC_FLAGS += -DMAT_COLS=$(MAT_COLS)
GCC_FLAGS += -DFFT_2D_DT=$(FFT_2D_DT)

GCC_INC_FLAGS := -I$(SDKTARGETSYSROOT)/usr/include/xrt
GCC_INC_FLAGS += -I$(SDKTARGETSYSROOT)/usr/include
GCC_INC_FLAGS += -I$(SDKTARGETSYSROOT)/usr/lib
GCC_INC_FLAGS += -I$(HOST_APP_SRC)

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

FFT_2D_KERNEL_VPP_FLAGS := --hls.pre_tcl $(DIRECTIVES_REPO)/hls_pre.tcl
FFT_2D_KERNEL_VPP_FLAGS += --hls.clock $(FFT_2D_VPP_CLOCK_FREQ):fft_2d
FFT_2D_KERNEL_VPP_FLAGS += -D MAT_ROWS=$(MAT_ROWS)
FFT_2D_KERNEL_VPP_FLAGS += -D MAT_COLS=$(MAT_COLS)
FFT_2D_KERNEL_VPP_FLAGS += -D FFT_2D_DT=$(FFT_2D_DT)
#FFT_2D_KERNEL_VPP_FLAGS += --hls.jobs 32

DATAMOVER_KERNEL_VPP_FLAGS := --hls.clock $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)
DATAMOVER_KERNEL_VPP_FLAGS += -D FFT_2D_DT=$(FFT_2D_DT)
#DATAMOVER_KERNEL_VPP_FLAGS += --hls.jobs 32

VPP_LINK_FLAGS := --clock.freqHz $(FFT_2D_VPP_CLOCK_FREQ):fft_2d_0
VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_0

ifeq ($(FFT_2D_INSTS),$(filter $(FFT_2D_INSTS),5 10))
   
   VPP_LINK_FLAGS += --clock.freqHz $(FFT_2D_VPP_CLOCK_FREQ):fft_2d_1
   VPP_LINK_FLAGS += --clock.freqHz $(FFT_2D_VPP_CLOCK_FREQ):fft_2d_2
   VPP_LINK_FLAGS += --clock.freqHz $(FFT_2D_VPP_CLOCK_FREQ):fft_2d_3
   VPP_LINK_FLAGS += --clock.freqHz $(FFT_2D_VPP_CLOCK_FREQ):fft_2d_4
   VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_1
   VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_2
   VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_3
   VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_4
   
endif

ifeq ($(FFT_2D_INSTS),10)
   
   VPP_LINK_FLAGS += --clock.freqHz $(FFT_2D_VPP_CLOCK_FREQ):fft_2d_5
   VPP_LINK_FLAGS += --clock.freqHz $(FFT_2D_VPP_CLOCK_FREQ):fft_2d_6
   VPP_LINK_FLAGS += --clock.freqHz $(FFT_2D_VPP_CLOCK_FREQ):fft_2d_7
   VPP_LINK_FLAGS += --clock.freqHz $(FFT_2D_VPP_CLOCK_FREQ):fft_2d_8
   VPP_LINK_FLAGS += --clock.freqHz $(FFT_2D_VPP_CLOCK_FREQ):fft_2d_9
   VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_5
   VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_6
   VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_7
   VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_8
   VPP_LINK_FLAGS += --clock.freqHz $(VPP_CLOCK_FREQ):$(DATAMOVER_KERNEL_TOP)_9
   
endif

VPP_LINK_FLAGS += --clock.defaultTolerance 0.001
VPP_LINK_FLAGS += --advanced.param compiler.userPostSysLinkOverlayTcl=$(DIRECTIVES_REPO)/cdc_async.tcl

ifeq ($(EN_TRACE),1)
   
   ifeq ($(FFT_2D_INSTS),1)
      
      VPP_LINK_FLAGS += --profile.data $(DATAMOVER_KERNEL_TOP):all:all
   
   else
   
      VPP_LINK_FLAGS += --profile.data $(DATAMOVER_KERNEL_TOP):all:strmInp_from_colwiseFFT
   
   endif
   
   VPP_LINK_FLAGS += --profile.trace_memory DDR
   
endif

VPP_LINK_FLAGS += --config $(SYSTEM_CONFIGS_REPO)/x$(FFT_2D_INSTS).cfg
VPP_LINK_FLAGS += --vivado.prop fileset.sim_1.xsim.simulate.log_all_signals=true

VPP_LINK_FLAGS += --vivado.prop run.synth_1.{STEPS.SYNTH_DESIGN.ARGS.CONTROL_SET_OPT_THRESHOLD}={16}
#VPP_LINK_FLAGS += --vivado.prop run.synth_1.{STEPS.SYNTH_DESIGN.ARGS.RETIMING}={true}

ifeq ($(FFT_2D_DT),0)
   ifeq ($(FFT_2D_PT),64)
      ifeq ($(FFT_2D_INSTS),1)
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_Auto_3}
         
      else ifeq ($(FFT_2D_INSTS),5)
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_ExploreWithRemap}
               
      else
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Congestion_SpreadLogic_medium}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.OPT_DESIGN.ARGS.DIRECTIVE}={Explore}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.PHYS_OPT_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         
      endif
   else ifeq ($(FFT_2D_PT),128)
      ifeq ($(FFT_2D_INSTS),1)
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Power_DefaultOpt}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
   
      else ifeq ($(FFT_2D_INSTS),5)
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_ExploreWithRemap}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.OPT_DESIGN.ARGS.DIRECTIVE}={Explore}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.PHYS_OPT_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         
      else
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_ExploreWithRemap}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.PHYS_OPT_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         
      endif
   else ifeq ($(FFT_2D_PT),256)
      ifeq ($(FFT_2D_INSTS),1)
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_NetDelay_low}
         
      else ifeq ($(FFT_2D_INSTS),5)
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_ExplorePostRoutePhysOpt}
         
      else
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_HighUtilSLRs}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.POST_ROUTE_PHYS_OPT_DESIGN.ARGS.IS_ENABLED}={true}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.POST_ROUTE_PHYS_OPT_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         
      endif
   else ifeq ($(FFT_2D_PT),512)
      ifeq ($(FFT_2D_INSTS),1)
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Power_DefaultOpt}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.OPT_DESIGN.ARGS.DIRECTIVE}={Explore}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.POST_ROUTE_PHYS_OPT_DESIGN.ARGS.IS_ENABLED}={true}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.POST_ROUTE_PHYS_OPT_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         
      else ifeq ($(FFT_2D_INSTS),5)
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_NetDelay_low}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         
      else
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_NetDelay_high}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         
      endif
   else ifeq ($(FFT_2D_PT),2048)
      ifeq ($(FFT_2D_INSTS),1)
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_Auto_2}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         
      else ifeq ($(FFT_2D_INSTS),5)
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_HighUtilSLRs}
         
      else
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_NetDelay_high}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.OPT_DESIGN.ARGS.DIRECTIVE}={Explore}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.POST_ROUTE_PHYS_OPT_DESIGN.ARGS.IS_ENABLED}={true}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.POST_ROUTE_PHYS_OPT_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}      
         
      endif
   endif
else
   ifeq ($(FFT_2D_PT),64)
      ifeq ($(FFT_2D_INSTS),1)
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Area_ExploreWithRemap}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         
      else ifeq ($(FFT_2D_INSTS),5)
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Area_Explore}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         
      else
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_WLBlockPlacement}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.POST_ROUTE_PHYS_OPT_DESIGN.ARGS.IS_ENABLED}={true}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.POST_ROUTE_PHYS_OPT_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         
      endif
   else ifeq ($(FFT_2D_PT),128)
      ifeq ($(FFT_2D_INSTS),1)
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_WLBlockPlacement}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.PHYS_OPT_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
   
      else ifeq ($(FFT_2D_INSTS),5)
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Flow_RunPhysOpt}
         
      else
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_Auto_2}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.POST_ROUTE_PHYS_OPT_DESIGN.ARGS.IS_ENABLED}={true}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.POST_ROUTE_PHYS_OPT_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
               
      endif
   else ifeq ($(FFT_2D_PT),256)
      ifeq ($(FFT_2D_INSTS),1)
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_WLBlockPlacement}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.PHYS_OPT_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
   
      else ifeq ($(FFT_2D_INSTS),5)
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_ExplorePostRoutePhysOpt}
         
      else
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_ExploreWithRemap}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.PHYS_OPT_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         
      endif
   else ifeq ($(FFT_2D_PT),512)
      ifeq ($(FFT_2D_INSTS),1)
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_ExploreWithRemap}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         
      else ifeq ($(FFT_2D_INSTS),5)
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_Explore}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.POST_ROUTE_PHYS_OPT_DESIGN.ARGS.IS_ENABLED}={true}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.POST_ROUTE_PHYS_OPT_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         
      else
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_NetDelay_high}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.PHYS_OPT_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.POST_ROUTE_PHYS_OPT_DESIGN.ARGS.IS_ENABLED}={true}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.POST_ROUTE_PHYS_OPT_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         
      endif
   else ifeq ($(FFT_2D_PT),2048)
      ifeq ($(FFT_2D_INSTS),1)
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_ExploreWithRemap}
         
      else ifeq ($(FFT_2D_INSTS),5)
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_ExploreWithRemap}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         
      else
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{strategy}={Performance_Auto_1}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.PHYS_OPT_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.POST_ROUTE_PHYS_OPT_DESIGN.ARGS.IS_ENABLED}={true}
         VPP_LINK_FLAGS += --vivado.prop run.impl_1.{STEPS.POST_ROUTE_PHYS_OPT_DESIGN.ARGS.DIRECTIVE}={AggressiveExplore}
         
      endif
   endif
endif

#VPP_LINK_FLAGS += --vivado.synth.jobs 32
#VPP_LINK_FLAGS += --vivado.impl.jobs 32

# =========================================================
# Packaging Flags
# ========================================================
PKG_FLAGS := -t $(TARGET)
PKG_FLAGS += --save-temps
PKG_FLAGS += --temp_dir $(BUILD_TARGET_DIR)/_x
PKG_FLAGS += -f $(PLATFORM)
PKG_FLAGS += --package.rootfs $(XLNX_VERSAL)/rootfs.ext4
PKG_FLAGS += --package.kernel_image $(XLNX_VERSAL)/Image
PKG_FLAGS += --package.boot_mode=sd
PKG_FLAGS += --package.out_dir $(EMBEDDED_PACKAGE_OUT)
PKG_FLAGS += --package.image_format=ext4
PKG_FLAGS += --package.sd_file $(BUILD_TARGET_DIR)/$(APP_ELF) $(BUILD_TARGET_DIR)/$(XCLBIN)
PKG_FLAGS += --package.sd_file $(HW_EMU_RUN_FILES_REPO)/$(EMBEDDED_EXEC_SCRIPT)

ifneq ($(EN_TRACE),0)

   PKG_FLAGS += --package.sd_file $(PROFILING_CONFIGS_REPO)/xrt.ini

endif

ifdef XRT_ROOT

   PKG_FLAGS += --package.sd_dir $(XRT_ROOT)

endif

# =========================================================
# Step 1. Kernel XO File Generation
# ========================================================
# This step compiles the HLS C PL kernels.
# Outputs: in build/[hw_emu | hw]/ directory
# 	fft_2d.[hw_emu | hw].xo  
#	fft_2d.[hw_emu | hw].xo.compile_summary  
#	v++_fft_2d.[hw_emu | hw].log
# 	$(DATAMOVER_KERNEL_TOP).[hw_emu | hw].xo 
#	$(DATAMOVER_KERNEL_TOP).[hw_emu | hw].xo.compile_summary 
#	v++_$(DATAMOVER_KERNEL_TOP).[hw_emu | hw].log
#	_x
KERNEL_XOS := $(BUILD_TARGET_DIR)/$(FFT_2D_KERNEL_XO).xo \
				  $(BUILD_TARGET_DIR)/$(DATAMOVER_KERNEL_XO).xo

kernels: $(KERNEL_XOS)

$(BUILD_TARGET_DIR)/$(FFT_2D_KERNEL_XO).xo:
	mkdir -p $(BUILD_TARGET_DIR); \
	cd $(BUILD_TARGET_DIR); \
	v++ --target $(TARGET) $(FFT_2D_KERNEL_VPP_FLAGS) \
		$(VPP_FLAGS) -c -k fft_2d \
		$(FFT_2D_KERNEL_SRC) -o $@

$(BUILD_TARGET_DIR)/$(DATAMOVER_KERNEL_XO).xo:
	mkdir -p $(BUILD_TARGET_DIR); \
	cd $(BUILD_TARGET_DIR); \
	v++ --target $(TARGET) $(DATAMOVER_KERNEL_VPP_FLAGS) \
		$(VPP_FLAGS) -c -k $(DATAMOVER_KERNEL_TOP) \
		$(DATAMOVER_KERNEL_SRC) -o $@

# =========================================================
# Step 2. XCLBIN File Generation
# ========================================================
# This step links the graph executable and
# the kernels into a xclbin file.
# Outputs: in build/[hw_emu | hw]/ directory
#	vck190_hls_fft_2d.[hw_emu | hw].xclbin
#	vck190_hls_fft_2d.[hw_emu | hw].xclbin.info
#	vck190_hls_fft_2d.[hw_emu | hw].xclbin.link_summary
#	vck190_hls_fft_2d.[hw_emu | hw].xsa
#	vck190_hls_fft_2d.[hw_emu | hw].log
xclbin: $(BUILD_TARGET_DIR)/$(XCLBIN)

$(BUILD_TARGET_DIR)/$(XCLBIN): kernels
	cd $(BUILD_TARGET_DIR);	\
	v++ -l $(VPP_FLAGS) $(VPP_LINK_FLAGS) -t $(TARGET) -o $@ \
		    $(KERNEL_XOS)

# =========================================================
# Step 3. A72 Application Executable File Generation
# ========================================================
# This step compiles the A72 application. This step is the 
# same for TARGET=[hw_emu | hw]. Compile the PS code.
# Outputs: in build/ directory
#	fft_2d_hls_app.o
# 	fft_2d_hls_xrt.elf
application:
	mkdir -p $(BUILD_TARGET_DIR)
	@rm -rf $(BUILD_TARGET_DIR)/fft_2d_hls_app.o $(BUILD_TARGET_DIR)/$(APP_ELF)
	$(CXX) $(GCC_FLAGS) $(GCC_INC_FLAGS) $(APP_SRC_CPP) -o $(BUILD_TARGET_DIR)/fft_2d_hls_app.o $(GCC_INC_LIB) $(GCC_LIB)
	$(CXX) $(BUILD_TARGET_DIR)/fft_2d_hls_app.o $(GCC_INC_LIB) $(GCC_LIB) -o $(BUILD_TARGET_DIR)/$(APP_ELF)

# =========================================================
# Step 4. Package Generation 
# ========================================================
# This step generates the package folder which contains the
# ./launch_hw_emu.sh script to launch hardware emulation
# if TARGET=hw_emu and the sd_card.img file. 
# Outputs: in build/[hw_emu | hw]/ directory
# 	a.xclbin
# 	package/ directory
# 	v++.package_summary
# 	v++_package.log
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
	./launch_hw_emu.sh -run-app $(EMBEDDED_EXEC_SCRIPT) -timeout 86400 | tee embedded_run.log

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

###########################################################################

# =========================================================
# Primary Build Targets
# ==> sd_card
# ==> run
# ==> clean_target
# ==> clean_insts
# ==> clean_fftpt
# ==> cleanall
# ========================================================

#Build the design without running the hardware emulation
sd_card: kernels xclbin application package

#Build the design and then run hardware emulation
run: sd_card run_emu

#Clean generated files
clean_target:
	@echo "Cleaning Target Build Dir..."
	rm -rf $(BUILD_TARGET_DIR)

clean_insts:
	@echo "Cleaning $(FFT_2D_INSTS)x Build Dir..."
	rm -rf $(INSTS_BLD_DIR)

clean_fftpt:
	@echo "Cleaning $(MAT_ROWS)x$(MAT_COLS) FFT Build Dir..."
	rm -rf $(FFTPT_BLD_DIR)

clean_fft2d_dt:
	@echo "Cleaning $(FFT_2D_DT)_impl Builds..."
	rm -rf $(FFT_2D_DT_DIR)

cleanall:
	rm -rf $(BASE_BLD_DIR)
	rm -rf vivado* .Xil ../vivado* ../.Xil *.log vitis* ../vitis* ../*.log core*
