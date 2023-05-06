/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include "graph.h"
using namespace adf;

adaptive_graph gr;

#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char **argv)
{
	gr.init();
	int iterations=20;
	int bytes_per_iteration=4096;
	int total_bytes=bytes_per_iteration * iterations;
	event::handle handle = event::start_profiling(gr.dataout, event::io_stream_start_to_bytes_transferred_cycles, total_bytes);
	if(handle==event::invalid_handle){
		printf("ERROR:Invalid handle. Only two performance counter in a AIE-PL interface tile\n");
		return 1;
	}
	gr.run(iterations);
	gr.wait();
	long long cycle_count = event::read_profiling(handle);
	std::cout<<"cycle count:"<<cycle_count<<std::endl;
	event::stop_profiling(handle);
	double throughput = (double)total_bytes / (cycle_count * 1e-3); 
	printf("Throughput of the graph: %f MB/s\n",throughput);
	return 0;
};
#endif


