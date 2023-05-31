<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Hardware Acceleration Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

> **_In this Initial Module:_**
>
>1. Description of the Cholesky algorithm
>2. Running the algorithm on the CPU

### Algorithm Description

This tutorial is based on a C++ kernel that you will  optimize for the highest throughput.

The algorithm is a common linear algebra solver, the decomposition of a Hermitian, positive-definite matrix into the product of a lower triangular matrix and its conjugate transpose. For this purpose, you will use the Cholesky decomposition or Cholesky factorization (pronounced /ʃo-LESS-key/). This solver is useful for several numerical problems, in particular for Monte Carlo simulations.

This algorithm has a serial complexity O(n<sup>3</sup>).

![Cholesky description](../images/description.png)

[_More information on wikipedia..._](https://en.wikipedia.org/wiki/Cholesky_decomposition)
Note that this solver is included as part the official Vitis accelerated libraries; here is a [link](https://xilinx.github.io/Vitis_Libraries/solver/2022.1/guide_L2/L2_api.html#potrf) to its documentation.

For your purposes, you will start with a simple description implemented in C++ and explain how to adapt it for acceleration with an AMD Alveo™ U50 card.

### Run this Design on the CPU

Open a terminal and navigate to the `./docs/cpu_src` directory.

Run the following command to compile the design:

```
g++ cpu_cholesky.cpp test.cpp matrixUtility.hpp -std=c++0x -O3 -o test
./test -M 512 -N 512 -seed 12
```

Run with different sizes of matrices small and large (maximum size is 2048x2048) to gauge the impact on the execution time. Study the code to check how it implements the Cholesky algorithm.

> **NOTE 1:** If your server has limited stack size, this program might issue a segmentation fault at values smaller than 2048x2048.

#### Next

Continue to [**Module 1**](../module1_baseline/README.md) to build and run a baseline version with Vitis.

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
