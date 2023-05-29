<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Hardware Acceleration Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html"> Vitis™ Development Environment</a>
    </td>
 </tr>
</table>

# Part 2: Running the Application

## Configuring the xrt.ini File

XRT uses various parameters to control the execution flow, debug, profiling, and message logging during host application and kernel execution in software emulation, hardware emulation, and system run on the acceleration board. These control parameters are optionally specified in a runtime initialization file called [xrt. ini](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/xrt.ini-File).

Following is an ``.ini`` file is for your reference.

```
#Start of Debug group 
[Debug] 
native_xrt_trace = true
device_trace = fine

#Start of Runtime group 
[Runtime] 
runtime_log = console

#Emulation group
[Emulation] 
debug_mode = batch

```

Place the [xrt. ini](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/xrt.ini-File) file in the executable directory.

## Compiling the Code and Running the Application

Run the following ``make`` command to build your XCLBIN and host binary targeting a specific device. Note that this process may take a few hours if the target is hw.

>**Note:** The TARGET environment variable can be set as sw_emu, hw_emu, and hw according to which Vitis target is expected to run. sw_emu is for C level emulations. hw_emu is for RTL level emulations. hw is for a real onboard test. Before you run the case on a real board, it is recommended that you test it on sw_emu and hw_emu.


You can run the application with the default setting:

```
  $ make help
  $ make run TARGET=hw # option for sw_emu or hw_emu
```

Or, you can compile the code with the ``make`` command:

```
  $ make xclbin TARGET=hw    # build the binary running on Alveo # option for sw_emu, hw_emu
  $ make host                # build the binary running on the host
  $ make clean               # clean the binary
```

Run the application with the following arguments:

```
  $ ./shortpath -xclbin shortpath_hw.xclbin -o csr_data/offset.mtx -c csr_data/indicesweights.mtx -n csr_data/node.mtx
```

Input Arguments:

```
 Usage: host.exe -[-xclbin -o -c -n]
         -xclbin:      The kernels name # option for sw.xclbin, hw.xclbin
         -o:           Offset file name
         -c:           Indices_weights file name
         -n:           Nodes information
```


>**Note:** If you run the sw_emu or hw_emu, set the emu_environment as the follows:

  ```
    $ emconfigutil --platform xilinx_u50_gen3x16_xdma_5_202210_1
    $ export XCL_EMULATION_MODE=sw_emu # option for hw_emu
  ```


## Reading the Output for HW TARGET

The output information displayed is similar to the one shown in the following example:

```
---------------------Short path compute----------------
numVertices: 24
numEdages: 76
Found Platform
Platform Name: Xilinx
Info: Context created
Info: Command queue created
INFO: Importing shortpath_hw.xclbin
Loading: 'shortpath_hw.xclbin'
Info: Program created
Info: Kernel created
Info: Kernel created
Info: Kernel created
kernels has been created
kernels start------
kernels end------
**********************************************************

--------------------- CityName Shortest Path from source 0----------------
The start point is: 0 longitude: -96.7704 latitude: 43.6128
Please input your choice: 
Input 0 : show all the result and exit.
Input point range in ( 1 to 23 ): show the path from start point to the enter point.
Input y to exit !

```

You can input your choice when prompted to get the results.


## Next

Proceed to [**Part 3**](../docs/profiling.md).


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
