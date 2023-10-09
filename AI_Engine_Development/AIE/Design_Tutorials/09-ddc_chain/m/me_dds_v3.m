%  
%  Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
%  SPDX-License-Identifier: MIT
%  

function [dds_out,phase_init_nxt]=me_dds_v3(fcstp,signal_length,v,phase_init)
%   function [dds_out]=me_dds(fcin,fs,signal_length)
%   ME DDS model with vectorization
    global ME ;
    if ~exist('phase_init', 'var'),
		phase_init = 0;
	end
    
    if isempty(ME),
        load dds_table.mat
    end
    
    if ~exist('v', 'var')
        v = 8;          
    end
    n  = floor(signal_length/v); 
    fc = fcstp;    
    
    %if(fc<0) 
    %    fc = fc + fs;
    %end

    % create sine cosine lookup table from ME refence data file.
    sc_tab = complex(double(ME.cos), double(ME.sin))/2^15 ;
	
	index_base = mod(fc,2^32);
	
    index_start = mod(index_base*v, 2^32);
    points = 0:n-1;
    index = phase_init + points * index_start;
	phase_init_nxt = mod(phase_init + n*index_start, 2^32);
    index = index/2^12;
    %wrap around to fetch from 20-bit addresable sine lookup table
    index = int32(mod(fix(index), 2^20));
    dds_in = me_quant(sc_tab(index+1));
	
	index_start = index_base;
	points = 0:v-1;
    index = points * index_start;
    index = index/2^12;
    %wrap around to fetch from 20-bit addresable sine lookup table
    index = int32(mod(fix(index), 2^20));
    ww = me_quant(sc_tab(index+1));
	
	
    %ww = round(exp(2j*pi*fc*(0:v-1)/fs)*2^15); 
    %ww(ww ==32768) = 32768-1 ;
    %ww = ww/2^15 ;

    %if fcin > 0
    %    ww = conj(ww);
    %    dds_in = conj(dds_in);
    %end
    dds_out = kron(dds_in, ww);
    dds_out = srs(dds_out, 15, 15);
    
return


function y = me_quant(x)

    xr = real(x);  xi=imag(x);
    yr = min(max(round(xr*2^15),-2^15),2^15-1);
    yi = min(max(round(xi*2^15),-2^15),2^15-1);
    y = complex(yr,yi);
return

