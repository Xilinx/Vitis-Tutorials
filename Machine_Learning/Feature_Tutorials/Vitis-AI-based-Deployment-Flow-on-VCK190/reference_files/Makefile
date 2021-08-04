############################## Help ##############################
.PHONY: help
help:
	@echo "Makefile Usage:"
	@echo "  make files"
	@echo "      Command to copy IP files to vitis_prj folder."
	@echo ""
	@echo "  make all"
	@echo "      Command to generate the design."
	@echo ""
	@echo "  make clean "
	@echo "      Command to remove IP files from vitis_prj folder and all the generated files."
############################ Project Variables #####################
#Absolute path of this makefile
ABS_PATH                := $(shell pwd -P)
$(eval FATHER_PATH      := $(realpath $(ABS_PATH)/..))
#Platform to build for
PLATFORM                 = $(FATHER_PATH)/vck190_base_trd_platform1_2020.2_v0.5/platform/vck190_base_trd_platform1/vck190_base_trd_platform1.xpfm
DEVICE					?= vck190
XCL_EMULATION_MODE      ?= hw
unexport XCL_EMULATION_MODE

#Build directory
BUILD_DIR                = $(XCL_EMULATION_MODE)

#Default v++ compiler flags
VXXFLAGS                += -t $(XCL_EMULATION_MODE) --platform $(PLATFORM) --save-temps --temp_dir $(BUILD_DIR)/binary_container_1 -g
VXXFLAGS                += --xp vivado_prop:run.impl_1.STEPS.PLACE_DESIGN.TCL.PRE=$(ABS_PATH)/scripts/pre_place.tcl
VXXFLAGS                += --xp vivado_prop:run.impl_1.STEPS.PLACE_DESIGN.TCL.POST=$(ABS_PATH)/scripts/post_place.tcl
VXXFLAGS				+= --xp param:compiler.userPostSysLinkOverlayTcl=$(ABS_PATH)/scripts/post_linker.tcl
VXXFLAGS                += --xp vivado_prop:run.impl_1.GEN_FULL_BITSTREAM=1
VXXFLAGS                += --log_dir $(BUILD_DIR)/binary_container_1/logs
VXXFLAGS                += --report_dir $(BUILD_DIR)/binary_container_1/reports
VXXFLAGS                += --xp vivado_prop:run.impl_1.STEPS.POST_ROUTE_PHYS_OPT_DESIGN.IS_ENABLED=false

VXX                      = $(XILINX_VITIS)/bin/v++

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#Build products
AIE_OBJECTS				 = $(ABS_PATH)/aie/ip/libadf.a

BINARY_CONTAINERS       += $(BUILD_DIR)/binary_container_1.xclbin
BUILD_SUBDIRS           += $(BUILD_DIR)/binary_container_1
BINARY_CONTAINER_1_OBJS += $(BUILD_DIR)/binary_container_1/DPUCVDX8G.xo $(BUILD_DIR)/binary_container_1/libadf.a

COSIM_PKG_DIR            = $(ABS_PATH)/aie/ip/Work
export COSIM_PKG_DIR

JSON_DEVICE_FILE_PATH    = ${XILINX_VITIS}/cardano/data/devices/VC1902.json
export JSON_DEVICE_FILE_PATH

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# Build AIE graph 
aie_sourcefile			+= $(ABS_PATH)/aie/src/* 
aie_sourcefile			+= $(ABS_PATH)/aie/scripts/* 
aie_sourcefile			+= $(ABS_PATH)/aie/genStream/*

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# Package RTL kernel 
VIVADO_ROOT             := $(XILINX_VIVADO)
VIVADO                  := $(VIVADO_ROOT)/bin/vivado

xvdpu_HDLSRCS	    	+= $(ABS_PATH)/scripts/kernel.xml
xvdpu_HDLSRCS	       	+= $(ABS_PATH)/scripts/package_xvdpu_kernel.tcl
xvdpu_HDLSRCS	       	+= $(ABS_PATH)/scripts/gen_xvdpu_xo.tcl
xvdpu_HDLSRCS	       	+= $(ABS_PATH)/xvdpu/hdl/*.sv
xvdpu_HDLSRCS	      	+= $(ABS_PATH)/xvdpu/inc/*.vh

xvdpu_TCL				 = $(ABS_PATH)/scripts/gen_xvdpu_xo.tcl

# Kernel name must match kernel name in kernel.xml
DPU_KERN_NAME 			 = DPUCVDX8G

#Prepare project files
.PHONY: files
files: 
	@echo "INFO:- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
	@echo "INFO:Preparing files for project"	
	cp -r $(FATHER_PATH)/xvdpu_ip/aie           aie	
	cp $(FATHER_PATH)/xvdpu_ip/rtl/hdl/*.sv     xvdpu/hdl
	cp $(FATHER_PATH)/xvdpu_ip/rtl/inc/*.vh     xvdpu/inc

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
.PHONY: all
all: $(BINARY_CONTAINERS)  $(BUILD_DIR)/xrt.ini $(BUILD_DIR)/dpu.xclbin 	
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#Clean build products
.PHONY: clean
.NOTPARALLEL: clean
clean:
	-@rm -rf .Xil vivado* *.xpe *.txt *.log hw.o *.xsa .ipcache .run hw hw_emu prep_target_output sw_emu sample*.ini packaged_kernel_*_hw_* tmp_kernel_pack_*_hw_*
	-@rm -rf xvdpu/hdl/*.sv xvdpu/inc/*.vh package_out 
	-@rm -rf aie 
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#Prepare AIE definition for Vitis
$(BUILD_DIR)/binary_container_1/libadf.a:
	@echo "INFO:- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
	@echo "INFO:Copying $(notdir $@)..."
	@mkdir -p $(@D)
	@cp aie/ip/libadf.a "$@"
	
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#Compile xvdpu kernel
$(BUILD_DIR)/binary_container_1/DPUCVDX8G.xo: $(xvdpu_HDLSRCS)
	@echo "INFO:- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
	@echo "INFO:Building $(notdir $@) for $(BUILD_DIR)..."
	@mkdir -p $(@D)
	-@rm -rf $@
	$(VIVADO) -mode batch -source $(xvdpu_TCL) -tclargs $@ $(DPU_KERN_NAME) $(XCL_EMULATION_MODE) $(DEVICE)	
	
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#Link xvdpu kernel and AIE graph
$(BUILD_DIR)/binary_container_1.xclbin: $(BINARY_CONTAINER_1_OBJS)
	@echo "INFO:- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
	@echo "INFO:Building $(notdir $@) for $(BUILD_DIR)..."
	@mkdir -p $(@D)
	@$(VXX) $(VXXFLAGS) -l --config scripts/system.cfg --config scripts/xvdpu_aie.cfg -o "$@" $(BINARY_CONTAINER_1_OBJS) 
	
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#Create default xrt.ini file
$(BUILD_DIR)/xrt.ini:
	@echo "INFO:- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
	@echo "INFO:Creating default $(notdir $@) for $(BUILD_DIR)..."
	@echo "[Emulation]" > $@
	@echo "keep_run_dir=true" >> $@
	@echo "launch_waveform=off" >> $@
	@echo "[Runtime]" >> $@
	@echo "pdi_load=false" >> $@
	@echo "ert=false" >> $@
	
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#Generate xclbin and Package design
.PHONY: xclbin
xclbin: $(BUILD_DIR)/dpu.xclbin
$(BUILD_DIR)/dpu.xclbin: $(BUILD_DIR)/binary_container_1.xclbin $(BUILD_DIR)/binary_container_1/libadf.a $(BUILD_DIR)/xrt.ini
	@echo "INFO:- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
	@echo "INFO:Packaging $(notdir $@) for $(BUILD_DIR)..."
	-@rm -rf package_out
	@cd $(BUILD_DIR); rm -rf dpu.xclbin binary_container_1.xsa BOOT_bh.bin BOOT.BIN boot_image.bif emulation_data launch_hw_emu.sh libsdf plm.bin pmc_args.txt pmc_cdo.bin qemu_args.txt qemu_ospi.bin sim
	@cd $(BUILD_DIR); $(VXX) -t $(XCL_EMULATION_MODE) --platform $(PLATFORM)  --save-temps --temp_dir binary_container_1 -p binary_container_1.xclbin binary_container_1/libadf.a -o "$@" \
	  --package.out_dir ../package_out \
	  --package.boot_mode sd  \
	  --package.rootfs $(FATHER_PATH)/vck190_base_trd_platform1_2020.2_v0.5/petalinux/xilinx-vck190-prod-base-trd-platform1-2020.2/images/linux/rootfs.ext4 \
	  --package.sd_file $(FATHER_PATH)/vck190_base_trd_platform1_2020.2_v0.5/petalinux/xilinx-vck190-prod-base-trd-platform1-2020.2/images/linux/Image \
	  --package.sd_file xrt.ini	
	-@rm -rf .ipcache .Xil packaged_kernel_*_hw_* tmp_kernel_pack_*_hw_* vivado* *.xsa *.log hw.o sample*.ini	
	 
