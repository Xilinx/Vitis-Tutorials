%
% Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Form a simple varray from a matlab array
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
v1 = varray.int32([1, 2, 3, 4]);
v2 = varray.bfloat16([.1,.2]);
v3 = varray.cint16([1+2j,3+4j]);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Convert a varray to a matlab array
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
vm1 = double(v3);   
vm2 = int16(v3);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Inspect the varray
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
size(v3)   % Shape of the varray
v3.ndims   % Number of dimensions
class(v3)  % Data type of the varray
length(v3) % Number of elements

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Get the byte reperesentation of the varray
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
v3.bytes  % Returns the byte representation of the varray

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Typecast the varray
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
v3
v4 = typecast(v3,'varray.uint32'); % Typecast the varray to uint32
v4
assert(all(v3.bytes == v4.bytes)); % The byte representation should be the same

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Some other supported operations
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
v5 = [v3, v3];  % Concatenate two varrays of the same type
c = isreal(v3)
