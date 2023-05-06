/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*/

#include "cordic_defines.h"



#define ITER    16


#ifdef SQRT_INT

unsigned short int cordic_isqrt (unsigned long int x)
{

#pragma	HLS INLINE
	unsigned char i;
	unsigned short int y;
	unsigned long int m;
	unsigned short int base = (1<<(ITER-1))&0xFFFF;

       y = 0 ;
 L0:      for (i = 1; i <= ITER; i++)
       {
#pragma	HLS PIPELINE
               y +=  base ;
               m = (unsigned long int) y * (unsigned long int) y;
               if  (m > x )
               {
                       y -= base ;  // base should not have been added, so we substract again
               }
               base = base >> 1 ;      // shift 1 digit to the right = divide by 2
        }
        return y ;
}



dout_t process_magnitude_int(dinp_t real_data, dinp_t imag_data)
{
#pragma	HLS INLINE OFF
	typedef signed long int  acc_t;
	dinp_t mag_data;
	acc_t accu_plus, temp_datar, temp_datai;

	acc_t temp_long;

	temp_datar = (acc_t)real_data*(acc_t)real_data;
	temp_datai = (acc_t)imag_data*(acc_t)imag_data;
	accu_plus = temp_datar + temp_datai;

	mag_data = cordic_isqrt((unsigned long int) accu_plus);

	return mag_data;
}

#endif
