# How to run this design on CPU

```
g++ cpu_cholesky.cpp test.cpp matrixUtility.hpp -std=c++0x -O3 -o test
./test -runs 1 -M 512 -N 512 -seed 12
```
Try smaller sizes of matrices to gauge the impact on the execution time...
Study the code to see how it approaches and implement the algorithm.
