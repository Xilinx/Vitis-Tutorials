# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
# */
EXTENSIBLE_XSA ?= 
PLATFORM           ?= $(EXTENSIBLE_XSA)
TARGET    = hw
GRAPH     = src/graph.cpp
LIBADF    = libadf.a
FIXED_XSA = design.xsa
HOST_EXE  = main.elf 

FIXED_PLATFORM = fixed_platform/export/fixed_platform/fixed_platform.xpfm

AIE_CMPL_CMD =${VCC} -c --mode aie --target hw --platform=${PLATFORM} --config aie.cfg  --work_dir ./Work ${GRAPH} |& tee compiler.log
AIE_SIM_CMD  = aiesimulator --pkg-dir=./Work 

##########################################################################################################################################################
### DO NOT MODIFY BELOW THIS LINE UNLESS NECESSARY
################################################################################################################################################

XOS       = mm2s_1/mm2s_1.xo s2mm_1/s2mm_1.xo
VCC       = v++
VPP_SPEC  = system.cfg
VPP_FLAGS = --save-temps --verbose --config ${VPP_SPEC} --config /proj/xtcRepo/dsv/HEAD/EVEREST/NOC/IPI/TCs/HEAD/xcvc1902/system_xsim.cfg

.PHONY: clean

all: ${FIXED_XSA} ${HOST_EXE} build 
sd_card: all

aie: ${LIBADF}

${LIBADF}: ${GRAPH}
	${AIE_CMPL_CMD}
	#${AIE_SIM_CMD}

${XOS}: xo
xo:
	${VCC} -c --mode hls --platform $(EXTENSIBLE_XSA) --config mm2s.cfg --work_dir mm2s_1
	${VCC} -c --mode hls --platform $(EXTENSIBLE_XSA) --config s2mm.cfg --work_dir s2mm_1
fixed_xsa: ${FIXED_XSA}
${FIXED_XSA}: ${LIBADF} ${VPP_SPEC} ${XOS}
	${VCC} -g -l --platform ${PLATFORM} ${XOS} ${LIBADF} --vivado.prop fileset.sim_1.xsim.simulate.runtime=400us \
	       -t ${TARGET} ${VPP_FLAGS} -o $@
	

${FIXED_PLATFORM}: ${FIXED_XSA}
	generate-platform.sh -name fixed_platform -hw ${FIXED_XSA} -domain psv_cortexa72_0:standalone
application:${HOST_EXE}
${HOST_EXE}: ${FIXED_PLATFORM} sw/main.cpp
	make -C sw/

build: ${FIXED_XSA} ${LIBADF}
	${VCC} -p -t ${TARGET} -f ${FIXED_PLATFORM} \
	    --package.defer_aie_run \
        --package.boot_mode sd \
        --package.image_format=fat32 \
        --package.out_dir package_${TARGET} \
		--package.ps_elf main.elf,a72-0 $^ 

run: build 
ifeq (${TARGET},hw_emu)
	@echo "INFO: Running Embedded Emulation: ${TARGET}"
	./package_${TARGET}/launch_hw_emu.sh
else
	@echo "INFO: Target is 'HW' and run sd_card.img on HW !!!"	
endif

run_hw: package 
ifeq (${TARGET}, hw)	
	@echo "INFO: Running HW test..."	
	/proj/sdxbf/emhw/bin/zboard -d /dev/ttyUSB1 run-test -i ./package_${TARGET}/sd_card.img -m jtag_dual_linux --sync-output 2>&1 | tee embedded_run.log
else
	@echo "INFO: Target is Not HW.."
endif

clean:
	rm -rf _x v++_* ${XOS} ${LIBADF} *.o.* *.xo.* \
	       *.xclbin* *.xsa *_log *.log Work Map_Report.csv *.bin  s2mm_1 mm2s_1\
	       BOOT* *.xo emu_qemu_scripts *.cdo *.bif compile_log_pre *.elf *.o launch_hw_emu.sh qemu_args.txt *.xsa xnwOut *.pdi fixed_platform v++.package_summary *.link_summary libadf* lpd* NOC* pmc* plm* sd_card* sim ilpProblem* sol.db drivers .Xil .ipcache package_${TARGET}
	make -C sw clean
