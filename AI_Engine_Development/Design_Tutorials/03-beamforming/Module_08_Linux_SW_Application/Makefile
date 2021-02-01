#  © Copyright 2020 Xilinx, Inc.
#  Licensed under the Apache License, Version 2.0 (the 'License');
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  
#       http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an 'AS IS' BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.


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
LOCAL_UTILS        := src/utils

EXE_INCLUDE     := -I$(LOCAL_INC) -I$(LOCAL_SRC) -I$(LOCAL_LIB) -I$(LOCAL_UTILS)
DLBF_UTILS      := $(LOCAL_UTILS)/utils_linux.cpp $(LOCAL_UTILS)/utils_dlbf.cpp $(LOCAL_UTILS)/dlbf_gold0.cpp $(LOCAL_UTILS)/dlbf_gold1.cpp $(LOCAL_UTILS)/dlbf_din.cpp $(LOCAL_UTILS)/dlbf_cin.cpp
ULBF_UTILS      := $(LOCAL_UTILS)/utils_ulbf.cpp $(LOCAL_UTILS)/ulbf_gold0.cpp $(LOCAL_UTILS)/ulbf_din.cpp $(LOCAL_UTILS)/ulbf_cin.cpp


PFM_SYSROOT     := $(MODULE_07_DIR)/build/vck190_linux/images/linux/sdk/sysroots/aarch64-xilinx-linux/
SYSROOT_INCLUDE := ${PFM_SYSROOT}/usr/include
SYSROOT_LIB     := ${PFM_SYSROOT}/usr/lib
AIE_INCLUDE     := $(PFM_SYSROOT)/usr/include/xaiengine
AIE_LINKER      := $(PFM_SYSROOT)/lib
LIB             := $(PFM_SYSROOT)

XRT_INCLUDE     := $(PFM_SYSROOT)/usr/include/xrt

CC              := aarch64-linux-gnu-g++
AIE_APP         := src/test_bf_app.cpp src/test_bf_func.cpp src/test_bf_perf.cpp $(DLBF_UTILS) $(ULBF_UTILS)
AIE_APP_EXE     := test_bf_app.exe
AIE_APP_CTRL    := $(MODULE_02_DIR)/build/Work/ps/c_rts/aie_control_xrt.cpp
CFLAGS          := -O -g -Wall -DXAIE_DEBUG 


exe: ${AIE_APP_EXE}

${AIE_APP_EXE}: ${AIE_APP}
	$(CC) $(CFLAGS) $(AIE_APP) $(AIE_APP_CTRL) -o $(AIE_APP_EXE) --sysroot=$(PFM_SYSROOT) -I$(SYSROOT_INCLUDE) -I$(XILINX_VITIS)/aietools/include/ -I$(AIE_INCLUDE) -I$(XRT_INCLUDE) $(EXE_INCLUDE) -Xlinker -rpath-link=$(PFM_SYSROOT) -ladf_api_xrt -L$(XILINX_VITIS)/aietools/lib/aarch64.o -L$(PFM_SYSROOT)/../ -lxrt_coreutil -L$(AIE_LINKER)


clean:
	rm -rf ${AIE_APP_EXE}
	clear
