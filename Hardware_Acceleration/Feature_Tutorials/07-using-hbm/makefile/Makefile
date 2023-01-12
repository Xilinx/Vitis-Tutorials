XF_PROJ_ROOT ?= $(shell bash -c 'export MK_PATH=$(MK_PATH); echo $${MK_PATH%/Hardware_Acceleration/*}')

CURRENT_DIR = $(PWD)
TARGET := hw
memtype := DDR
banks :=


ifeq ($(SPRITE),true)
	SRCDIR := $(XF_PROJ_ROOT)/Hardware_Acceleration/Feature_Tutorials/07-using-hbm/reference_files
else
	SRCDIR := ../reference_files
endif
	
include mem_connectivity.mk

ifeq ($(memtype), DDR)
	PLATFORM ?=xilinx_u200_gen3x16_xdma_2_202110_1
else
	PLATFORM ?=xilinx_u50_gen3x16_xdma_5_202210_1
endif

addrndm := 0
krnl_loop := 1
txSize   := 64
dsize    := 512
dwords   := $(dsize)/32
bl       := $(txSize)*8/$(dsize)
buildxclbin := 0

ifeq ($(rama),1)
  _rama := _rama
endif

default_make_goal: help

# Default Settings here ..
HOST_SRC_CPP := $(SRCDIR)/host.cpp
builddir := ./build/$(memtype)_$(_banks)Banks_d$(dsize)_txSize$(txSize)$(_rama)


# Common Source Code for FPGA and Host
FPGA_SRC_CPP := $(SRCDIR)/kernel.cpp
HOST_SRC_CPP += $(SRCDIR)/xcl2.cpp

# SoC variables
RUN_APP_SCRIPT = run_app.sh
PACKAGE_OUT = package.$(TARGET)

LAUNCH_EMULATOR = $(PACKAGE_OUT)/launch_$(TARGET).sh
RESULT_STRING = TEST PASSED

VPP := v++
SDCARD := sd_card

include common.mk

build: display_time host mem_config xo xclbin $(builddir)/$(EMCONFIG_FILE)

display_time:
	@echo "# make command started $(shell date) #"

#####################################################################################################
ifeq ($(memtype), DDR)
CFG:=$(builddir)/DDR_connectivity.cfg
mem_config:
	@echo  "Creating DDR configuration";
	$(shell echo [connectivity] > $(CFG))
	$(shell echo sp=vadd_1.in1:DDR[0] >> $(CFG))
	$(shell echo sp=vadd_1.in2:DDR[1] >> $(CFG))
	$(shell echo sp=vadd_1.out:DDR[2] >> $(CFG))
endif
ifeq ($(memtype), HBM)
CFG:=$(builddir)/HBM_connectivity.cfg
mem_config:
	@echo  "Creating HBM configuration";
	@echo [connectivity] > $(CFG)
	@echo sp=vadd_1.in1:$(in_M0) >> $(CFG)
	@echo sp=vadd_1.in2:$(in_M1) >> $(CFG)
	@echo sp=vadd_1.out:$(out_M2) >> $(CFG)
endif
########################################################################################################

ifneq ($(TARGET), hw)
PREFIX_COMMAND=XCL_EMULATION_MODE=$(TARGET)
endif

ddr2hbm_builds: 
	make ddr_addSeq_build hbm_addSeq_2Banks_build hbm_addSeq_overlap_build

hbm_bw_builds:
	make build_without_rama build_with_rama;
#OR
	#make noramajob-64 noramajob-128 noramajob-256 noramajob-512 noramajob-1024;
	#make ramajob-64 ramajob-128 ramajob-256 ramajob-512 ramajob-1024;

ifeq ($(buildxclbin), 0)
run:  host 
else
run:   build host
endif
	cp xrt.ini $(builddir)
	@echo "*** Running $(TARGET) mode ***  Use Command Line to run application!"
	cd $(builddir) && $(PREFIX_COMMAND) ./host vadd_$(TARGET).xclbin $(dsize) $(addrndm) $(krnl_loop) $(txSize);

create_perf:
	@echo "Addr Pattern   Total Size(MB) Transaction Size  Throughput Achieved(GB/s) " > ./Run.perf

# DDR2HBM Module - HW Build and Run DDR based xclbins 
ddr_addSeq_build:
	make build TARGET=hw memtype=DDR dsize=600 addrndm=0 krnl_loop=1 buildxclbin=1 
ddr_addSeq:
	make run TARGET=hw memtype=DDR dsize=600 addrndm=0 krnl_loop=1 buildxclbin=0 
ddr_addRandom:
	make run TARGET=hw memtype=DDR dsize=600 addrndm=1 krnl_loop=1 buildxclbin=0

hbm_addSeq_2Banks_build:
	make build TARGET=hw memtype=HBM banks=0_1 dsize=512 buildxclbin=1
hbm_addSeq_2Banks:
	make run TARGET=hw memtype=HBM banks=0_1 dsize=512 buildxclbin=0
hbm_addSeq_overlap_build:
	make build TARGET=hw memtype=HBM banks=overlap dsize=384 buildxclbin=1
hbm_addSeq_overlap:
	make run TARGET=hw memtype=HBM banks=overlap dsize=384 buildxclbin=0

# HBM_BW Module - HW Build and Run HBM based xclbins 
all_norama_builds : make noramajob-64 noramajob-128 noramajob-256 noramajob-512 noramajob-1024 -j6
all_rama_builds : make ramajob-64 ramajob-128 ramajob-256 ramajob-512 ramajob-1024 -j6
all_bw_run : all_hbm_seq_run all_hbm_rnd_run all_hbm_rnd_rama_run

# HBM Build/Run Targets with Address Pattern-Sequential, Transaction size of 64Byte and using all the HBM banks
M0_PC0-31_Addseq_txSize64:
	make run TARGET=hw memtype=HBM banks=0_31 dsize=512 addrndm=0  txSize=64 ;

banks:=0_31

TXSIZE_LIST := 64 128 256 512 1024
#bl            1   2   4   8
RAMAJOBPREFIX := ramajob-
NORAMAJOBPREFIX := no${RAMAJOBPREFIX}
NORAMAJOBS := $(addprefix ${NORAMAJOBPREFIX},${TXSIZE_LIST})
RAMAJOBS   := $(addprefix   ${RAMAJOBPREFIX},${TXSIZE_LIST})
BUILDLOGPREFIX := ybuild

display_message:
	@echo "jobs submitted in the background, and in parallel if -j NUMBER, see the build logs or tail them with something like 'tail -f -s 1 ${BUILDLOGPREFIX}*' ; a message will be displayed and all are completed (with or without errors)."

build_without_rama: display_message ${NORAMAJOBS}
	@echo "Makefile completed, builds done for list ${TXSIZE_LIST} of goal $@ with dependencies $^"
${NORAMAJOBS}: ${NORAMAJOBPREFIX}%:
	$(MAKE) build TARGET=$(TARGET) memtype=HBM banks=${banks} dsize=512 txSize=$* buildxclbin=1 rama=0 > ${BUILDLOGPREFIX}-txSize$*-rama0.txt;

build_with_rama: display_message ${RAMAJOBS}
	@echo "Makefile completed, builds done for list ${TXSIZE_LIST} of goal $@ with dependencies $^"
${RAMAJOBS}: ${RAMAJOBPREFIX}%:
	$(MAKE) build TARGET=$(TARGET) memtype=HBM banks=${banks} dsize=512 txSize=$* buildxclbin=1 rama=1 > ${BUILDLOGPREFIX}-txSize$*-rama1.txt;

all_hbm_seq_run: create_perf
	for dsize in 256 512 1024; do \
		for txSize in ${TXSIZE_LIST}; do \
		echo "Running with Sequential Address Sequence and txSize of $$txSize"; \
		make run TARGET=hw memtype=HBM banks=${banks} dsize=$$dsize addrndm=0  txSize=$$txSize ; \
		done \
	done
	mv ./Run.perf ./Run_SequentialAddress.perf;

all_hbm_rnd_run: create_perf
	for dsize in 256 512 1024; do \
	for txSize in ${TXSIZE_LIST}; do \
		echo "Running with Random Address Sequence and txSize of $$txSize"; \
		make run TARGET=hw memtype=HBM banks=${banks} dsize=$$dsize addrndm=1 krnl_loop=64 txSize=$$txSize ; \
		done \
	done
	mv ./Run.perf ./Run_RandomAddress.perf;

all_hbm_rnd_rama_run: create_perf
	for dsize in 256 512 1024; do \
	for txSize in ${TXSIZE_LIST}; do \
		echo "Running Build with Random Address Sequence and txSize of $$txSize using RAMA IP"; \
		make run TARGET=hw memtype=HBM banks=${banks} dsize=$$dsize addrndm=1 krnl_loop=64 txSize=$$txSize rama=1 ; \
		done \
	done
	mv ./Run.perf ./Run_RandomAddressRama.perf;
	

help:
	@echo  " "
	@echo  " "
	@echo  " Makefile Usage:"
	@echo  " "
	@echo  "   Use the following command to Target application for sending Data Size(kb) from Host to Device"
	@echo  "  "  
	@echo  "   make <build/run>    TARGET=<sw_emu/hw_emu/hw> memtype=<DDR/HBM> dsize=<From Host in MB> addrndm=<0/1> krnl_loop=<#> txSize=<64/128/256/512/1024> buildxclbin=<0/1>"
	@echo  "   dsize determines how much data to be transferred to HBM from host. "
	@echo  "   txSize determines transaction size for each access initiated by kernel"
	@echo  "   addrndm determines address to be random(1) or sequential(0) for Kernel accessing memtype" 
	@echo  "   krnl_loop can be set to any num for repeating kernel execution # of times"
	@echo  "  "  
	@echo  "   For Example : "
	@echo  "   make run TARGET=hw memtype=HBM banks=0_31 dsize=512 addrndm=0  txSize=64 buildxclbin=1; "
	@echo  "   Module - Migrating to HBM :"
	@echo  "   > Build DDR with Sequential Address Access     -- make ddr_addSeq_build "
	@echo  "   > Build HBM using Multiple Banks               -- make hbm_addSeq_2Banks_build"
	@echo  "   > Build HBM using overlapped Banks             -- make hbm_addSeq_overlap_build"         
	@echo  "   > Run DDR with Sequential Address Access       -- make ddr_addSeq "
	@echo  "   > Run HBM using Multiple Banks                 -- make hbm_addSeq_2Banks"
	@echo  "   > Run HBM using overlapped Banks               -- make hbm_addSeq_overlap"         
	@echo  " "
	@echo  "   Module - BW Explorations :"
	@echo  "   > Build all xclbins for all TXSIZE_LIST w/o rama   -- make noramajob-64 noramajob-128 noramajob-256 noramajob-512 noramajob-1024 "
	@echo  "   > Build all xclbins for all TXSIZE_LIST wth rama   -- make ramajob-64 ramajob-128 ramajob-256 ramajob-512 ramajob-1024 "
	@echo  "   > Run lab using HBM with Sequential Address Access -- make all_hbm_seq_run"
	@echo  "   > Run lab using HBM with Sequential Address Access -- make all_hbm_rnd_run"
	@echo  "   > Run lab using HBM with Sequential Address Access -- make all_hbm_rnd_rama_run"
	@echo  " "
	@echo  " "
