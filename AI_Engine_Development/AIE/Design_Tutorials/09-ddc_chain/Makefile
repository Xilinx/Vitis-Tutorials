# #########################################################################
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# #########################################################################
#            Makefile used for ME compilation and simulation
#           Set COMMSLIB_ROOT to the absolute path to commslib
# #########################################################################
VERSAL_VITIS_PLATFORM      = xilinx_vck190_base_202320_1
VITIS_PLATFORM_DIR         = ${PLATFORM_REPO_PATHS}/${VERSAL_VITIS_PLATFORM}
export VITIS_PLATFORM_XPFM = ${VITIS_PLATFORM_DIR}/${VERSAL_VITIS_PLATFORM}.xpfm

# Set SysRoot, RootFS and Image
export VITIS_SYSROOTS   = ${COMMON_IMAGE_VERSAL}/sysroots/cortexa72-cortexa53-xilinx-linux
export SDKTARGETSYSROOT = ${VITIS_SYSROOTS}
export KERNEL_IMAGE     = ${COMMON_IMAGE_VERSAL}/Image
export ROOTFS           = ${COMMON_IMAGE_VERSAL}/rootfs.ext4
export XLNX_VERSAL      = ${COMMON_IMAGE_VERSAL}
export PLATFORM         = ${VITIS_PLATFORM_XPFM}
export SYSROOT          = ${SDKTARGETSYSROOT}

LOCAL_INC          := src
LOCAL_SRC          := src/kernels
LOCAL_LIB          := src/inc
TEMP_FILE          := t.txt
TEST_BENCH         = src/ddc_chain.cpp
LOG_FILE           = log
OTHER_OPTS         = --aie.pl-freq=400 --aie.Xchess="-std=c++17" --aie.xlopt=0
ALL_INCLUDE       := --include=${PWD} --include=$(LOCAL_INC) --include=$(LOCAL_SRC) --include=$(LOCAL_LIB)
AIE_FLAGS         := --aie.verbose ${ALL_INCLUDE} ${TEST_BENCH} ${OTHER_OPTS} --platform=${PLATFORM}

.PHONY: clean help compile x86compile sim x86sim get_tp check_op

help::
	@echo "Makefile Usage:"
	@echo "  make all"
	@echo "      Command to compile and simulate the design with --target=hw"
	@echo ""
	@echo "  make x86all"
	@echo "      Command to compile and simulate the design with --target=x86sim"
	@echo ""
	@echo "  make gen_vectors"
	@echo "      Generates IO data needed to simulate the design"
	@echo ""
	@echo "  make clean"
	@echo "      Command to remove all the generated files."

all             : clean compile sim check_op get_tp

x86all          : clean x86compile x86sim check_op

gen_vectors:
	cd m && matlab -nodisplay -nojvm -r "batch; quit"

compile:
	v++ --compile --target=hw --mode aie ${AIE_FLAGS} 2>&1 | tee ${LOG_FILE}

x86compile:
	v++ --compile --target=x86sim --mode aie ${AIE_FLAGS} 2>&1 | tee ${LOG_FILE}

sim:
	aiesimulator -i=data0 -o=simulator_output/data0 2>&1 | tee -a ${LOG_FILE}
	aiesimulator -i=data1 -o=simulator_output/data1 2>&1 | tee -a ${LOG_FILE}

x86sim:
	x86simulator -i=data0 -o=simulator_output/data0 2>&1 | tee -a ${LOG_FILE}
	x86simulator -i=data1 -o=simulator_output/data1 2>&1 | tee -a ${LOG_FILE}

get_tp:
	@for z in 0 1 ; do \
	   for q in 0 1 2 3; do \
	      for p in 0 1 2 3 4 5 6 7; do \
			grep -ve '[XT]' simulator_output/data$${z}/ddc_out$${q}$${p}.txt > tmp;\
			x=`wc -l tmp | awk '{print $$1}'`;\
			grep 'T ' simulator_output/data$${z}/ddc_out$${q}$${p}.txt > tmp;\
			t0=`sed -ne '1p;$$p' tmp | awk '(NR==1){if($$3=="ns") print $$2; else if($$3=="us") print $$2*1000; else if ($$3=="ps") print $$2/1000}'`;\
			r=`sed -ne '1p;$$p' tmp | awk '(NR==2){if($$3=="ns") print $$2; else if($$3=="us") print $$2*1000; else if ($$3=="ps") print $$2/1000}' | awk -v a=$$x  -v b0=$$t0 '{print a/($$1-b0)*1000}'`;\
			echo "CFG" $$z "DDC" $$q$$p  "Throughput=" $$r "Msps (>153.6)"; \
		  done; \
	   done; \
	done;

check_op:
	@for z in 0 1 ; do \
	  for q in 0 1 2 3; do \
	    for p in 0 1 2 3 4 5 6 7; do \
			grep -ve '[XT]' simulator_output/data$${z}/ddc_out$${q}$${p}.txt > $(TEMP_FILE);\
			diff -w $(TEMP_FILE) data$${z}/ddc_golden$${q}$${p}.txt > diff_output;\
			r0=`wc diff_output | awk '{print "diff=" $$1 }'`;\
			r1=`head -1 diff_output`;\
			echo "CFG" $$z " DDC" $$q$$p "-" $$r0 "-" $$r1;\
		done; \
	   done; \
	done;

clean:
	@rm -rf Work*
	@rm -rf simulator_output diff_output
	@rm -rf $(TEMP_FILE) log* $(LOG_FILE)
	@rm -rf *.log *.jou *.debug *.jstack *.str *.db *tmp Map_Report.csv libadf.a .Xil .AIE_SIM_CMD* .crashReporter* pl_sample_counts
