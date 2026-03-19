#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Chandra Sekar Balakrishnan

ECHO					:= @echo

QEMU_PREBUILT			:= amd-cortexa78-mali-common_vek385_qemu_prebuilt
export QEMU_COMBINED	:= base_camp_images
export WIC_PARTITION	:= edf-linux-disk-image-amd-cortexa78-mali-common.rootfs.wic.ufs:2

export DUT				= dut
export XCLBIN			= ${DUT}.xclbin

TARGET					?= hw

export TOP_DIR			:= $(abspath ../)
IP_DIR					:= ${TOP_DIR}/aie_ml
export DATA_DIR			:= ${TOP_DIR}/aie_ml


GRAPH_O					:= ${IP_DIR}/libadf.a
XSA						:= my.xsa
# STIMULUS_IO          	:= ${DATA_DIR}/input_sig_i.h ${DATA_DIR}/del_i_optimized.txt ${DATA_DIR}/sig_o.txt
#export XRT_INI			:= ${TOP_DIR}/vitis/xrt.ini
export APP_EXE			:= ${TOP_DIR}/ps_apps/hw_emu/host.exe

export SD_CARD			:= $(abspath ../package)
export BUILD_DIR		:= build_${TARGET}

export EMBD_EXC       := embedded_exec.sh
SYSTEM_CFG				:= system.cfg

.PHONY: clean all

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate the sd_card in this design, including platform/petalinux/ip/ps_apps"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

ifeq (${TARGET},hw)
  all:	${SD_CARD}
else
  all:	${SD_CARD} qemu_combined copy_wic run_emu
endif

${BUILD_DIR}:
	mkdir ${BUILD_DIR}

${BUILD_DIR}/${XSA}: ${BUILD_DIR} ${GRAPH_O} ${SYSTEM_CFG}
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
	@echo "export XILINX_XRT=/usr"                                                             >> ${EMBD_EXC}
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
    
${SD_CARD}: ${BUILD_DIR}/${XSA} ${APP_EXE} ${EMBD_EXC}
	cd ${BUILD_DIR} && v++ -p -s -t ${TARGET} -f ${XSA} --package.out_dir ${SD_CARD} --package.defer_aie_run ${GRAPH_O} -o ${XCLBIN} 2>&1 | tee vpp_pack.log

ifeq (${TARGET},hw)
	cp -rf ${BUILD_DIR}/${DUT}.xclbin ${SD_CARD}/.
#	cp -rf ${XRT_INI} ${SD_CARD}/.
	cp -rf ${EMBD_EXC} ${SD_CARD}/.
	cp -rf ${APP_EXE} ${SD_CARD}/.
endif

qemu_combined:
	rm -rf ${QEMU_COMBINED}; \
	mkdir -p ${QEMU_COMBINED}; \
	cp -prf ${YOCTO_ARTIFACTS}/${QEMU_PREBUILT}/* ${QEMU_COMBINED}/.

copy_wic:
	cd ${TOP_DIR}/vitis && ./run_copy_wic.sh

run_emu:
	cd ../package && ./launch_hw_emu.sh -qemu-config ../vitis/${QEMU_COMBINED}/combined.qemuboot.conf -login "amd-edf" -password "amd-edf" -run-app " mount /dev/sda2 /media; cd /media; ./embedded_exec.sh"

clean:
	rm -rf ${SD_CARD} ${BUILD_DIR} _x ${QEMU_COMBINED} *o v++* embedded_run.log xcd.log analyzer_input ${EMBD_EXC} .ipcache
