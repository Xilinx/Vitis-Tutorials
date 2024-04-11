/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include <stdio.h>
#include <math.h>
#include "beamformer.h"


void beamformer(dio_t rx_i[SAMPLES][CHANNELS],   dio_t rx_q[SAMPLES][CHANNELS],
	dio_t w_i[BEAMS][CHANNELS],      dio_t w_q[BEAMS][CHANNELS],
	dio_t beamso_i[SAMPLES][BEAMS],  dio_t beamso_q[SAMPLES][BEAMS])
{

#pragma HLS INTERFACE mode=ap_memory port=beamso_q storage_type=ram_1p
#pragma HLS INTERFACE mode=ap_memory port=beamso_i storage_type=ram_1p
#pragma HLS INTERFACE mode=ap_memory port=w_q storage_type=ram_1p
#pragma HLS INTERFACE mode=ap_memory port=w_i storage_type=ram_1p
#pragma HLS INTERFACE mode=ap_memory port=rx_q storage_type=ram_1p
#pragma HLS INTERFACE mode=ap_memory port=rx_i storage_type=ram_1p

	int 	i, j, k;
	dio_t a,b,c,d,R,I;

	dio_t ti[CHANNELS], tq[CHANNELS], si, sq;

/////////////////////////////////
//create the beams
////////////////////////////////
	L1:for (i=0; i<SAMPLES; i++) {
#pragma HLS LOOP_FLATTEN off
		
		L2: for (j=0; j<BEAMS; j++) {
#pragma HLS LOOP_FLATTEN off
			si=0;
			sq=0;

			L3: for (k=0; k<CHANNELS; k++) {
#pragma HLS LOOP_FLATTEN off
				cmult(rx_i[i][k],rx_q[i][k],w_i[j][k],w_q[j][k],&ti[k],&tq[k]);
				si = si + ti[k];
				sq = sq + tq[k];
			} 
			beamso_i[i][j] = si;
			beamso_q[i][j] = sq;
		}
	}
	
}


void cmult(dio_t a, dio_t b, dio_t c, dio_t d, dio_t *R, dio_t *I)
{
//dio_t k1,k2,k3;
//k1 = a*(c + d); k2 = d*(a + b);  k3 = c*(b - a);
//	*R = k1 - k2;
//	*I = k1 + k3;
	*R = a*c - b*d;
	*I = a*d + b*c;
}
