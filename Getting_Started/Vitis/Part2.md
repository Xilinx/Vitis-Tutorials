<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2021.1 Vitis™ Getting Started Tutorial</h1>
   <a href="https://github.com/Xilinx/Vitis-Tutorials/tree/2020.2">See 2020.2 Tutorials</a>
   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# Vitis Flow 101 – Part 2 : Installation Guide

To develop and deploy applications with Vitis, you need to install the Vitis unified software environment, the Xilinx Runtime library (XRT) and the platform files specific to the acceleration card used in your project.


## Step 1 – Installation Requirements

Prior to starting the installation process, make sure to check the [Vitis installation requirements](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/acceleration_installation.html#igz1531201833632).

*NOTE: Windows OS support is limited to the Vitis embedded software development flow. The Vitis acceleration flow is only supported on Linux.*


## Step 2 – Download Vitis

Download the latest version of the Vitis from the [Xilinx download website](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vitis.html). Make sure to download the Linux Self-Extracting web installer or the Single-File installer.


## Step 3 – Install Downloaded Packages

After downloading the Vitis, XRT and platform packages, install them in the following order and according to the provided instructions:

1. [Install the Vitis Software Platform](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/acceleration_installation.html#dhg1543555360045)

2. [Install the Xilinx Runtime and Platforms](https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/acceleration_installation.html#pjr1542153622642)


## Step 4 – Download and Install Platform-specific files

<details>

<summary><b>For Embedded platforms, click here</b></summary>

Xilinx provides base platforms for the ZCU102 and ZCU104 cards. Before installing a platform, you need to download the following packages:

1. XRT
2. Base platform
3. Common image

The packages required for each platform can be found on the corresponding download pages:

* [ZCU102](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html)

* [ZCU104](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html)

</details>

<details>

<summary><b>For Data-Center platforms, click here</b></summary>

Xilinx provides base platforms for the Alveo U200, U250, U50 and U280 data-center acceleration cards. Before installing a platform, you need to download the following packages:

1. Xilinx Runtime (XRT)

2. Deployment Target Platform

3. Development Target Platform

The packages required for each platform can be found on the corresponding download pages:

* [U200](https://www.xilinx.com/products/boards-and-kits/alveo/u200.html#gettingStarted)

* [U250](https://www.xilinx.com/products/boards-and-kits/alveo/u250.html#gettingStarted)

* [U50](https://www.xilinx.com/products/boards-and-kits/alveo/u50.html#gettingStarted)

* [U280](https://www.xilinx.com/products/boards-and-kits/alveo/u280.html#gettingStarted)

</details>


## Step 5 – Setup the environment to run Vitis

To configure the environment to run Vitis, run the following scripts which set up the environment to run in a specific command shell.

```bash
source <Vitis_install_path>/Vitis/2021.1/settings64.sh
source /opt/xilinx/xrt/setup.sh
```

*NOTE: .csh scripts are also provided but this tutorial assumes a bash shell is used.*

To specify the location of any Data-Center or Embedded platforms you have installed, set the following environment variable:

```bash
export PLATFORM_REPO_PATHS=<path to platforms>
```

*NOTE: On some Ubuntu distributions, you must also export LIBRARY_PATH to properly set up Vitis.*

```bash
export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu
```

For more information see [Xilinx AR 73698](https://www.xilinx.com/support/answers/73698.html).


## Next Step

Congratulations on successfully installing everything you need to use Vitis! You can now move on to [**Part 3**](./Part3.md) and get going with your first example project.



<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
