/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "kernels.h"


alignas(32) int16_t weights[2][16] =
	{{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16},
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
const int ShiftVal[2] = {7,4};


#if ARCH_AIECMP == 10
void filter_buf(adf::input_buffer<cint16, adf::extents<BUFFER_SIZE>, adf::margin<MARGIN_SIZE>> &__restrict in,
				adf::output_buffer<cint16, adf::extents<BUFFER_SIZE>> &__restrict out,
				int32 FiltSel)
{
	aie::vector<cint16, 32> data; // 1024 bits
	aie::accum<cacc48, 4> acc;	  // 384 bits;
	auto pi1 = aie::begin_vector<8>(in);
	auto po1 = aie::begin_vector<4>(out);

	// Load the coeffs vector with weights
	aie::vector<int16, 16> coeffs = aie::load_v<16>(weights[FiltSel]);
	const int SHIFT_VAL = ShiftVal[FiltSel];

	// Define the operator
	constexpr unsigned Lanes = 4, Points = 16, CoeffStep = 1, DataStepX = 1, DataStepY = 1;

	using mul_ops = aie::sliding_mul_ops<Lanes, Points, CoeffStep, DataStepX, DataStepY, int16, cint16, cacc48>;

	// Reading 256 bits - 0
	data.insert(0, *pi1++);
	// Reading 256 bits - 1
	data.insert(1, *pi1++);

	for (unsigned i = 0; i < BUFFER_SIZE / 32; i++)
		chess_prepare_for_pipelining
			chess_loop_range(4, 32)
		{
			// Reading 256 bits - 2 --> 8 samples
			data.insert(2, *pi1++);

			// mul performs multiplication accumulations with no initial value for the accumulator
			acc = mul_ops::mul(coeffs, 0, data, 1);
			*po1++ = acc.to_vector<cint16>(SHIFT_VAL);

			acc = mul_ops::mul(coeffs, 0, data, 5);
			*po1++ = acc.to_vector<cint16>(SHIFT_VAL);

			// Reading 256 bits - 3 --> 8 samples
			data.insert(3, *pi1++);

			// mul performs multiplication accumulations with no initial value for the accumulator
			acc = mul_ops::mul(coeffs, 0, data, 9);
			*po1++ = acc.to_vector<cint16>(SHIFT_VAL);

			acc = mul_ops::mul(coeffs, 0, data, 13);
			*po1++ = acc.to_vector<cint16>(SHIFT_VAL);

			// Reading 256 bits - 0 --> 8 samples
			data.insert(0, *pi1++);

			// mul performs multiplication accumulations with no initial value for the accumulator
			acc = mul_ops::mul(coeffs, 0, data, 17);
			*po1++ = acc.to_vector<cint16>(SHIFT_VAL);

			acc = mul_ops::mul(coeffs, 0, data, 21);
			*po1++ = acc.to_vector<cint16>(SHIFT_VAL);

			// Reading 256 bits - 1 --> 8 samples
			data.insert(1, *pi1++);

			// mul performs multiplication accumulations with no initial value for the accumulator
			acc = mul_ops::mul(coeffs, 0, data, 25);
			*po1++ = acc.to_vector<cint16>(SHIFT_VAL);

			acc = mul_ops::mul(coeffs, 0, data, 29);
			*po1++ = acc.to_vector<cint16>(SHIFT_VAL);
		}
}

void filter_str(input_stream<cint16> *__restrict in, output_stream<cint16> *__restrict out,
		int32 FiltSel)

{
	static cint16 Memory[32];

	aie::accum<cacc48,4> acc;      // 384 bits;

	// Load the coeffs vector with weights
	aie::vector<int16,16> coeffs = aie::load_v<16>(weights[FiltSel]);
	const int SHIFT_VAL = ShiftVal[FiltSel];

	// Load the data vector with memory
	aie::vector<cint16,32> data = aie::load_v<32>(Memory);

	// Define the operator
	constexpr unsigned Lanes=4, Points=16, CoeffStep=1, DataStepX=1, DataStepY=1;

	using mul_ops = aie::sliding_mul_ops<Lanes, Points, CoeffStep, DataStepX, DataStepY, int16, cint16, cacc48>;

	for(unsigned i=0; i<BUFFER_SIZE/16; i++)
	chess_prepare_for_pipelining
	chess_loop_range(4,32)
	{

		// mul performs multiplication accumulations with no initial value for the accumulator
		data.insert(4, readincr_v<4>(in));
		acc = mul_ops::mul(coeffs, 0, data, 1);
		writeincr(out,acc.to_vector<cint16>(SHIFT_VAL));

		data.insert(5, readincr_v<4>(in));
		acc = mul_ops::mul(coeffs, 0, data, 5);
		writeincr(out,acc.to_vector<cint16>(SHIFT_VAL));


		// mul performs multiplication accumulations with no initial value for the accumulator
		data.insert(6, readincr_v<4>(in));
		acc = mul_ops::mul(coeffs, 0, data, 9);
		writeincr(out,acc.to_vector<cint16>(SHIFT_VAL));

		data.insert(7, readincr_v<4>(in));
		acc = mul_ops::mul(coeffs, 0, data, 13);
		writeincr(out,acc.to_vector<cint16>(SHIFT_VAL));


		// mul performs multiplication accumulations with no initial value for the accumulator
		data.insert(0, readincr_v<4>(in));
		acc = mul_ops::mul(coeffs, 0, data, 17);
		writeincr(out,acc.to_vector<cint16>(SHIFT_VAL));

		data.insert(1, readincr_v<4>(in));
		acc = mul_ops::mul(coeffs, 0, data, 21);
		writeincr(out,acc.to_vector<cint16>(SHIFT_VAL));


		// mul performs multiplication accumulations with no initial value for the accumulator
		data.insert(2, readincr_v<4>(in));
		acc = mul_ops::mul(coeffs, 0, data, 25);
		writeincr(out,acc.to_vector<cint16>(SHIFT_VAL));

		data.insert(3, readincr_v<4>(in));
		acc = mul_ops::mul(coeffs, 0, data, 29);
		writeincr(out,acc.to_vector<cint16>(SHIFT_VAL));
	}

	// Save filter memory
	data.store(Memory);
	
}
#elif ARCH_AIECMP == 20
void filter_buf(adf::input_buffer<cint16, adf::extents<BUFFER_SIZE>, adf::margin<MARGIN_SIZE>> &__restrict in,
				adf::output_buffer<cint16, adf::extents<BUFFER_SIZE>> &__restrict out,
				int32 FiltSel)
{
	aie::vector<cint16, 32> data; // 1024 bits
	aie::accum<cacc64, 16> acc;	  // 512 bits;
	auto pi1 = aie::begin_vector<16>(in);
	auto po1 = aie::begin_vector<16>(out);

	// Load the coeffs vector with weights
	aie::vector<int16, 16> coeffs = aie::load_v<16>(weights[FiltSel]);
	const int SHIFT_VAL = ShiftVal[FiltSel];

	// Define the operator
	constexpr unsigned Lanes = 16, Points = 16, CoeffStep = 1, DataStepX = 1, DataStepY = 1;

	using mul_ops = aie::sliding_mul_ops<Lanes, Points, CoeffStep, DataStepX, DataStepY, int16, cint16, cacc48>;

	// Reading 512 bits - 16 samples
	data.insert(0, *pi1++);

	for (unsigned i = 0; i < BUFFER_SIZE / 32; i++)
		chess_prepare_for_pipelining
			chess_loop_range(4, 32)
		{
			// Reading 512 bits - 1 --> 16 samples
			data.insert(1, *pi1++);

			// mul performs multiplication accumulations with no initial value for the accumulator
			acc = mul_ops::mul(coeffs, 0, data, 1);
			*po1++ = acc.to_vector<cint16>(SHIFT_VAL);

			// Reading 256 bits - 0 --> 16 samples
			data.insert(0, *pi1++);

			// mul performs multiplication accumulations with no initial value for the accumulator
			acc = mul_ops::mul(coeffs, 0, data, 17);
			*po1++ = acc.to_vector<cint16>(SHIFT_VAL);
		}
}
#endif



