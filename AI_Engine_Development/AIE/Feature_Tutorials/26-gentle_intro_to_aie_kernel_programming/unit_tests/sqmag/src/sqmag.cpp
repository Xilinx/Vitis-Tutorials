/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"

#include "sqmag.hpp"

//#define KDEBUG	// comment out to suppress debug messages

#ifdef KDEBUG
	#include "aie_api/utils.hpp"
#endif // KDEBUG

using namespace aie;

// calculate the squared magnitude of the input vector elements
// processes "burst_count" vectors during each invocation
template  <typename Ti, typename To, unsigned vlen, unsigned burst_count>
void sqmag(input_stream<Ti>  *istrm,	// input stream
           output_stream<To> *ostrm		// output stream
) {

	for (auto i = 0u; i < burst_count; i++) {

        auto vin = readincr_v<vlen>(istrm);	// read "vlen" samples from "istrm" (re + j*im)
                                        	// into the vector register "vin"
        auto magsq = abs_square<To>(vin);  	// use "abs_square()" API to calculate (re^2 + im^2)
                                            // for each element and store in vector register "magsq"

		#ifdef KDEBUG
			printf("i = %d:\n", i);
			print(vin, true, "  vin = ");
			print(magsq, true, "  magsq = ");
			printf("\n");
		#endif // KDEBUG

		writeincr(ostrm, magsq);    // write vector register "magsq"
                                    // to output stream "ostrm"

	} // end for (auto i = 0u; i < burst_cnt; i++)

} // end sqmag()

