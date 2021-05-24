%% Copyright 2021 Xilinx Inc.
% Licensed under the Apache License, Version 2.0 (the "License");
% you may not use this file except in compliance with the License.
% You may obtain a copy of the License at
%
%    http://www.apache.org/licenses/LICENSE-2.0
%
% Unless required by applicable law or agreed to in writing, software
% distributed under the License is distributed on an "AS IS" BASIS,
% WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
% See the License for the specific language governing permissions and
% limitations under the License.
%
% Author:    Daniele Bagni
% date:      26 Apr. 2021

%%
clc;
clear all;
close all;

 %% ref/DPU input[0] from TANH

  ref_tanh_data_inp_0 = load("ref_tanh_data_inp.bin_0.csv", "-ascii");
      tanh_data_inp_0 = load(    "tanh_data_inp.bin_0.csv", "-ascii");
 diff_tanh_data_inp_0 = abs(tanh_data_inp_0 - ref_tanh_data_inp_0);
 figure(1)
 hold on
 plot(     tanh_data_inp_0(:), 'g')
 plot( ref_tanh_data_inp_0(:), 'k')
 plot(diff_tanh_data_inp_0(:), 'r')
 grid
 title "tanh inp[0]"
 hold off

 %% ref/DPU input[7] from TANH

 figure(2)
  ref_tanh_data_inp_7 = load("ref_tanh_data_inp.bin_7.csv", "-ascii");
      tanh_data_inp_7 = load(    "tanh_data_inp.bin_7.csv", "-ascii");
 diff_tanh_data_inp_7 = abs(tanh_data_inp_7 - ref_tanh_data_inp_7);
 hold on
 plot(     tanh_data_inp_7(:), 'g')
 plot( ref_tanh_data_inp_7(:), 'k')
 plot(diff_tanh_data_inp_7(:), 'r')
 grid
 title "tanh inp[7]"
 hold off

 %% ref/DPU input[15] from TANH

 figure(3)
  ref_tanh_data_inp_15 = load("ref_tanh_data_inp.bin_15.csv", "-ascii");
      tanh_data_inp_15 = load(    "tanh_data_inp.bin_15.csv", "-ascii");
 diff_tanh_data_inp_15 = abs(tanh_data_inp_15 - ref_tanh_data_inp_15);
 hold on
 plot(     tanh_data_inp_15(:), 'g')
 plot( ref_tanh_data_inp_15(:), 'k')
 plot(diff_tanh_data_inp_15(:), 'r')
 grid
 title "tanh inp[15]"
 hold off

 %% ref/DPU output[0] from TANH

 figure(4)
  ref_tanh_data_out_0 = load("ref_tanh_data_out.bin_0.csv", "-ascii");
      tanh_data_out_0 = load(    "tanh_data_out.bin_0.csv", "-ascii");
 diff_tanh_data_out_0 = abs(tanh_data_out_0 - ref_tanh_data_out_0);
 hold on
 plot(     tanh_data_out_0(:), 'g')
 plot( ref_tanh_data_out_0(:), 'k')
 plot(diff_tanh_data_out_0(:), 'r')
 grid
 title "tanh out[0]"
 hold off

 %% ref/DPU output[7] from TANH

 figure(5)
  ref_tanh_data_out_7 = load("ref_tanh_data_out.bin_7.csv", "-ascii");
      tanh_data_out_7 = load(    "tanh_data_out.bin_7.csv", "-ascii");
 diff_tanh_data_out_7 = abs(tanh_data_out_7 - ref_tanh_data_out_7);
 hold on
 plot(     tanh_data_out_7(:), 'g')
 plot( ref_tanh_data_out_7(:), 'k')
 plot(diff_tanh_data_out_7(:), 'r')
 grid
 title "tanh out[7]"
 hold off

 %% ref/DPU output[15] from TANH

 figure(6)
  ref_tanh_data_out_15 = load("ref_tanh_data_out.bin_15.csv", "-ascii");
      tanh_data_out_15 = load(    "tanh_data_out.bin_15.csv", "-ascii");
 diff_tanh_data_out_15 = abs(tanh_data_out_15 - ref_tanh_data_out_15);
 hold on
 plot(     tanh_data_out_15(:), 'g')
 plot( ref_tanh_data_out_15(:), 'k')
 plot(diff_tanh_data_out_15(:), 'r')
 grid
 title "tanh out[15]"
 hold off


%% ref/DPU input[0] from SIGM1

figure(7)
 ref_sigm1_data_inp_0 = load("ref_sigm1_data_inp.bin_0.csv", "-ascii");
     sigm1_data_inp_0 = load(    "sigm1_data_inp.bin_0.csv", "-ascii");
diff_sigm1_data_inp_0 = abs(sigm1_data_inp_0 - ref_sigm1_data_inp_0);
hold on
plot(     sigm1_data_inp_0(:), 'g')
plot( ref_sigm1_data_inp_0(:), 'k')
plot(diff_sigm1_data_inp_0(:), 'r')
grid
title "sigm1 inp[0]"
hold off

%% ref/DPU input[7] from SIGM1

figure(8)
 ref_sigm1_data_inp_7 = load("ref_sigm1_data_inp.bin_7.csv", "-ascii");
     sigm1_data_inp_7 = load(    "sigm1_data_inp.bin_7.csv", "-ascii");
diff_sigm1_data_inp_7 = abs(sigm1_data_inp_7 - ref_sigm1_data_inp_7);
hold on
plot(     sigm1_data_inp_7(:), 'g')
plot( ref_sigm1_data_inp_7(:), 'k')
plot(diff_sigm1_data_inp_7(:), 'r')
grid
title "sigm1 inp[7]"
hold off

%% ref/DPU output[0] from SIGM1

figure(9)
 ref_sigm1_data_out_0 = load("ref_sigm1_data_out.bin_0.csv", "-ascii");
     sigm1_data_out_0 = load(    "sigm1_data_out.bin_0.csv", "-ascii");
diff_sigm1_data_out_0 = abs(sigm1_data_out_0 - ref_sigm1_data_out_0);
hold on
plot(     sigm1_data_out_0(:), 'g')
plot( ref_sigm1_data_out_0(:), 'k')
plot(diff_sigm1_data_out_0(:), 'r')
grid
title "sigm1 out[0]"
hold off

%% ref/DPU output[7] from SIGM1

figure(10)
 ref_sigm1_data_out_7 = load("ref_sigm1_data_out.bin_7.csv", "-ascii");
     sigm1_data_out_7 = load(    "sigm1_data_out.bin_7.csv", "-ascii");
diff_sigm1_data_out_7 = abs(sigm1_data_out_7 - ref_sigm1_data_out_7);
hold on
plot(     sigm1_data_out_7(:), 'g')
plot( ref_sigm1_data_out_7(:), 'k')
plot(diff_sigm1_data_out_7(:), 'r')
grid
title "sigm1 out[7]"
hold off

%% ref/DPU input from SIGM2

figure(11)
 ref_sigm2_data_inp = load("ref_sigm2_data_inp.bin.csv", "-ascii");
     sigm2_data_inp = load(    "sigm2_data_inp.bin.csv", "-ascii");
diff_sigm2_data_inp = abs(sigm2_data_inp - ref_sigm2_data_inp);
hold on
plot(     sigm2_data_inp(:), 'g')
plot( ref_sigm2_data_inp(:), 'k')
plot(diff_sigm2_data_inp(:), 'r')
grid
title "sigm2 inp"
hold off


%% ref/DPU output from sigm2

figure(12)
 ref_sigm2_data_out = load("ref_sigm2_data_out.bin.csv", "-ascii");
     sigm2_data_out = load(    "sigm2_data_out.bin.csv", "-ascii");
diff_sigm2_data_out = abs(sigm2_data_out - ref_sigm2_data_out);
hold on
plot(     sigm2_data_out(:), 'g')
plot( ref_sigm2_data_out(:), 'k')
plot(diff_sigm2_data_out(:), 'r')
grid
title "sigm2 out"
hold off

%% ref output from CNN
figure(13)
ref_out = load("ref_cnn_out.bin.csv", "-ascii");
stem(ref_out(:), '*r')
grid
title "ref out"


%% FC2 layer weights and bias
fc2_w =  load("fc2_weight_transp.csv", "-ascii")
fc2_b =  load("fc2_bias.csv", "-ascii")

x = ref_sigm2_data_out
y = x * fc2_w + fc2_b'

check = y - ref_out;
sum_abs_check = sum(abs(check(:)))
