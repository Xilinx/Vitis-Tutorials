/*
 * (c) Copyright 2020 Xilinx, Inc. All rights reserved.
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

#pragma once

#include "system_settings.h"

using namespace adf;
namespace dsplib = xf::dsp::aie;

class FirGraph: public graph
{
private:
	// Channel Filter coefficients
	std::vector<int16> chan_taps = std::vector<int16>{
		 -17,      -65,      -35,       34,      -13,       -6,       18,      -22,
		  18,       -8,       -5,       18,      -26,       26,      -16,       -1,
		  21,      -36,       40,      -31,        8,       21,      -46,       59,
		 -53,       26,       13,      -54,       81,      -83,       56,       -6,
		 -54,      102,     -122,      101,      -43,      -38,      116,     -164,
		 161,     -102,        1,      114,     -204,      235,     -190,       74,
		  83,     -231,      319,     -310,      193,        5,     -229,      406,
		-468,      380,     -147,     -174,      487,     -684,      680,     -437,
		 -10,      553,    -1030,     1262,    -1103,      474,      596,    -1977,
		3451,    -4759,     5660,    26983};

    // HalfBand Filter coefficients
    std::vector<int16> hb_taps = std::vector<int16>{
          23,      -63,      143,     -281,      503,     -845,     1364,    -2173,
        3557,    -6568,    20729,    32767};

	//FIR Graph class
	dsplib::fir::sr_sym::fir_sr_sym_graph<DATA_TYPE, COEFF_TYPE, FIR_LEN_CHAN, SHIFT_CHAN, ROUND_MODE_CHAN, WINDOW_SIZE, AIES_CHAN> chan_FIR;
	dsplib::fir::interpolate_hb::fir_interpolate_hb_graph<DATA_TYPE, COEFF_TYPE, FIR_LEN_HB, SHIFT_HB, ROUND_MODE_HB, WINDOW_SIZE> hb_FIR;

public:
	port<input> in;
	port<output> out;

	// Constructor - with FIR graph class initialization
	FirGraph():chan_FIR(chan_taps), hb_FIR(hb_taps)  {
		// Margin gets automatically added within the FIR graph class.
		// Margin equals to FIR length rounded up to nearest multiple of 32 Bytes.
		connect<>(in, chan_FIR.in);
		connect<>(chan_FIR.out, hb_FIR.in);
		connect<>(hb_FIR.out, out);

		location<kernel>(chan_FIR.m_firKernel) = tile(18,0);
		location<kernel>(hb_FIR.m_firKernel)   = tile(19,0);
	};
};
