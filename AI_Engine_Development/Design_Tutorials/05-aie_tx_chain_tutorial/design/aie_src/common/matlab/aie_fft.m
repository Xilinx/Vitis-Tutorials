function [y] = aie_fft(x,mode)

%  (c) Copyright 2021 Xilinx, Inc. All rights reserved.
%
%  This file contains confidential and proprietary information
%  of Xilinx, Inc. and is protected under U.S. and
%  international copyright and other intellectual property
%  laws.
%
%  DISCLAIMER
%  This disclaimer is not a license and does not grant any
%  rights to the materials distributed herewith. Except as
%  otherwise provided in a valid license issued to you by
%  Xilinx, and to the maximum extent permitted by applicable
%  law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
%  WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
%  AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
%  BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
%  INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
%  (2) Xilinx shall not be liable (whether in contract or tort,
%  including negligence, or under any other theory of
%  liability) for any loss or damage of any kind or nature
%  related to, arising under or in connection with these
%  materials, including for any direct, or any indirect,
%  special, incidental, or consequential loss or damage
%  (including loss of data, profits, goodwill, or any type of
%  loss or damage suffered as a result of any action brought
%  by a third party) even if such damage or loss was
%  reasonably foreseeable or Xilinx had been advised of the
%  possibility of the same.
%
%  CRITICAL APPLICATIONS
%  Xilinx products are not designed or intended to be fail-
%  safe, or for use in any application requiring fail-safe
%  performance, such as life-support or safety devices or
%  systems, Class III medical devices, nuclear facilities,
%  applications related to the deployment of airbags, or any
%  other applications that could lead to death, personal
%  injury, or severe property or environmental damage
%  (individually and collectively, "Critical
%  Applications"). Customer assumes the sole risk and
%  liability of any use of Xilinx products in Critical
%  Applications, subject only to applicable laws and
%  regulations governing limitations on product liability.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% function [y] = aie_fft(x,mode)
% mode.r = 4 (default radix)
% mode.inv = 1 (for IFFT, 0 for FFT)
% mode.shift = [15, 15, 15, 15, 15, 23]
% stage shifts as in ME kernel
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    N = length(x);
    xin = x;
    if(~isrow(x))
       xin = conj(x');
    end % if   
    nf = factor(N);
    if(sum(nf~=2))
       fprintf('Length has to be power of 2!!\n');
       y = 0 ;
       return;
    end %if  
    % Radix-4 Recursive Cooley-Tukey
    % Stage computes are same as ME
    y = aie_sat(radix4_dit_r(xin,mode,0));
    if(~isrow(x))
       y = conj(y');
    end % if
end

function y = radix4_dit_r(x,mode,i)
    N = length(x);
    shift = mode.shift(length(mode.shift)-i);
    if(N==4)
       tw1 = 32767/2^15; % 16-bit twiddle
       tw2 = tw1 ;
       x1 = x(1); x2 = x(2);
       x3 = x(3)*tw2; x4 = x(4)*tw2;
       inv_scale = -1i;
       if(mode.inv) 
          inv_scale = 1i;
       end %if
       t1 = aie_round(x1+x3); t2 = aie_round(x1-x3);
       t3 = aie_round(x2+x4)*tw1; t4 = aie_round(x2-x4)*(tw1*inv_scale);
       y = [t1+t3 t2+t4 t1-t3 t2-t4];
       y = aie_round(y*2^15/2^shift);
       return;
    elseif(N==2)
       tw = 32767;
       x1 = x(1);
       x2 = x(2)*tw/2^15;
       y = [x1+x2 x1-x2] ;
       y = aie_round(y*2^15/2^shift); % 16-bit SRS
       return; 
    end

    tw1 = exp(-2*pi*1*(0:N/4-1)*1i/N);
    tw1 = aie_sat(round(tw1*2^15))/2^15;
    tw2 = exp(-2*pi*2*(0:N/4-1)*1i/N);
    tw2 = aie_sat(round(tw2*2^15))/2^15;
    inv_scale = -1i;
    if(mode.inv) 
       tw1 = conj(tw1); tw2 = conj(tw2);
       inv_scale = 1i;
    end %if
    
    x1 = radix4_dit_r(x(1:4:end),mode,i+1);
    x2 = radix4_dit_r(x(2:4:end),mode,i+1);
    x3 = radix4_dit_r(x(3:4:end),mode,i+1).*tw2;
    x4 = radix4_dit_r(x(4:4:end),mode,i+1).*tw2;
    
    t1 = aie_round(x1+x3); t2 = aie_round(x1-x3);
    t3 = aie_round(x2+x4).*tw1; t4 = aie_round(x2-x4).*(tw1*inv_scale);
    
    y = [t1+t3 t2+t4 t1-t3 t2-t4];
    y = aie_round(y*2^15/2^shift);   
end

function y = aie_sat(x)
   y = x;
   yi = find(real(y)>=32768);
   y(yi) = 32767 + imag(y(yi))*1i ;
   yi = find(real(y)<=-32768);
   y(yi) = -32768 + imag(y(yi))*1i ;
   yi = find(imag(y)>=32768);
   y(yi) = real(y(yi)) + 32767*1i ;
   yi = find(imag(y)<=-32768);
   y(yi) = real(y(yi)) - 32768*1i ;
end

function y = aie_round(x)
   y = floor(x+0.5+0.5i);
end
