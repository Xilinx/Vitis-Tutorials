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


<hr class="sphinxhide"></hr>

<p class="sphinxhide" align="center"><sub>Copyright © 2021–2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
