#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

.PHONY: help

help::
	@echo "Makefile Usage:"
	@echo "  make all "
	@echo "       Command to run make animation"
	@echo ""
	@echo "  make animation "
	@echo "       Command to create *.gif of 300 timesteps"
	@echo ""
	@echo "  make clean "
	@echo "      Command to remove the generated files."
	@echo ""

all: clean animation

animation:
	mkdir -p build
	python3 ../Module_01_python_sims/pylib/animate.py data/animation_data.txt 12800 build/animation.gif

clean:
	rm -rf *~
	rm -rf build
	rm -rf data/*.txt
