.PHONY: help

help::
	@echo  " Makefile Usage:"
	@echo  ""
	@echo  "  make build TARGET=<sw_emu/hw_emu/hw> "
	@echo  "  Command to generate the design for specified target"
	@echo  ""
	@echo  "  make run TARGET=<sw_emu/hw_emu/hw> "
	@echo  "  Command to generate and run the design for specified target"
	@echo  ""
	@echo  "  make clean TARGET=<sw_emu/hw_emu/hw> "
	@echo  "  Command to remove the generated non-hardware files for specified target"
	@echo  ""

################ Make File Describing host and kernel compile options
include ./make_options.mk

################ Below are the names for host executable and xclbin.
## Please keep it unchanged.
HOST_EXE := host.exe
XO_NAME := fpgabinary.$(TARGET)
XCLBIN := fpgabinary.$(TARGET).xclbin

################ Source Folder 
SRC_REPO := ./src


################# If profiling is enabled pass host flag
ifeq ($(ENABLE_PROF),yes)
PROFILE_OPTIONS:=-c
ifneq ($(PROFILE_ALL_IMAGES),yes)
	NUM_IMAGES:=60
                $(warning [WARNING]: --------------60 Images will be for software profiling larger value will take long time --------------)
	else
		PROFILE_OPTIONS:=
	endif
endif

################ Build directory and number of images to process
ifeq ($(TARGET), sw_emu)
	BUILD_DIR ?= ./build_sw_emu
	NUM_IMAGES:=$(NUM_IMAGES_SW_EMU)
endif

ifeq ($(TARGET), hw_emu)
	BUILD_DIR ?= ./build_hw_emu
	NUM_IMAGES:=$(NUM_IMAGES_HW_EMU)
endif

ifeq ($(TARGET), hw)
	BUILD_DIR ?= ./build
endif

################ Input Image Used for processing
ifeq ($(INPUT_TYPE), random)
	INPUT_IMAGE_OPTION:= -w $(IMAGE_WIDTH) -h $(IMAGE_HEIGHT)
else
	INPUT_IMAGE_OPTION:= -i $(INPUT_IMAGE)
endif

############## Host Application CPP dependencies
ifeq ($(INPUT_TYPE), random)
	HOST_SRC_CPP := $(SRC_REPO)/host_randomized.cpp 
else
	HOST_SRC_CPP := $(SRC_REPO)/host.cpp 
endif
HOST_SRC_CPP += $(SRC_REPO)/xcl2.cpp
HOST_SRC_CPP += $(SRC_REPO)/filter2d_sw.cpp 
HOST_SRC_CPP += $(SRC_REPO)/cmdlineparser.cpp

############## Host Application header file dependencies
HOST_SRC_HEADERS := $(SRC_REPO)/*.h
HOST_SRC_HEADERS += $(SRC_REPO)/*.hpp


############## Kernel Source Files  Dependencies
KERNEL_SRC_CPP := $(SRC_REPO)/filter2d_hw.cpp
KERNEL_SRC_HEADERS := $(SRC_REPO)/common.h
KERNEL_INCLUDES := -I$(SRC_REPO)

############## Check the version of gcc avaiable and select
ifneq ($(shell expr $(shell g++ -dumpversion) \>= 5), 1)
   ifndef XILINX_VIVADO
        $(error [ERROR]: g++ version older. Please use 5.0 or above.)
   else
      CXX := $(XILINX_VIVADO)/tps/lnx64/gcc-6.2.0/bin/g++
      $(warning [WARNING]: g++ version is older. Using g++ provided by the Vitis tool : $(CXX))
   endif
endif

############## Set "HOST" Compiler Paths and Flags
CXXFLAGS += -I$(XILINX_XRT)/include/
CXXFLAGS += -I$(XILINX_VIVADO)/include/
ifneq ($(INPUT_TYPE), random)
CXXFLAGS += -I$(OPENCV_INCLUDE)/
endif
CXXFLAGS += -I$(SRC_REPO)
CXXFLAGS += -O3 -Wall -fmessage-length=0 -std=c++14

############## Set "HOST" Set Linker Paths and Flags
CXXLDFLAGS := -L$(XILINX_XRT)/lib/
ifneq ($(INPUT_TYPE),random)
CXXLDFLAGS += -L$(OPENCV_LIB)/
endif
CXXLDFLAGS += -lOpenCL -pthread -lrt -lstdc++ -lxilinxopencl -fopenmp
ifneq ($(INPUT_TYPE),random)
CXXLDFLAGS += -Wl,-rpath=$(OPENCV_LIB)/ -lopencv_core -lopencv_highgui
endif

############## Kernel Compiler and Linker Flags
VPPFLAGS := -t $(TARGET)
VPPFLAGS += --platform $(PLATFORM) -R1 --save-temps
VPPFLAGS += --temp_dir $(BUILD_DIR)/$(VPP_TEMP_DIRS)
VPPFLAGS += --log_dir $(BUILD_DIR)/$(VPP_LOG_DIRS)
VPPFLAGS += --profile.data all:all:all:all
VPPFLAGS += --profile.trace_memory $(TRACE_DDR)
ifeq ($(ENABLE_STALL_TRACE),yes)
	VPPFLAGS += --profile.stall all:all:all
endif
VPPFLAGS += --config $(KERNEL_CONFIG_FILE)

create_dirs: 
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/$(VPP_TEMP_DIRS)
	mkdir -p $(BUILD_DIR)/$(VPP_LOG_DIRS)


############## Host Executable File Generation
compile_host: $(BUILD_DIR)/$(HOST_EXE)

$(BUILD_DIR)/$(HOST_EXE): $(HOST_SRC_CPP) $(HOST_SRC_HEADERS)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(HOST_SRC_CPP) $(CXXLDFLAGS)
        #cp ./xrt.ini $(BUILD_DIR)/xrt.ini

############## Kernel XO and Xclbin File Generation
#Compile Kernel 
$(BUILD_DIR)/$(XO_NAME).xo: $(KERNEL_SRC_CPP) $(KERNEL_SRC_HEADERS)
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/$(VPP_TEMP_DIRS)
	mkdir -p $(BUILD_DIR)/$(VPP_LOG_DIRS)
	v++ $(VPPFLAGS) -c -k Filter2DKernel  $(KERNEL_INCLUDES) $(KERNEL_SRC_CPP) -o $@
# Link Kernel
$(BUILD_DIR)/$(XCLBIN): $(BUILD_DIR)/$(XO_NAME).xo
	v++ $(VPPFLAGS) -l -o $@ $(BUILD_DIR)/$(XO_NAME).xo

############## Emulation Files Generation
EMCONFIG_FILE = emconfig.json

$(BUILD_DIR)/$(EMCONFIG_FILE):
	 emconfigutil --nd 1  --platform $(PLATFORM) --od $(BUILD_DIR)

############## primary build targets

.PHONY: all clean  create_dirs

############## build the design without running host application
build: host xclbin emconfig
pre_build: host emconfig
host: $(BUILD_DIR)/$(HOST_EXE)
xclbin: $(BUILD_DIR)/$(XCLBIN)
emconfig: $(BUILD_DIR)/$(EMCONFIG_FILE)
############## build the design and then run host application
ifneq  ($(TARGET),hw)
run: build
	cp xrt.ini $(BUILD_DIR);
	cd $(BUILD_DIR) && XCL_EMULATION_MODE=$(TARGET) ./host.exe  -x ./$(XCLBIN) $(PROFILE_OPTIONS) -f $(FILTER_TYPE) -r $(PARALLEL_ENQ_REQS) -n $(NUM_IMAGES)  $(INPUT_IMAGE_OPTION)
else
ifeq ($(USE_PRE_BUILT_XCLBIN),1)
run: pre_build
	cp xrt.ini $(BUILD_DIR);
	cd $(BUILD_DIR) && unset XCL_EMULATION_MODE && ./host.exe  -x  $(PRE_BUILT_XCLBIN_PATH)  $(PROFILE_OPTIONS) -f $(FILTER_TYPE) -r $(PARALLEL_ENQ_REQS) -n $(NUM_IMAGES)  $(INPUT_IMAGE_OPTION)
else
run: build
	cp xrt.ini $(BUILD_DIR);
	cd $(BUILD_DIR) && unset XCL_EMULATION_MODE && ./host.exe  -x ./$(XCLBIN) $(PROFILE_OPTIONS) -f $(FILTER_TYPE) -r $(PARALLEL_ENQ_REQS) -n $(NUM_IMAGES)  $(INPUT_IMAGE_OPTION)
endif
endif


## Clean generated files
clean:
	rm -rf $(BUILD_DIR) host*.exe
cleanall:
	@echo "Cleaning Software Emulation related host and kernel build file ...."
	rm -rf ./build_sw_emu host*.exe
	@echo "Cleaning Hardware Emulationrelated host and kernel build file ...."
	rm -rf ./build_hw_emu host*.exe
	@echo "Cleaning Hardware/System run related host and kernel buil file...."
	rm -rf ./build host*.exe
	rm -rf ./.Xil
	rm  -rf xcd.log


