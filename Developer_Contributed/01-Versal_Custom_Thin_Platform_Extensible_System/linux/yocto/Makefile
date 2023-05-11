#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

ECHO                      := @echo

LINUX_YOCTO_ROOT          ?= ${HOME}/bin
LINUX_TMP_DIR             ?= /tmp/${USER}/2022.1/yocto
LINUX_ETH_CONFIG          ?= DHCP
LINUX_MACHINE             ?= vck190-versal

CURR_DIR                  := $(shell pwd)
PROJ_DIR                  := ${CURR_DIR}/../${LINUX_MACHINE}
META_DIR                  := ${CURR_DIR}/../${LINUX_MACHINE}-meta
SYSROOT_DIR               := ${CURR_DIR}/../sysroot

AUTO_CONF_SRC             := ./src/conf/auto.conf
AUTO_CONF                 := ${PROJ_DIR}/conf/auto.conf

XSA_DIR                   := ${CURR_DIR}/../../platform/hw/build
XSA_FILE                  := $(shell basename ${XSA_DIR}/*.xsa)

IMAGE                     := petalinux-image-minimal

RECIPE_ETH_STATIC         := ${META_DIR}/recipes-bsp/init-ifupdown
SRC_DIR_ETH_STATIC        := ../src/recipes-bsp/init-ifupdown

RECIPE_DEVICE_TREE        := ${META_DIR}/recipes-bsp/device-tree
SRC_DIR_DEVICE_TREE       := ../src/recipes-bsp/device-tree

RECIPE_AUTOMOUNT_MMC      := ${META_DIR}/recipes-core/base-files
SRC_DIR_AUTOMOUNT_MMC     := ../src/recipes-core/base-files

BUILD_IMAGES_DIR          := ${PROJ_DIR}/images/linux

.PHONY: 

all: ${PROJ_DIR} 

${PROJ_DIR}: ${XSA_DIR}/${XSA_FILE} ${AUTO_CONF_SRC}
	make clean
	make create
	make update_config
	make update_eth
	make update_device_tree
	make update_automount_mmc
	make update_build
	make update_sysroot

help:
	@echo "Make targets: create update_config update_eth update_device_tree update_build update_sysroot"

create: 
	. ${LINUX_YOCTO_ROOT}/setupsdk ${PROJ_DIR}
	. ${LINUX_YOCTO_ROOT}/setupsdk ${PROJ_DIR} && bitbake-layers create-layer ${META_DIR}
	. ${LINUX_YOCTO_ROOT}/setupsdk ${PROJ_DIR} && bitbake-layers add-layer ${META_DIR}

update_config:
	@cp   ${AUTO_CONF_SRC} ${AUTO_CONF}
	@echo -e "TMPDIR            = \"${LINUX_TMP_DIR}\""                   >> ${AUTO_CONF}
	@echo -e ""                                                           >> ${AUTO_CONF}
	@echo -e "DL_DIR            = \"${LINUX_YOCTO_ROOT}/downloads\""      >> ${AUTO_CONF}
	@echo -e "SSTATE_DIR        = \"${LINUX_YOCTO_ROOT}/sstate-cache\""   >> ${AUTO_CONF}
	@echo -e "XSCT_STAGING_DIR  = \"${LINUX_YOCTO_ROOT}/xsct\""           >> ${AUTO_CONF}
	@echo -e ""                                                           >> ${AUTO_CONF}
	@echo -e "MACHINE           = \"${LINUX_MACHINE}\""                   >> ${AUTO_CONF}
	@echo -e ""                                                           >> ${AUTO_CONF}
	@echo -e "HDF_BASE          = \"file://\""                            >> ${AUTO_CONF}
	@echo -e "HDF_PATH          = \"${XSA_DIR}/${XSA_FILE}\""             >> ${AUTO_CONF}
	@echo -e ""                                                           >> ${AUTO_CONF}

update_eth:
ifeq (${LINUX_ETH_CONFIG}, STATIC)
	@install -d ${RECIPE_ETH_STATIC}/files
	@cp      -r ${SRC_DIR_ETH_STATIC}/* ${RECIPE_ETH_STATIC}
endif

update_device_tree:
	@install -d ${RECIPE_DEVICE_TREE}/files
	@cp      -r ${SRC_DIR_DEVICE_TREE}/* ${RECIPE_DEVICE_TREE}/
	@touch      ${RECIPE_DEVICE_TREE}/files/system-conf.dtsi

update_automount_mmc:
	@install -d ${RECIPE_AUTOMOUNT_MMC}
	@cp      -r ${SRC_DIR_AUTOMOUNT_MMC}/* ${RECIPE_AUTOMOUNT_MMC}/

update_build:
	. ${LINUX_YOCTO_ROOT}/setupsdk ${PROJ_DIR} && bitbake ${IMAGE}

update_sysroot:
	. ${LINUX_YOCTO_ROOT}/setupsdk ${PROJ_DIR} && bitbake ${IMAGE} -c populate_sdk
	@install -d ${SYSROOT_DIR}
	${PROJ_DIR}/sdk/*${LINUX_MACHINE}*.sh -y -d ${SYSROOT_DIR}

dump-dts:
	cd ${BUILD_IMAGES_DIR} && ${LINUX_TMP_DIR}/sysroots-components/x86_64/dtc-native/usr/bin/dtc -I dtb -O dts -o dump.dts system.dtb

clean:
	rm -rf ${PROJ_DIR}
	rm -rf ${META_DIR}
	rm -rf ${SYSROOT_DIR}
	rm -rf ${LINUX_TMP_DIR}

ultraclean: clean
	$(RM) -r ${LINUX_YOCTO_ROOT}/sstate-cache

