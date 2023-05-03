.PHONY: help clean run
help: 
	@echo  " Makefile Usage: "
	@echo  ""
	@echo  "  make all "
	@echo  "  Command to generate the hardware and software files to test the application on the board"
	@echo  ""
	@echo  "  make <sw_emu/hw_emu> "
	@echo  "  Command to generate and run the design for specified target"
	@echo  ""
	@echo  "  make <sw/hw> "
	@echo  "  Command to generate the design for specified target"
	@echo  ""
	@echo  "  make clean "
	@echo  "  Command to remove the generated hardware and software files "
	@echo  ""
OPENCV_LIBS := $(shell pkg-config --libs opencv)
TARGET := hw
#PLATFORM := xilinx_u250_gen3x16_xdma_4_1_202210_1
LIB_PATH = ./design_source/hw_src/Vitis_Libraries
XCLBIN := ./design_source/hw_src/alveo_example.xclbin

all: | xclbin sw
run : all
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
ifeq ($(OPENCV_LIBS),)
	cd ./design_source/build; XCL_EMULATION_MODE=$(TARGET) ./00_load_kernels alveo_example.xclbin
	cd ./design_source/build; XCL_EMULATION_MODE=$(TARGET) ./01_simple_malloc alveo_example.xclbin
	cd ./design_source/build; XCL_EMULATION_MODE=$(TARGET) ./02_aligned_malloc alveo_example.xclbin
	cd ./design_source/build; XCL_EMULATION_MODE=$(TARGET) ./03_buffer_map alveo_example.xclbin
	cd ./design_source/build; XCL_EMULATION_MODE=$(TARGET) ./04_wide_vadd alveo_example.xclbin
	cd ./design_source/build; XCL_EMULATION_MODE=$(TARGET) ./05_pipelined_vadd alveo_example.xclbin
	cd ./design_source/build; XCL_EMULATION_MODE=$(TARGET) ./06_wide_processor alveo_example.xclbin
else 
	cd ./design_source/build; XCL_EMULATION_MODE=$(TARGET) ./00_load_kernels alveo_example.xclbin
        cd ./design_source/build; XCL_EMULATION_MODE=$(TARGET) ./01_simple_malloc alveo_example.xclbin
        cd ./design_source/build; XCL_EMULATION_MODE=$(TARGET) ./02_aligned_malloc alveo_example.xclbin
        cd ./design_source/build; XCL_EMULATION_MODE=$(TARGET) ./03_buffer_map alveo_example.xclbin
        cd ./design_source/build; XCL_EMULATION_MODE=$(TARGET) ./04_wide_vadd alveo_example.xclbin
        cd ./design_source/build; XCL_EMULATION_MODE=$(TARGET) ./05_pipelined_vadd alveo_example.xclbin
        cd ./design_source/build; XCL_EMULATION_MODE=$(TARGET) ./06_wide_processor alveo_example.xclbin
        cd ./design_source/build; XCL_EMULATION_MODE=$(TARGET) ./07_opencv_resize ../test_data/fish.jpg
        cd ./design_source/build; XCL_EMULATION_MODE=$(TARGET) ./08_opencv_resize_blur ../test_data/fish.jpg
endif
else
ifeq ($(OPENCV_LIBS),)
	cd ./design_source/build; ./00_load_kernels alveo_example.xclbin
	cd ./design_source/build; ./01_simple_malloc alveo_example.xclbin
	cd ./design_source/build; ./02_aligned_malloc alveo_example.xclbin
	cd ./design_source/build; ./03_buffer_map alveo_example.xclbin
	cd ./design_source/build; ./04_wide_vadd alveo_example.xclbin
	cd ./design_source/build; ./05_pipelined_vadd alveo_example.xclbin
	cd ./design_source/build; ./06_wide_processor alveo_example.xclbin
else
	cd ./design_source/build; ./00_load_kernels alveo_example.xclbin
        cd ./design_source/build; ./01_simple_malloc alveo_example.xclbin
        cd ./design_source/build; ./02_aligned_malloc alveo_example.xclbin
        cd ./design_source/build; ./03_buffer_map alveo_example.xclbin
        cd ./design_source/build; ./04_wide_vadd alveo_example.xclbin
        cd ./design_source/build; ./05_pipelined_vadd alveo_example.xclbin
        cd ./design_source/build; ./06_wide_processor alveo_example.xclbin
        cd ./design_source/build; ./07_opencv_resize ../test_data/fish.jpg
        cd ./design_source/build; ./08_opencv_resize_blur ../test_data/fish.jpg
endif
endif

xclbin: $(LIB_PATH)
	make TARGET=$(TARGET) -C ./design_source/hw_src

design_source/hw_src/Vitis_Libraries:
	git clone https://github.com/Xilinx/Vitis_Libraries.git ./design_source/hw_src/Vitis_Libraries

sw_emu: | xclbin sw
hw_emu: | xclbin sw
hw: xclbin	

sw:
	mkdir ./design_source/build
ifeq ($(TARGET),hw_emu)
	cd ./design_source/build; cmake -DCMAKE_CXX_FLAGS="-DHW_EMU_TEST" ..
	CPPFLAGS += -D HW_EMU_TEST; export HW_EMU_TEST;	make -j -C ./design_source/build
else
	cd ./design_source/build; cmake ..
	make -j -C ./design_source/build
endif
	emconfigutil --platform $(PLATFORM) --od ./design_source/build

clean:
	rm -rf ./design_source/build
	make clean -C ./design_source/hw_src
