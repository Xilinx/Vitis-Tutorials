#====================================================================
  Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
  SPDX-License-Identifier: MIT
====================================================================#

debug = true    # comment out to disable debug messages

include("../../../contrived/julia/utilities.jl")

# !!! User Parameters !!! #
# !!! Make sure that they match "src/params.h" !!!

vlen = 8     	# vector length
burst_count = 8 # number of "samples" to process per function invocation
iter_count = 2  # no. of function invocations

# !!! End User Parameters !!! #

# !!! Derived Parameters !!! #
N = burst_count * iter_count    # total number of "samples" to generate

# !!! End Derived Parameters !!! #

using Random
Random.seed!(11)    # set seed for repeatability

# create the complex vector
# !!! N.B.: Julia does "no automatic promotion" !!!
x = rand(Complex{Int8}, (vlen, N))	# column-major
x = convert.(Complex{Int16}, x)		# limit the elements to Int8 values

if debug
    @printf("\n")
    @printf("Input vector:\n")
    display(x)
    @printf("\n")
    max_x_re = maximum(real(x))
    max_x_im = maximum(imag(x))
    max_x = (max_x_re > max_x_im) ? max_x_re : max_x_im
    @printf("Maximum x value: %d\n", max_x)
    min_x_re = minimum(real(x))
    min_x_im = minimum(imag(x))
    min_x = (min_x_re < min_x_im) ? min_x_re : min_x_im
    @printf("Minimum x value: %d\n", min_x)
    @printf("\n")
end # if debug

# calculate the squared magnitude
y = abs2.(x)

if debug
    @printf("Output vector:\n")
    display(y)
    @printf("\n")
    max_y = maximum(y)
    min_y = minimum(y)
    @printf("Maximum y value: %d\n", max_y)
    @printf("Minimum y value: %d\n", min_y)
    @printf("\n")
end # if debug

# write results
# !!! Make sure that the PLIO width used matches that in graph.hpp !!!
arr2file(x, false, 64, "ivec.dat")	# AIE expects row-major
arr2file(y, false, 64, "ovec.dat")

run(`mv ivec.dat ../data/.`)	# move "ivec.dat" to the data directory

