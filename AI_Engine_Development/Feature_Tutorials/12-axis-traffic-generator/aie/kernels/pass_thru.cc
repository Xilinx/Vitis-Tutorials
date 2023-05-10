/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/


#include "kernels.h"
#include "../include.h"
#include <aie_api/aie.hpp>
using namespace adf;
void passth(
		input_buffer<int32> & __restrict wIn,
                output_buffer<int32> & __restrict wOut
	       ) {
   auto InIter = aie::begin_vector<4>(wIn);
   auto OutIter = aie::begin_vector<4>(wOut);

    printf("AIE:Inside the kernel\r\n");

   for (unsigned i=0; i<WIN_SAMPS; i++) {
      auto data = *InIter++;
      *OutIter++ = data;
   }
}


void passth_cint16(
		input_buffer<cint16> & __restrict wIn,
                output_buffer<cint16> & __restrict wOut
	       ) {
   printf("AIE passth_cint16:Inside the kernel\r\n");

   auto InIter = aie::begin_vector<4>(wIn);
   auto OutIter = aie::begin_vector<4>(wOut);
   for (unsigned i=0; i<WIN_SAMPS; i++) {
      auto data = *InIter++;
      *OutIter++ = data;
   }
}

