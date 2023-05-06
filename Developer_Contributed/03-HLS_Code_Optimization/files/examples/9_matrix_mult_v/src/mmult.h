/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*/


/*
 void example(hls::stream< ap_axis<32,2,5,6> > &A,
	     hls::stream< ap_axis<32,2,5,6> > &B)
{
	#pragma HLS INTERFACE axis port=A
	#pragma HLS INTERFACE axis port=B
	ap_axis<32,2,5,6> tmp;
    while(1)
    {
		A.read(tmp);
		tmp.data = tmp.data.to_int() + 5;
		B.write(tmp);
		if(tmp.last)
     	{
          break;
     	}
    }
*/

#include <assert.h>
#include "ap_axi_sdata.h"
#include "hls_stream.h"

const int DIM = 32;
const int SIZE = (DIM*DIM);

const int U =2;
const int TI=5;
const int TD=6;

typedef float T;
typedef ap_axis<sizeof(T)*8,U,TI,TD> AXI_VAL;



// function prototypes
void standalone_mmult (float A[DIM][DIM], float B[DIM][DIM], float C[DIM][DIM]);

int HLS_accel (hls::stream<AXI_VAL> &INPUT_STREAM, hls::stream<AXI_VAL> &OUTPUT_STREAM);

/* ****************************** C++ TEMPLATES ************************************** */

// reference function
template <typename T, int DIM>
void matrix_multiply_ref(T a[DIM][DIM], T b[DIM][DIM], T out[DIM][DIM])
{
	// matrix multiplication of a A*B matrix
	for (int ia = 0; ia < DIM; ++ia)
		for (int ib = 0; ib < DIM; ++ib)
		{
			float sum = 0;
			for (int id = 0; id < DIM; ++id)
				sum += a[ia][id] * b[id][ib];
			out[ia][ib] = sum;
		}
		return;
}

// --------------------------------------------------------
// function to be accelerated in HW
template <typename T, int DIM>
void mmult_hw(T a[DIM][DIM], T b[DIM][DIM], T out[DIM][DIM])
{

	int const FACTOR = DIM/2;
	#pragma HLS INLINE
	//#pragma HLS array_partition variable=a block factor=FACTOR dim=2
	//#pragma HLS array_partition variable=b block factor=FACTOR dim=1

	// matrix multiplication of a A*B matrix
	L1:for (int ia = 0; ia < DIM; ++ia)
		L2:for (int ib = 0; ib < DIM; ++ib)
		{
			//#pragma HLS PIPELINE II=1
			T sum = 0;
			L3:for (int id = 0; id < DIM; ++id)
				sum += a[ia][id] * b[id][ib];
			out[ia][ib] = sum;
		}
		return;
}

// --------------------------------------------------------
// functions to insert and extract elements from an axi stream
// includes conversion to correct data type

template <typename T, int U, int TI, int TD>
T pop_stream(hls::stream< ap_axis<sizeof(T)*8,U,TI,TD>> &inp_stream)
{
#pragma HLS INLINE

	ap_axis<sizeof(T)*8,U,TI,TD> e;
	inp_stream.read(e);

	assert(sizeof(T) == sizeof(int));
	union
	{
		int ival;
		T oval;
	} converter;
	converter.ival = e.data;
	T ret = converter.oval;

	ap_uint<sizeof(T)> strb = e.strb;
	ap_uint<sizeof(T)> keep = e.keep;
	ap_uint<U> user = e.user;
	ap_uint<1> last = e.last;
	//ap_uint<TI> id = e.id;
	//ap_uint<TD> dest = e.dest;

	return ret;
}

template <typename T, int U, int TI, int TD>
void push_stream(T data, bool last, hls::stream< ap_axis<sizeof(T)*8,U,TI,TD>> &out_stream)
{
#pragma HLS INLINE

	ap_axis<sizeof(T)*8,U,TI,TD> e;

	assert(sizeof(T) == sizeof(int));
	union
	{
		int oval;
		T ival;
	} converter;
	converter.ival = data;
	e.data = converter.oval;

	// set it to sizeof(T) ones
	e.strb = 1; //-1;
	e.keep = 1; //15;
	e.user = 1; //0;
	e.last = last ? 1 : 0;
	//e.id = 0;
	//e.dest = 0;

	out_stream.write(e);

}

// --------------------------------------------------------------------
// function to be accelerated in HW wrapped with AXI4-Stream interface

template <typename T, int DIM, int U, int TI, int TD>
int wrapped_mmult_hw (hls::stream<ap_axis <sizeof(T)*8,U,TI,TD>> &inp_stream, hls::stream<ap_axis <sizeof(T)*8,U,TI,TD>> &out_stream)
{

#pragma HLS INLINE

	T a[DIM][DIM];
	T b[DIM][DIM];
	T out[DIM][DIM];

	assert(sizeof(T)*8 == 32);

	// stream in first matrix
	for(int i=0; i<DIM; i++)
		for(int j=0; j<DIM; j++)
		{
		#pragma HLS PIPELINE II=1
			int k = i*DIM+j;
			a[i][j] = pop_stream<T,U,TI,TD>(inp_stream);
		}

		// stream in second matrix
		for(int i=0; i<DIM; i++)
			for(int j=0; j<DIM; j++)
			{
			#pragma HLS PIPELINE II=1
				b[i][j] = pop_stream<T,U,TI,TD>(inp_stream);
			}

			// do HW multiplication
			mmult_hw<T, DIM>(a,b,out);

			// stream out result matrix
			for(int i=0; i<DIM; i++)
				for(int j=0; j<DIM; j++)
				{
				#pragma HLS PIPELINE II=1
					int k = i*DIM+j;
					bool last = (k == (DIM*DIM-1));
					push_stream<T,U,TI,TD>(out[i][j], last, out_stream);
				}
	return 0;

}
