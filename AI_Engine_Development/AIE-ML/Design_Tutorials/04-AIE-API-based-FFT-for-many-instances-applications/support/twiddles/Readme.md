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

# Twiddle factors generation

Those support files generate twiddle factors for an user selectable number of points.
The supported decimations are radix-2, radix-3, radix-4, radix-5 and any combination of those for mixed-radix implementations accepted by the AI Engine API.
In addition to the twiddle creation, the script can also generate the twiddle variable declaration section of the kernel header file, putting the output in a "Twiddle_declaration.txt" text file.

## Usage
To use this script make sure have numpy and ml_dtypes installed on your setup. To do so just use type the following commands in your terminal:

```
    pip install numpy
    pip install ml_dtypes
```

After doing so, if you are using the script to generate a twiddle header file different from the tutorial one, set the parameters as explained in the "[Parameters settings]" section. 
If not, just to run the script with the following terminal command:
```
    python Twiddle_gen.py
```

### Parameters setting
To set the parameters to create a new twiddle header file, scroll down to the last lines of the Twiddle_gen.py script and set the parameters as needed.


If a mixed-radix implementation is desired, set **mixed_radix** to 1.
For single radix, if a radix-4 is possible, but you want twiddle factors to perform a radix-2, set ***force_r2*** to 1.
If an output file containing the twiddle factors is desired, set ***file*** to 1, otherwise set it to 0.
You can chose the name of the output file by modifying ***filename***.

The ***datatype*** is encoded like this:
  - 0 = float32
  - 1 = bfloat
  - 2 = cint16
  - 3 = cint32

For the mixed-radix implementation, you must chose **radixes** and **vectorizations** values for each stage.

If you are modifying the twiddles to create a different FFT, also the twiddles declaration in the kernel header file must be changed. Set the variable **variable_dec_file** to 1 to generate a text file containing the declaration lines.

## Support

GitHub issues will be used for tracking requests and bugs. For questions, go to [support.xilinx.com](http://support.xilinx.com/).


<hr class="sphinxhide"></hr>

<p class="sphinxhide" align="center"><sub>Copyright © 2021–2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
