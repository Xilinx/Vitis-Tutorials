/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <unistd.h>
#include <fstream>
#include "xrt/xrt_kernel.h"
#include "xrt/xrt_aie.h"
#include <chrono>
#include <ctime>
class Timer {
	std::chrono::high_resolution_clock::time_point mTimeStart;
    public:
  	Timer() { reset(); }
  	long long stop() {
    	std::chrono::high_resolution_clock::time_point timeEnd =
        	std::chrono::high_resolution_clock::now();
    	return std::chrono::duration_cast<std::chrono::microseconds>(timeEnd -
                                                                 mTimeStart)
        	.count();
	}
  	void reset() { mTimeStart = std::chrono::high_resolution_clock::now(); }
};
int main(int argc, char ** argv) {
	const int NUM=4;
	int ITERATION=8192;
	char* emu_mode = getenv("XCL_EMULATION_MODE");
	if (emu_mode != nullptr) {
		ITERATION=4;
	}
	char* xclbinFilename = argv[1];
	// Open xclbin
	auto device = xrt::device(0); //device index=0
	auto uuid = device.load_xclbin(xclbinFilename);
	auto ghdl=xrt::graph(device,uuid,"gr");


	std::cout<<"ITERATION:"<<ITERATION<<std::endl;
	const int BLOCK_SIZE_in_Bytes=1024*ITERATION;
	const int BLOCK_SIZE_out_Bytes=1032*ITERATION;
	int error=0;

	xrt::aie::bo *din_buffer[NUM];
	xrt::aie::bo *dout_buffer[NUM];
	xrt::aie::bo::async_handle *dout_buffer_handle[NUM];
	int* dinArray[NUM];
	int* doutArray[NUM];

	for(int i=0;i<NUM;i++){
		din_buffer[i] = new xrt::aie::bo (device, BLOCK_SIZE_in_Bytes,xrt::bo::flags::normal, /*memory group*/0); //Only non-cacheable buffer is supported
		dinArray[i] = din_buffer[i]->map<int*>();
		dout_buffer[i] = new xrt::aie::bo (device, BLOCK_SIZE_out_Bytes,xrt::bo::flags::normal, /*memory group*/0); //Only non-cacheable buffer is supported
		doutArray[i] = dout_buffer[i]->map<int*>();
	}
	std::cout<<"GMIO::malloc completed"<<std::endl;
    
	for(int i=0;i<NUM;i++){
		for(int j=0;j<BLOCK_SIZE_in_Bytes/sizeof(int);j++){
			dinArray[i][j]=j;
		}
	}
	ghdl.run(ITERATION);
	for(int i=0;i<NUM;i++){
		auto run=dout_buffer[i]->async("gr.gmioOut["+std::to_string(i)+"]",XCL_BO_SYNC_BO_AIE_TO_GMIO,BLOCK_SIZE_out_Bytes,/*offset*/0);
		dout_buffer_handle[i]=new xrt::aie::bo::async_handle(run);
	}
	
	#if __TIMER__
		Timer timer;
	#elif __AIE_CYCLES__
	#elif __USE_EVENT_PROFILE__
		std::cout<<"total input/output num="<<NUM<<std::endl;
		xrt::aie::profiling *handle[NUM];
		for(int i=0;i<NUM;i++){
			handle[i]=new xrt::aie::profiling(device);
			handle[i]->start(xrt::aie::profiling::profiling_option::io_stream_start_to_bytes_transferred_cycles, "gr.gmioOut["+std::to_string(i)+"]", "", BLOCK_SIZE_out_Bytes);
		}
	#else
	#endif

	for(int i=0;i<NUM;i++){
		din_buffer[i]->async("gr.gmioIn["+std::to_string(i)+"]",XCL_BO_SYNC_BO_GMIO_TO_AIE,BLOCK_SIZE_in_Bytes,/*offset*/0);
	}
	  
	for(int i=0;i<NUM;i++){
		dout_buffer_handle[i]->wait();
	}

	#if __TIMER__
		double timer_stop=timer.stop();
		double throughput=(BLOCK_SIZE_in_Bytes+BLOCK_SIZE_out_Bytes)*NUM/timer_stop;
		std::cout<<"Throughput (by timer GMIO in num="<<NUM<<",out num="<<NUM<<"):"<<throughput<<"M Bytes/s"<<std::endl;
	#elif __AIE_CYCLES__
		unsigned long long start[NUM];
		unsigned long long end[NUM];
		unsigned long long very_beginning=0xFFFFFFFFFFFFFFFF;
		unsigned long long the_last=0;
		for(int i=0;i<NUM;i++){
			start[i]=*(unsigned long long*)&doutArray[i][256];
			end[i]=*(unsigned long long*)&doutArray[i][BLOCK_SIZE_out_Bytes/sizeof(int)-2];
			if(start[i]<very_beginning){
				very_beginning=start[i];
			}
			if(end[i]>the_last){
				the_last=end[i];
			}
		}
		std::cout<<"Throughput (by AIE kernel cycles in="<<NUM<<",out="<<NUM<<") ="<<(double)(BLOCK_SIZE_in_Bytes+BLOCK_SIZE_out_Bytes)*NUM/((double)(the_last-very_beginning)*0.8)*1000<<"M Bytes/s"<<std::endl;
	#elif __USE_EVENT_PROFILE__
		long long cycle_count[NUM];
		long long total_cycle_count=0;
		for(int i=0;i<NUM;i++){
			cycle_count[i] = handle[i]->read();
			handle[i]->stop();
			if(cycle_count[i]>total_cycle_count){
				total_cycle_count=cycle_count[i];
			}
		}
		double bandwidth = (double)(BLOCK_SIZE_in_Bytes+BLOCK_SIZE_out_Bytes)*NUM / ((double)total_cycle_count*0.8) *1000; //byte per second
		std::cout<<"Throughput (by event API) bandwidth="<<bandwidth<<"M Bytes/s"<<std::endl;
	#else
		std::cout<<"Please define macro __TIMER__ or __AIE_CYCLES__ or __USE_EVENT_PROFILE__ in sw/Makefile to get throughput number"<<std::endl;
	#endif
	//check output correctness
	for(int i=0;i<NUM;i++){
	    for(int j=0;j<BLOCK_SIZE_out_Bytes/sizeof(int);j++){
			if(j%258!=256 && j%258!=257 && doutArray[i][j]!=j+1-j/258*2){
	  			std::cout<<"ERROR:dout["<<i<<"]["<<j<<"]="<<doutArray[i][j]<<std::endl;
	  			error++;
				break;
	  		}
		}
	}
	
	if(error==0){
		std::cout<<"TEST PASSED!"<<std::endl;
	}else{
		std::cout<<"TEST FAILED!"<<std::endl;
	}

	ghdl.end();
  
	return error;
};
