#
# Copyright (C) 2021-2022, Xilinx, Inc. All rights reserved.
# Copyright (C) 2022-2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#


ECHO                                = @echo

BOARD_NAME                         ?= vck190
PLATFORM_CUSTOM                    ?= ${BOARD_NAME}_thin
USER_DTS                           := ../src/${BOARD_NAME}/recipes-bsp/device-tree/files/system-user.dtsi

ifeq (${BOARD_NAME}, vck190)
  BOARD_DTS                          := versal-vck190-rev1.1
  PS_DOMAIN                          := psv_cortexa72_0
  LOP_FILE                           := lop-a72-imux.dts
else ifeq (${BOARD_NAME}, vck190seg)
  BOARD_DTS                          := versal-vck190-rev1.1
  USER_DTS                           := ../src/vck190/recipes-bsp/device-tree/files/system-user.dtsi
  PS_DOMAIN                          := psv_cortexa72_0
  LOP_FILE                           := lop-a72-imux.dts
else ifeq (${BOARD_NAME}, vek280)
  BOARD_DTS                          := versal-vek280-rev1.1
  PS_DOMAIN                          := psv_cortexa72_0
  LOP_FILE                           := lop-a72-imux.dts
else ifeq (${BOARD_NAME}, vek385)
  BOARD_DTS                          := versal2-vek385-reva
  PS_DOMAIN                          := cortexa78_0
  LOP_FILE                           := lop-a78-imux.dts
else ifeq (${BOARD_NAME}, vrk160)
  BOARD_DTS                          := versal-vrk160-reva
  PS_DOMAIN                          := psv_cortexa72_0
  LOP_FILE                           := lop-a72-imux.dts
endif

DOMAIN_YAML                        := ../src/${BOARD_NAME}/package_zocl/domain.yaml
BUILD_DIR                          := build
PACK_DIR                           := build_vpp

ifeq (${PREBUILD_EMBEDDED}, true)
  XSA_FIXED                          := ../../vivado/build/xsa_platform/${PLATFORM_CUSTOM}_base.xsa
else
  XSA_FIXED                          := ../../vivado/build/xsa_platform/${PLATFORM_CUSTOM}_fixed.xsa
endif
DTS_FILE                           := ${BUILD_DIR}/system-top.dts
PL_DTSI                            := ${BUILD_DIR}/pl.dtsi
DTB_FILE                           := ${BUILD_DIR}/system.dtb
DUMP_DTS_FILE                      := ${BUILD_DIR}/dump.dts
XCLBIN                             := my.xclbin
IP_DIR                             := ./ip
LIBADF                             := $(wildcard ../../vss/${IP_DIR}/aie/*_libadf.a)

##############
# Build Flow #
##############
.PHONY: clean help all

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate everything for this design"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all: ${DTS_FILE}
#all: ${DTB_FILE}

${PACK_DIR}:
	mkdir ${PACK_DIR}

${DTS_FILE}: ${XSA_FIXED}
	sdtgen -xsa ${XSA_FIXED} -dir ${BUILD_DIR} -user_dts ${BOARD_DTS}.dtsi ${USER_DTS} -zocl enable
	export LOPPER_DTC_FLAGS="-b 0 -@" && lopper -f --enhanced -i ${LOP_FILE} ${DTS_FILE} ${DTB_FILE} -- gen_domain_dts ${PS_DOMAIN} linux_dt

#${DTB_FILE}: ${DTS_FILE} ${PACK_DIR}
#ifeq (${SEG_CONF}, true)
#	cd ${PACK_DIR} && \
#		v++ --package --debug --save-temps \
#			--target ${TARGET} \
#			--platform ../${XSA_FIXED} \
#			--package.out_dir pack_out \
#			$(addprefix ../, ${LIBADF}) \
#			-o ${XCLBIN}
#else
#	export LOPPER_DTC_FLAGS="-b 0 -@" && lopper -f --enhanced -i ${LOP_FILE} ${DTS_FILE} ${DTB_FILE} -- gen_domain_dts ${PS_DOMAIN} linux_dt
#endif

.PHONY: dump_dts
dump_dts: ${DTB_FILE}
	dtc -I dtb -O dts -o ${DUMP_DTS_FILE} ${DTB_FILE}

clean:
	rm -rf ${BUILD_DIR}
	rm -rf .Xil

.PHONY: ultraclean
ultraclean:
	make clean
