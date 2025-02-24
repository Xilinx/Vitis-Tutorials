CXX := $(XILINX_VITIS)/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-g++
pfm = xilinx_vck190_base
dev = vck190
dev_name = base
TARGET=hw_emu

SOURCE_PATH = hw
XSA := hw/build/hw_emu/hw_emu.xsa

TEMP_DIR := ./_x.$(TARGET)
KERNEL_XO := stream_out.xo stream_in.xo polar_clip.xo
#KERNEL_XO := stream_in.xo polar_clip.xo
#KERNEL_XO := aie_stream_in.xo stream_in.xo polar_clip.xo


CONFIG := vitis_impl/system.cfg
#CONFIG := vitis_impl/system_custom_ip.cfg
#CONFIG := vitis_impl/system_new.cfg

SDKTARGETSYSROOT ?= $(SYSROOT)
PLATFORM ?= ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_202420_1/xilinx_vck190_base_202420_1.xpfm
ROOTFS ?= ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v2024.2/rootfs.ext4
IMAGE ?=  ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v2024.2/Image


GRAPH_O := libadf.a
VMA := design.vma

EMCONFIGUTIL := emconfigutil
EMCONFIG_FILE := emconfig.json
fixed_xsa := $(TARGET)_fixed.xsa
SD_IMAGE_FILE := ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal/Image
EMBEDDED_EXEC_SCRIPT := embedded_${TARGET}.sh
EMBEDDED_EMFLAGS := -verbose -no-reboot -login "petalinux" -password "petalinux"
EMBEDDED_PACKAGE_OUT := package.${TARGET}
EMBEDDED_EMULATOR := $(EMBEDDED_PACKAGE_OUT)/launch_${TARGET}.sh
SYSTEM_DTB:= $(shell cd  $(EDGE_COMMON_SW)/../../../$(DEVICE) && pwd)

XCLBIN := krnl_lab8.xclbin
HOST_SRC :=  sw/host.cpp Work/ps/c_rts/aie_control_xrt.cpp
HOST_EXE := ./host.exe
VPP := v++
VPP_FLAGS := --save-temps
AIECC := aiecompiler
AIE_INCLUDE_FLAGS := -include="$(XILINX_VITIS)/aietools/include" -include="./aie" -include="./data" -include="./"
AIE_FLAGS := --platform $(XSA) $(AIE_INCLUDE_FLAGS) --pl-freq=100 -workdir=./Work

<<<<<<< HEAD
# Host rules
host: $(HOST_SRC)
	mkdir -p host_code_dir
	$(CXX)  $(GCC_FLAGS) $(GCC_INCLUDES) $(HOST_SRC)  
	$(CXX) *.o $(GCC_LIB) -std=c++17 $(GCC_LIB) -o $(HOST_EXE) 
	@echo "INFO: Compiled Host Executable: $(HOST_EXE)"

EMBEDDED_EXEC_SCRIPT = embedded_exec.sh
EMCONFIGUTIL = emconfigutil

##################################################################################################################################################
## Package
##################################################################################################################################################

ifeq ($(TARGET),hw_emu)

package:
	echo "INFO: Generating ${EMCONFIG_FILE}";\
	${EMCONFIGUTIL} --platform ./hw/build/my_project/vck190_prj_fixed_hw_emu.xsa --nd 1;\
	cd ./sw
	v++ -p -t $(TARGET) -f ./hw/build/my_project/vck190_prj_fixed_hw_emu.xsa \
	--package.sd_dir ${PLATFORM_REPO_PATHS}/sw/versal/aie ./aie/libadf.a \
	--package.out_dir package.hw_emu \
	--package.rootfs ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal/rootfs.ext4 \
	--package.sd_file ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal/Image \
	--package.sd_file emconfig.json \
	--package.bl31_elf ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_bdc_202510_1/sw/xilinx_vck190_base_bdc_202510_1/boot/bl31.elf \
	--package.boot_mode sd \
	--package.uboot ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_bdc_202510_1/sw/xilinx_vck190_base_bdc_202510_1/boot/u-boot.elf \
	--package.sd_file ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal/boot.scr \
	--package.sd_file host.exe \
	--package.sd_file embedded_exec.sh \
	--package.defer_aie_run \
	--package.dtb ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_bdc_202510_1/sw/xilinx_vck190_base_bdc_202510_1/boot/system.dtb \
	-o vck190_prj_hw_emu.xclbin 
=======
HOST_CFLAGS = -Isrc -Isw -Iaie -ladf_api_xrt -lxrt_coreutil -D__SYNCBO_ENABLE__ $(TARGET_FLAGS) -DTARGET_DEVICE=$(DEVICE) -std=c++14 -O0 -g -Wall -fmessage-length=0 -I${GITHUB_COMMON_REPO}/common/includes/xcl2 --sysroot=${SYSROOT} -I=/usr/include -I=/usr/include/xrt -L=/usr/lib -ldl -lcrypt -lstdc++ -lxilinxopencl -lpthread -lrt -I$(XILINX_VITIS)/aietools/include -L$(XILINX_VITIS)/aietools/lib/aarch64.o -I${XILINX_VITIS}/include -I${XILINX_HLS}/include
>>>>>>> 139a170b (PFM/F/03_Vitis_Export_To_Vivado: Updated Tutorial for 24.2 )

ifeq ($(TARGET),$(filter $(TARGET),hw_emu))
package_config := --package.sd_file $(EMCONFIG_FILE)
TARGET_FLAGS := -DTARGET_FLOW_HW=0 -DTARGET_FLOW_HW_EMU=1 -DTARGET_FLOW_SW_EMU=0 
else
<<<<<<< HEAD

package:
	v++ -p -t $(TARGET) -f ./hw/build/my_project/vck190_prj_fixed_hw.xsa \
	--package.sd_dir ${PLATFORM_REPO_PATHS}/sw/versal/aie ./aie/libadf.a \
	--package.out_dir package.hw \
	--package.rootfs ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal/rootfs.ext4 \
	--package.sd_file ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal/Image \
	--package.bl31_elf ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_bdc_202510_1/sw/xilinx_vck190_base_bdc_202510_1/boot/bl31.elf \
	--package.boot_mode sd \
	--package.uboot ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_bdc_202510_1/sw/xilinx_vck190_base_bdc_202510_1/boot/u-boot.elf \
	--package.sd_file ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal/boot.scr \
	--package.sd_file host.exe \
	--package.defer_aie_run \
	--package.dtb ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_bdc_202510_1/sw/xilinx_vck190_base_bdc_202510_1/boot/system.dtb \
	-o vck190_prj_hw.xclbin 

=======
package_config :=
TARGET_FLAGS := -DTARGET_FLOW_HW=1 -DTARGET_FLOW_HW_EMU=0 -DTARGET_FLOW_SW_EMU=0
>>>>>>> 139a170b (PFM/F/03_Vitis_Export_To_Vivado: Updated Tutorial for 24.2 )

endif
xsa:
	make -C $(SOURCE_PATH) all

all: build run_emu
build:xsa viv_changes kernels graph export link host package

kernels: $(KERNEL_XO)
stream_in.xo: pl_kernels/stream_in.cpp
	$(VPP) -c $(VPP_FLAGS) --target $(TARGET) --platform $(XSA) --temp_dir $(TEMP_DIR) -k $(basename $(notdir $<)) $< -o $@
polar_clip.xo: pl_kernels/polar_clip.cpp
	$(VPP) -c $(VPP_FLAGS) --platform $(XSA) --target $(TARGET) --temp_dir $(TEMP_DIR) -k $(basename $(notdir $<)) $< -o $@
aie_stream_in.xo: pl_kernels/aie_stream_in.cpp
	$(VPP) -c $(VPP_FLAGS) --platform $(XSA) --target $(TARGET) --temp_dir $(TEMP_DIR) -k $(basename $(notdir $<)) $< -o $@
stream_out.xo: pl_kernels/stream_out.cpp
	$(VPP) -c $(VPP_FLAGS) --platform $(XSA) --target $(TARGET) --temp_dir $(TEMP_DIR) -k $(basename $(notdir $<)) $< -o $@

graph: $(GRAPH_O)

libadf.a: aie/graph.cpp
	$(AIECC) $(AIE_FLAGS) $< 

viv_changes : 
	cp -r $(SOURCE_PATH)/build/my_project ./
	vivado -mode batch -source viv_changes.tcl -tclargs $(TARGET)

export: 
	$(VPP) -l $(VPP_FLAGS) --platform $(XSA) --temp_dir $(TEMP_DIR) $(KERNEL_XO) $(GRAPH_O) --config $(CONFIG) --export_archive --advanced.param compiler.exportArchiveHierName=vmaHier -o $(VMA) 

link: 
	$(VPP) -l $(VPP_FLAGS) --platform $(XSA) --temp_dir $(TEMP_DIR) $(KERNEL_XO) $(GRAPH_O) --config $(CONFIG) --target $(TARGET) -o hw_emu_fixed.xsa 


import:
	vivado -mode batch -source import_$(dev_name).tcl -tclargs $(VMA) $(TARGET)


ifeq ($(TARGET),$(filter $(TARGET),hw_emu))
package :
	v++ -p -t ${TARGET} --package.out_dir $(EMBEDDED_PACKAGE_OUT) \
		-f ${PLATFORM} \
		--package.rootfs=${ROOTFS} \
		--package.image_format=ext4 \
		--package.boot_mode=sd \
		--package.kernel_image=${IMAGE} \
		--package.defer_aie_run \
		--package.sd_file embedded_hw_emu.sh \
		--package.sd_file ./host.exe ./hw_emu_fixed.xsa ./libadf.a \
		-o $(XCLBIN)
else 
package: 
	echo "INFO: Generating $(EMCONFIG_FILE)";\
	$(EMCONFIGUTIL) --platform $(fixed_xsa) --nd 1;\
	$(VPP) -p $(VPP_FLAGS) -t $(TARGET) --platform $(fixed_xsa) --temp_dir $(TEMP_DIR) --package.out_dir $(EMBEDDED_PACKAGE_OUT) --package.rootfs ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal/rootfs.ext4 --package.image_format=ext4 --package.boot_mode=sd --package.kernel_image=$(SD_IMAGE_FILE) --package.defer_aie_run --package.sd_file $(EMBEDDED_EXEC_SCRIPT) --package.sd_file $(HOST_EXE) ./$(GRAPH_O) -o $(XCLBIN) $(package_config) --package.uboot ${PLATFORM_REPO_PATHS}/xilinx_vck190_base_202420_1/sw/boot/u-boot.elf --package.sd_file ${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal/boot.scr --package.bl31_elf $(EDGE_COMMON_SW)/sw/boot/bl31.elf --package.dtb $(EDGE_COMMON_SW)/sw/boot/system.dtb

endif

.PHONY:xsa all cleanall

run_emu:
ifeq ($(TARGET),$(filter $(TARGET),hw_emu))
	@echo "INFO: Running Embedded Emulation: ${TARGET}"
	./${EMBEDDED_EMULATOR} ${EMBEDDED_EMFLAGS} -run-app  ./${EMBEDDED_EXEC_SCRIPT}  | tee embedded_run.log;
endif
run_board:
	/proj/sdxbf/emhw/bin/zboard run-test -e ${EMBEDDED_EXEC_SCRIPT} -i ${EMBEDDED_PACKAGE_OUT}/sd_card.img -m pl_mode 2>&1 | tee embedded_run.log;

host:
	$(CXX) $(HOST_SRC) -o $(HOST_EXE) $(HOST_CFLAGS)
	@echo "INFO: Compiled Host Executable: $(HOST_EXE)"

cleanall:
	rm -rf *.xclbin* *.xo* *.vma* *.log *.xsa *.o *.db *.a *.json *.csv package.hw host.exe hw/build
	rm -rf _x* Work package.hw_emu/ *.log *.jou *.pdi *.xsa *.vma *.xo *.*_summary  _x.hw_emu/ my_project* drivers/ extracted/ libadf.a  Map_Report.csv krnl_lab8.xclbin* sol.db Work/ aie_primitive.json emconfig.json	
