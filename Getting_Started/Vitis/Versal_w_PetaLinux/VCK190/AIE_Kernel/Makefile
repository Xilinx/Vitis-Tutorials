#Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

.PHONY: all compile aiesim clean

all: compile 
compile:
	mkdir output_dir -p
	cd output_dir; v++ --compile --mode aie --target=hw ../aie_srcs/graph.cpp --part=xcvc1902-vsva2197-2MP-e-S -I../data_srcs -I../aie_srcs
aiesim:
	cd output_dir; aiesimulator --profile  --pkg-dir Work

clean:
	rm -rf output_dir
