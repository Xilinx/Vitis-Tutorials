
all: app.exe vadd.xclbin

app.exe: ../../src/host.cpp
	g++ -Wall -g -std=c++11 ../../src/host.cpp -o app.exe \
		-I${XILINX_XRT}/include/ \
		-L${XILINX_XRT}/lib/ -lOpenCL -lpthread -lrt -lstdc++
	
vadd.xo: ../../src/vadd.cpp
	v++ -c -t ${TARGET} --config ../../src/u200.cfg -k vadd -I../../src ../../src/vadd.cpp -o vadd.xo 

vadd.xclbin: ./vadd.xo
	v++ -l -t ${TARGET} --config ../../src/u200.cfg ./vadd.xo -o vadd.xclbin

clean:
	rm -rf vadd* app.exe *json *csv *log *summary _x .run .Xil .ipcache *.jou

# Unless specified, use the current directory name as the v++ build target
TARGET ?= $(notdir $(CURDIR))