/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
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

		//FIR Graph class
		dsplib::fir::sr_sym::fir_sr_sym_graph<DATA_TYPE, COEFF_TYPE, FIR_LEN_CHAN, SHIFT_CHAN, ROUND_MODE_CHAN, WINDOW_SIZE, AIES_CHAN> chan_FIR;

	public:
		port<input> in;
		port<output> out;

		// Constructor - with FIR graph class initialization
		FirGraph():chan_FIR(chan_taps)
		{
			// Margin gets automatically added within the FIR graph class.
			// Margin equals to FIR length rounded up to nearest multiple of 32 Bytes.
			connect<>(in, chan_FIR.in[0]);
			connect<>(chan_FIR.out[0], out);

			location<kernel>(chan_FIR.m_firKernels[0]) = tile(18,0);

		};
	};


	class TopGraph : public graph
	{
	public:
		input_plio in;
		output_plio out;

		FirGraph F;

		TopGraph()
		{
			in = input_plio::create("128 bits read in",  adf::plio_128_bits,"data/input_128b.txt",  250);
			out = output_plio::create("128 bits read out", adf::plio_128_bits,"data/output_128b.txt", 250);

			connect<> (in.out[0],F.in);
			connect<> (F.out, out.in[0]);
		};
	};
