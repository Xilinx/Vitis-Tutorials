/**********
© Copyright 2020 Xilinx, Inc.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**********/
#include <adf.h>
#include "graph.h"
#include <unistd.h>
#include <fstream>
#if !defined(__AIESIM__) && !defined(__X86SIM__) && !defined(__ADF_FRONTEND__)
    #include "adf/adf_api/XRTConfig.h"
    #include "experimental/xrt_kernel.h"
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
#endif

using namespace adf;

mygraph gr;

#if !defined(__AIESIM__) && !defined(__X86SIM__) && !defined(__ADF_FRONTEND__)
const int ITERATION=512;
#else
const int ITERATION=4;
#endif
const int BLOCK_SIZE_in_Bytes=1024*ITERATION;
const int BLOCK_SIZE_out_Bytes=1032*ITERATION;

int main(int argc, char ** argv) {
#if !defined(__AIESIM__) && !defined(__X86SIM__) && !defined(__ADF_FRONTEND__)
    // Create XRT device handle for ADF API
    char* xclbinFilename = argv[1];
    auto dhdl = xrtDeviceOpen(0);//device index=0
    xrtDeviceLoadXclbinFile(dhdl,xclbinFilename);
    xuid_t uuid;
    xrtDeviceGetXclbinUUID(dhdl, uuid);
    adf::registerXRT(dhdl, uuid);
#endif
    gr.init();
    int error=0;

    int32* dinArray[32];
    int32* doutArray[32];

    for(int i=0;i<32;i++){
	dinArray[i]=(int32*)GMIO::malloc(BLOCK_SIZE_in_Bytes);
	doutArray[i]=(int32*)GMIO::malloc(BLOCK_SIZE_out_Bytes);
	sleep(1);
    }
    std::cout<<"GMIO::malloc completed"<<std::endl;
    
    for(int num=1;num<=32;num*=2){
    	for(int i=0;i<32;i++){
		for(int j=0;j<BLOCK_SIZE_in_Bytes/sizeof(int);j++){
			dinArray[i][j]=j+num;
		}
    	}
	gr.run(ITERATION);
	
#if !defined(__AIESIM__) && !defined(__X86SIM__) && !defined(__ADF_FRONTEND__)
	#if __TIMER__
		Timer timer;
	#elif __AIE_CYCLES__
	#elif __USE_EVENT_PROFILE__
		std::cout<<"total input/output num="<<num<<std::endl;
		event::handle handle[32];
		for(int i=0;i<1;i++){
			handle[i] = event::start_profiling(gr.gmioOut[i], event::io_stream_start_to_bytes_transferred_cycles, BLOCK_SIZE_out_Bytes);
		}
	#else
	#endif
#endif
	for(int i=0;i<num;i++){
		gr.gmioIn[i].gm2aie_nb(dinArray[i], BLOCK_SIZE_in_Bytes);
		gr.gmioOut[i].aie2gm_nb(doutArray[i], BLOCK_SIZE_out_Bytes);
	}
	  
	for(int i=0;i<num;i++){
		gr.gmioOut[i].wait();
	}
#if !defined(__AIESIM__) && !defined(__X86SIM__) && !defined(__ADF_FRONTEND__)
	#if __TIMER__
		double timer_stop=timer.stop();
    		double throughput=(BLOCK_SIZE_in_Bytes+BLOCK_SIZE_out_Bytes)*num/timer_stop*1000000/1024/1024;
    		std::cout<<"Throughput (by timer GMIO in num="<<num<<",out num="<<num<<"):"<<throughput<<"M Bytes/s"<<std::endl;
	#elif __AIE_CYCLES__
		long long start[32];
		long long end[32];
		long long very_beginning=0x0FFFFFFFFFFFFFFF;
		long long the_last=0;
		for(int i=0;i<num;i++){
			start[i]=*(long long*)&doutArray[i][256];
			end[i]=*(long long*)&doutArray[i][BLOCK_SIZE_out_Bytes/sizeof(int)-2];
			if(start[i]<very_beginning){
				very_beginning=start[i];
			}
			if(end[i]>the_last){
				the_last=end[i];
			}
		}
		std::cout<<"Throughput (by AIE kernel cycles in="<<num<<",out="<<num<<") ="<<(double)(BLOCK_SIZE_in_Bytes+BLOCK_SIZE_out_Bytes)*num/(double)(the_last-very_beginning)*1000<<"M Bytes/s"<<std::endl;
	#elif __USE_EVENT_PROFILE__
		long long cycle_count[32];
		for(int i=0;i<1;i++){
			cycle_count[i] = event::read_profiling(handle[i]);
			event::stop_profiling(handle[i]);
		}
		for(int i=0;i<1;i++){
			double bandwidth = (double)BLOCK_SIZE_out_Bytes / ((double)cycle_count[i] ) *1000; //byte per second
			std::cout<<"Throughput (by event API) gmioOut["<<i<<"] bandwidth="<<bandwidth<<"M Bytes/s"<<std::endl;
		}
	#else
		std::cout<<"Please define macro __TIMER__ or __AIE_CYCLES__ or __USE_EVENT_PROFILE__ in sw/Makefile to get throughput number"<<std::endl;
	#endif
#endif
	//check output correctness
	for(int i=0;i<num;i++){
	    	for(int j=0;j<BLOCK_SIZE_out_Bytes/sizeof(int);j++){
			if(j%258!=256 && j%258!=257 && doutArray[i][j]!=j+num+1-j/258*2){
	  			std::cout<<"ERROR:dout["<<i<<"]["<<j<<"]="<<doutArray[i][j]<<std::endl;
	  			error++;
				break;
	  		}
		}
	}
	
	//flush remain stalling kernels
	for(int i=num;i<32;i++){
		gr.gmioIn[i].gm2aie_nb(dinArray[i], BLOCK_SIZE_in_Bytes);
		gr.gmioOut[i].aie2gm_nb(doutArray[i], BLOCK_SIZE_out_Bytes);
	}
	gr.wait();
    }

    if(!error){
	std::cout<<"AIE GMIO PASSED!"<<std::endl;
    }else{
	std::cout<<"AIE GMIO FAILED!"<<std::endl;
    }
    for(int i=0;i<32;i++){
    	GMIO::free(dinArray[i]);
    	GMIO::free(doutArray[i]);
    }
    std::cout<<"GMIO::free completed"<<std::endl;

    if(error==0){
  	  std::cout<<"TEST PASSED!"<<std::endl;
    }else{
  	  std::cout<<"TEST FAILED!"<<std::endl;
    }

	gr.end();
#if !defined(__AIESIM__) && !defined(__X86SIM__) && !defined(__ADF_FRONTEND__)
    xrtDeviceClose(dhdl);
#endif
  
    return error;
};
