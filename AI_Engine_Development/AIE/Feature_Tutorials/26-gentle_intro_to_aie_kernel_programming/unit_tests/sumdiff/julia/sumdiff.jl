#====================================================================
  Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
  SPDX-License-Identifier: MIT
====================================================================#

debug = true    # comment out to disable debug messages

include("../../../contrived/julia/utilities.jl")

# !!! User Parameters !!! #
# !!! Make sure that they match "params.h" !!!
vlen = 8        # vector length
burst_count = 8 # number of "samples" to process per function invocation
iter_count = 2  # no. of function invocations

# !!! End User Parameters !!! #

# !!! Derived Parameters !!! #
N = burst_count * iter_count    # total number of "samples" to generate

# !!! End Derived Parameters !!! #

using Random
Random.seed!(11)    # set seed for repeatability

# create input vectors
x = rand(Int32, (vlen, N))
y = rand(Int32, (vlen, N))

# create space for result
z = Array{Int32, 2}(undef, vlen, N)

# 1st half contains the sum
z[:, (1 : N ÷ 2)] = x[:, (1 : N ÷ 2)] + y[:, (1 : N ÷ 2)]

# 2nd half contains the difference
z[:, (N ÷ 2 + 1 : N)] = x[:, (N ÷ 2 + 1 : N)] - y[:, (N ÷ 2 + 1 : N)]

# write results
arr2file(x, false, 64, "x.dat")
arr2file(y, false, 64, "y.dat")
arr2file(z, false, 64, "z.dat")

run(`mv x.dat ../data/.`) # move stimulus files to data directory
run(`mv y.dat ../data/.`)  
