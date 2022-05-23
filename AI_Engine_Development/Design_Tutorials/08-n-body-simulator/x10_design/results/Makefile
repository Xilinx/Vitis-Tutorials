# Copyright 2021 Xilinx Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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

all: animation

animation:
	mkdir -p build
	python3 ../../Module_01_python_sims/pylib/animate.py data/animation_data.txt 1280 build/animation.gif

clean:
	rm -rf *~
	rm -rf build
	rm -rf data/animation_data.txt
