<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ - Section 2 - System Setup</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

In order to proceed to the Vitis optimization part of the tutorial, we'll need to setup the environment and the tools. Please follow the below steps to install and setup Vitis and Alveo U50 accelerator card.

## Install Vitis Software Platform
The [Vitis Unified Software Development Platform](https://www.xilinx.com/products/design-tools/vitis/vitis-platform.html) provides a unified programming model for accelerating Edge, Cloud, and Hybrid computing applications. To develop applications, you will need to download and install the Vitis core development kit from [Xilinx Download Center](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vitis.html), Vitis 2020.1 will be used for this tutorial. 

*Note:* The Vitis core development kit does not need to be installed on the same server as the Alveo Data Center accelerator card. It can be installed on a system without any cards installed if you only need for development flow, but XRT and targat development platform for Alveo card must be installed to work with Vitis. 

### Alternative Solution 
Alternatively, you can also develop and deploy applications on cloud server such as [Nimbix](https://www.nimbix.net/alveo/), it comes with everything you need to develop your hardware acceleration code and deploy applications to be run on the host server and installed accelerator cards, please follow instructions on Nimbix to setup Vitis environment. 

## Install Alveo U50 Accelerator card
For details of Alveo U50 accelerator card we are about to use for this tutorial, visit the [Alveo U50 Product Page](https://www.xilinx.com/products/boards-and-kits/alveo/u50.html). It provides links to all the resources necessary to become familiar with the Alveo U50 Data Center accelerator card along with installation packages and instructions on both RedHat/CentOS and Ubuntu operating systems under **Getting Started** tab.

To install Alveo U50 accelerator card on your server, follow the instructions in the *Alveo U50 Data Center Accelerator Card Installation Guide* ([UG1370](https://www.xilinx.com/cgi-bin/docs/bkdoc?k=accelerator-cards;v=latest;d=ug1370-u50-installation.pdf)). It provides the latest hardware and driver software installation procedures including card bring-up and validation, also the steps to deploy hardware accelerated applications using the Alveo U50 card.

### **Alternative Solution** 
Alternatively, you can also develop and deploy applications on cloud server such as [Nimbix](https://www.nimbix.net/alveo/), it comes with everything you need to develop your hardware acceleration code and deploy applications to be run on the host server and installed accelerator cards, please follow instructions on Nimbix to setup environment for Alveo U50 card. 

## Setup Environment to Run Vitis 
To configure the environment to run the Vitis software platform, run the following scripts, which set up the environment to run in a specific command shell.
~~~
#setup XILINX_VITIS and XILINX_VIVADO variables
source <Vitis_install_path>/Vitis/2020.1/settings64.sh
#setup XILINX_XRT
source /opt/xilinx/xrt/setup.sh
~~~

To specify the location of the target development platform for Alveo U50 accelerator card, set the following environment variable:
~~~
export PLATFORM_REPO_PATHS=<path to platforms>
~~~

## Validate Alveo U50 Accelerator card
With the Alveo U50 accelerator card installed in a server, you can deploy applications to be run on the host server and accelerator card. Follow the instructions in Chaptor 5 -  Card Bring-Up and Validation of the *Alveo U50 Data Center Accelerator Card Installation Guide* ([UG1370](https://www.xilinx.com/cgi-bin/docs/bkdoc?k=accelerator-cards;v=latest;d=ug1370-u50-installation.pdf)) to validate your installed Alveo U50 card.

The **xbutil validate** command validates the correct installation by performing a various set of tests on the giving Alveo card, and you need to setup environment for XRT to perform the validation. 
~~~
#setup XILINX_XRT
source /opt/xilinx/xrt/setup.sh
#validate Alveo card
xbutil validate
~~~
