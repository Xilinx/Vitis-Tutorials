<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™Hardware Acceleration Introduction Tutorial</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

## Overview

For our first example, look at how to load images onto the AMD Alveo™ Data Center accelerator card. When you power on the system, the Alveo card will initialize its shell. Recall from earlier, that the shell implements connectivity with the host PC but leaves most of the logic as a blank canvas for you to build designs. Before we can use that logic in our applications, we must first configure it.

Also, recall from earlier that certain operations are inherently "expensive" in terms of latency. Configuring the FPGA is, in fact, one of the most inherently time consuming parts of the application flow. To get a feel for exactly how expensive, try loading an image.

## Key Code

In this example, we initialize the OpenCL™ runtime API for Xilinx Runtime (XRT), create a command queue, and — most importantly — configure the Alveo card FPGA. This is generally a one-time operation: once the card is configured it will typically remain configured until power is removed or it is reconfigured by another application. If multiple independent applications attempt to load hardware into the card, the second application will be blocked until the first one relinquishes control, although multiple independent applications can share the same image running on a card.

To begin, we must include the headers as shown in the following code. For the full code referenced in this application, refer to the file `design_source/sw_src/00_load_kernels.cpp`.

```cpp
// Xilinx OpenCL and XRT includes
#include"xcl2.hpp"

#include <CL/cl.h>
```

Of these two, only `CL/cl.h` is required. `xcl2.hpp` is a library of AMD-provided helper functions to wraparound some of the required initialization functions.

Once we include the appropriate headers, we need to initialize the command queue, load the binary file, and program it into the FPGA, as shown in the following listing. This is effectively boilerplate code you will need to include in every program at some point.

```cpp
// This application will use the first AMD device found in the system
std::vector<cl::Device> devices = xcl::get_xil_devices();
cl::Device device               = devices[0];

cl::Context context(device);
cl::CommandQueue q(context, device);

std::stringdevice_name      = device.getInfo<CL_DEVICE_NAME>();
std::stringbinaryFile       = xcl::find_binary_file(device_name, argv[1]);
cl::Program::Binaries bins  = xcl::import_binary_file(binaryFile);

devices.resize(1);
cl::Program program(context, devices, bins);
```

The workflow here can be summed up as follows:

1. Discover and number the AMD devices in the system. In this case, we can assume device 0 is the targeted card, but in a multi-accelerator card system, you will have to specify them.
2. Initialize the OpenCL context and command queues.
3. Load the binary file targeting our Alveo board. In these examples, we pass the file name on the command line, but this can be hard-coded or otherwise handled on an application-specific basis.
4. Program the FPGA.

Line 44 is where the programming operation is actually triggered. During the programming phase the runtime checks the current Alveo card configuration. If it is already programmed, we can return after loading the device metadata from the xclbin. But if not, program the device now.

## Running the Application

With the XRT initialized, run the application by running the following command from the build directory:

`./00_load_kernels alveo_examples`

The program will output a message similar to this:

```
-- Example 0: Loading the FPGA Binary --

Loading XCLBin to program the Alveo board:

Found Platform
Platform Name: Xilinx
XCLBIN File Name: alveo_examples
INFO: Importing ./alveo_examples.xclbin
Loading: ?./alveo_examples.xclbin?

FPGA programmed, example complete!

-- Key execution times --
OpenCL Initialization : 1624.634 ms
```

Our FPGA took 1.6 seconds to initialize. Be aware of this kernel load time; it includes disk I/O, PCIe® latency, configuration overhead, and a number of other operations. Usually you will want to configure the FPGA during your application's startup time, or even pre-configure it. Run the application again with the bitstream already loaded:

```
-- Key execution times --
OpenCL Initialization : 262.374 ms
```

0.26 seconds is much better than 1.6 seconds! We still have to read the file from disk, parse it, and ensure the xclbin loaded into the FPGA is correct, but the overall initialization time is significantly lower.

## Extra Exercises

Some things to try to build on this experiment:

1. Use the **xbutil** utility to query the board. Can you see which `.xclbin` file is loaded?
2. Again using **xbutil**, which kernel(s) are present in the FPGA? Do you see a difference before and after the FPGA is programmed?

## Key Takeaways

- FPGA configuration is an expensive operation. Ideally, initialize the FPGA long before it's needed. This can be done in a separate thread, during other initialization tasks in your application, or even at system boot time for dedicated systems.
- Once the FPGA is loaded, subsequent loads are substantially faster.
- Now that we can load images into the FPGA, run something!

Read [**Example 1:** Simple Memory Allocation](./01-simple-memory-allocation.md)

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
