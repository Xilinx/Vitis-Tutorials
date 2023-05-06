/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*/

#include <stdio.h>
#include <math.h>
#include "cordic_defines.h"



void func1_process_magnitude(dinp_t real_data[NC], dinp_t imag_data[NC], dout_t magn_data[NC])
{

	unsigned short int i;

	L1:for (i=0; i<NC/4; i++)
	{

#if  defined(SQRT_DOUBLE) 	// SOLUTION 0
		magn_data[i] = process_magnitude_double(    real_data[i],  imag_data[i]);
#elif  defined(SQRT_REF)	// SOLUTION 1
		magn_data[i] = process_magnitude_ref(    real_data[i],  imag_data[i]);
#elif  defined(SQRT_FLOAT)	// SOLUTION 2
		magn_data[i] = process_magnitude_float(  real_data[i],  imag_data[i]);
#elif defined(SQRT_CORDIC)	// SOLUTION 3
		magn_data[i] = process_magnitude_cordic( real_data[i],  imag_data[i]);
//#elif defined(SQRT_INT)		// SOLUTION 4
//		magn_data[i] = process_magnitude_int(    real_data[i],  imag_data[i]);
#endif
	}

}



void func2_process_magnitude(dinp_t real_data[NC], dinp_t imag_data[NC], dout_t magn_data[NC])
{

	unsigned short int i;

	L2: for (i=NC/4; i<NC/2; i++)
	{
#if  defined(SQRT_DOUBLE) 	// SOLUTION 0
		magn_data[i] = process_magnitude_double(    real_data[i],  imag_data[i]);
#elif  defined(SQRT_REF)	// SOLUTION 1
		magn_data[i] = process_magnitude_ref(    real_data[i],  imag_data[i]);
#elif  defined(SQRT_FLOAT)	// SOLUTION 2
		magn_data[i] = process_magnitude_float(  real_data[i],  imag_data[i]);
#elif defined(SQRT_CORDIC)	// SOLUTION 3
		magn_data[i] = process_magnitude_cordic( real_data[i],  imag_data[i]);
//#elif defined(SQRT_INT)		// SOLUTION 4
//		magn_data[i] = process_magnitude_int(    real_data[i],  imag_data[i]);
#endif
		}

}


void func3_process_magnitude(dinp_t real_data[NC], dinp_t imag_data[NC], dout_t magn_data[NC])
{

	unsigned short int i;

	L3: for (i=NC/2; i<3*NC/4; i++)
	{
#if  defined(SQRT_DOUBLE) 	// SOLUTION 0
		magn_data[i] = process_magnitude_double(    real_data[i],  imag_data[i]);
#elif  defined(SQRT_REF)	// SOLUTION 1
		magn_data[i] = process_magnitude_ref(    real_data[i],  imag_data[i]);
#elif  defined(SQRT_FLOAT)	// SOLUTION 2
		magn_data[i] = process_magnitude_float(  real_data[i],  imag_data[i]);
#elif defined(SQRT_CORDIC)	// SOLUTION 3
		magn_data[i] = process_magnitude_cordic( real_data[i],  imag_data[i]);
//#elif defined(SQRT_INT)		// SOLUTION 4
//		magn_data[i] = process_magnitude_int(    real_data[i],  imag_data[i]);
#endif
		}

}


void func4_process_magnitude(dinp_t real_data[NC], dinp_t imag_data[NC], dout_t magn_data[NC])
{

	unsigned short int i;

	L4: for (i=3*NC/4; i<NC; i++)
	{
#if  defined(SQRT_DOUBLE) 	// SOLUTION 0
		magn_data[i] = process_magnitude_double(    real_data[i],  imag_data[i]);
#elif  defined(SQRT_REF)	// SOLUTION 1
		magn_data[i] = process_magnitude_ref(    real_data[i],  imag_data[i]);
#elif  defined(SQRT_FLOAT)	// SOLUTION 2
		magn_data[i] = process_magnitude_float(  real_data[i],  imag_data[i]);
#elif defined(SQRT_CORDIC)	// SOLUTION 3
		magn_data[i] = process_magnitude_cordic( real_data[i],  imag_data[i]);
//#elif defined(SQRT_INT)		// SOLUTION 4
//		magn_data[i] = process_magnitude_int(    real_data[i],  imag_data[i]);
#endif
		}

}


void top_process_magnitude(dinp_t real_data[NC], dinp_t imag_data[NC], dout_t magn_data[NC])
{

	unsigned short int i;

#if  defined(SQRT_DOUBLE)  	  // SOLUTION 0
	printf("now running 64-bit floating point method\n");
#elif  defined(SQRT_REF)	  // SOLUTION 1
	printf("now running 32-bit floating point method\n");
#elif  defined(SQRT_FLOAT)	  // SOLUTION 2
	printf("now running 32-bit floating point sqrt on 32-bit integers\n");
//#elif defined(SQRT_INT)
//	printf("now running int cordic approssimation method\n");
#elif ( defined(SQRT_CORDIC) & (!defined(SQRT_AP_INT)) )	  // SOLUTION 3
	printf("now running full cordic 32-bit approssimation method\n");
#elif ( defined(SQRT_CORDIC) & defined(SQRT_AP_INT) )	  // SOLUTION 4
	printf("now running full cordic 18-bit approssimation method\n");
#else
#error < YOU MUST SELECT A SQRT METHOD IN COMPILATION !>
#endif

 func1_process_magnitude( real_data, imag_data, magn_data);
 func2_process_magnitude( real_data, imag_data, magn_data);
 func3_process_magnitude( real_data, imag_data, magn_data);
 func4_process_magnitude( real_data, imag_data, magn_data);

}
