<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Application Acceleration Development Flow Tutorials</h1>
   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# Bottom-up RTL Kernel Flow with Vitis for Acceleration

***Version: Vitis 2022.1***

RTL design is a traditional and important hardware accelerator development methodology for FPGA. RTL modules provide excellent flexibility and efficiency, while the design process is a timing consuming and error-prone process. The Xilinx&reg; Vitis&trade; unified software platform provides a mature and proven RTL kernel design methodology. With Vitis and the included Vivado&reg; Design Suite, you can focus on your core accelerating module, instead of spending a lot of time on integration, host-FPGA communication, DMA, and other supporting tasks.

## About This Tutorial
 
This tutorial introduces a bottom-up Vitis-based RTL kernel construct and wrap-up process, as well as the host-kernel interaction with [Xilinx Runtime library (XRT)](https://xilinx.github.io/XRT/). All the steps in this tutorial use the command-line interface, except those needed to view waveform or system diagram.

This tutorial uses an example design to illustrate the relevant concept and steps. The following image shows the block diagram of this design. It is a simple AES accelerator design and comprises two kernels, ```krnl_aes``` and ```krnl_cbc```. The ```krnl_aes``` kernel is the core AES computation core with AXI streams slave and master ports. The ```krnl_cbc``` kernel handles the host-kernel data exchange, and the implementation of AES-ECB and AES-CBD modes along with the krnl_aes module.

<div align="center">
<img src="./doc/images/block_diagram-top.svg" alt="top block diagram" >
</div>

Alveo&trade; Data Center accelerator cards are the target platform for this tutorial, which describes the following tasks:

* Packing RTL design to Vitis-compliant RTL kernel with command line interface
* Using MMCM/PLL in the RTL kernel
* Using Vivado XSIM to simulate the design
* Using Xilinx AXI VIP to verify the RTL design with AXI interface
* Using three RTL kernel control modes with XRT: ```ap_ctrl_none```, ```ap_ctrl_hs``` and ```ap_ctrl_chain```.
* Host programming for the RTL kernel with XRT Native API
* Using Vitis hardware emulation to get an insight look of the RTL kernel operation

**Note**: This is not a detailed tools usage guideline, so only basic ideas and steps are shown. You will need to refer to other documents for details.

## Device and Software Requirements

The designs have been verified with the following software/hardware environment and tool chain versions:
* Operating Systems:
  * Redhat/CentOS 7 (See [Additional Requiements for RedHat/CentOS](#additional-requirements-for-redhatcentos))
  * Ubuntu 18.04/20.04 (See [Additional Requiements for Ubuntu](#additional-requirements-for-ubuntu))
  * Perl package installed for Verilog simulation (**required**)
  * OpenSSL library installed for hardware output validate (**required**)
  * GCC 7
* Vitis: 2022.1
* XRT: 2.13.466
* Hardware and Platform for your Alveo card (you need both the deployment and development platforms):
  * Alveo U200: xilinx_u200_gen3x16_xdma_2_202110_1
  * Alveo U250: xilinx_u250_gen3x16_xdma_4_1_202210_1
  * Alveo U50: xilinx_u50_gen3x16_xdma_5_202210_1
  * Alveo U55C: xilinx_u55c_gen3x16_xdma_3_202210_1

### Additional Requirements for RedHat/CentOS
If you are using RedHat/CentOS 7, the default installed GCC version is 4.x.x. You must use the  following command to install and switch to GCC 7 before compiling the host program.

```shell
sudo yum install centos-release-scl
sudo yum install devtoolset-7-gcc-c++
scl enable devtoolset-7 bash
```

To install OpenSSL library for CentOS/Redhat, use:

```shell
sudo yum install openssl-devel
```

### Additional Requirements for Ubuntu
To install OpenSSL library for Ubuntu, use:

``` shell
sudo apt install libssl-dev
```

## Tutorial Contents

This tutorial is separated into three parts, and are intended to be used in order.

1. [AES Introduction and Core AES Computing RTL Module](./doc/aes.md)

  This part gives a brief introduction to AES algorithm and the core RTL module for AES acceleration. The RTL AES module uses native Verilog interface. A simple Verilog testbench is provided to verify the function of the RTL module.

2. [Creating and Packing Vitis Compatible RTL Kernel krnl_aes](./doc/krnl_aes.md)

  This part packs the RTL module described in the first part into a Vitis kernel called ```krnl_aes```, which can be integrated into Vitis. The ```krnl_aes``` kernel has four internal AES pipelines and includes AXI control slave and AXI stream master/slave ports.

  The ```krn_aes``` kernel has a mix control model of ```ap_ctrl_none``` and ```ap_ctrl_hs```.
  An MMCM IP is used in this kernel to generate required core clock, while the AXI interface uses the standard platform clock. A simple Verilog testbench with Xilinx AXI VIPs are provided to verify the function of the kernel.

  In this part, we also integrate the ```krnl_aes``` kernel with a few HLS kernels to generate a usable XCLBIN file. A host test program with XRT native API is used to test the function of the ```krnl_aes``` kernel.

3. [Creating and Packing Vitis Compatible RTL Kernel krnl_cbc](./doc/krnl_cbc.md)

  This part introduces a new RTL kernel called ```krnl_cbc```, which has AXI master interfaces as well as AXI stream interfaces. This kernel will be connected with the ```krnl_aes``` kernel to implement AES-ECB and AES-CBC mode encryption/decryption functions.

  The ```krnl_cbc``` kernel supports ```ap_ctrl_chain``` control models. You can study and compare the behavior and performance differences between ```ap_ctrl_chain``` and ```ap_ctrl_hs``` model usint the provided AXI VIP based testbench, hardware emulation, and host program.

The directory structure and brief explanations of the design repository are as follows.

```
├── aes                                         # Directory for first part: aes
│   ├── rtl/                                    #   Verilog sources
│   ├── tbench/                                 #   Verilog/SystemVerilog testbench files
│   ├── filelist_aes.f                          #   file list file for xsim simulation
│   ├── Makefile                                #   Makefile for all operation
│   ├── runsim_aes_xsim.sh                      #   shell script to run the simulation with xsim
│   └── xsim.tcl                                #   xsim tcl script for simulation
│
├── common                                      # common resources
│   └── plain_gen.pl                                # Perl script to generate input data for RTL simulation
│
├── doc                                         # documents
│
├── krnl_aes                                    # Directory for second part: krnl_aes
│   ├── hls                                     #   HLS sources
│   ├── host/                                   #   Host test program    
│   ├── rtl/                                    #   Verilog sources
│   ├── tbench/                                 #   Verilog/SystemVerilog testbench files
│   ├── filelist_krnl_cbc.f                     #   xsim simulation command file
│   ├── gen_ip.tcl                              #   Vivado tcl script to generate required IPs
│   ├── krnl_aes_test.cfg                       #   Vitis v++ link config file
│   ├── krnl_aes.xdc                            #   constraint for kernel krnl_aes
│   ├── Makefile                                #   Makefile for all operation
│   ├── pack_kernel.tcl                         #   Vivado tcl script to pack the kernel
│   ├── runsim_krnl_aes_xsim.sh                 #   shell script to run the simulation with xsim   
│   ├── setup_emu.sh                            #   shell script to enable/disable hw_emu emulation mode
│   └── xrt.ini                                 #   XRT config file for emulation/profiling
│
├── krnl_cbc                                    # Directory for third pard: krnl_cbc
│   ├── host/                                   #   Host test program  
│   ├── rtl/                                    #   Verilog sources
│   ├── tbench/                                 #   Verilog/SystemVerilog testbench files
│   ├── filelist_krnl_cbc.f                     #   xsim simulation command file
│   ├── gen_ip.tcl                              #   Vivado tcl script to generate required IPs
│   ├── krnl_cbc_test.cfg                       #   Vitis v++ link config file
│   ├── Makefile                                #   Makefile for all operation
│   ├── pack_kernel.tcl                         #   Vivado tcl script to pack the kernel
│   ├── runsim_krnl_cbc_xsim.sh                 #   shell script to run the simulation with xsim   
│   ├── setup_emu.sh                            #   shell script to enable/disable hw_emu emulation mode
│   ├── xrt.ini                                 #   XRT config file for emulation/profiling
│   └── xsim.tcl                                #   xsim tcl script to set hw_emu simulation options
│
└── README.md
```

## Revision History

<details>
  <summary> 2022.1 </summary>

 - Update Vitis target platform support
 - remove Vivado clock routing XDC

</details>

<details>
  <summary>2020.2</summary>

  - Initial release

 </details>

 <p align="center"><sup>Copyright&copy; 2022 Xilinx</sup></p>
