#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

# Top-level Makefile -- Calls sub-makefiles

ECHO                               = @echo

################################################################################
# Variables needed for AMD tools version checking, PLEASE DO NOT MODIFY #
################################################################################
REQUIRED_VERSION                   = 2023.1
VIVADO_VERSION                    := $(shell vivado -version 2>/dev/null | grep "vivado ")
VITIS_VERSION                     := $(shell vitis -version 2>/dev/null | grep "Vitis " | sed 's/\*//g')

###############################################
# Variables that may be changed to your needs #
###############################################

# TARGET = hw or hw_emu
export TARGET                     := hw

# SW_BUILD_TOOL = petalinux or yocto
export SW_BUILD_TOOL              := petalinux

# XPFM_PRE_BUILDS = false (Build xpfm) or true (Use xpfm pre-builds)
export XPFM_PRE_BUILDS            := false

# LINUX_PRE_BUILDS = false (Build linux) or true (Use linux pre-builds)
export LINUX_PRE_BUILDS           := false

# ILA_EN = 0 (Disabled) or 1 (Enabled)
export ILA_EN                     := 0

###########################################################
# Linux/Yocto Variables that may be changed to your needs #
###########################################################

# LINUX_TMP_DIR = Any dir which is not NFS
export LINUX_TMP_DIR              := /tmp/${USER}/${REQUIRED_VERSION}/${SW_BUILD_TOOL}

# LINUX_YOCTO_ROOT = Directory where yocto tools are installed
# Used ONLY when SW_BUILD_TOOL := yocto!
export LINUX_YOCTO_ROOT           := ${HOME}/bin

# LINUX_BUILD_SOURCE = network or local
# Used ONLY when SW_BUILD_TOOL := petalinux!
export LINUX_BUILD_SOURCES        := network

# LINUX_DOWNLOADS_DIR = Absolute Path where "downloads" is located
# Used ONLY when SW_BUILD_TOOL := petalinux and LINUX_BUILD_SOURCES := local
export LINUX_LOCAL_DOWNLOADS_DIR  := /tmp/plnx-workspace/downloads

# LINUX_LOCAL_SSTATE_DIR = Absolute Path where "sstate-cache" is located
# Used ONLY when SW_BUILD_TOOL := petalinux and LINUX_BUILD_SOURCES := local
export LINUX_LOCAL_SSTATE_DIR     := /tmp/plnx-workspace/sstate_aarch64_${REQUIRED_VERSION}/aarch64

# LINUX_ETH_CONFIG = DHCP or STATIC
export LINUX_ETH_CONFIG           := DHCP

################################################################
# Variables needed for the build process, PLEASE DO NOT MODIFY #
################################################################
export DEVICE_NAME                := xcvc1902-vsva2197-2MP-e-S
export PLATFORM_CUSTOM            := vck190_thin
export PLATFORM_PRE_BUILD         := xilinx_vck190_base_202310_1
export LINUX_MACHINE              := vck190-versal

LINUX_SRC_DIR                     := ./linux/src
LINUX_BUILD_DIR                   := ./linux/${LINUX_MACHINE}
LINUX_DTB_DIR                     := ./linux/dtg/dts

XSA_DIR                           := ./vivado/build/xsa_platform
XSA_FILE                          := ${XSA_DIR}/${PLATFORM_CUSTOM}.xsa
XSA_PRE_BUILD                     := ${PLATFORM_REPO_PATHS}/${PLATFORM_PRE_BUILD}/hw/hw.xsa

XPFM_EXPORT                       := ./vitis/xpfm_export/build/platform/${PLATFORM_CUSTOM}/export/${PLATFORM_CUSTOM}
XPFM_EXPORT_SW                    := ${XPFM_EXPORT}/sw
XPFM_PRE_BUILD                    := ${PLATFORM_REPO_PATHS}/${PLATFORM_PRE_BUILD}

ifeq (${LINUX_PRE_BUILDS}, false)
  LINUX_SRC_IMAGES                := ${LINUX_BUILD_DIR}/images/linux
else
  LINUX_SRC_IMAGES                := ${SDKTARGETSYSROOT}/../..
endif

ifeq (${XPFM_PRE_BUILDS}, false)
  XPFM_EXPORT_BOOT                := ${XPFM_EXPORT}/sw/${PLATFORM_CUSTOM}/boot
  XPFM_EXPORT_XRT_IMAGE           := ${XPFM_EXPORT}/sw/${PLATFORM_CUSTOM}/xrt/image
else
  XPFM_EXPORT_BOOT                := ${XPFM_EXPORT}/sw/${PLATFORM_PRE_BUILD}/boot
  XPFM_EXPORT_XRT_IMAGE           := ${XPFM_EXPORT}/sw/${PLATFORM_PRE_BUILD}/xrt/image
endif

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
.PHONY: clean help all vivado vitis_platform vitis_ip vitis ps_apps linux

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate everything for this design"
	$(ECHO) ""
	$(ECHO) "  make version_check"
	$(ECHO) "      checks out if the correct tools/versions are enabled"
	$(ECHO) ""
	$(ECHO) "  make vivado_platform"
	$(ECHO) "      Builds a Vivado custom base HW platform using Pre-Synth flow"
	$(ECHO) "      To run full implementation platform, override using environment variable"
	$(ECHO) "      PRE_SYNTH = False"
	$(ECHO) ""
	$(ECHO) "  make vitis_platform"
	$(ECHO) "      Builds the Vitis platform"
	$(ECHO) "      * Depends on vivado_platform rule to be completed"
	$(ECHO) ""
	$(ECHO) "  make linux"
	$(ECHO) "      Builds Linux related RootFS, U-boot, etc. using Petalinux or Yocto"
	$(ECHO) "      * Depends on vivado_platform rule to be completed"
	$(ECHO) ""
	$(ECHO) "  make dtg"
	$(ECHO) "      Generates the devicetree when building the vivado platform and using the linux pre-builds"
	$(ECHO) "      * Depends on vivado_platform rule to be completed"
	$(ECHO) ""
	$(ECHO) "  make bif"
	$(ECHO) "      Copies Boot, devicetree and linux image to Vitis SW Platform"
	$(ECHO) "      * Depends on linux rule to be completed"
	$(ECHO) ""
	$(ECHO) "  make vitis_ip"
	$(ECHO) "      Compile AIE, RTL and HLS kernels"
	$(ECHO) "      * Depends on vitis_platform rule to be completed"
	$(ECHO) ""
	$(ECHO) "  make full_impl"
	$(ECHO) "      Extends and links the HW Platform with AIE, RTL and HLS kernels using Vitis v++ linker"
	$(ECHO) "      Synthesize and Implements the complete design"
	$(ECHO) "      * Depends on vitis_ip rule to be completed"
	$(ECHO) ""
	$(ECHO) "  make ps_apps"
	$(ECHO) "      Compile PS applications"
	$(ECHO) "      * Depends on linux and vitis_ip rule to be completed"
	$(ECHO) ""
	$(ECHO) "  make package"
	$(ECHO) "      Packages the implemented design with the builded linux & PS apps"
	$(ECHO) "      Prepares and export SD card images"
	$(ECHO) "      * Depends on all previous rules to be completed"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."


all: version_check vivado_platform vitis_platform linux dtg bif vitis_ip full_impl ps_apps package

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
ifeq (${LINUX_PRE_BUILDS}, true)
ifndef SDKTARGETSYSROOT
	$(ECHO) "[ERROR] SDKTARGETSYSROOT NOT found, please setup SDKTARGETSYSROOT correctly!"; exit 1
endif
else
ifeq (${SW_BUILD_TOOL}, petalinux)
ifndef PETALINUX_VER
	$(ECHO) "[ERROR] Petalinux NOT found, please setup Petalinux correctly!"; exit 1
endif
ifeq (,$(findstring $(REQUIRED_VERSION),$(PETALINUX_VER)))
	$(ECHO) "[ERROR] Petalinux $(REQUIRED_VERSION) NOT found, please setup Petalinux correctly!"; exit 1
endif
endif
endif
ifndef VITIS_VERSION
	$(ECHO) "[ERROR] Vitis NOT found, please setup Vitis correctly!"; exit 1
endif
ifeq (,$(findstring $(REQUIRED_VERSION),$(VITIS_VERSION)))
	$(ECHO) "[ERROR] Vitis $(REQUIRED_VERSION) NOT found, please setup Vitis correctly!"; exit 1
endif
ifeq (${XPFM_PRE_BUILDS}, true)
ifndef PLATFORM_REPO_PATHS
	$(ECHO) "[ERROR] PLATFORM_REPO_PATHS NOT found, please setup PLATFORM_REPO_PATHS correctly!"; exit 1
endif
ifeq (${TARGET}, hw_emu)
	$(ECHO) "[ERROR] XPFM_PRE_BUILDS = true and TARGET = hw_emu is NOT supported!"; exit 1
endif
endif
	$(ECHO) "AMD TOOLS & VERSION CHECK SUCCESSFUL"

vivado_platform:
ifeq (${XPFM_PRE_BUILDS}, true)
	$(MAKE) xsa_pre_build
else
	$(MAKE) platform_classic -C vivado
endif

xsa_pre_build: ${XSA_FILE}
${XSA_FILE}:
	mkdir -p  ${XSA_DIR}
	ln    -s  ${XSA_PRE_BUILD} ${XSA_FILE}

linux:
ifeq (${LINUX_PRE_BUILDS}, true)
	$(MAKE) linux_pre_build
else ifeq (${LINUX_BUILD_SKIP}, false)
	$(MAKE) all -C linux/${SW_BUILD_TOOL}
else
	touch ${LINUX_SRC_IMAGES}/*
endif

linux_pre_build: ./linux/sysroot
./linux/sysroot:
	ln -s $(abspath ${SDKTARGETSYSROOT}/../..) ./linux/sysroot	

dtg:
ifeq (${XPFM_PRE_BUILDS}, false)
ifeq (${LINUX_PRE_BUILDS}, true)
	$(MAKE) all -C linux/dtg
endif
endif

vitis_platform:
ifeq (${XPFM_PRE_BUILDS}, true)
	$(MAKE) vitis_platform_pre_build
else 
	$(MAKE) platform -C vitis/xpfm_export
endif

vitis_platform_pre_build: ${XPFM_EXPORT}
${XPFM_EXPORT}:
	mkdir -p  ${XPFM_EXPORT}
	cp    -rf ${XPFM_PRE_BUILD}/*                           ${XPFM_EXPORT}
	mv    -f  ${XPFM_EXPORT}/${PLATFORM_PRE_BUILD}.xpfm     ${XPFM_EXPORT}/${PLATFORM_CUSTOM}.xpfm
	mv    -f  ${XPFM_EXPORT_BOOT}/*                         ${XPFM_EXPORT_SW}
	rm    -rf ${XPFM_EXPORT_SW}/*.bif

bif: 
	$(MAKE) ${XPFM_EXPORT_BOOT}/linux.bif
ifeq (${LINUX_PRE_BUILDS}, false)
	$(MAKE) ${XPFM_EXPORT_SW}/rootfs.cpio.gz.u-boot
else 
	$(MAKE) ${XPFM_EXPORT_SW}/rootfs.ext4
ifeq (${XPFM_PRE_BUILDS}, false)
	$(MAKE) ${XPFM_EXPORT_SW}/system.dtb
endif
endif

${XPFM_EXPORT_BOOT}/linux.bif: ${LINUX_SRC_DIR}/boot_custom.bif
	cp -f ${LINUX_SRC_DIR}/boot_custom.bif           ${XPFM_EXPORT_BOOT}/linux.bif

${XPFM_EXPORT_SW}/rootfs.cpio.gz.u-boot: ${LINUX_SRC_IMAGES}/rootfs.cpio.gz.u-boot
	cp -f ${LINUX_SRC_IMAGES}/system.dtb             ${XPFM_EXPORT_SW}/system.dtb
	cp -f ${LINUX_SRC_IMAGES}/u-boot.elf             ${XPFM_EXPORT_SW}/u-boot.elf
	cp -f ${LINUX_SRC_IMAGES}/bl31.elf               ${XPFM_EXPORT_SW}/bl31.elf
	cp -f ${LINUX_SRC_IMAGES}/Image                  ${XPFM_EXPORT_SW}/Image
	cp -f ${LINUX_SRC_IMAGES}/rootfs.cpio.gz.u-boot  ${XPFM_EXPORT_SW}/rootfs.cpio.gz.u-boot
	cp -f ${LINUX_SRC_IMAGES}/boot.scr               ${XPFM_EXPORT_XRT_IMAGE}/boot.scr

${XPFM_EXPORT_SW}/rootfs.ext4: ${LINUX_SRC_IMAGES}/rootfs.ext4
	cp -f ${LINUX_SRC_IMAGES}/Image                  ${XPFM_EXPORT_SW}/Image
	cp -f ${LINUX_SRC_IMAGES}/rootfs.ext4            ${XPFM_EXPORT_SW}/rootfs.ext4
	cp -f ${LINUX_SRC_IMAGES}/boot.scr               ${XPFM_EXPORT_XRT_IMAGE}/boot.scr

${XPFM_EXPORT_SW}/system.dtb: ${LINUX_DTB_DIR}/system-top.dtb
	cp -f ${LINUX_DTB_DIR}/system-top.dtb            ${XPFM_EXPORT_SW}/system.dtb
	cp -f ${LINUX_SRC_IMAGES}/u-boot.elf             ${XPFM_EXPORT_SW}/u-boot.elf
	cp -f ${LINUX_SRC_IMAGES}/bl31.elf               ${XPFM_EXPORT_SW}/bl31.elf

vitis_ip:
	$(MAKE) all -C vitis/ip

ps_apps:
	$(MAKE) all -C ps_apps/linux

full_impl:
	$(MAKE) link -C vitis

package:
	$(MAKE) package_linux -C vitis

clean_vitis:
	$(MAKE) clean -C vitis/xpfm_export
	$(MAKE) clean -C vitis/ip
	$(MAKE) clean -C ps_apps/linux
	$(MAKE) clean -C vitis

ultraclean_vitis:
	$(MAKE) clean_vitis TARGET=hw
	$(MAKE) clean_vitis TARGET=hw_emu

clean:
	$(MAKE) clean -C vivado
	$(MAKE) clean -C linux/${SW_BUILD_TOOL}
	$(MAKE) clean -C linux/dtg
	$(MAKE) clean_vitis

ultraclean:
	$(MAKE) clean TARGET=hw
	$(MAKE) clean TARGET=hw_emu

