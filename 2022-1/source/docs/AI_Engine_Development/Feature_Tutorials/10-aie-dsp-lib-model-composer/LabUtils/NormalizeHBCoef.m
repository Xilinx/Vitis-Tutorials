% Copyright 2021-2022 Xilinx, Inc.
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

function [IntCoef] = NormalizeHBCoef(Cbase, Norm)

IntCoef = Cbase;
l = length(Cbase);

p = Cbase(1:2:l);
pint = NormalizeCoef(p,Norm);
IntCoef(1:2:l) = pint;
IntCoef((l+1)/2) = Norm;
