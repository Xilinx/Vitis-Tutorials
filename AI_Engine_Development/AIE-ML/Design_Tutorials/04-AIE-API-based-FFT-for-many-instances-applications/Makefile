#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#
# Author: Donato Carlo Giorgio

ECHO = @echo
################################################################################
# Variables needed for AMD tools version checking, PLEASE DO NOT MODIFY #
################################################################################
REQUIRED_VERSION                   = 2024.1
VITIS_VERSION                     := $(shell vitis -version 2>/dev/null | grep "Vitis " | sed 's/\*//g')

##############
# Build Flow #
##############
.PHONY: clean help all 3D_buf 4D_buf


help:
	$(ECHO) ""
	$(ECHO) "Makefile Usage:"
	$(ECHO) ""
	$(ECHO) ""
	$(ECHO) "  make 3D_buf"
	$(ECHO) "      Creates and Builds the three-dimensional buffer implementation"
	$(ECHO) ""
	$(ECHO) "  make 4D_buf"
	$(ECHO) "      Creates and Builds the four-dimensional buffer implementation"
	$(ECHO) ""
	$(ECHO) "  make run"
	$(ECHO) "      Generate the full project"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."
	$(ECHO) ""
	$(ECHO) ""
	
3D_buf:
	vitis -s Step1_3Dbuf.py

4D_buf:
	vitis -s Step2_4Dbuf.py
	
run:
	vitis -s Step1_3Dbuf.py
	vitis -s Step2_4Dbuf.py
	
clean:
	rm -rf AIEML_API_FFT_Tutorial