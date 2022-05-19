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
.PHONY: aie
aie: 
	$(MAKE) all -C aie

# ################ TARGET: make pl_kernels ################
.PHONY: pl_kernels
pl_kernels: 
	$(MAKE) all -C pl_kernels

# ################ TARGET: make xclbin ################
.PHONY: xclbin
xclbin: 
	$(MAKE) all -C xclbin

# ################ TARGET: make host ################
host: 
	$(MAKE) all -C host_sw

# ################ TARGET: make sd_card ################
.PHONY: sd_card
sd_card:
	$(MAKE) all -C sd_card

# ################ TARGET: make build ################
build: all

# ################ TARGET: make run ################
run: build
	$(MAKE) run -C sd_card

# ################ TARGET: make clean ################
clean: 
	$(MAKE) clean -C results
	$(MAKE) clean -C sd_card
	$(MAKE) clean -C host_sw
	$(MAKE) clean -C xclbin
	$(MAKE) clean -C pl_kernels
	$(MAKE) clean -C aie
	rm -rf *~
