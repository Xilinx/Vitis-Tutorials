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
# Top-level Makefile -- Calls sub-makefiles

ECHO                               = @echo

################################################################################
# Variables needed for AMD/XILINX tools version checking, PLEASE DO NOT MODIFY #
################################################################################
DTG                                := device-tree-xlnx
DTG_VERSION                        := xlnx_rel_v2022.2

DTC                                := dtc
DTC_PATH                           := $(abspath ${DTC})

DTS                                := dts

XSA_FILE                           := $(wildcard ../../platform/hw/build/*.xsa)
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
	xsct -eval "createdts -hw ${XSA_FILE} -zocl -out . -platform-name vck190_thin -git-branch xlnx_rel_v2022.2 -dtsi ${DTSI_FILE} -compile"
	cp ./vck190_thin/psv_cortexa72_0/device_tree_domain/bsp/system.dtb ${DTB_FILE}

build_dts:
	rm -rf ${DTS}
	xsct -eval "source src/build_dts.tcl; build_dts $(XSA_FILE) ${DTS}"	

include_board:
	xsct -eval "source src/build_dts.tcl; include_board ${DTS}"

include_dtsi:
	xsct -eval "source src/build_dts.tcl; include_dtsi ${DTSI_DIR} $(DTSI_FILE) ${DTS}"
	
build_dtb:
	rm -rf ${DTB_FILE}
	gcc -I ${DTS} -E -nostdinc -undef -D__DTS__ -x assembler-with-cpp -o ${DTS_FILE}.tmp ${DTS_FILE}
	dtc -I dts -O dtb -@ -o ${DTB_FILE} ${DTS_FILE}.tmp

dump_dts:
	dtc -I dtb -O dts -o ${DUMP_DTS_FILE} ${DTB_FILE}

clean:
	rm -rf .Xil ${DTS}
	rm -rf vck190_thin

ultraclean:
	rm -rf ${DTG}
	rm -rf ${DTC}
	make clean

