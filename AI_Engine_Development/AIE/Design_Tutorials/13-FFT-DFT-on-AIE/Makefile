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

all:	all_fft

all_fft:
	$(MAKE) all -C dft16 all
	$(MAKE) all -C fft32_dsplib all
	$(MAKE) all -C fft32_dsplib_split  all
	$(MAKE) all -C fft32_dsplib_ssr all
	$(MAKE) all -C fft32_r2 all

clean:
	$(MAKE) clean -C dft16
	$(MAKE) clean -C fft32_dsplib
	$(MAKE) clean -C fft32_dsplib_split
	$(MAKE) clean -C fft32_dsplib_ssr
	$(MAKE) clean -C fft32_r2
	@/bin/rm -rf .crashReporter*

