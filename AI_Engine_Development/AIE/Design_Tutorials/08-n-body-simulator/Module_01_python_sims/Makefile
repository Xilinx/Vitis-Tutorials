
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

.PHONY: help

help::
	@echo "Makefile Usage:"
	@echo "  make all "
	@echo "       Command runs make tests"
	@echo ""
	@echo "  make tests "
	@echo "       Command to run unit tests"
	@echo ""
	@echo "  make animations "
	@echo "       Command to create *.gif of 300 timesteps"
	@echo ""
	@echo "  make clean "
	@echo "      Command to remove the generated files."
	@echo ""

# ################ TARGET: make all ################
all: tests

# ################ TARGET: make tests ################
tests: 
	mkdir -p build
	python3 test.py -v 

# ################ TARGET: make animations ################
animations: build/animation_128_particles.gif build/animation_1280_particles.gif build/animation_12800_particles.gif

build/animation_128_particles.gif: 
	mkdir -p build
	CREATE_ANIMATION_DATA=ON python3 test.py PySimUnitTest.test_random_x1 -v 
	python3 pylib/animate.py build/animation_data_128_particles.txt 128 build/animation_128_particles.gif

build/animation_1280_particles.gif:
	mkdir -p build
	CREATE_ANIMATION_DATA=ON python3 test.py PySimUnitTest.test_random_x10 -v 
	python3 pylib/animate.py build/animation_data_1280_particles.txt 1280 build/animation_1280_particles.gif

build/animation_12800_particles.gif:
	mkdir -p build
	CREATE_ANIMATION_DATA=ON python3 test.py PySimUnitTest.test_random_x100 -v 
	python3 pylib/animate.py build/animation_data_12800_particles.txt 12800 build/animation_12800_particles.gif

clean:
	rm -rf *~
	rm -rf build
