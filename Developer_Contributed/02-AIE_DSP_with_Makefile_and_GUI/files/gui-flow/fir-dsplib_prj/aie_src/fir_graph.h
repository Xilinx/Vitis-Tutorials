//Copyright (C) 2022, Advanced Micro Devices, Inc. All rights reserved.
//SPDX-License-Identifier: MIT

//DB: daniele.bagni@amd.com
//DB: 09 Aug 2022

// based on https://github.com/Xilinx/Vitis-Tutorials/blob/2022.1/AI_Engine_Development/Feature_Tutorials/08-dsp-library/part_1/aie/src/fir_graph.h



//#ifndef __GRAPH_H__
//#define __GRAPH_H__

#include <adf.h> //DB

#pragma once

//#include "fir_aie_defines.h"
#include "system_settings.h"   //DB: this was commented in the Makefile flow
//#include "system_settings_comm.h"

using namespace adf;
namespace dsplib = xf::dsp::aie;

class FirGraph: public graph
{
private:
	// Channel Filter coefficients
	std::vector<int16> FIR_TAP_COEFS  = std::vector<int16>{-1124, -2408,  4655, 14455, 14455,  4655, -2408, -1124};
	//std::vector<int16> FIR_TAP_COEFS = std::vector<int16>{1, 0,  0, 0, 0,  0, 0, 0};

	//FIR Graph class
	//dsplib::fir::sr_sym::fir_sr_sym_graph<DATA_TYPE, COEFF_TYPE, FIR_LEN_CHAN, SHIFT_CHAN, ROUND_MODE_CHAN, WINDOW_SIZE, AIES_CHAN> chan_FIR;
	dsplib::fir::decimate_asym::fir_decimate_asym_graph<DATA_TYPE, COEFF_TYPE, FIR_LEN_CHAN, DECIMATE_FACTOR, SHIFT_CHAN, ROUND_MODE_CHAN, WINDOW_SIZE> chan_FIR;

public:
	port<input> in;
	port<output> out;

	// Constructor - with FIR graph class initialization
	FirGraph():chan_FIR(FIR_TAP_COEFS)  {
		// Margin gets automatically added within the FIR graph class.
		// Margin equals to FIR length rounded up to nearest multiple of 32 Bytes.
		connect<>(in, chan_FIR.in);
		connect<>(chan_FIR.out, out);

		//location<kernel>(chan_FIR.m_firKernels[0]) = tile(18,0);

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
			in = input_plio::create( "DataIn1",   adf::plio_32_bits,"data/input_2col.txt",  1000);
			out = output_plio::create("DataOut1", adf::plio_32_bits,"data/output_2col.txt", 1000);

			connect<> (in.out[0],F.in);
			connect<> (F.out, out.in[0]);
		};
	};


//#endif
