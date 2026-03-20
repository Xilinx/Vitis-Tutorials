#
# Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani

QEMU_PREBUILT          := amd-cortexa78-mali-common_vek385_qemu_prebuilt
export QEMU_COMBINED   := base_camp_images
export WIC_PARTITION   := edf-linux-disk-image-amd-cortexa78-mali-common.rootfs.wic.ufs:2

export DUT       = dut
export XCLBIN    = ${DUT}.xclbin

TARGET                ?= hw
export IP_DIR         := $(abspath ../aie/radioml_top)
GRAPH_O               := ${IP_DIR}/libadf.a
export XSA            := my.xsa
export STIMULUS_DIR   := ${IP_DIR}/data

export APP_EXE        := ../ps_apps/host.exe

export SD_CARD        := $(abspath ../package.$(TARGET)/sd_card)
export BUILD_DIR      := build_${TARGET}

export EMBD_EXC       := embedded_exec.sh
SYSTEM_CFG            := system.cfg

.PHONY: help all clean

help::
	@echo "Makefile Usage:"
	@echo "  make all"
	@echo "      Command to generate the sd_card in this design, including platform/petalinux/ip/ps_apps"
	@echo ""
	@echo "  make clean"
	@echo "      Command to remove all the generated files."

ifeq (${TARGET},hw)
  all:	${SD_CARD}
else
  all:	${SD_CARD} qemu_combined copy_wic run_emu
endif

${BUILD_DIR}:
	mkdir ${BUILD_DIR}

${BUILD_DIR}/${XSA}: ${GRAPH_O} ${SYSTEM_CFG} ${BUILD_DIR}
	cd ${BUILD_DIR} && \
		v++ --link --debug --save-temps \
			--target ${TARGET} \
			--platform ${PLATFORM} \
			$(addprefix --config ../, ${SYSTEM_CFG}) \
			${GRAPH_O} \
			--output ${XSA} \
			2>&1 | tee vpp_link.log

${EMBD_EXC}:
	@echo "#!/bin/bash"                                                                        >> ${EMBD_EXC}
	@echo "#"                                                                                  >> ${EMBD_EXC}
	@echo "# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved."            >> ${EMBD_EXC}
	@echo "# SPDX-License-Identifier: MIT"                                                     >> ${EMBD_EXC}
	@echo "#"                                                                                  >> ${EMBD_EXC}
	@echo ""                                                                                   >> ${EMBD_EXC}
ifeq ("$(TARGET)","hw_emu")
	@echo "export XCL_EMULATION_MODE=hw_emu"                                                   >> ${EMBD_EXC}
endif
	@echo 'echo "INFO: Load the pdi and dtbo using fpgautil"'                                  >> ${EMBD_EXC}
	@echo "fpgautil -b ${DUT}.pdi -o ${DUT}.dtbo"                                              >> ${EMBD_EXC}
	@echo ""                                                                                   >> ${EMBD_EXC}
	@echo "./host.exe ${DUT}.xclbin"                                                           >> ${EMBD_EXC}
	@echo 'return_code=$$?'                                                                    >> ${EMBD_EXC}
	@echo 'if [ $$return_code -ne 0 ]; then'                                                   >> ${EMBD_EXC}
	@echo '	echo "ERROR: Embedded host run failed, RC=$$return_code"'                          >> ${EMBD_EXC}
	@echo 'fi'                                                                                 >> ${EMBD_EXC}
	@echo echo '"INFO: Embedded host run completed."'                                          >> ${EMBD_EXC}
	chmod 755 ${EMBD_EXC}

${SD_CARD}: ${BUILD_DIR}/${XSA} ${APP_EXE} ${EMBD_EXC}
	cd ${BUILD_DIR} && v++ -p -s -t ${TARGET} -f ${XSA} --package.out_dir ${SD_CARD} --package.defer_aie_run ${GRAPH_O} -o ${XCLBIN} 2>&1 | tee vpp_pack.log
ifeq (${TARGET},hw)
	cp -rf ${BUILD_DIR}/${DUT}.xclbin ${SD_CARD}/.
	cp -rf ${STIMULUS_DIR}/* ${SD_CARD}/.
	cp -rf ${EMBD_EXC} ${SD_CARD}/.
	cp -rf ${APP_EXE} ${SD_CARD}/.
endif

qemu_combined:
	rm -rf ${QEMU_COMBINED}; \
	mkdir -p ${QEMU_COMBINED}; \
	cp -prf ${YOCTO_ARTIFACTS}/${QEMU_PREBUILT}/* ${QEMU_COMBINED}/.; \

copy_wic:
	./run_copy_wic.sh

run_emu:
	cd $(SD_CARD) && ./launch_hw_emu.sh -qemu-config ../vitis/${QEMU_COMBINED}/combined.qemuboot.conf -login "amd-edf" -password "amd-edf" -run-app " mount /dev/sda2 /media; cd /media; ./${EMBD_EXC}"

clean:
	/bin/rm -rf ${SD_CARD} ${BUILD_DIR} *.o ${QEMU_COMBINED} .Xil ${EMBD_EXC} .ipcache

