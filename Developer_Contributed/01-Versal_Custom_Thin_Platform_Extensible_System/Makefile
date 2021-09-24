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

ECHO                     = @echo

REQUIRED_VERSION         = 2021.2
VIVADO_VERSION          := $(shell vivado -version 2>/dev/null | grep "Vivado ")
VITIS_VERSION           := $(shell vitis -version 2>/dev/null | grep "Vitis " | sed 's/\*//g')
PETALINUX_VERSION       := $(shell which petalinux-build)

export PLATFORM_CUSTOM  := vck190_thin
export DEVICE_NAME      := xcvc1902-vsva2197-2MP-e-S-es1
export XSA              := ${PLATFORM_CUSTOM}.xsa
export XSA_PATH         := $(shell pwd)/platform/hw/build/
export TARGET           := hw
export PFM_EXPORT       := ./platform/sw/build/${PLATFORM_CUSTOM}/export/${PLATFORM_CUSTOM}
export BASE_PFM         := ${PFM_EXPORT}/${PLATFORM_CUSTOM}.xpfm

BOARD_REPO              := platform/hw/board_repo

.PHONY: clean help all version_check xsa petalinux xpfm bif ip ps_apps vitis

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
	$(ECHO) "  make petalinux"
	$(ECHO) "      Builds Linux related RootFS, U-boot, etc. using Petalinux"
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

all: version_check ${BOARD_REPO} xsa petalinux xpfm bif ip ps_apps vitis

version_check:
ifndef VIVADO_VERSION
	$(ECHO) "Vivado: not found"; exit 1
endif
ifeq (,$(findstring $(REQUIRED_VERSION),$(VIVADO_VERSION)))
	$(ECHO) "Vivado: $(REQUIRED_VERSION) not found"; exit 1
endif
ifndef PETALINUX_VERSION
	$(ECHO) "Petalinux: not found"; exit 1
endif
ifeq (,$(findstring $(REQUIRED_VERSION),$(PETALINUX_VERSION)))
	$(ECHO) "Petalinux: $(REQUIRED_VERSION) not found"; exit 1
endif
ifndef VITIS_VERSION
	$(ECHO) "Vitis: not found"; exit 1
endif
ifeq (,$(findstring $(REQUIRED_VERSION),$(VITIS_VERSION)))
	$(ECHO) "Vitis: $(REQUIRED_VERSION) not found"; exit 1
endif
	$(ECHO) "VERSION CHECK SUCCESSFUL"

${BOARD_REPO}:
	git clone https://github.com/Xilinx/XilinxBoardStore.git ${BOARD_REPO}

xsa:
	$(MAKE) xsa -C platform

petalinux:
	$(MAKE) all -C petalinux

xpfm:
	$(MAKE) xpfm -C platform

ip:
	$(MAKE) all -C ip

bif:
	cp -f ./petalinux/linux/images/linux/u-boot.elf   ${PFM_EXPORT}/sw
	cp -f ./petalinux/linux/images/linux/system.dtb   ${PFM_EXPORT}/sw
	cp -f ./petalinux/linux/images/linux/bl31.elf     ${PFM_EXPORT}/sw
	cp -f ./petalinux/linux/images/linux/boot.scr     ${PFM_EXPORT}/sw/${PLATFORM_CUSTOM}/xrt/image
	cp -f ./petalinux/src/boot_custom.bif             ${PFM_EXPORT}/sw/${PLATFORM_CUSTOM}/boot/linux.bif

ps_apps:
	$(MAKE) all -C ps_apps

vitis:
	$(MAKE) all -C vitis

clean:
	$(MAKE) clean -C platform
	$(MAKE) clean -C petalinux
	$(MAKE) clean -C ip
	$(MAKE) clean -C ps_apps
	$(MAKE) clean -C vitis
	rm -rf ${BOARD_REPO}
  

