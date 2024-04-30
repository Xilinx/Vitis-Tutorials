//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "fft_subsys.h"

#define PLFreq 520

using namespace adf ;


class dut: public graph {

public:

	input_plio      in[64];
	input_plio  mid_in[64];
	output_plio  mid_out[64];
	output_plio   out[64];


	ssr_fft_fp_1mpt_graph<3> fft;

    dut(){

		in[  0]   = input_plio::create( "fft_in_00a",   plio_64_bits, "data/fft_din_00a.txt", PLFreq);
		in[  1]   = input_plio::create( "fft_in_00b",   plio_64_bits, "data/fft_din_00b.txt", PLFreq);
		in[  2]   = input_plio::create( "fft_in_02a",   plio_64_bits, "data/fft_din_02a.txt", PLFreq);
		in[  3]   = input_plio::create( "fft_in_02b",   plio_64_bits, "data/fft_din_02b.txt", PLFreq);
		in[  4]   = input_plio::create( "fft_in_04a",   plio_64_bits, "data/fft_din_04a.txt", PLFreq);
		in[  5]   = input_plio::create( "fft_in_04b",   plio_64_bits, "data/fft_din_04b.txt", PLFreq);
		in[  6]   = input_plio::create( "fft_in_06a",   plio_64_bits, "data/fft_din_06a.txt", PLFreq);
		in[  7]   = input_plio::create( "fft_in_06b",   plio_64_bits, "data/fft_din_06b.txt", PLFreq);

		in[0+8]   = input_plio::create( "fft_in_08a",   plio_64_bits, "data/fft_din_08a.txt", PLFreq);
		in[1+8]   = input_plio::create( "fft_in_08b",   plio_64_bits, "data/fft_din_08b.txt", PLFreq);
		in[2+8]   = input_plio::create( "fft_in_10a",   plio_64_bits, "data/fft_din_10a.txt", PLFreq);
		in[3+8]   = input_plio::create( "fft_in_10b",   plio_64_bits, "data/fft_din_10b.txt", PLFreq);
		in[4+8]   = input_plio::create( "fft_in_12a",   plio_64_bits, "data/fft_din_12a.txt", PLFreq);
		in[5+8]   = input_plio::create( "fft_in_12b",   plio_64_bits, "data/fft_din_12b.txt", PLFreq);
		in[6+8]   = input_plio::create( "fft_in_14a",   plio_64_bits, "data/fft_din_14a.txt", PLFreq);
		in[7+8]   = input_plio::create( "fft_in_14b",   plio_64_bits, "data/fft_din_14b.txt", PLFreq);

		in[0+8*2] = input_plio::create( "fft_in_16a",   plio_64_bits, "data/fft_din_16a.txt", PLFreq);
		in[1+8*2] = input_plio::create( "fft_in_16b",   plio_64_bits, "data/fft_din_16b.txt", PLFreq);
		in[2+8*2] = input_plio::create( "fft_in_18a",   plio_64_bits, "data/fft_din_18a.txt", PLFreq);
		in[3+8*2] = input_plio::create( "fft_in_18b",   plio_64_bits, "data/fft_din_18b.txt", PLFreq);
		in[4+8*2] = input_plio::create( "fft_in_20a",   plio_64_bits, "data/fft_din_20a.txt", PLFreq);
		in[5+8*2] = input_plio::create( "fft_in_20b",   plio_64_bits, "data/fft_din_20b.txt", PLFreq);
		in[6+8*2] = input_plio::create( "fft_in_22a",   plio_64_bits, "data/fft_din_22a.txt", PLFreq);
		in[7+8*2] = input_plio::create( "fft_in_22b",   plio_64_bits, "data/fft_din_22b.txt", PLFreq);

		in[0+8*3] = input_plio::create( "fft_in_24a",   plio_64_bits, "data/fft_din_24a.txt", PLFreq);
		in[1+8*3] = input_plio::create( "fft_in_24b",   plio_64_bits, "data/fft_din_24b.txt", PLFreq);
		in[2+8*3] = input_plio::create( "fft_in_26a",   plio_64_bits, "data/fft_din_26a.txt", PLFreq);
		in[3+8*3] = input_plio::create( "fft_in_26b",   plio_64_bits, "data/fft_din_26b.txt", PLFreq);
		in[4+8*3] = input_plio::create( "fft_in_28a",   plio_64_bits, "data/fft_din_28a.txt", PLFreq);
		in[5+8*3] = input_plio::create( "fft_in_28b",   plio_64_bits, "data/fft_din_28b.txt", PLFreq);
		in[6+8*3] = input_plio::create( "fft_in_30a",   plio_64_bits, "data/fft_din_30a.txt", PLFreq);
		in[7+8*3] = input_plio::create( "fft_in_30b",   plio_64_bits, "data/fft_din_30b.txt", PLFreq);


		in[0+8*4] = input_plio::create( "fft_in_01a",   plio_64_bits, "data/fft_din_01a.txt", PLFreq);
		in[1+8*4] = input_plio::create( "fft_in_01b",   plio_64_bits, "data/fft_din_01b.txt", PLFreq);
		in[2+8*4] = input_plio::create( "fft_in_03a",   plio_64_bits, "data/fft_din_03a.txt", PLFreq);
		in[3+8*4] = input_plio::create( "fft_in_03b",   plio_64_bits, "data/fft_din_03b.txt", PLFreq);
		in[4+8*4] = input_plio::create( "fft_in_05a",   plio_64_bits, "data/fft_din_05a.txt", PLFreq);
		in[5+8*4] = input_plio::create( "fft_in_05b",   plio_64_bits, "data/fft_din_05b.txt", PLFreq);
		in[6+8*4] = input_plio::create( "fft_in_07a",   plio_64_bits, "data/fft_din_07a.txt", PLFreq);
		in[7+8*4] = input_plio::create( "fft_in_07b",   plio_64_bits, "data/fft_din_07b.txt", PLFreq);

		in[0+8*5] = input_plio::create( "fft_in_09a",   plio_64_bits, "data/fft_din_09a.txt", PLFreq);
		in[1+8*5] = input_plio::create( "fft_in_09b",   plio_64_bits, "data/fft_din_09b.txt", PLFreq);
		in[2+8*5] = input_plio::create( "fft_in_11a",   plio_64_bits, "data/fft_din_11a.txt", PLFreq);
		in[3+8*5] = input_plio::create( "fft_in_11b",   plio_64_bits, "data/fft_din_11b.txt", PLFreq);
		in[4+8*5] = input_plio::create( "fft_in_13a",   plio_64_bits, "data/fft_din_13a.txt", PLFreq);
		in[5+8*5] = input_plio::create( "fft_in_13b",   plio_64_bits, "data/fft_din_13b.txt", PLFreq);
		in[6+8*5] = input_plio::create( "fft_in_15a",   plio_64_bits, "data/fft_din_15a.txt", PLFreq);
		in[7+8*5] = input_plio::create( "fft_in_15b",   plio_64_bits, "data/fft_din_15b.txt", PLFreq);

		in[0+8*6] = input_plio::create( "fft_in_17a",   plio_64_bits, "data/fft_din_17a.txt", PLFreq);
		in[1+8*6] = input_plio::create( "fft_in_17b",   plio_64_bits, "data/fft_din_17b.txt", PLFreq);
		in[2+8*6] = input_plio::create( "fft_in_19a",   plio_64_bits, "data/fft_din_19a.txt", PLFreq);
		in[3+8*6] = input_plio::create( "fft_in_19b",   plio_64_bits, "data/fft_din_19b.txt", PLFreq);
		in[4+8*6] = input_plio::create( "fft_in_21a",   plio_64_bits, "data/fft_din_21a.txt", PLFreq);
		in[5+8*6] = input_plio::create( "fft_in_21b",   plio_64_bits, "data/fft_din_21b.txt", PLFreq);
		in[6+8*6] = input_plio::create( "fft_in_23a",   plio_64_bits, "data/fft_din_23a.txt", PLFreq);
		in[7+8*6] = input_plio::create( "fft_in_23b",   plio_64_bits, "data/fft_din_23b.txt", PLFreq);

		in[0+8*7] = input_plio::create( "fft_in_25a",   plio_64_bits, "data/fft_din_25a.txt", PLFreq);
		in[1+8*7] = input_plio::create( "fft_in_25b",   plio_64_bits, "data/fft_din_25b.txt", PLFreq);
		in[2+8*7] = input_plio::create( "fft_in_27a",   plio_64_bits, "data/fft_din_27a.txt", PLFreq);
		in[3+8*7] = input_plio::create( "fft_in_27b",   plio_64_bits, "data/fft_din_27b.txt", PLFreq);
		in[4+8*7] = input_plio::create( "fft_in_29a",   plio_64_bits, "data/fft_din_29a.txt", PLFreq);
		in[5+8*7] = input_plio::create( "fft_in_29b",   plio_64_bits, "data/fft_din_29b.txt", PLFreq);
		in[6+8*7] = input_plio::create( "fft_in_31a",   plio_64_bits, "data/fft_din_31a.txt", PLFreq);
		in[7+8*7] = input_plio::create( "fft_in_31b",   plio_64_bits, "data/fft_din_31b.txt", PLFreq);

		mid_out[0] = output_plio::create("fft_mid_out_00a",  plio_64_bits, "data/fft_mid_out_00a.txt", PLFreq);
		mid_out[1] = output_plio::create("fft_mid_out_00b",  plio_64_bits, "data/fft_mid_out_00b.txt", PLFreq);
		mid_out[2] = output_plio::create("fft_mid_out_02a",  plio_64_bits, "data/fft_mid_out_02a.txt", PLFreq);
		mid_out[3] = output_plio::create("fft_mid_out_02b",  plio_64_bits, "data/fft_mid_out_02b.txt", PLFreq);
		mid_out[4] = output_plio::create("fft_mid_out_04a",  plio_64_bits, "data/fft_mid_out_04a.txt", PLFreq);
		mid_out[5] = output_plio::create("fft_mid_out_04b",  plio_64_bits, "data/fft_mid_out_04b.txt", PLFreq);
		mid_out[6] = output_plio::create("fft_mid_out_06a",  plio_64_bits, "data/fft_mid_out_06a.txt", PLFreq);
		mid_out[7] = output_plio::create("fft_mid_out_06b",  plio_64_bits, "data/fft_mid_out_06b.txt", PLFreq);

		mid_out[0+8]   = output_plio::create( "fft_mid_out_08a",   plio_64_bits, "data/fft_mid_out_08a.txt", PLFreq);
		mid_out[1+8]   = output_plio::create( "fft_mid_out_08b",   plio_64_bits, "data/fft_mid_out_08b.txt", PLFreq);
		mid_out[2+8]   = output_plio::create( "fft_mid_out_10a",   plio_64_bits, "data/fft_mid_out_10a.txt", PLFreq);
		mid_out[3+8]   = output_plio::create( "fft_mid_out_10b",   plio_64_bits, "data/fft_mid_out_10b.txt", PLFreq);
		mid_out[4+8]   = output_plio::create( "fft_mid_out_12a",   plio_64_bits, "data/fft_mid_out_12a.txt", PLFreq);
		mid_out[5+8]   = output_plio::create( "fft_mid_out_12b",   plio_64_bits, "data/fft_mid_out_12b.txt", PLFreq);
		mid_out[6+8]   = output_plio::create( "fft_mid_out_14a",   plio_64_bits, "data/fft_mid_out_14a.txt", PLFreq);
		mid_out[7+8]   = output_plio::create( "fft_mid_out_14b",   plio_64_bits, "data/fft_mid_out_14b.txt", PLFreq);

		mid_out[0+8*2] = output_plio::create( "fft_mid_out_16a",   plio_64_bits, "data/fft_mid_out_16a.txt", PLFreq);
		mid_out[1+8*2] = output_plio::create( "fft_mid_out_16b",   plio_64_bits, "data/fft_mid_out_16b.txt", PLFreq);
		mid_out[2+8*2] = output_plio::create( "fft_mid_out_18a",   plio_64_bits, "data/fft_mid_out_18a.txt", PLFreq);
		mid_out[3+8*2] = output_plio::create( "fft_mid_out_18b",   plio_64_bits, "data/fft_mid_out_18b.txt", PLFreq);
		mid_out[4+8*2] = output_plio::create( "fft_mid_out_20a",   plio_64_bits, "data/fft_mid_out_20a.txt", PLFreq);
		mid_out[5+8*2] = output_plio::create( "fft_mid_out_20b",   plio_64_bits, "data/fft_mid_out_20b.txt", PLFreq);
		mid_out[6+8*2] = output_plio::create( "fft_mid_out_22a",   plio_64_bits, "data/fft_mid_out_22a.txt", PLFreq);
		mid_out[7+8*2] = output_plio::create( "fft_mid_out_22b",   plio_64_bits, "data/fft_mid_out_22b.txt", PLFreq);

		mid_out[0+8*3] = output_plio::create( "fft_mid_out_24a",   plio_64_bits, "data/fft_mid_out_24a.txt", PLFreq);
		mid_out[1+8*3] = output_plio::create( "fft_mid_out_24b",   plio_64_bits, "data/fft_mid_out_24b.txt", PLFreq);
		mid_out[2+8*3] = output_plio::create( "fft_mid_out_26a",   plio_64_bits, "data/fft_mid_out_26a.txt", PLFreq);
		mid_out[3+8*3] = output_plio::create( "fft_mid_out_26b",   plio_64_bits, "data/fft_mid_out_26b.txt", PLFreq);
		mid_out[4+8*3] = output_plio::create( "fft_mid_out_28a",   plio_64_bits, "data/fft_mid_out_28a.txt", PLFreq);
		mid_out[5+8*3] = output_plio::create( "fft_mid_out_28b",   plio_64_bits, "data/fft_mid_out_28b.txt", PLFreq);
		mid_out[6+8*3] = output_plio::create( "fft_mid_out_30a",   plio_64_bits, "data/fft_mid_out_30a.txt", PLFreq);
		mid_out[7+8*3] = output_plio::create( "fft_mid_out_30b",   plio_64_bits, "data/fft_mid_out_30b.txt", PLFreq);

		mid_out[0+8*4] = output_plio::create( "fft_mid_out_01a",   plio_64_bits, "data/fft_mid_out_01a.txt", PLFreq);
		mid_out[1+8*4] = output_plio::create( "fft_mid_out_01b",   plio_64_bits, "data/fft_mid_out_01b.txt", PLFreq);
		mid_out[2+8*4] = output_plio::create( "fft_mid_out_03a",   plio_64_bits, "data/fft_mid_out_03a.txt", PLFreq);
		mid_out[3+8*4] = output_plio::create( "fft_mid_out_03b",   plio_64_bits, "data/fft_mid_out_03b.txt", PLFreq);
		mid_out[4+8*4] = output_plio::create( "fft_mid_out_05a",   plio_64_bits, "data/fft_mid_out_05a.txt", PLFreq);
		mid_out[5+8*4] = output_plio::create( "fft_mid_out_05b",   plio_64_bits, "data/fft_mid_out_05b.txt", PLFreq);
		mid_out[6+8*4] = output_plio::create( "fft_mid_out_07a",   plio_64_bits, "data/fft_mid_out_07a.txt", PLFreq);
		mid_out[7+8*4] = output_plio::create( "fft_mid_out_07b",   plio_64_bits, "data/fft_mid_out_07b.txt", PLFreq);

		mid_out[0+8*5] = output_plio::create( "fft_mid_out_09a",   plio_64_bits, "data/fft_mid_out_09a.txt", PLFreq);
		mid_out[1+8*5] = output_plio::create( "fft_mid_out_09b",   plio_64_bits, "data/fft_mid_out_09b.txt", PLFreq);
		mid_out[2+8*5] = output_plio::create( "fft_mid_out_11a",   plio_64_bits, "data/fft_mid_out_11a.txt", PLFreq);
		mid_out[3+8*5] = output_plio::create( "fft_mid_out_11b",   plio_64_bits, "data/fft_mid_out_11b.txt", PLFreq);
		mid_out[4+8*5] = output_plio::create( "fft_mid_out_13a",   plio_64_bits, "data/fft_mid_out_13a.txt", PLFreq);
		mid_out[5+8*5] = output_plio::create( "fft_mid_out_13b",   plio_64_bits, "data/fft_mid_out_13b.txt", PLFreq);
		mid_out[6+8*5] = output_plio::create( "fft_mid_out_15a",   plio_64_bits, "data/fft_mid_out_15a.txt", PLFreq);
		mid_out[7+8*5] = output_plio::create( "fft_mid_out_15b",   plio_64_bits, "data/fft_mid_out_15b.txt", PLFreq);

		mid_out[0+8*6] = output_plio::create( "fft_mid_out_17a",   plio_64_bits, "data/fft_mid_out_17a.txt", PLFreq);
		mid_out[1+8*6] = output_plio::create( "fft_mid_out_17b",   plio_64_bits, "data/fft_mid_out_17b.txt", PLFreq);
		mid_out[2+8*6] = output_plio::create( "fft_mid_out_19a",   plio_64_bits, "data/fft_mid_out_19a.txt", PLFreq);
		mid_out[3+8*6] = output_plio::create( "fft_mid_out_19b",   plio_64_bits, "data/fft_mid_out_19b.txt", PLFreq);
		mid_out[4+8*6] = output_plio::create( "fft_mid_out_21a",   plio_64_bits, "data/fft_mid_out_21a.txt", PLFreq);
		mid_out[5+8*6] = output_plio::create( "fft_mid_out_21b",   plio_64_bits, "data/fft_mid_out_21b.txt", PLFreq);
		mid_out[6+8*6] = output_plio::create( "fft_mid_out_23a",   plio_64_bits, "data/fft_mid_out_23a.txt", PLFreq);
		mid_out[7+8*6] = output_plio::create( "fft_mid_out_23b",   plio_64_bits, "data/fft_mid_out_23b.txt", PLFreq);

		mid_out[0+8*7] = output_plio::create( "fft_mid_out_25a",   plio_64_bits, "data/fft_mid_out_25a.txt", PLFreq);
		mid_out[1+8*7] = output_plio::create( "fft_mid_out_25b",   plio_64_bits, "data/fft_mid_out_25b.txt", PLFreq);
		mid_out[2+8*7] = output_plio::create( "fft_mid_out_27a",   plio_64_bits, "data/fft_mid_out_27a.txt", PLFreq);
		mid_out[3+8*7] = output_plio::create( "fft_mid_out_27b",   plio_64_bits, "data/fft_mid_out_27b.txt", PLFreq);
		mid_out[4+8*7] = output_plio::create( "fft_mid_out_29a",   plio_64_bits, "data/fft_mid_out_29a.txt", PLFreq);
		mid_out[5+8*7] = output_plio::create( "fft_mid_out_29b",   plio_64_bits, "data/fft_mid_out_29b.txt", PLFreq);
		mid_out[6+8*7] = output_plio::create( "fft_mid_out_31a",   plio_64_bits, "data/fft_mid_out_31a.txt", PLFreq);
		mid_out[7+8*7] = output_plio::create( "fft_mid_out_31b",   plio_64_bits, "data/fft_mid_out_31b.txt", PLFreq);

		mid_in[  0]   = input_plio::create( "fft_mid_in_00a",   plio_64_bits, "data/fft_mid_in_00a.txt", PLFreq);
		mid_in[  1]   = input_plio::create( "fft_mid_in_00b",   plio_64_bits, "data/fft_mid_in_00b.txt", PLFreq);
		mid_in[  2]   = input_plio::create( "fft_mid_in_01a",   plio_64_bits, "data/fft_mid_in_01a.txt", PLFreq);
		mid_in[  3]   = input_plio::create( "fft_mid_in_01b",   plio_64_bits, "data/fft_mid_in_01b.txt", PLFreq);
		mid_in[  4]   = input_plio::create( "fft_mid_in_02a",   plio_64_bits, "data/fft_mid_in_02a.txt", PLFreq);
		mid_in[  5]   = input_plio::create( "fft_mid_in_02b",   plio_64_bits, "data/fft_mid_in_02b.txt", PLFreq);
		mid_in[  6]   = input_plio::create( "fft_mid_in_03a",   plio_64_bits, "data/fft_mid_in_03a.txt", PLFreq);
		mid_in[  7]   = input_plio::create( "fft_mid_in_03b",   plio_64_bits, "data/fft_mid_in_03b.txt", PLFreq);

		mid_in[0+8]   = input_plio::create( "fft_mid_in_08a",   plio_64_bits, "data/fft_mid_in_08a.txt", PLFreq);
		mid_in[1+8]   = input_plio::create( "fft_mid_in_08b",   plio_64_bits, "data/fft_mid_in_08b.txt", PLFreq);
		mid_in[2+8]   = input_plio::create( "fft_mid_in_09a",   plio_64_bits, "data/fft_mid_in_09a.txt", PLFreq);
		mid_in[3+8]   = input_plio::create( "fft_mid_in_09b",   plio_64_bits, "data/fft_mid_in_09b.txt", PLFreq);
		mid_in[4+8]   = input_plio::create( "fft_mid_in_10a",   plio_64_bits, "data/fft_mid_in_10a.txt", PLFreq);
		mid_in[5+8]   = input_plio::create( "fft_mid_in_10b",   plio_64_bits, "data/fft_mid_in_10b.txt", PLFreq);
		mid_in[6+8]   = input_plio::create( "fft_mid_in_11a",   plio_64_bits, "data/fft_mid_in_11a.txt", PLFreq);
		mid_in[7+8]   = input_plio::create( "fft_mid_in_11b",   plio_64_bits, "data/fft_mid_in_11b.txt", PLFreq);

		mid_in[0+8*2] = input_plio::create( "fft_mid_in_16a",   plio_64_bits, "data/fft_mid_in_16a.txt", PLFreq);
		mid_in[1+8*2] = input_plio::create( "fft_mid_in_16b",   plio_64_bits, "data/fft_mid_in_16b.txt", PLFreq);
		mid_in[2+8*2] = input_plio::create( "fft_mid_in_17a",   plio_64_bits, "data/fft_mid_in_17a.txt", PLFreq);
		mid_in[3+8*2] = input_plio::create( "fft_mid_in_17b",   plio_64_bits, "data/fft_mid_in_17b.txt", PLFreq);
		mid_in[4+8*2] = input_plio::create( "fft_mid_in_18a",   plio_64_bits, "data/fft_mid_in_18a.txt", PLFreq);
		mid_in[5+8*2] = input_plio::create( "fft_mid_in_18b",   plio_64_bits, "data/fft_mid_in_18b.txt", PLFreq);
		mid_in[6+8*2] = input_plio::create( "fft_mid_in_19a",   plio_64_bits, "data/fft_mid_in_19a.txt", PLFreq);
		mid_in[7+8*2] = input_plio::create( "fft_mid_in_19b",   plio_64_bits, "data/fft_mid_in_19b.txt", PLFreq);

		mid_in[0+8*3] = input_plio::create( "fft_mid_in_24a",   plio_64_bits, "data/fft_mid_in_24a.txt", PLFreq);
		mid_in[1+8*3] = input_plio::create( "fft_mid_in_24b",   plio_64_bits, "data/fft_mid_in_24b.txt", PLFreq);
		mid_in[2+8*3] = input_plio::create( "fft_mid_in_25a",   plio_64_bits, "data/fft_mid_in_25a.txt", PLFreq);
		mid_in[3+8*3] = input_plio::create( "fft_mid_in_25b",   plio_64_bits, "data/fft_mid_in_25b.txt", PLFreq);
		mid_in[4+8*3] = input_plio::create( "fft_mid_in_26a",   plio_64_bits, "data/fft_mid_in_26a.txt", PLFreq);
		mid_in[5+8*3] = input_plio::create( "fft_mid_in_26b",   plio_64_bits, "data/fft_mid_in_26b.txt", PLFreq);
		mid_in[6+8*3] = input_plio::create( "fft_mid_in_27a",   plio_64_bits, "data/fft_mid_in_27a.txt", PLFreq);
		mid_in[7+8*3] = input_plio::create( "fft_mid_in_27b",   plio_64_bits, "data/fft_mid_in_27b.txt", PLFreq);

		mid_in[0+8*4] = input_plio::create( "fft_mid_in_04a",   plio_64_bits, "data/fft_mid_in_04a.txt", PLFreq);
		mid_in[1+8*4] = input_plio::create( "fft_mid_in_04b",   plio_64_bits, "data/fft_mid_in_04b.txt", PLFreq);
		mid_in[2+8*4] = input_plio::create( "fft_mid_in_05a",   plio_64_bits, "data/fft_mid_in_05a.txt", PLFreq);
		mid_in[3+8*4] = input_plio::create( "fft_mid_in_05b",   plio_64_bits, "data/fft_mid_in_05b.txt", PLFreq);
		mid_in[4+8*4] = input_plio::create( "fft_mid_in_06a",   plio_64_bits, "data/fft_mid_in_06a.txt", PLFreq);
		mid_in[5+8*4] = input_plio::create( "fft_mid_in_06b",   plio_64_bits, "data/fft_mid_in_06b.txt", PLFreq);
		mid_in[6+8*4] = input_plio::create( "fft_mid_in_07a",   plio_64_bits, "data/fft_mid_in_07a.txt", PLFreq);
		mid_in[7+8*4] = input_plio::create( "fft_mid_in_07b",   plio_64_bits, "data/fft_mid_in_07b.txt", PLFreq);

		mid_in[0+8*5] = input_plio::create( "fft_mid_in_12a",   plio_64_bits, "data/fft_mid_in_12a.txt", PLFreq);
		mid_in[1+8*5] = input_plio::create( "fft_mid_in_12b",   plio_64_bits, "data/fft_mid_in_12b.txt", PLFreq);
		mid_in[2+8*5] = input_plio::create( "fft_mid_in_13a",   plio_64_bits, "data/fft_mid_in_13a.txt", PLFreq);
		mid_in[3+8*5] = input_plio::create( "fft_mid_in_13b",   plio_64_bits, "data/fft_mid_in_13b.txt", PLFreq);
		mid_in[4+8*5] = input_plio::create( "fft_mid_in_14a",   plio_64_bits, "data/fft_mid_in_14a.txt", PLFreq);
		mid_in[5+8*5] = input_plio::create( "fft_mid_in_14b",   plio_64_bits, "data/fft_mid_in_14b.txt", PLFreq);
		mid_in[6+8*5] = input_plio::create( "fft_mid_in_15a",   plio_64_bits, "data/fft_mid_in_15a.txt", PLFreq);
		mid_in[7+8*5] = input_plio::create( "fft_mid_in_15b",   plio_64_bits, "data/fft_mid_in_15b.txt", PLFreq);

		mid_in[0+8*6] = input_plio::create( "fft_mid_in_20a",   plio_64_bits, "data/fft_mid_in_20a.txt", PLFreq);
		mid_in[1+8*6] = input_plio::create( "fft_mid_in_20b",   plio_64_bits, "data/fft_mid_in_20b.txt", PLFreq);
		mid_in[2+8*6] = input_plio::create( "fft_mid_in_21a",   plio_64_bits, "data/fft_mid_in_21a.txt", PLFreq);
		mid_in[3+8*6] = input_plio::create( "fft_mid_in_21b",   plio_64_bits, "data/fft_mid_in_21b.txt", PLFreq);
		mid_in[4+8*6] = input_plio::create( "fft_mid_in_22a",   plio_64_bits, "data/fft_mid_in_22a.txt", PLFreq);
		mid_in[5+8*6] = input_plio::create( "fft_mid_in_22b",   plio_64_bits, "data/fft_mid_in_22b.txt", PLFreq);
		mid_in[6+8*6] = input_plio::create( "fft_mid_in_23a",   plio_64_bits, "data/fft_mid_in_23a.txt", PLFreq);
		mid_in[7+8*6] = input_plio::create( "fft_mid_in_23b",   plio_64_bits, "data/fft_mid_in_23b.txt", PLFreq);

		mid_in[0+8*7] = input_plio::create( "fft_mid_in_28a",   plio_64_bits, "data/fft_mid_in_28a.txt", PLFreq);
		mid_in[1+8*7] = input_plio::create( "fft_mid_in_28b",   plio_64_bits, "data/fft_mid_in_28b.txt", PLFreq);
		mid_in[2+8*7] = input_plio::create( "fft_mid_in_29a",   plio_64_bits, "data/fft_mid_in_29a.txt", PLFreq);
		mid_in[3+8*7] = input_plio::create( "fft_mid_in_29b",   plio_64_bits, "data/fft_mid_in_29b.txt", PLFreq);
		mid_in[4+8*7] = input_plio::create( "fft_mid_in_30a",   plio_64_bits, "data/fft_mid_in_30a.txt", PLFreq);
		mid_in[5+8*7] = input_plio::create( "fft_mid_in_30b",   plio_64_bits, "data/fft_mid_in_30b.txt", PLFreq);
		mid_in[6+8*7] = input_plio::create( "fft_mid_in_31a",   plio_64_bits, "data/fft_mid_in_31a.txt", PLFreq);
		mid_in[7+8*7] = input_plio::create( "fft_mid_in_31b",   plio_64_bits, "data/fft_mid_in_31b.txt", PLFreq);

		out[0] = output_plio::create("fft_out_00a",  plio_64_bits, "data/fft_out_00a.txt", PLFreq);
		out[1] = output_plio::create("fft_out_00b",  plio_64_bits, "data/fft_out_00b.txt", PLFreq);
		out[2] = output_plio::create("fft_out_01a",  plio_64_bits, "data/fft_out_01a.txt", PLFreq);
		out[3] = output_plio::create("fft_out_01b",  plio_64_bits, "data/fft_out_01b.txt", PLFreq);
		out[4] = output_plio::create("fft_out_02a",  plio_64_bits, "data/fft_out_02a.txt", PLFreq);
		out[5] = output_plio::create("fft_out_02b",  plio_64_bits, "data/fft_out_02b.txt", PLFreq);
		out[6] = output_plio::create("fft_out_03a",  plio_64_bits, "data/fft_out_03a.txt", PLFreq);
		out[7] = output_plio::create("fft_out_03b",  plio_64_bits, "data/fft_out_03b.txt", PLFreq);

		out[0+8]   = output_plio::create( "fft_out_08a",   plio_64_bits, "data/fft_out_08a.txt", PLFreq);
		out[1+8]   = output_plio::create( "fft_out_08b",   plio_64_bits, "data/fft_out_08b.txt", PLFreq);
		out[2+8]   = output_plio::create( "fft_out_09a",   plio_64_bits, "data/fft_out_09a.txt", PLFreq);
		out[3+8]   = output_plio::create( "fft_out_09b",   plio_64_bits, "data/fft_out_09b.txt", PLFreq);
		out[4+8]   = output_plio::create( "fft_out_10a",   plio_64_bits, "data/fft_out_10a.txt", PLFreq);
		out[5+8]   = output_plio::create( "fft_out_10b",   plio_64_bits, "data/fft_out_10b.txt", PLFreq);
		out[6+8]   = output_plio::create( "fft_out_11a",   plio_64_bits, "data/fft_out_11a.txt", PLFreq);
		out[7+8]   = output_plio::create( "fft_out_11b",   plio_64_bits, "data/fft_out_11b.txt", PLFreq);

		out[0+8*2] = output_plio::create( "fft_out_16a",   plio_64_bits, "data/fft_out_16a.txt", PLFreq);
		out[1+8*2] = output_plio::create( "fft_out_16b",   plio_64_bits, "data/fft_out_16b.txt", PLFreq);
		out[2+8*2] = output_plio::create( "fft_out_17a",   plio_64_bits, "data/fft_out_17a.txt", PLFreq);
		out[3+8*2] = output_plio::create( "fft_out_17b",   plio_64_bits, "data/fft_out_17b.txt", PLFreq);
		out[4+8*2] = output_plio::create( "fft_out_18a",   plio_64_bits, "data/fft_out_18a.txt", PLFreq);
		out[5+8*2] = output_plio::create( "fft_out_18b",   plio_64_bits, "data/fft_out_18b.txt", PLFreq);
		out[6+8*2] = output_plio::create( "fft_out_19a",   plio_64_bits, "data/fft_out_19a.txt", PLFreq);
		out[7+8*2] = output_plio::create( "fft_out_19b",   plio_64_bits, "data/fft_out_19b.txt", PLFreq);

		out[0+8*3] = output_plio::create( "fft_out_24a",   plio_64_bits, "data/fft_out_24a.txt", PLFreq);
		out[1+8*3] = output_plio::create( "fft_out_24b",   plio_64_bits, "data/fft_out_24b.txt", PLFreq);
		out[2+8*3] = output_plio::create( "fft_out_25a",   plio_64_bits, "data/fft_out_25a.txt", PLFreq);
		out[3+8*3] = output_plio::create( "fft_out_25b",   plio_64_bits, "data/fft_out_25b.txt", PLFreq);
		out[4+8*3] = output_plio::create( "fft_out_26a",   plio_64_bits, "data/fft_out_26a.txt", PLFreq);
		out[5+8*3] = output_plio::create( "fft_out_26b",   plio_64_bits, "data/fft_out_26b.txt", PLFreq);
		out[6+8*3] = output_plio::create( "fft_out_27a",   plio_64_bits, "data/fft_out_27a.txt", PLFreq);
		out[7+8*3] = output_plio::create( "fft_out_27b",   plio_64_bits, "data/fft_out_27b.txt", PLFreq);

		out[0+8*4] = output_plio::create( "fft_out_04a",   plio_64_bits, "data/fft_out_04a.txt", PLFreq);
		out[1+8*4] = output_plio::create( "fft_out_04b",   plio_64_bits, "data/fft_out_04b.txt", PLFreq);
		out[2+8*4] = output_plio::create( "fft_out_05a",   plio_64_bits, "data/fft_out_05a.txt", PLFreq);
		out[3+8*4] = output_plio::create( "fft_out_05b",   plio_64_bits, "data/fft_out_05b.txt", PLFreq);
		out[4+8*4] = output_plio::create( "fft_out_06a",   plio_64_bits, "data/fft_out_06a.txt", PLFreq);
		out[5+8*4] = output_plio::create( "fft_out_06b",   plio_64_bits, "data/fft_out_06b.txt", PLFreq);
		out[6+8*4] = output_plio::create( "fft_out_07a",   plio_64_bits, "data/fft_out_07a.txt", PLFreq);
		out[7+8*4] = output_plio::create( "fft_out_07b",   plio_64_bits, "data/fft_out_07b.txt", PLFreq);

		out[0+8*5] = output_plio::create( "fft_out_12a",   plio_64_bits, "data/fft_out_12a.txt", PLFreq);
		out[1+8*5] = output_plio::create( "fft_out_12b",   plio_64_bits, "data/fft_out_12b.txt", PLFreq);
		out[2+8*5] = output_plio::create( "fft_out_13a",   plio_64_bits, "data/fft_out_13a.txt", PLFreq);
		out[3+8*5] = output_plio::create( "fft_out_13b",   plio_64_bits, "data/fft_out_13b.txt", PLFreq);
		out[4+8*5] = output_plio::create( "fft_out_14a",   plio_64_bits, "data/fft_out_14a.txt", PLFreq);
		out[5+8*5] = output_plio::create( "fft_out_14b",   plio_64_bits, "data/fft_out_14b.txt", PLFreq);
		out[6+8*5] = output_plio::create( "fft_out_15a",   plio_64_bits, "data/fft_out_15a.txt", PLFreq);
		out[7+8*5] = output_plio::create( "fft_out_15b",   plio_64_bits, "data/fft_out_15b.txt", PLFreq);

		out[0+8*6] = output_plio::create( "fft_out_20a",   plio_64_bits, "data/fft_out_20a.txt", PLFreq);
		out[1+8*6] = output_plio::create( "fft_out_20b",   plio_64_bits, "data/fft_out_20b.txt", PLFreq);
		out[2+8*6] = output_plio::create( "fft_out_21a",   plio_64_bits, "data/fft_out_21a.txt", PLFreq);
		out[3+8*6] = output_plio::create( "fft_out_21b",   plio_64_bits, "data/fft_out_21b.txt", PLFreq);
		out[4+8*6] = output_plio::create( "fft_out_22a",   plio_64_bits, "data/fft_out_22a.txt", PLFreq);
		out[5+8*6] = output_plio::create( "fft_out_22b",   plio_64_bits, "data/fft_out_22b.txt", PLFreq);
		out[6+8*6] = output_plio::create( "fft_out_23a",   plio_64_bits, "data/fft_out_23a.txt", PLFreq);
		out[7+8*6] = output_plio::create( "fft_out_23b",   plio_64_bits, "data/fft_out_23b.txt", PLFreq);

		out[0+8*7] = output_plio::create( "fft_out_28a",   plio_64_bits, "data/fft_out_28a.txt", PLFreq);
		out[1+8*7] = output_plio::create( "fft_out_28b",   plio_64_bits, "data/fft_out_28b.txt", PLFreq);
		out[2+8*7] = output_plio::create( "fft_out_29a",   plio_64_bits, "data/fft_out_29a.txt", PLFreq);
		out[3+8*7] = output_plio::create( "fft_out_29b",   plio_64_bits, "data/fft_out_29b.txt", PLFreq);
		out[4+8*7] = output_plio::create( "fft_out_30a",   plio_64_bits, "data/fft_out_30a.txt", PLFreq);
		out[5+8*7] = output_plio::create( "fft_out_30b",   plio_64_bits, "data/fft_out_30b.txt", PLFreq);
		out[6+8*7] = output_plio::create( "fft_out_31a",   plio_64_bits, "data/fft_out_31a.txt", PLFreq);
		out[7+8*7] = output_plio::create( "fft_out_31b",   plio_64_bits, "data/fft_out_31b.txt", PLFreq);



		for(int i=0; i<64; i++){
			connect<>(in[i].out[0],     fft.in[i]);
			connect<>(mid_in[i].out[0], fft.mid_in[i]);
			connect<>(fft.out[i], out[i].in[0]);
			connect<>(fft.mid_out[i], mid_out[i].in[0]);
		}


	};

}; // end of class
