 #
 # Copyright 2021 Xilinx, Inc.
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
 #

ECHO=@echo

TARGET            ?= hw
PLATFORM_CUSTOM   ?= vck190_thin

BASE_PFM          := ./platform/sw/build/${PLATFORM_CUSTOM}/export/${PLATFORM_CUSTOM}/${PLATFORM_CUSTOM}.xpfm
XPFM              := ../${BASE_PFM}

IP_DIR            := ../ip
XO                := $(wildcard ${IP_DIR}/xo_${TARGET}/*.xo)
GRAPH_O           := ${IP_DIR}/aie/libadf.a
XCLBIN            := my.xclbin

PS_APPS_DIR       := ../ps_apps
PS_EXE            := $(wildcard ${PS_APPS_DIR}/exe/*.exe)

PETALINUX_IMAGES  := ../petalinux/linux/images/linux
SD_CARD           := ../package_output_${TARGET}
BUILD_DIR         := build_${TARGET}

# Force ILA_EN=0 (Disabled) for TARGET=hw_emu
ifeq (${TARGET}, hw_emu)
    ILA_EN  = 0
else
    ILA_EN ?= 0
endif

ifeq (${ILA_EN}, 1)  
    SYSTEM_CFG := $(wildcard ./src/*.cfg)
else 
    SYSTEM_CFG := ./src/system.cfg
endif

.PHONY: clean all

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate the sd_card in this design, including platform/petalinux/ip/ps_apps"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all: ${BUILD_DIR} ${SD_CARD}

${BUILD_DIR}:
	mkdir ${BUILD_DIR}

${BUILD_DIR}/${XCLBIN}: ${XO} ${SYSTEM_CFG}
	cd ${BUILD_DIR} && v++ -l -t ${TARGET} -g --save-temps --platform ../${XPFM} $(addprefix --config ../, ${SYSTEM_CFG}) $(addprefix ../, ${XO}) ../${GRAPH_O} -o ${XCLBIN} |& tee vpp_link.log

${SD_CARD}: ${BUILD_DIR}/${XCLBIN} ${PS_EXE} ${GRAPH_O}
	cd ${BUILD_DIR} && v++ -p --package.out_dir ../${SD_CARD} -t ${TARGET} -g --save-temps -f ../${XPFM} --package.image_format=fat32 --package.boot_mode=sd --package.rootfs=../${PETALINUX_IMAGES}/rootfs.cpio.gz.u-boot  --package.kernel_image=../${PETALINUX_IMAGES}/Image ${XCLBIN} ../${GRAPH_O} $(addprefix --package.sd_file=../, ${PS_EXE}) |& tee vpp_pack.log
	if [ -e my.ltx ] ; then cp -f my.ltx $(SD_CARD)/probe_0.ltx ; else echo "no probe_0 ltx file" ; fi 
	echo ${PLATFORM_CUSTOM} > ${SD_CARD}/sd_card/platform_desc.txt

clean:
	rm -rf ${SD_CARD} ${BUILD_DIR}

