%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Richard Buz

function genvectors_softmax_aie_ml_bfloat16(numclass, numcalc)

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
max_x = 100.0;
min_x = -100.0;
min_abs_x = realmin('single');
hexlut = ['0':'9','A':'F'];
sm_in = single(Inf * ones(nclass*ncalc,1));
I = -1;
while ~isempty(I)
    I = find(sm_in < min_x | sm_in > max_x | isnan(sm_in) | (abs(sm_in) < min_abs_x & sm_in ~= 0.0));
    if ~isempty(I)
        sm_in(I) = typecast(int32(hex2dec([hexlut(randi(16,length(I),4)) repmat('0000',length(I),1)])), 'single');
    end
end
sm_in = reshape(sm_in, nclass, ncalc);

% compute softmax using double-precision
max_sm_in = max(sm_in);
sm_in_shft = sm_in - max_sm_in;
exp_dbl = exp(double(sm_in_shft));
exp_dbl_sum = sum(exp_dbl);
smax_gold = exp_dbl ./ exp_dbl_sum;

% estimate exponentials to match AIE-ML processing
exp_int = int16(floor(to_bfloat16(185.0 * to_bfloat16(sm_in_shft) + 16256.0)));
exp_int(exp_int < 0) = 0; % overflow results set to zero
exp_int(exp_int > 16256) = 0; % greater than 1.0
exp_est = reshape(typecast(int32(2^16*double(exp_int(:))),'single'),nclass,ncalc);

% sum exponentials and compute inverse
exp_scl = to_bfloat16(1.0 ./ to_bfloat16(sum(exp_est)));
smax = to_bfloat16(exp_scl .* exp_est);

% flush subnormals to zero
I = smax < 2^-126;
smax(I) = 0.0;

% compare with double precision gold reference
for idx = 1:ncalc
    [max_diff, I] = max(abs(smax_gold(:,idx) - double(smax(:,idx))));
    fprintf('Vec %d: softmax sum = %0.9e, max_diff = %0.9e, smax(bfloat16) = %0.9e, smax(gold) = %0.9e\n', ...
        idx, sum(smax(:,idx)), max_diff, smax(I,idx), smax_gold(I,idx));
end

% determine test vector sizes so AIE simulation has enough data
buffsz = 16 * ceil(nclass / 16);
pad = buffsz - nclass;

% create config.h file
fid = fopen('../aie-ml/src/config.h','w');
fprintf(fid, ['//\n// Copyright (C) 2024, Advanced Micro Devices, Inc. All ' ...
              'rights reserved.\n// SPDX-License-Identifier: MIT\n//\n' ...
              '// Author: Richard Buz\n\n#pragma once\n\n#define\tBUFFSZ\t%d\n' ...
              '#define\tNRUN\t%d'],buffsz, ncalc);
fclose(fid);

% create input test vector
fname = '../aie-ml/data/smax_input.txt';
fid = fopen(fname,'w');
if fid > 0
    input_tv = [sm_in; to_bfloat16(-2^127 * ones(pad,ncalc))];
    input_tv_int16 = int16(fix(double(typecast(input_tv(:),'int32'))/2^16));
    itv = reshape(input_tv_int16(:),4,length(input_tv_int16(:))/4);
    fprintf(fid,'%d %d %d %d\n',itv);
    fclose(fid);
end

% create output test vector
fname = '../aie-ml/data/smax_output_ref.txt';
fid = fopen(fname,'w');
if fid > 0
    output_tv = [smax; to_bfloat16(zeros(pad,ncalc))];
    output_tv_int16 = int16(fix(double(typecast(output_tv(:),'int32'))/2^16));
    otv = reshape(output_tv_int16(:),4,length(output_tv_int16(:))/4);
    fprintf(fid,'%d %d %d %d\n',otv);
    fclose(fid);
end

% create golden reference
fname = '../aie-ml/data/smax_gold_ref.txt';
fid = fopen(fname,'w');
if fid > 0
    output_gld = [smax_gold; zeros(pad,ncalc)];
    ogld = reshape(output_gld(:),4,length(output_gld(:))/4);
    fprintf(fid,'%.25e %.25e %.25e %.25e\n',ogld);
    fclose(fid);
end

fprintf('\nGenerated test vectors for SoftMax - categories = %d, vectors = %d\n', nclass, ncalc)
