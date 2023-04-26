
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
aie: 
	$(MAKE) all -C Module_02_aie

# ################ TARGET: make aie ################
aiesim: 
	$(MAKE) sim -C Module_02_aie

# ################ TARGET: make pl_kernels ################
pl_kernels: 
	$(MAKE) all -C Module_03_pl_kernels

# ################ TARGET: make xsa ################
xsa: 
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
x1_design:
	$(MAKE) all -C x1_design

# ################ TARGET: make x10_design ################
.PHONY: x10_design
x10_design:
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
