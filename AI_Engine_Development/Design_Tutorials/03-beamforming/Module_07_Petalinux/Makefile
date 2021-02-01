#  © Copyright 2020 Xilinx, Inc.
#  Licensed under the Apache License, Version 2.0 (the 'License');
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  
#       http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an 'AS IS' BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

# =========================================================
# PROJECT and MODULE REPOS
# =========================================================
#relative beamforming directory
RELATIVE_PROJECT_DIR := ../

#absolute beamforming directory = <user path>/Tutorials/AI_Engine/beamforming
PROJECT_DIR	:= $(shell readlink -f $(RELATIVE_PROJECT_DIR))

MODULE_04_DIR := $(PROJECT_DIR)/Module_04_AI_Engine_and_PL_Integration

# =========================================================
# BSP SELECTION
# =========================================================
BSP :=  xilinx-vck190-es1-v2020.2-final.bsp

# =========================================================
# XSA SELECTION
# =========================================================
XSA := $(MODULE_04_DIR)/build/rev1/hw/beamforming.rev1.hw.xsa

# =========================================================
# BUILD PRODUCTS
# =========================================================
BUILD_DIR := build
PETALINUX_DIR := vck190_linux

update_rootfs_config:
# Edit the vck190_linux/project-spec/configs/rootfs_config
# CONFIG_libmetal=y
# CONFIG_libsysfs=y
# CONFIG_open-amp=y
# CONFIG_ai-engine-driver=y
# CONFIG_auto-login=y
	@sed -i -E 's/.*CONFIG_libmetal.+/CONFIG_libmetal=y/' $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/configs/rootfs_config
	@sed -i -E 's/.*CONFIG_libsysfs.+/CONFIG_libsysfs=y/' $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/configs/rootfs_config
	@sed -i -E 's/.*CONFIG_open-amp.+/CONFIG_open-amp=y/' $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/configs/rootfs_config
	@sed -i -E 's/.*CONFIG_ai-engine-driver.+/CONFIG_ai-engine-driver=y/' $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/configs/rootfs_config
	@sed -i -E 's/.*CONFIG_auto-login.+/CONFIG_auto-login=y/' $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/configs/rootfs_config

update_kernel_config:
# Add vck190_linux/project-spec/meta-user/recipes-kernel/linux/linux-xlnx/bsp.cfg
# CONFIG_I2C_XILINX=y
# CONFIG_GPIO_XILINX=y
# CONFIG_FPGA=y
# CONFIG_FPGA_MGR_VERSAL_FPGA=y
# CONFIG_XILINX_INTC=y
	@echo "CONFIG_I2C_XILINX=y" >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/meta-user/recipes-kernel/linux/linux-xlnx/bsp.cfg
	@echo "CONFIG_GPIO_XILINX=y" >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/meta-user/recipes-kernel/linux/linux-xlnx/bsp.cfg
	@echo "CONFIG_XILINX_INTC=y" >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/meta-user/recipes-kernel/linux/linux-xlnx/bsp.cfg
	@echo "CONFIG_FPGA=y" >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/meta-user/recipes-kernel/linux/linux-xlnx/bsp.cfg
	@echo "CONFIG_FPGA_MGR_VERSAL_FPGA=y" >> $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/meta-user/recipes-kernel/linux/linux-xlnx/bsp.cfg


update_dtsi:
	@cp -f uio-system-user.dtsi $(BUILD_DIR)/$(PETALINUX_DIR)/project-spec/meta-user/recipes-bsp/device-tree/files/system-user.dtsi

petalinux_create: 
	mkdir $(BUILD_DIR) -p; \
	cd $(BUILD_DIR); \
	petalinux-create -t project -s $(PATH_TO_BSP)/$(BSP) -n $(PETALINUX_DIR)

petalinux_config:
	petalinux-config -p $(BUILD_DIR)/$(PETALINUX_DIR) --get-hw-description=$(XSA) --silentconfig
	$(MAKE) -f $(lastword $(MAKEFILE_LIST)) update_rootfs_config
	$(MAKE) -f $(lastword $(MAKEFILE_LIST)) update_dtsi
	$(MAKE) -f $(lastword $(MAKEFILE_LIST)) update_kernel_config

petalinux_build:
	petalinux-build -p $(BUILD_DIR)/$(PETALINUX_DIR)
	petalinux-build -p $(BUILD_DIR)/$(PETALINUX_DIR) --sdk
	cd $(BUILD_DIR)/$(PETALINUX_DIR) && petalinux-package --sysroot
	cd $(BUILD_DIR)/$(PETALINUX_DIR) && petalinux-package --boot --u-boot

petalinux: $(BUILD_DIR)/$(PETALINUX_DIR)/images/linux/Image

$(BUILD_DIR)/$(PETALINUX_DIR)/images/linux/Image: $(XSA)
	$(MAKE) petalinux_create 
	$(MAKE) petalinux_config 
	$(MAKE) petalinux_build


clean:
	rm -rf $(BUILD_DIR)

