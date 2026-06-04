%
% Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

% Example: Multiply with Preprocessor Definition
%
% This example demonstrates the use of the Xpreproc parameter to pass
% preprocessor definitions to the AIE compiler. The kernel multiplies
% input values by a factor that can be configured at compile-time using
% the -DMULTIPLY_FACTOR=<value> preprocessor definition.

%% Example 1: Default behavior (multiply by 2)
fprintf('Example 1: Default behavior (MULTIPLY_FACTOR=2)\n');
myGraph = vfs.aieGraph(input_file="../src/mySub.cpp", ...
                       include_paths="../src/");

in_data = varray.int32([1, 2, 3, 4, 5]);
out_data = myGraph.run(in_data);
ref = [2, 4, 6, 8, 10].';
assert(isequal(double(out_data), ref), 'Example 1 failed');
fprintf('Output: [%s] (matches reference)\n\n', num2str(double(out_data).'));

%% Example 2: Using Xpreproc with MULTIPLY_FACTOR=3
fprintf('Example 2: Using Xpreproc with MULTIPLY_FACTOR=3\n');
myGraph2 = vfs.aieGraph(input_file="../src/mySub.cpp", ...
                        include_paths="../src/", ...
                        Xpreproc="-DMULTIPLY_FACTOR=3");

in_data2 = varray.int32([1, 2, 3, 4, 5]);
out_data2 = myGraph2.run(in_data2);
ref2 = [3, 6, 9, 12, 15].';
assert(isequal(double(out_data2), ref2), 'Example 2 failed');
fprintf('Output: [%s] (matches reference)\n\n', num2str(double(out_data2).'));

%% Example 3: Using Xpreproc with MULTIPLY_FACTOR=5
fprintf('Example 3: Using Xpreproc with MULTIPLY_FACTOR=5\n');
myGraph3 = vfs.aieGraph(input_file="../src/mySub.cpp", ...
                        include_paths="../src/", ...
                        Xpreproc="-DMULTIPLY_FACTOR=5");

in_data3 = varray.int32([1, 2, 3, 4, 5]);
out_data3 = myGraph3.run(in_data3);
ref3 = [5, 10, 15, 20, 25].';
assert(isequal(double(out_data3), ref3), 'Example 3 failed');
fprintf('Output: [%s] (matches reference)\n\n', num2str(double(out_data3).'));

%% Example 4: Multiple preprocessor definitions
fprintf('Example 4: Multiple preprocessor definitions\n');
myGraph4 = vfs.aieGraph(input_file="../src/mySub.cpp", ...
                        include_paths="../src/", ...
                        Xpreproc={"-DMULTIPLY_FACTOR=10", "-DENABLE_DEBUG"});

in_data4 = varray.int32([1, 2, 3]);
out_data4 = myGraph4.run(in_data4);
ref4 = [10, 20, 30].';
assert(isequal(double(out_data4), ref4), 'Example 4 failed');
fprintf('Output: [%s] (matches reference)\n\n', num2str(double(out_data4).'));

fprintf('Test Passed\n');
