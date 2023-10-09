%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function regression(x86)
  addpath('../common/matlab');

  if (x86==0)
    act0_o = load_aiesim('aiesimulator_output/data/sig0_o.txt','int',1);
    act1_o = load_aiesim('aiesimulator_output/data/sig1_o.txt','int',1);
    act2_o = load_aiesim('aiesimulator_output/data/sig2_o.txt','int',1);
    act3_o = load_aiesim('aiesimulator_output/data/sig3_o.txt','int',1);
    act4_o = load_aiesim('aiesimulator_output/data/sig4_o.txt','int',1);
    act5_o = load_aiesim('aiesimulator_output/data/sig5_o.txt','int',1);
    act6_o = load_aiesim('aiesimulator_output/data/sig6_o.txt','int',1);
    act7_o = load_aiesim('aiesimulator_output/data/sig7_o.txt','int',1);
  else
    act0_o = load_aiesim('x86simulator_output/data/sig0_o.txt','int',1);
    act1_o = load_aiesim('x86simulator_output/data/sig1_o.txt','int',1);
    act2_o = load_aiesim('x86simulator_output/data/sig2_o.txt','int',1);
    act3_o = load_aiesim('x86simulator_output/data/sig3_o.txt','int',1);
    act4_o = load_aiesim('x86simulator_output/data/sig4_o.txt','int',1);
    act5_o = load_aiesim('x86simulator_output/data/sig5_o.txt','int',1);
    act6_o = load_aiesim('x86simulator_output/data/sig6_o.txt','int',1);
    act7_o = load_aiesim('x86simulator_output/data/sig7_o.txt','int',1);
  end
  
  gld0_o = load_aiesim('data/sig0_o.txt','int',1);
  gld1_o = load_aiesim('data/sig1_o.txt','int',1);
  gld2_o = load_aiesim('data/sig2_o.txt','int',1);
  gld3_o = load_aiesim('data/sig3_o.txt','int',1);
  gld4_o = load_aiesim('data/sig4_o.txt','int',1);
  gld5_o = load_aiesim('data/sig5_o.txt','int',1);
  gld6_o = load_aiesim('data/sig6_o.txt','int',1);
  gld7_o = load_aiesim('data/sig7_o.txt','int',1);

  if (1)
    subplot(4,2,1); plot(abs(gld0_o),'b.-'); hold on; plot(abs(act0_o),'r.--'); hold off; title('Stream 0');
    subplot(4,2,2); plot(abs(gld1_o),'b.-'); hold on; plot(abs(act1_o),'r.--'); hold off; title('Stream 1');
    subplot(4,2,3); plot(abs(gld2_o),'b.-'); hold on; plot(abs(act2_o),'r.--'); hold off; title('Stream 2');
    subplot(4,2,4); plot(abs(gld3_o),'b.-'); hold on; plot(abs(act3_o),'r.--'); hold off; title('Stream 3');
    subplot(4,2,5); plot(abs(gld4_o),'b.-'); hold on; plot(abs(act4_o),'r.--'); hold off; title('Stream 4');
    subplot(4,2,6); plot(abs(gld5_o),'b.-'); hold on; plot(abs(act5_o),'r.--'); hold off; title('Stream 5');
    subplot(4,2,7); plot(abs(gld6_o),'b.-'); hold on; plot(abs(act6_o),'r.--'); hold off; title('Stream 6');
    subplot(4,2,8); plot(abs(gld7_o),'b.-'); hold on; plot(abs(act7_o),'r.--'); hold off; title('Stream 7');
  end
  err0 = act0_o - gld0_o(1:numel(act0_o));
  err1 = act1_o - gld1_o(1:numel(act1_o));
  err2 = act2_o - gld2_o(1:numel(act2_o));
  err3 = act3_o - gld3_o(1:numel(act3_o));
  err4 = act4_o - gld4_o(1:numel(act4_o));
  err5 = act5_o - gld5_o(1:numel(act5_o));
  err6 = act6_o - gld6_o(1:numel(act6_o));
  err7 = act7_o - gld7_o(1:numel(act7_o));

  max_err = max([abs(real(err0)),abs(imag(err0)),...
                 abs(real(err1)),abs(imag(err1)),...
                 abs(real(err2)),abs(imag(err2)),...
                 abs(real(err3)),abs(imag(err3)),...
                 abs(real(err4)),abs(imag(err4)),...
                 abs(real(err5)),abs(imag(err5)),...
                 abs(real(err6)),abs(imag(err6)),...
                 abs(real(err7)),abs(imag(err7))]);
  fprintf(1,'Max Err: %g\n',max_err);
  
  if ( max(abs(real(err0))) <= 1 && max(abs(imag(err0))) <= 1 && ...
       max(abs(real(err1))) <= 1 && max(abs(imag(err1))) <= 1 && ...
       max(abs(real(err2))) <= 1 && max(abs(imag(err2))) <= 1 && ...
       max(abs(real(err3))) <= 1 && max(abs(imag(err3))) <= 1 && ...
       max(abs(real(err4))) <= 1 && max(abs(imag(err4))) <= 1 && ...
       max(abs(real(err5))) <= 1 && max(abs(imag(err5))) <= 1 && ...
       max(abs(real(err6))) <= 1 && max(abs(imag(err6))) <= 1 && ...
       max(abs(real(err7))) <= 1 && max(abs(imag(err7))) <= 1 )
    fprintf(1,'--- PASSED ---\n');
  else
    fprintf(1,'*** FAILED ***\n');
  end
end
