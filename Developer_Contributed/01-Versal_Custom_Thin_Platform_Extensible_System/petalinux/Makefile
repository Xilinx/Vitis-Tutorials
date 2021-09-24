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

CPU_ARCH  :=a72
PROJ_NAME ?=linux
SYSROOT   :=../sysroot

XSA_PATH  ?=$(shell pwd)/../platform/hw/build

CFG       :=./src/config

SRC_PATH_DEVICE_TREE  :=./src/device-tree
DEST_PATH_DEVICE_TREE :=${PROJ_NAME}/project-spec/meta-user/recipes-bsp/device-tree

FILE_MAIN_CFG    :=${PROJ_NAME}/project-spec/configs/config
FILE_ROOT_FS_CFG :=${PROJ_NAME}/project-spec/configs/rootfs_config

.PHONY: 

all: ${PROJ_NAME} 

${PROJ_NAME}: ${XSA_PATH}
	make clean
	make create
	make update_hw
	make update_device_tree
	make update_config
	make update_build
	make update_bootimage
	make update_sysroot

targets:
	echo "Make targets: create update_hw update_config update_device_tree update_build update_bootimage update_sysroot"

help: targets

create: 
	petalinux-create -t project --template versal -n ${PROJ_NAME}
	${CFG} --file ${FILE_MAIN_CFG}      --keep-case --enable YOCTO_BUILDTOOLS_EXTENDED 

update_hw:
	cd ${PROJ_NAME} && petalinux-config --get-hw-description=${XSA_PATH} --silentconfig

update_device_tree:
	cp -r ${SRC_PATH_DEVICE_TREE}/* ${DEST_PATH_DEVICE_TREE}/

update_config:
	${CFG} --file ${FILE_MAIN_CFG}      --set-str CONFIG_SUBSYSTEM_RFS_FORMATS "cpio cpio.gz cpio.gz.u-boot tar.gz jffs2 ext4"
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable xrt 
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable xrt-dev
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable auto-login
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable packagegroup-petalinux-openamp
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable packagegroup-petalinux-openamp-dev 
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable libsysfs
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable ai-engine-driver
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable ai-engine-driver-dev 
	${CFG} --file ${FILE_ROOT_FS_CFG}   --keep-case --enable sysfsutils-dev 
	cd ${PROJ_NAME} && petalinux-config --silentconfig
	cd ${PROJ_NAME} && petalinux-config -c rootfs --silentconfig 
#	cd ${PROJ_NAME}/build/conf && sed -i 's/ATF_CONSOLE_versal-generic = "dcc"/ATF_CONSOLE_versal-generic = "pl011"/g' plnxtool.conf

update_build:
	cd ${PROJ_NAME} && petalinux-build

update_bootimage:
	@echo "BOOT image for base platforms"
	cd ${PROJ_NAME} && petalinux-package --boot --plm --psmfw --u-boot --dtb --force

update_sysroot:
	cd ${PROJ_NAME} && mkdir -p ${SYSROOT}
	cd ${PROJ_NAME} && petalinux-build --sdk
	cd ${PROJ_NAME} && petalinux-package --sysroot -d ${SYSROOT}

dump-dts:
	cd ${PROJ_NAME}/images/linux && dtc -I dtb -O dts -o dump.dts system.dtb

clean:
	$(RM) -r ${PROJ_NAME}
	$(RM) -r sysroot

ultraclean:
	$(MAKE) ultraclean -C ${PROJ_NAME}

rmproj:
	$(RM) -r ${PROJ_NAME}

