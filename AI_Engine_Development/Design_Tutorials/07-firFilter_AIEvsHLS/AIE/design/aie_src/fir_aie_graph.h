//------------------------------------------------------------------------------
// © Copyright 2021–2022 Xilinx, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//------------------------------------------------------------------------------

#pragma once

#include "fir_aie_defines.h"
//#include "system_settings.h"
#include "system_settings_comm.h"

using namespace adf;
namespace dsplib = xf::dsp::aie;

class FirGraph: public graph
{
private:
	// Channel Filter coefficients
	std::vector<int16> FIR_TAP_COEFS = std::vector<int16> FIR_COEF_VALUES;

	//FIR Graph class
    dsplib::fir::sr_sym::fir_sr_sym_graph<T_DATA, T_COEF, N_FIR_TAPS, FIR_DOWNSHIFT, FIR_ROUND_MODE, FIR_WINDOW_SIZE, N_AIES_PER_FIR>  FIR_ARRAY[N_FIR_FILTERS];

public:
	input_plio   in    = input_plio::create("DataIn", plio_128_bits, "input_impulse.txt");
   output_plio  out  = output_plio::create("DataOut", plio_128_bits, "data/fir_output_impulse.txt");
	// Constructor - with FIR graph class initialization
   FirGraph():  FIR_ARRAY {
      dsplib::fir::sr_sym::fir_sr_sym_graph<T_DATA, T_COEF, N_FIR_TAPS, FIR_DOWNSHIFT, FIR_ROUND_MODE, FIR_WINDOW_SIZE, N_AIES_PER_FIR> (FIR_TAP_COEFS)
#if (N_FIR_FILTERS >= 2)
      , dsplib::fir::sr_sym::fir_sr_sym_graph<T_DATA, T_COEF, N_FIR_TAPS, FIR_DOWNSHIFT, FIR_ROUND_MODE, FIR_WINDOW_SIZE, N_AIES_PER_FIR> (FIR_TAP_COEFS)
#endif
#if (N_FIR_FILTERS >= 3)
      , dsplib::fir::sr_sym::fir_sr_sym_graph<T_DATA, T_COEF, N_FIR_TAPS, FIR_DOWNSHIFT, FIR_ROUND_MODE, FIR_WINDOW_SIZE, N_AIES_PER_FIR> (FIR_TAP_COEFS)
#endif
#if (N_FIR_FILTERS >= 4)
      , dsplib::fir::sr_sym::fir_sr_sym_graph<T_DATA, T_COEF, N_FIR_TAPS, FIR_DOWNSHIFT, FIR_ROUND_MODE, FIR_WINDOW_SIZE, N_AIES_PER_FIR> (FIR_TAP_COEFS)
#endif
#if (N_FIR_FILTERS >= 5)
      , dsplib::fir::sr_sym::fir_sr_sym_graph<T_DATA, T_COEF, N_FIR_TAPS, FIR_DOWNSHIFT, FIR_ROUND_MODE, FIR_WINDOW_SIZE, N_AIES_PER_FIR> (FIR_TAP_COEFS)
#endif
#if (N_FIR_FILTERS >= 6)
      , dsplib::fir::sr_sym::fir_sr_sym_graph<T_DATA, T_COEF, N_FIR_TAPS, FIR_DOWNSHIFT, FIR_ROUND_MODE, FIR_WINDOW_SIZE, N_AIES_PER_FIR> (FIR_TAP_COEFS)
#endif
#if (N_FIR_FILTERS >= 7)
      , dsplib::fir::sr_sym::fir_sr_sym_graph<T_DATA, T_COEF, N_FIR_TAPS, FIR_DOWNSHIFT, FIR_ROUND_MODE, FIR_WINDOW_SIZE, N_AIES_PER_FIR> (FIR_TAP_COEFS)
#endif
#if (N_FIR_FILTERS >= 8)
      , dsplib::fir::sr_sym::fir_sr_sym_graph<T_DATA, T_COEF, N_FIR_TAPS, FIR_DOWNSHIFT, FIR_ROUND_MODE, FIR_WINDOW_SIZE, N_AIES_PER_FIR> (FIR_TAP_COEFS)
#endif
#if (N_FIR_FILTERS >= 9)
      , dsplib::fir::sr_sym::fir_sr_sym_graph<T_DATA, T_COEF, N_FIR_TAPS, FIR_DOWNSHIFT, FIR_ROUND_MODE, FIR_WINDOW_SIZE, N_AIES_PER_FIR> (FIR_TAP_COEFS)
#endif
#if (N_FIR_FILTERS >= 10)
      , dsplib::fir::sr_sym::fir_sr_sym_graph<T_DATA, T_COEF, N_FIR_TAPS, FIR_DOWNSHIFT, FIR_ROUND_MODE, FIR_WINDOW_SIZE, N_AIES_PER_FIR> (FIR_TAP_COEFS)
#endif

	}
	{
		int ix;

		connect<>(in.out[0], FIR_ARRAY[0].in[0]);
		if (N_FIR_FILTERS > 1)  {
			for (ix = 1; ix < N_FIR_FILTERS; ix++)  {
				connect<>(FIR_ARRAY[ix-1].out[0], FIR_ARRAY[ix].in[0]);
			}
		}
		connect<>(FIR_ARRAY[N_FIR_FILTERS-1].out[0], out.in[0]);

	};
};

