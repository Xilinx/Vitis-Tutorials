/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <unistd.h>
#include <fstream>
#include "xrt/xrt_kernel.h"
#include "xrt/xrt_graph.h"
#include "xrt/xrt_aie.h"

void ref_func(int* din,int c[8],int* dout,int size){
	static int d[8]={0,0,0,0,0,0,0,0};
	for(int i=0;i<size;i++){
		for(int j=0;j<7;j++){
			d[j]=d[j+1];
		}
		d[7]=*(din+i);
		int tmp=0;
		for(int j=0;j<8;j++){
			tmp+=d[j]*c[j];
		}
		*(dout+i)=tmp;
	}
}
const int ITERATION=1;
const int BLOCK_SIZE_in_Bytes=ITERATION*1024;
int main(int argc, char ** argv) {
    // Create XRT device handle for ADF API
    std::string xclbinFilename = argv[1];
   	int error=0;
	std::ofstream myfile("DATA");
	myfile << "HELLO World";
	
	try{
		// Open xclbin
		auto device = xrt::device(0); //device index=0
		std::cout<<"Open device successfully"<<std::endl;
		auto xclbin_1 = xrt::xclbin(xclbinFilename);
		auto uuid = device.register_xclbin(xclbin_1);
		xrt::hw_context hwctx_1{device, uuid};
		std::cout<<"Load XCLBIN successfully"<<std::endl;

		auto din_buffer = xrt::aie::bo (hwctx_1, BLOCK_SIZE_in_Bytes,xrt::bo::flags::normal, /*memory group*/0); //Only non-cacheable buffer is supported
		int* dinArray= din_buffer.map<int*>();
		auto dout_buffer = xrt::aie::bo (hwctx_1, BLOCK_SIZE_in_Bytes,xrt::bo::flags::normal, /*memory group*/0); //Only non-cacheable buffer is supported
		int* doutArray= dout_buffer.map<int*>();
    	int* doutRef=(int*)malloc(BLOCK_SIZE_in_Bytes);
    	std::cout<<"Allocate buffer completed"<<std::endl;

    	int coeff[8]={1,2,3,4,5,6,7,8};
	
    	for(int i=0;i<ITERATION*1024/4;i++){
			dinArray[i]=i;
    	}

		auto ghdl=xrt::graph(hwctx_1,"gr");
		std::cout<<"Open graph pr0 successfully"<<std::endl;
		xrt::aie::buffer bufIn(hwctx_1, "gr.gmioIn");
		bufIn.async(din_buffer, XCL_BO_SYNC_BO_GMIO_TO_AIE, BLOCK_SIZE_in_Bytes, 0);
    	ghdl.run(ITERATION);
		xrt::aie::buffer bufOut(hwctx_1, "gr.gmioOut");
		bufOut.async(dout_buffer, XCL_BO_SYNC_BO_AIE_TO_GMIO, BLOCK_SIZE_in_Bytes, 0);
    	//PS can do other tasks here when data is transferring
    	std::cout<<"Waiting for graph to be completed"<<std::endl;
		bufOut.wait();

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
	}catch(std::exception const& e){  
		std::cout<<"Execution Error:"<<e.what()<<std::endl;  
		return 1;	
	}

    return error;
};
