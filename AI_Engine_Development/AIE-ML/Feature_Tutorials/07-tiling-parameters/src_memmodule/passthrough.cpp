// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "kernels.h"


template<int FrameLength,int VectorLength>
void passthrough_buffer(adf::input_buffer<int32, adf::extents<FrameLength>> &__restrict in, adf::output_buffer<int32, adf::extents<FrameLength>> &__restrict out)
{
    int32 i;
    int32 FrameLengthOut = out.size();

    auto pi1 = aie::begin_vector<VectorLength>(in);
    auto po1 = aie::begin_vector<VectorLength>(out);

    // Copy input to output
    for (i = 0; i < FrameLengthOut / VectorLength; i++)
        chess_prepare_for_pipelining
        {
            *po1++ = *pi1++;
        }
}

template <int FrameLength, int VectorLength>
void passthrough_buffer2(adf::input_buffer<int32, adf::extents<FrameLength>> &__restrict in, adf::output_buffer<int32, adf::extents<FrameLength>> &__restrict out1, adf::output_buffer<int32, adf::extents<FrameLength>> &__restrict out2)
{
    int32 i;
    int32 FrameLengthOut = out1.size();

    auto pi1 = aie::begin_vector<VectorLength>(in);
    auto po1 = aie::begin_vector<VectorLength>(out1);
    auto po2 = aie::begin_vector<VectorLength>(out2);

    // Copy input to output
    for (i = 0; i < FrameLengthOut / VectorLength; i++)
        chess_prepare_for_pipelining
        {
            auto v = *pi1++;
            *po1++ = v;
            *po2++ = v;
        }
}

template <int FrameLengthIn,int FrameLengthOut, int VectorLength>
void passthrough_buffer_block(adf::input_buffer<int32, adf::extents<FrameLengthIn>> &__restrict in, adf::output_buffer<int32, adf::extents<FrameLengthOut>> &__restrict out1)
{
    auto po1 = aie::begin_vector<VectorLength>(out1);

    // Create a tensor descriptor:
    //        - Dimension 0: 2 elements (vectors of size VectorLength) with a step of 4 vectors in between each
    //        - Dimension 1: 1 element (actually no limit on last dimension) with a step of 1 ion between each
    
    auto desc = aie::make_tensor_descriptor<int,VectorLength>(aie::tensor_dim(1u,1),aie::tensor_dim(2u,4)); 
    
    //Create a tensor buffer stream where buffer 'in' is associated with the tensor descriptor
    auto tbs = aie::make_tensor_buffer_stream((int *)in.data(), desc);

    aie::vector<int, VectorLength> v;
    
    // read from the tensor descriptor and write on the output buffer
    for(int i=0;i<FrameLengthOut/VectorLength;i++)
    {
        tbs >> v;     
        *po1++ = v;
    }
}

template <int FrameLengthIn, int FrameLengthOut, int VectorLength>
void passthrough_buffer_block_4d(adf::input_buffer<int32, adf::extents<FrameLengthIn>> &__restrict in, adf::output_buffer<int32, adf::extents<FrameLengthOut>> &__restrict out1)
{
    auto po1 = aie::begin_vector<VectorLength>(out1);

    // Create a tensor descriptor:
    //        Outer tensor
    //        - Dimension 3: 2 elements with a step of 2 (Offset 2 --> Samples 0 16)
    //        - Dimension 2: 2 elements with step of 0 --> Repetition of the same scheme (Dim 0-1)
    //        - Dimension 1: 4 elements with step of 4 ( Offset 0 4 8 12 --> Samples 0 32 64 96)
    //        Inner tensor
    //        - Dimension 0: 2 elements with step of 1 ( Offset 0 1 --> Samples 0 8)
    //
    // Total iterations: 32
    auto desc = aie::make_tensor_descriptor<int32, VectorLength>(
        aie::tensor_dim(2u, 2),
        aie::tensor_dim(2u, 0),
        aie::tensor_dim(4u, 4),
        aie::tensor_dim(2u, 1));

    // Create a tensor buffer stream where buffer 'in' is associated with the tensor descriptor
    auto tbs = aie::make_tensor_buffer_stream((int *)in.data(), desc);

    aie::vector<int, VectorLength> v;

    for(int outer=0;outer<2*4*2;outer++) // 3 outer dimensions
    {
        auto in_tbs = tbs.pop();
        for(int inner=0;inner<2;inner++)  // 1 inner dimension
        {
            in_tbs >> v;
            *po1++ = v;
        }
    }
}

