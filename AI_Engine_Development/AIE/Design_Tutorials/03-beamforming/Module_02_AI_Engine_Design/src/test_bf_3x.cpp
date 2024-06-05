
/*
Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
 
#include "test_bf.h"

using namespace adf;
#define COL_OFFSET 0

#define UL_INPUT_BA  (4+32)
#define UL_OUTPUT_BA (8)
TEST_BF<COL_OFFSET, COL_OFFSET+16, COL_OFFSET+32> dut;

#ifdef __AIESIM__
int main(void) {

	dut.init();
	const int NITER = 20;
	const int totalWords = 	NITER*OUT_DATA_BLKSZ;

	event::handle handle0 = event::start_profiling(dut.dlout[0], event::io_stream_start_to_bytes_transferred_cycles, totalWords*sizeof(cint16));
	event::handle handle1 = event::start_profiling(dut.ulout[0], event::io_stream_start_to_bytes_transferred_cycles, totalWords*sizeof(cint16));

	dut.run(NITER);
	dut.wait();
	
	long long cycle_count0 = event::read_profiling(handle0);
	long long cycle_count1 = event::read_profiling(handle1);

	event::stop_profiling(handle0);
	event::stop_profiling(handle1);

	double throughput0 = (double)totalWords * sizeof(cint16) / (cycle_count0 * 1e-9); //byte per second
	double throughput1 = (double)totalWords * sizeof(cint16) / (cycle_count1 * 1e-9); //byte per second
   
	std::cout << " DLBF Output0: Throughput " << throughput0/4.0 << " Samples/s, i.e. " << throughput0 << " Bytes/s" <<std::endl;
	std::cout << " ULBF Output0: Throughput " << throughput1/4.0 << " Samples/s, i.e. " << throughput1 << " Bytes/s" <<std::endl;

	dut.end();
	return 0;

}
#endif
