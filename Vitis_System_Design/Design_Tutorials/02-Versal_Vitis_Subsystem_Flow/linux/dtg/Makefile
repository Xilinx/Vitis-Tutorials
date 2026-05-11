#
# Copyright (C) 2021-2022, Xilinx, Inc. All rights reserved.
# Copyright (C) 2022-2025, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#


ECHO                                = @echo

BOARD_NAME                         ?= vck190
PLATFORM_CUSTOM                    ?= ${BOARD_NAME}_thin

DTG                                := device-tree-xlnx
#DTG_VERSION                        := xlnx_rel_v2025.2
DTG_VERSION                        := master

DTC                                := dtc
DTC_PATH                           := $(abspath ${DTC})

DTS                                := dts

XSA_FILE                           := ../../vivado/build/xsa_platform/${PLATFORM_CUSTOM}_fixed.xsa
#XSA_FILE                           := $(wildcard ../../vivado/build/xsa_platform/*.xsa)
DTSI_DIR                           := ./src
DTSI_FILE                          := system-user.dtsi
DTS_FILE                           := ${DTS}/system-top.dts
DTB_FILE                           := ${DTS}/system-top.dtb
DUMP_DTS_FILE                      := ${DTS}/dump.dts

SEG_FLOW						   ?= 0

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
	make build_dtb
	make dump_dts

${DTG}:
	git clone https://gitenterprise.xilinx.com/Linux/${DTG}.git
	cd ${DTG} && git checkout ${DTG_VERSION}

#	git clone https://github.com/Xilinx/${DTG}.git
https://gitenterprise.xilinx.com/Linux/device-tree-xlnx
${DTC}:
	git clone https://git.kernel.org/pub/scm/utils/dtc/dtc.git ${DTC}
	cd ${DTC} && make
	export PATH=$PATH:${DTC_PATH}

build_sdt:
	$(ECHO) "dtg/build_sdt: Generating the system device tree with zocl enabled"
	rm -rf sdt_out
	xsdb -eval "source src/xsct_utils.tcl; run_sdtgen -version 2025.2 -xsa $(XSA_FILE) -sdt_dt_zocl 1"

lop_procs:
	$(ECHO) "dtg/lop_procs: Extract the procs from SDT using Lopper"
	if [ ! -d "./lopper" ];then \
		git clone https://github.com/devicetree-org/lopper; \
		cd lopper && git checkout v0.2024.x; \
	fi
	source ./src/lopper_settings.sh; \
	export LOPPER_DTC_FLAGS="-b 0 -@"; \
	lopper -f -i ./lopper/lopper/lops/lop-cpulist.dts ./sdt_out/system-top.dts

lop_dtb:
	$(ECHO) "dtg/lop_dtb: Lopping off the dtb from SDT"
	if [ ! -d "./lopper" ];then \
		git clone https://github.com/devicetree-org/lopper; \
		cd lopper && git checkout v0.2024.x; \
	fi
	source ./src/lopper_settings.sh; \
	export LOPPER_DTC_FLAGS="-b 0 -@"; \
	if [ $(SEG_FLOW) == 0 ]; then \
		lopper -f --enhanced -i ./lopper/lopper/lops/lop-a72-imux.dts ./sdt_out/system-top.dts system.dtb -- gen_domain_dts psv_cortexa72_0 linux_dt; \
	else \
		lopper -f --enhanced ./system-top.dts ./sdt_out/system.dts  -- xlnx_overlay_dt cortexa72-versal full; \
		lopper -f --enhanced -O . -i ./lopper/lopper/lops/lop-a72-imux.dts ./sdt_out/system.dts ./system.dtb -- gen_domain_dts psv_cortexa72_0 linux_dt; \
		dtc -I dts -O dtb -o ./pl.dtbo pl.dtsi; \
	fi
	
build_dts:
	$(ECHO) "dtg/build_dts: Building the dts from the supplied xsa"
	rm -rf ${DTS}
	xsdb -eval "source src/build_dts.tcl; build_dts $(XSA_FILE) ${DTS}"	

include_board:
	$(ECHO) "dtg/include_board: Currently not needed since included by the supplied xsa"
	xsdb -eval "source src/build_dts.tcl; include_board ${DTS}"

include_dtsi:
	$(ECHO) "dtg/include_dtsi: Adding custum specific parameters to the dts"
	xsdb -eval "source src/build_dts.tcl; include_dtsi ${DTSI_DIR} $(DTSI_FILE) ${DTS}"
	
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

