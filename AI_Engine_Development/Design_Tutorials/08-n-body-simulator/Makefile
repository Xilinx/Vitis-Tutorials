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

help::
	@echo "Makefile Usage:"
	@echo "  make all TARGET=<hw_emu|hw>"
	@echo "       Command runs make aie, make pl_kernels, make xclbin, make host, make sd_card for x100_design"
	@echo "       Default TARGET=hw"
	@echo ""
	@echo "  make aie"
	@echo "       Command to create the AIE graph"
	@echo ""
	@echo "  make pl_kernels TARGET=<hw_emu|hw> "
	@echo "       Command to create PL HLS kernels *.xo"
	@echo "       Default TARGET=hw"
	@echo ""
	@echo "  make xclbin TARGET=<hw_emu|hw>"
	@echo "       Command to create XCLBIN" 
	@echo "       Default TARGET=hw"
	@echo ""
	@echo "  make host <hw_emu|hw>"
	@echo "       Command to create host application" 
	@echo "       Default TARGET=hw"
	@echo ""
	@echo "  make sd_card <hw_emu|hw>"
	@echo "       Command to create sd_card.img" 
	@echo "       Default TARGET=hw"
	@echo ""
	@echo "  make x1_design"
	@echo "       Command to create the x1 AIE tile design" 
	@echo ""
	@echo "  make x10_design"
	@echo "       Command to create the x10 AIE tile design" 
	@echo ""
	@echo "  make all_designs"
	@echo "       Command to create the x100, x1, and x10 AIE tile designs" 
	@echo ""
	@echo "  make clean"
	@echo "      Command to remove all generated files."
	@echo ""

# =========================================================
# TARGET SELECTION
# TARGET=hw (default)
# TARGET=hw_emu
# =========================================================
export TARGET :=hw

# ################ TARGET: make all ################
all: aie pl_kernels xclbin host sd_card

# ################ TARGET: make aie ################
aie: 
	$(MAKE) all -C Module_02_aie

# ################ TARGET: make pl_kernels ################
pl_kernels: 
	$(MAKE) all -C Module_03_pl_kernels

# ################ TARGET: make xclbin ################
xclbin: 
	$(MAKE) all -C Module_04_full_system_design

# ################ TARGET: make host ################
host: 
	$(MAKE) all -C Module_05_host_sw

# ################ TARGET: make sd_card ################
sd_card:
	$(MAKE) all -C Module_06_sd_card_and_hw_run

# ################ TARGET: make build ################
build: all

# ################ TARGET: make run ################
run: build
	$(MAKE) run -C Module_06_sd_card_and_hw_run

# ################ TARGET: make x1_design ################
.PHONY: x1_design
x10_design:
	$(MAKE) all -C x1_design

# ################ TARGET: make x10_design ################
.PHONY: x10_design
x100_design:
	$(MAKE) all -C x10_design

# ################ TARGET: make all_designs ################
.PHONY: all_designs
all_designs: all x1_design x10_design

# ################ TARGET: make clean ################
clean: 
	$(MAKE) clean -C Module_07_results
	$(MAKE) clean -C Module_06_sd_card_and_hw_run
	$(MAKE) clean -C Module_05_host_sw
	$(MAKE) clean -C Module_04_full_system_design
	$(MAKE) clean -C Module_03_pl_kernels
	$(MAKE) clean -C Module_02_aie
	$(MAKE) clean -C Module_01_python_sims
	rm -rf *~
