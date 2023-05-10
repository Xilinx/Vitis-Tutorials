
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11


.PHONY: clean

BASE_PLATFORM ?= "Unknown Platform"
BASE_DIR := $(PWD)

all:$(subst .cpp,.xo,$(wildcard *.cpp))
%.xo: %.cpp
	v++ -c -k $(basename $<) --platform $(BASE_PLATFORM) --target=$(TARGET) -DDATA_WIDTH=32 $< -o $@ --verbose --save-temps -g -I $(BASE_DIR)/pl_kernels

clean:
	rm -rf _x *.xo* *.log .Xil
