In the Vitis™ core development flow, an application program is split between a host application and hardware accelerated kernels. The host application is developed in C/C++ with OpenCL™ API calls. The hardware kernels, on the other hand, can be developed in C/C++, OpenCL C, or RTL.

This tutorial is based on a C++ kernel that we'll optimize for highest throughput.

The algorithm to accelerate is a common linear algebra solver, the decomposition of a Hermitian, positive-definite matrix into the product of a lower triangular matrix and its conjugate transpose.  For this purpose we will use the Cholesky decomposition or Cholesky factorization (pronounced /ʃo-LESS-key/).  This solver is useful for several numerical problems, in particular for Monte Carlo simulations.
This solver is included as part the official Vitis accelerated libraries, here is a link to its documentation: https://xilinx.github.io/Vitis_Libraries/solver/guide_L2/L2_api.html#potrf

For our purpose, we will star with a straighforward description implemented in C++.

The souce code can be found here: ./src/reference.cpp
It can be run like so at the command line: g++ ....

Open the source code and get familiar with the algorithm.

Make a note of the perofrmance numbers for diffeent size of the matrices: 16, 64, 256...
These numbers will become references later during the tutorial.
