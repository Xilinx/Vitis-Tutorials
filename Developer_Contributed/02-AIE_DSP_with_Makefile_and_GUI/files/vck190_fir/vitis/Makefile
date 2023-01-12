# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


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
XSA               := my.xsa

PS_APPS_DIR       := ../ps_apps
PS_EXE            := $(wildcard ${PS_APPS_DIR}/exe/*.exe)

SD_CARD           := ../package_output_${TARGET}
SD_CARD_EXTRA     := sd_card
SD_CARD_DATA      := sd_card/data
SD_CARD_PROFILING := sd_card/a72_profiling

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

${BUILD_DIR}/${XSA}: ${XO} ${GRAPH_O} ${SYSTEM_CFG}
	cd ${BUILD_DIR} && \
		v++ --link --debug --save-temps \
			--target ${TARGET} \
			--platform ../${XPFM} \
			$(addprefix --config ../, ${SYSTEM_CFG}) \
			$(addprefix ../, ${XO}) \
			../${GRAPH_O} \
			--output ${XSA} \
			|& tee vpp_link.log

${SD_CARD_DATA}:
	mkdir -p ${SD_CARD_EXTRA}
	cd ${SD_CARD_EXTRA} && unzip ./../../../sd_card/data/data.zip

${SD_CARD_PROFILING}:
	mkdir -p ${SD_CARD_PROFILING}
	cd ${SD_CARD_PROFILING} && unzip ./../../../../sd_card/a72_profiling/Inputs.zip
	cd ${SD_CARD_PROFILING} && unzip ./../../../../sd_card/a72_profiling/Outputs.zip
	cd ${SD_CARD_PROFILING} && cp    ./../../../../sd_card/a72_profiling/launch_all_on_arm72.sh .
	cd ${SD_CARD_PROFILING} && cp    ./../../../../sd_card/a72_profiling/Makefile .
	cd ${SD_CARD_PROFILING} && cp -r ./../../../../sd_card/a72_profiling/TRFO_BenchMark/ .

${SD_CARD}: ${BUILD_DIR}/${XSA} ${PS_EXE} ${GRAPH_O} ${LINUX_KERNEL} ${LINUX_ROOTFS}
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
			${XSA} \
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
