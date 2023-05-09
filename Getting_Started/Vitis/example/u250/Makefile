XF_PROJ_ROOT ?= $(shell bash -c 'export MK_PATH=$(MK_PATH); echo $${MK_PATH%/Getting_Started/Vitis/example/src/*}')

TARGET := hw_emu
PLATFORM ?= xilinx_u250_gen3x16_xdma_4_1_202210_1
SRCDIR := $(XF_PROJ_ROOT)/Getting_Started/Vitis/example/src
BUILD_DIR := $(TARGET)
CXXFLAGS += -g -std=c++17 -Wall -O0

run: build
ifeq ($(TARGET),hw)
	cp xrt.ini $(BUILD_DIR)
	cd $(BUILD_DIR) && ./app.exe
else
	cp xrt.ini $(BUILD_DIR)
	cd $(BUILD_DIR) && XCL_EMULATION_MODE=$(TARGET) ./app.exe
endif

build: host emconfig xclbin
#build: $(BUILD_DIR)/app.exe $(BUILD_DIR)/emconfig.json $(BUILD_DIR)/vadd.xclbin

host: $(BUILD_DIR)/app.exe
$(BUILD_DIR)/app.exe:
	mkdir -p $(BUILD_DIR)
	g++ $(CXXFLAGS) $(SRCDIR)/host.cpp -o $(BUILD_DIR)/app.exe \
		-I${XILINX_XRT}/include/ \
                -L$(XILINX_XRT)/lib -lxrt_coreutil -pthread

xo: $(BUILD_DIR)/vadd.xo
$(BUILD_DIR)/vadd.xo:
	v++ -c -t ${TARGET} --platform $(PLATFORM) --config $(SRCDIR)/u250.cfg -k vadd -I$(SRCDIR) $(SRCDIR)/vadd.cpp -o $(BUILD_DIR)/vadd.xo 

xclbin: $(BUILD_DIR)/vadd.xclbin
$(BUILD_DIR)/vadd.xclbin: $(BUILD_DIR)/vadd.xo
	v++ -l -t ${TARGET} --platform $(PLATFORM) --config $(SRCDIR)/u250.cfg $(BUILD_DIR)/vadd.xo -o $(BUILD_DIR)/vadd.xclbin

emconfig: $(BUILD_DIR)/emconfig.json
$(BUILD_DIR)/emconfig.json:
	emconfigutil --platform $(PLATFORM) --od $(BUILD_DIR) --nd 1

clean:
	rm -rf $(BUILD_DIR) vadd* app.exe *json opencl* *log *summary _x xilinx* .run .Xil .ipcache *.jou

# Unless specified, use the current directory name as the v++ build target
#TARGET ?= $(notdir $(CURDIR))
