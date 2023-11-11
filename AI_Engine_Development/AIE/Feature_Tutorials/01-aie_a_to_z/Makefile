#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

################################################################################
# Variables needed for AMD tools version checking, PLEASE DO NOT MODIFY #
################################################################################
REQUIRED_VERSION                   = 2023.2
VIVADO_VERSION                    := $(shell vivado -version 2>/dev/null | grep "vivado ")
VITIS_VERSION                     := $(shell vitis -version 2>/dev/null | grep "Vitis " | sed 's/\*//g')

###############################################
# Variables that may be changed to your needs #
###############################################

# TARGET = hw or hw_emu
export TARGET                     := hw_emu

##############
# Build Flow #
##############
.PHONY: clean help all vivado vitis_platform vitis_ip vitis ps_apps linux

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make vivado_platform"
	$(ECHO) "      Builds a Vivado custom base HW platform using Pre-Synth flow"
	$(ECHO) "      To run full implementation platform, override using environment variable"
	$(ECHO) ""
	$(ECHO) "  make vitis_platform"
	$(ECHO) "      Builds the Vitis platform"
	$(ECHO) "      * Depends on vivado_platform rule to be completed"
	$(ECHO) ""
	$(ECHO) "  make aie_app"
	$(ECHO) "      Creates and Builds the AI Engine component"
	$(ECHO) "      * Depends on vivado_platform and vitis_platform rule to be completed"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."
	
vivado_platform:
	$(MAKE) platform_classic -C vivado
	
vitis_platform:
	$(MAKE) pfm -C vitis
	
aie_app:
	$(MAKE) aie_app -C vitis

clean:
	$(MAKE) clean -C vivado
	$(MAKE) clean -C vitis
