PLATFORM ?= xilinx_zcu104_vcu_202010_1
CWD=$(shell pwd)
XSA_DIR ?= $(CWD)/vivado
OUTPUT_PATH ?= $(CWD)/platform_repo
XSCT = $(XILINX_VITIS)/bin/xsct
CPU_ARCH=a53
export COMMON_RFS_KRNL_SYSROOT = FALSE

.phony: all

$(PLATOFORM): all
all: $(PLATFORM)

$(PLATFORM): xsa petalinux_proj peta_sysroot pfm
	@echo 'Platform build complete'

xsa:
	$(MAKE) -C vivado PLATFORM=$(PLATFORM)

petalinux_proj: ./vivado/xilinx_zcu104_vcu_202010_1.xsa
	$(MAKE) -C petalinux all XSA_DIR=$(XSA_DIR) PLATFORM=$(PLATFORM)

peta_sysroot:
	$(MAKE) -C petalinux sysroot

ifeq ($(COMMON_RFS_KRNL_SYSROOT), TRUE)
pfm:
	$(XSCT) -nodisp -sdx scripts/${PLATFORM}_pfm.tcl ${PLATFORM} ${XSA_DIR} ${OUTPUT_PATH}
	mkdir platform_repo/${PLATFORM}/export/${PLATFORM}/sw/${PLATFORM}/pre-built/
	cp -rf platform_repo/tmp/sw_components/src/${CPU_ARCH}/BOOT.BIN platform_repo/${PLATFORM}/export/${PLATFORM}/sw/${PLATFORM}/pre-built/
else
pfm:
	$(XSCT) -nodisp -sdx scripts/${PLATFORM}_pfm.tcl ${PLATFORM} ${XSA_DIR} ${OUTPUT_PATH}
	mkdir platform_repo/${PLATFORM}/export/${PLATFORM}/sw/${PLATFORM}/pre-built/
	cp -rf platform_repo/tmp/sw_components/src/${CPU_ARCH}/BOOT.BIN platform_repo/${PLATFORM}/export/${PLATFORM}/sw/${PLATFORM}/pre-built/
	cp -rf platform_repo/tmp/sw_components/src/${CPU_ARCH}/xrt/filesystem platform_repo/${PLATFORM}/export/${PLATFORM}/sw/${PLATFORM}/
	rm -rf ../../repo
	mkdir -p ../../repo/${PLATFORM}
	mv platform_repo/${PLATFORM}/export/${PLATFORM} ../../repo
	mv platform_repo/sysroot ../../repo/${PLATFORM}/sysroot
	mv petalinux/images/linux/sdk.sh ../../repo/${PLATFORM}/sysroot
endif

clean:
	${RM} -r platform_repo
	$(MAKE) -C vivado clean PLATFORM=$(PLATFORM)
	$(MAKE) -C petalinux ultraclean PLATFORM=$(PLATFORM)
