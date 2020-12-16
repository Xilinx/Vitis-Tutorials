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
MODULE_08_DIR := $(PROJECT_DIR)/Module_08_Linux_SW_Application
FINAL_PDI := hw.pdi

DIR_AIE_APP = ${MODULE_02_DIR}/build
DIR_AIE_DATA = ${MODULE_02_DIR}/data
AIE_APP = test_bf_app
AIE_APP_EXE = ${MODULE_08_DIR}/${AIE_APP}.exe

TARGET=hw

LIBADF_A = ${DIR_AIE_APP}/libadf.a
PFM_XPFM=${MODULE_01_DIR}/sw/build/vck190_v1_0/export/vck190_v1_0/vck190_v1_0.xpfm
XCLBIN=${MODULE_04_DIR}/build/rev1/hw/beamforming.rev1.hw.xclbin
AIE_WORK=${DIR_AIE_APP}/Work

DIR_PFM=${MODULE_07_DIR}/build/vck190_linux
PFM_SYSROOT=$(DIR_PFM)/images/linux/sdk/sysroots/aarch64-xilinx-linux/
XRT_INCLUDE=$(PFM_SYSROOT)/usr/include/xrt


VPP = v++
A_XCLBIN = a.xclbin
AIE_XCLBIN = aie.xclbin


.PHONY: clean

all: clean ${A_XCLBIN} package 

package: $(AIE_APP_EXE)
		cp aie/a.xclbin ./aie.xclbin; \
		v++ -p  -t $(TARGET) --save-temps                                           \
                -f $(PFM_XPFM)                                                      \
                --package.rootfs $(DIR_PFM)/images/linux/rootfs.cpio.gz.u-boot      \
                --package.kernel_image $(DIR_PFM)/images/linux/Image                \
		--package.sd_dir $(XRT_INCLUDE)                                     \
                --package.boot_mode=sd                                              \
                --package.out_dir ./ps_pl                                           \
                --package.image_format=ext4                                         \
                --package.defer_aie_run                                             \
                --package.sd_file $(AIE_XCLBIN)                                     \
                --package.sd_file $(AIE_APP_EXE) ${XCLBIN} $(LIBADF_A) ; \
		 mv a.xclbin ps_pl/; \
		 mv *.log ps_pl/; \
		 mv _x/ ps_pl/; \
		 mv v++.package_summary ps_pl/

${A_XCLBIN}: 
	${VPP} -s -p -t ${TARGET} -f ${PFM_XPFM} \
	--package.out_dir ./aie \
	--config ./package.cfg \
	${DIR_AIE_APP}/libadf.a; \
	mv *.log aie/; \
	mv _x aie/; \
	mv v++.package_summary aie/; \
	mv a.xclbin aie/

clean:
	rm -rf aie ps_pl
	rm -rf *pdi* ls *.bin *.exe *.bif
	rm -rf *.log
	rm -rf _x *_summary
	rm -rf *.log *.xclbin *.BIN package package.zip
