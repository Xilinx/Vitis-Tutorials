#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT


ifeq (,$(GENKERNEL))
$(error GENKERNEL is not set)
endif
ifeq (,$(SPEC))
$(error SPEC is not set)
endif
ifeq (,$(TOOLDIR))
$(error TOOLDIR is not set)
endif


KSTAMP := $(dir $(SPEC))/.stamp

$(KSTAMP): $(SPEC)
	$(GENKERNEL) --tool-dir $(TOOLDIR) $(SPEC)
	touch $(KSTAMP)
