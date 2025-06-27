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
   	int error=0;
	try{
		// Open xclbin
		auto device = xrt::device(0); //device index=0
		std::cout<<"Open device successfully"<<std::endl;
    	std::string xclbinFilename = argv[1];
		//xclbin related to partition 0
		auto xclbin_1 = xrt::xclbin(xclbinFilename);
		auto uuid1 = device.register_xclbin(xclbin_1);
		xrt::hw_context *hwctx_1=new xrt::hw_context(device, uuid1);
		std::cout<<"Load XCLBIN successfully"<<std::endl;

		auto din_buffer = new xrt::aie::bo (*hwctx_1, BLOCK_SIZE_in_Bytes,xrt::bo::flags::normal, /*memory group*/0); //Only non-cacheable buffer is supported
		int* dinArray= din_buffer->map<int*>();
		auto dout_buffer = new xrt::aie::bo (*hwctx_1, BLOCK_SIZE_in_Bytes,xrt::bo::flags::normal, /*memory group*/0); //Only non-cacheable buffer is supported
		int* doutArray= dout_buffer->map<int*>();
    	int* doutRef=(int*)malloc(BLOCK_SIZE_in_Bytes);
    	std::cout<<"Allocate buffer completed"<<std::endl;

    	int coeff[8]={1,2,3,4,5,6,7,8};
	
    	for(int i=0;i<ITERATION*1024/4;i++){
			dinArray[i]=i;
    	}

		auto ghdl=new xrt::graph(*hwctx_1,"gr");
		std::cout<<"Open partition 0 with graph1 successfully"<<std::endl;
		xrt::aie::buffer *bufIn=new xrt::aie::buffer(*hwctx_1, "gr.gmioIn");
		//memory group is 0, depending on the platform
		bufIn->async(*din_buffer, XCL_BO_SYNC_BO_GMIO_TO_AIE, BLOCK_SIZE_in_Bytes, 0); 
    	ghdl->run(ITERATION);
		xrt::aie::buffer *bufOut=new xrt::aie::buffer(*hwctx_1, "gr.gmioOut");
		//memory group is 0, depending on the platform
		bufOut->async(*dout_buffer, XCL_BO_SYNC_BO_AIE_TO_GMIO, BLOCK_SIZE_in_Bytes, 0);
    	//PS can do other tasks here when data is transferring
    	std::cout<<"Waiting for graph to be completed"<<std::endl;
		bufOut->wait();

    	ref_func(dinArray,coeff,doutRef,ITERATION*1024/4);
    	for(int i=0;i<ITERATION*1024/4;i++){
			if(doutArray[i]!=doutRef[i]){
				std::cout<<"ERROR:dout["<<i<<"]="<<doutArray[i]<<",gold="<<doutRef[i]<<std::endl;
				error++;
			}
    	}
    
    	std::cout<<"GMIO transactions finished"<<std::endl;

		ghdl->end();
    	if(error==0){
			std::cout<<"TEST PASSED!"<<std::endl;
    	}else{
			std::cout<<"ERROR!"<<std::endl;
    	}
		//delete the objects that is associated to the hw context, and then delete the hw context.
		delete bufIn;
		delete bufOut;
		delete din_buffer;
		delete dout_buffer;
		delete ghdl;
		delete hwctx_1;
		error=0;

		std::cout<<"Make sure the context is deleted successfully before starting again."<<std::endl;
		std::cout<<"reloading partition 0 with graph2"<<std::endl;
		//xclbin related to partition 0
		std::string xclbinFilename2 = argv[2];
		std::cout<<"xclbin file name:"<<xclbinFilename2<<std::endl;
		auto xclbin_2 = xrt::xclbin(xclbinFilename2);
		auto uuid2 = device.register_xclbin(xclbin_2);
		xrt::hw_context *hwctx_2=new xrt::hw_context(device, uuid2);
		std::cout<<"Load XCLBIN2 successfully"<<std::endl;
		
		//memory group is 0, depending on the platform
		auto din_buffer2 = new xrt::aie::bo (*hwctx_2, BLOCK_SIZE_in_Bytes,xrt::bo::flags::normal, 0); //Only non-cacheable buffer is supported
		int* dinArray2 = din_buffer2->map<int*>();
		//memory group is 0, depending on the platform
		auto dout_buffer2 = new xrt::aie::bo (*hwctx_2, BLOCK_SIZE_in_Bytes,xrt::bo::flags::normal, 0); //Only non-cacheable buffer is supported
		int* doutArray2= dout_buffer2->map<int*>();
    	std::cout<<"Allocate buffer2 completed"<<std::endl;
		for(int i=0;i<ITERATION*1024/4;i++){
			dinArray2[i]=i;
    		}		

		auto ghdl2=new xrt::graph(*hwctx_2,"gr");
		std::cout<<"Open pr0 graph2 successfully"<<std::endl;
		xrt::aie::buffer *bufIn2=new xrt::aie::buffer(*hwctx_2, "gr.gmioIn");
		bufIn2->async(*din_buffer2, XCL_BO_SYNC_BO_GMIO_TO_AIE, BLOCK_SIZE_in_Bytes, 0);
    	ghdl2->run(ITERATION);
		xrt::aie::buffer *bufOut2=new xrt::aie::buffer(*hwctx_2, "gr.gmioOut");
		bufOut2->async(*dout_buffer2, XCL_BO_SYNC_BO_AIE_TO_GMIO, BLOCK_SIZE_in_Bytes, 0);
    	//PS can do other tasks here when data is transferring
    	std::cout<<"Waiting for graph to be completed"<<std::endl;
		bufOut2->wait();

    		for(int i=0;i<ITERATION*1024/4;i++){
			if(doutArray2[i]!=dinArray2[i]+1){
				std::cout<<"ERROR:dout["<<i<<"]="<<doutArray2[i]<<",gold="<<dinArray2[i]+1<<std::endl;
				error++;
			}
		}
    		std::cout<<"GMIO transactions finished"<<std::endl;
		ghdl2->end();

		if(error==0){
			std::cout<<"TEST PR0 Graph2 PASSED!"<<std::endl;
    	}else{
			std::cout<<"TEST PR0 Graph2 ERROR!"<<std::endl;
    	}
		delete bufIn2;
		delete bufOut2;
		delete din_buffer2;
		delete dout_buffer2;
		delete ghdl2;
		delete hwctx_2;
	}catch(std::exception const& e){  
		std::cout<<"Execution Error:"<<e.what()<<std::endl;  
		return 1;	
	}

    return error;
};
