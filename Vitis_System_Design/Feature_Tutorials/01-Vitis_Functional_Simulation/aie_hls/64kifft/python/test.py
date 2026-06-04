#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El Shabani

import vfs
import varray as va
import numpy as np

# Create input signal
Ntransform = 4
Nfft = 65536

# Generate random complex input
sig_i = (np.random.randint(-2**17, 2**17, size=(Nfft,Ntransform)).astype(np.int32) + 
                  1j * np.random.randint(-2**17, 2**17, size=(Nfft,Ntransform)).astype(np.int32))

# Initialize AIE Graphs and HLS Kernel objects
ifft_dma_src = vfs.hlsKernel(part = 'xcvc1902-vsva2197-2MP-e-S',
                             hls_function = 'ifft_dma_src_wrapper',
                             input_files = ['../hls_src/ifft_dma_src.cpp',
                                             '../hls_src/ifft_dma_src.h'])

front_ifft_with_twid = vfs.aieGraph(input_file = '../aie_src/front_ifft_with_twid/front_ifft_with_twid.cpp',
                                    part = 'xcvc1902-vsva2197-2MP-e-S',
                                    include_paths = ['../aie_src/front_ifft_with_twid'])

ifft_transpose = vfs.hlsKernel(part = 'xcvc1902-vsva2197-2MP-e-S',
                               hls_function = 'ifft_transpose_wrapper',
                               input_files = ['../hls_src/ifft_transpose.cpp',
                                            '../hls_src/ifft_transpose.h'])

back_ifft = vfs.aieGraph(input_file = '../aie_src/back_ifft/back_ifft.cpp',
                         part = 'xcvc1902-vsva2197-2MP-e-S',
                         include_paths = ['../aie_src/back_ifft/'])

ifft_dma_snk = vfs.hlsKernel(part = 'xcvc1902-vsva2197-2MP-e-S',
                             hls_function = 'ifft_dma_snk_wrapper',
                             input_files = ['../hls_src/ifft_dma_snk.cpp',
                                             '../hls_src/ifft_dma_snk.h'])

# Python Model
sig_g = np.zeros((Nfft,Ntransform),dtype=np.int32)+ 1j * np.zeros((Nfft,Ntransform),dtype=np.int32)
for i in range(Ntransform):
    sig_g[:,i] = Nfft*np.fft.ifft(sig_i[:,i])
sig_g = np.array(sig_g.real,dtype=np.int32) + 1j * np.array(sig_g.imag,dtype=np.int32)

# Implementation model

# Cast input data to varray
sig_i_vfs = va.array(sig_i.reshape(-1,order='F'),va.cint32)

# Process data through the computational pipeline
ifft_dma_src_out = ifft_dma_src.run(sig_i_vfs);                # HLS Kernel
front_ifft_out = front_ifft_with_twid.run(ifft_dma_src_out);   # AIE Graph
ifft_transpose_out = ifft_transpose.run(front_ifft_out);       # HLS Kernel
back_ifft_out = back_ifft.run(ifft_transpose_out);             # AIE Graph
ifft_dma_snk_out = ifft_dma_snk.run(back_ifft_out);            # HLS Kernel

# Cast output data from uint128 back to cint32 then to np.array
ifft_vfs_out = ifft_dma_snk_out.view(va.cint32)
ifft_vfs_out = np.array(ifft_vfs_out,dtype=complex)
ifft_vfs_out = ifft_vfs_out.reshape((Nfft,-1),order='F')

# Compare Golden Reference against Implementation
# Note: one transform delay in C model compared to MATLAB model due to nature of implementation
max_error_real = np.max(np.abs((sig_g[:,-2]-ifft_vfs_out[:,-1]).real))
max_error_imag = np.max(np.abs((sig_g[:,-2]-ifft_vfs_out[:,-1]).imag))
max_allowed_err = 2**16
if ( max(max_error_real,max_error_imag) < max_allowed_err ):
    print('Test Passed')
else:
    print('Test Failed')

