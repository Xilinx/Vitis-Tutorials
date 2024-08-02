#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

ECHO                         = @echo
WORKSPACE					:= workspace
HW_EMU_PKG 					:= simple_aie_application_system_project/build/hw_emu/package/package

# Makefile input options
TARGET 						:= hw_emu
XSA							:= custom_platform_vck190.xsa
VITIS_PLATFORM				:= base_pfm_vck190

.PHONY: clean all vitis

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate the full Vitis workspace"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

pfm:../vivado/build/xsa_platform/${XSA}
	mkdir workspace
	vitis -s vitis_pfm.py ${VITIS_PLATFORM} ${XSA}

pl_app: hls_comp vpp_link
	
aie_app:
	vitis -s aie_app.py ${VITIS_PLATFORM}

hls_comp:
	vitis -s hls_comp.py ${VITIS_PLATFORM}
	
vpp_link:
	sed -i '9 a\#if defined(__AIESIM__) || defined(__X86SIM__) || defined(__ADF_FRONTEND__)' ./workspace/simple_aie_application/src/project.cpp;
	echo "#endif" >> ./workspace/simple_aie_application/src/project.cpp;
	vitis -s vpp_link.py ${VITIS_PLATFORM} ${TARGET}

ps_app: create_ps_app ps_config build_ps

create_ps_app:
	$(ECHO) "Creating PS platform and PS application"
	vitis -s ps_app.py ${TARGET}

ps_config:./workspace/A-to-Z_app/src/lscript.ld
	$(ECHO) "Editing Heap Size in Linker script"
	$(shell sed -i 's/_HEAP_SIZE :.*;/_HEAP_SIZE : 0x100000;/g' ./workspace/A-to-Z_app/src/lscript.ld)

build_ps:
	vitis -s build_ps_app.py

package:
	vitis -s package.py ${TARGET}

vitis: pfm aie_app hls_comp vpp_link ps_app package

run_hw_emu: ${WORKSPACE}/${HW_EMU_PKG}/launch_hw_emu.sh
	./${WORKSPACE}/${HW_EMU_PKG}/launch_hw_emu.sh

clean:
	rm -rf ${WORKSPACE} .Xil package
