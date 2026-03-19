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

# Basic Verification Script

The basic verification Python script contained in this folder works as follows:

- Reads the input data from the text files named ``PLIO_i[*]`` in the ``v_input`` folder.
  - This data follows the AIE-ML Kernel and Graph programming User Guide (UG1603)
  - The ``v_input`` folder has two other folders: one contains text files representing a sampled rectangular wave, the other contains text files with random data. You can replace the text files in the ``v_input`` folder with those.
- Reads the AI Engine simulation output data from the text files named ``PLIO_o[*]`` in the ``v_output`` folder.
  - This data has been copied and pasted from the simulation outputs created with the AMD Vitis™ Unified IDE.
- Performs the FFT on the input data with the NumPy library functions to create a golden reference to compare with the AI Engine simulation outputs.
- Creates a graph that plots instance 0 for both the AI Engine output data and the NumPy golden reference.
- Performs the iFFT on the AI Engine simulation output data with the NumPy library functions to compare it with the input data.
- Creates a graph that plots instance 0 for both the NumPy iFFT of the AI Engine outputs and the input data.
- Creates a graph displaying the relative error between the NumPy golden reference FFT and the AI Engine one for each point of instance 0.

Inside the script there is a section where it is possible to set the following parameters:

- The datatype
- The number of points
- The number of signal parallel instances
- The I/O temporal interleaving factor $\theta$ which is the ratio between the PL clock speed and the sample rate
- The I/O spacial interleaving factor which is the amount of samples packed together in a I/O channel
- The number of sampling repetitions: the parameter inside the AI Engine Run method in the graph .cpp file

Do not change such settings parameters while reproducing this tutorial.

## Using the Script

Before using this script, install numpy and ml_dtypes on your system. To do so, use the following commands in your terminal:

```bash
    pip install numpy
    pip install ml_dtypes
```

To use the verification script, use the following command while in the ```support/verification``` folder:

```python
python -i ./Basic_verification
```

The script runs for some time and generates the graphs one after the other. After you analyze and close all the graphs, type ```quit()``` to close the Python terminal.

## Support

GitHub issues are used for tracking requests and bugs. For questions, go to [support.amd.com](https://adaptivesupport.amd.com/s/?language=en_US).

<hr class="sphinxhide"></hr>

<p class="sphinxhide" align="center"><sub>Copyright © 2021–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
