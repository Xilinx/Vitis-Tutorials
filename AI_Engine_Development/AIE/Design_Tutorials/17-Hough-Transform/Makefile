#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

ECHO                               = @echo

##############
# Build Flow #
##############
.PHONY: clean help

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to compile and simulate all designs"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all:	all_hough

all_hough:
	$(MAKE) all -C aie

clean:
	$(MAKE) clean -C aie
	@/bin/rm -rf .crashReporter*

