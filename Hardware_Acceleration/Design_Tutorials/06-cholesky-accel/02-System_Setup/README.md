<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Hardware Acceleration Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Section 2 - System Setup

In order to proceed to the Vitis optimization part of the tutorial, you will need to set up the environment and the tools. Use the following steps to install and set up Vitis and an AMD Alveo™ U50 accelerator card.

## Install the Vitis Software Platform

The [Vitis Unified Software Development Platform](https://www.xilinx.com/products/design-tools/vitis/vitis-platform.html) provides a unified programming model for accelerating Edge, Cloud, and Hybrid computing applications. To develop applications, you will need to download and install the Vitis core development kit from [Xilinx Download Center](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vitis.html); Vitis 2023.1 will be used for this tutorial.

>**NOTE:** The Vitis core development kit does not need to be installed on the same server as the Alveo Data Center accelerator card. It can be installed on a system without any cards installed if you only need for development flow, but XRT and target development platform for Alveo card must be installed to work with Vitis.

## Install the Alveo U50 Accelerator Card

For details of Alveo U50 accelerator card you are about to use for this tutorial, visit the [Alveo U50 Product Page](https://www.xilinx.com/products/boards-and-kits/alveo/u50.html). It provides links to all the resources necessary to become familiar with the Alveo U50 Data Center accelerator card along with installation packages and instructions on both RedHat®/CentOS and Ubuntu operating systems under **Getting Started** tab.

To install Alveo U50 accelerator card on your server, follow the instructions in the *Alveo U50 Data Center Accelerator Card Installation Guide* ([UG1370](https://www.xilinx.com/cgi-bin/docs/bkdoc?k=accelerator-cards;v=latest;d=ug1370-u50-installation.pdf)). It provides the latest hardware and driver software installation procedures including card bring-up and validation, also the steps to deploy hardware accelerated applications using the Alveo U50 card.

## Set Up the Environment to Run Vitis

To configure the environment to run the Vitis software platform, run the following scripts, which set up the environment to run in a specific command shell.
~~~
#setup XILINX_VITIS and XILINX_VIVADO variables
source <Vitis_install_path>/Vitis/2023.1/settings64.sh
#setup XILINX_XRT
source /opt/xilinx/xrt/setup.sh
~~~

To specify the location of the target development platform for an Alveo U50 accelerator card, set the following environment variable:

~~~
export PLATFORM_REPO_PATHS=<path to platforms>
~~~

## Validate the Alveo U50 Accelerator Card

With the Alveo U50 accelerator card installed in a server, you can deploy applications to be run on the host server and accelerator card. Follow the instructions in Chaptor 5 -  Card Bring-Up and Validation of the *Alveo U50 Data Center Accelerator Card Installation Guide* ([UG1370](https://www.xilinx.com/cgi-bin/docs/bkdoc?k=accelerator-cards;v=latest;d=ug1370-u50-installation.pdf)) to validate your installed Alveo U50 card.

The `xbutil validate` command validates the correct installation by performing a various set of tests on the given Alveo card, and you need to set up the environment for XRT to perform the validation.

~~~
#setup XILINX_XRT
source /opt/xilinx/xrt/setup.sh
#validate Alveo card
xbutil validate
~~~

If your Alveo U50 has been correctly installed, you will see a high-level summary of the tests performed similar to the following output after performing `xbutil validate`.

~~~
INFO: Found 1 cards
INFO: Validating card[0]: xilinx_u50_gen3x16_xdma_5_202210_1
INFO: == Starting AUX power connector check:
AUX power connector not available. Skipping validation
INFO: == AUX power connector check SKIPPED
INFO: == Starting PCIE link check:
INFO: == PCIE link check PASSED
INFO: == Starting SC firmware version check:
INFO: == SC firmware version check PASSED
INFO: == Starting verify kernel test:
INFO: == verify kernel test PASSED
INFO: == Starting DMA test:
Host -> PCIe -> FPGA write bandwidth = 11933.1 MB/s
Host <- PCIe <- FPGA read bandwidth = 11966.5 MB/s
INFO: == DMA test PASSED
INFO: == Starting device memory bandwidth test:
...........
Maximum throughput: 52428 MB/s
INFO: == device memory bandwidth test PASSED
INFO: == Starting PCIE peer-to-peer test:
P2P BAR is not enabled. Skipping validation
INFO: == PCIE peer-to-peer test SKIPPED
INFO: == Starting memory-to-memory DMA test:
M2M is not available. Skipping validation
INFO: == memory-to-memory DMA test SKIPPED
INFO: Card[0] validated successfully.
INFO: All cards validated successfully.
~~~

Proceed to [**Section 3**](../03-Algorithm_Acceleration/README.md).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
