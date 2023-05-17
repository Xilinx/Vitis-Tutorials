<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Asynchronous Array RTP Update and Read for AI Engine Kernel

This step demonstrates:

* The Array RTP update for AI Engine kernels
* Asynchronous read of array RTP for AI Engine kernels
* C++ version of the Xilinx® Runtime (XRT) API to control PL kernels
* C++ version of the XRT API to control graph execution

The example design is similar to [Asynchronous Update of Scalar RTP for PL inside a Graph, and Array RTP Update for AI Engine Kernel](./step4_async_aie_array.md), except that the AI Engine kernel has an asynchronous output, and the PL kernel inside the graph is pulled out of the graph. The example shows how to perform asynchronous reads of array RTP by the XRT API. The PL kernels and graph execution are controlled by the C++ version of the XRT API.

The system to be implemented is as follows:

![missing image](./images/figure9.PNG)

>**Note:** The default working directory in this step is "step5", unless explicitly specified otherwise.

## Review Graph and RTP Code

In the AI Engine kernel code (`aie/kernels/hb24.cc`), the interface is declared as:

```cpp
void fir24_sym(input_buffer<cint16,adf::extents<64>,adf::margin<24>> & iwin, output_buffer<cint16,adf::extents<64>> & owin,  const int32(&coeffs)[12], int32(&coeffs_readback)[12]);
```

For the RTP array input, `const` is used for the array reference. From the graph, the RTP port can only be `input` or `inout`. The `inout` port in the graph can only be read by the PS program, it cannot be written by the PS program. Therefore, another port `coeffs_readback` is defined to read back the coefficient.

In the graph definition (`aie/graph.h`), the RTP declaration and connection are added as follows:

```cpp
port<direction::in> coefficients;
port<direction::inout> coefficients_readback;
connect< parameter >(coefficients, async(fir24.in[1]));
connect< parameter >(async(fir24.inout[0]),coefficients_readback);
```

In `aie/graph.cpp` (for AI Engine simulator), the RTP update and read commands are:

```cpp
gr.update(gr.coefficients, narrow_filter, 12);
gr.run(16); // start PL kernel & AIE kernel
gr.read(gr.coefficients_readback,coeffs_readback,12);
std::cout<<"Coefficients read back are:";
for(int i=0;i<12;i++)std::cout<<coeffs_readback[i]<<",\t";
    std::cout<<coeffs_readback[i]<<",\t";
    }
    std::cout<<std::endl;
    gr.wait(); // wait PL kernel & AIE kernel to complete
    gr.read(gr.coefficients_readback,coeffs_readback,12);
    std::cout<<"Coefficients read back are:";
    for(int i=0;i<12;i++){
        std::cout<<coeffs_readback[i]<<",\t";
        }
        std::cout<<std::endl;
        gr.update(gr.coefficients, wide_filter, 12);
        gr.run(16); // start PL kernel & AIE kernel
        gr.read(gr.coefficients_readback,coeffs_readback,12);
        std::cout<<"Coefficients read back are:";
        for(int i=0;i<12;i++){
            std::cout<<coeffs_readback[i]<<",\t";
        }
        std::cout<<std::endl;
```

Because the RTP read is asynchronous, it can not ensure that reads are happening in between kernels running. Only after graph `wait()` runs (which is a synchronization point), are the coefficients updated and can be read back.

## Run AI Engine Compiler and AI Engine Simulator

1. Compile the AI Engine graph (`libadf.a`) using the AI Engine compiler:

   ```
   make aie
   ```

   The corresponding AI Engine compiler command is:

   ```
   aiecompiler -platform=$PLATFORM_REPO_PATHS/xilinx_vck190g_base_202310_1/xilinx_vck190_base_202310_1.xpfm -include="./aie" -include="./data" -include="./aie/kernels" -include="./" -workdir=./Work aie/graph.cpp
   ```

2. After the AI Engine graph (`libadf.a`) has been generated, verify for correctness using the AI Engine simulator:

   ```
   make aiesim
   ```

## Run Hardware Cosimulation and Hardware Flow

The Makefile rule targets introduced in [Synchronous update of Scalar RTP](./step1_sync_scalar.md), [Asynchronous Update of Scalar RTP](./step2_async_scalar.md) and [Asynchronous Update of Array RTP](./step3_async_array.md) still apply here.

Details about tool options and host code in [Asynchronous Update of Scalar RTP for PL inside a Graph](./step2_async_scalar.md), and [Array RTP Update for AI Engine Kernel](./step4_async_aie_array.md) are similar. This section only focuses on the different part.

In `sw/host.cpp`, the C++ version of the XRT API is used to control PL kernels outside of the graph.

1. Run the following `make` command to build the host exectuable file.

   ```
   make host
   ```

2. Run the following make command to build all necessary files and launch HW cosimulation:

   ```
   make run_hw_emu
   ```

3. At the Linux prompt, run following commands:

   ```bash
   ./host.exe a.xclbin
   ```

    To exit QEMU press `Ctrl+A`, and then press `x`.

4. For hw mode, run following `make` command to generate an SD card package:

   ```
   make package TARGET=hw
   ```

5. In hardware, after booting Linux from the SD card, run following commands in the Linux prompt:

   ```bash
   mount /dev/mmcblk0p1 /mnt
   cd /mnt
   ./host.exe a.xclbin
   ```

The host code is self-checking. It will check the output data against the golden data. If the output matches the golden data, after the run is complete, it will print a message similar to:

```cpp
Coefficients read back are:180, 89,     -80,    -391,   -720,   -834,   -478,   505,    2063,   3896,   5535,   6504,
Graph wait done

Coefficients read back are:180, 89,     -80,    -391,   -720,   -834,   -478,   505,    2063,   3896,   5535,   6504,
Update fir24 done

Coefficients read back are:-21, -249,   319,    -78,    -511,   977,    -610,   -844,   2574,   -2754,  -1066,  18539,
s2mm completed with status(4)
TEST PASSED
```

## Conclusion

In this tutorial you learned about:

* The concepts of synchronous and asynchronous scalar RTP
* Asynchronous array RTP
* Asynchronous array RTP read
* Launching AI Engine simulator, HW cosimulation, and HW

### Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums](http://forums.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
