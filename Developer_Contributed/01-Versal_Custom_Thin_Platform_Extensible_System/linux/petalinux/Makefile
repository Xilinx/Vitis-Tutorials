#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

ECHO                       := @echo

BOARD_NAME                 ?= vck190
LINUX_MACHINE              ?= ${BOARD_NAME}-versal
LINUX_ETH_CONFIG           ?= DHCP
LINUX_BUILD_SOURCES        ?= network
LINUX_LOCAL_DOWNLOADS_DIR  ?= /tmp/plnx-workspace/downloads
LINUX_LOCAL_SSTATE_DIR     ?= /tmp/plnx-workspace/sstate_aarch64_2024.1/aarch64
LINUX_TMP_DIR              ?= /tmp/${USER}/2024.1/${BOARD_NAME}/petalinux

CURR_DIR                   := $(shell pwd)
PROJ_DIR                   := ${CURR_DIR}/../${LINUX_MACHINE}
META_DIR                   := ${CURR_DIR}/../${LINUX_MACHINE}-meta
SYSROOT_DIR                := ${CURR_DIR}/../sysroot

XSA_DIR                    := ${CURR_DIR}/../../vivado/build/xsa_platform
XSA_FILE                   := $(shell basename ${XSA_DIR}/*.xsa)

CFG                        := ./src/config

FILE_MAIN_CFG              := ${PROJ_DIR}/project-spec/configs/config
FILE_ROOT_FS_CFG           := ${PROJ_DIR}/project-spec/configs/rootfs_config
FILE_BSP_CFG               := ${PROJ_DIR}/project-spec/meta-user/conf/petalinuxbsp.conf

SRC_DIR_DEVICE_TREE        := ../src/recipes-bsp/device-tree/files
DEST_DIR_DEVICE_TREE       := ${PROJ_DIR}/project-spec/meta-user/recipes-bsp/device-tree/files

SRC_DIR_ETH_STATIC         := ../src/recipes-bsp/init-ifupdown/files
DEST_DIR_ETH_STATIC        := ${PROJ_DIR}/project-spec/configs/init-ifupdown

.PHONY: help all clean

help: 
	echo "Make targets: create update_hw update_config update_device_tree update_build update_bootimage update_sysroot"

all: ${PROJ_DIR} 

${PROJ_DIR}: ${XSA_DIR}/${XSA_FILE}
	make clean
	make create
	make update_hw
	make update_device_tree
	make update_config
	make update_build
	make update_bootimage
	make update_sysroot
	make dump_dts

create: 
	cd ../ && petalinux-create -t project --template versal -n ${LINUX_MACHINE} --tmpdir ${LINUX_TMP_DIR}
	${CFG} --file ${FILE_MAIN_CFG}      --keep-case --enable  CONFIG_YOCTO_BUILDTOOLS_EXTENDED
ifeq (${LINUX_BUILD_SOURCES}, local)
	${CFG} --file ${FILE_MAIN_CFG}      --keep-case --enable  CONFIG_YOCTO_BB_NO_NETWORK
	${CFG} --file ${FILE_MAIN_CFG}      --keep-case --disable CONFIG_YOCTO_NETWORK_SSTATE_FEEDS
	${CFG} --file ${FILE_MAIN_CFG}      --set-str CONFIG_YOCTO_LOCAL_SSTATE_FEEDS_URL ${LINUX_LOCAL_SSTATE_DIR}
	${CFG} --file ${FILE_MAIN_CFG}      --set-str CONFIG_PRE_MIRROR_URL "file\:\/\/"${LINUX_LOCAL_DOWNLOADS_DIR}
endif

update_hw:
	cd ${PROJ_DIR} && petalinux-config --get-hw-description=${XSA_DIR} --silentconfig

update_device_tree:
	cp -r ${SRC_DIR_DEVICE_TREE}/* ${DEST_DIR_DEVICE_TREE}/

update_config:
	chmod 777 ${CFG}
	${CFG} --file ${FILE_MAIN_CFG}      --set-str CONFIG_SUBSYSTEM_RFS_FORMATS "cpio cpio.gz cpio.gz.u-boot tar.gz jffs2 ext4"
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable  xrt
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable  xrt-dev
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable  packagegroup-petalinux-openamp
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable  packagegroup-petalinux-openamp-dev
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable  libsysfs
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable  ai-engine-driver
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable  ai-engine-driver-dev
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable  sysfsutils-dev
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --disable dropbear
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --disable packagegroup-core-ssh-dropbear
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --disable imagefeature-ssh-server-dropbear
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable  dnf
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable  openssh
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable  openssh-ssh
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable  openssh-sshd
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable  openssh-sftp
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable  openssh-sftp-server
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable  openssh-scp
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable  openssh-keygen
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable  openssh-misc
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable  imagefeature-ssh-server-openssh
	cd ${PROJ_DIR} && petalinux-config  --silentconfig
	cd ${PROJ_DIR} && petalinux-config -c rootfs --silentconfig
ifeq (${LINUX_ETH_CONFIG}, STATIC)
	cp -r ${SRC_DIR_ETH_STATIC}/* ${DEST_DIR_ETH_STATIC}/
endif
      
update_build:
	cd ${PROJ_DIR} && petalinux-build

update_bootimage:
	cd ${PROJ_DIR} && petalinux-package --boot --plm --psmfw --u-boot --dtb --force

update_sysroot:
ifeq (${LINUX_BUILD_SOURCES}, local)
# Turn Network Access ON here, since it's not included in the MIRROR
	${CFG} --file ${FILE_MAIN_CFG}      --keep-case --disable  CONFIG_YOCTO_BB_NO_NETWORK
	cd ${PROJ_DIR} && petalinux-config  --silentconfig
endif
	mkdir -p ${SYSROOT_DIR}
	cd ${PROJ_DIR} && petalinux-build   --sdk
	cd ${PROJ_DIR} && petalinux-package --sysroot -d ${SYSROOT_DIR}
ifeq (${LINUX_BUILD_SOURCES}, local)
# Turn Network Access OFF again here, since it's not required anymore from here
	${CFG} --file ${FILE_MAIN_CFG}      --keep-case --enable   CONFIG_YOCTO_BB_NO_NETWORK
	cd ${PROJ_DIR} && petalinux-config  --silentconfig
endif

dump_dts:
	cd ${PROJ_DIR}/images/linux && dtc -I dtb -O dts -o dump.dts system.dtb

clean:
	rm -rf ${PROJ_DIR}
	rm -rf ${META_DIR}
	rm -rf ${SYSROOT_DIR}
	rm -rf ${LINUX_TMP_DIR}

