%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;

% ------------------------------------------------------------
% Create stimulus for AI Engine
% ------------------------------------------------------------

Niter = 8;
NSAMP = 1024;

sig_i = reshape(repmat(transpose(linspace(-16,+16,NSAMP)),[1,Niter]),1,[]);
sig_i = double(varray.bfloat16(sig_i));
sig_o = selu(sig_i);
sig_o = double(varray.bfloat16(sig_o));

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

fid = fopen('data/sig_i.txt','w');
for ii = 1 : 4 : numel(sig_i)
  fprintf(fid,'%.7e %.7e %.7e %.7e\n',sig_i(ii),sig_i(ii+1),sig_i(ii+2),sig_i(ii+3));
end
fclose(fid);

fid = fopen('data/sig_o.txt','w');
for ii = 1 : 4 : numel(sig_o)
  fprintf(fid,'%.7e %.7e %.7e %.7e\n',sig_o(ii),sig_o(ii+1),sig_o(ii+2),sig_o(ii+3));
end
fclose(fid);

% ------------------------------------------------------------
% Vitis Functional Simulation
% ------------------------------------------------------------

vfs_model = vfs.aieGraph(input_file="selu_app.cpp",...
                         include_paths = { '.' },...
                         part="xc2ve3858-ssva2112-2MP-e-S");

vfs_i = varray.bfloat16(sig_i);
vfs_o = vfs_model.run(vfs_i);
vfs_o = reshape(vfs_o,1,numel(vfs_o));
err_o = sig_o - double(vfs_o);

figure;
subplot(2,1,1); plot(sig_o,'b.-'); hold on; plot(double(vfs_o),'r.--'); hold off;legend('golden','actual');
subplot(2,2,3); plot(err_o,'k.-');title('error');


% ------------------------------------------------------------
% Function SELU
% ------------------------------------------------------------

function [sig_o] = selu(sig_i)
   idx_p = find(sig_i  > 0);
   idx_n = find(sig_i <= 0);
   sig_o = zeros(size(sig_i));
   scale = 1.05070098;
   alpha = 1.67326324;
   sig_o(idx_p) = scale*sig_i(idx_p);
   sig_o(idx_n) = scale*alpha*(exp(sig_i(idx_n))-1);
end
