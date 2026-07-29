#
# Copyright (C) 2021-2022, Xilinx, Inc. All rights reserved.
# Copyright (C) 2022-2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

# Top-level Makefile -- Calls sub-makefiles

ECHO                               = @echo

ifndef XILINX_VITIS
  $(error "Missing XILINX_VITIS environment variable pointing to tools please setup as environment or pass as argument")
else
  VITIS_SETUP                       := $(XILINX_VITIS)/settings64.sh
endif

################################################################################
# Variables needed for AMD tools version checking, PLEASE DO NOT MODIFY #
################################################################################
REQUIRED_VERSION                   := 2026.1

###############################################
# Variables that may be changed to your needs #
###############################################

# TARGET = hw or hw_emu
export TARGET                     := hw

# SW_BUILD_TOOL = petalinux or yocto
export SW_BUILD_TOOL              := yocto
export YOCTO_FLOW                 := edf

# PREBUILD_EMBEDDED = true or false
# Prebuild embedded to reduce Yocto iterations by preparing a custom base platform
# for preparing EDF artifacts, bootbin extracted, board and user dtsi. This provide a
# foundation for extensible Vivado platform and adding Vitis signal processing kernels
# benefiting from automation with v++.
export PREBUILD_EMBEDDED          := false

ifeq (${SW_BUILD_TOOL}, petalinux)
  ifndef PETALINUX
    $(error "Missing PETALINUX environment pointing to tools, please setup as environment or pass as argument")
  else
    PETALINUX_SETUP                   := $(PETALINUX)/settings.sh
  endif
endif

# ILA_EN = 0 (Disabled) or 1 (Enabled)
export ILA_EN                     := 0

# BOARD_NAME = vck190 or vek280 or vek280_es1
export BOARD_NAME                 := vck190

###########################################################
# Linux/Yocto Variables that may be changed to your needs #
###########################################################

# LINUX_TMP_DIR = Any dir which is not NFS
export TMP_DIR                    := tmp
export LINUX_TMP_DIR              := /${TMP_DIR}/${USER}/${REQUIRED_VERSION}/${BOARD_NAME}/${SW_BUILD_TOOL}

# LINUX_ETH_CONFIG = DHCP or STATIC
export LINUX_ETH_CONFIG           := DHCP

################################################################
# Variables needed for the build process, PLEASE DO NOT MODIFY #
################################################################
export PLATFORM_CUSTOM            := ${BOARD_NAME}_thin
export LINUX_MACHINE              := my-${BOARD_NAME}-versal

ifeq (${BOARD_NAME}, vck190)
  export DEVICE_NAME                := xcvc1902-vsva2197-2MP-e-S
  export PLATFORM_PRE_BUILD         := xilinx_vck190_base_202410_1
  export SEG_CONF                   := false
else ifeq (${BOARD_NAME}, vck190seg)
  export DEVICE_NAME                := xcvc1902-vsva2197-2MP-e-S
  export PLATFORM_PRE_BUILD         := xilinx_vck190_base_202410_1
  export SEG_CONF                   := true
else ifeq (${BOARD_NAME}, vek280)
  export DEVICE_NAME                := xcve2802-vsvh1760-2MP-e-S
  export PLATFORM_PRE_BUILD         := xilinx_vek280_base_202410_1
  export SEG_CONF                   := false
else ifeq (${BOARD_NAME}, vek385)
  export DEVICE_NAME                := xc2ve3858-ssva2112-2MP-e-S
  export SEG_CONF                   := true
else ifeq (${BOARD_NAME}, vrk160)
  export DEVICE_NAME                := xcvr1602-vsva2488-2MP-e-S-es1
  export SEG_CONF                   := true
else
  export DEVICE_NAME                := xcvc1902-vsva2197-2MP-e-S
  export PLATFORM_PRE_BUILD         := xilinx_vck190_base_202410_1
  export SEG_CONF                   := false
endif
export PART                       := $(DEVICE_NAME)

XSA_DIR                           := ./vivado/build/xsa_platform
XSA_FILE                          := ${XSA_DIR}/${PLATFORM_CUSTOM}.xsa


##############
# Build Flow #
##############
.PHONY: clean help all vss vivado_platform vitis_ip vma_export vivado_fixed sdtgen linux ps_apps package sdcard_image

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate everything for this design."
	$(ECHO) ""
	$(ECHO) "  make version_check"
	$(ECHO) "      checks out if the correct tools/versions are enabled."
	$(ECHO) ""
	$(ECHO) "  make vss"
	$(ECHO) "      Builds a Vitis Subsystem using selected part with the AI Engine and PL Kernels in vss/ip folder."
	$(ECHO) "      Can be used for sandbox testing of AI Engine, HLS and RTL IP before deploying to a Vivado platform."
	$(ECHO) "      The VSS structure is setup with examples to leverage Vitis Functional Simulation and VSS Simulation to verify the IP prior to platform integration."
	$(ECHO) "      See vss/matlab/README.md and vss/python/README.md for standalone VFS testing of VSS IP."
	$(ECHO) ""
	$(ECHO) "  make vss_cosim"
	$(ECHO) "      [Optional] Builds a Vivado simulation project and simulates Vitis Subsystem with XSIM."
	$(ECHO) "      * Depends on vss rule to be completed."
	$(ECHO) ""
	$(ECHO) "  make vivado_base"
	$(ECHO) "      [Optional] Builds a Vivado custom base HW platform that is used to prebuild EDF artifacts"
	$(ECHO) "      To run full implementation platform, override using environment variable"
	$(ECHO) ""
	$(ECHO) "  make vivado_platform"
	$(ECHO) "      Builds a Vivado custom extensible HW platform using Pre-Synth flow"
	$(ECHO) "      If prebuild embedded flow is used, the bootbin extracted, domains.yaml, and board DTSI"
	$(ECHO) "      are imported as zocl.dtsi zip container which is used later by v++ package."
	$(ECHO) ""
	$(ECHO) "  make vitis_ip"
	$(ECHO) "      [Optional] Compile RTL and HLS kernel additions to platform linked by Vitis."
	$(ECHO) "      * Will be called automatically by vma_export or vitis_flow_impl"
	$(ECHO) ""
	$(ECHO) "  make vitis_flow_impl"
	$(ECHO) "      [Alternative flow] Links and implements the fixed XSA using Vitis"
	$(ECHO) "      * Depends on vss and vivado_platform rules to be completed"
	$(ECHO) ""
	$(ECHO) "  make vma_export"
	$(ECHO) "      Runs vitis_ip rule if needed and call Vitis to link the design for export to Vivado."
	$(ECHO) "      * Depends on vss and vivado_platform rules to be completed"
	$(ECHO) ""
	$(ECHO) "  make vivado_fixed"
	$(ECHO) "      Imports the VMA into Vivado and run complete synthesis and implementation."
	$(ECHO) "      * Depends on vma_export rule to be completed"
	$(ECHO) ""
	$(ECHO) "  make sdtgen"
	$(ECHO) "      Generates the devicetree when building the vivado platform and using the linux pre-builds"
	$(ECHO) "      * Depends on either vivado_fixed or vitis_flow_impl rule to be completed"
	$(ECHO) ""
	$(ECHO) "  make linux"
	$(ECHO) "      Builds Linux related RootFS, U-boot, etc. using Petalinux or Yocto/EDF"
	$(ECHO) "      * Depends on sdtgen rule to be completed"
	$(ECHO) ""
	$(ECHO) "  make ps_apps"
	$(ECHO) "      Compile PS applications"
	$(ECHO) "      * Depends on linux rule to be completed"
	$(ECHO) ""
	$(ECHO) "  make package"
	$(ECHO) "      Packages the implemented design with the builded linux & PS apps"
	$(ECHO) "      Prepares and export SD card images"
	$(ECHO) "      * Depends on all previous rules to be completed"
	$(ECHO) ""
	$(ECHO) "  make sdcard_image"
	$(ECHO) "      Use wic tools to copy BOOT.bin, xclbin and host apps to the SD card partitions in the wic image"
	$(ECHO) "      Once copied, the Yocto deploy folder is copied from TMPDIR location to the project linux/${LINUX_MACHINE}/images/linux folder"
	$(ECHO) "      * Depends on all previous rules to be completed"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."


all:
	$(MAKE) vss
ifeq (${PREBUILD_EMBEDDED}, true)
	$(MAKE) vivado_base
	$(MAKE) sdtgen
	$(MAKE) linux
	$(MAKE) zocl_dtsi
endif
	$(MAKE) vivado_platform
	$(MAKE) vitis_ip
	$(MAKE) vma_export
	$(MAKE) vivado_fixed
ifeq (${PREBUILD_EMBEDDED}, false)
	$(MAKE) sdtgen
	$(MAKE) linux
endif
	$(MAKE) ps_apps
	$(MAKE) package
	$(MAKE) sdcard_image


.PHONY: all_targets
all_targets:
	$(MAKE) all TARGET=hw
	$(MAKE) all TARGET=hw_emu

.PHONY: version_check
version_check:
ifndef XILINX_VITIS
	$(error Environment variable XILINX_VITIS not declared, please set to Vitis install directory)
else
ifeq (,$(findstring $(REQUIRED_VERSION), $(shell . ${VITIS_SETUP}; vivado -version 2>/dev/null | grep "vivado ")))
	$(ECHO) "[ERROR] Vivado $(REQUIRED_VERSION) NOT found, please setup Vivado correctly!"; exit 1
endif
ifeq (,$(findstring $(REQUIRED_VERSION), $(shell . ${VITIS_SETUP}; vitis -version 2>/dev/null | grep "Vitis " | sed 's/\*//g')))
	$(ECHO) "[ERROR] Vitis $(REQUIRED_VERSION) NOT found, please setup Vitis correctly!"; exit 1
endif
endif
ifeq (${SW_BUILD_TOOL}, petalinux)
ifndef PETALINUX
	$(error Environment variable PETALINUX not declared, please set to Petalinux install directory)
endif
endif
	$(ECHO) "AMD TOOLS & VERSION CHECK SUCCESSFUL"

.PHONY: vivado_base
vivado_base:
	. $(VITIS_SETUP) && $(MAKE) platform_base -C vivado

vivado_platform:
	. $(VITIS_SETUP) && $(MAKE) platform_classic -C vivado

linux:
ifeq (${SW_BUILD_TOOL}, petalinux)
	. $(PETALINUX_SETUP) && \
  $(MAKE) all -C linux/${SW_BUILD_TOOL}
else
	$(MAKE) all -C linux/${SW_BUILD_TOOL}
endif

.PHONY: update_zocl
update_zocl:
	$(MAKE) update_zocl -C linux/${SW_BUILD_TOOL}

sdtgen:
	. $(VITIS_SETUP) && \
  $(MAKE) all -C linux/sdtgen

.PHONY: vitis_platform
vitis_platform:
	. $(VITIS_SETUP) && \
  $(MAKE) platform -C vitis

vss:
	. $(VITIS_SETUP) && \
  $(MAKE) vss_ip -C vss && \
  $(MAKE) vss_link -C vss

.PHONY: vss_cosim
vss_cosim:
	. $(VITIS_SETUP) && \
  $(MAKE) vss_cosim -C vss

vitis_ip:
	. $(VITIS_SETUP) && \
  $(MAKE) vitis_ip -C vitis

ps_apps:
	$(MAKE) all -C ps_apps/linux

vma_export:
	. $(VITIS_SETUP) && \
  $(MAKE) vitis_ip -C vitis && \
  $(MAKE) vma_link -C vitis

.PHONY: vitis_flow_impl
vitis_flow_impl:
	. $(VITIS_SETUP) &&  \
  $(MAKE) vitis_ip -C vitis && \
  $(MAKE) link -C vitis

vivado_fixed:
	. $(VITIS_SETUP) && $(MAKE) vivado_fixed -C vivado

package:
ifeq (${SW_BUILD_TOOL}, petalinux)
	. $(VITIS_SETUP) && \
    $(MAKE) platform -C vitis && \
    $(MAKE) package_linux -C vitis
else
  ifeq (${SEG_CONF}, true)
		. $(VITIS_SETUP) && \
    $(MAKE) package_seg -C vitis
  else
		. $(VITIS_SETUP) && \
    $(MAKE) package_zocl -C vitis
  endif
endif

.PHONY: package_edf
package_edf:
	. $(VITIS_SETUP) && $(MAKE) package_edf -C vitis

.PHONY: edf_sysroot
edf_sysroot:
	$(MAKE) update_sysroot -C linux/yocto

.PHONY: edf_bootbin
edf_bootbin:
	$(MAKE) update_bootbin -C linux/yocto

sdcard_image:
ifeq (${SW_BUILD_TOOL}, petalinux)
	$(ECHO) "Skipping, already completed by v++ package."
else
	$(MAKE) sdcard_image -C linux/yocto
endif

.PHONY: sdcard_app
sdcard_app:
	$(MAKE) update_wic_apps -C linux/yocto

.PHONY: update_bmap
update_bmap:
	$(MAKE) update_bmap -C linux/yocto

.PHONY: clean_vitis
clean_vitis:
	$(MAKE) clean -C ps_apps/linux
	$(MAKE) clean -C vitis

.PHONY: ultraclean_vitis
ultraclean_vitis:
	$(MAKE) clean_vitis TARGET=hw
	$(MAKE) clean_vitis TARGET=hw_emu

clean:
	$(MAKE) clean -C vss
	$(MAKE) clean -C vitis
	$(MAKE) clean -C vivado
	$(MAKE) clean -C linux/${SW_BUILD_TOOL}
	$(MAKE) clean -C linux/sdtgen
	$(MAKE) clean_vitis

.PHONY: ultraclean
ultraclean:
	$(MAKE) clean TARGET=hw
	$(MAKE) clean TARGET=hw_emu
