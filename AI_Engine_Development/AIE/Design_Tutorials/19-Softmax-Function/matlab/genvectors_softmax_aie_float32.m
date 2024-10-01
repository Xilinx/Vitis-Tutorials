%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Richard Buz

function genvectors_softmax_aie_float32(numclass, numcalc)

% assign default arguments
if nargin < 2
    ncalc = 10;
else
    ncalc = numcalc;
end

if nargin == 0
    nclass = 2048;
else
    nclass = numclass;
end

% generate random inputs for softmax
rng('shuffle')
max_x = 800.0;
min_x = -800.0;
min_abs_x = realmin('single');
hexlut = ['0':'9','A':'F'];
sm_in = single(Inf * ones(nclass*ncalc,1));
I = -1;
while ~isempty(I)
    I = find(sm_in < min_x | sm_in > max_x | isnan(sm_in) | (abs(sm_in) < min_abs_x & sm_in ~= 0.0));
    if ~isempty(I)
        sm_in(I) = typecast(uint32(hex2dec(hexlut(randi(16,length(I),8)))), 'single');
    end
end
sm_in = reshape(sm_in, nclass, ncalc);

% compute softmax using double-precision
max_sm_in = max(sm_in);
sm_in_shft = sm_in - max_sm_in;
exp_dbl = exp(double(sm_in_shft));
exp_dbl_sum = sum(exp_dbl);
smax_gold = exp_dbl ./ exp_dbl_sum;

% estimate exponentials to match AIE processing
max_scl = max_sm_in * single(1.0/log(2));
x = sm_in * single(1.0/log(2)) - max_scl;
xfrac = x - floor(x);
ply = single(get_cheby_poly(5));
crct_fcn = ply(1) * xfrac + ply(2);
crct_fcn = crct_fcn .* xfrac + ply(3);
crct_fcn = crct_fcn .* xfrac + ply(4);
crct_fcn = crct_fcn .* xfrac + ply(5);
x_crct = x - crct_fcn;
exp_int = int32(2^23 * (x_crct + 127));
exp_int(exp_int < 0) = 0; % overflow results set to zero
exp_int(exp_int > 1065353216) = 0; % greater than 1.0
exp_est = reshape(typecast(exp_int(:),'single'),nclass,ncalc);

% sum exponentials and compute inverse
exp_scl = 1.0 ./ sum(exp_est);
smax = exp_scl .* exp_est;

% compare with double precision gold reference
for idx = 1:ncalc
    [max_diff, I] = max(abs(smax_gold(:,idx) - double(smax(:,idx))));
    fprintf('Vec %d: softmax sum = %0.9e, max_diff = %0.9e, smax(fp32) = %0.9e, smax(gold) = %0.9e\n', ...
        idx, sum(smax(:,idx)), max_diff, smax(I,idx), smax_gold(I,idx));
end

% determine test vector sizes so AIE simulation has enough data
buffsz = 8 * ceil(nclass / 8);
pad = buffsz - nclass;

% create config.h file
fid = fopen('../aie/src/config.h','w');
fprintf(fid, ['//\n// Copyright (C) 2024, Advanced Micro Devices, Inc. All ' ...
              'rights reserved.\n// SPDX-License-Identifier: MIT\n//\n' ...
              '// Author: Richard Buz\n\n#pragma once\n\n#define\tBUFFSZ\t%d\n' ...
              '#define\tNRUN\t%d'],buffsz, ncalc);
fclose(fid);

% create input test vector
fname = '../aie/data/smax_input.txt';
fid = fopen(fname,'w');
if fid > 0
    input_tv = [sm_in; single(-2^127 * ones(pad,ncalc))];
    input_tv_int32 = typecast(input_tv(:),'int32');
    itv = reshape(input_tv_int32(:),2,length(input_tv_int32(:))/2);
    fprintf(fid,'%d %d\n',itv);
    fclose(fid);
end

% create output test vector
fname = '../aie/data/smax_output_ref.txt';
fid = fopen(fname,'w');
if fid > 0
    output_tv = [smax; single(zeros(pad,ncalc))];
    output_tv_int32 = typecast(output_tv(:),'int32');
    otv = reshape(output_tv_int32(:),2,length(output_tv_int32(:))/2);
    fprintf(fid,'%d %d\n',otv);
    fclose(fid);
end

% create golden reference
fname = '../aie/data/smax_gold_ref.txt';
fid = fopen(fname,'w');
if fid > 0
    output_gld = [smax_gold; zeros(pad,ncalc)];
    ogld = reshape(output_gld(:),2,length(output_gld(:))/2);
    fprintf(fid,'%.25e %.25e\n',ogld);
    fclose(fid);
end

fprintf('\nGenerated test vectors for SoftMax - categories = %d, vectors = %d\n', nclass, ncalc)