
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

.PHONY: clean

all: $(subst .cpp,.xo,$(wildcard *.cpp)) ../Work/temp/packet_ids_c.h
%.xo: %.cpp
	cp ../Work/temp/packet_ids_c.h .
	sed 's/s2mm/$(basename $<)/' config.cfg > tmp.cfg
	v++ -c --mode hls --platform ${PLATFORM} --config tmp.cfg

clean:
	rm -rf _x *.xo* *.log
