/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/



#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>


extern "C" {
	#define BUS_SIZE 
	#define DATA_SIZE 
	#define ARRAY_SIZE 

	#define RATIO (BUS_SIZE/DATA_SIZE)


void FUNCTION_NAME (hls::stream<ap_axis<BUS_SIZE, 0, 0, 0>  >& s, int size) {

		#pragma HLS interface axis port=s

		#pragma HLS INTERFACE s_axilite port=size bundle=control
		#pragma HLS interface s_axilite port=return bundle=control
		ap_int<DATA_SIZE> d[RATIO];
		#pragma HLS ARRAY_RESHAPE variable=d type=complete dim=1
		ap_int<BUS_SIZE> v[ARRAY_SIZE];

		// Generator Table initialization
		INIT_D:for(int i=0; i<RATIO;i++)
			d[i] =  i;

		INIT_V:for(int i=0;i<ARRAY_SIZE;i++)
		{
			#pragma HLS PIPELINE II=1
			// View d as a single int of size PLIO_SIZE --> D
			if constexpr (RATIO==1)
				v[i] = d[0];
			else if constexpr (RATIO==2)
				v[i] = (d[0],d[1]);
			else if constexpr (RATIO==4)
				v[i] = (d[0],d[1],d[2],d[3]);
			else if constexpr (RATIO==8)
				v[i] = (d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7]);

			for(int j=0;j<RATIO;j++)
				d[j] += RATIO;
		}

		// Send Data
		SEND_DATA:for(int i = 0; i < size; i++) {
			#pragma HLS PIPELINE II=1
			ap_axis<BUS_SIZE, 0, 0, 0> x;
			x.data = v[i%ARRAY_SIZE];
			s.write(x);
		}
	}
}
