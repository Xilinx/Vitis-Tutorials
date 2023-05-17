<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Buffer-based AI Engine Kernels with Mixed Data Types

This example is similar to the previous [Buffer-based AI Engine Kernels](./buffer_based_aie_kernel.md) example, except that one AI Engine kernel has floating point interfaces and one AI Engine kernel has `cint16` interfaces. It will focus on the difference with the previous example. One difference is that because the input and output data are in integer format for the AI Engine simulator, it has to convert data between `float`/`cint16` and integer data types. See [Prepare Data for AI Engine Simulator](#prepare-data-for-ai-engine-simulator). Another difference is that the PS code has to take care of the input and output data types. See [PS Application and HW Emulation Flows](#ps-application-and-hw-emulation-flows).

The following topics are already covered in [Buffer Based AI Engine Kernels](./buffer_based_aie_kernel.md).

* [Construct Graph with Packet Switching Capability](./buffer_based_aie_kernel.md/#Construct-Graph-with-Packet-Switching-Capability)
* [Packet Format](./buffer_based_aie_kernel.md/#Packet-Format)
* [Example PL Kernels for Packet Switching](./buffer_based_aie_kernel.md/#Example-PL-Kernels-for-Packet-Switching)
* [Example PS code for Packet Switching](./buffer_based_aie_kernel.md/#Example-PS-code-for-Packet-Switching)

## Prepare Data for AI Engine Simulator

Change the working directory to `buffer_aie_mix_int32_float_cint16`. The graph code for this example is the same as the [Buffer Based AI Engine Kernels](./buffer_based_aie_kernel.md) example. The AI Engine kernel `core[2]` (`aie/aie_core3.cpp`) has floating point interfaces, and the AI Engine kernel `core[3]` (`aie/aie_core4.cpp`) has `cint16` interfaces.

When preparing the data for the AI Engine simulator, all values should be in 32-bit integer format. The conversion is similar to the `reinterpret_cast` operation in C++. It is done manually in any language. For example, when you want to feed float data `1.0f`, `2.0f`,..., into the AI Engine kernel, the integer format can be generated in C as shown in the following code.

```cpp
    //input data for float
    for(int i=0;i<16;i++){
      float tmp=i;
      printf("%d\n",*(int*)&tmp);
    }
```

Then the data in the input file (`data/input.txt`) for float, `1.0f`,`2.0f`,...,`16.0f`, should be as follows.

```
    0
    1065353216
    1073741824
    1077936128
    1082130432
    1084227584
    1086324736
    1088421888
    1090519040
    1091567616
    1092616192
    1093664768
    1094713344
    1095761920
    1096810496
    1097859072
```

Similarly, type `cint16` should be converted to integer type. For example, for `cint16` data, {0,0},{4,4},{8,8},..., the integer format can be generated in C as shown in the following code.

```cpp
    //input data for cint16
    for(int i=0;i<16;i++){
      int tmp=i*4;
      tmp=tmp<<16;
      tmp+=i*4;
      printf("%d\n",tmp);
    }
```

Then the data in the input file (`data/input.txt`) for `cint16` data, {0,0},{4,4},{8,8},...,{60,60}, should be as follows.

```
    0
    262148
    524296
    786444
    1048592
    1310740
    1572888
    1835036
    2097184
    2359332
    2621480
    2883628
    3145776
    3407924
    3670072
    3932220
```

Take a look at the input file `data/input.txt` to see how input data is organized.

Run the following `make` command to run the AI Engine compiler and simulator.

```
make aiesim
```

The output data is in `aiesimulator_output/data/output.txt`. Similarly, the output data can be converted from integer to `float` or `cint16` to be human-readable.

### PS Application and HW Emulation Flows

The difference in the PS application from [Buffer Based AI Engine Kernels](./buffer_based_aie_kernel.md) is that the input buffers and output buffers for different data types should be modified accordingly. Take a look at the code in `sw/host.cpp`. Note how `float` and complex type (for `cint16`) is used in the code.

```cpp
    // output memory
    xrtBufferHandle out_bo3 = xrtBOAlloc(dhdl, mem_size, 0, /*BANK=*/0);
    xrtBufferHandle out_bo4 = xrtBOAlloc(dhdl, mem_size, 0, /*BANK=*/0);
    float *host_out3 = (float*)xrtBOMap(out_bo3);
    std::complex<short> *host_out4 = (std::complex<short>*)xrtBOMap(out_bo4);
    
    // input memory
    xrtBufferHandle in_bo3 = xrtBOAlloc(dhdl, mem_size, 0, /*BANK=*/0);
    xrtBufferHandle in_bo4 = xrtBOAlloc(dhdl, mem_size, 0, /*BANK=*/0);
    float *host_in3 = (float*)xrtBOMap(in_bo3);
    std::complex<short> *host_in4 = (std::complex<short>*)xrtBOMap(in_bo4);
```

Correspondingly, the pre-processing and post-processing of this data has been changed.

1. Run HW emulation with the following ``make` command to build the HW system and host application.

   ```
   make run_hw_emu
   ```

   >**Tip:** If the keyboard is accidentally hit and stops the system booting automatically, type boot at the ``Versal>`` prompt to resume the system booting.

2. After Linux has booted, run the following commands at the Linux prompt (this is only for HW cosim).

   ```bash
    mount /dev/mmcblk0p1 /mnt
    cd /mnt
    export XILINX_XRT=/usr
    export XCL_EMULATION_MODE=hw_emu
    ./host.exe a.xclbin
    ```

    To exit QEMU press Ctrl+A, x.

3. To run in hardware, first build the system and application using the following `make` command.

   ```
   make package TARGET=hw
   ```

4. After Linux has booted, run the following commands at the Linux prompt.

   ```bash
    mount /dev/mmcblk0p1 /mnt
    cd /mnt
    export XILINX_XRT=/usr
    ./host.exe a.xclbin
    ```

The host code is self-checking. It will check the correctness of output data. If the output data is correct, after the run has completed, it will print:

```
    TEST PASSED
```

## Conclusion

In this step, you learned about the following concepts.

* Preparing `float` and `cint16` data types for the AI Engine simulator.
* PS application for different data types.

Next, review [Packet Stream Based AI Engine Kernels](./pktstream_based_aie_kernel.md).

### Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums](http://forums.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
