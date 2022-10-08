# Copyright 2021 Xilinx Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# User editing area start
XSA_NAME=vck190_custom_dfx
DTB_PLT_NAME=vck190_custom_dt
PLATFORM_NAME=vck190_custom_dfx


# User editing area end
# -------------------
ROOT_DIR=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))
ABS_BUILD_PATH=$(ROOT_DIR)/build
STATIC_XSA=$(ROOT_DIR)/../step1_hw/build/vivado/$(XSA_NAME)_static.xsa
RP_XSA=$(ROOT_DIR)/../step1_hw/build/vivado/$(XSA_NAME)_rp.xsa
HW_EMU_XSA=$(ROOT_DIR)/../step1_hw/build/vivado/$(XSA_NAME)_hw_emu.xsa
DT_PATH=$(ABS_BUILD_PATH)/$(DTB_PLT_NAME)/psv_cortexa72_0/device_tree_domain/bsp
DTB_FILE=$(DT_PATH)/system.dtb
BOOT_DIR=$(ABS_BUILD_PATH)/boot
SD_DIR=$(ABS_BUILD_PATH)/sd_dir
VERSION=2022.1
COMMON_IMAGE_PATH ?=${PLATFORM_REPO_PATHS}/sw/versal/xilinx-versal-common-v$(VERSION)/

SW_COMP_PATH=$(ABS_BUILD_PATH)/sw_comp
SYSROOT_DIR=$(SW_COMP_PATH)/sysroots

BOOT_BIN_DIR=$(ABS_BUILD_PATH)/boot_bin
BOOT_IMAGE=$(ABS_BUILD_PATH)/boot_bin/BOOT.BIN
SW_FILES=$(SD_DIR)/boot.scr $(BOOT_DIR)/u-boot.elf $(BOOT_DIR)/bl31.elf
PDI_FILE=$(BOOT_BIN_DIR)/boot_static.pdi
PLATFORM_OUT_PATH=$(ABS_BUILD_PATH)/pfm

.PHONY: all sw_comp dtb help pfm bootimage

# --------------------------------------
# Public targets
# all: create PetaLinux project from XSA, update configuration, build PetaLinux project and sysroot
# main: create PetaLinux project from XSA, update configuration, build PetaLinux project, without sysroot
#
# Note: This makefile is not designed with dependent awareness. 
# If you change a source file, please run `make clean; make all` to rebuild from scratch.
# --------------------------------------

all: $(DTB_FILE) sw_comp pfm


help:
	@echo "Makefile usage"
	@echo "make all COMMON_IMAGE_PATH=<path/to/common_image/>  	## to generate platform with pre-built software components and run vadd application in software emulation mode"
	


check-common-image:
ifeq ($(wildcard $(COMMON_IMAGE_PATH)/Image),)
	@echo "'COMMON_IMAGE_PATH' is not accessible, please download common image from Xilinx Downloads website and set this flag to specify the common image path."
	@make help
	@echo "exit ....." && exit 1
endif
	@echo "INFO: Using COMMON_IMAGE_PATH=$(COMMON_IMAGE_PATH)"

# Target: dtb
# generate the device tree blob
dtb: $(DTB_FILE)

$(DTB_FILE): $(STATIC_XSA) system-user.dtsi
	$(info executing createdts to generate dts file)
	-xsct -eval "createdts -hw $(STATIC_XSA) \
		-out $(ABS_BUILD_PATH) \
		-zocl  \
		-platform-name $(DTB_PLT_NAME)  \
		-git-branch xlnx_rel_v2022.1 \
		-board versal-vck190-reva-x-ebm-01-reva" 

	@echo "INFO: Adding system-user.dtsi to add bootargs"
	cp system-user.dtsi $(DT_PATH)
	echo "#include \"system-user.dtsi\"" >> $(DT_PATH)/system-top.dts

	@echo "INFO: Compiling DTS to DTB"
	gcc -I my_dts -E -nostdinc -undef -D__DTS__ -x assembler-with-cpp -o $(DT_PATH)/system.dts $(DT_PATH)/system-top.dts && \
	dtc -I dts -O dtb -o $(DT_PATH)/system.dtb $(DT_PATH)/system.dts




# target :software components
# copy software components to local directory
sw_comp: $(SW_FILES)

$(SW_FILES): $(DTB_FILE)
	@echo "INFO: Copying components from common image and dtb output to boot and SD dir for FAT32 partition"
	mkdir -p $(BOOT_DIR) $(SD_DIR) $(SW_COMP_PATH)
	cp -f $(DTB_FILE) $(BOOT_DIR)
	cp -f $(COMMON_IMAGE_PATH)/bl31.elf $(BOOT_DIR)
	cp -f $(COMMON_IMAGE_PATH)/u-boot.elf $(BOOT_DIR)
	cp -f $(COMMON_IMAGE_PATH)/boot.scr $(SD_DIR)
	cp -f $(COMMON_IMAGE_PATH)/Image  $(SW_COMP_PATH)
	cp -f $(COMMON_IMAGE_PATH)/rootfs.ext4  $(SW_COMP_PATH)

# Target: sysroot
# Install sysroot to sw_comp directory
sysroot: $(COMMON_IMAGE_PATH)/sdk.sh
	rm -rf $(SYSROOT_DIR)/ && mkdir -p $(SYSROOT_DIR) && sh $(COMMON_IMAGE_PATH)/sdk.sh -d $(SW_COMP_DIR)/ -y

#----------
$(PDI_FILE): $(STATIC_XSA)
	$(info "Extracting PDI from static XSA")
	mkdir -p $(BOOT_BIN_DIR) 
	cp $(STATIC_XSA) $(BOOT_BIN_DIR)
	cd $(BOOT_BIN_DIR) && xsct -eval "openhw $(XSA_NAME)_static.xsa" && mv *.pdi boot_static.pdi
	

pdi: $(PDI_FILE)

$(BOOT_IMAGE): $(PDI_FILE) $(SW_FILES) $(DTB_FILE)
	$(info "BOOT image for platforms")
	bootgen -arch versal -image bootgen.bif -o $(BOOT_IMAGE) -w;

bootimage: $(BOOT_IMAGE)
#----------



pfm: $(STATIC_XSA) $(RP_XSA) $(SW_FILES) ${BOOT_IMAGE}
	$(info Creating Platform $(XSA_NAME))
	xsct xsct_create_pfm.tcl platform_name "${PLATFORM_NAME}" \
		static_xsa_path "${STATIC_XSA}" \
		rp_xsa_path "${RP_XSA}" \
		emu_xsa_path "${HW_EMU_XSA}" \
		boot_dir_path "${BOOT_DIR}" \
		boot_bin "${BOOT_IMAGE}" \
		sd_dir_path "${SD_DIR}" \
		platform_out "${PLATFORM_OUT_PATH}"

clean:
	$(RM) -r $(ABS_BUILD_PATH)/$(DTB_PLT_NAME) $(BOOT_DIR) $(SD_DIR) $(BOOT_BIN_DIR) $(PLATFORM_OUT_PATH)

ultraclean:
	$(RM) -r $(ABS_BUILD_PATH) 
