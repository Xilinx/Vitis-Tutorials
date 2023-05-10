% MIT License
%
% Copyright (C) 2023 Advanced Micro Devices, Inc. All rights reserved.
%
% Permission is hereby granted, free of charge, to any person obtaining a copy
% of this software and associated documentation files (the "Software"), to deal
% in the Software without restriction, including without limitation the rights
% to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
% copies of the Software, and to permit persons to whom the Software is
% furnished to do so, subject to the following conditions:
%
% The above copyright notice and this permission notice shall be included in all
% copies or substantial portions of the Software.
%
% THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
% IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
% FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
% AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
% LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
% OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
% SOFTWARE.


% floating-point reference
A1 = reshape(1:128,16,8);
A2 = A+10;
SaveFloat([A1;A2],'inputa_float.txt');

B1 = reshape(64:-1:1,8,8);
B2 = B1-10;
SaveFloat([B1;B2],'inputb_float.txt');


C1 = A1*B1;
C2 = A2*B2;
SaveFloat([C1;C2],'ref_outputc_float.txt');

% complex floating-point reference

A1 = reshape(1:64,16,4)+i*(reshape(64:-1:1,16,4)+10);
A2 = (100+i*100)-A1;
SaveCFloat([A1;A2],'inputa_cfloat.txt');

B1 = (reshape(1:16,4,4)+10) + i*(reshape(16:-1:1,4,4)-10);
B2 = 10-B1;
SaveCFloat([B1;B2],'inputb_cfloat.txt');

C1 = A1*B1;
C2 = A2*B2;
SaveCFloat([C1;C2],'ref_outputc_cfloat.txt');

Cc1 = A1*conj(B1);
Cc2 = A2*conj(B2);
SaveCFloat([Cc1;Cc2],'ref_outputcc_cfloat.txt');
