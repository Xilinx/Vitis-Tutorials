<table>
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Versal™ AI Engine</h1>
   </td>
 </tr>
 <tr>
 <td align="center"><h1>Beamforming Tutorial</h1>
 </td>
 </tr>
</table>

# Introduction
The Xilinx Versal ACAP is a fully software-programmable, heterogeneous compute platform that combines the processing system (PS) (Scalar Engines that include the ARM processors), Programmable Logic (PL) (Adaptable Engines that include the programmable logic), and AI Engines which belong in the Intelligent Engine category.
This tutorial demonstrates creating a beamforming system running on the AI Engine, PL, and PS, and validating the design running on this heterogeneous domain. 

The tutorial has been modularized and takes you through the steps of creating a custom embedded platform, a baremetal host application, a custom Petalinux-based Linux host application, and hardware emulation & hardware build flows in the context of a complete Versal ACAP system integration. Each module uses a Makefile to build the aspect of the design. 

## Tutorial Overview
This Beamforming tutorial is a system level design that utilizes the AI Engine, PL, and PS resources. This design showcases the following features: 
* A large utilization of PL and AI Engine resources which require advanced Timing Closure techniques 
* Custom platform creation
* An AI Engine graph that implements matrix multiplication functions for uplink and downlink beamforming
* RTL Kernels that interface with the AI Engine and operate at 400MHz 
* A scalable architecture that only needs a small number of kernels to be develop and can be copied multiple times to extend compute power
* Baremetal and Petalinux PS Host application development process
* Timing closure method for a high utilization design 
* Runs through the hardware emulation and running on the VCK190 board flows
* A hierarchical makefile structure to highlight dependencies between build steps and showcase a way multiple developers can work on the same repository at the same time (AI Engine developers, RTL designers, and Software Developers) 

### Assumptions
To fully grasp the design we assume users have the following

* Ability to read TCL scripts
* Ability to read C++ based source code to understand the AI Engine kernels, and host application source code (Baremetal and Linux) 
* Ability to read Verilog RTL to understand the Vivado projects created for the RTL PL kernels
* Have a base bootable design (i.e. you have brought up your board, and have a working hardware and board through a simple Vivado design)  

# Before You Begin
Note: This tutorial targets the VCK190 ES board (see https://www.xilinx.com/products/boards-and-kits/vck190.html). This board is currently available via early access. If you have already purchased this board, download the necessary files from the lounge and ensure you have the correct licenses installed. If you do not have a board and ES license please contact your Xilinx sales contact.

## *Documentation*: Explore AI Engine Architecture

* [AM009 AI Engine Architecture Manual](https://www.xilinx.com/support/documentation/architecture-manuals/am009-versal-ai-engine.pdf)

* [Versal ACAP AI Engines for Dummies](https://forums.xilinx.com/t5/Design-and-Debug-Techniques-Blog/Versal-ACAP-AI-Engines-for-Dummies/ba-p/1132493)

## *Tools*: Installing the Tools
Tools Documentation: 

* [AI Engine Tools lounge](https://www.xilinx.com/member/versal_ai_tools_ea.html)

* [AI Engine Documentation](https://www.xilinx.com/html_docs/xilinx2020_2/vitis_doc/yii1603912637443.html)

* Obtain a license to enable Beta Devices in Xilinx tools (to use the VCK190 ES1 platform)

* Obtain licenses for AI Engine tools

* Follow the instructions in [Installing Xilinx Runtime and Platforms](https://www.xilinx.com/html_docs/xilinx2020_2/vitis_doc/acceleration_installation.html#dhg1543555360045__ae364401) (XRT) 

* Follow the instructions to install Petalinux Tools in [UG1144 PetaLinux Tools Documentation](https://www.xilinx.com/search/support-keyword-search.html#q=ug1144) 

To build and run the Beamforming tutorial, you will need the following tools downloaded/installed:

* [Vitis Unified Software Development Platform 2020.1](https://www.xilinx.com/html_docs/xilinx2020_1/vitis_doc/dhg1543555360045.html)
* [The Xilinx RunTime (XRT)](https://www.xilinx.com/html_docs/xilinx2020_1/vitis_doc/pjr1542153622642.html) 
* [Petalinux Tools](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-design-tools.html)

## *Environment*: Setting Up Your Shell Environment
When the elements of the Vitis software platform are installed, update the shell environment script. Set the necessary environment variables to your system specific paths. 

Edit `env_setup_2020.sh` script with your file paths: 

```bash
export XILINX_XRT=/opt/xilinx/xrt

source <XILINX-INSTALL-LOCATION>/Vitis/2020.2/settings64.sh
source $XILINX_XRT/setup.sh
source <path-to-installed-PetaLinux>/settings.sh
```
Then source the environment script: 
```bash
source env_setup_2020.sh
```  

## *Validation*: Confirming Tool Installation
Make sure you are using the 2020.2 version of Xilinx tools. 
```bash
which vitis
which aiecompiler
```

## *Other Tutorials*: Learn Basic V++ and AI Engine Concepts
For novice users, please review the following tutorials to understand the basic V++ concepts and building simple AI Engine designs: 

* XD002: AI Engine Versal Integration and Cosim Tutorial (TBD ADD LINK)

* XD004: V++ Intermediate Flow (TBD ADD LINK)

# Module Headings

## Module 01 - Custom Platform 
* Show users when they need to create a custom platform (instead of using the base platform)
* How users how to create a custom platform (using beamforming platform as an example)

[Read more ...](Module_01_Custom_Platform)

## Module 02 - AI Engine Design
* teach AI Engine developers to map beamforming functions to AI Engine kernels
* teach AI Engine developers to design AI Engine kernels, design AI Engine graphs with beamforming source code as example.  
* teach AI Engine developers to use the AI Engine compilers (and why unique options are used for this design)
* teach AI Engine developers to use the AI Simulator to test against reference output data 

[Read more...](Module_02_AI_Engine_Design)

## Module 03 - PL Design
* teach RTL designers how to map beamforming functions to PL kernels with a base platform
* teach RTL designers how to design PL kernels with the beamforming source code as an example  
* teach RTL designers how to to use the V++ compiler (and why unique options are used for this design)

[Read more...](Module_03_PL_Design)

## Module 04 - AI Engine and PL Integration
* teach developers how to combine an AI Engine graph (libsdf.a) and \*.xo PL kernels into an XCLBIN

[Read more...](Module_04_AI_Engine_and_PL_Integration)

## Module 05 - Baremetal PS Host Application
* teach software developers how to create baremetal application for beamforming example
* showcase baremetal emulation for PL debug

[Read more...](Module_05_Baremetal_Host_Application)

## Module 06 - System Integration - Baremetal
* teach developers how to package their design using the V++ compiler for a hw or hw_emu
* teach developers how to run hardware emulation 
* teach developers how to run design on hardware vck190 board 

[Read more...](Module_06_System_Integration)

## Module 07 - Petalinux 
* teach developers how to build a Petalinux software platform 

[Read more...](Module_07_Petalinux)

## Module 08 - Linux SW Application
* teach developers how to create a Linux PS Host Application for functional and performance tests

[Read more...](Module_08_Linux_SW_Application)

## Module 09 - System Integration - Linux 
* teach developers how to package their design using the V++ compiler for a hw run with a Linux PS Host Application 
* teach developers how to run design on hardware vck190 board 

[Read more...](Module_09)

## Beamforming Introduction
This tutorial shows efficient implementation of beamforming functionality on AI Engine arrays in the Xilinx Versal AI Core devices. The design methodology is applicable to many use cases needing high-throughput matrix multiplication such as 5G wireless communication. The figure below shows an example illustration on how matrix multiplication is used in the beamforming of an orthogonal frequency division multiplex (OFDM) system with 4 layers and 6 antennas. 

![Beamforming General Image](images/Beamforming_introduction.png)

### Downlink Beamforming 
A single symbol of an OFDM system contains a frequency component and time component allocated to a single user (X<sub>0,0</sub>). Multiple symbols in different layers of an OFDM system (X<sub>0,0</sub>,X<sub>0,1</sub>,X<sub>0,2</sub>,X<sub>0,3</sub>) are multiplied by a specific set of complex weights (H<sub>0,0</sub>,H<sub>1,0</sub>,H<sub>2,0</sub>,H<sub>3,0</sub>) so the data between layers becomes “orthogonal” to each other. This orthogonality allows us to sum the layers together into a single signal (Y<sub>0,0</sub>) which is sent to an antenna. A second antenna signal (X<sub>0,1</sub>) can be created by multiplying another set of weights (H<sub>0,1</sub>,H<sub>1,1</sub>,H<sub>2,1</sub>,H<sub>3,1</sub>) to each layer (X<sub>0,0</sub>,X<sub>0,1</sub>,X<sub>0,2</sub>,X<sub>0,3</sub>). The same is done to create the rest of the antenna signals (Y<sub>0,2</sub>,Y<sub>0,3</sub>,Y<sub>0,4</sub>,and Y<sub>0,5</sub>). 

#### Downlink Beamforming Formulation 
The example and generalized downlink matrix multiplication formulas are given below: 

##### Example Equations for a Single Subcarrier: 
![Downlink Example Formulas](images/downlink_example_formulas.PNG)

##### Generalized Downlink Beamforming Equations: 
![Downlink Generlized Formulas](images/downlink_generalized_formulas.PNG)

### Uplink Beamforming 
At the receiving end, the antenna data (Y<sub>0,0</sub>-Y<sub>0,5</sub>) can be demultiplexed back into their original layers (X<sub>0,0</sub>-Y<sub>0,3</sub>) because of their orthogonal feature.   

##### Generalized Uplink Beamforming Equations:
![Uplink Generalized Formulas](images/uplink_generalized_formulas.PNG)

## System Design Overview
This tutorial showcases a beamforming system with 32 layers and 64 antennas implemented on a xcvc1902 Versal ACAP device in the VCK190 board. The beamforming system consists of downlink subsystem which contains the DL64A32L AI Engine subgraph and the dlbf_data, dlbf_coeff, and dlbf_slave PL RTL kernels. The beamforming system also consists of the uplink subsystem which contains the UL64A32L AI Engine subgraph and the ulbf_data, ulbf_coeff, and ulbf_slave PL RTL kernels. Together, the downlink and uplink subsystems implement the Uplink and Downlink Matrix Multiplication Equations for M=32 layers and N=64 antennas and compute sample data. The results are compared to reference downlink and uplink result data for verification. The entire beamforming system is copied three times to fully utilize the available AI Engine and PL resources. 

### Block Diagram
![Beamforming Block Diagram](images/beamforming_block%20_diagram.png)
