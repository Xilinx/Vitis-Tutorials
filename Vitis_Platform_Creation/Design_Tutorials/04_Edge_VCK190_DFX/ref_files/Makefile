#/*
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11
#*/

# ---------------------------------
# Target all: Generate the Vitis platform and verify it including base bootable design
# ---------------------------------

# Board: VCK190, VCK190_ES1
export BOARD=VCK190
PLATFORM_NAME=vck190_custom_dfx


XSA=./step1_hw/build/vivado/$(PLATFORM_NAME)_static.xsa
XPFM=./step2_sw/build/pfm/$(PLATFORM_NAME)/export/$(PLATFORM_NAME)/$(PLATFORM_NAME).xpfm

.PHONY: all clean step1 step2 step3 
all: step1 step2 step3
sd_card:step1 step2 step3_hw
step1: $(XSA)

$(XSA):
	echo "Step 1: Generating XSA $(XSA)"
	$(MAKE) -C step1_hw all

step2: $(XPFM)

$(XPFM): $(XSA) 
	echo "Step 2: Generating XPFM $(XPFM)"
	$(MAKE) -C step2_sw all



step3: $(XPFM)
	echo "Step 3: Validating the platform"
	$(MAKE) -C step3_validate all
step3_hw: $(XPFM)
	echo "Step 3: Validating the platform"
	$(MAKE) -C step3_validate sd_card
clean:
	$(MAKE) -C step1_hw clean
	$(MAKE) -C step2_sw clean
	$(MAKE) -C step3_validate clean
