#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Stephen MacMahon

SHELL := /bin/bash
TOOLS ?= ""
VERSION ?= 2026.1
LINUX_TMP_DIR ?= /tmp/${USER}/${VERSION}/zcu102/petalinux

all: create_xsa sysroots unified_workspace

create_xsa:
	$(RM) -rf ./zcu102
	vivado -mode tcl -source create_xsa.tcl

sysroots:
	$(RM) -rf ./xilinx-zcu102-v2026.1 ${LINUX_TMP_DIR}
	petalinux-create project --template zynqMP -n ./xilinx-zcu102-v2026.1 --tmpdir ${LINUX_TMP_DIR}
	cd ./xilinx-zcu102-v2026.1 && petalinux-config --get-hw-description=../zcu102/design_1_wrapper.xsa --silentconfig
	sed -i 's/template/zcu102-rev1.0/' ./xilinx-zcu102-v2026.1/project-spec/configs/config
	cd ./xilinx-zcu102-v2026.1 && petalinux-build
	cd ./xilinx-zcu102-v2026.1 && petalinux-build --sdk
	cd xilinx-zcu102-v2026.1/images/linux/ && unset LD_LIBRARY_PATH && ./sdk.sh -y -d sysroot

unified_workspace:
	$(RM) -rf ./unified_workspace
	vitis -s unified_workspace.py ./zcu102/design_1_wrapper.xsa

clean:
	$(RM) -rf ./zcu102 ./classic_workspace ./unified_workspace ./xilinx-zcu102-v2026.1
