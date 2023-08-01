# /*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
# */
EXTENSIBLE_XSA ?= 
PLATFORM           ?= $(EXTENSIBLE_XSA)
TARGET    = hw

FIXED_XSA = design.xsa
HOST_EXE  = main.elf 

FIXED_PLATFORM = fixed_platform/export/fixed_platform/fixed_platform.xpfm


##########################################################################################################################################################
### DO NOT MODIFY BELOW THIS LINE UNLESS NECESSARY
################################################################################################################################################

XOS       = mm2s_1.xo s2mm_1.xo
VCC       = v++
VPP_SPEC  = system.cfg
VPP_FLAGS = --save-temps --verbose --config ${VPP_SPEC} --config /proj/xtcRepo/dsv/HEAD/EVEREST/NOC/IPI/TCs/HEAD/xcvc1902/system_xsim.cfg

.PHONY: clean

all: fixed_xsa fixed_platform baremetal_elf package run

check-xsa:
ifeq ($(wildcard $(EXTENSIBLE_XSA)),)
	@echo "'XSA' is not accesible, please provide your xsa file path"
	@make help
	@echo "exit ....." && exit 1
endif
	@echo "XSA is accesible"


%.xo: %.cpp 
	${VCC} -c --platform ${PLATFORM} -t ${TARGET} -k $(basename $<) $< -o $@ --verbose --save-temps
fixed_xsa: $(FIXED_XSA)

${FIXED_XSA}: ${VPP_SPEC} ${XOS}
#${VCC} -g -l --platform ${PLATFORM} ${XOS} \
	       -t ${TARGET} ${VPP_FLAGS} -o $@
	${VCC} -g -l --platform ${PLATFORM} ${XOS} --vivado.prop fileset.sim_1.xsim.simulate.runtime=400us \
	       -t ${TARGET} ${VPP_FLAGS} -o $@
	
fixed_platform: ${FIXED_PLATFORM}

${FIXED_PLATFORM}: ${FIXED_XSA}
	generate-platform.sh -name fixed_platform -hw ${FIXED_XSA} -domain psv_cortexa72_0:standalone

baremetal_elf: $(HOST_EXE)
${HOST_EXE}: ${FIXED_PLATFORM} sw/main.cpp
	make -C sw/

package:$(FIXED_XSA) 
	${VCC} -p -t ${TARGET} -f ${FIXED_PLATFORM} \
        --package.boot_mode sd \
        --package.image_format=fat32 \
        --package.out_dir package.${TARGET} \
		--package.ps_elf main.elf,a72-0 $^ 


run: 
ifeq (${TARGET},hw_emu)
	@echo "INFO: Running Embedded Emulation: ${TARGET}"
	./package.${TARGET}/launch_hw_emu.sh
else
	@echo "INFO: Target is 'HW' and Please copy BOOT.BIN to the FAT32 partition of your SD card and insert to your board, then start test on HW !!!"	
endif



clean:
	rm -rf _x v++_* ${XOS} *.o.* *.xo.* \
	       *.xclbin* *.xsa *_log *.log Work Map_Report.csv *.bin  \
	       BOOT* *.xo emu_qemu_scripts *.cdo *.bif compile_log_pre *.elf *.o launch_hw_emu.sh qemu_args.txt *.xsa xnwOut *.pdi fixed_platform v++.package_summary *.link_summary libadf* lpd* NOC* pmc* plm* sd_card* sim ilpProblem* sol.db drivers .Xil .ipcache package.hw* ext_platform_wrapper_*
	make -C sw clean