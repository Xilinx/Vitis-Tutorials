/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <complex>
#include "xrt/xrt_aie.h"
#include "xrt/xrt_kernel.h"
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

#define GRAPH_SIZE_in_Bytes (256*384*2)

using namespace std;

int open_xclbin(char* xclbinFilename,xrt::device &device, xrt::uuid &id){
	int ret;
	// Open xclbin
	device = xrt::device(0); //device index=0
	id = device.load_xclbin(xclbinFilename);
	return ret;
}

int run_plio_graph(const xrt::device &device,const xrt::uuid &id,int iter_graph){
	int iterations=iter_graph;
	int output_size_in_bytes=iterations*GRAPH_SIZE_in_Bytes;
	int OUTPUT_SIZE=output_size_in_bytes/16; //128 bits interface
	std::cout<<"iterations:"<<iterations<<std::endl;

    // PL ip
    auto s2ss1 = xrt::kernel(device, id, "s2ss:{s2ss_1}");
	auto mm2s1 = xrt::kernel(device, id, "datagen:{datagen_1}");

	auto s2ss1_run = s2ss1(nullptr, OUTPUT_SIZE);

    auto gr=xrt::graph(device,id,"gr");
	std::cout<<"graph run iteration="<<iterations<<std::endl;
    gr.run(iterations);

	Timer timer;

	auto mm2s1_run = mm2s1(nullptr, OUTPUT_SIZE);
	s2ss1_run.wait();
	
	double timer_stop=timer.stop();
	double throughput=output_size_in_bytes/timer_stop;
    std::cout<<"Throughput of the graph:"<<throughput<<"M Bytes/s"<<std::endl;

	int match=0;
	return match;
}

int main(int argc, char* argv[]) {
	std::cout<<"Version:"<<"1.0"<<std::endl;
    if(argc < 3) {
		std::cout << "Usage: " << argv[0] <<" <xclbin> <ITERATION> " << std::endl;
		return EXIT_FAILURE;
    }
    char* xclbinFilename = argv[1];
	int iteration=stoi(argv[2]);
	xrt::device device;
    xrt::uuid id;
	int ret;

	ret=open_xclbin(xclbinFilename,device,id);

	ret=run_plio_graph(device,id,iteration);
    std::cout << "TEST PLIO " << xclbinFilename << (ret ? " FAILED" : " PASSED") << std::endl;

    return (ret ? EXIT_FAILURE :  EXIT_SUCCESS);
}
