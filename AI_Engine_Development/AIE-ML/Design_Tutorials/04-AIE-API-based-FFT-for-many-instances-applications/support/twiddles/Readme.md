<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ AI Engine Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
        </br>
      <a href="https://www.amd.com/en/products/software/vitis-ai.html">See Vitis™ AI Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# Twiddle Factors Generation

The support files generate twiddle factors for a selectable number of points. Supported decimations are radix-2, radix-3, radix-4, radix-5, and any combination of those. The AI Engine API accepts these for mixed-radix implementations.

Besides generating the twiddle factors, the script can also generate the twiddle variable declaration section of the kernel header file. It puts the output in a "Twiddle_declaration.txt" text file.

## Usage

To use this script, install numpy and ml_dtypes on your system. To do so, use the following commands in your terminal:

```bash
    pip install numpy
    pip install ml_dtypes
```

After doing so, if you are using the script to generate a twiddle header file different from the tutorial one, set the parameters as explained in the "[Parameters Setting](#parameters-setting)" section.

If not, run the script with the following terminal command:

```python
    python Twiddle_gen.py
```

### Parameters Setting

To set the parameters to create a new twiddle header file, scroll down to the last lines of the `Twiddle_gen.py` script. Set the parameters as needed.

If you need a mixed-radix implementation, set **mixed_radix** to 1. For single radix, if a radix-4 is possible, but you want twiddle factors to perform a radix-2, set ***force_r2*** to 1.

If you need an output file containing the twiddle factors, set ***file*** to 1. Otherwise, set it to 0. You can chose the name of the output file by modifying ***filename***.

The system encodes datatypes as follows:

- 0 = float32
- 1 = bfloat
- 2 = cint16
- 3 = cint32

For the mixed-radix implementation, chose **radixes** and **vectorizations** values for each stage.

If you are modifying the twiddles to create a different FFT, you must also change the twiddles declaration in the kernel header file. Set the `variable_dec_file` variable to 1 to generate a text file containing the declaration lines.

## Support

GitHub issues are used for tracking requests and bugs. For questions, go to [support.amd.com](https://adaptivesupport.amd.com/s/?language=en_US).

<hr class="sphinxhide"></hr>

<p class="sphinxhide" align="center"><sub>Copyright © 2021–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
