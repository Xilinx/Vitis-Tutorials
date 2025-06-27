/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <complex>
#include "xrt/xrt_kernel.h"
#include "xrt/xrt_aie.h"
void ref_func(int* din,int c[8],int* dout,int size){
	int d[8]={0,0,0,0,0,0,0,0};
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
int run_gmio(xrt::device &device, xrt::uuid &uuid_aie){
	const int ITERATION=4;
	const int BLOCK_SIZE_in_Bytes=ITERATION*1024;

	try{
		xrt::hw_context hwctx{device, uuid_aie};
		auto din_buffer = xrt::aie::bo (device, BLOCK_SIZE_in_Bytes,xrt::bo::flags::normal, /*memory group*/0); //Only non-cacheable buffer is supported
		int* dinArray= din_buffer.map<int*>();
		auto dout_buffer = xrt::aie::bo (device, BLOCK_SIZE_in_Bytes,xrt::bo::flags::normal, /*memory group*/0); //Only non-cacheable buffer is supported
		int* doutArray= dout_buffer.map<int*>();
    	int* doutRef=(int*)malloc(BLOCK_SIZE_in_Bytes);
    	std::cout<<"Allocate buffer completed"<<std::endl;

    	int coeff[8]={1,2,3,4,5,6,7,8};
	
    	for(int i=0;i<ITERATION*1024/4;i++){
			dinArray[i]=i;
    	}

		auto ghdl=xrt::graph(hwctx,"gr");
		std::cout<<"Open graph pr0_gr successfully"<<std::endl;
		xrt::aie::buffer bufIn(hwctx, "gr.gmioIn");
		bufIn.async(din_buffer, XCL_BO_SYNC_BO_GMIO_TO_AIE, BLOCK_SIZE_in_Bytes, 0);
    	ghdl.run(ITERATION);
		xrt::aie::buffer bufOut(hwctx, "gr.gmioOut");
		bufOut.async(dout_buffer, XCL_BO_SYNC_BO_AIE_TO_GMIO, BLOCK_SIZE_in_Bytes, 0);
    	std::cout<<"Waiting for graph to be completed"<<std::endl;
    	bufOut.wait();//Wait for gmioOut to complete
    	std::cout<<"GMIO transactions finished"<<std::endl;
		int error=0;
    	ref_func(dinArray,coeff,doutRef,ITERATION*1024/4);
    	for(int i=0;i<ITERATION*1024/4;i++){
			if(doutArray[i]!=doutRef[i]){
				std::cout<<"ERROR:dout["<<i<<"]="<<doutArray[i]<<",gold="<<doutRef[i]<<std::endl;
				error++;
			}
    	}
    

		ghdl.end();
    	if(error==0){
			std::cout<<"TEST GMIO PASSED!"<<std::endl;
    	}else{
			std::cout<<"TEST GMIO ERROR!"<<std::endl;
    	}
	}catch(std::exception const& e){  
		std::cout<<"Execution Error:"<<e.what()<<std::endl;  
		return 1;	
	}
	return 0;
}
int run_rtp(xrt::device &device, xrt::uuid &uuid_pl, xrt::uuid &uuid_aie){
	const int ITERATION=4;
	const int ELEM_per_iter=256;
	size_t output_size_in_bytes = ELEM_per_iter*4*ITERATION;

	xrt::hw_context hwctx_pl{device, uuid_pl};
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
	ghdl.update("gr.k.in[1]",10);
	ghdl.end();

	s2mm_run.wait();
	out_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

	int match = 0;	
	int base=10;
	for (int i = 0; i < ITERATION; i++) {
		for(int j=0;j<ELEM_per_iter;j++){
			if(host_out[ELEM_per_iter*i+j]!=base+i*ELEM_per_iter+j){
				match=1;
			}
		}
	}

	return match;
}
int run_perf(xrt::device &device, xrt::uuid &uuid_pl, xrt::uuid &uuid_aie){
	size_t iterations = 100;
	size_t bytes_per_iteration=128;
	size_t output_size_in_bytes = iterations * bytes_per_iteration;
	size_t OUTPUT_SIZE = output_size_in_bytes / 4; //32bits width interface

	xrt::hw_context hwctx_pl{device, uuid_pl};
	xrt::hw_context hwctx_aie{device, uuid_aie};
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
	int match = 0;
	for (int i = 0; i < OUTPUT_SIZE; i++) {
		if(host_in[i]*2!=host_out[i]){
			std::cout<<"ERROR: i="<<i<<" gold="<<host_in[i]*2<<" out="<<host_out[i]<<std::endl;
			match=1;
			break;
		}
		out<<host_out[i]<<std::endl;
	}
	out.close();
	return match;
}
int main(int argc, char* argv[])
{
	try {
		if(argc != 5) {
			std::cout << "Usage: " << argv[0] <<" <pl.xclbin> <pr0.xclbin> <pr1.xclbin> <pr2.xclbin> " << std::endl;
			return EXIT_FAILURE;
		}
		std::string xclbinFilename_pl = argv[1];
		std::string xclbinFilename_aie_pr0 = argv[2];
		std::string xclbinFilename_aie_pr1 = argv[3];
		std::string xclbinFilename_aie_pr2 = argv[4];

		// Open xclbin
		auto device = xrt::device(0); //device index=0
		auto xclbin_pl = xrt::xclbin(xclbinFilename_pl);
		auto uuid_pl = device.register_xclbin(xclbin_pl);
		auto xclbin_aie_pr0 = xrt::xclbin(xclbinFilename_aie_pr0);
		auto uuid_aie_pr0 = device.register_xclbin(xclbin_aie_pr0);
		auto xclbin_aie_pr1 = xrt::xclbin(xclbinFilename_aie_pr1);
		auto uuid_aie_pr1 = device.register_xclbin(xclbin_aie_pr1);
		auto xclbin_aie_pr2 = xrt::xclbin(xclbinFilename_aie_pr2);
		auto uuid_aie_pr2 = device.register_xclbin(xclbin_aie_pr2);
		std::cout<<"Creating context done"<<std::endl;

		auto match1 = run_gmio(device,uuid_aie_pr0);
		auto match2 = run_rtp(device, uuid_pl, uuid_aie_pr1);
		auto match3 = run_perf(device, uuid_pl, uuid_aie_pr2);
		std::cout << "TEST GMIO " << (match1 ? "FAILED" : "PASSED") << "\n";
		std::cout << "TEST RTP " << (match2 ? "FAILED" : "PASSED") << "\n";
		std::cout << "TEST PERF " << (match3 ? "FAILED" : "PASSED") << "\n";
		//Test the same graph second time
		match1 = run_gmio(device,uuid_aie_pr0);
		match2 = run_rtp(device, uuid_pl, uuid_aie_pr1);
		match3 = run_perf(device, uuid_pl, uuid_aie_pr2);
		std::cout << "TEST GMIO Second Time " << (match1 ? "FAILED" : "PASSED") << "\n";
		std::cout << "TEST RTP Second Time " << (match2 ? "FAILED" : "PASSED") << "\n";
		std::cout << "TEST PERF Second Time " << (match3 ? "FAILED" : "PASSED") << "\n";
		return ((match1 || match2 || match3) ? EXIT_FAILURE :  EXIT_SUCCESS);
	}	
		catch (std::exception const& e) {
		std::cout << "Exception: " << e.what() << "\n";
		std::cout << "FAILED TEST\n";
		return 1;
	}
}
