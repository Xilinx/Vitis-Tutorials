<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Asynchronous Update of Array RTP Update for AI Engine Kernel

This step demonstrates:

  * The Array RTP update for AI Engine kernels
  * C XRT API to control graph and PL kernels executions

The example is similar to [Asynchronous Update of Array RTP](./step3_async_array.md), except that the random noise generator is not free-running in this example. The system to be implemented is as follows.

![missing image](./images/figure8.PNG)

__Note__: The default working directory for this step is "step4", unless explicitly specified otherwise.

### Review Graph and RTP Code
In the graph definition (`aie/graph.h`), the RTP declaration and connection are added as follows:

    port<direction::in> coefficients;
    connect< parameter >(size, async(fir24.in[1]));

In `aie/graph.cpp` (for AI Engine simulator):
    
    gr.init();
    //run for 16 iterations, update narrow filter coefficients, wait, update wide filter coefficients, run for 16 iterations
    gr.update(gr.coefficients, narrow_filter, 12);
    gr.run(16); // 16 iterations for AIE kernel
    gr.wait();
    gr.update(gr.coefficients, wide_filter, 12);
    gr.run(16);
    gr.end();

### Run AI Engine Compiler and AI Engine Simulator
Compile the AI Engine graph (`libadf.a`) using the AI Engine compiler:

    make aie

The corresponding AI Engine compiler command is:

    aiecompiler -platform=xilinx_vck190_es1_base_202210_1.xpfm -include="./aie" -include="./data" -include="./aie/kernels" -include="./" -workdir=./Work aie/graph.cpp
    
After the AI Engine graph (`libadf.a`) has been generated, verify for correctness using the AI Engine simulator:

    make aiesim

## Run Hardware Cosimulation and Hardware Flow
For the PL kernel (`random_noise`) , an additional parameter `size` has been added, which is different from the free-running kernel in [previous step](https://gitenterprise.xilinx.com/brucey/AIE_RTP_tutorials/blob/master/step3_async_array.md). A loop is also added in the kernel to iterate `size` times. The code in `pl_kernels/random_noise.cpp` is as follows:

    extern "C" void random_noise(hls::stream<std::complex<short> > & out, int size) {
    #pragma HLS INTERFACE axis port=out
    #pragma HLS INTERFACE s_axilite port=return bundle=control
    #pragma HLS INTERFACE ap_ctrl_hs port=return
    #pragma HLS interface s_axilite port=size bundle=control
            for(int i=0;i<size;i++){
                    ...
            }
    }

Pragma `#pragma HLS INTERFACE s_axilite port=return bundle=control` indicates that the kernel has an AXI4-Lite interface for block control. Other scalar inputs and global memory addresses are also bundled into this AXI4-Lite interface. Here, the scalar input `size` is bundled into the AXI4-Lite interface by `#pragma HLS interface s_axilite port=size bundle=control` for Vitis HLS. Pragma `#pragma HLS INTERFACE ap_ctrl_hs port=return` indicates the control protocol for the kernel is `ap_ctrl_hs`, meaning that the kernel has to be explicitly started and polled for completion. This is normally done by the OpenCL API or the XILINX Runtime API.

For information about the pragmas and protocols for Vitis HLS, refer to the Vitis HLS User Guide. 

The Makefile rule targets introduced in [Synchronous update of Scalar RTP](./step1_sync_scalar.md), [Asynchronous Update of Scalar RTP](./step2_async_scalar.md), and [Asynchronous Update of Array RTP](./step3_async_array.md) still apply here. Details about tool options and host code in [Synchronous Update of Scalar RTP](./step1_sync_scalar.md) are similar. 

In Linux mode, the following header files are required for the ADF and XRT APIs:
	
	#include "adf/adf_api/XRTConfig.h"
	#include "experimental/xrt_kernel.h"
	
Besides using the ADF API to control graph execution, you can also use the XRT API for graph control. In this host code, `__USE_ADF_API__` is used to switch between the ADF and XRT APIs. You can modify the Makefile in the `sw` directory to choose which API to use. 

In `sw/host.cpp`, for the ADF API, the graph `update()` is used to update the RTP, and the graph `run()` is used to launch the AI Engine kernels. But the `adf` API calls the Xilinx Runtime (XRT) API underneath, and `adf::registerXRT()` is introduced to build the relationship between them. The code for RTP update and execution by the `adf` API is as follows:

    // update graph parameters (RTP) & run
    adf::registerXRT(dhdl, uuid);
    int narrow_filter[12] = {180, 89, -80, -391, -720, -834, -478, 505, 2063, 3896, 5535, 6504};
    int wide_filter[12] = {-21, -249, 319, -78, -511, 977, -610, -844, 2574, -2754, -1066, 18539};
    gr.update(gr.coefficients, narrow_filter, 12);//update AIE kernel RTP
    gr.run(16);//start AIE kernel
    gr.wait();
    gr.update(gr.coefficients, wide_filter, 12);//Update AIE kernel RTP
    gr.run(16);//start AIE kernel

In `sw/host.cpp`, notice that `adf::registerXRT()` requires the device handle and UUID of the XCLBIN image. These can be obtained by the Xilinx Runtime (XRT) API:
    
    auto dhdl = xrtDeviceOpen(0);//device index=0
    ret=xrtDeviceLoadXclbinFile(dhdl,xclbinFilename);
    xuid_t uuid;
    xrtDeviceGetXclbinUUID(dhdl, uuid);
        
Notice that the XRT API was used instead of the OpenCL API to manage the PL kernels (`s2mm` and `random_noise`). This execution model is similar to that introduced in [Asynchronous Update of Array RTP](./step3_async_array.md). The code in `sw/host.cpp` is as follows:

    // Allocate output global memory and map to host memory
    xrtBufferHandle out_bohdl = xrtBOAlloc(dhdl, output_size_in_bytes, 0, /*BANK=*/0);
    std::complex<short> *host_out = (std::complex<short>*)xrtBOMap(out_bohdl);

    // s2mm ip
    xrtKernelHandle s2mm_khdl = xrtPLKernelOpen(dhdl, uuid, "s2mm"); // Open kernel handle
    xrtRunHandle s2mm_rhdl = xrtRunOpen(s2mm_khdl); 
    xrtRunSetArg(s2mm_rhdl, 0, out_bohdl); // set kernel arg
    xrtRunSetArg(s2mm_rhdl, 2, OUTPUT_SIZE); // set kernel arg
    xrtRunStart(s2mm_rhdl); //launch s2mm kernel

    // random_noise ip
    xrtKernelHandle random_noise_khdl = xrtPLKernelOpen(dhdl, uuid, "random_noise");
    xrtRunHandle random_noise_rhdl = xrtRunOpen(random_noise_khdl);
    xrtRunSetArg(random_noise_rhdl, 1, OUTPUT_SIZE);
    xrtRunStart(random_noise_rhdl);
    printf("run random_noise\n");

    // update graph parameters (RTP) & run
    ...
    
    // wait for s2mm done
    auto state = xrtRunWait(s2mm_rhdl);
	
    // Transfer data from global memory in device back to host memory 
    xrtBOSync(out_bohdl, XCL_BO_SYNC_BO_FROM_DEVICE , output_size_in_bytes,/*OFFSET=*/ 0);
    
    // Post-processing of data

After post-processing the data, release the allocated objects:

	gr.end();
	xrtRunClose(s2mm_rhdl);
    xrtKernelClose(s2mm_khdl);
	xrtRunClose(random_noise_rhdl);
    xrtKernelClose(random_noise_khdl);
    xrtBOFree(out_bohdl);
    xrtDeviceClose(dhdl);

The XRT API version to control graph exectuion is as follows:

	auto ghdl=xrtGraphOpen(dhdl,uuid,"gr");
	
	ret=xrtGraphUpdateRTP(ghdl,"gr.fir24.in[1]",(char*)narrow_filter,12*sizeof(int));
	ret=xrtGraphRun(ghdl,16);
	ret=xrtGraphWait(ghdl,0);

	ret=xrtGraphUpdateRTP(ghdl,"gr.fir24.in[1]",(char*)wide_filter,12*sizeof(int));
	ret=xrtGraphRun(ghdl,16);

You can see that the XRT API to open, run, wait, and RTP update graph are: xrtGraphOpen(), xrtGraphRun(), xrtGraphWait(), xrtGraphUpdateRTP(). For more information about XRT APIs on graphs, see the *Versal ACAP AI Engine Programming Environment User Guide* (UG1076).

Run the following `make` command to build the host exectuable file:

    make host
    
Notice the following linker script links the libraries `adf_api_xrt` and `xrt_coreutil`. these are necessary for the `adf` API to work together with the XRT API.

    ${CXX} -o ../host.exe aie_control_xrt.o host.o -ladf_api_xrt -lgcc -lc -lxilinxopencl -lxrt_coreutil -lpthread -lrt -ldl -lcrypt -lstdc++ -L${SDKTARGETSYSROOT}/usr/lib/ --sysroot=${SDKTARGETSYSROOT} -L$(XILINX_VITIS)/aietools/lib/aarch64.o

Run the following `make` command to build all necessary files and launch HW cosimulation:

    make run_hw_emu
        
In the Linux prompt, run following commands:

    mount /dev/mmcblk0p1 /mnt
    cd /mnt
    export XILINX_XRT=/usr
    export XCL_EMULATION_MODE=hw_emu
    ./host.exe a.xclbin
        
To exit QEMU press Ctrl+A, x

For hw mode, run following `make` command to generate an SD card package:

    make package TARGET=hw

In hardware, after booting Linux from the SD card, run following commands in the Linux prompt:

    mount /dev/mmcblk0p1 /mnt
    cd /mnt
    export XILINX_XRT=/usr
    ./host.exe a.xclbin

The host code is self-checking. It will check the output data against the golden data. If the output matches the golden data, after the run is complete, it will print the following:

    TEST PASSED
        
### Conclusion
In this step you learned about:
   * Asynchronous array RTP
   * Launching AI Engine simulator, HW cosimulation, and HW
  
Next, review [Asynchronous Array RTP Update and Read for AI Engine Kernel](./step5_async_array_update_read.md).	


# Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

# License

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.

You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )


Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

<p align="center"><sup>XD001 | &copy; Copyright 2020-2022 Xilinx, Inc.</sup></p>
