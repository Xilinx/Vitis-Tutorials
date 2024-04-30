%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

function [y, dbinfo] = aie_model_fft_fp_1mpt(x)


	%------------------------------
	% 1024 1K-pt FFTs
	%------------------------------
	x = reshape(x, 1024, 1024);
	
	xx = zeros(1024, 1024);
	
	for k=1:1024
		xx(k, :) = fft(x(k,:));
	end
	
	dbinfo.x1 = xx;
	
	
	%------------------------------
	% Phase rotation
	%------------------------------
	t = 0:1023;
	t = kron(t, t');
	t = exp( -1i * 2 * pi / (1024*1024) .* t);
	
	xx = xx .* t;
	
	dbinfo.t  = t;
	dbinfo.x2 = xx;
	
	
	%------------------------------
	% 1024 1K-pt FFTs
	%------------------------------	
	for k=1:1024
		xx(:, k) = fft(xx(:,k));
	end	

	% save to y
	y = reshape( xx.', 1, []);