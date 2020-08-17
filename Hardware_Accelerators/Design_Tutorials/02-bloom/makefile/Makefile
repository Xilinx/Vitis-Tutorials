CURRENT_DIR = $(PWD)
TARGET := hw

#####################################################################################################
## Detemines how many batches of buffers are sent from host to kernel
#####################################################################################################
ITER   := 

#####################################################################################################
## PF sets how many words to process in parallel 
#####################################################################################################
PF     := 8
ifeq ($(STEP), kernel_4)
PF   := 4
endif
ifeq ($(STEP), kernel_16)
PF   := 16
endif


PLATFORM :=xilinx_u200_xdma_201830_2

SRCDIR := ./../reference_files

# Default Settings here ..
HOST_SRC_CPP := $(SRCDIR)/compute_score_host.cpp
BUILDDIR := ./../build/$(STEP)/kernel_$(PF)/$(TARGET)


# Common Source Code for FPGA and Host
HOST_SRC_FPGA := $(SRCDIR)/compute_score_fpga_kernel.cpp
HOST_SRC_CPP := $(SRCDIR)/compute_score_host.cpp
HOST_SRC_CPP += $(SRCDIR)/MurmurHash2.c
HOST_SRC_CPP += $(SRCDIR)/xcl2.cpp
HOST_SRC_CPP += $(SRCDIR)/main.cpp 

#### For the following steps, don't rebuild the kernels as only Host is changing
ifeq ($(STEP), $(filter $(STEP), split_buffer , generic_buffer , sw_overlap ))
NOXCLBIN=1
endif

#####################################################################################################
##  User appropriate host file 
#####################################################################################################
HOST_SRC_RUN_CPP = $(SRCDIR)/run_single_buffer.cpp
ifeq ($(STEP),multiDDR)
HOST_SRC_RUN_CPP = $(SRCDIR)/run_sw_overlap_multiDDR.cpp
endif
ifeq ($(STEP),split_buffer)
HOST_SRC_RUN_CPP = $(SRCDIR)/run_split_buffer.cpp
endif
ifeq ($(STEP),generic_buffer)
HOST_SRC_RUN_CPP = $(SRCDIR)/run_generic_buffer.cpp
endif
ifeq ($(STEP),sw_overlap)
HOST_SRC_RUN_CPP = $(SRCDIR)/run_sw_overlap.cpp
endif

HOST_SRC_CPP += $(HOST_SRC_RUN_CPP)
include common.mk

setup:
	if ! [ -f temp.txt ]; \
  	then \
		touch temp.txt; \
		sudo -E -- bash -c 'source ~/aws-fpga/sdaccel_setup.sh'; \
	fi;



#####################################################################################################
### Build the host executable. This step is always executed
#####################################################################################################
compile_host:  host
	@echo $(HOST_SRC_FPGA) is being used as source for Generating Kernel; \
	mkdir -p $(BUILDDIR)

#####################################################################################################
### if SOLUTION=1, Copy the necessary xclbin to $(BUILDDIR)
### if STEP != single_buffer, NOXCLBIN=1 . Copy the xclbin from single_buffer to $(BUILDDIR)
#####################################################################################################
ifeq ($(SOLUTION),1)
build: compile_host $(BUILDDIR)/$(EMCONFIG_FILE)
		@echo "Copying the xclbin saved from xclbin_save to $(BUILDDIR) "; 
ifeq ($(STEP),multiDDR)
		cp ../xclbin_save/multiDDR/runOnfpga_$(TARGET).xclbin $(BUILDDIR); 
else
		cp ../xclbin_save/kernel_$(PF)/runOnfpga_$(TARGET).xclbin $(BUILDDIR); 
endif
else
ifeq ($(NOXCLBIN),1)
build: compile_host $(BUILDDIR)/$(EMCONFIG_FILE)
	if [ -f ../build/single_buffer/kernel_$(PF)/$(TARGET)/runOnfpga_$(TARGET).xclbin ]; \
  	then \
		echo "Copying the xclbin created from single_bufer to $(BUILDDIR) "; \
		cp ../build/single_buffer/kernel_$(PF)/$(TARGET)/runOnfpga_$(TARGET).xclbin $(BUILDDIR);  \
	fi;
	if ! [ -f ../build/single_buffer/kernel_$(PF)/$(TARGET)/runOnfpga_$(TARGET).xclbin ]; \
  	then \
		echo ""; \
		echo "***   Please run STEP=single_buffer first! *** "; \
		echo ""; \
	fi;
else
#####################################################################################################
###  If SOLUTION != 1 and NOXCLBIN != 1, generate the xclbin from scrath. 
###  Required if using other than U200 platform
###  For multiDDR optional step, this is always executed
#####################################################################################################
build: compile_host ./$(BUILDDIR)/runOnfpga_$(TARGET).xclbin $(BUILDDIR)/$(EMCONFIG_FILE)
endif
endif

#####################################################################################################
###   For HW run, use 100,000 words for computation 
###   For Emulation run, use 100 words for computation 
#####################################################################################################

run:   build host
	cp xrt.ini $(BUILDDIR)
ifeq ($(TARGET), hw)
	@echo "************  Use Command Line to run application!  ************"
	cd $(BUILDDIR) && ./host 100000 $(ITER) ;
else
	@echo "Running $(TAGET) mode"; 
	emconfigutil --nd 1  --platform $(PLATFORM) --od $(BUILDDIR)
	cd $(BUILDDIR) && XCL_EMULATION_MODE=$(TARGET) ./host 100 $(ITER) ;
endif

#####################################################################################################
###  make target to run all the build/run targets. 
###  Following will generate all the xclbins and run all steps 
###  e.g. make execute_all ACTION=run TARGET=hw
#####################################################################################################
	 
execute_all :
	cd $(CURRENT_DIR); make $(ACTION) STEP=single_buffer TARGET=$(TARGET)  PF=4
	cd $(CURRENT_DIR); make $(ACTION) STEP=single_buffer TARGET=$(TARGET)  PF=8
	cd $(CURRENT_DIR); make $(ACTION) STEP=single_buffer TARGET=$(TARGET)  PF=16
	cd $(CURRENT_DIR); make $(ACTION) STEP=generic_buffer TARGET=$(TARGET)  PF=4
	cd $(CURRENT_DIR); make $(ACTION) STEP=generic_buffer TARGET=$(TARGET)  PF=8
	cd $(CURRENT_DIR); make $(ACTION) STEP=generic_buffer TARGET=$(TARGET)  PF=16
	cd $(CURRENT_DIR); make $(ACTION) STEP=sw_overlap TARGET=$(TARGET)  PF=4
	cd $(CURRENT_DIR); make $(ACTION) STEP=sw_overlap TARGET=$(TARGET)  PF=8
	cd $(CURRENT_DIR); make $(ACTION) STEP=sw_overlap TARGET=$(TARGET)  PF=16
	cd $(CURRENT_DIR); make $(ACTION) STEP=multiDDR TARGET=$(TARGET)  PF=4
	cd $(CURRENT_DIR); make $(ACTION) STEP=multiDDR TARGET=$(TARGET)  PF=8
	cd $(CURRENT_DIR); make $(ACTION) STEP=multiDDR TARGET=$(TARGET)  PF=16

help:
	@echo  " "
	@echo  " "
	@echo  " Makefile Usage:"
	@echo  " "
	@echo  "        Build or Run   Step to run                                              Parallel Hashes Documents split into ITER"
	@echo  " > make <build/run>    STEP=<single_buffer/generic_buffer/sw_overlap/multiDDR > PF=<4/8/16>     ITER=8 "
	@echo  " "
	@echo  " Execute Following to build kernel for 4,8.16 Hashes in Parallel "
	@echo  " > make execute_all ACTION=build TARGET=hw "
	@echo  " "
	@echo  " Execute Run Following to run kernel for 4,8.16 Hashes in Parallel "
	@echo  " > make execute_all ACTION=run TARGET=hw "
	@echo  " "
	@echo  " "
	@echo  " To run any STEP involving generic_buffer/sw_overlap/multiDDR, kernel should be built using STEP=single_buffer "
	@echo  " "
	@echo  " "
