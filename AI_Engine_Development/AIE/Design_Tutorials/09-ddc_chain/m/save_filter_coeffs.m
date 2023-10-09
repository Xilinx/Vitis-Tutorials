% //-------------------------------------------------------------------------------
% //--   ____  ____
% //--  /   /\/   /
% //-- /___/  \  /   Vendor: Xilinx
% //-- \   \   \/    Version: 1.0
% //--  \   \        Filename: $RCSfile: save_filter_coeffs.m,v $
% //--  /   /        Date Last Modified: $Date: 2017/03/01 10:16:28 $
% //-- /___/   /\    Date Created: 2017/02/20
% //-- \   \  /  \
% //--  \___\/\___\
% //--
% //-- Device  : All
% //-- Library : 
% //-- Purpose : Compute the filter coefficients and save to separate txt files
% //-------------------------------------------------------------------------------
% //-- 
% //-- Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% //-- SPDX-License-Identifier: MIT
% //-------------------------------------------------------------------------------

%---------------------------------------------
% Generate Coefficients for Channel Filter
%---------------------------------------------
N     = 88;        % Order
Fpass = 9000000;   % Passband Frequency
Fstop = 10000000;  % Stopband Frequency
Astop = 70;        % Stopband Attenuation (dB)
Fs    = 30720000;  % Sampling Frequency
h = fdesign.lowpass('n,fp,fst,ast', N, Fpass, Fstop, Astop, Fs);
Hd = design(h, 'equiripple');

% Quantize the coefficient to 16 bit fixed number
coef_float = Hd.numerator;
% normalize
coef_float = coef_float./sqrt(sum(coef_float.^2));
% quantize
coef_fix = round(coef_float*2^15);

% Write out the coefficients into a file
fid = fopen('coef_chFir89t.txt', 'wt');
fprintf(fid, '%d ', coef_fix);
fclose(fid);



%---------------------------------------------
% Generate Coefficients for HB47
%---------------------------------------------
N  = 46;         % Order
TW = 24000000;   % Transition Width
Fs = 245760000;  % Sampling Frequency
h = fdesign.halfband('Type', 'Lowpass', 'n,tw', N, TW, Fs);
Hd = design(h, 'equiripple', 'StopbandShape', 'flat');

% Quantize the coefficient to 16 bit fixed number
coef_float = Hd.numerator;
% quantize
coef_fix = round(coef_float*2^16);
%coef_fix(coef_fix==32768)=32767;
% Write out the coefficients into a file
fid = fopen('coef_hbf47t.txt', 'wt');
fprintf(fid, '%d ', coef_fix);
fclose(fid);




%---------------------------------------------
% Generate Coefficients for HB23
%---------------------------------------------
N  = 22;           % Order
TW = 30720-18015;  % Transition Width
Fs = 61440;        % Sampling Frequency
h = fdesign.halfband('Type', 'Lowpass', 'n,tw', N, TW, Fs);
Hd = design(h, 'equiripple', 'StopbandShape', 'flat');

% Quantize the coefficient to 16 bit fixed number
coef_float = Hd.numerator;
% quantize
coef_fix = round(coef_float*2^16);
coef_fix(coef_fix==32768)=32767;
% Write out the coefficients into a file
fid = fopen('coef_hbf23t.txt', 'wt');
fprintf(fid, '%d ', coef_fix);
fclose(fid);


%---------------------------------------------
% Generate Coefficients for HB11
%---------------------------------------------
N  = 10;           % Order
TW = 61440-18015;  % Transition Width
Fs = 122880;       % Sampling Frequency
h = fdesign.halfband('Type', 'Lowpass', 'n,tw', N, TW, Fs);
Hd = design(h, 'equiripple', 'StopbandShape', 'flat');

% Quantize the coefficient to 16 bit fixed number
coef_float = Hd.numerator;
% quantize
coef_fix = round(coef_float*2^16);
coef_fix(coef_fix==32768)=32767;
% Write out the coefficients into a file
fid = fopen('coef_hbf11t.txt', 'wt');
fprintf(fid, '%d ', coef_fix);
fclose(fid);





%---------------------------------------------
% Generate Coefficients for Channel Filter
%---------------------------------------------
N     = 198;        % Order
Fpass = 49.15e6;   % Passband Frequency
Fstop = 50.65e6;  % Stopband Frequency
Astop = 60;        % Stopband Attenuation (dB)
Fs    = 122.88e6;  % Sampling Frequency
h = fdesign.lowpass('n,fp,fst,ast', N, Fpass, Fstop, Astop, Fs);
Hd = design(h, 'equiripple');

% Quantize the coefficient to 16 bit fixed number
coef_float = Hd.numerator;
% normalize
coef_float = coef_float./sqrt(sum(coef_float.^2));
% quantize
coef_fix = round(coef_float*2^15);

% Write out the coefficients into a file
fid = fopen('coef_chFir199t.txt', 'wt');
fprintf(fid, '%d ', coef_fix);
fclose(fid);

