%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function regression(x86)

   addpath('../common');

   if (x86)
     act0_o = load_aiesim('x86simulator_output/data/sig0_o.txt','int',1);
     act1_o = load_aiesim('x86simulator_output/data/sig1_o.txt','int',1);
     act2_o = load_aiesim('x86simulator_output/data/sig2_o.txt','int',1);
     act3_o = load_aiesim('x86simulator_output/data/sig3_o.txt','int',1);
     act4_o = load_aiesim('x86simulator_output/data/sig4_o.txt','int',1);
     act5_o = load_aiesim('x86simulator_output/data/sig5_o.txt','int',1);
     act6_o = load_aiesim('x86simulator_output/data/sig6_o.txt','int',1);
     act7_o = load_aiesim('x86simulator_output/data/sig7_o.txt','int',1);
   else
     act0_o = load_aiesim('aiesimulator_output/data/sig0_o.txt','int',1);
     act1_o = load_aiesim('aiesimulator_output/data/sig1_o.txt','int',1);
     act2_o = load_aiesim('aiesimulator_output/data/sig2_o.txt','int',1);
     act3_o = load_aiesim('aiesimulator_output/data/sig3_o.txt','int',1);
     act4_o = load_aiesim('aiesimulator_output/data/sig4_o.txt','int',1);
     act5_o = load_aiesim('aiesimulator_output/data/sig5_o.txt','int',1);
     act6_o = load_aiesim('aiesimulator_output/data/sig6_o.txt','int',1);
     act7_o = load_aiesim('aiesimulator_output/data/sig7_o.txt','int',1);
   end
   gld0_o = load_aiesim('data/sig0_o.txt','int',1);
   gld1_o = load_aiesim('data/sig1_o.txt','int',1);
   gld2_o = load_aiesim('data/sig2_o.txt','int',1);
   gld3_o = load_aiesim('data/sig3_o.txt','int',1);
   gld4_o = load_aiesim('data/sig4_o.txt','int',1);
   gld5_o = load_aiesim('data/sig5_o.txt','int',1);
   gld6_o = load_aiesim('data/sig6_o.txt','int',1);
   gld7_o = load_aiesim('data/sig7_o.txt','int',1);

   err0 = act0_o - gld0_o; 
   err1 = act1_o - gld1_o;
   err2 = act2_o - gld2_o;
   err3 = act3_o - gld3_o;
   err4 = act4_o - gld4_o;
   err5 = act5_o - gld5_o;
   err6 = act6_o - gld6_o;
   err7 = act7_o - gld7_o;

   if (1)
     subplot(4,4,1); plot(abs(gld0_o),'b.-'); hold on; plot(abs(act0_o),'r.--'); hold off; title('Stream 0');
     subplot(4,4,2); plot(abs(gld1_o),'b.-'); hold on; plot(abs(act1_o),'r.--'); hold off; title('Stream 1');
     subplot(4,4,3); plot(abs(gld2_o),'b.-'); hold on; plot(abs(act2_o),'r.--'); hold off; title('Stream 2');
     subplot(4,4,4); plot(abs(gld3_o),'b.-'); hold on; plot(abs(act3_o),'r.--'); hold off; title('Stream 3');
     subplot(4,4,5); plot(abs(gld4_o),'b.-'); hold on; plot(abs(act4_o),'r.--'); hold off; title('Stream 4');
     subplot(4,4,6); plot(abs(gld5_o),'b.-'); hold on; plot(abs(act5_o),'r.--'); hold off; title('Stream 5');
     subplot(4,4,7); plot(abs(gld6_o),'b.-'); hold on; plot(abs(act6_o),'r.--'); hold off; title('Stream 6');
     subplot(4,4,8); plot(abs(gld7_o),'b.-'); hold on; plot(abs(act7_o),'r.--'); hold off; title('Stream 7');
     subplot(4,4, 9); plot(abs(err0),'k.');  
     subplot(4,4,10); plot(abs(err1),'k.'); 
     subplot(4,4,11); plot(abs(err2),'k.'); 
     subplot(4,4,12); plot(abs(err3),'k.'); 
     subplot(4,4,13); plot(abs(err4),'k.'); 
     subplot(4,4,14); plot(abs(err5),'k.'); 
     subplot(4,4,15); plot(abs(err6),'k.'); 
     subplot(4,4,16); plot(abs(err7),'k.'); 
   end
   
   max_err = max(max(abs([real(err0);real(err1);real(err2);real(err3);real(err4);real(err5);real(err6);real(err7);...
                          imag(err0);imag(err1);imag(err2);imag(err3);imag(err4);imag(err5);imag(err6);imag(err7)])));
   fprintf(1,sprintf('Max Error: %g\n',max_err));

   if ( max_err <= 1 )
     fprintf(1,'--- PASSED ---\n');
   else
     fprintf(1,'*** FAILED ***\n');
   end
end

