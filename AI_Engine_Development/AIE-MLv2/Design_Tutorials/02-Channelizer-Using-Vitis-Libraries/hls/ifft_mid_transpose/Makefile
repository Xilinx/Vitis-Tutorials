#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani

ECHO            := @echo

PART            :=xcve2802-vsvh1760-2MP-e-S

DATAWIDTH       := 64
SSR             := 16
POINT_SIZE      := 4096
DATA_TYPE       := cint32
TWIDDLE_TYPE    := cint16
VSS_MODE        := 1

KERNEL          := mid_transpose_wrapper
KERNEL_SRC      := ${DSPLIB_ROOT}/L1/src/hw/mid_transpose/mid_transpose.cpp
XO              := ${KERNEL}.xo
HLS_CFG         := hls.cfg

.PHONY: clean all help

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate the kernel used in this design"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all: ${XO}

${XO}: ${HLS_CFG} ${KERNEL_SRC}
	v++ --config ${HLS_CFG} --compile --mode hls --work_dir ${KERNEL}

${HLS_CFG}:
	@echo "#"                                                                                                               >> ${HLS_CFG}
	@echo "# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved."                                         >> ${HLS_CFG}
	@echo "# SPDX-License-Identifier: MIT"                                                                                  >> ${HLS_CFG}
	@echo "#"                                                                                                               >> ${HLS_CFG}
	@echo "part=${PART}"                                                                                                    >> ${HLS_CFG}
	@echo "[hls]"                                                                                                           >> ${HLS_CFG}
	@echo "flow_target=vitis"                                                                                               >> ${HLS_CFG}
	@echo "clock=3.2ns"                                                                                                     >> ${HLS_CFG}
	@echo "# C-Synthesis:"                                                                                                  >> ${HLS_CFG}
	@echo "syn.top=${KERNEL}"                                                                                               >> ${HLS_CFG}
	@echo "syn.cflags= -I${DSPLIB_ROOT}/L1/include/vss/common -I${DSPLIB_ROOT}/L1/include/vss/vss_fft_ifft_1d"              >> ${HLS_CFG}
	@echo "syn.file=${KERNEL_SRC}"                                                                                          >> ${HLS_CFG}
	@echo "syn.file_cflags=${KERNEL_SRC}, -DSSR=${SSR} -DPOINT_SIZE=${POINT_SIZE} -DDATAWIDTH=${DATAWIDTH} -DDATA_TYPE=${DATA_TYPE} -DTWIDDLE_TYPE=${TWIDDLE_TYPE} -DVSS_MODE=${VSS_MODE}"                 >> ${HLS_CFG}
	@echo "# Package:"                                                                                                      >> ${HLS_CFG}
	@echo "package.output.file=${KERNEL}.xo"                                                                                >> ${HLS_CFG}
	@echo "package.output.format=xo"                                                                                        >> ${HLS_CFG}
	@echo "# OOC Place & Route:"                                                                                            >> ${HLS_CFG}
	@echo "vivado.flow=impl"                                                                                                >> ${HLS_CFG}

clean:
	rm -f $(XO)* ${HLS_CFG} *.log vitis-comp.json
	rm -rf .Xil _x/ .crashReporter*
	rm -rf ${KERNEL}

