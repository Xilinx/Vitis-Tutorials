

#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11

MK_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
XF_PROJ_ROOT ?= $(shell bash -c 'export MK_PATH=$(MK_PATH); echo $${MK_PATH%/AI_Engine_Development/*}')
PLATFORM ?= xilinx_vck190_base_202310_1

DEVICE_NAME := xcvc1902-vsva2197-2MP-e-S

platform: xsa xpfm

xsa:
	$(MAKE) xsa -C hw

xpfm:
	$(MAKE) xpfm -C sw

clean:
	$(MAKE) clean -C hw
	$(MAKE) clean -C sw
