#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11



# =========================================================
# PROJECT and MODULE REPOS
# =========================================================
#relative beamforming directory
RELATIVE_PROJECT_DIR := ../

#absolute beamforming directory = <user path>/Tutorials/AI_Engine/beamforming
PROJECT_DIR	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))

MODULE_01_DIR := $(PROJECT_DIR)/Module_01_Custom_Platform
MODULE_02_DIR := $(PROJECT_DIR)/Module_02_AI_Engine_Design
MODULE_03_DIR := $(PROJECT_DIR)/Module_03_PL_Design
MODULE_04_DIR := $(PROJECT_DIR)/Module_04_AI_Engine_and_PL_Integration
MODULE_05_DIR := $(PROJECT_DIR)/Module_05_Baremetal_Host_Application
MODULE_07_DIR := $(PROJECT_DIR)/Module_07_Petalinux


LOCAL_INC          := $(MODULE_02_DIR)/src
LOCAL_SRC          := $(MODULE_02_DIR)/src/kernels
LOCAL_LIB          := $(MODULE_02_DIR)/src/inc

LOCAL_UTILS        := ./src/utils

#EXE_INCLUDE     := -I$(LOCAL_INC) -I$(LOCAL_SRC) -I$(LOCAL_LIB) 
EXE_INCLUDE     := -I$(LOCAL_UTILS)

DLBF_UTILS      := $(LOCAL_UTILS)/utils_linux.cpp $(LOCAL_UTILS)/utils_dlbf.cpp $(LOCAL_UTILS)/dlbf_gold0.cpp $(LOCAL_UTILS)/dlbf_gold1.cpp $(LOCAL_UTILS)/dlbf_din.cpp $(LOCAL_UTILS)/dlbf_cin.cpp
ULBF_UTILS      := $(LOCAL_UTILS)/utils_ulbf.cpp $(LOCAL_UTILS)/ulbf_gold0.cpp $(LOCAL_UTILS)/ulbf_din.cpp $(LOCAL_UTILS)/ulbf_cin.cpp


PFM_SYSROOT     := $(MODULE_07_DIR)/build/vck190_linux/images/linux/sdk/sysroots/cortexa72-cortexa53-xilinx-linux/
SYSROOT_INCLUDE := ${PFM_SYSROOT}/usr/include
SYSROOT_LIB     := ${PFM_SYSROOT}/usr/lib
AIE_INCLUDE     := $(PFM_SYSROOT)/usr/include/xaiengine
AIE_LINKER      := $(PFM_SYSROOT)/lib
LIB             := $(PFM_SYSROOT)

XRT_INCLUDE     := $(PFM_SYSROOT)/usr/include/xrt

CXX             := $(XILINX_VITIS)/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-g++

AIE_APP         := src/test_bf_app.cpp src/test_bf_func.cpp src/test_bf_perf.cpp $(DLBF_UTILS) $(ULBF_UTILS)
AIE_APP_EXE     := test_bf_app.exe
AIE_APP_CTRL    := $(MODULE_02_DIR)/build/Work/ps/c_rts/aie_control_xrt.cpp
CFLAGS          := -O -g -Wall -DXAIE_DEBUG 


exe: ${AIE_APP_EXE}

${AIE_APP_EXE}: ${AIE_APP}
	$(CXX) $(CFLAGS) $(AIE_APP) $(AIE_APP_CTRL) -o $(AIE_APP_EXE) --sysroot=$(PFM_SYSROOT) -I$(SYSROOT_INCLUDE) -I$(XILINX_VITIS)/aietools/include/ -I$(AIE_INCLUDE) -I$(XRT_INCLUDE) $(EXE_INCLUDE) -Xlinker -rpath-link=$(PFM_SYSROOT) -ladf_api_xrt -L$(XILINX_VITIS)/aietools/lib/aarch64.o -L$(PFM_SYSROOT)/../ -lxrt_coreutil -L$(AIE_LINKER)


clean:
	rm -rf ${AIE_APP_EXE}
	clear
