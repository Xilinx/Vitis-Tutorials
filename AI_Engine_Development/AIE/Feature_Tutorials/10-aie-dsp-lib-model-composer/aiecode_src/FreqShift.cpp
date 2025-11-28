/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/
 
/*
 * Frequency Shift
 *
 */

#include <adf.h>  
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>

// Definition of the 2 constants 1 and 1/sqrt(2)
#define One 16384	
#define RS2 11585
#define ACC_SHIFT 14

// Sine-Cosine values for a frequency shifting of fs/8
alignas (32) int16_t  cos_sin[16] = 
{ One, 0, RS2, RS2, 0, One, -RS2, RS2, -One, 0, -RS2, -RS2, 0, -One, RS2, -RS2 };


// FRAME_LENGTH should be a power of 2 greater than or equal to 32
template <int FRAME_LENGTH>
void FreqShift(adf::input_buffer<cint16>& __restrict in, 
               adf::output_buffer<cint16>& __restrict out)
{
    // Load sine-cosine values to this kernel
    aie::vector<cint16,8> coeffs = aie::load_v<8>((cint16*)cos_sin);
    
    // Placeholder vectors for incoming data and multiplication product
    aie::vector<cint16,8> data;
    aie::accum<cacc48,8> acc;
    
    // Pointers to 8-element vectors from input and output
    auto pi1 = aie::begin_vector_circular<8>(in);
    auto po1 = aie::begin_vector_circular<8>(out);
    
    // Loop over the input frame in 8 element chunks
    for(unsigned i = 0; i < FRAME_LENGTH/8; i++)
        chess_prepare_for_pipelining
        chess_loop_range(4,)
    {
        // Get 8 samples from input
        data = *pi1++;
        
        // Frequency shift: Element-wise multiplication with sine-cosine
        acc = aie::mul(data,coeffs);
        
        // Send 8 samples to output with bit shift
        *po1++ = srs(acc,ACC_SHIFT);
    }
}

// SSR=8 implementation: we need to interleave the input streams
template <int FRAME_LENGTH>
void FreqShift8(adf::input_buffer<cint16>& __restrict in1, 
                adf::input_buffer<cint16>& __restrict in2, 
                adf::input_buffer<cint16>& __restrict in3, 
                adf::input_buffer<cint16>& __restrict in4, 
                adf::input_buffer<cint16>& __restrict in5, 
                adf::input_buffer<cint16>& __restrict in6, 
                adf::input_buffer<cint16>& __restrict in7, 
                adf::input_buffer<cint16>& __restrict in8,
                adf::output_buffer<cint16>& __restrict out)
{
    // Load sine-cosine values to this kernel    
    aie::vector<cint16,8> coeffs = aie::load_v<8>((cint16*)cos_sin);
    
    // Placeholder vectors for incoming data and multiplication product
    aie::vector<cint16,8> data;
    aie::accum<cacc48,8> acc;
    
    // Pointers to single element from each input
    cint16 * __restrict pi1 = in1.data();
    cint16 * __restrict pi2 = in2.data();
    cint16 * __restrict pi3 = in3.data();
    cint16 * __restrict pi4 = in4.data();
    cint16 * __restrict pi5 = in5.data();
    cint16 * __restrict pi6 = in6.data();
    cint16 * __restrict pi7 = in7.data();
    cint16 * __restrict pi8 = in8.data();
    
    // Pointer to 8-element vector from output
    auto po1 = aie::begin_vector<8>(out);
    
    // Loop over the input frame in 8 element chunks
	for(unsigned i=0; i<FRAME_LENGTH/8; i++)
		chess_prepare_for_pipelining
		chess_loop_range(4,)
	{
        // Get samples from each input stream
        data[0] = *pi1++;
        data[1] = *pi2++;
        data[2] = *pi3++;
        data[3] = *pi4++;
        data[4] = *pi5++;
        data[5] = *pi6++;
        data[6] = *pi7++;
        data[7] = *pi8++;
        
        // Multiply by sinusoid to perform frequency shift
        acc = aie::mul(data,coeffs);
        
        // Write result to output
        *po1++ = acc.to_vector<cint16>(ACC_SHIFT);
	}
}