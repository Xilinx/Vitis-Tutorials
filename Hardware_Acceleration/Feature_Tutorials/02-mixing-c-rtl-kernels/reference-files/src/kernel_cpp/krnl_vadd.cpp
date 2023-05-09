/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

//------------------------------------------------------------------------------
//
// kernel:  vadd
//
// Purpose: Demonstrate Vector Add in OpenCL
//

#define BUFFER_SIZE 256
extern "C" {

void krnl_vadd(
                int* a,
                int* b,
                int* c,
                const int n_elements)
{

#pragma HLS INTERFACE m_axi offset=SLAVE bundle=gmem port=a max_read_burst_length = 256
#pragma HLS INTERFACE m_axi offset=SLAVE bundle=gmem port=b max_read_burst_length = 256
#pragma HLS INTERFACE m_axi offset=SLAVE bundle=gmem1 port=c max_write_burst_length = 256

#pragma HLS INTERFACE s_axilite port=a  bundle=control
#pragma HLS INTERFACE s_axilite port=b  bundle=control
#pragma HLS INTERFACE s_axilite port=c  bundle=control

#pragma HLS INTERFACE s_axilite port=n_elements  bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

	int arrayA[BUFFER_SIZE];
	int arrayB[BUFFER_SIZE];

    for (int i = 0 ; i < n_elements ; i += BUFFER_SIZE)
    {
        int size = BUFFER_SIZE;
        //boundary check
        if (i + size > n_elements) size = n_elements - i;

        //Burst reading A and B
        readA: for (int j = 0 ; j < size ; j++) {
		#pragma HLS pipeline ii = 1 rewind
            arrayA[j] = a[i+j];
            arrayB[j] = b[i+j];
        }

        //Burst reading B and calculating C and Burst writing
        // to  Global memory
    vadd_wrteC: for (int j = 0 ; j < size ; j++){
		#pragma HLS pipeline ii = 1 rewind
            c[i+j] = arrayA[j] + arrayB[j];
        }
    }
}
}
