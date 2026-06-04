%
% Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

%%
myHLSKernel = vfs.hlsKernel(input_files = "../src/kernels.cpp",...
                            part = 'xcvc1902-vsva2197-2MP-e-S',...
                            hls_function = "invert7_13");
N = 10;

in7 = randi([0,2^7-1],N,1);
in13 = randi([0,2^13-1],N,1);

%fixed point inputs
input7 = varray.fi(in7,0,7,0);
input13 = varray.fi(in13,0,13,0);
[output7,output13] = myHLSKernel.run(input7,input13);

% Now we use bitcmp (bit compliment) command in MATLAB to create a refernce output.
% We also mask the bits that are not relevant and then compare the referene to the output.
outref7 = bitcmp(uint8(in7));
outref7 = bitand(outref7, 2^7-1); % mask the last bit

assert(all(outref7 == uint8(output7)));

outref13 = bitcmp(uint16(in13));
outref13 = bitand(outref13, 2^13-1); % mask the last three bits

assert(all(outref13 == uint16(output13)));
disp('Test Passed')