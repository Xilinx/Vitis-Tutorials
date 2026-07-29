%
% Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Richard Buz
%
% HIL host script for the M=16, SSR=8, P/Q=8/7 channelizer.
% Streams wideband signal samples to the hardware channelizer via HIL,
% collects the channelized outputs, and displays spectra.
%

clearvars; clear getWbSmp; clc

% Configure host code.
nIter = 1024;     % specifies how many times HIL run function is called
N = 16384;        % sets block size for HIL processing
tictocsum = 0.0;  % total HIL processing time
totalsamp = 0;    % total output samples
stats_en = false; % delays accumulating stats until output is available

% Instantiate HIL object.
hil_dir = '../channelizer_hil';
hil_server_ip = '10.23.196.92';   % modify for your IP
hil_chnlzr = hil('hil_dir', hil_dir);
hil_chnlzr.setConnection('ip_addr', hil_server_ip);

% Configure HIL object.
chanFrameSize = 4*N;  % adjust frame size and timeout to optimize HIL for your network
timeout = 0.01;
for prt_idx = 1:7
    hil_chnlzr.setInputFrameSize(prt_idx, chanFrameSize);
end
for prt_idx = 1:8
    hil_chnlzr.setOutputFrameSize(prt_idx, chanFrameSize);
end
inputSpec = hil_chnlzr.getInputSpec()
outputSpec = hil_chnlzr.getOutputSpec()
hil_chnlzr.setTimeout(timeout);
hil_chnlzr.initialize();

% Create a FIFO to align HIL output.
ssro = 8;
fifo_max = 5*chanFrameSize;
fifo_buffer = zeros(fifo_max,ssro);
fifo_ptr = ones(1,ssro);
fifo_size = zeros(1,ssro);

% Configure wideband signal source.
Fs = 8.75e9;
upsmp = 28;
chan_en = [0 1 1 0 0 1 1 0 0 0 0 0 0 0 0 0];  % enable individual sub-channels
qam_en = [0 0 0 0 0 1 1 0 0 0 0 0 0 0 0 0];   % modulate selected sub-channels
swp_en = [0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0];   % enable carrier sweep on sub-channels
swp_rate = [0 0.02 0 0 0 0 0 0 0 0 0 0 0 0 0 0]; % set carrier sweep rate: range [-1.0, +1.0]

% Setup spectrum displays.
scope_wb = spectrumAnalyzer(Name="Wideband Signal Input",SampleRate=Fs,LineColor="c",YLimits=[-50 20]);
disp_slct = [1 2 5 6];
scope_chA = spectrumAnalyzer(Name=sprintf("Channel %d Output",disp_slct(1)),SampleRate=(8/7)*Fs/16,LineColor="y",YLimits=[-60 20]);
scope_chB = spectrumAnalyzer(Name=sprintf("Channel %d Output",disp_slct(2)),SampleRate=(8/7)*Fs/16,LineColor="y",YLimits=[-60 20]);
scope_chC = spectrumAnalyzer(Name=sprintf("Channel %d Output",disp_slct(3)),SampleRate=(8/7)*Fs/16,LineColor="y",YLimits=[-60 20]);
scope_chD = spectrumAnalyzer(Name=sprintf("Channel %d Output",disp_slct(4)),SampleRate=(8/7)*Fs/16,LineColor="y",YLimits=[-60 20]);

% Main processing loop.
for iter = 1:nIter
    % Generate wideband samples for input to channelizer.
    smp = getWbSmp(N,upsmp,chan_en,qam_en,swp_en,swp_rate);
    scope_wb(smp);

    % Demux wideband signal for SSR input to channelizer.
    chnlzr_in = dmux_ssr_input(smp);

    % Create a cell array to pass input to HW.
    send_data = num2cell(varray.cint16(chnlzr_in),1);

    % Process data with HIL
    tic;
    return_data = hil_chnlzr.run(send_data);
    if stats_en
        tictocsum = tictocsum + toc;
    else
        t = toc;
    end

    % put new samples into fifo
    for idx = 1:ssro
        nsmp = length(return_data{idx});
        fifo_buffer(mod(fifo_ptr(idx)+fifo_size(idx)+(0:nsmp-1)-1,fifo_max)+1,idx) = return_data{idx};
        fifo_size(idx) = fifo_size(idx) + nsmp;
    end

    % Determine number of output samples to read from fifo.
    % More data than specified by chnFrameSize may be available for
    % reading, but a fixed amount is read so that output displays function
    % more like real spectrum analyzers. This also adds latency between
    % input and output displays.
    if min(fifo_size) == 0
        nout = 0;
    else
        nout = chanFrameSize;
    end

    if stats_en
        totalsamp = totalsamp + ssro * nout;
        fprintf("HIL processing throughput = %e samples per second.\n", totalsamp / tictocsum)
    elseif nout ~= 0
        stats_en = true;
    end

    if nout ~= 0
        % Get output samples from fifo.
        chnlzr_ssr_out = zeros(nout,ssro);
        for idx = 1:ssro
            chnlzr_ssr_out(:,idx) = fifo_buffer(mod(fifo_ptr(idx)+(0:nout-1)-1,fifo_max)+1,idx);
            fifo_ptr(idx) = mod(fifo_ptr(idx)+nout-1,fifo_max) + 1;
            fifo_size(idx) = fifo_size(idx) - nout;
        end

        % Extract individual channel streams from channelizer SSR output.
        chnlzr_out = mux_ssr_output(chnlzr_ssr_out);
    else
        chnlzr_out = [];
    end

    % Display spectrum of selected channels
    if ~isempty(chnlzr_out)
        scope_chA(chnlzr_out(:,disp_slct(1)+1))
        scope_chB(chnlzr_out(:,disp_slct(2)+1))
        scope_chC(chnlzr_out(:,disp_slct(3)+1))
        scope_chD(chnlzr_out(:,disp_slct(4)+1))
    end
end

% drain loop
drain_iter = 16;
empty_input = cell(1,7);
for i = 1:7
    empty_input{i} = varray.cint16([]);
end

for iter = 1:drain_iter
    return_data = hil_chnlzr.run(empty_input);
end

hil_chnlzr.getStats()
