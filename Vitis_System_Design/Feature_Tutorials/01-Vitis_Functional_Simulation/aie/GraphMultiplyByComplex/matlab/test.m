%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

%% This graph multiplies the real part by two and the imaginary part by 3.
%  input is cint16 and output is cint16.
myGraph = vfs.aieGraph(input_file = '../src/mySub.cpp',...
                       part = 'xcvc1902-vsva2197-2MP-e-S',...
                       include_paths = "../src/");
% Inspect the I/O ports (this is optional)
myGraph.getInputSpec()
myGraph.getOutputSpec()

inr = int16(randi([-2^10, 2^10-1], 256, 1));
ini = int16(randi([-2^10, 2^10-1], 256, 1));

in = complex(inr, ini);

% Inputs and outputs to the run method are varray objects.
out_vfs = myGraph.run(varray.cint16(in));

assert(all(inr*2 == real(out_vfs)));
assert(all(ini*3 == imag(out_vfs)));

% Use the same object to run the graph again
in = conj(double(in));
out = myGraph.run(varray.cint16(in));

% Here we are converting the varray object to a numpy array using np.asarray
assert(all(real(in)*2 == real(out)));
assert(all(imag(in)*3 == imag(out)));

disp('Test Passed')
