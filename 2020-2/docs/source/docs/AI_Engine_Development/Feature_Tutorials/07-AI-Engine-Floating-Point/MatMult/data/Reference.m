%
% Copyright 2020 Xilinx, Inc.
%
% Licensed under the Apache License, Version 2.0 (the "License");
% you may not use this file except in compliance with the License.
% You may obtain a copy of the License at
%
%     http://www.apache.org/licenses/LICENSE-2.0
%
% Unless required by applicable law or agreed to in writing, software
% distributed under the License is distributed on an "AS IS" BASIS,
% WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
% See the License for the specific language governing permissions and
% limitations under the License.
%

%floating-point reference
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
