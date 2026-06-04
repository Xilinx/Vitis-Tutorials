%
% Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

DSPLIB_ROOT = getenv('DSPLIB_ROOT');

% FIR filter coefficients (32-tap symmetric lowpass)
taps = [-13, 569, 95, -427, -289, 504, 601, -495, -1052, 323, 1700, 169, ...
        -2777, -1562, 5883, 13536, 13536, 5883, -1562, -2777, 169, 1700, ...
        323, -1052, -495, 601, 504, -289, -427, 95, 569, -13];

num_taps = length(taps);
myfir = vfs.aieGraph(input_file = "../src/AI_Engine_FIR.cpp",...
                     include_paths = {"../src",...
                                fullfile(DSPLIB_ROOT, 'L2', 'include', 'aie'),...
                                fullfile(DSPLIB_ROOT, 'L1', 'include', 'aie'), ...
                                fullfile(DSPLIB_ROOT, 'L1', 'src', 'aie')});

zi = zeros(num_taps - 1, 1);  
  
in_16bit_r = randi([-2^12, 2^12-1], [2^10, 1], 'int16');  
in_16bit_i = randi([-2^12, 2^12-1], [2^10, 1], 'int16');  

in_c16 = complex(in_16bit_r, in_16bit_i);  

out_c16 = myfir.run(varray.cint16(in_c16));  

[out_c16_r, zi] = filter(taps, 1.0, double(in_c16), zi);  

delta = real(double(out_c16)) - double(real(int16(out_c16_r)));
absDelta = abs(delta);
assert(all(absDelta/(2^16) < 1.42));  

fprintf('Test Passed\n');
