#
# Copyright (C) Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#


.PHONY: clean

all: $(subst .cpp,.xo,$(wildcard *_dual_$(PLIOW).cpp))
%.xo: %.cpp
	v++ -c --platform ${PLATFORM} --clock.defaultFreqHz ${FREQ_HZ} --hls.clock ${FREQ_HZ}:$(basename $<) -k $(basename $<) $< -o $@ --verbose --save-temps

clean:
	rm -rf _x *.xo* *.log .Xil
