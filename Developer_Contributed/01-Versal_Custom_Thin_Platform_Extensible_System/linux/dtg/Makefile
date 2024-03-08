#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#


ECHO                                = @echo

BOARD_NAME                         ?= vck190
PLATFORM_CUSTOM                    ?= ${BOARD_NAME}_thin

DTG                                := device-tree-xlnx
DTG_VERSION                        := xlnx_rel_v2024.1

DTC                                := dtc
DTC_PATH                           := $(abspath ${DTC})

DTS                                := dts

XSA_FILE                           := $(wildcard ../../vivado/build/xsa_platform/*.xsa)
DTSI_DIR                           := ./src
DTSI_FILE                          := system-user.dtsi
DTS_FILE                           := ${DTS}/system-top.dts
DTB_FILE                           := ${DTS}/system-top.dtb
DUMP_DTS_FILE                      := ${DTS}/dump.dts

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

all: ${DTG} ${DTB_FILE}

${DTB_FILE}: ${XSA_FILE}
	make build_dts
#	make include_board
	make include_dtsi
#	make build_xsct
	make build_dtb
	make dump_dts

${DTG}:
	git clone https://github.com/Xilinx/${DTG}.git
	cd ${DTG} && git checkout ${DTG_VERSION}

${DTC}:
	git clone https://git.kernel.org/pub/scm/utils/dtc/dtc.git ${DTC}
	cd ${DTC} && make
	export PATH=$PATH:${DTC_PATH}

build_xsct:
	$(ECHO) "dtg/build_xsct: Currently errors out, but should be the prefered method"
	xsct -eval "createdts -hw ${XSA_FILE} -zocl -out . -platform-name ${PLATFORM_CUSTOM} -git-branch ${DTG_VERSION} -dtsi ${DTSI_DIR}/${DTSI_FILE} -compile"
	cp ./${PLATFORM_CUSTOM}/psv_cortexa72_0/device_tree_domain/bsp/system.dtb ${DTB_FILE}

build_dts:
	$(ECHO) "dtg/build_dts: Building the dts from the supplied xsa"
	rm -rf ${DTS}
	xsct -eval "source src/build_dts.tcl; build_dts $(XSA_FILE) ${DTS}"	

include_board:
	$(ECHO) "dtg/include_board: Currently not needed since included by the supplied xsa"
	xsct -eval "source src/build_dts.tcl; include_board ${DTS}"

include_dtsi:
	$(ECHO) "dtg/include_dtsi: Adding custum specific parameters to the dts"
	xsct -eval "source src/build_dts.tcl; include_dtsi ${DTSI_DIR} $(DTSI_FILE) ${DTS}"
	
build_dtb:
	$(ECHO) "dtg/build_dtb: Building the dtb from the generated dts"
	rm -rf ${DTB_FILE}
	gcc -I ${DTS} -E -nostdinc -undef -D__DTS__ -x assembler-with-cpp -o ${DTS_FILE}.tmp ${DTS_FILE}
	dtc -I dts -O dtb -@ -o ${DTB_FILE} ${DTS_FILE}.tmp

dump_dts:
	dtc -I dtb -O dts -o ${DUMP_DTS_FILE} ${DTB_FILE}

clean:
	rm -rf .Xil ${DTS}
	rm -rf ${PLATFORM_CUSTOM}

ultraclean:
	rm -rf ${DTG}
	rm -rf ${DTC}
	make clean

