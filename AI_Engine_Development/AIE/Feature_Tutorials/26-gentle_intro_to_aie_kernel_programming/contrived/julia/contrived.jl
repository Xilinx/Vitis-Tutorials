#====================================================================
  Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
  SPDX-License-Identifier: MIT
====================================================================#

debug = true    # comment out to disable debug messages

include("utilities.jl")

# !!! User Parameters !!! #
# !!! Make sure that they match "src/params.h" !!!

vlen = 8     	# vector length
mrows = 4       # no. of rows in input matrix
mcols = 4       # no. of columns in input matrix
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

# create input matrices
A = rand(Int8, (mrows, mcols, N))
A = convert.(Int16, A)

B = rand(Int8, (mrows, mcols, N))
B = convert.(Int16, B)

C = rand(Int8, (mrows, mcols, N))
C = convert.(Int16, C)

D = rand(Int8, (mrows, mcols, N))
D = convert.(Int16, D)

E = rand(Int8, (mrows, mcols, N))
E = convert.(Int16, E)

F = rand(Int8, (mrows, mcols, N))
F = convert.(Int16, F)

# create the y vector
y = rand(Int32, (vlen, N))	# column-major

x2 = abs2.(x)   # squared magnitude of x

# initialize products
AC = Array{Int16, 3}(undef, mrows, mcols, N)
BC = Array{Int16, 3}(undef, mrows, mcols, N)
DF = Array{Int16, 3}(undef, mrows, mcols, N)
EF = Array{Int16, 3}(undef, mrows, mcols, N)
M  = Array{Int32, 3}(undef, (mrows * 2), (mcols * 2), N)
u  = Array{Int32, 2}(undef, vlen, N) 
z  = Array{Int32, 2}(undef, vlen, N) 

for i = 1 : N 
    # calculate the matrix products
    AC[:, :, i] = A[:, :, i] * C[:, :, i]
    BC[:, :, i] = B[:, :, i] * C[:, :, i]
    DF[:, :, i] = D[:, :, i] * F[:, :, i]
    EF[:, :, i] = E[:, :, i] * F[:, :, i]

    M[:, :, i]  = vcat(hcat(AC[:, :, i], DF[:, :, i]), hcat(BC[:, :, i], EF[:, :, i]))  # concatenate
    u[:, i] = M[:, :, i] * x2[:, i]                 # matrix-vector product

    if (i <= N ÷ 2)
        z[:, i] = u[:, i] + y[:, i]
    else
        z[:, i] = u[:, i] - y[:, i]
    end # if-else (i <= N ÷ 2)

    if (debug)
        @printf("x[%d]:\n", i)
        display(x[:, i])
        @printf("\n")
        @printf("A[:, :, %d]:\n", i)
        display(A[:, :, i])
        @printf("\n")
        @printf("B[:, :, %d]:\n", i)
        display(B[:, :, i])
        @printf("\n")
        @printf("C[:, :, %d]:\n", i)
        display(C[:, :, i])
        @printf("\n")
        @printf("D[:, :, %d]:\n", i)
        display(D[:, :, i])
        @printf("\n")
        @printf("E[:, :, %d]:\n", i)
        display(E[:, :, i])
        @printf("\n")
        @printf("F[:, :, %d]:\n", i)
        display(F[:, :, i])
        @printf("\n")
        @printf("x2[%d]\n", i)
        display(x2[:, i])
        @printf("\n")
        @printf("AC[:, :, %d]:\n", i)
        display(AC[:, :, i])
        @printf("\n")
        @printf("BC[:, :, %d]:\n", i)
        display(BC[:, :, i])
        @printf("\n")
        @printf("DF[:, :, %d]:\n", i)
        display(DF[:, :, i])
        @printf("\n")
        @printf("EF[:, :, %d]:\n", i)
        display(EF[:, :, i])
        @printf("\n")
        @printf("M[:, :, %d]:\n", i)
        display(M[:, :, i])
        @printf("\n")
        @printf("u[:, %d]\n", i)
        display(u[:, i])
        @printf("\n")
        @printf("y[:, %d]:\n", i)
        display(y[:, i])
        @printf("\n")
        @printf("z[:, %d]\n", i)
        display(z[:, i])
        @printf("\n")
        @printf("********************************************\n")

    end # if (debug)
end # for i = 1 : N     

# write results
# !!! Make sure that the PLIO width used matches that in graph.hpp !!!
arr2file(x, false, 64, "x.dat")
arr2file(y, false, 64, "y.dat")
arr2file(A, true, 64, "A.dat")
arr2file(B, true, 64, "B.dat")
arr2file(C, true, 64, "C.dat")
arr2file(D, true, 64, "D.dat")
arr2file(E, true, 64, "E.dat")
arr2file(F, true, 64, "F.dat")
arr2file(z, false, 64, "z.dat")

# move stimulus files to data directory
run(`mv x.dat ../data/.`)
run(`mv y.dat ../data/.`)
run(`mv A.dat ../data/.`)
run(`mv B.dat ../data/.`)
run(`mv C.dat ../data/.`)
run(`mv D.dat ../data/.`)
run(`mv E.dat ../data/.`)
run(`mv F.dat ../data/.`)

