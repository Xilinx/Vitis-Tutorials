#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved. Portions of this file consist of AI-generated content.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani

# Linear Equations - AIE Example Makefile

PART := xcvc1902-vsva2197-2MP-e-S 

# Model selection: channel_estimation | linear_equalization
MODEL ?= channel_estimation
VALID_MODELS = channel_estimation linear_equalization
MATLAB_SCRIPT = mpsk_$(MODEL)

# Directories
SRC_DIR = src
DATA_DIR = data
MATLAB_DIR = matlab

# Graph source files
MY_APP = $(SRC_DIR)/graph.cpp
MY_SOURCES = $(MY_APP) $(SRC_DIR)/graph.h
LOG_FILE = log
#user should download Vitis library and set the following DSP and solver library path
SOLVERLIB_ROOT = ${DSPLIB_VITIS}/solver
DSPLIB_ROOT =${DSPLIB_VITIS}/dsp
# AIE compiler settings
OTHER_OPTS = --aie.pl-freq=625.0
LIB_OPTS  = --include=${DSPLIB_ROOT}/L2/include/aie \
            --include=${DSPLIB_ROOT}/L1/include/aie \
            --include=${DSPLIB_ROOT}/L1/src/aie \
            --include=${SOLVERLIB_ROOT}/L2/include/aie \
		    --include=${SOLVERLIB_ROOT}/L1/include/aie \
		    --include=${SOLVERLIB_ROOT}/L1/src/aie
ALL_INCLUDE = $(LIB_OPTS)

AIE_FLAGS := --aie.verbose $(ALL_INCLUDE) $(MY_APP) $(OTHER_OPTS)
AIE_FLAGS += --part=$(PART)

# Targets
.PHONY: all clean help compile sim x86compile x86sim data check-model profile extract-latency extract-resources extract-throughput

help::
	@echo "Makefile Usage:"
	@echo "  make all [MODEL=<model>]"
	@echo "    Command to generate AIE design and run simulation"
	@echo ""
	@echo "  make compile"
	@echo "    Run v++ AIE compiler and build the design"
	@echo ""
	@echo "  make sim"
	@echo "    Run AIE simulator"
	@echo ""
	@echo "  make profile"
	@echo "    Run AIE simulator with profiling enabled"
	@echo ""
	@echo "  make extract-latency"
	@echo "    Extract latency metrics from AIE simulation using Vitis Python API"
	@echo ""
	@echo "  make extract-resources"
	@echo "    Extract AIE resource utilization metrics to CSV"
	@echo ""
	@echo "  make x86compile"
	@echo "    Compile for x86 simulation"
	@echo ""
	@echo "  make x86sim"
	@echo "    Run x86 simulator"
	@echo ""
	@echo "  make data [MODEL=<model>]"
	@echo "    Generate test data using MATLAB"
	@echo ""
	@echo "  make verify-aie"
	@echo "    Verify AIE simulation results"
	@echo ""
	@echo "  make verify-x86"
	@echo "    Verify x86 simulation results"
	@echo ""
	@echo "  make clean"
	@echo "    Remove all generated files"
	@echo ""
	@echo "  MODEL variable (default: channel_estimation):"
	@echo "    channel_estimation      - M-PSK channel estimation model"
	@echo "    linear_equalization     - M-PSK linear equalization model"
	@echo "    Example: make data MODEL=linear_equalization"

all: compile profile extract-resources extract-latency extract-throughput 

x86all: x86compile x86sim verify-x86

compile: $(MY_SOURCES)
	v++ -c --mode aie --target=hw $(AIE_FLAGS) 2>&1 | tee $(LOG_FILE)

x86compile: $(MY_SOURCES)
	v++ -c --mode aie --target=x86sim $(AIE_FLAGS) 2>&1 | tee $(LOG_FILE)

check-model:
	@if echo "$(VALID_MODELS)" | grep -qw "$(MODEL)"; then \
		true; \
	else \
		echo "ERROR: Invalid MODEL='$(MODEL)'. Valid options: $(VALID_MODELS)"; \
		exit 1; \
	fi

# Generate test data using MATLAB
data: check-model
	@echo "Generating test data with MATLAB (model: $(MODEL))..."
	cd $(MATLAB_DIR) && matlab -batch "$(MATLAB_SCRIPT)"

sim:
	@echo "Running AIE simulation..."
	aiesimulator 2>&1 | tee -a $(LOG_FILE)

profile:
	@echo "Running AIE simulation with profiling..."
	aiesimulator --profile --online -wdb -text 2>&1 | tee -a $(LOG_FILE)

extract-latency:
	@echo "Extracting latency metrics using Vitis Python API..."
	vitis -s ../utility_scripts/extract_latency.py ../design

extract-resources:
	@echo "Extracting AIE resource utilization metrics..."
	python3 ../utility_scripts/extract_aie_resources.py design

extract-throughput:
	@echo "Extracting throughput metrics..."
	python3 ../utility_scripts/extract_throughput.py ../design cfloat 64

x86sim:
	@echo "Running x86 simulation..."
	x86simulator 2>&1 | tee -a $(LOG_FILE)

verify-aie:
	@echo "Verifying AIE simulation results..."
	cd $(MATLAB_DIR) && matlab -batch "verify_results('aie')"

verify-x86:
	@echo "Verifying x86 simulation results..."
	cd $(MATLAB_DIR) && matlab -batch "verify_results('x86')"

analyze:
	vitis_analyzer aiesimulator_output/default.aierun_summary
	
clean:
	@rm -rf Work* .Xil function_wdb_dir .crashReporter .AIE_SIM_CMD_LINE_OPTIONS
	@rm -rf AIECompiler.log xcd.log $(LOG_FILE) aiesimulator_output x86simulator_output
	@rm -rf libadf.a sol.db Map_Report.csv AIESimulator.log pl_sample_counts
	@rm -rf ISS_RPC_SERVER_PORT system_flat.wcfg system.wcfg system.wdb
	@rm -rf tmp.vcd.vcd trdata.aiesim vcdanalyze.log vitis_analyzer_pid* logs
	@rm -rf VCDAnalyze.log plio_throughput_info.json qemu_rp.log .wsdata _ide
	@rm -rf diag_report.log xsc_report.log vitis_analyzer.* throughput_info.json
	@rm -rf  matlab/AIECompiler.log matlab/vfs_work matlab/.Xil
	@echo "Clean complete."
