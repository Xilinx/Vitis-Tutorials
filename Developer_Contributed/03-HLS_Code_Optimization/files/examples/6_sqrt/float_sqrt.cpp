/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*/

#include <stdio.h>
#include <math.h>
#include "cordic_defines.h"



#ifdef SQRT_FLOAT

typedef long long int acc_t;

dout_t process_magnitude_float(dinp_t real_data, dinp_t imag_data)
{
#pragma	HLS INLINE OFF
#pragma	HLS PIPELINE

	dinp_t mag_data;
	acc_t accu_plus, temp_datar, temp_datai;

	temp_datar = (acc_t)real_data * (acc_t)real_data;


	temp_datai = (acc_t)imag_data * (acc_t)imag_data;

	accu_plus = temp_datar + temp_datai;

	mag_data = (dout_t) floor(sqrtf( (float)accu_plus ));

	return mag_data;
}
#endif // SQRT_FLOAT


#ifdef SQRT_DOUBLE
double process_magnitude_double(double real_data, double imag_data)
{
#pragma	HLS INLINE OFF
#pragma	HLS PIPELINE

	double mag_data, accu_plus, temp_datar, temp_datai;

	temp_datar = real_data * real_data;
	temp_datai = imag_data * imag_data;
	accu_plus = temp_datar + temp_datai;

	mag_data =       sqrt(accu_plus);

	return mag_data;
}
#endif //SQRT_DOUBLE


float process_magnitude_ref(float real_data, float imag_data)
{
#pragma	HLS INLINE OFF
#pragma	HLS PIPELINE

	float mag_data, accu_plus, temp_datar, temp_datai;

	temp_datar = real_data * real_data;
	temp_datai = imag_data * imag_data;
	accu_plus = temp_datar + temp_datai;

	mag_data =       sqrtf(accu_plus);

	return mag_data;
}
