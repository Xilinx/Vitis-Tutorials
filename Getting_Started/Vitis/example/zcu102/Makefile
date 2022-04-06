ndef = $(if $(value $(1)),,$(error $(1) must be set prior to running))

XF_PROJ_ROOT ?= $(shell bash -c 'export MK_PATH=$(MK_PATH); echo $${MK_PATH%/Getting_Started/Vitis/example/src/*}')

TARGET := hw_emu
PLATFORM := xilinx_zcu102_base_202120_1
SRCDIR := $(XF_PROJ_ROOT)/Getting_Started/Vitis/example/src
BUILD_DIR := $(TARGET)
EMBEDDED_EXEC_SCRIPT := run_$(TARGET).sh
EMBEDDED_PACKAGE_OUT := $(BUILD_DIR)package/sd_card.img
SDKTARGETSYSROOT := $(SYSROOT)
ROOTFS ?= $(EDGE_COMMON_SW)

run: build
ifeq ($(TARGET),hw)
	#cd $(BUILD_DIR) && ./app.exe
	echo "Copy the contents of the sd_card folder to an SD card, and run './app.exe' on the target platform."
else
	#cd $(BUILD_DIR) && XCL_EMULATION_MODE=$(TARGET) ./app.exe
	cd $(BUILD_DIR)/package && launch_$(TARGET).sh -no-reboot -run-app $(EMBEDDED_EXEC_SCRIPT)
endif

#build: host emconfig xclbin
build: $(BUILD_DIR)/app.exe $(BUILD_DIR)/vadd.xclbin $(BUILD_DIR)/package/sd_card.img

host: $(BUILD_DIR)/app.exe
$(BUILD_DIR)/app.exe:
	$(call ndef,SDKTARGETSYSROOT)
	mkdir -p $(BUILD_DIR)
	cp xrt.ini $(BUILD_DIR)
	cp run_$(TARGET).sh $(BUILD_DIR)
	$(CXX) -Wall -g -std=c++11 $(SRCDIR)/host.cpp -o $(BUILD_DIR)/app.exe \
		-I/usr/include/xrt \
		-lOpenCL -lpthread -lrt -lstdc++

xo: $(BUILD_DIR)/vadd.xo
$(BUILD_DIR)/vadd.xo:
	v++ -c -t ${TARGET} --platform $(PLATFORM) --config $(SRCDIR)/zcu102.cfg -k vadd -I$(SRCDIR) $(SRCDIR)/vadd.cpp -o $(BUILD_DIR)/vadd.xo 

xclbin: $(BUILD_DIR)/vadd.xclbin
$(BUILD_DIR)/vadd.xclbin: $(BUILD_DIR)/vadd.xo
	v++ -l -t ${TARGET} --platform $(PLATFORM) --config $(SRCDIR)/zcu102.cfg $(BUILD_DIR)/vadd.xo -o $(BUILD_DIR)/vadd.xclbin

package: $(BUILD_DIR)/package/sd_card.img
$(BUILD_DIR)/package/sd_card.img: $(BUILD_DIR)/app.exe $(BUILD_DIR)/vadd.xclbin
	$(call ndef,ROOTFS)
	cd $(BUILD_DIR) && v++ -p -t ${TARGET} --platform $(PLATFORM) --config $(SRCDIR)/zcu102.cfg ./vadd.xclbin \
		--package.out_dir ./package \
		--package.rootfs ${ROOTFS}/rootfs.ext4 \
		--package.sd_file ./vadd.xclbin \
		--package.sd_file ${ROOTFS}/Image \
		--package.sd_file ./xrt.ini \
		--package.sd_file ./app.exe \
		--package.sd_file ./run_$(TARGET).sh

clean:
	rm -rf $(BUILD_DIR) a.xlcbin vadd* app.exe *json opencl* *log *summary _x xilinx* .run .Xil .ipcache *.jou

# Unless specified, use the current directory name as the v++ build target
#TARGET ?= $(notdir $(CURDIR))
