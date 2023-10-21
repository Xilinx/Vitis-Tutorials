/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <adf.h>
#include <unistd.h>
#include <fstream>
#include "xrt/xrt_kernel.h"
#include "xrt/xrt_graph.h"
#include "xrt/xrt_aie.h"

using namespace adf;

void ref_func(int32* din,int32 c[8],int32* dout,int size){
	static int32 d[8]={0,0,0,0,0,0,0,0};
	for(int i=0;i<size;i++){
		for(int j=0;j<7;j++){
			d[j]=d[j+1];
		}
		d[7]=*(din+i);
		int32 tmp=0;
		for(int j=0;j<8;j++){
			tmp+=d[j]*c[j];
		}
		*(dout+i)=tmp;
	}
}
const int ITERATION=4;
const int BLOCK_SIZE_in_Bytes=ITERATION*1024;
int main(int argc, char ** argv) {
    // Create XRT device handle for ADF API
    char* xclbinFilename = argv[1];
	// Open xclbin
	auto device = xrt::device(0); //device index=0
	auto uuid = device.load_xclbin(xclbinFilename);

	auto din_buffer = xrt::aie::bo (device, BLOCK_SIZE_in_Bytes,xrt::bo::flags::normal, /*memory group*/0); //Only non-cacheable buffer is supported
	int* dinArray= din_buffer.map<int*>();
	auto dout_buffer = xrt::aie::bo (device, BLOCK_SIZE_in_Bytes,xrt::bo::flags::normal, /*memory group*/0); //Only non-cacheable buffer is supported
	int* doutArray= dout_buffer.map<int*>();
    int32* doutRef=(int32*)malloc(BLOCK_SIZE_in_Bytes);
    std::cout<<"GMIO::malloc completed"<<std::endl;

    int error=0;
    int32 coeff[8]={1,2,3,4,5,6,7,8};

    for(int i=0;i<ITERATION*1024/4;i++){
		dinArray[i]=i;
    }

	auto ghdl=xrt::graph(device,uuid,"gr");
	din_buffer.async("gr.gmioIn",XCL_BO_SYNC_BO_GMIO_TO_AIE,BLOCK_SIZE_in_Bytes,/*offset*/0);
    ghdl.run(ITERATION);
	auto dout_buffer_run=dout_buffer.async("gr.gmioOut",XCL_BO_SYNC_BO_AIE_TO_GMIO,BLOCK_SIZE_in_Bytes,/*offset*/0);
    //PS can do other tasks here when data is transferring
    dout_buffer_run.wait();//Wait for gmioOut to complete

    ref_func(dinArray,coeff,doutRef,ITERATION*1024/4);
    for(int i=0;i<ITERATION*1024/4;i++){
		if(doutArray[i]!=doutRef[i]){
			std::cout<<"ERROR:dout["<<i<<"]="<<doutArray[i]<<",gold="<<doutRef[i]<<std::endl;
			error++;
		}
    }
    
    std::cout<<"GMIO transactions finished"<<std::endl;

	ghdl.end();
    if(error==0){
		std::cout<<"TEST PASSED!"<<std::endl;
    }else{
		std::cout<<"ERROR!"<<std::endl;
    }

    return error;
};
