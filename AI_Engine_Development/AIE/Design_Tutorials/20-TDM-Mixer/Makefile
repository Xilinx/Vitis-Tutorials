#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

RELEASE=2025.1

PLATFORM_NAME              = xilinx_vck190_base_202510_1
PLATFORM_PATH              = ${PLATFORM_REPO_PATHS}

##############
# Build Flow #
##############
.PHONY: clean help

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate everything for this design"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all:	tdm_mixer

tdm_mixer:
	$(MAKE) all -C aie

clean:
	$(MAKE) clean -C aie
	@/bin/rm -rf .crashReporter*


