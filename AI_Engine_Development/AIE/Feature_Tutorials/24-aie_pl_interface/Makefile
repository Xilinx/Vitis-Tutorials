#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

################################################################################
# Variables needed for AMD tools version checking, PLEASE DO NOT MODIFY #
################################################################################
REQUIRED_VERSION                  := 2024.1
VIVADO_VERSION                    := $(shell vivado -version 2>/dev/null | grep "vivado ")
VITIS_VERSION                     := $(shell vitis -version 2>/dev/null | grep "Vitis " | sed 's/\*//g')

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
	$(ECHO) "  make vitis_project"
	$(ECHO) "      Generates the full Vitis project which can be opened in Vitis IDE"
	$(ECHO) ""
	$(ECHO) "  make all"
	$(ECHO) "      Generate the full project
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

vivado_platform:
	$(MAKE) platform -C Vivado

vitis_platform:
	$(MAKE) pfm -C Vitis

vitis_project:
	$(MAKE) all -C Vitis

all: vivado_platform vitis_project

all_designs:
	$(MAKE) all
	$(MAKE) all RTL_OUT_BD=1
	$(MAKE) all INCLUDE_ILAS=1
	$(MAKE)	all AXI4S_BROADCAST=1


