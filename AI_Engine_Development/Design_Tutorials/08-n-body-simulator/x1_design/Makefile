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
	@echo "  make all "
	@echo "       Command runs make aie, make pl_kernels, make xclbin, make host, make sd_card"
	@echo ""
	@echo "  make aie"
	@echo "       Command to create the AIE graph"
	@echo ""
	@echo "  make pl_kernels TARGET=<hw_emu|hw>"
	@echo "       Command to create PL HLS kernels *.xo"
	@echo "       Default TARGET=hw_emu"
	@echo ""
	@echo "  make xclbin TARGET=<hw_emu|hw>"
	@echo "       Command to create XCLBIN" 
	@echo "       Default TARGET=hw_emu"
	@echo ""
	@echo "  make host <hw_emu|hw>"
	@echo "       Command to create host application" 
	@echo "       Default TARGET=hw_emu"
	@echo ""
	@echo "  make sd_card <hw_emu|hw>"
	@echo "       Command to create sd_card.img" 
	@echo "       Default TARGET=hw_emu"
	@echo ""
	@echo "  make run"
	@echo "       Command to launch QEMU hardware emulation" 
	@echo ""
	@echo "  make clean "
	@echo "      Command to remove all generated files."
	@echo ""

all: aie pl_kernels xclbin host sd_card

# ################ TARGET: make graph ################
.PHONY: aie
aie: 
	$(MAKE) aie -C aie

# ################ TARGET: make kernels ################
.PHONY: pl_kernels
pl_kernels: 
	$(MAKE) pl_kernels -C pl_kernels

# ################ TARGET: make xclbin ################
.PHONY: xclbin
xclbin: 
	$(MAKE) xclbin -C xclbin

# ################ TARGET: make host ################
host: 
	$(MAKE) host -C host_sw

# ################ TARGET: make sd_card ################
.PHONY: sd_card
sd_card:
	$(MAKE) sd_card -C sd_card

# ################ TARGET: make run ################
run:
	$(MAKE) run -C sd_card

# ################ TARGET: make clean ################
clean: 
	$(MAKE) clean -C sd_card
	$(MAKE) clean -C host_sw
	$(MAKE) clean -C xclbin
	$(MAKE) clean -C pl_kernels
	$(MAKE) clean -C aie
	rm -rf *~
