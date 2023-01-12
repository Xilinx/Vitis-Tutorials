//Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
//SPDX-License-Identifier: MIT

//DB: daniele.bagni@amd.com
//DB: 09 Aug 2022

#include "fir_graph.h"

//adf::PLIO *pl128_in  = new adf::PLIO("DataIn1",  adf::plio_128_bits,"data/input_128b.txt",  250);
//adf::PLIO *pl128_out = new adf::PLIO("DataOut1", adf::plio_128_bits,"data/output_128b.txt", 250);
//simulation::platform<1,1> platform(pl128_in, pl128_out);

using namespace adf;

TopGraph filter;

//connect<> net0(platform.src[0], filter.in);
//connect<> net1(filter.out, platform.sink[0]);


#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char ** argv) {
	filter.init() ;
	filter.run(ITER_CNT);  //N_ITERATIONS);
	filter.end() ;
    return 0 ;
}
#endif


