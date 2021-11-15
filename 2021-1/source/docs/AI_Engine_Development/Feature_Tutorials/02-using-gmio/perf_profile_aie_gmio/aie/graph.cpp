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
GMIO gmioIn[32]={
	GMIO("gmioIn0", 256, 100),
	GMIO("gmioIn1", 256, 100),
	GMIO("gmioIn2", 256, 100),
	GMIO("gmioIn3", 256, 100),
	GMIO("gmioIn4", 256, 100),
	GMIO("gmioIn5", 256, 100),
	GMIO("gmioIn6", 256, 100),
	GMIO("gmioIn7", 256, 100),
	GMIO("gmioIn8", 256, 100),
	GMIO("gmioIn9", 256, 100),
	GMIO("gmioIn10", 256, 100),
	GMIO("gmioIn11", 256, 100),
	GMIO("gmioIn12", 256, 100),
	GMIO("gmioIn13", 256, 100),
	GMIO("gmioIn14", 256, 100),
	GMIO("gmioIn15", 256, 100),
	GMIO("gmioIn16", 256, 100),
	GMIO("gmioIn17", 256, 100),
	GMIO("gmioIn18", 256, 100),
	GMIO("gmioIn19", 256, 100),
	GMIO("gmioIn20", 256, 100),
	GMIO("gmioIn21", 256, 100),
	GMIO("gmioIn22", 256, 100),
	GMIO("gmioIn23", 256, 100),
	GMIO("gmioIn24", 256, 100),
	GMIO("gmioIn25", 256, 100),
	GMIO("gmioIn26", 256, 100),
	GMIO("gmioIn27", 256, 100),
	GMIO("gmioIn28", 256, 100),
	GMIO("gmioIn29", 256, 100),
	GMIO("gmioIn30", 256, 100),
	GMIO("gmioIn31", 256, 100)
};
GMIO gmioOut[32]={
	GMIO("gmioOut0", 256, 100),
	GMIO("gmioOut1", 256, 100),
	GMIO("gmioOut2", 256, 100),
	GMIO("gmioOut3", 256, 100),
	GMIO("gmioOut4", 256, 100),
	GMIO("gmioOut5", 256, 100),
	GMIO("gmioOut6", 256, 100),
	GMIO("gmioOut7", 256, 100),
	GMIO("gmioOut8", 256, 100),
	GMIO("gmioOut9", 256, 100),
	GMIO("gmioOut10", 256, 100),
	GMIO("gmioOut11", 256, 100),
	GMIO("gmioOut12", 256, 100),
	GMIO("gmioOut13", 256, 100),
	GMIO("gmioOut14", 256, 100),
	GMIO("gmioOut15", 256, 100),
	GMIO("gmioOut16", 256, 100),
	GMIO("gmioOut17", 256, 100),
	GMIO("gmioOut18", 256, 100),
	GMIO("gmioOut19", 256, 100),
	GMIO("gmioOut20", 256, 100),
	GMIO("gmioOut21", 256, 100),
	GMIO("gmioOut22", 256, 100),
	GMIO("gmioOut23", 256, 100),
	GMIO("gmioOut24", 256, 100),
	GMIO("gmioOut25", 256, 100),
	GMIO("gmioOut26", 256, 100),
	GMIO("gmioOut27", 256, 100),
	GMIO("gmioOut28", 256, 100),
	GMIO("gmioOut29", 256, 100),
	GMIO("gmioOut30", 256, 100),
	GMIO("gmioOut31", 256, 100)
};
adf::simulation::platform<32,32> platform(&gmioIn[0],&gmioIn[1],&gmioIn[2],&gmioIn[3],&gmioIn[4],&gmioIn[5],&gmioIn[6],&gmioIn[7],&gmioIn[8],&gmioIn[9],
		&gmioIn[10],&gmioIn[11],&gmioIn[12],&gmioIn[13],&gmioIn[14],&gmioIn[15],&gmioIn[16],&gmioIn[17],&gmioIn[18],&gmioIn[19],
		&gmioIn[20],&gmioIn[21],&gmioIn[22],&gmioIn[23],&gmioIn[24],&gmioIn[25],&gmioIn[26],&gmioIn[27],&gmioIn[28],&gmioIn[29],&gmioIn[30],&gmioIn[31],
		&gmioOut[0],&gmioOut[1],&gmioOut[2],&gmioOut[3],&gmioOut[4],&gmioOut[5],&gmioOut[6],&gmioOut[7],&gmioOut[8],&gmioOut[9],
		&gmioOut[10],&gmioOut[11],&gmioOut[12],&gmioOut[13],&gmioOut[14],&gmioOut[15],&gmioOut[16],&gmioOut[17],&gmioOut[18],&gmioOut[19],
		&gmioOut[20],&gmioOut[21],&gmioOut[22],&gmioOut[23],&gmioOut[24],&gmioOut[25],&gmioOut[26],&gmioOut[27],&gmioOut[28],&gmioOut[29],&gmioOut[30],&gmioOut[31]);

mygraph gr;
adf::connect<> net0(platform.src[0], gr.din[0]);
adf::connect<> net1(platform.src[1], gr.din[1]);
adf::connect<> net2(platform.src[2], gr.din[2]);
adf::connect<> net3(platform.src[3], gr.din[3]);
adf::connect<> net4(platform.src[4], gr.din[4]);
adf::connect<> net5(platform.src[5], gr.din[5]);
adf::connect<> net6(platform.src[6], gr.din[6]);
adf::connect<> net7(platform.src[7], gr.din[7]);
adf::connect<> net8(platform.src[8], gr.din[8]);
adf::connect<> net9(platform.src[9], gr.din[9]);
adf::connect<> net10(platform.src[10], gr.din[10]);
adf::connect<> net11(platform.src[11], gr.din[11]);
adf::connect<> net12(platform.src[12], gr.din[12]);
adf::connect<> net13(platform.src[13], gr.din[13]);
adf::connect<> net14(platform.src[14], gr.din[14]);
adf::connect<> net15(platform.src[15], gr.din[15]);
adf::connect<> net16(platform.src[16], gr.din[16]);
adf::connect<> net17(platform.src[17], gr.din[17]);
adf::connect<> net18(platform.src[18], gr.din[18]);
adf::connect<> net19(platform.src[19], gr.din[19]);
adf::connect<> net20(platform.src[20], gr.din[20]);
adf::connect<> net21(platform.src[21], gr.din[21]);
adf::connect<> net22(platform.src[22], gr.din[22]);
adf::connect<> net23(platform.src[23], gr.din[23]);
adf::connect<> net24(platform.src[24], gr.din[24]);
adf::connect<> net25(platform.src[25], gr.din[25]);
adf::connect<> net26(platform.src[26], gr.din[26]);
adf::connect<> net27(platform.src[27], gr.din[27]);
adf::connect<> net28(platform.src[28], gr.din[28]);
adf::connect<> net29(platform.src[29], gr.din[29]);
adf::connect<> net30(platform.src[30], gr.din[30]);
adf::connect<> net31(platform.src[31], gr.din[31]);
adf::connect<> nout0(gr.dout[0],platform.sink[0]);
adf::connect<> nout1(gr.dout[1],platform.sink[1]);
adf::connect<> nout2(gr.dout[2],platform.sink[2]);
adf::connect<> nout3(gr.dout[3],platform.sink[3]);
adf::connect<> nout4(gr.dout[4],platform.sink[4]);
adf::connect<> nout5(gr.dout[5],platform.sink[5]);
adf::connect<> nout6(gr.dout[6],platform.sink[6]);
adf::connect<> nout7(gr.dout[7],platform.sink[7]);
adf::connect<> nout8(gr.dout[8],platform.sink[8]);
adf::connect<> nout9(gr.dout[9],platform.sink[9]);
adf::connect<> nout10(gr.dout[10],platform.sink[10]);
adf::connect<> nout11(gr.dout[11],platform.sink[11]);
adf::connect<> nout12(gr.dout[12],platform.sink[12]);
adf::connect<> nout13(gr.dout[13],platform.sink[13]);
adf::connect<> nout14(gr.dout[14],platform.sink[14]);
adf::connect<> nout15(gr.dout[15],platform.sink[15]);
adf::connect<> nout16(gr.dout[16],platform.sink[16]);
adf::connect<> nout17(gr.dout[17],platform.sink[17]);
adf::connect<> nout18(gr.dout[18],platform.sink[18]);
adf::connect<> nout19(gr.dout[19],platform.sink[19]);
adf::connect<> nout20(gr.dout[20],platform.sink[20]);
adf::connect<> nout21(gr.dout[21],platform.sink[21]);
adf::connect<> nout22(gr.dout[22],platform.sink[22]);
adf::connect<> nout23(gr.dout[23],platform.sink[23]);
adf::connect<> nout24(gr.dout[24],platform.sink[24]);
adf::connect<> nout25(gr.dout[25],platform.sink[25]);
adf::connect<> nout26(gr.dout[26],platform.sink[26]);
adf::connect<> nout27(gr.dout[27],platform.sink[27]);
adf::connect<> nout28(gr.dout[28],platform.sink[28]);
adf::connect<> nout29(gr.dout[29],platform.sink[29]);
adf::connect<> nout30(gr.dout[30],platform.sink[30]);
adf::connect<> nout31(gr.dout[31],platform.sink[31]);

#if !defined(__AIESIM__) && !defined(__X86SIM__) && !defined(__ADF_FRONTEND__)
const int ITERATION=4096;
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
	#ifdef __TIME_STAMP__
		xuid_t uuid;
    		xrtDeviceGetXclbinUUID(dhdl, uuid);
		auto ghdl=xrtGraphOpen(dhdl,uuid,"gr");
		long long t_stamp=xrtGraphTimeStamp(ghdl);
	#elif __TIMER__
		Timer timer;
	#elif __AIE_CYCLES__
	#elif __USE_EVENT_PROFILE__
		std::cout<<"total input/output num="<<num<<std::endl;
		event::handle handle[32];
		for(int i=0;i<1;i++){
			handle[i] = event::start_profiling(gmioOut[i], event::io_stream_start_to_bytes_transferred_cycles, BLOCK_SIZE_out_Bytes);
		}
	#else
	#endif
#endif
	for(int i=0;i<num;i++){
		gmioIn[i].gm2aie_nb(dinArray[i], BLOCK_SIZE_in_Bytes);
		gmioOut[i].aie2gm_nb(doutArray[i], BLOCK_SIZE_out_Bytes);
	}
	  
	for(int i=0;i<num;i++){
		gmioOut[i].wait();
	}
#if !defined(__AIESIM__) && !defined(__X86SIM__) && !defined(__ADF_FRONTEND__)
	#ifdef __TIME_STAMP__
		t_stamp=xrtGraphTimeStamp(ghdl)-t_stamp;
		std::cout<<"Throughput (by graph timstamp) bandwidth(GMIO in num="<<num<<",out num="<<num<<"):"<<(double)(BLOCK_SIZE_in_Bytes+BLOCK_SIZE_out_Bytes)*num/(double)t_stamp*1000<<"M bytes/s"<<std::endl;
	#elif __TIMER__
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
		std::cout<<"Please define macro __TIMER__ or __AIE_CYCLES__ or __TIME_STAMP__ or __USE_EVENT_PROFILE__ in sw/Makefile to get throughput number"<<std::endl;
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
		gmioIn[i].gm2aie_nb(dinArray[i], BLOCK_SIZE_in_Bytes);
		gmioOut[i].aie2gm_nb(doutArray[i], BLOCK_SIZE_out_Bytes);
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
  	  std::cout<<"PASS!"<<std::endl;
    }else{
  	  std::cout<<"ERROR!"<<std::endl;
    }

#if !defined(__AIESIM__) && !defined(__X86SIM__) && !defined(__ADF_FRONTEND__)
    xrtDeviceClose(dhdl);
#endif
  
    return error;
};
