#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

help::
	@echo "Makefile Usage:"
	@echo "  make all TARGET=<hw_emu|hw>"
	@echo "       Command runs make aie, make pl_kernels, make xsa, make host, make sd_card for x100_design"
	@echo "       Default TARGET=hw"
	@echo ""
	@echo "  make aie"
	@echo "       Command to create the AIE graph"
	@echo ""
	@echo "  make pl_kernels TARGET=<hw_emu|hw> "
	@echo "       Command to create PL HLS kernels *.xo"
	@echo "       Default TARGET=hw"
	@echo ""
	@echo "  make xsa TARGET=<hw_emu|hw>"
	@echo "       Command to create XSA" 
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
all: aie pl_kernels xsa host sd_card

# ################ TARGET: make aie ################
.PHONY: aie
aie: 
	$(MAKE) all -C aie

# ################ TARGET: make pl_kernels ################
.PHONY: pl_kernels
pl_kernels: 
	$(MAKE) all -C pl_kernels

# ################ TARGET: make xsa ################
.PHONY: xsa
xsa: 
	$(MAKE) all -C xsa

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
	$(MAKE) clean -C xsa
	$(MAKE) clean -C pl_kernels
	$(MAKE) clean -C aie
	rm -rf *~
