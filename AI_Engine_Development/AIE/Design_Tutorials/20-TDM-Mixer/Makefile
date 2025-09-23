#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

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
	@/bin/rm -rf matlab/vfs_work
	@/bin/rm -rf .crashReporter*


