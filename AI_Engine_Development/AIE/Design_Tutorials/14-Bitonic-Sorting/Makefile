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

all:	all_sort

all_sort:
	$(MAKE) all -C bitonic_fp16
	$(MAKE) all -C bitonic_fp1024
	$(MAKE) all -C sortcpp_fp16
	$(MAKE) all -C sortcpp_fp1024

clean:
	$(MAKE) clean -C bitonic_fp16
	$(MAKE) clean -C bitonic_fp1024
	$(MAKE) clean -C sortcpp_fp16
	$(MAKE) clean -C sortcpp_fp1024
	@/bin/rm -rf .crashReporter*

