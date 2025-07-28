#====================================================================
  Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
  SPDX-License-Identifier: MIT
====================================================================#

debug = true    # comment out to disable debug messages

include("../../../contrived/julia/utilities.jl")

# !!! User Parameters !!! #
# !!! Make sure that they match "params.h" !!!
mrows = 4       # no. of rows in input matrices
mcols = 4       # no. of columns in input matrices
burst_count = 8 # number of "samples" to process per function invocation
iter_count = 2  # no. of function invocations

# !!! End User Parameters !!! #

# !!! Derived Parameters !!! #
N = burst_count * iter_count    # total number of "samples" to generate
vlen = mcols * 2				# input vector length

# !!! End Derived Parameters !!! #

using Random
Random.seed!(11)    # set seed for repeatability

# create input matrices
M_ul = rand(Int16, (mrows, mcols, N))	# upper left matrix
M_ll = rand(Int16, (mrows, mcols, N))	# lower left matrix
M_ur = rand(Int16, (mrows, mcols, N))	# upper right matrix
M_lr = rand(Int16, (mrows, mcols, N))	# lower right matrix

# create input vectors
vi = rand(Int16, (vlen, N))

# reserve space
M = Array{Int32, 3}(undef, mrows * 2, mcols * 2, N)
vo = Array{Int32, 2}(undef, vlen, N)

# concatenate input matrices and calculate product
for i = 1 : N
	ul = M_ul[:, :, i]
	ll = M_ll[:, :, i]
	ur = M_ur[:, :, i]
	lr = M_lr[:, :, i]
	M[:, :, i] = vcat(hcat(ul, ur), hcat(ll, lr))	# concatenate matrices
	vo[:, i] = M[:, :, i] * vi[:, i]				# calculate product
end # for i = 1 : N

# write results
# !!! make sure that the PLIO width used matches that in src/graph.hpp !!!
arr2file(M_ul, true, 64, "ul.dat")
arr2file(M_ll, true, 64, "ll.dat")
arr2file(M_ur, true, 64, "ur.dat")
arr2file(M_lr, true, 64, "lr.dat")
arr2file(vi, false, 64, "vi.dat")
arr2file(vo, false, 64, "vo.dat")

run(`mv ul.dat ../data/.`)	# move stimulus files to data directory
run(`mv ll.dat ../data/.`)
run(`mv ur.dat ../data/.`)
run(`mv lr.dat ../data/.`)
run(`mv vi.dat ../data/.`)

