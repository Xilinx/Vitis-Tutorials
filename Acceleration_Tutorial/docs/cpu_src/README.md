# Run this design on CPU

Open a terminal and navigate to the <code>./docs/cpu_src</code> directory.

Run the following command to compile the design:
```
g++ cpu_cholesky.cpp test.cpp matrixUtility.hpp -std=c++0x -O3 -o test
./test -runs 1 -M 512 -N 512 -seed 12
```
Try smaller sizes of matrices to gauge the impact on the execution time...
Study the code to see how it implements the Cholesky algorithm.
