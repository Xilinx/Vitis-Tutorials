#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani

DO_COSIM        := false
ECHO            := @echo

PART            :=xcve2802-vsvh1760-2MP-e-S

SSR             :=8
POINT_SIZE      :=4096
KERNEL          := ifft_transpose_wrapper
KERNEL_SRC      := ${DSPLIB_ROOT}/L1/src/hw/ifft_transpose/ifft_transpose.cpp
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
ifeq (${DO_COSIM},true)
	vitis-run --mode hls --config ${HLS_CFG} --csim --work_dir ${KERNEL}
	vitis-run --mode hls --config ${HLS_CFG} --cosim --work_dir ${KERNEL}
	vitis-run --mode hls --config ${HLS_CFG} --impl --work_dir ${KERNEL}
endif

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
	@echo "syn.file_cflags=${KERNEL_SRC}, -DSSR=${SSR} -DPOINT_SIZE=${POINT_SIZE}"                                          >> ${HLS_CFG}
	@echo "syn.file_csimflags=${KERNEL_SRC}"                                                                                >> ${HLS_CFG}
	@echo "# Testbench:"                                                                                                    >> ${HLS_CFG}
	@echo "tb.file=tb_wrapper.cpp"                                                                                          >> ${HLS_CFG}
	@echo "tb.file=data/stream0_i.txt"                                                                                      >> ${HLS_CFG}
	@echo "tb.file=data/stream1_i.txt"                                                                                      >> ${HLS_CFG}
	@echo "tb.file=data/stream2_i.txt"                                                                                      >> ${HLS_CFG}
	@echo "tb.file=data/stream3_i.txt"                                                                                      >> ${HLS_CFG}
	@echo "tb.file=data/stream4_i.txt"                                                                                      >> ${HLS_CFG}
	@echo "tb.file=data/stream5_i.txt"                                                                                      >> ${HLS_CFG}
	@echo "tb.file=data/stream6_i.txt"                                                                                      >> ${HLS_CFG}
	@echo "tb.file=data/stream7_i.txt"                                                                                      >> ${HLS_CFG}
	@echo "tb.file=data/stream0_o.txt"                                                                                      >> ${HLS_CFG}
	@echo "tb.file=data/stream1_o.txt"                                                                                      >> ${HLS_CFG}
	@echo "tb.file=data/stream2_o.txt"                                                                                      >> ${HLS_CFG}
	@echo "tb.file=data/stream3_o.txt"                                                                                      >> ${HLS_CFG}
	@echo "tb.file=data/stream4_o.txt"                                                                                      >> ${HLS_CFG}
	@echo "tb.file=data/stream5_o.txt"                                                                                      >> ${HLS_CFG}
	@echo "tb.file=data/stream6_o.txt"                                                                                      >> ${HLS_CFG}
	@echo "tb.file=data/stream7_o.txt"                                                                                      >> ${HLS_CFG}
	@echo "tb.file_cflags=tb_wrapper.cpp,-I${DSPLIB_ROOT}/L1/src/hw/ifft_transpose -I${DSPLIB_ROOT}/L1/include/vss/common -I${DSPLIB_ROOT}/L1/include/vss/vss_fft_ifft_1d -DSSR=${SSR} -DPOINT_SIZE=${POINT_SIZE}" >> ${HLS_CFG}
	@echo "# C-Simulation:"                                                                                                 >> ${HLS_CFG}
	@echo "csim.clean=true"                                                                                                 >> ${HLS_CFG}
	@echo "csim.setup=false"                                                                                                >> ${HLS_CFG}
	@echo "# C/RTL Co-Simulation:"                                                                                          >> ${HLS_CFG}
	@echo "cosim.setup=false"                                                                                               >> ${HLS_CFG}
	@echo "# Package:"                                                                                                      >> ${HLS_CFG}
	@echo "package.output.file=${KERNEL}.xo"                                                                                >> ${HLS_CFG}
	@echo "package.output.format=xo"                                                                                        >> ${HLS_CFG}
	@echo "# OOC Place & Route:"                                                                                            >> ${HLS_CFG}
	@echo "vivado.flow=impl"                                                                                                >> ${HLS_CFG}

clean:
	rm -f $(XO)* ${HLS_CFG} *.log vitis-comp.json
	rm -rf .Xil _x/ .crashReporter*
	rm -rf ${KERNEL}

