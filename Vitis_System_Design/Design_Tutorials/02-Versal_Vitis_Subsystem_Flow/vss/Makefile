#
# Copyright (C) 2023-2025, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

ECHO                  := @echo

BOARD_NAME            ?= vck190
PART                  ?= xcvc1902-vsva2197-2MP-e-S
PLATFORM_CUSTOM       ?= ${BOARD_NAME}_thin
LINUX_PRE_BUILDS      ?= false


IP_DIR                := ./ip
XO                    := $(wildcard ${IP_DIR}/xo_hw/*.xo)
LIBADF                := $(wildcard ${IP_DIR}/aie/*_libadf.a)
AIE_WORK              := $(abspath $(wildcard ${IP_DIR}/aie/*_app_ws)/Work)

VSS                   ?= vss_top
VSS_CFG               := src/vss_conn.cfg
BUILD_DIR             := build



.PHONY: clean all

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate the sd_card in this design, including platform/petalinux/ip/ps_apps"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

${BUILD_DIR}:
	mkdir ${BUILD_DIR}

vss_ip:
	$(MAKE) all -C ${IP_DIR} PART=${PART} 

#vss_link: vitis_ip ${BUILD_DIR} ${BUILD_DIR}/${VSS}
vss_link: ${BUILD_DIR} ${BUILD_DIR}/${VSS}

${BUILD_DIR}/${VSS}: ${VSS_CFG}
	cd ${BUILD_DIR} && \
		v++ --link --mode vss --save-temps \
			--part ${PART} \
			$(addprefix --config ../, ${VSS_CFG}) \
			$(addprefix ../, ${XO}) \
			$(addprefix ../, ${LIBADF}) \
			--out_dir ${VSS} \
			|& tee vss_link.log

crts:
	$(MAKE) crts -C cosim AIE_WORK=$(AIE_WORK)

vss_cosim:  ${BUILD_DIR} ${BUILD_DIR}/${VSS}
	$(MAKE) cosim_vss -C cosim AIE_WORK=$(AIE_WORK)

clean:
	$(MAKE) clean -C ip
	rm -rf ${BUILD_DIR}

