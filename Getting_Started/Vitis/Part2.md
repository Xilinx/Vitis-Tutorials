<table class="sphinxhide" width="100%">
 <tr>
   <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Application Acceleration Development Flow Tutorials</h1>
   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# Vitis Getting Started Tutorial

## Part 2: Installation and Configuration

To develop and deploy applications with Vitis, you need to install the Vitis unified software environment, the Xilinx Runtime library (XRT), and the platform files specific to the acceleration card used in your project.

### Step 1: Review the Installation Requirements

Prior to starting the installation process, make sure to check the [Vitis installation requirements](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Installation-Requirements).

>**NOTE:** Windows OS support is limited to the Vitis embedded software development flow. The Vitis application acceleration flow is only supported on Linux.

### Step 2:  Download Vitis

Download the latest version of the Vitis from the [Adaptive Computing Support download website](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vitis.html). Make sure to download the Linux Self-Extracting web installer or the Single-File installer.

### Step 3: Install Downloaded Packages

After downloading the Vitis, XRT and platform packages, install them in the following order and according to the provided instructions:

1. [Install the Vitis Software Platform](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Installing-the-Vitis-Software-Platform)

2. [Install the Xilinx Runtime and Platforms](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Installing-Xilinx-Runtime-and-Platforms)

>**NOTE:** Installing XRT is not required when targeting Arm®-based embedded platforms. The Vitis compiler has its own copy of xclbinutil for hardware generation; and for software compilation, you can use the XRT from the sysroot on the Embedded Processor platform.

### Step 4: Download and Install the Required Platform files

#### Step 4A: Installing Data Center Platform Files

AMD provides base platforms for the an AMD Alveo™ U200, U250, U50 and U280 data-center acceleration cards. Before installing a platform, you need to download the following packages:

1. Xilinx Runtime (XRT)

2. Deployment Target Platform

3. Development Target Platform

The packages required for each platform can be found on the corresponding download pages:

* [U200](https://www.xilinx.com/products/boards-and-kits/alveo/u200.html#gettingStarted)

* [U250](https://www.xilinx.com/products/boards-and-kits/alveo/u250.html#gettingStarted)

* [U50](https://www.xilinx.com/products/boards-and-kits/alveo/u50.html#gettingStarted)

* [U280](https://www.xilinx.com/products/boards-and-kits/alveo/u280.html#gettingStarted)

#### Step 4B: Installing Embedded Processor Platform Files

AMD provides base platforms for the ZCU102 and ZCU104 cards. Before installing a platform, you need to download the following packages:

1. Base platform
2. Common image

The packages required for each platform can be found on the corresponding download pages:

* [ZCU102](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html)

* [ZCU104](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html)

### Step 5: Set Up the Environment to Run Vitis

To configure the environment to run Vitis, run the following script, which sets up the environment to run in a specific command shell.

```bash
source <Vitis_install_path>/Vitis/2022.2/settings64.sh
```

>**NOTE:** `.csh` scripts are also provided but this tutorial assumes a bash shell is used.

To configure XRT for Alveo Data Center accelerator cards, run the following script. This is not required for Embedded platforms.

```bash
source /opt/xilinx/xrt/setup.sh
```

To specify the location of any Data Center or Embedded platforms you have installed, set the following environment variable:

```bash
export PLATFORM_REPO_PATHS=<path to platforms>
```

>**NOTE:** On some Ubuntu distributions, you must also export the `LIBRARY_PATH` to properly set up Vitis.

```bash
export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu
```

For more information, see [Xilinx AR 73698](https://www.xilinx.com/support/answers/73698.html).

## Next Step

  **Click here to [Review the Host and Kernel Code](./Part3.md)**

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
