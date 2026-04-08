#
# Copyright (C) Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

.PHONY: run


PFM_NAME := xilinx_vck190_base_202520_1

run:
	make -C SingleKernel PFM_NAME=$(PFM_NAME) run
	make -C MultiKernel PFM_NAME=$(PFM_NAME) run
	make -C SingleStreamSSR PFM_NAME=$(PFM_NAME) run
	make -C DualStreamSSR PFM_NAME=$(PFM_NAME) run

performance:
	cd SingleKernel ; make throughput
	cd MultiKernel ; make throughput
	cd SingleStreamSSR ; make throughput
	cd DualStreamSSR ; make throughput


clean:
	make -C SingleKernel clean
	make -C MultiKernel  clean
	make -C SingleStreamSSR  clean
	make -C DualStreamSSR  clean
	make -C DualSSR16_hw clean
	rm -rf Utils/__pycache__
