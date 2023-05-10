#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11


#relative beamforming directory
RELATIVE_PROJECT_DIR := ../

#absolute beamforming directory = <user path>/Tutorials/AI_Engine/beamforming
PROJECT_DIR	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))

MODULE_04_DIR := $(PROJECT_DIR)/Module_04_AI_Engine_and_PL_Integration

# =========================================================
# BSP
# =========================================================
BSP := xilinx-vck190-v2023.1-final.bsp

# =========================================================
# XSA SELECTION
# =========================================================
XSA := $(MODULE_04_DIR)/build/rev1/hw/beamforming.rev1.hw.xsa

# =========================================================
# BSP SELECTION
# =========================================================
BSP := xilinx-vck190-v2023.1-final.bsp

# =========================================================
# BUILD PRODUCTS
# =========================================================
BUILD_DIR := build
PETALINUX_DIR := vck190_linux

# Edit the vck190_linux/project-spec/configs/config
# PetaLinux Config: Use EXT4 as rootfs format
update_config:
	echo "CONFIG_SUBSYSTEM_ROOTFS_EXT4=y" >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/configs/config
	echo 'CONFIG_SUBSYSTEM_MACHINE_NAME="versal-vck190-reva-x-ebm-02-reva"' >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/configs/config

# Edit the vck190_linux/project-spec/configs/rootfs_config
# PetaLinux Rootfs: Enable XR
# PetaLinux Rootfs: Enable DNF
# PetaLinux Rootfs: Enable packages for EoU
# PetaLinux Rootfs: CONFIG_libmetal=y
# PetaLinux Rootfs: CONFIG_libsysfs=y
# PetaLinux Rootfs: CONFIG_open-amp=y
# PetaLinux Rootfs: CONFIG_ai-engine-driver=y
# PetaLinux Rootfs: CONFIG_auto-login=y
update_rootfs_config:
	echo 'CONFIG_xrt=y' >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/configs/rootfs_config
	echo 'CONFIG_dnf=y' >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/configs/rootfs_config
	echo 'CONFIG_imagefeature-package-management=y' >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/configs/rootfs_config
	echo 'CONFIG_e2fsprogs-resize2fs=y' >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/configs/rootfs_config
	echo 'CONFIG_parted=y' >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/configs/rootfs_config
	echo 'CONFIG_libmetal=y' >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/configs/rootfs_config
	echo 'CONFIG_libsysfs=y' >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/configs/rootfs_config
	echo 'CONFIG_open-amp=y' >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/configs/rootfs_config
	echo 'CONFIG_ai-engine-driver=y' >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/configs/rootfs_config
	echo "# CONFIG_aie-matrix-multiplication is not set" >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/configs/rootfs_config
	echo "# CONFIG_aie-notebooks is not set" >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/configs/rootfs_config

# Add vck190_linux/project-spec/meta-user/recipes-kernel/linux/linux-xlnx/bsp.cfg
# PetaLinux Kernels: CONFIG_I2C_XILINX=y
# PetaLinux Kernels: CONFIG_GPIO_XILINX=y
# PetaLinux Kernels: CONFIG_FPGA=y
# PetaLinux Kernels: CONFIG_FPGA_MGR_VERSAL_FPGA=y
# PetaLinux Kernels: CONFIG_XILINX_INTC=y
update_kernel_config:
	@echo "CONFIG_I2C_XILINX=y" >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/meta-user/recipes-kernel/linux/linux-xlnx/bsp.cfg
	@echo "CONFIG_GPIO_XILINX=y" >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/meta-user/recipes-kernel/linux/linux-xlnx/bsp.cfg
	@echo "CONFIG_XILINX_INTC=y" >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/meta-user/recipes-kernel/linux/linux-xlnx/bsp.cfg
	@echo "CONFIG_FPGA=y" >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/meta-user/recipes-kernel/linux/linux-xlnx/bsp.cfg
	@echo "CONFIG_FPGA_MGR_VERSAL_FPGA=y" >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/meta-user/recipes-kernel/linux/linux-xlnx/bsp.cfg

update_dtsi:
	@cp -f uio-system-user.dtsi $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/meta-user/recipes-bsp/device-tree/files/system-user.dtsi

update_linux.bif:
	@cp -f linux.bif $(BUILD_DIR)/$(PETALINUX_DIR)/images/linux/linux.bif

update_boot:
	mkdir -p build/image
	cp -f build/vck190_linux/images/linux/boot.scr ./build/image/.
	cp -f init.sh ./build/image/.
	cp -f platform_desc.txt ./build/image/.

petalinux_create:
	mkdir $(BUILD_DIR) -p; \
	cd $(BUILD_DIR); \
	petalinux-create -t project -s $(BSP_DIR)/$(BSP) -n $(PETALINUX_DIR)
	echo "CONFIG_YOCTO_BUILDTOOLS_EXTENDED=y" >>  $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/configs/config
	@sed -i -E 's/.*CONFIG_SUBSYSTEM_INITRAMFS_IMAGE_NAME.+/CONFIG_SUBSYSTEM_INITRAMFS_IMAGE_NAME="petalinux-image-minimal"/' $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/configs/config

petalinux_config:
	petalinux-config -p $(BUILD_DIR)/$(PETALINUX_DIR) --get-hw-description=$(XSA) --silentconfig
	$(MAKE) -f $(lastword $(MAKEFILE_LIST)) update_config
	$(MAKE) -f $(lastword $(MAKEFILE_LIST)) update_rootfs_config
	$(MAKE) -f $(lastword $(MAKEFILE_LIST)) update_dtsi
	$(MAKE) -f $(lastword $(MAKEFILE_LIST)) update_kernel_config
	petalinux-config -p $(BUILD_DIR)/$(PETALINUX_DIR) --silentconfig

petalinux_build:
	petalinux-build -p $(BUILD_DIR)/$(PETALINUX_DIR)
	petalinux-build -p $(BUILD_DIR)/$(PETALINUX_DIR) --sdk
	cd $(BUILD_DIR)/$(PETALINUX_DIR) && petalinux-package --sysroot
	cd $(BUILD_DIR)/$(PETALINUX_DIR) && petalinux-package --boot --u-boot
	$(MAKE) -f $(lastword $(MAKEFILE_LIST)) update_linux.bif

platform:
	xsct xsct_create_pfm.tcl vck190_custom $(XSA)

petalinux: $(BUILD_DIR)/$(PETALINUX_DIR)/images/linux/Image

$(BUILD_DIR)/$(PETALINUX_DIR)/images/linux/Image: $(XSA)
	$(MAKE) petalinux_create
	$(MAKE) petalinux_config
	$(MAKE) petalinux_build
	$(MAKE) update_boot
	$(MAKE) platform

clean:
	rm -rf $(BUILD_DIR)
	rm -rf .Xil
