/*
 * (c) Copyright 2021 Xilinx, Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */


#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <adf.h>

#include "aie_kernels.h"
#include "include.h"
#include "hls_kernels.h"

using namespace adf;

class clipped : public graph {

private:
	kernel weightsum;
	kernel average;
	kernel clip;
	kernel classify;

public:
	port<input> in;
	port<output> out;





	clipped() {
		weightsum 	= kernel::create(weighted_cpx_sum_with_margin);
		average 		= kernel::create(vectorized_cpx_average_div);
		clip 			= kernel::create(polar_clip);
		classify		= kernel::create(classifier);

		fabric<pl>(clip);


		connect< window<WEIGHTSUM_INPUT_SAMPLES*NBYTES_DATA,WEIGHTSUM_INPUT_MARGIN*NBYTES_DATA> > net0 (in, weightsum.in[0]);
		connect< window<AVERAGE_INPUT_SAMPLES*NBYTES_DATA*2> > net1 (weightsum.out[0], average.in[0]);
		connect< window<POLAR_CLIP_INPUT_SAMPLES*NBYTES_DATA>, stream > net2 (average.out[0], clip.in[0]);

		connect< stream > net4(clip.out[0], classify.in[0]);

		connect< window<CLASSIFIER_OUTPUT_SAMPLES*NBYTES_DATA> > net5 (classify.out[0], out);

		std::vector<std::string> myheaders;
		myheaders.push_back("include.h");

		adf::headers(classify) = myheaders;

	    source(weightsum) = "aie_kernels/weighted_sum.cc";
	    source(average) = "aie_kernels/average_div.cc";
		source(clip)         = "polar_clip.cpp";
	    source(classify)    = "aie_kernels/classify.cc";

	    location<kernel>(weightsum) = tile(5,5);

		runtime<ratio>(weightsum) = 0.8;
		runtime<ratio>(average) = 0.8;
		runtime<ratio>(classify) = 0.8;
	};
};

#endif /* __GRAPH_H__ */
