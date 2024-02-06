% 
% © Copyright 2023 Advanced Micro Devices, Inc.
%
% Licensed under the Apache License, Version 2.0 (the "License");
% you may not use this file except in compliance with the License.
% You may obtain a copy of the License at
%
%     http:%www.apache.org/licenses/LICENSE-2.0
%
% Unless required by applicable law or agreed to in writing, software
% distributed under the License is distributed on an "AS IS" BASIS,
% WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
% See the License for the specific language governing permissions and
% limitations under the License.
% 

function regression(x86,do_plot)
   if     (nargin == 0) x86 = 1; do_plot = 0;
   elseif (nargin == 1)          do_plot = 0;
   elseif (nargin ~= 2)          error('Incorrect');
   end
   if (x86)
     fir_a = load_aiesim('x86simulator_output/data/fir_o.txt','int',1);
     fft_a = load_aiesim('x86simulator_output/data/fft_o.txt','int',1);
   else
     fir_a = load_aiesim('aiesimulator_output/data/fir_o.txt','int',1);
     fft_a = load_aiesim('aiesimulator_output/data/fft_o.txt','int',1);
   end
   fir_g = load_aiesim('data/fir_o.txt','int',1);
   fft_g = load_aiesim('data/fft_o.txt','int',1);
   if (do_plot == 1)
     if (x86 == 1) tag='x86sim'; else tag='aiesim'; end
     figure;
     A = 2^15*0.6;
     subplot(2,2,1); plot(real(fir_g),'b.-'); hold on; plot(real(fir_a),'r.--'); hold off; axis([1,128,-A,A]);
     xlabel('Sample Index'); ylabel('Real');
     title(sprintf('Filter #1 ''%s'' Output',tag));
     legend('Gold','Actual');
     subplot(2,2,3); plot(imag(fir_g),'b.-'); hold on; plot(imag(fir_a),'r.--'); hold off; axis([1,128,-A,A]);
     xlabel('Sample Index'); ylabel('Imag');
     legend('Gold','Actual');
     subplot(2,2,[2,4]); plot(abs(fft_g),'b.-'); hold on; plot(abs(fft_a),'r.--'); hold off;
     v = axis; axis([1,1024,v(3),v(4)]);
     xlabel('Sample Index'); ylabel('Magnitude');
     title('Spectrum');
     legend('Gold','Actual');
     saveas(gcf,'png/fir1_result.png');
   end
   err_fir = fir_a - fir_g;
   err_fft = fft_a - fft_g(1:numel(fft_a));
   
   if ( max(abs(real(err_fir))) <= 1 && max(abs(imag(err_fir))) <= 1 && ...
        max(abs(real(err_fft))) <= 1 && max(abs(imag(err_fft))) <= 1 )
     fprintf(1,'--- PASSED ---\n');
   else
     fprintf(1,'*** FAILED ***\n');
   end
end

% ------------------------------------------------------------
% Load AIE simulation output file
% ------------------------------------------------------------

function [data] = load_aiesim( fname, dtype, is_complex )

   fid = fopen(fname,'r');
   done = 0;
   data = [];
   while ( done == 0 )
     line = fgets(fid);
     if ( line == -1 )
       done = 1;
     elseif ( strcmp(line(1),'T') == 0 )
       switch dtype
        case 'int',
         if ( is_complex == 1 )
           [X] = sscanf(line,'%d');
           data = [data;complex(X(1:2:end),X(2:2:end))];
         else
            [val] = sscanf(line,'%d');
            data = [data;val];
         end
        case 'double',
         if ( is_complex == 1 )
            [X] = sscanf(line,'%lf');
            data = [data;complex(X(1:2:end),X(2:2:end))];
         else
            [val] = sscanf(line,'%lf');
            data = [data;val];
         end
        otherwise,
         error(sprintf('Unsupported %s',dtype));
       end
     end
   end
end

