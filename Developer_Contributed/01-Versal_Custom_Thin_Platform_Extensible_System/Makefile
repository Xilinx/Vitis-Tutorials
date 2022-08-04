 #
 # Copyright 2021 Xilinx, Inc.
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
 #
# Top-level Makefile -- Calls sub-makefiles

ECHO                               = @echo

################################################################################
# Variables needed for AMD/XILINX tools version checking, PLEASE DO NOT MODIFY #
################################################################################
REQUIRED_VERSION                   = 2022.2
VIVADO_VERSION                    := $(shell vivado -version 2>/dev/null | grep "Vivado ")
VITIS_VERSION                     := $(shell vitis -version 2>/dev/null | grep "Vitis " | sed 's/\*//g')

###############################################
# Variables that may be changed to your needs #
###############################################

# TARGET = hw or hw_emu
export TARGET                     := hw

# ILA_EN = 0 (Disabled) or 1 (Enabled)
export ILA_EN                     := 0

# LINUX_BUILD_TOOL = petalinux or yocto
export LINUX_BUILD_TOOL           := petalinux

# LINUX_TMP_DIR = Any dir which is not NFS
export LINUX_TMP_DIR              := /tmp/${USER}/${REQUIRED_VERSION}/${LINUX_BUILD_TOOL}

# LINUX_YOCTO_ROOT = Directory where yocto tools are installed
# Used ONLY when LINUX_BUILD_TOOL := yocto!
export LINUX_YOCTO_ROOT           := ${HOME}/bin

# LINUX_BUILD_SOURCE = network or local
# Used ONLY when LINUX_BUILD_TOOL := petalinux!
export LINUX_BUILD_SOURCES        := network

# LINUX_DOWNLOADS_DIR = Absolute Path where "downloads" is located
# Used ONLY when LINUX_BUILD_TOOL := petalinux and LINUX_BUILD_SOURCES := local
export LINUX_LOCAL_DOWNLOADS_DIR  := /tmp/plnx-workspace/downloads

# LINUX_LOCAL_SSTATE_DIR = Absolute Path where "sstate-cache" is located
# Used ONLY when LINUX_BUILD_TOOL := petalinux and LINUX_BUILD_SOURCES := local
export LINUX_LOCAL_SSTATE_DIR     := /tmp/plnx-workspace/sstate_aarch64_${REQUIRED_VERSION}/aarch64

# LINUX_ETH_CONFIG = DHCP or STATIC
export LINUX_ETH_CONFIG           := DHCP

################################################################
# Variables needed for the build process, PLEASE DO NOT MODIFY #
################################################################
export DEVICE_NAME                := xcvc1902-vsva2197-2MP-e-S
export PLATFORM_CUSTOM            := vck190_thin
export LINUX_MACHINE              := vck190-versal

LINUX_SRC_DIR                     := ./linux/src
LINUX_BUILD_DIR                   := ./linux/${LINUX_MACHINE}
LINUX_IMAGES                      := ${LINUX_BUILD_DIR}/images/linux
XPFM_EXPORT                       := ./platform/sw/build/${PLATFORM_CUSTOM}/export/${PLATFORM_CUSTOM}
BOARD_REPO                        := ./platform/hw/board_repo

################################################################
# Variables needed to shorten he build process when TRYING out #
# build-flow after platform modification                       #
################################################################

# LINUX_BUILD_SKIP = false or true
# Used to skip the linux-build ONLY when trying out platform CHANGES and want to see if the full build-flow is still working out
LINUX_BUILD_SKIP                  := false

##############
# Build Flow #
##############
.PHONY: clean help all version_check xsa linux petalinux yocto xpfm bif ip ps_apps vitis

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate everything for this design"
	$(ECHO) ""
	$(ECHO) "  make version_check"
	$(ECHO) "      checks out if the correct tools/versions are enabled"
	$(ECHO) ""
	$(ECHO) "  make ${BOARD_REPO}"
	$(ECHO) "      clones the XilinxBoardStore for es1 and pre-production boards"
	$(ECHO) ""
	$(ECHO) "  make xsa"
	$(ECHO) "      Builds a Vivado custom base HW platform using Pre-Synth flow"
	$(ECHO) "      To run full implementation platform, override using environment variable"
	$(ECHO) "      PRE_SYNTH = False"
	$(ECHO) ""
	$(ECHO) "  make linux"
	$(ECHO) "      Builds Linux related RootFS, U-boot, etc. using Petalinux or Yocto"
	$(ECHO) "      * Requires xsa rule to be completed"
	$(ECHO) ""
	$(ECHO) "  make xpfm"
	$(ECHO) "      Setups Vitis SW platform"
	$(ECHO) "      * Requires xsa and petalinux rules to be completed"
	$(ECHO) ""
	$(ECHO) "  make bif"
	$(ECHO) "      Copies Boot, devicetree and linux image to Vitis SW Platform"
	$(ECHO) "      * Requires petalinux to be completed"
	$(ECHO) ""
	$(ECHO) "  make ip"
	$(ECHO) "      Compile AIE, RTL and HLS kernels"
	$(ECHO) "      * Note, HLS kernel requires all steps for Vitis SW Platform to be completed"
	$(ECHO) ""
	$(ECHO) "  make ps_apps"
	$(ECHO) "      Compile PS applications"
	$(ECHO) "      * Requires all steps above"
	$(ECHO) ""
	$(ECHO) "  make vitis"
	$(ECHO) "      Extends and links the HW Platform with AIE, RTL and HLS kernels using Vitis v++"
	$(ECHO) "      Synthesize and Implements the complete design"
	$(ECHO) "      Packages the implemented design with the compiled PS apps"
	$(ECHO) "      Prepares and export SD card images"
	$(ECHO) "      * Requires all steps above"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all: version_check ${BOARD_REPO} xsa linux xpfm bif ip ps_apps vitis

all_targets:
	$(MAKE) all TARGET=hw
	$(MAKE) all TARGET=hw_emu

version_check:
ifndef VIVADO_VERSION
	$(ECHO) "[ERROR] Vivado NOT found, please setup Vivado correctly!"; exit 1
endif
ifeq (,$(findstring $(REQUIRED_VERSION),$(VIVADO_VERSION)))
	$(ECHO) "[ERROR] Vivado $(REQUIRED_VERSION) NOT found, please setup Vivado correctly!"; exit 1
endif
ifeq (${LINUX_BUILD_TOOL}, petalinux)
ifndef PETALINUX_VER
	$(ECHO) "[ERROR] Petalinux NOT found, please setup Petalinux correctly!"; exit 1
endif
ifeq (,$(findstring $(REQUIRED_VERSION),$(PETALINUX_VER)))
	$(ECHO) "[ERROR] Petalinux $(REQUIRED_VERSION) NOT found, please setup Petalinux correctly!"; exit 1
endif
endif
ifndef VITIS_VERSION
	$(ECHO) "[ERROR] Vitis NOT found, please setup Vitis correctly!"; exit 1
endif
ifeq (,$(findstring $(REQUIRED_VERSION),$(VITIS_VERSION)))
	$(ECHO) "[ERROR] Vitis $(REQUIRED_VERSION) NOT found, please setup Vitis correctly!"; exit 1
endif
	$(ECHO) "AMD/XILINX TOOLS & VERSION CHECK SUCCESSFUL"

${BOARD_REPO}:
	git clone https://github.com/Xilinx/XilinxBoardStore.git ${BOARD_REPO}

xsa:
	$(MAKE) xsa -C platform

linux:
ifeq (${LINUX_BUILD_SKIP}, false)
	$(MAKE) all -C linux/${LINUX_BUILD_TOOL}
else
	touch ${LINUX_IMAGES}/*
endif

xpfm:
	$(MAKE) xpfm -C platform

ip:
	$(MAKE) all -C ip

bif:
	$(MAKE) ${XPFM_EXPORT}/sw/Image

${XPFM_EXPORT}/sw/Image: ${LINUX_IMAGES}/* ${LINUX_SRC_DIR}/boot_custom.bif
	cp -f ${LINUX_IMAGES}/u-boot.elf             ${XPFM_EXPORT}/sw/u-boot.elf
	cp -f ${LINUX_IMAGES}/system.dtb             ${XPFM_EXPORT}/sw/system.dtb
	cp -f ${LINUX_IMAGES}/bl31.elf               ${XPFM_EXPORT}/sw/bl31.elf
	cp -f ${LINUX_IMAGES}/Image                  ${XPFM_EXPORT}/sw/Image
	cp -f ${LINUX_IMAGES}/rootfs.cpio.gz.u-boot  ${XPFM_EXPORT}/sw/rootfs.cpio.gz.u-boot
	cp -f ${LINUX_IMAGES}/boot.scr               ${XPFM_EXPORT}/sw/${PLATFORM_CUSTOM}/xrt/image/boot.scr
	cp -f ${LINUX_SRC_DIR}/boot_custom.bif       ${XPFM_EXPORT}/sw/${PLATFORM_CUSTOM}/boot/linux.bif

ps_apps:
	$(MAKE) all -C ps_apps

vitis:
	$(MAKE) all -C vitis

clean_vitis:
	$(MAKE) clean -C ip
	$(MAKE) clean -C ps_apps
	$(MAKE) clean -C vitis

ultraclean_vitis:
	$(MAKE) clean_vitis TARGET=hw
	$(MAKE) clean_vitis TARGET=hw_emu

clean:
	$(MAKE) clean -C platform
	$(MAKE) clean -C linux/${LINUX_BUILD_TOOL}
	$(MAKE) clean_vitis
	rm -rf ${BOARD_REPO}

ultraclean:
	$(MAKE) clean TARGET=hw
	$(MAKE) clean TARGET=hw_emu

