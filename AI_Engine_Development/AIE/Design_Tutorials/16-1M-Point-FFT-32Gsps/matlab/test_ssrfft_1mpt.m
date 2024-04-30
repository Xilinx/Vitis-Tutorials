%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

function test_ssrfft_1mpt(N_symb, InputMode, TODEBUG)

if ~exist('N_symb', 'var')
	N_symb = 4;
	rng(12345);
else
	rng shuffle;
end

N_Parallel = 32;
N_datablk = 1024*1024;


%-------------------------------------------------------
% Generate Input
%-------------------------------------------------------
a = single(randn(2, N_symb*N_datablk));
tpwx_dat = complex(a(1,:), a(2,:));
%-------------------------------------------------------


%-------------------------------
% Allocate memory for output
%-------------------------------
y     = zeros(1, N_datablk*N_symb);
%--------------------------------


%-------------------------------
% Call the reference model and compare the results
%-------------------------------
total_diff = 0;
total_pwr  = 0;

for symbid = 1:N_symb

    this_x = tpwx_dat((symbid-1)*N_datablk + (1:N_datablk));

	% call golden ref model
    this_y = ref_model_fft_1mpt(this_x);
	
	% call floating point ref model
	[this_y_fp, dbinfo(symbid)] = aie_model_fft_fp_1mpt(this_x);

	% save the test vectors
	y(   (symbid-1)*N_datablk + (1:N_datablk)) = this_y;

    % calculate difference between floating point model output and FFT function output
	total_diff = total_diff + sum(abs(this_y(:) - this_y_fp(:)).^2);
    total_pwr  = total_pwr  + sum(abs(this_y(:)).^2);

end
%-----------------------------------

fprintf(1, 'Mean Square Error = %.1f dBc\n', 10*log10(total_diff./total_pwr) );


%-----------------------------------
% Save test vectors
%-----------------------------------
if ~exist('../aie_src/data', 'dir'), mkdir('../aie_src/data'); end


% (1) FFT input and output data
idx = reshape(1:1024, 8, []);
idx = reshape([idx(1:4, :), idx(5:8, :)], 1, []);

for k=1:N_symb

	if(k==1)
		opmod = 'wt';
	else
		opmod = 'at';
	end


    xx = reshape(tpwx_dat((k-1)*N_datablk + (1:N_datablk)), 1024, []);
	yy = reshape(y(       (k-1)*N_datablk + (1:N_datablk)), 1024, []);
	
	for phase=1:N_Parallel
	
		for t=1:2
		
            % input in int32 format
            a = xx(phase:N_Parallel:end, t:2:end);
		    a = reshape(a.', 1, []); a = [real(a);imag(a)]; a = float2int(a);

		    fid = fopen(sprintf('../aie_src/data/fft_din_%02d%c.txt', phase-1, 'a'+t-1), opmod);
		    fprintf(fid, '%d %d\n', a);
		    fclose(fid);

            % output in float format
			a = yy(phase:N_Parallel:end, idx((t-1)*512+(1:512))); 
			a = reshape(a.', 1, []); a = [real(a);imag(a)];
			
			fid = fopen(sprintf('../aie_src/data/fft_gold_%02d%c.txt', phase-1, 'a'+t-1), opmod);
			fprintf(fid, '%f %f\n', a);
			fclose(fid);
		end
	end

end


% save mid-output
for k=1:numel(dbinfo)

	if(k==1)
		opmod = 'wt';
	else
		opmod = 'at';
	end

	x2 = dbinfo(k).x2;
	
	for phase=1:N_Parallel
	
		aa2 = x2(phase:N_Parallel:end, :); aa2 = reshape(aa2.', 8, []);
		
		for t=1:2
		
			a = aa2( (t-1)*4 + (1:4), :);
			a = reshape(a, 1, []); a = [real(a);imag(a)];
			
			fid = fopen(sprintf('../aie_src/data/fft_mid_out_%02d%c.txt', phase-1, 'a'+t-1), opmod);
			fprintf(fid, '%f %f\n', a);
			fclose(fid);
		end
	end
	
end



% save mid-input
for k=1:numel(dbinfo)

	if(k==1)
		opmod = 'wt';
	else
		opmod = 'at';
	end

	x2 = dbinfo(k).x2;
	
	for phase=1:N_Parallel
	
		for t=1:2
		
			a = x2(t:2:end, phase:N_Parallel:end);
			a = reshape(a, 1, []); a = [real(a);imag(a)]; a = float2int(a);
			
			fid = fopen(sprintf('../aie_src/data/fft_mid_in_%02d%c.txt', phase-1, 'a'+t-1), opmod);
			fprintf(fid, '%d %d\n', a);
			fclose(fid);

		end
	end
	
end

