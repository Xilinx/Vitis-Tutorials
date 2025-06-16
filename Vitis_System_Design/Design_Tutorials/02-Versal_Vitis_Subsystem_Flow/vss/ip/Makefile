#
# Copyright (C) 2023-2025, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

ECHO                  := @echo

PART                  ?= xcvc1902-vsva2197-2MP-e-S
FREQHZ                ?= 250000000

XO_DIR                := xo_hw
XO_DIRS_EXCLUDE       := $(patsubst %/,%,$(dir $(wildcard ${XO_DIR}*/)))
KERNEL_DIRS           := $(patsubst %/,%,$(dir $(wildcard */)))
KERNEL_NAME           := $(filter-out ${XO_DIRS_EXCLUDE},$(KERNEL_DIRS))
KERNEL_NAME_CLEAN     := $(addsuffix .clean,$(KERNEL_NAME))
TARGET                ?= hw

.PHONY: help clean all $(KERNEL_NAME)

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate the All IPs used in this design"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

all: ${XO_DIR} $(KERNEL_NAME)

$(XO_DIR):
	mkdir    ${XO_DIR}

$(KERNEL_NAME):
	make all -C $@ PART=${PART} FREQHZ=${FREQHZ}

clean: $(KERNEL_NAME_CLEAN)
	rm -rf $(XO_DIR)

$(KERNEL_NAME_CLEAN):
	make clean -C $(basename $@)

