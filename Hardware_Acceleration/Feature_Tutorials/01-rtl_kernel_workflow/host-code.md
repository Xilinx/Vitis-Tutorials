<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Hardware Acceleration Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Host Code Programming

User-managed RTL kernels are only supported in host applications written with the XRT native API, as described in the [XRT documentation](https://xilinx.github.io/XRT/master/html/xrt_native_apis.html). The XRT native API provides a rich set of class objects and functions for XRT-managed kernels, as discussed in the *XRT Native API Host Application* tutorial, and for user-managed kernels as described here. 

Open the `reference-files/src/host/user-host.cpp` file to examine its content and structure. As decribed in [Host Programming](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Host-Programming) in the Vitis Application Acceleration Development Flow Documentation (UG1393), there are several key steps the host application must follow: 

1. Specifying the accelerator device ID and loading the `.xclbin`
2. Setting up the kernel and kernel arguments
3. Transferring data between the host and kernels
4. Running the kernel and returning results

This tutorial will walk you through each of these, and discuss the requirements of these steps when working with user-managed RTL kernels.

## Setting Up the XRT Native API

```
#include <iostream>
#include <cstring>

// XRT includes
#include "xrt/xrt_bo.h"
#include "xrt/xrt_device.h"
#include <experimental/xrt_xclbin.h>
#include <experimental/xrt_ip.h>
```

The XRT native API requires some `#include` statements to support the various class objects and functions of the different elements of the system. In the code sample above you can see the inclusion of header files for the Xilinx device, the device binary (`.xclbin`), the buffer objects (`xrt_bo`), and the user-managed kernel (`xrt_ip`). This last is an important difference in the XRT native API when working with XRT-managed kernels and user-managed kernels. 

XRT managed kernels (`xrt::kernel`) meet specific requirements of control protocols and interfaces, as described in [Requirements of an RTL Kernel](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Requirements-of-an-RTL-Kernel) and as such have a richer feature set in the XRT API. User-managed kernels are a bit more unstructured, and so are represented in the IP class with limited features. 

## Specifying the Device ID and Loading the XCLBIN

```
    // Read settings
    std::string binaryFile = "./vadd.xclbin";
    auto xclbin = xrt::xclbin(binaryFile);
    int device_index = 0;

    std::cout << "Open the device" << device_index << std::endl;
    auto device = xrt::device(device_index);
    std::cout << "Load the xclbin " << binaryFile << std::endl;
    auto uuid = device.load_xclbin(binaryFile);
``` 

In the Vitis application acceleration development flow you must specify a Xilinx device binary file, or `xclbin`. This is an executable to be loaded into the Xilinx device and run by the accelerator card or platform. You must also identify the Xilinx device to load the xclbin into. In some systems there may be multiple accelerator cards with multiple devices. So you must specify the device as well as the `xclbin`. 

There are a number of ways to identify both the device and binary file during execution. You can hard code them, as has been done for the device in this tutorial, specify them from the command line as as been done for the `.xclbin` file here, or you can specify them in other ways as demonstrated in the [Vitis_Accel_Examples](https://github.com/Xilinx/Vitis_Accel_Examples). 

In the code example above, taken from the `user-host.cpp` file in this tutorial, the device is simply `device_index` 0, and the binary file is passed from the command line. The code: 

1. Identifies the Xilinx binary file: `xrt::xclbin(binaryFile)`
2. Opens the device: `xrt::device(device_index)`
3. Loads the xclbin into the device: `device.load_xclbin(binaryFile)`

## Setting up the kernel and kernel arguments

For XRT-managed kernels the kernel is identified as an xrt::kernel object. However, for user-managed RTL kernels the host application uses the `xrt::ip` object to identify the kernel in the `xclbin`, as shown in the code example below. In addition, the host application identifies any compute units (CUs) of the kernel in the `xclbin`. The CU is an instance of the kernel, as described in [Creating Multiple Instances of a Kernel](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Creating-Multiple-Instances-of-a-Kernel). In the code example below there are three user-managed kernels identified by the host application: ip1, ip2, and ip3. This enables the host to run multiple instances of the kernel at the same time when available in the `xclbin`. 

>NOTE: `ip2` and `ip3` are added for example purposes, and are not actually used in the host application. 

```
    auto ip1 = xrt::ip(device, uuid, "Vadd_A_B:{Vadd_A_B_1}");
    auto ip2 = xrt::ip(device, uuid, "Vadd_A_B:{Vadd_A_B_2}");
    auto ip3 = xrt::ip(device, uuid, "Vadd_A_B:{Vadd_A_B_3}");

    std::cout << "Allocate Buffer in Global Memory\n";
    //auto boA = xrt::bo(device, vector_size_bytes, krnl.group_id(1)); //Match kernel arguments to RTL kernel
    //auto boB = xrt::bo(device, vector_size_bytes, krnl.group_id(2));
    auto ip1_boA = xrt::bo(device, vector_size_bytes, 0);
    auto ip1_boB = xrt::bo(device, vector_size_bytes, 1);
    //auto ip2_boA = xrt::bo(device, vector_size_bytes, 0);
    //auto ip2_boB = xrt::bo(device, vector_size_bytes, 1);
    //auto ip3_boA = xrt::bo(device, vector_size_bytes, 0);
    //auto ip3_boB = xrt::bo(device, vector_size_bytes, 1);
```

In XRT-managed kernels the XRT API can create a buffer for the memory bank used by a specific argument. This is done using the `krnl.group_id(1)` as shown for buffer objects `boA` and `boB` in the example above. Just as with the XRT managed kernels, the `xrt::bo` object is used to create buffers for the function arguments, or ports of the RTL kernel. However, unlike XRT managed kernels, the memory that the kernel argument is mapped to must be manually specified as shown for buffer objects `ip1_boA` and `ip1_boB` in the code example above.


For user-managed kernels, there is no equivalent method to map the kernel argument to the memory bank it uses. However, you either know this information because you built the user-managed kernel, or you can determine the information by examining the `xclbin.info` file produced by the `xclbinutil` command as described in the [Vitis Environment Reference Materials](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Vitis-Environment-Reference-Materials). 

>**TIP:** The `xclbin.info` report is generated by the Vitis compiler while linking the kernels and generating the `xclbin` file. 

You must determine the memory bank used for specific arguments in the kernel, and so create a buffer for the argument associated with that memory bank. Then you must use that buffer for the specified kernel argument when performing a `read_register` or `write_register` command for the kernel, as shown below: 

```
ip.write_register(register offset for argument A, address of buffer for argument A)`  
```

This information is hard-coded in this `user-host.cpp` application. However, there are examples of different coding techniques demonstrated in the [Vitis_Accel_Examples](https://github.com/Xilinx/Vitis_Accel_Examples). 

## Transferring Data

After creatng and filling the buffers with data from the host application, you must then sync the buffer to the Xilinx device to make the data accessible to the device binary (`xclbin`). The data is transferred from the host computer to the Xilinx device and accelerator card where it is available for the kernel to use, as shown in the code example below: 

```
    // Map the contents of the buffer object into host memory
    auto bo0_map = ip1_boA.map<int*>();
    auto bo1_map = ip1_boB.map<int*>();
 
    std::fill(bo0_map, bo0_map + DATA_SIZE, 0);
    std::fill(bo1_map, bo1_map + DATA_SIZE, 0);

    // Create the test data
    int bufReference[DATA_SIZE];
    for (int i = 0; i < DATA_SIZE; ++i) {
        bo0_map[i] = i;
        bo1_map[i] = i;
        bufReference[i] = bo0_map[i] + bo1_map[i]; //Generate check data for validation
    }

    std::cout << "loaded the data" << std::endl;
    uint64_t buf_addr[2];
    // Get the buffer physical address
    buf_addr[0] = ip1_boA.address();
    buf_addr[1] = ip1_boB.address();

    // Synchronize buffer content with device side
    std::cout << "synchronize input buffer data to device global memory\n";
    ip1_boA.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    ip1_boB.sync(XCL_BO_SYNC_BO_TO_DEVICE);

```

In the example code, you can see that the addresses of the buffers are captured in the `buf_addr` vector. This information will be needed when writing or reading to the registers associated with the kernel arguments. 

## Running the kernel and returning results

For XRT managed kernels, running the kernel is a simple matter of creating a run object as shown in the code below. XRT manages the start and stop of the kernel. You simply need to wait for the results. 

```
    //std::cout << "Execution of the kernel\n";
    //auto run = krnl(DATA_SIZE, boA, boB); //DATA_SIZE=size
    //run.wait();
```

However, in user-managed kernels XRT doesn't know how the kernel is started or stopped. That is in your hands. User-managed kernels offer a significant degree of flexibility and permit creative solutions to design problems because they are not constrained to the `ap_ctrl_hs` or `ap_ctrl_chain` control protocols. There is a lot of capability, but it is up to you to manage it. 

You must first setup the kernel, by writing to the registers to pass associated buffer addresses for the various kernel arguments. Then you must trigger the kernel execution by setting a register value or enabling a signal bit, followig the control protocol designed by the user and implemented by the kernel.

```
    std::cout << "INFO: Setting IP Data" << std::endl;
    std::cout << "Setting Register \"A\" (Input Address)" << std::endl;
    ip1.write_register(A_OFFSET, buf_addr[0]);
    ip1.write_register(A_OFFSET + 4, buf_addr[0] >> 32);

    std::cout << "Setting Register \"B\" (Input Address)" << std::endl;
    ip1.write_register(B_OFFSET, buf_addr[1]);
    ip1.write_register(B_OFFSET + 4, buf_addr[1] >> 32);

    uint32_t axi_ctrl = IP_START;
    std::cout << "INFO: IP Start" << std::endl;
    //axi_ctrl = IP_START;
    ip1.write_register(USER_OFFSET, axi_ctrl);

    // Wait until the IP is DONE
    int i = 0;
    //axi_ctrl = 0;
    while (axi_ctrl != IP_IDLE) {
    //while ((axi_ctrl & IP_IDLE) != IP_IDLE) {
        axi_ctrl = ip1.read_register(USER_OFFSET);
        i = i + 1;
        std::cout << "Read Loop iteration: " << i << " and Axi Control = " << axi_ctrl << "\n";
        if (i > 100000) {
	  axi_ctrl = IP_IDLE;
          ip1.write_register(USER_OFFSET, axi_ctrl);
        }
    }
```

## Next Steps

Now you have created the RTL Kernel (`.xo`) file from a Vivado IP, and examined the host coding requirements of the native XRT API. Now you can create a Vitis application project to [test the RTL Kernel](./using_the_rtl_kernel.md).
</br>

<hr/>
<p align="center" class="sphinxhide"><b><a href="/README.md">Return to Main Page</a> — <a href="./README.md">Return to Start of this Tutorial</a></b></p>

<p align="center" class="sphinxhide"><sup>Copyright&copy; 2021-2022 Xilinx</sup></p>

