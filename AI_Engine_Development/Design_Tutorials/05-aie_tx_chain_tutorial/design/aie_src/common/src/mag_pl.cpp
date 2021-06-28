/*  (c) Copyright 2014 - 2021 Xilinx, Inc. All rights reserved.
   
    This file contains confidential and proprietary information
    of Xilinx, Inc. and is protected under U.S. and
    international copyright and other intellectual property
    laws.
   
    DISCLAIMER
    This disclaimer is not a license and does not grant any
    rights to the materials distributed herewith. Except as
    otherwise provided in a valid license issued to you by
    Xilinx, and to the maximum extent permitted by applicable
    law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
    WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
    AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
    BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
    INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
    (2) Xilinx shall not be liable (whether in contract or tort,
    including negligence, or under any other theory of
    liability) for any loss or damage of any kind or nature
    related to, arising under or in connection with these
    materials, including for any direct, or any indirect,
    special, incidental, or consequential loss or damage
    (including loss of data, profits, goodwill, or any type of
    loss or damage suffered as a result of any action brought
    by a third party) even if such damage or loss was
    reasonably foreseeable or Xilinx had been advised of the
    possibility of the same.
   
    CRITICAL APPLICATIONS
    Xilinx products are not designed or intended to be fail-
    safe, or for use in any application requiring fail-safe
    performance, such as life-support or safety devices or
    systems, Class III medical devices, nuclear facilities,
    applications related to the deployment of airbags, or any
    other applications that could lead to death, personal
    injury, or severe property or environmental damage
    (individually and collectively, "Critical
    Applications"). Customer assumes the sole risk and
    liability of any use of Xilinx products in Critical
    Applications, subject only to applicable laws and
    regulations governing limitations on product liability.
   
    THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
    PART OF THIS FILE AT ALL TIMES.                       */


#include "adf.h"

#define ITERATIONS 8	// number of CORDIC iterations
#define K_GAIN 79594			// CORDIC gain

void mag_pl(input_stream_cint16 * in, output_stream_cint16 * sig_out, output_stream_int32 * mag_out)
{
	cint16 value = readincr(in); 

	int i,abs_I,abs_Q,y;
	long X_step,Y_step,next_X_step,next_Y_step,mag;

	// >>>>>>>>>>>>>>>>> CORDIC ALGORITHM STARTS HERE >>>>>>>>>>>>>>>>>>>>>
	//% force input to be in first quadrant (RTL uses 2's complement to find
	//% arithmetic complement)
	abs_I = abs(value.real);
	abs_Q = abs(value.imag);

	//% first CORDIC rotation
	//% arrange in 1st octant (flip IQ if Q > I) and do 1st step, rotation by -45 degress
	X_step = (abs_I + abs_Q)*pow(2.0,ITERATIONS);
	Y_step = (abs_Q - abs_I)*pow(2.0,ITERATIONS);

	//% match RTL which does ITERATIONS+1 total  
	for(i=1; i<ITERATIONS+1; i++)
	{
		//% discard LSBs outside of RTL dynamic range
		next_X_step = X_step - floor(Y_step/pow(2.0,i));
		next_Y_step = Y_step + floor(X_step/pow(2.0,i));

		//% rotate by a negative angle
		//% discard LSBs outside of RTL dynamic range
		if(Y_step >= 0)
		{
			next_X_step = X_step + floor(Y_step/pow(2.0,i));
			next_Y_step = Y_step - floor(X_step/pow(2.0,i));
		}

		X_step = next_X_step;
		Y_step = next_Y_step;
	}

	//% this models the loss of the LSB in the RTL when fitting output of last
	//% CORDIC step into the 25-bit multiplier signal to match width in DSP48
	mag = X_step & 0xfffffffffffffffe;

	//% apply CORDIC gain and shift results down to match bits kept in the RTL
	//% implementation
	y = floor((double)mag*(double)K_GAIN/pow(2.0,25.0));
	// >>>>>>>>>>>>>>>>> CORDIC ALGORITHM ENDS HERE <<<<<<<<<<<<<<<<<<<<<<<<

	// 2-outputs
	writeincr(sig_out, value); // original complex input signal
	writeincr(mag_out, y); // computed magnitude result
};

