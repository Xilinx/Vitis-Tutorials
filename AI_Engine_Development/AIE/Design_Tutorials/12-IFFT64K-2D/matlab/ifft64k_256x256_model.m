%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function [ifft_o] = ifft64k_256x256_model( ifft_i )
   N = numel(ifft_i);
   Nhalf = sqrt(N);
   assert( N == 65536 );
   
   % Need to use DIT when computing transforms up front:
   twid = complex(zeros(Nhalf,Nhalf),zeros(Nhalf,Nhalf));
   for rr = 0 : Nhalf-1
     for cc = 0 : Nhalf-1
       twid(1+rr,1+cc) = exp(+1i*2*pi*rr*cc/N);
     end
   end
   
   % First compute transforms along rows:
   data0 = reshape(ifft_i,Nhalf,Nhalf);
   data1 = Nhalf*ifft(data0,Nhalf,2);

   % Apply pointwise twiddle matrix:
   % --> Note: first row is multiplication by unity
   data2 = data1 .* twid;
   
   % Second compute 16-K point transforms along columns:
   data3 = Nhalf*ifft(data2,Nhalf,1);
   ifft_o = reshape(data3.',1,[]);
end
