
all: package/sd_card.img

app.exe: ../../src/host.cpp
	arm-linux-gnueabihf-g++ -Wall -g -std=c++11 ../../src/host.cpp -o app.exe \
		-I${SYSROOT}/usr/include/xrt \
		-L${SYSROOT}/usr/lib -lOpenCL -lpthread -lrt -lstdc++ --sysroot=${SYSROOT}

vadd.xo: ../../src/vadd.cpp
	v++ -c -t ${TARGET} --config ../../src/zc706.cfg -k vadd -I../../src ../../src/vadd.cpp -o vadd.xo 

vadd.xclbin: ./vadd.xo
	v++ -l -t ${TARGET} --config ../../src/zc706.cfg ./vadd.xo -o vadd.xclbin

package/sd_card.img: app.exe emconfig.json vadd.xclbin xrt.ini run_app.sh
	v++ -p -t ${TARGET} --config ../../src/zc706.cfg ./vadd.xclbin \
		--package.out_dir package \
		--package.rootfs ${ROOTFS}/rootfs.ext4 \
		--package.sd_file ${ROOTFS}/uImage \
		--package.sd_file xrt.ini \
		--package.sd_file emconfig.json \
		--package.sd_file app.exe \
		--package.sd_file run_app.sh

emconfig.json:
	emconfigutil --platform xilinx_zc706_base_202010_1 --nd 1

clean:
	rm -rf vadd* app.exe *json *csv *log *summary _x package *.json .run .Xil .ipcache *.jou

# Unless specified, use the current directory name as the v++ build target
TARGET ?= $(notdir $(CURDIR))
