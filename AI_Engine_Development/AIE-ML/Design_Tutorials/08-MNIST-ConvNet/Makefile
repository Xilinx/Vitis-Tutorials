#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins
#

.PHONY:  help clean all

help:
	@echo "Run 'make my-venv' to build a Python Virtual Environment for this tutorial"
	@echo "Afterwards run 'make all' to build the MNIST ConvNet design for AIE-ML"

all:
	make all -C aie/mnist

my-venv:	create_venv.sh
	./create_venv.sh

clean:
	make clean -C aie/conv2d_w1
	make clean -C aie/max_pooling2d_w2
	make clean -C aie/conv2d_w3
	make clean -C aie/max_pooling2d_w4
	make clean -C aie/conv2d_w5
	make clean -C aie/dense_w7
	make clean -C aie/mnist
	/bin/rm -rf .ipynb_checkpoints Thumbs.db
	/bin/rm -rf aie/conv2d_w1/bias_trained.txt
	/bin/rm -rf aie/conv2d_w1/taps_trained.txt
	/bin/rm -rf aie/conv2d_w3/bias_trained.txt
	/bin/rm -rf aie/conv2d_w3/taps_trained.txt
	/bin/rm -rf aie/conv2d_w5/bias_trained.txt
	/bin/rm -rf aie/conv2d_w5/taps_trained.txt
	/bin/rm -rf aie/dense_w7/bias_trained.txt
	/bin/rm -rf aie/dense_w7/taps_trained.txt



