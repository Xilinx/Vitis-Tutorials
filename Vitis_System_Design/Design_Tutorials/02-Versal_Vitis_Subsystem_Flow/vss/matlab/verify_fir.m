%
% Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: X11
%
% Author Derek Hagen

%% Setup paths to source files
aie_src_paths = {...
    "../ip/aie/src", ...
    "../ip/aie/src/kernels", ...
    "../ip/aie/src/graphs"};

%% Setup buffer sizes and precision
B_SIZE = 256;
M_SIZE = 16;
SIM_TIME = 5000;
REF_DLY = 0;

n_taps = 16;
n_shift = 14;
mid_tap = (n_taps+1)/2;
size_coe_vec = ceil(n_taps/16)*8;
f_start = 0.25;
f_stop = 0.75;
bits=16;
N = 2^(bits-2);

%% Two tone with noise input stimuli
decim=1;
Fs = decim*400;
f0 = 12;
f1 = 180;
f2 = 100;
ph0 = 0;
ph1 = 0;%pi/3;
ph2 = 0;%pi/3;
t = decim*4096;
fft_size = 2^floor(log(t)/log(2))/2/2;
amp0 = -20;
amp1 = -20;
amp2 = -20;
crest_factor = 1;%sqrt(2); % Peak to average for sine
ssb = 1;
g0 = 10^(amp0/20)*crest_factor*ssb;
g1 = 10^(amp1/20)*crest_factor*ssb;
g2 = 10^(amp1/20)*crest_factor*ssb;
noise_level = -25;
noise_gain = 10^(noise_level/20);
noise = 2*rand(t, 2)-1;
yn=noise_gain.*complex(noise(:,1), noise(:,2));

x = linspace(0,2*pi*t/Fs,t);
y = g0.*sin(f0*x+ph0)+g1.*sin(f1*x+ph1)+g2.*sin(f2*x+ph2)+yn';
bb = round((y)*N);

%% Step or Impulse response inputs

% t = 512*2;
% bb_len=1;
% bb_start=75;
% %bb=[zeros(1,bb_start), N/2*ones(1,bb_len), zeros(1,t-bb_start-bb_len)];
% bb=complex(N*[zeros(1,bb_start), ones(1,bb_len), zeros(1,t-bb_start-bb_len)]);


%% ramp filter
% coe = 1:n_taps;
% coe_d3p = reshape(coe_d3, 3, []);
% coe0 =  coe_d3p(1, :);
% coe1 =  coe_d3p(2, :);
% coe2 =  coe_d3p(3, :);

%% Design filter
%coe=round(firhalfband(n_taps-1, f_stop)*2^(bits-1));
coe_lpass=round(fir2(n_taps-1, [0 f_start f_start 1], [1 1 0 0])*2^(bits-1));
coe_hpass=round([fir2(n_taps-2, [0 f_stop f_stop 1], [0 0 1 1])*2^(bits-1) 0]); % Reduce order to nearest even number and fit to 16 taps
coe_bpass=round(fir2(2*n_taps-1, [0 f_start f_stop 1], [0 1 1 0])*2^(bits-1));
coe_bstop=round(fir2(2*n_taps-2, [0 f_start f_stop 1], [1 0 0 1])*2^(bits-1));
writematrix(coe_lpass',  '../ip/aie/data/coe0.txt', 'Delimiter', ' ');
writematrix(coe_hpass', '../ip/aie/data/coe1.txt', 'Delimiter', ' ');
%fvtool(coe_lpass);fvtool(coe_hpass);fvtool(coe_bpass);fvtool(coe_bstop);

%% Write stimuili for aiesimulator

% dlmwrite('../ip/aie/data/input0.txt', d0, ' ');
% dlmwrite('../ip/aie/data/input1.txt', d1, ' ');

%% Run Simulink
%clear out;
%out = sim('my_testbench', t);
%out = sim('fir_decim3_tb', t);
%out = sim('tb_fir16t', t);

%% Run x86sim via Matlab
% Create graph object:
my_aiefir = vfs.aieGraph(input_file = '../ip/aie/src/tb_fir.cpp', include_paths = aie_src_paths);
%my_vadd = vfs.hlsKernel(input_files = '../ip/vadd_s/src/vadd_s.cpp', hls_function = "vadd_s");

[fir_out0, fir_out1] = my_aiefir.run(varray.cint16(bb), varray.cint16(bb), varray.int16(fliplr(coe_lpass)), varray.int16(fliplr(coe_hpass)));
% Recast vfs types to matlab types
out_fir0 = int16(fir_out0); 
out_fir1 = int16(fir_out1);

%% Reference model
y_ref_lp = round((bits-2)*filter(coe_lpass, 2^(n_shift), bb))/(bits-2);
y_ref_hp = round((bits-2)*filter(coe_hpass, 2^(n_shift), bb))/(bits-2);


%% Frequency Domain
% offset data to avoid startup settling in spectrum plot
t_start = 300;
t_range = t_start:t_start+fft_size;
% compensation for propagation delay of AIE design vs reference model and check alignment
dly_trim = 0; %-16; % Trim this when using cycle approximate AIE sim
t_range_trim = t_start+dly_trim:t_start+dly_trim+fft_size;

% Input signal in Freqency Domain
y_fd = 20*log10(1/N*abs(fftshift(fft(bb(t_range))))/fft_size*2);
fd_axis = Fs/2*linspace(0, 1, fft_size/2+1)/decim;

% Outputs in Frequency Domain
out_fir0_fd = (20*log10(1/N*abs(fftshift(fft(out_fir0(t_range))))/fft_size*2));
out_fir1_fd = (20*log10(1/N*abs(fftshift(fft(out_fir1(t_range))))/fft_size*2));
y_ref_lp_fd = (20*log10(1/N*abs(fftshift(fft(y_ref_lp(t_range_trim))))/fft_size*2));
y_ref_hp_fd = (20*log10(1/N*abs(fftshift(fft(y_ref_hp(t_range_trim))))/fft_size*2));


%% Plotting FIR0 filter
fig1 = figure(1); clf;
tiledlayout(3,3);hold on;

ax_td_in=nexttile;
plot(t_range, real(bb(t_range_trim)), 'g');
title(ax_td_in, 'Input', 'Time (real)');
ylabel('Amplitude');
xlabel('Sample');

ax_fd_in=nexttile([1 2]);
plot(fd_axis, y_fd(fft_size/2:fft_size), 'g');
title(ax_fd_in, 'FIR Input Spectrum', [num2str(fft_size) ' point FFT']);

ax_td_out=nexttile; hold on;
plot(t_range, real(out_fir0(t_range)), 'b');
plot(t_range, real(y_ref_lp(t_range_trim)), 'ko');
title(ax_td_out, 'Output', 'Time (real)');
legend({'out\_fir0', 'y\_ref'});
ylabel('Amplitude');
xlabel('Sample');

ax_fd_out=nexttile([1 2]);hold on;
plot(fd_axis, out_fir0_fd(fft_size/2:fft_size), 'b');
plot(fd_axis, y_ref_lp_fd(fft_size/2:fft_size), 'ko');
title(ax_fd_out, 'FIR Output0 Spectrum', [num2str(fft_size) ' point FFT']);
legend({'out\_fir0', 'y\_refi\_fd'});
ylabel('Magnitude [dB]');
xlabel('Frequency [MHz]');

ax_td_err=nexttile; hold on;
plot(t_range, real(out_fir0(t_range)) - int16(real(y_ref_lp(t_range_trim))'), 'R');
title(ax_td_err, 'Error', 'Time (real)');
ylabel('Amplitude');
xlabel('Sample');

ax_fd_comp=nexttile([1 2]);hold on;
plot(fd_axis, y_fd(fft_size/2:fft_size), 'g');
plot(fd_axis, out_fir0_fd(fft_size/2:fft_size), 'b');
plot(fd_axis, y_ref_lp_fd(fft_size/2:fft_size), 'ko');
title(ax_fd_comp, 'Compare Input & Output Spectrum', [num2str(fft_size) ' point FFT']);
legend({'y\_fd', 'out\_fir0\_fd', 'y\_ref\_lp\_fd'});
ylabel('Magnitude [dB]');
xlabel('Frequency [MHz]');

%% Plotting FIR0 filter
fig2 = figure(2); clf;
tiledlayout(3,3);hold on;

ax_td_in=nexttile;
plot(t_range, real(bb(t_range_trim)), 'g');
title(ax_td_in, 'Input', 'Time (real)');
ylabel('Amplitude');
xlabel('Sample');

ax_fd_in=nexttile([1 2]);
plot(fd_axis, y_fd(fft_size/2:fft_size), 'g');
title(ax_fd_in, 'FIR Input Spectrum', [num2str(fft_size) ' point FFT']);

ax_td_out=nexttile; hold on;
plot(t_range, real(out_fir1(t_range)), 'b');
plot(t_range, real(y_ref_hp(t_range_trim)), 'ko');
title(ax_td_out, 'Output', 'Time (real)');
legend({'out\_fir1', 'y\_ref'});
ylabel('Amplitude');
xlabel('Sample');

ax_fd_out=nexttile([1 2]);hold on;
plot(fd_axis, out_fir1_fd(fft_size/2:fft_size), 'b');
plot(fd_axis, y_ref_hp_fd(fft_size/2:fft_size), 'ko');
title(ax_fd_out, 'FIR Output0 Spectrum', [num2str(fft_size) ' point FFT']);
legend({'out\_fir1', 'y\_refi\_fd'});
ylabel('Magnitude [dB]');
xlabel('Frequency [MHz]');

ax_td_err=nexttile; hold on;
plot(t_range, real(out_fir1(t_range)) - int16(real(y_ref_hp(t_range_trim))'), 'R');
title(ax_td_err, 'Error', 'Time (real)');
ylabel('Amplitude');
xlabel('Sample');

ax_fd_comp=nexttile([1 2]);hold on;
plot(fd_axis, y_fd(fft_size/2:fft_size), 'g');
plot(fd_axis, out_fir1_fd(fft_size/2:fft_size), 'b');
plot(fd_axis, y_ref_hp_fd(fft_size/2:fft_size), 'ko');
title(ax_fd_comp, 'Compare Input & Output Spectrum', [num2str(fft_size) ' point FFT']);
legend({'y\_fd', 'out\_fir1\_fd', 'y\_ref\_hp\_fd'});
ylabel('Magnitude [dB]');
xlabel('Frequency [MHz]');


