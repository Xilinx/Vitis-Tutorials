/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#define CHANNELS 16		//number of rec. channels
#define BEAMS 3			//number of beams to form
#define SAMPLES 2500	//number of samples per PRI (rec. pulse data)

#include <stdio.h>

typedef float dio_t;

void beamformer(dio_t rx_i[SAMPLES][CHANNELS],   dio_t rx_q[SAMPLES][CHANNELS],
	dio_t w_i[BEAMS][CHANNELS],      dio_t w_q[BEAMS][CHANNELS],
	dio_t beamso_i[SAMPLES][BEAMS],  dio_t beamso_q[SAMPLES][BEAMS]);

void cmult(dio_t a, dio_t b, dio_t c, dio_t d, dio_t *R, dio_t *I);
