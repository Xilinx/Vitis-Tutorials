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

ECHO              := @echo

TARGET            ?= hw
PLATFORM_CUSTOM   ?= vck190_thin

XPFM_EXPORT       := ../platform/sw/build/${PLATFORM_CUSTOM}/export/${PLATFORM_CUSTOM}
XPFM              := ${XPFM_EXPORT}/${PLATFORM_CUSTOM}.xpfm

LINUX_KERNEL      := ${XPFM_EXPORT}/sw/Image
LINUX_ROOTFS      := ${XPFM_EXPORT}/sw/rootfs.cpio.gz.u-boot

IP_DIR            := ../ip
XO                := $(wildcard ${IP_DIR}/xo_hw/*.xo)
GRAPH_O           := ${IP_DIR}/aie/libadf.a
XCLBIN            := my.xclbin

PS_APPS_DIR       := ../ps_apps
PS_EXE            := $(wildcard ${PS_APPS_DIR}/exe/*.exe)

SD_CARD           := ../package_output_${TARGET}
BUILD_DIR         := build_${TARGET}

ILA_LTX           := ${BUILD_DIR}/_x/link/vivado/vpl/prj/probe_0.ltx

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

${BUILD_DIR}/${XCLBIN}: ${XO} ${GRAPH_O} ${SYSTEM_CFG}
	cd ${BUILD_DIR} && \
		v++ --link --debug --save-temps \
			--target ${TARGET} \
			--platform ../${XPFM} \
			$(addprefix --config ../, ${SYSTEM_CFG}) \
			$(addprefix ../, ${XO}) \
			../${GRAPH_O} \
			--output ${XCLBIN} \
			|& tee vpp_link.log

${SD_CARD}: ${BUILD_DIR}/${XCLBIN} ${PS_EXE} ${GRAPH_O} ${LINUX_KERNEL} ${LINUX_ROOTFS}
	cd ${BUILD_DIR} && \
		v++ --package --debug --save-temps \
			--target ${TARGET} \
			--platform ../${XPFM} \
			--package.out_dir ../${SD_CARD} \
			--package.boot_mode=sd \
			--package.image_format=fat32 \
			--package.kernel_image=../${LINUX_KERNEL} \
			--package.rootfs=../${LINUX_ROOTFS}  \
			../${GRAPH_O} \
			${XCLBIN} \
			$(addprefix --package.sd_file=../, ${PS_EXE}) \
			|& tee vpp_pack.log
	echo ${PLATFORM_CUSTOM} > ${SD_CARD}/sd_card/platform_desc.txt
	make update_ila

update_ila:
ifeq (${TARGET}, hw)
	if [ -e ${ILA_LTX} ] ; \
		then cp -f ${ILA_LTX} $(SD_CARD)/ ; \
		else echo "no probe_0 ltx file" ; \
	fi 
endif

clean:
	rm -rf ${SD_CARD} ${BUILD_DIR}

