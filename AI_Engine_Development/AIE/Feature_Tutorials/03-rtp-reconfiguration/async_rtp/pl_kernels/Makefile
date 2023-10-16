
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

.PHONY: clean

all: $(subst .cpp,.xo,$(wildcard *.cpp))
%.xo: %.cpp
	v++ -c --platform ${PLATFORM} -k $(basename $<) $< -o $@ --verbose --save-temps

clean:
	rm -rf _x *.xo* *.log
