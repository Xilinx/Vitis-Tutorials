<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AIE-ML Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

# Basic verification script

The basic verification Python script contained in this folder works as follows:
- Reads the input data from the text files named ``PLIO_i[*]`` in the ``v_input`` folder.
  - Such data has been generated following the AIE-ML Kernel and Graph programming User Guide (UG1603)
  - In the ``v_input`` folder, there are two other folders: one contains text files representing a sampled rectangular wave, the other contains text files with random data. You can replace the text files in the ``v_input`` folder with those.
- Reads the AI Engine simulation output data from the text files named ``PLIO_o[*]`` in the v_output folder.
  - Such data has been copied and pasted from the simulation outputs created with Vitis Unified IDE.
- Performs the FFT on the input data with the Numpy library functions, to create a golden reference to be compared with the AI Engine simulation outputs.
- Creates a graph where the instance 0 is plotted both for the AI Engine output data and for the Numpy golden reference.
- Performs the IFFT on the AI Engine simulation output data with the Numpy library functions, to compare it with the input data.
- Creates a graph where the instance 0 is plotted both for the Numpy IFFT of the AI Engine outputs and for the input data.
- Creates a graph displaying the relative error between the Numpy golden reference FFT and the AI Engine one for each point of instance 0. 

Inside the script there is a section where it is possible to set the following parameters:
- The datatype;
- The number of points;
- The number of signal parallel instances;
- The IO temporal interleaving factor $\theta$: the ratio between the PL clock speed and the sample rate
- The IO spacial interleaving factor: the amount of samples packed together in a IO channel
- The number of sampling repetitions: the parameter inside the AI Engine Run method in the graph .cpp file;

Do not change such settings parameters while reproducing this tutorial.

## Using the script

Before using this script, please make sure having numpy and ml_dtypes installed on your setup. To do so, just use the following commands in your terminal:

```
    pip install numpy
    pip install ml_dtypes
```

To use the verification script, use the following command while being located in the ```support/verification``` folder:

```python -i ./Basic_verification```

The script will run for some moments and generate the graph one after the other.
Once all the graph have been analyzed and closed, type ```quit()``` to close the python terminal.

## Support

GitHub issues will be used for tracking requests and bugs. For questions, go to [support.xilinx.com](http://support.xilinx.com/).

## License

Components: xilinx-images

images in the documentation

Components: xilinx-files

The MIT License (MIT)

Copyright (c) 2024 Advanced Micro Devices, Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

<p class="sphinxhide" align="center">  &copy; Copyright 2024 Advanced Micro Devices, Inc.</p>
<p class="sphinxhide" align="center">  &copy; Copyright 2021 Xilinx Inc.</p>