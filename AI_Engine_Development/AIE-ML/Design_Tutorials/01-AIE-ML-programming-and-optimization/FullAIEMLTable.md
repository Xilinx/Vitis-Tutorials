<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ AI Engine Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis Development Environment on amd.com</a>
        </br>
      <a href="https://www.amd.com/en/products/software/vitis-ai.html">See Vitis AI Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# Matrix Multiplication Compute Performance of the AI Engine-ML Tiles

This table summarizes the I/O and compute balance for each supported input data type and matrix size. Review the columns using these definitions:

- Mat A Type: number of bits of matrix's A data type (`bf16` is 16 bit wide)
- Mat B Type: number of bits of matrix B's data type (`bf16` is 16 bit wide)
- Compute (MAC/cyc): parallel multiply‑accumulate operations per clock cycle on the AI Engine‑ML vector processor
- M: number of rows in matrix A
- K: number of columns in matrix A = number of rows of matrix B
- N: number of columns of matrix B
- Mat A Size B: bytes required to encode matrix A
- Mat B Size (B): bytes required to encode matrix B
- Load Mat A (cyc): ncycles required to load matrix A
- Load Mat B (cyc): cycles required to load matrix B
- Compute (cyc): cycles required to perform the multiplication
- Compute (%): vector processor efficiency compared to the greater of I/O or compute load
- IO A (%): matrix A load efficiency compared to the greater of I/O or compute load
- IO B (%): matrix B load efficiency compared to the greater of I/O or compute load


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

GitHub issues are for tracking requests and bugs. For questions, go to [support.xilinx.com](https://support.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2023-2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/legal/copyright.html">Terms and Conditions</a></sup></p>
