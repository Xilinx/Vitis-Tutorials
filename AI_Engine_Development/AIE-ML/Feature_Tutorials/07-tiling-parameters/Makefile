# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

# Global Simulation parameters 
include common.mk

# Path checking include
include verif_path.mk

# Compilation and Simulation parameters

SRC_DIR := src_$(SECTION)
GRAPH    := $(SRC_DIR)/graph.cpp
UTILS_DIR := ./Utils
DATA_DIR := data

LIBADFAIE  = libadfaie_$(SECTION).a
WorkDirAIE := ./$(SECTION)_WorkAie
OutDirAIE := ./$(SECTION)_aiesimulator_output

LIBADFX86  = libadfx86_$(SECTION).a
WorkDirX86 := ./$(SECTION)_WorkX86
OutDirX86 := ./$(SECTION)_x86simulator_output

ADDON_DIR := addon_files
XSA := ExtBuf.xsa

VPP_SPEC = $(ADDON_DIR)/system_ExtBuf.cfg


# Targets

.PHONY: data disp_repetition full_extbuf_hw full_extbuf_hwemu full_extbuf_sim_hwemu run_hw_emu clean preprocessor run_tests

run_tests: 
	make SECTION=doc clean_all data aie aiesim 
	make SECTION=memtile T2 T3 aie aiesim
	make SECTION=extbuf aie aiesim
	make SECTION=memmodule T1 T2 T3 aie aiesim
	


disp_repetition:
	@echo "==================================================="
	@echo "               Repetition Count"
	@echo "==================================================="
	@echo ""
	@grep -e "Repetition count" AIECompiler.log
	@echo ""
	@echo "==================================================="

	

data:
	mkdir -p $(DATA_DIR)
	$(UTILS_DIR)/CreateNDData.py 2 $(Base) 5 $(Dimensions) $(MaxIterations) > $(DATA_DIR)/Input.txt ;\
	$(UTILS_DIR)/CreateNDData.py 2 100 3 64 512 $(MaxIterations) > $(DATA_DIR)/Input_Large.txt ;\
	$(UTILS_DIR)/CreateNDData.py 2 $(Base) 2 256 $(MaxIterations) > $(DATA_DIR)/Input_1D_256.txt ; \
	$(UTILS_DIR)/CreateNDData.py 2 $(Base) 2 1024 $(MaxIterations) > $(DATA_DIR)/Input_1D_1024.txt ; \
	$(UTILS_DIR)/CreateNDData.py 2 $(Base) 2 64 $(MaxIterations) > $(DATA_DIR)/Input_1D_64.txt ; \


# Test cases
TC1 ?= 0
TC2 ?= 0
TC3 ?= 0
TC4 ?= 0
TC5 ?= 0
TC6 ?= 0
TC7 ?= 0
TC8 ?= 0


	
T1: 
	$(eval TC1 := 1)  

T2: 
	$(eval TC2 := 1)  

T3: 
	$(eval TC3 := 1)  

T4: 
	$(eval TC4 := 1)  

T5: 
	$(eval TC5 := 1)  

T6: 
	$(eval TC6 := 1)  

T7: 
	$(eval TC7 := 1)  

T8: 
	$(eval TC8 := 1)  

update_defines:
	$(eval D_VAR_TC := -DTC1=$(TC1) -DTC2=$(TC2) -DTC3=$(TC3) -DTC4=$(TC4))
	$(eval D_VAR_TC += -DTC5=$(TC5) -DTC6=$(TC6) -DTC7=$(TC7) -DTC8=$(TC8))



D_VAR := -DDim0=$(Dim0) -DDim1=$(Dim1) -DDim2=$(Dim2) -DDim3=$(Dim3) -DNIterations=$(NIterations_Data)

ifeq ($(TOOLV),2024.2)
preprocessor: update_defines
	$(eval PreProc := --aie.Xpreproc "\"$(D_VAR) $(D_VAR_TC)\"")
else
preprocessor: update_defines
	$(eval PreProc := --aie.Xpreproc "$(D_VAR)  $(D_VAR_TC)")
endif


AIE_CMPL_CMD = v++ --compile --mode aie --include "./$(SRC_DIR)" --aie.workdir $(WorkDirAIE) ${GRAPH} --platform=$(PLATFORM) --aie.output-archive $(LIBADFAIE) 
X86_CMPL_CMD = v++ --compile --mode aie --target x86sim --include "./$(SRC_DIR)" --aie.workdir $(WorkDirX86) ${GRAPH} --platform $(PLATFORM) --aie.output-archive $(LIBADFX86) 

AIE_SIM_CMD = aiesimulator --pkg-dir=$(WorkDirAIE) --dump-vcd foo --output-dir=$(OutDirAIE)
AIE_SIM_CMD += --hang-detect-time=100000

X86_SIM_CMD = x86simulator --pkg-dir=$(WorkDirX86) --trace --trace-print --output-dir=$(OutDirX86)


aie: ${LIBADFAIE} 
${LIBADFAIE}: $(SRC_DIR)/* preprocessor
	${AIE_CMPL_CMD} $(PreProc)




x86:${LIBADFX86}
${LIBADFX86}: $(SRC_DIR)/* preprocessor
	${X86_CMPL_CMD} $(PreProc)

aiesim: ${LIBADFAIE}
	${AIE_SIM_CMD}
	make process_files
	
# Define a target to perform an action on each file

outputfiles:
	$(eval OUTPUT_FILES := $(shell cd $(OutDirAIE)/data; ls Output_*.txt))


process_files: outputfiles
	echo "Processing -->    $(OUTPUT_FILES)"
	@echo "Processing the following files:"
	@for file in $(OUTPUT_FILES); do \
		echo "Processing $$file"; \
		grep -v T $(OutDirAIE)/data/$$file > $(OutDirAIE)/data/NoStamps_$$file; \
	done

extractBDs:
	$(UTILS_DIR)/CompilerReport.py $(WorkDirAIE) -bd_all

aieviz:
	vitis_analyzer $(WorkDirAIE)/graph.aiecompile_summary


aiesimviz:
	vitis_analyzer aiesimulator_output/default.aierun_summary


x86sim: ${LIBADFX86}
	${X86_SIM_CMD}


###########################################################
##
##          Link Stage for External Buffer System
##
###########################################################

VPP_FLAGS=--save-temps --verbose --config ${VPP_SPEC}
VPP := v++ 
VPP_LINK_FLAG := -l --platform $(PLATFORM)  $(LIBADFAIE) -t $(TARGET) --save-temps -g  -o $(XSA)


link: guard-PLATFORM_REPO_PATHS ${XSA}
${XSA}: ${LIBADFAIE} ${VPP_SPEC}
	$(VPP) $(VPP_LINK_FLAG)

###########################################################
##
##          Host code compilation for External Buffer System
##
###########################################################
HOST_DIR := $(PWD)/sw_extbuf
HOST_EXE := $(HOST_DIR)/host.exe
HOST_OBJ :=  $(HOST_DIR)/host.o
HOST_SRC := $(HOST_DIR)/host.cpp
HOST_INC := -I$(HOST_DIR) 

CXXFLAGS += -std=c++17 -I$(XILINX_HLS)/include/ -I${SDKTARGETSYSROOT}/usr/include/xrt/ -g -Wall -c -fmessage-length=0 --sysroot=${SDKTARGETSYSROOT}  ${HOST_INC} $(D_VAR) -O1
LDFLAGS +=  -lgcc -lc  -lxrt_coreutil -lpthread -lrt -ldl -lcrypt -lstdc++ -L${SDKTARGETSYSROOT}/usr/lib/ --sysroot=${SDKTARGETSYSROOT} -L$(XILINX_VITIS)/aietools/lib/aarch64.o

host: guard-CXX guard-SDKTARGETSYSROOT ${HOST_EXE}

${HOST_EXE}: ${HOST_OBJ} 
	${CXX} -o $@  ${LDFLAGS} $^

%.o: %.cpp
	${CXX} ${CXXFLAGS} $(D_VAR) -o $@ $<

###########################################################
##
##          Packaging for External Buffer System
##
###########################################################

package: guard-ROOTFS guard-IMAGE guard-PLATFORM_REPO_PATHS package_${TARGET}
package_${TARGET}: ${LIBADFAIE} ${XSA} ${HOST_EXE}
	v++ -p -t ${TARGET} -f ${PLATFORM} \
		--package.rootfs ${ROOTFS} \
		--package.kernel_image ${IMAGE} \
		--package.boot_mode=sd \
		--package.image_format=ext4 \
		--package.defer_aie_run \
		--package.sd_file addon_files/embedded_exec.sh \
		--package.sd_file ${HOST_EXE} ${XSA} ${LIBADFAIE}
ifeq ($(TARGET),hw)
	zip sd_card_$(shell date +%y%m%d_%H%M).zip sd_card.img
endif


full_extbuf_hw:
	make SECTION=extbuf TARGET=hw aie link host package

full_extbuf_hwemu:
	make SECTION=extbuf TARGET=hw_emu  aie link host package run_hw_emu

full_extbuf_sim_hwemu:
	make SECTION=extbuf TARGET=hw_emu clean data aie aiesim link host package run_hw_emu

run_hw_emu:
	chmod 744 qemu_resize_img.sh
	qemu_resize_img.sh sd_card.img 4G
	./launch_hw_emu.sh -add-env AIE_COMPILER_WORKDIR=./extbuf_WorkAie  -run-app embedded_exec.sh
	
		
clean:
	rm -rf _x libadf* .Xil vitis_analyzer* foo.vcd *.log pl_sample*
	rm -rf  vitis_design_wrapper*
	rm -rf .AIE_SIM_CMD_LINE_OPTIONS *.csv *.db temp *.txt
	rm -rf  logs _ide .crashReporter
	find *.json | grep -v "description.json" | xargs rm -f
	rm -rf  doc_*  memtile_* memmodule_* extbuf_*
	rm  -f *.zip *.img *.wcfg *.wdb plm.bin pmc_cdo.bin qemu_resize_img.sh v++.package_summary
	rm -f *.xclbin aie.merged.cdo.bin BOOT* boot_image.bif ExtBuf.* launch_hw_emu.sh
	rm -rf sd_card .ipcache emu_qemu_scripts cfg sim sw_extbuf/host.exe sw_extbuf/host.o

clean_all: clean
	rm -rf data
	
clean_git: clean_all
	rm -rf Utils/Thumbs.db
	rm -rf images/Thumbs.db

	
	
