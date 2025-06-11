#
#Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#

# Get version number
TOOLV := $(shell basename  $(XILINX_VITIS))
ifeq ($(TOOLV), Vitis)
DIRNAME := $(shell dirname $(XILINX_VITIS))
TOOLV := $(shell basename $(DIRNAME))
endif


###
# Guarding Checks. Do not modify.
###
check_defined = \
	$(strip $(foreach 1,$1, \
		$(call __check_defined,$1,$(strip $(value 2)))))

__check_defined = \
	$(if $(value $1),, \
		$(error Undefined $1$(if $2, ($2))))

guard-PLATFORM_REPO_PATHS:
	$(call check_defined, PLATFORM_REPO_PATHS, Set to: $(PLATFORM_REPO_PATHS))

guard-ROOTFS:
	$(call check_defined, ROOTFS, Set to: xilinx-versal-common-v$(TOOLV)/rootfs.ext4)

guard-IMAGE:
	$(call check_defined, IMAGE, Set to: xilinx-versal-common-v(TOOLV)/Image)

guard-CXX:
	$(call check_defined, CXX, Run: xilinx-versal-common-v(TOOLV)/environment-setup-cortexa72-cortexa53-amd-linux)

guard-SDKTARGETSYSROOT:
	$(call check_defined, SDKTARGETSYSROOT, Run: xilinx-versal-common-v(TOOLV)/environment-setup-cortexa72-cortexa53-amd-linux)