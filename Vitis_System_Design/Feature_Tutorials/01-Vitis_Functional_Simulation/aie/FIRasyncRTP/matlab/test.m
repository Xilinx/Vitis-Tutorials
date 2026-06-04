%
% Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

%% Initialize the AIE graph
myaiefir = vfs.aieGraph(input_file = "../src/AIE_subsystem.cpp", ...
    part = 'xcvc1902-vsva2197-2MP-e-S',...
    include_paths = {"../src", ...
    sprintf('%s/L2/include/aie/',getenv("DSPLIB_ROOT")),...
    sprintf('%s/L1/include/aie/',getenv("DSPLIB_ROOT")), ...
    sprintf('%s/L1/src/aie/',getenv("DSPLIB_ROOT"))});

b = [180,89,-80,-391,-720,-834,-478,505,2063,3896,5535,6504];

frameSize = 256;
numOfFrames = 40;
scale = 2^9;

%% Set filter in MATLAB
Hd = dfilt.dfsymfir([b fliplr(b)]);
Hd.PersistentMemory = true;
Hd.States = zeros(2*length(b)-1,1);

%% Set outputs
out_ref = zeros(frameSize*numOfFrames,1);
out_aie = zeros(size(out_ref));

rtp = varray.int32(b);

for i=1:numOfFrames
    input = int16(complex(randi([-2^6,2^6-1],[frameSize,1]),randi([-2^6,2^6-1],[frameSize,1])));
     
    y_ref = filter(Hd,input);
    out_ref((i-1)*frameSize+1:i*frameSize) = y_ref/scale;

    y_aie = myaiefir.run(varray.cint16(input), rtp);
    out_aie((i-1)*frameSize+1:i*frameSize) = y_aie;

    % Optional, since this is an asynchronous rtp, we can set the 
    % rtp array to an empty array for subsequent runs
    rtp = varray.int32([]);
end

%% Error checking
error = out_ref-out_aie;
max_error = max(max(real(error)),max(imag(error)));
if (max_error<1)
    disp('Test Passed')
else
    disp('Test Failed')
end
%% Plot outputs
subplot(3,1,1); plot(real(out_ref(1:30))); title("MATLAB");
subplot(3,1,2); plot(real(out_aie(1:30)),'r'); title("AIEngine");
subplot(3,1,3); plot(real(out_ref-out_aie)); title("Error real-part");
