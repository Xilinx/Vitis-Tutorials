</table>
<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Matrix Multiplication Compute Performance of the AI Engin ML Tiles

This table summarizes IO/Compute balance for all supported input data types and matrix sizes:

- Mat A Type: number of bits of Matrix A data type. `bf16`is 16 bit wide.
- Mat B Type: number of bits of Matrix A data type. `bf16`is 16 bit wide.
- Compute (MAC/cyc): number of parallel multiplication-accumulations that can be performed by the vector processor of the AI Engine ML
- M: number of rows of matrix A
- K: number of columns of matrix A = number of rows of matrix B
- N: number of columns of matrix B
- Mat A Size B): number of bytes used to encode matrix A
- Mat B Size (B): number of bytes used to encode matrix B
- Load Mat A (cyc): number of cycles necessary to load Matrix A
- Load Mat B (cyc): number of cycles necessary to load Matrix B
- Compute (cyc): Number of cycles necessary to compute the matrix multiplication
- Compute (%): Efficiency of the vector processor usage compared to the max of IO and compute burden
- IO A (%): Efficiency of the matrix A load  compared to the the max IO and compute burden
- IO B (%): Efficiency of the matrix B load  compared to the the max IO and compute burden


| Mat A Type   | Mat B Type   |     Compute (MAC/cyc) |   M |   K |   N |   Mat A Size (B) |   Mat B Size (B) |   Load Mat A (cyc) |   Load Mat B (cyc) |   Compute (cyc) |   Compute (%) |   IO A % |   IO B % |
|:-------------|:-------------|----------------------:|----------------:|------------:|----------------:|-------------:|-------------:|-------------:|-------------:|----------:|----------:|-------:|-------:|
| 8b           | 4b           |                   512 |               4 |          16 |               8 |           64 |           64 |          2   |          2   |       1   |      0.5  |  1     |   1    |
| 8b           | 4b           |                   512 |               8 |          16 |               8 |          128 |           64 |          4   |          2   |       2   |      0.5  |  1     |   0.5  |
| 8b           | 4b           |                   512 |               4 |          32 |               8 |          128 |          128 |          4   |          4   |       2   |      0.5  |  1     |   1    |
| 8b           | 8b           |                   256 |               4 |           8 |               4 |           32 |           32 |          1   |          1   |       0.5 |      0.5  |  1     |   1    |
| 8b           | 8b           |                   256 |               4 |          16 |               4 |           64 |           64 |          2   |          2   |       1   |      0.5  |  1     |   1    |
| 8b           | 8b           |                   256 |               8 |           8 |               4 |           64 |           32 |          2   |          1   |       1   |      0.5  |  1     |   0.5  |
| 8b           | 8b           |                   256 |               2 |           8 |               8 |           16 |           64 |          0.5 |          2   |       0.5 |      0.25 |  0.25  |   1    |
| 8b           | 8b           |                   256 |               4 |           8 |               8 |           32 |           64 |          1   |          2   |       1   |      0.5  |  0.5   |   1    |
| 8b           | 8b           |                   256 |               2 |          16 |               8 |           32 |          128 |          1   |          4   |       1   |      0.25 |  0.25  |   1    |
| 8b           | 8b           |                   256 |               4 |          16 |               8 |           64 |          128 |          2   |          4   |       2   |      0.5  |  0.5   |   1    |
| 16b          | 8b           |                   128 |               4 |           4 |               4 |           32 |           16 |          1   |          0.5 |       0.5 |      0.5  |  1     |   0.5  |
| 16b          | 8b           |                   128 |               8 |           4 |               4 |           64 |           16 |          2   |          0.5 |       1   |      0.5  |  1     |   0.25 |
| 16b          | 8b           |                   128 |               4 |           8 |               4 |           64 |           32 |          2   |          1   |       1   |      0.5  |  1     |   0.5  |
| 16b          | 8b           |                   128 |               4 |           4 |               8 |           32 |           32 |          1   |          1   |       1   |      1    |  1     |   1    |
| 8b           | 16b          |                   128 |               4 |           4 |               8 |           16 |           64 |          0.5 |          2   |       1   |      0.5  |  0.25  |   1    |
| 8b           | 16b          |                   128 |               4 |           4 |               4 |           16 |           32 |          0.5 |          1   |       0.5 |      0.5  |  0.5   |   1    |
| 16b          | 16b          |                    64 |               4 |           4 |               4 |           32 |           32 |          1   |          1   |       1   |      1    |  1     |   1    |
| 16b          | 16b          |                    64 |               2 |           4 |               8 |           16 |           64 |          0.5 |          2   |       1   |      0.5  |  0.25  |   1    |
| 16b          | 16b          |                    64 |               4 |           4 |               8 |           32 |           64 |          1   |          2   |       2   |      1    |  0.5   |   1    |
| 16b          | 16b          |                    64 |               4 |           2 |               8 |           16 |           32 |          0.5 |          1   |       1   |      1    |  0.5   |   1    |
| 32b          | 16b          |                    32 |               2 |           4 |               8 |           32 |           64 |          1   |          2   |       2   |      1    |  0.5   |   1    |
| 32b          | 16b          |                    32 |               4 |           4 |               4 |           64 |           32 |          2   |          1   |       2   |      1    |  1     |   0.5  |
| 32b          | 16b          |                    32 |               4 |           2 |               4 |           32 |           16 |          1   |          0.5 |       1   |      1    |  1     |   0.5  |
| 16b          | 32b          |                    32 |               2 |           4 |               8 |           16 |          128 |          0.5 |          4   |       2   |      0.5  |  0.125 |   1    |
| 16b          | 32b          |                    32 |               4 |           4 |               4 |           32 |           64 |          1   |          2   |       2   |      1    |  0.5   |   1    |
| 32b          | 32b          |                    16 |               4 |           2 |               4 |           32 |           32 |          1   |          1   |       2   |      1    |  0.5   |   0.5  |
| 32b          | 32b          |                    16 |               4 |           4 |               4 |           64 |           64 |          2   |          2   |       4   |      1    |  0.5   |   0.5  |
| 32b          | 32b          |                    16 |               8 |           2 |               4 |           64 |           32 |          2   |          1   |       4   |      1    |  0.5   |   0.25 |
| bf16         | bf16         |                   128 |               4 |           8 |               4 |           64 |           64 |          2   |          2   |       1   |      0.5  |  1     |   1    |

## Support

GitHub issues will be used for tracking requests and bugs. For questions, go to [support.xilinx.com](https://support.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2023 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
