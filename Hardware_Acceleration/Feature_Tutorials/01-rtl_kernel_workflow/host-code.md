<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2021.2 Vitis™ Application Acceleration Development Flow Tutorials</h1>
   <a href="https://github.com/Xilinx/Vitis-Tutorials/tree/2020.2">See 2020.2 Vitis Application Acceleration Development Flow Tutorials</a>
   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# Host Code Programming

User-managed RTL kernels are only supported in host applications written with the XRT native API, as described in the [XRT documentation](https://xilinx.github.io/XRT/master/html/xrt_native_apis.html). The XRT native API provides a rich set of class objects and functions for XRT-managed kernels, as discussed in the *XRT Native API Host Application* tutorial, and for user-managed kernels as described here. 

Open the reference-files/src/host/user-host.cpp file to examine its content and structure. As decribed in [Host Programming](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/devhostapp.html#vpy1519742402284) in the Vitis Application Acceleration Development Flow Documentation (UG1393), there are several key steps the host application must follow: 

1. Specifying the accelerator device ID and loading the `.xclbin`
2. Setting up the kernel and kernel arguments
3. Transferring data between the host and kernels
4. Running the kernel and returning results

This tutorial will walk you through each of these, amd discuss the requirements of these steps when working with user-managed RTL kernels.

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

The XRT native API requires some `#include` statements to support the various class objects and functions of the different elements of the system. In the code sample above you can see the inclusion of header files for the Xilinx device, the device binary (`.xclbin`), the buffer objects (`xrt_bo`), and the user-managed kernel (`xrt_ip`). This last is an important difference in the XRT native API when working with XRT-managed kernels and user-managed kernels. XRT managed kernels (`xrt::kernel`) meet specific requirements of control protocols and interfaces, as described in [Requirements of an RTL Kernel](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/devrtlkernel.html#qbh1504034323531) and as such have a richer feature set in the XRT API. User-managed kernels are a bit more unstructured, and so are represented in the IP class with limited features. 

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

There are a number of ways to identify both the device and binary file during execution. You can hard code them, as has been done in this tutorial, or you can specify them in other ways as demonstrated in the [Vitis_Accel_Examples](https://github.com/Xilinx/Vitis_Accel_Examples). 

In the code example above, taken from the user-host.cpp file in this tutorial, the device is simply `device_index` 0, and the binary file is hardcoded as `vadd.xclbin`. The code: 

1. Identifies the Xilinx binary file: `xrt::xclbin(binaryFile)`
2. Opens the device: `xrt::device(device_index)`
3. Loads the xclbin into the device: `device.load_xclbin(binaryFile)`

## Setting up the kernel and kernel arguments

```
    size_t vector_size_bytes = sizeof(int) * DATA_SIZE;

    xrt::xclbin::mem mem_used;
    auto ip = xrt::ip(device, uuid, "Vadd_A_B");

    std::cout << "Fetch compute Units" << std::endl;
    std::vector<xrt::xclbin::ip> cu;
    for (auto& kernel : xclbin.get_kernels()) {
        if (kernel.get_name() == "Vadd_A_B") {
            cu = kernel.get_cus();
        }
    }

    if (cu.empty()) throw std::runtime_error("IP krnl_vadd_rtl not found in the provided xclbin");

    std::cout << "Determine memory index\n";
    for (auto& mem : xclbin.get_mems()) {
        if (mem.get_used()) {
            mem_used = mem;
            std::cout << "Mem  Used Index: " << mem_used.get_index() << std::endl;
            break;
        }
    }
```

For user-managed RTL kernels, the host application uses the `xrt::ip` object to identify the kernel in the `xclbin`, as shown in the code snippet above. In addition, the host application identifies any compute units (CUs) of the kernel in the `xclbin`. The CU is an instance of the kernel, and enables the host to run multiple instances of the kernel at the same time when available in the `xclbin`. 

Also, the host application needs to identify the memory used by the CUs to connect to the global memory. The `mem_used` variable in the code example above indicates which memories the IP is connected to. 


## Defining Buffers and Transferring Data

Just as with the XRT managed kernels, the `xrt::bo` object is used to create buffers for the function arguments, or ports of the RTL kernel. However, unlike XRT managed kernels, the memory that the kernel argument is mapped to must be manually specified as shown in the code example below. You can see the original command for the XRT managed kernel shown next to the updated command required for the user-managed kernel: 

```
    std::cout << "Allocate Buffer in Global Memory\n";
    //auto boA = xrt::bo(device, vector_size_bytes, krnl.group_id(1)); //Match kernel arguments to RTL kernel
    //auto boB = xrt::bo(device, vector_size_bytes, krnl.group_id(2));
    auto boA = xrt::bo(device, vector_size_bytes, mem_used.get_index());
    auto boB = xrt::bo(device, vector_size_bytes, mem_used.get_index());
```

While the XRT managed kernel has an API to extract the kernel argument (`krnl.grp_id()`), the user-managed kernel must manually identify the memory index, or hard code it as shown in the following example. Note the last argument of `xrt::bo` is the enumerated index of the memory bank as specified by the `v++ --conectivity.sp` option as described at [Mapping Kernel Ports to Memory](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Mapping-Kernel-Ports-to-Memory). The bank index can be obtained by xbutil examine --report memory command.

```
   auto buf_in_a = xrt::bo(device, DATA_SIZE, xrt::bo::flags::host_only, 8);
```

After creatng and filling the buffers with data from the host application, you must then sync the buffer to the Xilinx device to make the data accessible to the device binary (`xclbin`). The data is transferred from the host computer to the Xilinx device and accelerator card, where it is available for the kernel to use: 

```
    // Synchronize buffer content with device side
    std::cout << "synchronize input buffer data to device global memory\n";
    boA.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    boB.sync(XCL_BO_SYNC_BO_TO_DEVICE);
```


## Running the kernel and returning results

After transferring the data, you simply write to registers in the kernel to trigger the memory read by passing the address offsets for the `m_axi` or `s_axilite` interfaces as shown below. Keep in mind that `args[0]` refers to the scalar DATA_SIZE argument, `args[1]` and `args[2]` are the `m_axi` interfaces that manages the bulk of data needed by the kernel. 

```
    //std::cout << "Execution of the kernel\n";
    //auto run = krnl(DATA_SIZE, boA, boB); //DATA_SIZE=size
    //run.wait();

    std::cout << "INFO: Setting IP Data" << std::endl;
    auto args = cu[0].get_args();

    std::cout << "Setting the Scalar Register \"size\"" << std::endl;
    ip.write_register(args[0].get_offset(), DATA_SIZE);

    std::cout << "Setting the Register \"A\" (Input Address)" << std::endl;
    ip.write_register(args[1].get_offset(), buf_addr[0]);
    ip.write_register(args[1].get_offset() + 4, buf_addr[0] >> 32);

    std::cout << "Setting the Register \"B\" (Input Address)" << std::endl;
    ip.write_register(args[2].get_offset(), buf_addr[1]);
    ip.write_register(args[2].get_offset() + 4, buf_addr[1] >> 32);

    uint32_t axi_ctrl = IP_START;
    std::cout << "INFO: IP Start" << std::endl;
    //axi_ctrl = IP_START;
    ip.write_register(USER_OFFSET, axi_ctrl);

    // Wait until the IP is DONE
    int i = 0;
    //axi_ctrl = 0;
    while (axi_ctrl != IP_IDLE) {
    //while ((axi_ctrl & IP_IDLE) != IP_IDLE) {
        axi_ctrl = ip.read_register(USER_OFFSET);
        i = i + 1;
        std::cout << "Read Loop iteration: " << i << " and Axi Control = " << axi_ctrl << "\n";
        if (i > 100000) {
	  axi_ctrl = IP_IDLE;
          ip.write_register(USER_OFFSET, axi_ctrl);
        }
    }

    std::cout << "INFO: IP Done" << std::endl;
    // Get the output;
    std::cout << "Get the output data from the device" << std::endl;
    boB.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
```

## Next Steps

Now you have created the RTL Kernel (.xo) file from a Vivado IP, and examined the host coding requirements of the native XRT API, you can create a Vitis application project to [Test the RTL Kernel](./using_the_rtl_kernel.md).
</br>

<hr/>
<p align="center" class="sphinxhide"><b><a href="/README.md">Return to Main Page</a> — <a href="./README.md">Return to Start of this Tutorial</a></b></p>

<p align="center" class="sphinxhide"><sup>Copyright&copy; 2021 Xilinx</sup></p>

