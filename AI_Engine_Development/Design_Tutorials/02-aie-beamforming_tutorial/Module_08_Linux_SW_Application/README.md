# Building the Design
Next, we’ll create the Linux PS Host Application
```
make exe
```
The individual commands are explained later on in this README.md 

# Introduction: Programming the PS Host Application 
A top-level PS application running on the Corex-A72 processor controls the AI Engine graph and the PL kernels. In Module 05, we created a PS Host Application for a bare-metal system. In this Module, we will create a PS Host Application that runs on a Linux Operating System (which we built in Module 07).
A detailed documentation on compiler flags, include directories, and linker flags are described in [AI Engine Documentation: Integrating the Application Using the Vitis Tools Flow, Compile the Embedded Application for the Cortex-A72 Processor](https://www.xilinx.com/html_docs/xilinx2020_2/vitis_doc/integrate_ai_engine_application.html#ariaid-title7)

For this tutorial design, XRT is used for AI Engine control and generic UIO drivers are used to control the PL kernels. 

# Execution Flow Chart
The Linux PS Host Application is the beamformer.cpp file. Below is the execution flow diagram the beamformer.cpp implements. 

![Linux PS Host Application Execution Flow](images/Linux_Host_Application_Execution_Flow.png)

Next, we'll explain some important steps in the execution flow.  

# Bind UIO drivers with PL kernels
Remember in Module 07, we created the Petalinux software platform with a custom DTSI file that listed out the generic UIO drivers for each PL kernel instance. Each UIO driver in the DTSI file has the same name as each PL kernel instance name we specified in the config_2regslice.ini in Module 04. 

One of the first things the Host Application does is bind the UIO drivers to the PL kernels. It searches for the UIO devices in the system for the known PL kernel names. This is done by calling the `dlbf_assign_addr` and `ulbf_assign_addr` functions defined in the `src/utils/utils_dlbf.cpp` and `src/utils/utils_ulbf.cpp` files.

These functions call the `mmap` function to bind the physical address of the PL kernels to a memory pointer. The host application uses the memory pointer to access the PL kernels' registers maps.   

# Load AIE XCLBIN   
Next, the host application resets the AI Engine array, loads the AI Engine array, and enables the AI Engine graph. 

## Reset AI Engine
Host application can reset and reload the AIE array whenever needed. In order to do this, XRT has to be built into the Petalinux build. In the host application, user can use the following API to reset the the AI Engine with the `xrtResetAIEArray` function. 
```C++
 auto dhdl   = xrtDeviceOpen(0);
 ...
reset_done = xrtResetAIEArray(dhdl);
if (reset_done == -1){
    printf("[Beamformer] AIE reset FAILS \n");
    log_plnx << "[Beamformer] AIE reset FAILS" <<std::endl;
    printf("[Beamformer] Exiting the application as it cannot be run\n");
    log_plnx << "[Beamformer] Exiting the application as it cannot be run" <<std::endl;
    return 1;
} 
else if(reset_done == 0) {
    printf("[Beamformer] AIE reset done successfully\n");
    log_plnx << "[Beamformer] AIE reset done successfully" << std::endl;
}
```
If the `xrtResetAIEArray` function fails, AI Engine reset has not been accomplished successfully and system reboot maybe needed.

[API Documentaton](https://github.com/Xilinx/XRT/blob/a155e1630d18884c9f82d71f3a0b4e8a91074069/src/runtime_src/core/include/experimental/xrt_aie.h) 

This function does not orchestrate system reset but ONLY resets AI Engine array.

## Load AI Engine with XCLBIN 
Next, the host application loads the AI Engine with the XCLBIN with the xclloadxclbin() function. 

The following snippet of code shows usage in the application shows how to load AIE.

[API Documentation](https://xilinx.github.io/XRT/master/html/xrt.main.html?highlight=xclloadxclbin#c.xclLoadXclBin) 

```C++
//In load_xclbin()
...
if (xclLoadXclBin(device, top))
 throw std::runtime_error("Bitstream download failed");
...
```
If the AI Engine load fails, the `load_xclbin()` function would throw an exception. This may need system reboot. 

## Reset AI Engine in the Middle of Execution
Sometimes it is nessecary to stop a running execution of the system and start over. In order to restart the system and start again, there's a flow to follow. These steps can be done any number of times in the host application.
 
**1. Quiesce PL kernels (i.e stop PL<->AIE data paths)** 

As a pre-requisite, the PL kernels should have the ability to be quiesced by host application. All the examples shown in this design have this feature via the start register (i.e. all PL kernels are AXI-MM slaves to PS). The PS communicates with PL kernels to hold them in a state of "idle" before reset/load of AI Engine. 
   
**2. Reset AI Engine** 

Next, reset the AI Engine using the xrtResetAIEArray function. Make sure to verify the return value and proceed if it is SUCCESSFUL. Otherwise, reboot the system. 
   
**3. Load AI Engine with the XCLBIN**

Next, reload AI Engine with the XCLBIN using the `xclloadxclbin()` function. Verify the return value and proceed if it is SUCCESSFUL. Otherwise, reboot the system.
   
**4. Re-start PL kernels**

Lastly, restart the PL kernels to send/recieve PL traffic to/from the AI Engine.


## Command Line Arguments
The beamformer.cpp takes 2 command line arguments:
```
./beamformer.exe <argv[1]> a.xclbin
```
The `a.xclbin` is the AI Engine PDI. The user can specify the \<argv\[1\]\> variable that the program will execute a different test. Below is a table of the test options: 

| argv\[1\]  | Description  |
| -------- |-----|
| 0      |  <ul><li>Reset/ Load AIE</li><li>Functional test for N iter</li><li>Reset/ Load AIE </li><li>Functional test for N iter </li><li>Reset/load AIE </li><li>PERF test with one input and one output PLIO of each sub graphs </li></ul> | 
| 1      | <ul><li>Reset/ Load AIE</li><li>Functional test for 100k iters w data integrity test every N iters</li><li>Reset/ Load AIE</li><li>PERF test for all PLIOs  </li></ul>|
|2|Continuously run the functional test for infinite number of iters. To exit, hit ctrl c. This does not guarantee graceful exit. System reboot is required. This mode helps measure power of the system while it runs continuously. |
