#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

.PHONY: run


PFM_NAME := xilinx_vck190_base_202310_1

run:
	make -C SingleKernel PFM_NAME=$(PFM_NAME) run
	make -C MultiKernel PFM_NAME=$(PFM_NAME) run
	make -C SingleStreamSSR PFM_NAME=$(PFM_NAME) run
	make -C DualStreamSSR PFM_NAME=$(PFM_NAME) run

clean:
	make -C SingleKernel clean
	make -C MultiKernel  clean
	make -C SingleStreamSSR  clean
	make -C DualStreamSSR  clean
