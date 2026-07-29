#
# Copyright (C) 2022,      Xilinx, Inc. All rights reserved.
# Copyright (C) 2022-2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

ECHO                      := @echo

REQUIRED_VERSION          ?= 2026.1
BOARD_NAME                ?= vck190
PLATFORM_CUSTOM           ?= ${BOARD_NAME}_thin
LINUX_MACHINE             ?= my-${BOARD_NAME}-versal
CURR_DIR                  := $(shell pwd)
DEPLOY_DIR                := $(abspath ${CURR_DIR}/../${LINUX_MACHINE})/images/linux
TMP_DIR                   ?= tmp
LINUX_TMP_DIR             ?= /${TMP_DIR}/${USER}/${REQUIRED_VERSION}/${BOARD_NAME}/${SW_BUILD_TOOL}
LINUX_ETH_CONFIG          ?= DHCP
CLEAN_AFTER_BUILDS        ?= false

REPO                      :=$(abspath ${CURR_DIR}/bin/repo)
REPO_URL                  :=https://storage.googleapis.com/git-repo-downloads/repo
YOCTO_URL                 :=https://github.com/Xilinx/yocto-manifests.git
YOCTO_REL                 :=rel-v2026.1
YOCTO_FLOW                ?=edf
SEG_CONF                  ?= false

ifeq (${BOARD_NAME}, vck190)
  SOC_FAMILY                         := versal
  GEN_MACHINE_OPT                    :=
else ifeq (${BOARD_NAME}, vck190seg)
  SOC_FAMILY                         := versal
  GEN_MACHINE_OPT                    := -g full
else ifeq (${BOARD_NAME}, vek280)
  SOC_FAMILY                         := versal
  GEN_MACHINE_OPT                    :=
else ifeq (${BOARD_NAME}, vek385)
  SOC_FAMILY                         := versal-2ve-2vm
  GEN_MACHINE_OPT                    := -g full
else ifeq (${BOARD_NAME}, vrk160)
  SOC_FAMILY                         := versal
  GEN_MACHINE_OPT                    := -g full
endif

ifeq (${YOCTO_FLOW}, edf)
  YOCTO_MANIFEST          := default-edf.xml
  YOCTO_ENV_SETUP         := edf-init-build-env
  RECIPE_SYSROOT          := meta-edf-app-sdk
  IMAGE                   := edf-linux-disk-image
  BOOTBIN                 := xilinx-bootbin
else ifeq (${YOCTO_FLOW}, edf_core)
  YOCTO_MANIFEST          := default-edf.xml
  YOCTO_ENV_SETUP         := edf-init-build-env
  RECIPE_SYSROOT          := meta-edf-app-sdk
  IMAGE                   := core-image-minimal
  BOOTBIN                 := xilinx-bootbin
else
  YOCTO_MANIFEST          := default.xml
  YOCTO_ENV_SETUP         := setupsdk
  RECIPE_SYSROOT          := build-sysroots
  RECIPE_SYSROOT_EXTRA    := -c build_native_sysroot
  IMAGE                   := petalinux-image-minimal
  BOOTBIN                 := petalinux-image-minimal
endif

#LINUX_YOCTO_ROOT          ?= ${CURR_DIR}/${YOCTO_FLOW}
LINUX_YOCTO_ROOT          ?= $(abspath ${LINUX_TMP_DIR}/../${YOCTO_FLOW})
POKY_ENV_SETUP            := ${LINUX_YOCTO_ROOT}/sources/poky/buildtools/environment-setup-x86_64-pokysdk-linux
WIC_CMD                   := ${LINUX_YOCTO_ROOT}/sources/poky/scripts/wic

META_DIR                  := ${CURR_DIR}/../${LINUX_MACHINE}-meta
SYSROOT_DIR               := ${CURR_DIR}/../sysroot
SYSROOT_ENV               := ${SYSROOT_DIR}/environment-setup-cortexa72-cortexa53-amd-linux
NATIVE_SYSROOT            :=  ${LINUX_YOCTO_ROOT}/sources/poky/buildtools/sysroots/x86_64-pokysdk-linux/

AUTO_CONF_SRC             := ./src/conf/auto.conf
LOCAL_CONF                := ${LINUX_YOCTO_ROOT}/build/conf/local.conf

XSA_DIR                   := ${CURR_DIR}/../../vivado/build/xsa_platform
#XSA_FILE                  := ${PLATFORM_CUSTOM}_base.xsa
#XSA_FILE                  := ${PLATFORM_CUSTOM}_fixed.xsa


RECIPE_ETH_STATIC         := ${META_DIR}/recipes-bsp/init-ifupdown
SRC_DIR_ETH_STATIC        := ../src/${BOARD_NAME}/recipes-bsp/init-ifupdown

RECIPE_DEVICE_TREE        := ${META_DIR}/recipes-bsp/device-tree
SRC_DIR_DEVICE_TREE       := ../src/${BOARD_NAME}/recipes-bsp/device-tree

RECIPE_AUTOMOUNT_MMC      := ${META_DIR}/recipes-core/base-files
SRC_DIR_AUTOMOUNT_MMC     := ../src/${BOARD_NAME}/recipes-core/base-files

#BUILD_BOOTBINS_DIR        := ${PROJ_DIR}/images/linux
BOOTBIN_EXT               := boot.bin-extracted
ZOCL                      := zocl.dtsi
ZOCL_ABS                  := $(abspath ${XSA_DIR}/${ZOCL})
ZOCL_SRC                  := $(abspath ../src/${BOARD_NAME}/package_zocl)
DOMAIN_YAML               := $(abspath ../src/${BOARD_NAME}/package_zocl/domain.yaml)

PS_APPS_LINUX_DIR         := ../../ps_apps/linux
PACKAGE_OUTPUT_DIR        := ../../package_linux_hw
PACKAGE_BUILD_DIR         := ../../vitis/build_hw
USER_FILES                := $(wildcard ${PACKAGE_BUILD_DIR}/*.xclbin)
USER_FILES                += $(wildcard ${PS_APPS_LINUX_DIR}/exe/*.exe)
ifeq (${SEG_CONF}, true)
  USER_FILES              += $(wildcard ${PACKAGE_OUTPUT_DIR}/*.pdi)
  USER_FILES              += $(wildcard ${PACKAGE_OUTPUT_DIR}/*.dtbo)
endif
USER_FILES_GUARD          := $(addsuffix .guard, $(USER_FILES))

.PHONY: add_layers machineconf poky_env yocto_env

.PHONY: all
all: add_layers machineconf update_build update_sysroot update_zocl


.PHONY: help
help:
	@echo "Make targets: add_layers machineconf update_eth update_device_tree update_build update_sysroot"

${REPO}:
	rm -rf bin
	mkdir bin
	curl ${REPO_URL} > ${REPO}
	chmod a+x ${REPO}

${LINUX_YOCTO_ROOT}/${YOCTO_ENV_SETUP}: ${REPO}
	mkdir -p ${LINUX_YOCTO_ROOT}
	cd ${LINUX_YOCTO_ROOT} && ${REPO} init -u ${YOCTO_URL} -b ${YOCTO_REL} -m ${YOCTO_MANIFEST}
	cd ${LINUX_YOCTO_ROOT} && ${REPO} sync

${POKY_ENV_SETUP}: ${LINUX_YOCTO_ROOT}/${YOCTO_ENV_SETUP}
	cd ${LINUX_YOCTO_ROOT} && ./sources/poky/scripts/install-buildtools

${LOCAL_CONF}: ${LINUX_YOCTO_ROOT}/${YOCTO_ENV_SETUP}
	cd ${LINUX_YOCTO_ROOT} && . ${LINUX_YOCTO_ROOT}/${YOCTO_ENV_SETUP}
	cat   ${AUTO_CONF_SRC}                                                >> ${LOCAL_CONF}
	echo -e "CONFIG_YOCTO_MACHINE_NAME = \"${LINUX_MACHINE}\""            >> ${LOCAL_CONF}
	echo -e "TMPDIR                    = \"${LINUX_TMP_DIR}\""            >> ${LOCAL_CONF}
	echo -e "DEPLOY_DIR_BOOTBIN        = \"${DEPLOY_DIR}\""               >> ${LOCAL_CONF}

${META_DIR}: ${LOCAL_CONF}
	cd ${LINUX_YOCTO_ROOT} && . ${LINUX_YOCTO_ROOT}/${YOCTO_ENV_SETUP} && bitbake-layers create-layer ${META_DIR}
	cd ${LINUX_YOCTO_ROOT} && . ${LINUX_YOCTO_ROOT}/${YOCTO_ENV_SETUP} && bitbake-layers add-layer ${META_DIR}

yocto_env: ${LINUX_YOCTO_ROOT}/${YOCTO_ENV_SETUP}

poky_env: ${POKY_ENV_SETUP}

add_layers: ${META_DIR}

machineconf: ${POKY_ENV_SETUP}
	cd ${LINUX_YOCTO_ROOT} && . ${LINUX_YOCTO_ROOT}/${YOCTO_ENV_SETUP} && . ${POKY_ENV_SETUP} && gen-machine-conf parse-sdt --domain-file ${DOMAIN_YAML} --soc-family ${SOC_FAMILY} --hw-description ${CURR_DIR}/../sdtgen/build/ ${GEN_MACHINE_OPT} -c conf --machine-name ${LINUX_MACHINE} -O ${LINUX_MACHINE} --native-sysroot ${NATIVE_SYSROOT}

# Replace if needed with custom layer or machine yaml include	@echo "Temporary fix, adding aie and efi to MACHINE_FEATURES in ${LINUX_MACHINE}.conf"
# Replace if needed with custom layer or machine yaml include	sed -i 's/MACHINE_FEATURES += "vdu"/MACHINE_FEATURES += "vdu fpga-overlay aie efi"/g' ${LINUX_YOCTO_ROOT}/build/conf/machine/${LINUX_MACHINE}.conf

.PHONY: update_eth
update_eth:
ifeq (${LINUX_ETH_CONFIG}, STATIC)
	@install -d ${RECIPE_ETH_STATIC}/files
	@cp      -r ${SRC_DIR_ETH_STATIC}/* ${RECIPE_ETH_STATIC}
endif

.PHONY: update_device_tree
update_device_tree:
	@install -d ${RECIPE_DEVICE_TREE}/files
	@cp      -r ${SRC_DIR_DEVICE_TREE}/* ${RECIPE_DEVICE_TREE}/
	@touch      ${RECIPE_DEVICE_TREE}/files/system-conf.dtsi

.PHONY: update_automount_mmc
update_automount_mmc:
	@install -d ${RECIPE_AUTOMOUNT_MMC}
	@cp      -r ${SRC_DIR_AUTOMOUNT_MMC}/* ${RECIPE_AUTOMOUNT_MMC}/

.PHONY: update_bootbin
update_bootbin:
	cd ${LINUX_YOCTO_ROOT} && . ${LINUX_YOCTO_ROOT}/${YOCTO_ENV_SETUP} && MACHINE=${LINUX_MACHINE} bitbake ${BOOTBIN}
ifeq (${CLEAN_AFTER_BUILDS}, true)
	@echo "Moving deploy folder before purging yocto tmp folder"
	mv ${LINUX_TMP_DIR}/deploy ${LINUX_TMP_DIR}/../deploy_bootbin
	rm -rf ${LINUX_TMP_DIR}/*
endif

.PHONY: update_systemd
update_systemd:
	cd ${LINUX_YOCTO_ROOT} && . ${LINUX_YOCTO_ROOT}/${YOCTO_ENV_SETUP} && MACHINE=${LINUX_MACHINE} bitbake systemd-boot
ifeq (${CLEAN_AFTER_BUILDS}, true)
	@echo "Moving deploy folder before purging yocto tmp folder"
	mv ${LINUX_TMP_DIR}/deploy ${LINUX_TMP_DIR}/../deploy_systemd
	rm -rf ${LINUX_TMP_DIR}/*
endif


.PHONY: update_build
update_build:
	cd ${LINUX_YOCTO_ROOT} && . ${LINUX_YOCTO_ROOT}/${YOCTO_ENV_SETUP} && MACHINE=${LINUX_MACHINE} bitbake ${IMAGE}
ifeq (${CLEAN_AFTER_BUILDS}, true)
	@echo "Moving deploy folder before purging yocto tmp folder"
	mv ${LINUX_TMP_DIR}/deploy ${LINUX_TMP_DIR}/../deploy_image
	rm -rf ${LINUX_TMP_DIR}/*
endif


.PHONY: update_bootscr
update_bootscr:
	cd ${LINUX_YOCTO_ROOT} && . ${LINUX_YOCTO_ROOT}/${YOCTO_ENV_SETUP} && bitbake u-boot-edf-scr
ifeq (${CLEAN_AFTER_BUILDS}, true)
	@echo "Moving deploy folder before purging yocto tmp folder"
	mv ${LINUX_TMP_DIR}/deploy ${LINUX_TMP_DIR}/../deploy_bootscr
	rm -rf ${LINUX_TMP_DIR}/*
endif

.PHONY: update_sysroot
update_sysroot:
	cd ${LINUX_YOCTO_ROOT} && . ${LINUX_YOCTO_ROOT}/${YOCTO_ENV_SETUP} && MACHINE=${LINUX_MACHINE} bitbake ${RECIPE_SYSROOT_EXTRA} ${RECIPE_SYSROOT}
	@install -d ${SYSROOT_DIR}
	${LINUX_TMP_DIR}/deploy/sdk/*${RECIPE_SYSROOT}*.sh -y -d ${SYSROOT_DIR}

.PHONY: update_zocl
update_zocl:
ifeq (${PREBUILD_EMBEDDED}, true)
	$(MAKE) ${ZOCL_ABS}
endif


${ZOCL_ABS}:
	cd ${XSA_DIR} && \
	(cd ${LINUX_TMP_DIR}/deploy/images/${LINUX_MACHINE}/ && zip -r ${ZOCL_ABS} ${BOOTBIN_EXT}) && \
	(cd ${ZOCL_SRC} && zip -r ${ZOCL_ABS} .)

.PHONY: sdcard_image
sdcard_image: update_wic_boot update_wic_apps update_bmap wic_xz
	@install -d ${DEPLOY_DIR}
	@cp -r ${LINUX_TMP_DIR}/deploy/images/${LINUX_MACHINE} ${DEPLOY_DIR}

.PHONY: update_wic_boot
update_wic_boot:
	@echo "Copy BOOT.bin to primary partition in wic image"
ifeq (${SEG_CONF}, true)
	. ${SYSROOT_ENV} && \
    wic cp  `readlink -f ${LINUX_TMP_DIR}/deploy/images/${LINUX_MACHINE}/BOOT-${LINUX_MACHINE}.bin` `readlink -f ${LINUX_TMP_DIR}/deploy/images/${LINUX_MACHINE}/${IMAGE}-${LINUX_MACHINE}.rootfs.wic`:1/BOOT.bin
else
	. ${SYSROOT_ENV} && \
    wic cp ${PACKAGE_OUTPUT_DIR}/BOOT.bin `readlink -f ${LINUX_TMP_DIR}/deploy/images/${LINUX_MACHINE}/${IMAGE}-${LINUX_MACHINE}.rootfs.wic`:1/BOOT.bin
endif


.PHONY: update_wic_apps
update_wic_apps: $(USER_FILES_GUARD)

$(USER_FILES_GUARD):
	@echo "Copy $(basename $@) to ${IMAGE}-${LINUX_MACHINE}.rootfs.wic:3/home/amd-edf/"
	. ${SYSROOT_ENV} && \
    wic cp $(basename $@) `readlink -f ${LINUX_TMP_DIR}/deploy/images/${LINUX_MACHINE}/${IMAGE}-${LINUX_MACHINE}.rootfs.wic`:3/home/amd-edf/
	@touch $@

.PHONY: update_bmap
update_bmap:
	. ${SYSROOT_ENV} && \
    bmaptool create `readlink -f ${LINUX_TMP_DIR}/deploy/images/${LINUX_MACHINE}/${IMAGE}-${LINUX_MACHINE}.rootfs.wic` \
    > `readlink -f ${LINUX_TMP_DIR}/deploy/images/${LINUX_MACHINE}/${IMAGE}-${LINUX_MACHINE}.rootfs.wic.bmap`

.PHONY: wic_xz
wic_xz:
	xz -k -f `readlink -f ${LINUX_TMP_DIR}/deploy/images/${LINUX_MACHINE}/${IMAGE}-${LINUX_MACHINE}.rootfs.wic`

#dump-dts:
#	cd ${BUILD_BOOTBINS_DIR} && ${LINUX_TMP_DIR}/sysroots-components/x86_64/dtc-native/usr/bin/dtc -I dtb -O dts -o dump.dts system.dtb

.PHONY: clean
clean:
	rm -rf ${CURR_DIR}/../${LINUX_MACHINE}*
	rm -rf ${META_DIR}
	rm -rf ${USER_FILES_GUARD}
	rm -rf ${LINUX_YOCTO_ROOT}
	rm -rf ${SYSROOT_DIR}
	rm -rf ${LINUX_TMP_DIR}*

.PHONY: ultraclean
ultraclean: clean
	$(RM) -r ${LINUX_YOCTO_ROOT}/sstate-cache
