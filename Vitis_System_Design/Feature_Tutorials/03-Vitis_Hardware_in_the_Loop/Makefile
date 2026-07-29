#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Richard Buz
#
# ============================================================
# Top-level Makefile: Vitis Hardware in the Loop tutorial
#
# Provides tutorial verification by invoking the lower-level
# Makefile and confirming it executes without error.
#
# Usage:
#   make all     # invoke the lower-level build
#   make clean   # remove generated files
#
# Required environment:
#   source <Vitis_install>/settings64.sh
# ============================================================

ECHO := @echo

.PHONY: all clean help

all:
	$(MAKE) all -C channelizer_vss

clean:
	$(MAKE) clean -C channelizer_vss

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Invoke the lower-level build for verification."
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Remove all generated files."
