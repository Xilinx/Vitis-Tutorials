%
% Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

%% Create input signal

Ntransform = 4;
Nfft = 65536;
backoff_dB = -15;
TT = numerictype(1,32,31);
FF = fimath('RoundingMethod','Convergent','OverflowAction','Saturate');

% Random noise:
tmp = sqrt(0.5)*10^(0.05*backoff_dB)*complex(randn(1,Nfft*Ntransform),randn(1,Nfft*Ntransform));
scale = 0.5^12;
sig_i = fi(reshape(scale*tmp,Nfft,Ntransform),TT,FF);

%% 
% Initialize AIE Graphs and HLS Kernel objects
ifft_dma_src = vfs.hlsKernel(part = 'xcvc1902-vsva2197-2MP-e-S',...
                              hls_function = 'ifft_dma_src_wrapper',...
                              input_files = {'../hls_src/ifft_dma_src.cpp',...
                                             '../hls_src/ifft_dma_src.h'});

front_ifft_with_twid = vfs.aieGraph(input_file = '../aie_src/front_ifft_with_twid/front_ifft_with_twid.cpp',...
                                    part = 'xcvc1902-vsva2197-2MP-e-S',...
                                    include_paths = {'../aie_src/front_ifft_with_twid'});

ifft_transpose = vfs.hlsKernel(part = 'xcvc1902-vsva2197-2MP-e-S',...
                              hls_function = 'ifft_transpose_wrapper',...
                              input_files = {'../hls_src/ifft_transpose.cpp',...
                                            '../hls_src/ifft_transpose.h'});

back_ifft = vfs.aieGraph(input_file = '../aie_src/back_ifft/back_ifft.cpp',...
                                    part = 'xcvc1902-vsva2197-2MP-e-S',...
                                    include_paths = {'../aie_src/back_ifft/'});

ifft_dma_snk = vfs.hlsKernel(part = 'xcvc1902-vsva2197-2MP-e-S',...
                              hls_function = 'ifft_dma_snk_wrapper',...
                              input_files = {'../hls_src/ifft_dma_snk.cpp',...
                                             '../hls_src/ifft_dma_snk.h'});
%% MATLAB Model

sig_g = Nfft*ifft(double(sig_i));
sig_g = fi(sig_g,TT,FF);
sig_g = double(sig_g.int);

%% Implementation model

% Cast input data to varray
sig_i_vfs = varray.cint32(reshape(sig_i.int,[],1));

% Process data through the computational pipeline
ifft_dma_src_out = ifft_dma_src.run({sig_i_vfs});              % HLS Kernel
front_ifft_out = front_ifft_with_twid.run(ifft_dma_src_out);   % AIE Graph
ifft_transpose_out = ifft_transpose.run(front_ifft_out);       % HLS Kernel
back_ifft_out = back_ifft.run(ifft_transpose_out);             % AIE Graph
ifft_dma_snk_out = ifft_dma_snk.run(back_ifft_out);            % HLS Kernel

% Cast output data from uint128 back to cint32 then to double
ifft_vfs_out = typecast(ifft_dma_snk_out{1},'varray.cint32');
ifft_vfs_out = double(reshape(ifft_vfs_out,Nfft,[]));

%% Compare Golden Reference against Implementation
% Note: one transform delay in C model compared to MATLAB model due to nature of implementation
max_error_real = max(abs(real(sig_g(:,end-1)-ifft_vfs_out(:,end))));
max_error_imag = max(abs(imag(sig_g(:,end-1)-ifft_vfs_out(:,end))));
max_allowed_err = 2^16;
if ( max(max_error_real,max_error_imag) < max_allowed_err )
    disp('Test Passed');
else
    disp('Test Failed');
end