# Building the Design
Next, we will create two Bare-metal PS Host Applications: 
```
make application MAIN=main_partial
make application MAIN=main_full
``` 
The individual commands are explained later on in this README.md

# Introduction: Building a Baremetal System
The Vitis™ Unified Software Development Environment provides a variety of Xilinx software packages, including drivers, libraries, board support packages, and complete operating systems to help you develop a software platform. In this module, we will create a Standalone platform (a single-threaded, simple operating system (OS) platform) that provides the lowest layer of software modules used to access processor-specific functions. We will then compile a baremetal application that orchestrates the data flow between the PL kernels and AI Engine graph. In module 07 and 08, we will create a Petalinux software platform to run Linux applications which conduct functional and performance tests.

TODO: why are baremetal system needed first before Linux? 

# Building the Design 
Building bare-metal applications can be broken down into 3 parts: generating the platform, compiling the PS Application source code, and linking the PS Application source code. These steps are repeated to create two bare-metal PS Host Applications (main_partial.elf and main_full.elf). 

## Difference between main_partial.cpp and main_full.cpp
You’ll notice there are two main files: main_partial.cpp and main_full.cpp. The only difference between them is that the main_partial.cpp runs through 10 complex data samples (tdata) per iteration while main_full.cpp runs through TODO: larger set of data. The main_partial.cpp was created to run through hardware emulation in a timely manner. In Module06, we will run the main_partial.elf for hardware emulation and main_full.elf on the hardware run on the VCK190 board.

## Generating the platform 
Building baremetal applications requires a baremetal domain in the platform. In module 01, we created a custom platform with a baremetal domain. In module 04 (Linking the System), we built on top of our custom platform and added PL kernels (and subsequently their PS drivers) to the design. The first step to building the Baremetal PS Host Application is to use the XSA generated during the link process to create a new software platform using the following command. 

```
generate-platform.sh -name vck190_baremetal             \
                     -hw  ../../Module_04_AI_Engine_and_PL_Integration/build/rev1/hw/beamforming.rev1.hw.xsa \
                     -domain ai_engine:aie_runtime      \
                     -domain psv_cortexa72_0:standalone \
                     -out-dir build

```
The options are explained in the AI Engine Documentation: Building a Bare-metal System.  

## Compiling the PS Application Source Code    
Next we need to compile our PS Host Application (main_partial.cpp or main_full.cpp) and additional source code our PS Host Applications need which are stored in the baremetal_src/ folder. Our Bare-metal PS Host Application only controls the PL kernels, and therefore we do not need to compile the aie_control.cpp file in this design.  

Compile the main_partial.cpp file using the following command: 
```
aarch64-none-elf-g++ -c -Wall -O0 -g -mcpu=cortex-a72 -MMD -MP \
                     -I=build/vck190_baremetal/psv_cortexa72_0/standalone_domain/bsp/psv_cortexa72_0/include  \  
                     -o build/main_partial.o main_partial.cpp 

```

In this step, we need to include the BSP include files from the generated platform. This command create a `build/main_partial.o` object. The same options are used when compiling the main_full.cpp file.   

Next, compile the helper source code stored the baremetal_src/\*.cpp files. Below is an example of how to compile the dlbf_din.cpp file. The rest of the baremetal_src/\*.cpp files are compiled the same way. 

```
aarch64-none-elf-g++ -c -Wall -O0 -g -mcpu=cortex-a72 -MMD -MP \
                     -I=build/vck190_baremetal/psv_cortexa72_0/standalone_domain/bsp/psv_cortexa72_0/include \   
                     -o build/dlbf_din.o baremetal_src/dlbf_din.cpp 
```
This step creates a build/\*.o objects for each file in the baremetal_src/\*.cpp. 

## Linking the PS Application Source Code  
Lastly, we link the PS Application object (main_partial.o) with the baremetal_src object files: 

```
aarch64-none-elf-g++ build/main_partial.o \
                     build/dlbf_cin.o       \
                     build/dlbf_din.o       \
                     build/dlbf_gold0.o     \
                     build/ulbf_cin.o       \
                     build/ulbf_din.o       \
                     build/ulbf_gold0.o     \
                     build/utils.o          \
                     build/utils_dlbf.o     \
                     build/utils_ulbf.o     \
                     -Wl, -T -Wl,./lscript.ld -mcpu=cortex-a72 -Wl,     \
                     --start-group,-lxil,-lgcc,-lc,-lstdc++,--end-group \
                     -L build/vck190_baremetal/psv_cortexa72_0/standalone_domain/bsp/psv_cortexa72_0/lib \
                     -o build/main_partial_ps.elf
```
During this step, we need the BSP libxil.a which is linked by including the BSP library from the generated platform. This command creates the baremetal executable (main_partial.elf). The main_full.elf file is created the same way. 

# Baremetal Source Code
Next, we'll dive into the baremetal_src code and describe what each file's purpose is. Open the baremetal_src/\*.cpp files to get a sense of what the source code does. A summary of the files is given below. 

**dlbf_din.cpp ulbf_din.cpp dlbf_cin.cpp ulbf_cin.cpp**

These files contain the data input for the dlbf_data ulbf_data dlbf_coeffs and ulbf_coeffs PL kernels. These PL kernels were already initialized with the input data from the `.mem` files. The PS Host Application checks that PL kernels got initialized correctly by comparing the BRAM content in the PL kernels with the data in these files.

**dlbf_gold0.cpp ulbf_gold0.cpp**

These files contain the golden data output we expect from the AI Engine. The AI Engine generates the output data and stores it in the dlbf_slave and ulbf_slave PL kernels. The PS Host Applications checks the URAM content in these PL kernels and compares it to the expected golden output in these files. 

**dlbf_utils.cpp ulbf_utils.cpp dlbf_utils.h ulbf_utils.h**

These files contain utility functions that help the main PS Host application check the input data, verify the output data, reset the PL IPs, and more. 

Open the dlbf_utils.cpp file, and you'll notice there are three global variable arrays: `dlbfDataAddr`, `dlbfCoeffsAddr`, and `dlbfSlaveAddr`. They are arrays that contain variables that start with `XPAR_*`. These variables definitions are located in the `psv_corexa72_0/standalone_domain/bsp/psv_cortexa72_0/include/xparameters.h` file. The xparameters are the base addresses the PS Host Application uses to access the PL kernels' control and status registers. 

**utils.h utils.cpp**

These files contain functions that are common to the dlbf and ulbf operations. For now, it contains one function `extractIQ` which returns the imaginary and real parts of a given integer. 

**params.h**
This file contains all the global variables that are used in the PS Host Application. You’ll notice that each PL kernel has its own set of global defines in this file. 

Remember in Module 3, we learned that the PL kernels contain a control status register (CSR) block. The PL kernels were designed so that CIPS block could access the registers in the CSR block to control the Data Masters or the RAM Slave in the PL kernels. The CIPs block contains the A72 processor which the PS host application runs on. These global defines are the addresses the host application uses to directly access the CSR registers in the PL kernels.  Below is a table of the dlbf_data CSR registers. 

**Control and Status Register Address Map**

| Register Space Offset | Bits and Name | R/W? | Global Defines (params.h) | Description |  
|  ---  |  ---  |  ---  | --- | --- |
| 0x0 | \[31:0\] ID | R | DLBF_DATA_REG_OFFSET_ID | 32 bit ID Register.|
| 0x4 | \[0\] RESET | W | DLBF_DATA_REG_OFFSET_RESET | 1:assert, 0:de-assert. Also assigned to the `m_axis_rst_bram` input in CRS module. | 
| 0x4 | \[4\] GO | W | DLBF_DATA_REG_OFFSET_START | 1:Start PL traffic, 0: Stop PL traffic. Also assigned to the `go_bram` input in CRS module. | 
| 0x8 | \[11:0\] BLOCK_SIZE | W | DLBF_DATA_REG_OFFSET_BLOCK_SIZE | Sets the block size of stream frame. TLAST is asserted for every "Block Size" number of cycles. Also assigned to the `block_size_bram` input in the CRS module. |  
| 0xC | \[11:0\] NITER | W| DLBF_DATA_REG_OFFSET_NITER | Sets the number of iterations of the data to go through. If this set to 0m data will be transmitted to the AIE forever. Also assigned to the `niter_bram` input in the CRS module. The main_partial.cpp sets this to 4. The main_full.cpp sets this is TODO. | 
| 0x10 | \[15:0\] ROLLOVER_ADDR | W | DLBF_DATA_REG_OFFSET_ROLLOVER | When BRAM address reaches rollover address, it will reset to 0. Also assigned to the `rollover_addr_bram` input in the CRS module. | 
| 0x20 | \[0\] MASTER_DONE | R | DLBF_DATA_REG_OFFSET_DONE | When this status register is 1'b, the Data Master is done sending data to AI Engine. Also assigned to the `m0_done_bram` input in the CRS module. |

All the PL Master kernels (dlbf_data, dlfbf_coeffs, ulbf_data, ulbf_coeffs) also contain multiple PL Data Masters (BRAMs). The dlbf_data and dlbf_coeffs have 4 Data Masters. The ulbf_data and ulbf_coeffs have 8 data masters. Each of these Data Masters also have their own set of CRS registers. The PS Host Application can access each PL data master register by adding the dlbf_data xparameter + Data Master’s offset + the CRS offset + CRS register offset. Below is a list of dlbf_data Data Masters’ offsets and the CRS Offset: 

**Register Address Map**

| Register Space Offset | Bits and Name | R/W? | Global Defines (params.h) | Description |  
|  ---  |  ---  |  ---  | --- | --- |
|0x0000_0000|--|R| DLBF_DATA_RAM0_OFFSET | Master 0 data offset. |
|0x0010_0000|--|R| DLBF_DATA_RAM1_OFFSET | Master 1 data offset. |
|0x0020_0000|--|R| DLBF_DATA_RAM2_OFFSET | Master 2 data offset. |
|0x0030_0000|--|R| DLBF_DATA_RAM3_OFFSET | Master 3 data offset. |
|0x0008_0000|--|R| DLBF_DATA_CSR_OFFSET  | CSR offset            |

For example, if the PS Host Application wanted to write to the RESET register of the Data Master 0 in the dlbf_data_00 PL kernel, they would write to the following address:
```
RESET0_ADDR = XPAR_DLBF_DATA_00 + DLBF_DATA_RAM0_OFFSET + DLBF_DATA_CSR_OFFSET + DLBF_DATA_REG_OFFSET_RESET
```

The rest of the PL Master kernels (dlbf_data, dlbf_coeffs, ulbf_data, ulbf_coeffs) also have the similar register address mappings. 

The dlbf_slave PL kernel's control and status registers are shown below:

**Register Address Map**

| Register Space Offset | Bits and Name | R/W? | Global Defines (params.h) | Description |  
|  ---  |  ---  |  ---  | --- | --- |
| 0x0 | \[31:0\] ID | R | DLBF_SLAVE_REG_OFFSET_ID | 32 bit ID Register.|
| 0x4 | \[0\] RESET | W | DLBF_SLAVE_REG_OFFSET_RESET | 1:assert, 0:de-assert. Also assigned to the `slave_rst_bram` input in CRS module. | 
| 0xC | \[11:0\] NITER | W | DLBF_SLAVE_REG_OFFSET_NITER | Sets the number of iterations of the data to go through. If this set to 0m data will be transmitted to the AIE forever. Also assigned to the `niter_bram` input in the CRS module. The main_partial.cpp sets this to 4. The main_full.cpp sets this is TODO. | 
| 0x20 | \[0\] SLAVE_DONE | R | DLBF_SLAVE_REG_OFFSET_DONE | When this status register is 1'b, the RAM slave is done recieving data to AI Engine. Also assigned to the `rxdone_bram` input in the CRS module. | 

Each Data Slave PL kernel (dlbf_slave and ulbf_slave) contain only one RAM Slave (URAM). The PS Host Application can access each RAM Slave module by adding the CRS offset (0x0008_0000) to the CRS register offset. For example, if to access the NITER register, you would write to the following address:

```
NITER_ADDR = DLBF_SLAVE_CSR_OFFSET + DLBF_SLAVE_REG_OFFSET_NITER
```
The ulbf_slave PL kernel also has the same register address mapping, and can its CSR registers are accessed the same way. 

# PS Host Application
Now, we'll review the PS Host Application and understand how it orchistrates the data flow between the PL kernels and the AI Engine. The PS Host Application also verifies the output data stored in the dlbf_slave and ulbf_slave PL kernels by comparing it to golden reference data. 

## Main Function 
Open the main_partial.cpp source code and review the main function. You’ll notice it calls two functions: test_dlbf and test_ulbf. If either of them return 0, then the test failed. If both of the return 1, then the test passed. The test_ulbf function is structured the same as the test_dlbf function. 

## dlbf_test/ulbf_test Function
We will do through test_dlbf function, and we will leave it up to the user to review the test_ulbf function. The diagram below shows how the execution flow of the test_dlbf and test_ulbf functions. 

![PS Host Application Execution Flow](images/Mod5_application_execution_flow.PNG) 

## Reset
The first thing the `test_dlbf` function does is call the `dlbf_reset` function (defined in the `utils_dlbf.cpp`). This function resets the dlbf_data, dlbf_coeff, and dlbf_slave PL kernels. This is done by asserting and deasserting the `DLBF_DATA_REG_OFFSET_RESET` register using the `Xil_Out32`. The `Xil_Out32` function is part of the Xilinx Hardware Abstraction Layer API in the Standalone Library. This API is used throughout this PS Host Application. See UG643 for the full API documentation. 

## Configuration
Next, the `test_dlbf` function calls the `dlbf_data_config_ips`, `dlbf_coeffs_config_ips`, and `dlbf_slave_config_ips` functions. The functions configure the `BLOCK_SIZE`, `NITER`, and `ROLLOVER_ADDR` registers.

| PL Kernel | BLOCK_SIZE | NITER | ROLLOVER_ADDR |  
|  ---  |  ---  |  ---  |  ---  |
| dlbf_data | 768/2 | 4 | 1536 |
| dlbf_coeffs | 512/2 | 4 | 1024 |
| dlbf_slave | 768 | 4 | -- |


## Check RAM
Next, the `dlbf_data_check_ram` and `dlbf_coeffs_check_ram` functions are called. These functions are optional, but they are useful for debugging. In Module 03, the BRAMs in dlbf_data and dlbf_coeffs PL kernel got initialized to input data stored in hex.mem files. These check_ram functions ensure that the dlbf_data and dlbf_coeffs RAMs got initialized to the correct values. The golden input data is stored in the `dlbf_din.cpp` and `dlbf_coeffs.cpp`.

## Start
Next, the `dlbf_start` function is called. This function asserts the `GO` register bit for the dlbf_data and dlbf_coeffs PL kernels to start the PL traffic to the AI Engine. 

## Wait for Done: Inputs
Next, the `dlbf_data_wait_for_done` is called continuously in a while loop. This function reads the `MASTER_DONE` register on each dlbf_data PL kernel. When all four dlbf_data PL kernels have a status of DONE, the function returns a 1'b which breaks the while loop. The while loop only calls the function 100 times maximum, and times out if the dlbf_data kernels were not done by then.

After the dlbf_data kernels are done sending their Data to the AI Engine, we then wait for the dlbf_coeffs to send their data to the AI Engine. If the dlbf_data or dlbf_coeffs timeout, then the test fails. 

## Wait for Done: Outputs
Next, we wait for the dlbf_slave kernels to be done receiving output data from the AI Engine. The dlbf_slaves are done when  NITER\*BLOCKSIZE number of 32-bit complex data samples (tdata) are recieved from the AI Engine. If the dlbf_slaves timeout, then the test fails.

## Verify Output
Lastly, if the dlbf_slave didn’t timeout, then the `test_dlbf` function calls the `dlbf_slave_read_and_verify` function. This function compares the data in the dlbf_slave to the golden output data in the `dlbf_gold0.cpp` file. If there are any mismatches, then the test fails. If all the output data matches to the golden output data, then the `dlbf_test` passes and returns a 1'b to the main function. 

# Test ULBF
The main function then calls the `test_ulbf` function. It starts the ULBF kernels and verifies the output of the AI Engine using the same execution flow as the `test_dlbf` function. 

# References 
UG643 
AI Engine Documentation: Building a Bare-Metal System
