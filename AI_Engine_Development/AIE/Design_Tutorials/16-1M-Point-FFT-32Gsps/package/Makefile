#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

TARGET       ?= hw
PF_STDNAME   ?= base_pfm_vck190
PLATFORMNAME ?= platform/$(PF_STDNAME)/export/$(PF_STDNAME)/$(PF_STDNAME).xpfm

LIBNAME      = ../aie_src/libadf.a
BUILD_DIR    = build_$(TARGET)
XSA          = ../link/$(BUILD_DIR)/$(PF_STDNAME).xsa
SD_CARD      = $(BUILD_DIR)/a.xclbin
MAIN_ELF     = ../host/$(BUILD_DIR)/main.elf
SOURCES      = ../$(PLATFORMNAME) $(LIBNAME) $(XSA) $(MAIN_ELF)

.PHONY:		all clean cleanall

all:	$(BUILD_DIR) $(SD_CARD)

$(BUILD_DIR):	$(SOURCES)
	/bin/rm -rf $(BUILD_DIR)
	mkdir $(BUILD_DIR)

$(SD_CARD):	$(SOURCES)
	cd $(BUILD_DIR) && \
	v++ --package --debug \
	    --target $(TARGET) \
	    --platform ../../$(PLATFORMNAME) \
	    --package.domain aie_runtime_domain \
	    --package.boot_mode sd \
	    --package.image_format fat32 \
	    --package.ps_elf ../$(MAIN_ELF),a72-0 \
	    ../$(LIBNAME) \
	    ../$(XSA)
	@echo "*********************************************************************************"
	@echo "  Done! VCK190 boot image '$(BUILD_DIR)/BOOT.BIN' is available for testing."
	@echo "*********************************************************************************"

clean:
	/bin/rm -rf $(BUILD_DIR) _x .Xil *.log a.xclbin xcd.log v++.package_summary

cleanall:	clean
