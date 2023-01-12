# Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT


TARGET          ?= hw
PLATFORM_CUSTOM ?= vck190_thin
DEVICE_NAME     ?= xcvc1902-vsva2197-2MP-e-S

platform: xsa xpfm

xsa:
	$(MAKE) xsa -C hw PLATFORM_CUSTOM=$(PLATFORM_CUSTOM) DEVICE_NAME=$(DEVICE_NAME)

xpfm:
	$(MAKE) xpfm -C sw PLATFORM_CUSTOM=$(PLATFORM_CUSTOM)

clean:
	$(MAKE) clean -C hw PLATFORM_CUSTOM=$(PLATFORM_CUSTOM) DEVICE_NAME=$(DEVICE_NAME)
	$(MAKE) clean -C sw PLATFORM_CUSTOM=$(PLATFORM_CUSTOM)
