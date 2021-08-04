#!/usr/bin/env python
# coding: utf-8

"""
Copyright 2021 Xilinx Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

"""
Author:    Daniele Bagni
date:      26 Apr. 2021
"""

from scipy.io import savemat
import numpy as np
np.set_printoptions(precision=10)
import glob
import os

print("START")

ref_file_list = glob.glob("ref_*.npy")
file_list = []
for filename in ref_file_list:
    new_filename=filename.split("ref_")[1]
    file_list.append(new_filename)
print(ref_file_list)
print(    file_list)

ref_data = []
data = []
diff = []
info = [ ]
for i in range(0, len(ref_file_list)):
    fnr = ref_file_list[i]
    ref_d = np.load(fnr)
    print(fnr, ref_d.shape)
    ref_data.append(ref_d)
    fn = file_list[i]
    d = np.load(fn)
    print(fn, d.shape)
    data.append(d)
    tmp_diff = ref_d - d
    diff.append(tmp_diff)
    abs_diff = np.absolute(tmp_diff)
    diff_min = tmp_diff.min()
    diff_max = tmp_diff.max()
    tmp_info = [abs_diff, diff_min, diff_max]
    info.append(tmp_info)
    print("diff     min ", diff_min)
    print("diff     max ", diff_max)
    sz = ref_d.shape
    if len(sz)==3:
        for k in range(0, sz[2]):
            fmr = os.path.splitext(fnr)[0] + "_" + str(k) + ".csv"
            ref_dk = ref_d[:,:,k]
            np.savetxt(fmr, ref_dk)
            fm = os.path.splitext(fn)[0] + "_" + str(k) + ".csv"
            dk = d[:,:,k]
            np.savetxt(fm, dk)
        sum_abs_diff = abs_diff.sum()/(sz[0]*sz[1]*sz[2])
    else:
        fmr = os.path.splitext(fnr)[0] + ".csv"
        np.savetxt(fmr, ref_d)
        fm = os.path.splitext(fn)[0] + ".csv"
        np.savetxt(fm, d)
        sum_abs_diff = abs_diff.sum()/(sz[0]*sz[1])

    print("sum_abs_diff ", sum_abs_diff)

fc2_w = np.load("fc2_weight.bin.npy")
fc2_b = np.load("fc2_bias.bin.npy")
wt = np.transpose(fc2_w,(1,0))
print("weight Transposed = ")
print(wt)
print("bias = ", fc2_b)
np.savetxt("fc2_weight_transp.csv", wt)
np.savetxt("fc2_bias.csv",   fc2_b)

print("END")


"""
tanh_inp     = np.load(    "tanh_data_inp.bin.npy")
ref_tanh_inp = np.load("ref_tanh_data_inp.bin.npy")

print(ref_tanh_inp[0].shape)
print(ref_tanh_inp[0][0].shape)

diff_0     = ref_tanh_inp[0][0] - tanh_inp[:,:,0]
abs_diff_0 = np.absolute(diff_0)
sum_abs_diff_0 = abs_diff_0.sum()

print("sum_abs_diff_0 ", sum_abs_diff_0/(32*32) )
print("diff_0 min ",     diff_0.min())
print("diff_0 max ",     diff_0.max())
"""
