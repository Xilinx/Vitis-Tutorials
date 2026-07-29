#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#
# Author Derek Hagen

import varray as va
import vfs
import numpy as np
import argparse as argp
import matplotlib.pyplot as plt

my_parser = argp.ArgumentParser()
my_parser.add_argument("-s", "--size",    help="Size of input data in samples, default=8", type=int, default=8)
my_parser.add_argument("-p", "--plot",    help="Visualize using pyplot", action="store_true")
my_parser.add_argument("-e", "--error",   help="Inject error in golden model to trigger error handling", action="store_true")
my_parser.add_argument("-k", "--kernel",  help="Selects which VADD HLS IP to test, currently only 'vadd_s' (default) is supported", default="vadd_s")
my_parser.add_argument("-d", "--debug",   help="Enable extra debugging", action="store_true")
my_args = my_parser.parse_args()

# Prepare data types and input vectors
buf_size = my_args.size
n_shift = 14
N = 2**n_shift
Nlo = -2**n_shift
Nhi = 2**n_shift-1
#buf_size = 256

in0 = np.random.randint(low=Nlo, high=Nhi, size=buf_size, dtype=np.int16) + 1j*np.random.randint(low=Nlo, high=Nhi, size=buf_size, dtype=np.int16)
in1 = np.random.randint(low=Nlo, high=Nhi, size=buf_size, dtype=np.int16) + 1j*np.random.randint(low=Nlo, high=Nhi, size=buf_size, dtype=np.int16)

# For debugging the testbench, inject "wrong" data to golden model to check failure detection
if my_args.error:
  print("Injecting new random numbers to golden model to force error")
  golden = np.random.randint(low=Nlo, high=Nhi, size=buf_size, dtype=np.int16) + 1j*np.random.randint(low=Nlo, high=Nhi, size=buf_size, dtype=np.int16)
else:
  golden = np.add(in0, in1)

# Declare the VFS HLS object
my_vadd = vfs.hlsKernel(input_files = '../ip/'+my_args.kernel+'/src/'+my_args.kernel+'.cpp', hls_function = my_args.kernel)

# When debugging is enabled, the kernel input/output interface info is printed
if my_args.debug:
  print("\nDebug enabled, printing input and output interface info")
  print(my_vadd.getInputSpec())
  print(my_vadd.getOutputSpec())

# Simulate the VFS object with the input stimuli
out0 = my_vadd.run(va.array(in0, dtype='cint16'), va.array(in1, dtype='cint16'))
out0np = np.asarray(out0)

# For small vectors print them for visual inspection and validation
if buf_size <= 8:
  print('\nSmall data set, adding prints for visual confirmation of results')
  print('input 0')
  print(in0)
  print('\ninput 1')
  print(in1)
  print('\ngolden')
  print(golden)
  print('\nout0')
  print(out0np)
  #print(np.asarray(out0))
else:
  print('\nLarge data sets, skipping print outputs')

# Add some visualization of the results using matplotlib
if my_args.plot:
  t = np.arange(buf_size)
  err0 = np.subtract(out0np, golden)
  fig, axs = plt.subplots(3, 1, layout='constrained')
  axs[0].plot(t, in0.real,  'b', label=my_args.kernel+'_in0.real')
  axs[0].plot(t, in1.real,  'r', label=my_args.kernel+'_in1.real')
  axs[0].plot(t, out0np.real, 'g', label=my_args.kernel+'_out0.real')
  axs[1].plot(t, in0.imag,  'b', label=my_args.kernel+'_in0.imag')
  axs[1].plot(t, in1.imag,  'r', label=my_args.kernel+'_in1.imag')
  axs[1].plot(t, out0np.imag, 'g', label=my_args.kernel+'_out0.imag')
  axs[2].plot(t, err0.real, 'b', label=my_args.kernel+'_err.real')
  axs[2].plot(t, err0.imag, 'r', label=my_args.kernel+'_err.imag')
  axs[0].set_ylabel('Real')
  axs[1].set_ylabel('Imag')
  axs[2].set_ylabel('Error')
  axs[0].legend()
  axs[1].legend()
  axs[2].legend()
  plt.show()

# Compare output with golden model and raise error if they diverge
if np.array_equal(out0np, golden):
  print("\n'vadd_s' output matches golden")
else:
  raise ValueError("\n'vadd_s' differ from golden")

