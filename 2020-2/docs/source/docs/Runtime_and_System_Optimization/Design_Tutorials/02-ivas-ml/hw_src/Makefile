PFM_REPO ?= ../platform/repo

IP_CACHE_DIR ?= ./ip_cache
PLATFORM := xilinx_zcu104_vcu_202010_1

ROOTFS_FILE := $(PFM_REPO)/$(PLATFORM)/sw/$(PLATFORM)/filesystem/rootfs.ext4
PLATFORM_FILE := $(PFM_REPO)/$(PLATFORM)/$(PLATFORM).xpfm

TARGET ?= hw

NCPUS := $(shell grep -c ^processor /proc/cpuinfo)
JOBS := $(shell expr $(NCPUS) - 1)

PRJ_DIR := $(shell pwd)

ZCU104_XOCCFLAGS := --platform $(PLATFORM_FILE) -t $(TARGET) -s
ifneq ($(TARGET),hw)
	ZCU104_XOCCFLAGS += -g
endif
ZCU104_XOCCLFLAGS := --config zcu10x_config --xp param:compiler.userPostSysLinkOverlayTcl=${PRJ_DIR}/scripts/strip_interconnects.tcl

RESIZE_FLAGS := --kernel resize_accel_rgb -I. -I./Vitis_Libraries/vision/L1/include

MULTISCALER_FLAGS := --kernel v_multi_scaler -I. -I./hls

#ZCU104_XOS = resize_rgb.xo dpu_b4096_zcu104.xo yuv_crop_accel.xo nv122rgb_multicrop_resize.xo
ZCU104_XOS = resize_rgb.xo dpu_b4096_zcu104.xo v_multi_scaler.xo

### DPU Information

# Kernel name must match kernel name in kernel.xml
KERN_NAME := DPUCZDX8G
dpu_HDLSRCS=kernel_xml/dpu/kernel.xml\
             scripts/package_dpu_kernel.tcl\
             scripts/gen_dpu_xo.tcl\
             ./dpu_conf_zcu104.vh\
             ./dpu_ip/Vitis/dpu/hdl/DPUCZDX8G.v\
             ./dpu_ip/Vitis/dpu/inc/arch_def.vh\
             ./dpu_ip/Vitis/dpu/xdc/*.xdc\
             ./dpu_ip/DPUCZDX8G_*/hdl/DPUCZDX8G_*_dpu.sv\
             ./dpu_ip/DPUCZDX8G_*/inc/function.vh\
             ./dpu_ip/DPUCZDX8G_*/inc/arch_para.vh

dpu_TCL=scripts/gen_dpu_xo.tcl

.phony: clean traces zcu104 package

zcu104: XOCCFLAGS = $(ZCU104_XOCCFLAGS)
zcu104: dpu.xclbin package

dpu.xclbin: $(ZCU104_XOS)
	v++ -l $(ZCU104_XOCCFLAGS) $(ZCU104_XOCCLFLAGS) \
		--config zcu104_vivado.ini \
		-j $(JOBS) \
		--remote_ip_cache ${IP_CACHE_DIR} \
		-o $@ $(ZCU104_XOS)
	cp _x/link/vivado/vpl/prj/prj.srcs/sources_1/bd/*/hw_handoff/*.hwh ./project.hwh

package: dpu.xclbin
	v++ -p $(ZCU104_XOCCFLAGS) \
		dpu.xclbin \
		--package.out_dir sd_card_zcu104 \
		--package.rootfs $(ROOTFS_FILE)

dpu_b4096_zcu104.xo: $(dpu_HDLSRCS) dpu_conf_zcu104.vh
	cp dpu_conf_zcu104.vh dpu_conf.vh
	vivado -mode batch -source ${dpu_TCL} -tclargs $@ $(KERN_NAME) ${TARGET} zcu104

resize_rgb.xo: resize_rgb.cpp
	v++ $(XOCCFLAGS) $(RESIZE_FLAGS) --config resize_config.ini -c -o $@ $<

v_multi_scaler.xo: v_multi_scaler.cpp v_hresampler.cpp v_hscaler.cpp v_dma.cpp v_csc.cpp v_vresampler.cpp v_vscaler.cpp
	v++ $(XOCCFLAGS) $(MULTISCALER_FLAGS) -c -o $@ $^

clean:
	$(RM) -r *.xo *_x .Xil sd_card* *.xclbin *.ltx *.log *.info packaged_kernel* tmp_kernel* vivado* pfm_sw dpu_conf.vh
	$(RM) *summary* *.str *.hwh

boot_zcu104:
	$(RM) -r ./pfm_sw
	cp -r $(PFM_REPO)/$(PLATFORM)/sw/$(PLATFORM)/boot ./pfm_sw
	bootgen -arch zynqmp -w on -image bootgen_zcu104.bif -o sd_card_zcu104/BOOT.BIN
