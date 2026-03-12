#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani

ECHO                  := @echo

TARGET                ?= hw

IP_DIR                := ../aie/channelizer
DATA_DIR              := ../aie/channelizer/data
XO_VSS                := ../hls/packet_sender/packet_sender_wrapper.xo \
                         ../hls/packet_receiver/packet_receiver_wrapper.xo \
                         ../hls/ifft_mid_splitter/splitter.xo \
                         ../hls/ifft_mid_transpose/mid_transpose_wrapper.xo \
                         ../hls/ifft_mid_joiner/joiner.xo \
                         ../hls/merge_8x4/merge_8x4_wrapper.xo

XO                    := ../hls/channelizer_dma_src/channelizer_dma_src_wrapper.xo \
                         ../hls/channelizer_dma_snk/channelizer_dma_snk_wrapper.xo

GRAPH_O               := ${IP_DIR}/libadf.a
XSA                   := my.xsa
STIMULUS_IO           := ${DATA_DIR}/sig_o.txt ${DATA_DIR}/sig_i.txt
PS_EXE                := ../ps_apps/host.exe

VSS                   := channelizer
VSSFILE               := ${VSS}/${VSS}.vss
VSSCFG                := vss.cfg
PART                  := xcve2802-vsvh1760-2MP-e-S

SD_CARD               := $(abspath ../package)
BUILD_DIR             := build_${TARGET}

EMBD_EXC              := embedded_exec.sh
SYSTEM_CFG            := system.cfg

.PHONY: clean all

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate the sd_card in this design, including platform/petalinux/ip/ps_apps"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all: ${SD_CARD}

${BUILD_DIR}:
	mkdir ${BUILD_DIR}

${BUILD_DIR}/${VSS}: ${BUILD_DIR} ${GRAPH_O} ${XO_VSS} ${VSSCFG}
	v++ --link --mode vss --part ${PART} --save-temps --out_dir ${VSS} --config ${VSSCFG} ${GRAPH_O} ${XO_VSS}

${BUILD_DIR}/${XSA}: ${BUILD_DIR}/${VSS} ${XO} ${SYSTEM_CFG}
	cd ${BUILD_DIR} && \
		v++ --link --debug --save-temps \
			--target ${TARGET} \
			--platform ${PLATFORM} \
			$(addprefix --config ../, ${SYSTEM_CFG}) \
			$(addprefix ../, ${XO}) \
			$(addprefix ../, ${VSSFILE}) \
			--output ${XSA} \
			2>&1 | tee vpp_link.log

${EMBD_EXC}:
	@echo "#!/bin/bash"                                                                        >> ${EMBD_EXC}
	@echo "#"                                                                                  >> ${EMBD_EXC}
	@echo "# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved."            >> ${EMBD_EXC}
	@echo "# SPDX-License-Identifier: MIT"                                                     >> ${EMBD_EXC}
	@echo "#"                                                                                  >> ${EMBD_EXC}
	@echo ""                                                                                   >> ${EMBD_EXC}
ifeq ("$(TARGET)","hw_emu")
	@echo "export XCL_EMULATION_MODE=hw_emu"                                                   >> ${EMBD_EXC}
endif
	@echo ""                                                                                   >> ${EMBD_EXC}
	@echo "./host.exe a.xclbin"                                                                >> ${EMBD_EXC}
	@echo 'return_code=$$?'                                                                    >> ${EMBD_EXC}
	@echo 'if [ $$return_code -ne 0 ]; then'                                                   >> ${EMBD_EXC}
	@echo '	echo "ERROR: Embedded host run failed, RC=$$return_code"'                          >> ${EMBD_EXC}
	@echo 'fi'                                                                                 >> ${EMBD_EXC}
	@echo echo '"INFO: Embedded host run completed."'                                          >> ${EMBD_EXC}
	@chmod 755 ${EMBD_EXC}

${SD_CARD}: ${BUILD_DIR}/${XSA} ${PS_EXE} ${KERNEL_IMAGE} ${ROOTFS} ${STIMULUS_IO} ${EMBD_EXC}
	cd ${BUILD_DIR} && \
		v++ --package --debug --save-temps \
			--target ${TARGET} \
			--platform ${PLATFORM} \
			--package.out_dir ${SD_CARD} \
			--package.boot_mode=sd \
			--package.image_format=ext4 \
			--package.kernel_image=${KERNEL_IMAGE} \
			--package.rootfs=${ROOTFS}  \
			--package.defer_aie_run \
			../${GRAPH_O} \
			${XSA} \
			$(addprefix --package.sd_file=../, ${PS_EXE}) \
			$(addprefix --package.sd_file=../,${STIMULUS_IO}) \
			$(addprefix --package.sd_file=../,${EMBD_EXC}) \
			2>&1 | tee vpp_pack.log
	echo ${PLATFORM_CUSTOM} > ${SD_CARD}/sd_card/platform_desc.txt

clean:
	rm -rf ${SD_CARD} ${BUILD_DIR} _x *.xo ${VSS} xcd.log v++* .Xil .ipcache analyzer_input ${EMBD_EXC}