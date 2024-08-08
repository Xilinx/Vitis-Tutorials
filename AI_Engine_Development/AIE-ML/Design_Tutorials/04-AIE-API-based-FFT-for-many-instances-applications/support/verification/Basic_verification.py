#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Donato Carlo Giorgio

import numpy as np
import matplotlib.pyplot as plt
from ml_dtypes import bfloat16
import os

####################################### FUNCTIONS DEFINITION #######################################
def read_data(filename, plio_width):

  with open(filename, 'r') as f:
    data = []
    for line in f:
      # Extract the value from the line
      values = line.strip().split(' ')
      data.append(values)

    final_data = np.empty(int(len(data)*plio_width), dtype=complex)

    for i in range(len(data)):
      for jj in range(2):
        final_data[2*i + jj] = complex(int(data[i][2*jj]),int(data[i][2*jj+1]))

  return final_data


def erase_T_lines_regex(filename):
  """
  Erases even lines from a text file using regex.
  """
  with open(filename, "r") as infile, open("temp.txt", "w") as outfile:
    for line in infile:
      if "T" not in line.strip("\n"):
        outfile.write(line)

  # Rename temporary file to original file
  os.replace("temp.txt", filename)

def reorder_IOs(io_list, points, plio_width, io_ilv):
  batch = plio_width*io_ilv
  final = []

  for inst in range(batch):
    for io in range(len(io_list)):
      res = np.empty(points, dtype=complex)
      for samp in range(points):
        res[samp] = io_list[io][samp*batch + inst]
      final.append(res)
  return final

####################################### FUNCTIONS DEFINITION #######################################

################################### BEGIN USER DEFINED VARIABLES ##################################

points = 1024
instances = 128
io_ilv = 4          # PL_clock/sample_rate
plio_width = 2      # Width of the PLIOs in terms of number of samples
repetitions = 3     # Argument of the graph .cpp run method
datatype = 2        # 0 = cfloat, 1 = cfloat16, 2 = cint16, 3 = cint32

################################### END USER DEFINED VARIABLES ###################################

if datatype == 0 or datatype == 1:
  dt_coef = 3.4e+38
elif datatype == 2:
  dt_coef = 2**15-1
elif datatype == 3:
  dt_coef = 2**31-1
else:
  print("Invalid datatype!\nValidation failed.")



new_file = 1 # Set if you have a new file from hw simulation to delete timing lines
N_IOs = int(instances/(plio_width*io_ilv))
AIE_in = []
AIE_out = []
for i in range(N_IOs):
  i_file = "v_inputs/PLIO_i["+str(i)+"].txt"
  o_file = "v_outputs/PLIO_o["+str(i)+"].txt"
  erase_T_lines_regex(o_file)
  AIE_in.append(read_data(i_file, plio_width))
  AIE_out.append(read_data(o_file, plio_width))


inputs = reorder_IOs(AIE_in, points, plio_width, io_ilv)
outputs_t = reorder_IOs(AIE_out, points, plio_width, io_ilv)

inputs_t = []
inputs_mod = []
outputs_mod = []
distances = []
for i in range(instances):
  inputs_t.append(np.fft.fft(inputs[i]))
  inputs_mod.append(np.abs(np.fft.fft(inputs[i])))
  outputs_mod.append(np.abs(outputs_t[i]))
  for j in range(points):
    if inputs_mod[i][j] >= dt_coef:
      inputs_mod[i][j] = dt_coef
    if outputs_mod[i][j] >= dt_coef:
      outputs_mod[i][j] = dt_coef
  distances.append(np.abs(inputs_mod[i]-outputs_mod[i]))

# Calculate corresponding frequencies
sample_rate_MHz = 125 #Sample rate
sample_rate_ns = 8
freq_axis = np.linspace(-sample_rate_MHz/2, sample_rate_MHz/2, points)

# Plot
plt.figure(figsize=(12, 6))
plt.plot(freq_axis, np.fft.fftshift(inputs_mod[0]), label='Numpy reference')
plt.plot(freq_axis, np.fft.fftshift(outputs_mod[0]), label='AIE results', linestyle='dashed')
plt.legend()
plt.xlabel('Frequency [Hz]')
plt.ylabel('Amplitude')
plt.title('FFT Comparison')
plt.grid()
plt.show()

# Plot
plt.figure(figsize=(12, 6))
plt.plot(np.arange(points), np.real(inputs)[0], label='Inputs')
plt.plot(np.arange(points), np.real(np.fft.ifft(outputs_t[0])), label='IFFT of AIE outputs', linestyle='dashed')
plt.legend()
plt.xlabel('Time [ns]')
plt.ylabel('Amplitude')
plt.title('IFFT Comparison')
plt.xticks(np.arange(0, 1025, 128), [str(tick*8) for tick in range(0,1025,128)])
plt.grid()
plt.show()

# Plot
plt.figure(figsize=(12, 6))
plt.plot(freq_axis, np.abs(np.fft.fftshift(distances[0]/inputs_mod[0])), label='absolute error')
plt.xlabel('Frequency [Hz]')
plt.ylabel('Magnitude')
plt.title('Absolute Error')
plt.grid()
plt.show()