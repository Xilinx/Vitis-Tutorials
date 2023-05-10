#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#


TARGET   = hw_emu
HOST_EXE = host.exe

PFM_NAME := xilinx_vck190_base_202310_1
PFM_NAME := $(strip $(PFM_NAME))
PLATFORM := ${PLATFORM_REPO_PATHS}/${PFM_NAME}/${PFM_NAME}.xpfm

AIEDIR = aie
SRCDIR = aie/src

GRAPH    = $(SRCDIR)/graph.cpp
LIBADF  = $(AIEDIR)/libadf.a
WORKDIR = $(AIEDIR)/Work

AIE_CMPL_CMD = aiecompiler -include=$(SRCDIR) -include=$(SRCDIR)/aie_kernels -workdir=$(WORKDIR) ${GRAPH} --platform=$(PLATFORM) --output-archive=$(LIBADF)
X86_CMPL_CMD = aiecompiler --target=x86sim -include=$(SRCDIR) -include=$(SRCDIR)/aie_kernels -workdir=$(WORKDIR) ${GRAPH} --platform=$(PLATFORM)  --output-archive=$(LIBADF)

AIE_SIM_CMD = aiesimulator --pkg-dir=$(WORKDIR) --dump-vcd $(AIEDIR)/foo --output-dir=$(AIEDIR)/aiesimulator_output
X86_SIM_CMD = x86simulator --pkg-dir=$(WORKDIR) --output-dir=$(AIEDIR)/x86simulator_output

allaie: clean aie aiesim aiecmp aieviz

allx86: clean x86 x86sim

run: clean aie aiesim aiecmp

run_nocmp: clean aie aiesim

aie: ${LIBADF}
${LIBADF}: $(SRCDIR)/*.cpp  $(SRCDIR)/*.h  $(SRCDIR)/aie_kernels/*.cpp
	${AIE_CMPL_CMD}

x86:
	${X86_CMPL_CMD}

aiesim: ${LIBADF}
	${AIE_SIM_CMD}

x86sim:
	${X86_SIM_CMD}

aiecmp:
	grep -v T aie/aiesimulator_output/float_output.txt > aie/aiesimulator_output/NoStamps_float_output.txt
	grep -v T aie/aiesimulator_output/cfloat_output.txt > aie/aiesimulator_output/NoStamps_cfloat_output.txt
	grep -v T aie/aiesimulator_output/cfloat_conf_output.txt > aie/aiesimulator_output/NoStamps_cfloat_conf_output.txt
	grep -v T aie/aiesimulator_output/cfloat_conj_output.txt > aie/aiesimulator_output/NoStamps_cfloat_conj_output.txt
	CompareOutput.py aie/data/ref_outputc_float.txt aie/aiesimulator_output/NoStamps_float_output.txt
	CompareOutput.py aie/data/ref_outputc_cfloat.txt aie/aiesimulator_output/NoStamps_cfloat_output.txt
	CompareOutput.py aie/data/ref_outputc_cfloat.txt aie/aiesimulator_output/NoStamps_cfloat_conf_output.txt
	CompareOutput.py aie/data/ref_outputcc_cfloat.txt aie/aiesimulator_output/NoStamps_cfloat_conj_output.txt

x86cmp:
	CompareOutput.py aie/data/ref_outputc_float.txt aie/x86simulator_output/float_output.txt
	CompareOutput.py aie/data/ref_outputc_cfloat.txt aie/x86simulator_output/cfloat_output.txt
	CompareOutput.py aie/data/ref_outputc_cfloat.txt aie/x86simulator_output/cfloat_conf_output.txt
	CompareOutput.py aie/data/ref_outputcc_cfloat.txt aie/x86simulator_output/cfloat_conj_output.txt

aieviz:
	vitis_analyzer vitis_analyzer aie/aiesimulator_output/default.aierun_summary

clean: aieclean

aieclean:
	rm -rf *.log *.csv *.db $(LIBADF) $(WORKDIR) $(AIEDIR)/aiesimulator_output $(AIEDIR)/x86simulator_output
	rm -rf pl_sample_counts $(AIEDIR)/foo.vcd logs




otherclean:
	rm -rf _x ${LIBADF} $(WORKDIR) .Xil vitis_analyzer* foo.vcd *.log pl_sample*
	rm -rf  vitis_design_wrapper*
	rm -rf .AIE_SIM_CMD_LINE_OPTIONS *.csv *.db temp *.txt
