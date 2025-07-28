#=========================================
  Copyright © 2025 Advanced Micro Devices
  SPDX-License-Identifier: MIT
==========================================#

debug = true    # comment out to disable debug messages

include("../../../contrived/julia/utilities.jl")

# !!! User Parameters !!! #
# !!! Make sure that they match "params.h" !!!
Arows = 4       # no. of rows in A
Acols = 4       # no. of columns in A
Bcols = 4       # no. of columns in B
burst_count = 8 # number of "samples" to process per function invocation
iter_count = 2  # no. of function invocations

# !!! End User Parameters !!! #

# !!! Derived Parameters !!! #
Brows = Acols                   # no. of rows in B
Prows = Arows                   # no. of rows in P = A * B
Pcols = Bcols                   # no. of columns in P = A * B
N = burst_count * iter_count    # total number of "samples" to generate

# !!! End Derived Parameters !!! #

using Random
Random.seed!(11)    # set seed for repeatability

# create the A and B matrices
# use Int8 values for the elements of A and B such that the product will fit into Int16
A = rand(Int8, (Arows, Acols, N))
A = convert.(Int16, A)
B = rand(Int8, (Brows, Bcols, N))
B = convert.(Int16, B)

if debug
    @printf("\n")
    @printf("A:\n")
    display(A)
    @printf("\n")
    max_A = maximum(A)
    @printf("Maximum A value: %d\n", max_A)
    min_A = minimum(A)
    @printf("Minimum A value: %d\n", min_A)
    @printf("\n")
    @printf("B:\n")
    display(B)
    @printf("\n")
    max_B = maximum(B)
    @printf("Maximum B value: %d\n", max_B)
    min_B = minimum(B)
    @printf("Minimum B value: %d\n", min_B)
    @printf("\n")
end # if debug

# calculate products
P = Array{Int16, 3}(undef, Prows, Pcols, N) # set size
for i = 1 : N
    P[:, :, i] = A[:, :, i ] * B[:, :, i]
end # for i = 1 : N

if debug
    @printf("P:\n")
    display(P)
    max_P = maximum(P)
    @printf("Maximum P value: %d\n", max_P)
    min_P = minimum(P)
    @printf("Minimum P value: %d\n", min_P)
    @printf("\n")    
end # if debug

# write results
# !!! Make sure that the PLIO width used matches that in src/graph.hpp !!!
arr2file(A, true, 64, "Ain.dat")
arr2file(B, true, 64, "Bin.dat")
arr2file(P, true, 64, "Pout.dat")

run(`mv Ain.dat ../data/.`) # move stimulus files to data directory
run(`mv Bin.dat ../data/.`)

