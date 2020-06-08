In the Vitis™ core development flow, an application program is split between a host application and hardware accelerated kernels. The host application is developed in C/C++ with OpenCL™ API calls. The hardware kernels, on the other hand, can be developed in C/C++, OpenCL C, or RTL.

In this tutorial we'll use a C++ kernel.

The algorithm we will accelerate is a common linear algebra solver, the decomposition of a Hermitian, positive-definite matrix into the product of a lower triangular matrix and its conjugate transpose.  For this purpose we will use the Cholesky decomposition or Cholesky factorization (pronounced /ʃo-LESS-key/).  This solver is useful for several numerical problems, in particular for Monte Carlo simulations.
This solver is in fact already included as part the Vitis accelerated libraries, here is a link to its documentation:

For our purpose, we will star with a straighforward description implemented in C++.

The souce code can be found here: ./src/reference.cpp
It can be run like so at the command line: g++ ....

Open the source code and get familiar with the algorithm.

Make a note of the perofrmance numbers for diffeent size of the matrices: 16, 64, 256...
We'll use these numbers as references later during the tutorial.
