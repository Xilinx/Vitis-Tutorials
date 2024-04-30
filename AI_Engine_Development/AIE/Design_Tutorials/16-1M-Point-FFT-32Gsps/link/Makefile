#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

TARGET       ?= hw
LIBNAME      ?= ../aie_src/libadf.a
PF_STDNAME   ?= base_pfm_vck190
PLATFORMNAME ?= platform/$(PF_STDNAME)/export/$(PF_STDNAME)/$(PF_STDNAME).xpfm

XO_NAME   = ../demo_ip/demo_ip.xo
XSA       = $(PF_STDNAME).xsa
BUILD_DIR = build_$(TARGET)
SOURCES   = system.cfg $(XO_NAME) $(LIBNAME) ../$(PLATFORMNAME)
THIS_XSA  = $(BUILD_DIR)/$(XSA)

.PHONY: all clean cleanall

all:	$(BUILD_DIR) $(THIS_XSA)

$(BUILD_DIR):	$(SOURCES)
	/bin/rm -rf $(BUILD_DIR)
	mkdir $(BUILD_DIR)

$(THIS_XSA):	$(SOURCES)
	cd $(BUILD_DIR) && \
	v++ --link --debug --save-temps \
	    --target $(TARGET) \
	    --platform ../../$(PLATFORMNAME) \
	    --config ../system.cfg \
	    --output $(XSA) \
	    ../$(XO_NAME) \
	    ../$(LIBNAME)
	cd $(BUILD_DIR) && \
	generate-platform.sh -name $(PF_STDNAME) -hw $(XSA) -domain ai_engine:aie_runtime \
	    -domain psv_cortexa72_0:standalone

clean:
		/bin/rm -rf $(BUILD_DIR)

cleanall:	clean
