/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*/

#ifndef H_CORDIC_DEFINES_H
#define H_CORDIC_DEFINES_H

/********************************************************************************/
// COMPILATION FLAGS: you can select only one of them alternatively

// to enable TOTALLY floating point sqrt reference function for SOLUTION 0
//#define SQRT_DOUBLE

// to enable TOTALLY floating point sqrt reference function for SOLUTION 1
//#define SQRT_REF

// to enable floating point sqrt function while the rest is integer for SOLUTION 2
//#define SQRT_FLOAT

//to enable full version of Cordic sqrt function for SOLUTIONS 3 and 4
//#define SQRT_CORDIC  // ONLY FOR SOLUTION 3
//#define  SQRT_AP_INT // FOR SOLUTION 4

/********************************************************************************/
// CONSTANTS

#define NC 1024/4

#define THRESHOLD 3.0


/********************************************************************************/
// MACROS
#define ABS(X) ( (X) > 0 ? (X) : -(X) )


/********************************************************************************/
// DATA TYPES

#include "ap_int.h"
#include "ap_fixed.h"

#ifndef SQRT_AP_INT
typedef signed short  int  teta_t;
typedef signed long   int  dinp_t;
typedef signed long   int  dout_t;
#else
typedef  ap_int<10>  teta_t;
typedef ap_int<18>   dinp_t;
typedef ap_int<24>   dout_t;
#endif

/********************************************************************************/
// EXTERN FUNCTION PROTOTYPES
dout_t process_magnitude_float(dinp_t real_data, dinp_t imag_data);
dout_t process_magnitude_int(dinp_t real_data, dinp_t imag_data);
dout_t process_magnitude_cordic(dinp_t real_data, dinp_t imag_data);
float process_magnitude_ref(float real_data, float imag_data);
double process_magnitude_double(double real_data, double imag_data);

void top_process_magnitude(dinp_t real_data[NC], dinp_t imag_data[NC], dout_t magn_data[NC]);
unsigned short int cordic_isqrt (unsigned long int x);
signed long int cordic_sqrt(signed long int x0, signed long int y0);






/********************************************************************************/
//// CHECK COMPILATION PARAMETERS
//
//#if defined(SQRT_DOUBLE)
//#undef SQRT_FLOAT
//#undef SQRT_INT
//#undef SQRT_CORDIC
//#undef SQRT_REF
//#elif defined(SQRT_REF)
//#undef SQRT_FLOAT
//#undef SQRT_INT
//#undef SQRT_CORDIC
//#undef SQRT_DOUBLE
//#elif defined(SQRT_FLOAT)
//#undef SQRT_REF
//#undef SQRT_INT
//#undef SQRT_CORDIC
//#undef SQRT_DOUBLE
//#elif defined(SQRT_INT)
//#undef SQRT_REF
//#undef SQRT_FLOAT
//#undef SQRT_CORDIC
//#undef SQRT_DOUBLE
//#elif defined(SQRT_CORDIC)
//#undef SQRT_REF
//#undef SQRT_FLOAT
//#undef SQRT_INT
//#undef SQRT_DOUBLE
//#else
//#error < NO METHOD SELECTED !>
//#endif

#endif //H_CORDIC_DEFINES_H
