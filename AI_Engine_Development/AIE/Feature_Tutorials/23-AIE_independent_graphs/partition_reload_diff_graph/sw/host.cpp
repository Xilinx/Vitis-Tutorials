/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <unistd.h>
#include <fstream>
#include "xrt/xrt_kernel.h"
#include "xrt/xrt_graph.h"
#include "xrt/xrt_aie.h"
#include <iostream>
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
int run_gmio(xrt::device &device, std::string &xclbinfile_gr1, std::string &xclbinfile_gr2){
	const int ITERATION=1;
	const int BLOCK_SIZE_in_Bytes=ITERATION*1024;
    // Create XRT device handle for ADF API
   	int error=0;
	try{
		//xclbin related to partition 0
		auto xclbin_1 = xrt::xclbin(xclbinfile_gr1);
		auto uuid1 = device.register_xclbin(xclbin_1);
		std::cout<<"xclbin file name:"<<xclbinfile_gr1<<std::endl;
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
		bufIn->wait();
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
		std::cout<<"xclbin file name:"<<xclbinfile_gr2<<std::endl;
		auto xclbin_2 = xrt::xclbin(xclbinfile_gr2);
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
		bufIn2->wait();
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
}

int run_rtp(xrt::device &device, std::string &xclbinFilename_pl, std::string &xclbinFilename_aie, int rtp_type){
	const int ITERATION=4;
	const int ELEM_per_iter=256;
	size_t output_size_in_bytes = ELEM_per_iter*4*ITERATION;
	int ret;
	auto xclbin_pl = xrt::xclbin(std::string(xclbinFilename_pl));
	auto uuid_pl = device.register_xclbin(xclbin_pl);
	xrt::hw_context hwctx_pl{device, uuid_pl};
	auto xclbin_aie = xrt::xclbin(std::string(xclbinFilename_aie));
	auto uuid_aie = device.register_xclbin(xclbin_aie);
	xrt::hw_context hwctx_aie{device, uuid_aie};

	// s2mm & datagen kernel handle
	auto s2mm = xrt::kernel(hwctx_pl, "s2mm:{s2mm_1}");
	auto datagen = xrt::kernel(hwctx_pl, "datagen");

	// output memory
	auto out_bo = xrt::bo(hwctx_pl, output_size_in_bytes,static_cast<xrt::bo::flags>(0),s2mm.group_id(0));
	auto host_out=out_bo.map<int*>();

	//kernel run
	auto s2mm_run = s2mm(out_bo, nullptr, ELEM_per_iter*ITERATION);//1st run for s2mm has started
	auto datagen_run = datagen(nullptr, ELEM_per_iter*ITERATION,0);

	auto ghdl=xrt::graph(hwctx_aie,"gr");
	ghdl.run(ITERATION);
	int value[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	if(rtp_type==0){
		ghdl.update("gr.k.in[1]",10);
	}else if(rtp_type==1){
		ghdl.update("gr.k.in[1]",value);
	}
	ghdl.end();

	s2mm_run.wait();
	out_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

	static int match = 0;	
	int base=10;
	for (int i = 0; i < ITERATION; i++) {
		for(int j=0;j<ELEM_per_iter;j++){
			if(rtp_type==0){
				if(host_out[ELEM_per_iter*i+j]!=base+i*ELEM_per_iter+j){
					match=1;
				}
			}else if(rtp_type==1){
				if(host_out[ELEM_per_iter*i+j]!=value[j%16]+i*ELEM_per_iter+j){
					match=1;
				}
			}
		}
	}

	if(match==0){
		std::cout<<"TEST PR1 PASSED!"<<std::endl;
   	}else{
		std::cout<<"TEST PR1 ERROR!"<<std::endl;
    }

	return match;
}

int run_perf(xrt::device &device, std::string &xclbinFilename_pl, std::string &xclbinFilename_aie){
	size_t iterations = 100;
	size_t bytes_per_iteration=128;
	size_t output_size_in_bytes = iterations * bytes_per_iteration;
	size_t OUTPUT_SIZE = output_size_in_bytes / 4; //32bits width interface

	int ret;
	// Open xclbin
	auto xclbin_pl = xrt::xclbin(std::string(xclbinFilename_pl));
	auto uuid_pl = device.register_xclbin(xclbin_pl);
	xrt::hw_context hwctx_pl{device, uuid_pl};
	auto xclbin_aie = xrt::xclbin(std::string(xclbinFilename_aie));
	auto uuid_aie = device.register_xclbin(xclbin_aie);
	xrt::hw_context hwctx_aie{device, uuid_aie};
	std::cout<<"Xclbin Open done"<<std::endl;

	// s2mm & mm2s kernel handle
	auto s2mm = xrt::kernel(hwctx_pl, "s2mm:{s2mm_2}");
	auto mm2s = xrt::kernel(hwctx_pl, "mm2s");
	std::cout<<"PL kernel creation done"<<std::endl;

	// output memory
	auto out_bo = xrt::bo(hwctx_pl, output_size_in_bytes,static_cast<xrt::bo::flags>(0), s2mm.group_id(0));
	auto host_out=out_bo.map<int*>();

	// input memory
	auto in_bo = xrt::bo(hwctx_pl, output_size_in_bytes,static_cast<xrt::bo::flags>(0), mm2s.group_id(0));
	auto host_in=in_bo.map<int*>();
	std::cout<<"In & Out buffer creation done"<<std::endl;

	// Initialization input memory
	for(int i=0;i<output_size_in_bytes/4;i++){
		host_in[i]=i;
	}

	//kernel run
	auto s2mm_run = s2mm(out_bo, nullptr, OUTPUT_SIZE);//1st run for s2mm has started
	auto mm2s_run = mm2s(in_bo, nullptr, OUTPUT_SIZE);
	std::cout<<"PL kernel launching done "<<std::endl;

	auto ghdl=xrt::graph(hwctx_aie,"gr");
	ghdl.run(iterations);
	std::cout<<"Graph run enqueue done"<<std::endl;
	ghdl.end();
	std::cout<<"Graph done"<<std::endl;
	s2mm_run.wait();
	std::cout<<"s2mm done"<<std::endl;

	out_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

	std::ofstream out("out.txt",std::ofstream::out);
	static int match = 0;
	for (int i = 0; i < OUTPUT_SIZE; i++) {
		if(host_in[i]*2!=host_out[i]){
			std::cout<<"ERROR: i="<<i<<" gold="<<host_in[i]*2<<" out="<<host_out[i]<<std::endl;
			match=1;
			break;
		}
		out<<host_out[i]<<std::endl;
	}
	out.close();
	if(match==0){
		std::cout<<"TEST PR2 PASSED!"<<std::endl;
   	}else{
		std::cout<<"TEST PR2 ERROR!"<<std::endl;
    }
	return match;
}


int main(int argc, char* argv[])
{
	try {
		if(argc != 7) {
			std::cout << "Usage: " << argv[0] <<" <pl.xclbin> <pr0.xclbin> <pr0_2.xclbin> <pr1.xclbin> <pr1_2.xclbin> <pr2.xclbin> " << std::endl;
			return EXIT_FAILURE;
		}
		std::string xclbinFilename_pl = argv[1];
		std::string xclbinFilename_aie_pr0 = argv[2];
		std::string xclbinFilename_aie_pr0_2 = argv[3];
		std::string xclbinFilename_aie_pr1 = argv[4];
		std::string xclbinFilename_aie_pr1_2 = argv[5];
		std::string xclbinFilename_aie_pr2 = argv[6];

		// Open xclbin
		auto device = xrt::device(0); //device index=0
		std::cout<<"Open device successfully"<<std::endl;

		auto match1 = run_gmio(device, xclbinFilename_aie_pr0, xclbinFilename_aie_pr0_2);
		auto match2 = run_rtp(device, xclbinFilename_pl, xclbinFilename_aie_pr1, 0);
		match2 = run_rtp(device, xclbinFilename_pl, xclbinFilename_aie_pr1_2, 1);
		auto match3 = run_perf(device, xclbinFilename_pl, xclbinFilename_aie_pr2);
		std::cout << "TEST GMIO " << (match1 ? "FAILED" : "PASSED") << "\n";
		std::cout << "TEST RTP " << (match2 ? "FAILED" : "PASSED") << "\n";
		std::cout << "TEST PERF " << (match3 ? "FAILED" : "PASSED") << "\n";
		return ((match1 || match2 || match3) ? EXIT_FAILURE :  EXIT_SUCCESS);
	}	
		catch (std::exception const& e) {
		std::cout << "Exception: " << e.what() << "\n";
		std::cout << "FAILED TEST\n";
		return 1;
	}
}
