# Install and Run SDAccel on Your Own Machine

The SDAccel™ flow for AWS F1 supports the following development models:
- Cloud-based development on AWS EC2 cloud instances
- On-premise development on your own local workstations

In both cases, the final binaries are deployed on an AWS F1 instance.

This guide provides step-by-step instructions for getting started with the on-premise flow and covers the following:
1. Installing and licensing SDAccel in your own environment
1. Building your application on-premise with SDAccel
1. Uploading and executing your application on F1

## Prerequisites
Before going through the steps described in this document, you should complete the tutorial on how to [Create, Configure, and Test an AWS F1 instance](STEP1.md). It is important that you are familiar with the cloud-based development environment before setting up SDAccel in your own environment.

## Requirements
The supported operating systems for SDaccel on-premise development are:
  - Red Hat Enterprise Workstation/Server 7.3-7.4 (64-bit)
  - CentOS 7.2
  - CentOS 7.3-7.4 (64-bit)
  - Ubuntu Linux 16.04.3 LTS (64-bit)
    - Linux kernel 4.4.0 is supported
    - Ubuntu LTS enablement (also called HWE or Hardware Enablement) is _not_ supported

# 1. Installing and licensing SDAccel in your own environment

## Downloading the SDAccel Development Environment
In order to develop any SDAccel application on-premise, you must install the same version of SDAccel as deployed on AWS F1.
The SDAccel installer can be found here:

* Xilinx Vivado v2018.2 or v2018.2.op (64-bit)
* License: EF-VIVADO-SDX-VU9P-OP
* SW Build 2258646 on Thu Jun 14 20:02:38 MDT 2018
* IP Build 2256618 on Thu Jun 14 22:10:49 MDT 2018
* URL: [https://www.xilinx.com/member/forms/download/xef.html?filename=Xilinx_SDx_op_Lin_2018.2_0614_1954_Lin64.bin&akdm=0](https://www.xilinx.com/member/forms/download/xef.html?filename=Xilinx_SDx_op_Lin_2018.2_0614_1954_Lin64.bin&akdm=0)
* MD5 SUM Value: 6b6939e70d4fa90677d2c54a37ec25c7

## Requesting a License

If you are a new user, you will also need to obtain an on-premise license of Vivado®. You can request both _node-locked_ and _floating licenses_ [here](https://www.xilinx.com/products/design-tools/acceleration-zone/ef-vivado-sdx-vu9p-op-fl-nl.html) (links are on the right side of the page).

## Installing SDAccel

* To install the tool, refer to the instructions in the _SDAccel Environment Release Notes, Installation, and Licensing Guide_ [(UG1238)](https://www.xilinx.com/support/documentation/sw_manuals/xilinx2018_3/ug1238-sdx-rnil.pdf).

## Cloning the AWS-FPGA Git Repository

The AWS GitHub repository contains all the necessary platform definition files and setup scripts to run SDAccel and build a design for F1 instances. It also contains numerous examples that will help you learn more about SDAccel.  

Execute the following commands on your local machine to clone the GitHub repository and configure the SDAccel environment:
```bash
cd $HOME
git clone https://github.com/aws/aws-fpga.git
cd aws-fpga                                      
source sdaccel_setup.sh
```

**IMPORTANT**: Sourcing sdaccel_setup.sh may show some errors as it also tries to install runtime drivers which requires sudo access. These errors are non-intrusive, and they can be ignored.

# 2. Building your design on-premise with SDAccel

These steps will show you how to:
- Confirm that you can run SDAccel on your local machine.
- Generate binaries which you can then deploy on the F1 instance.

When using GitHub examples, you can execute same sets of commands that you have used on an AWS EC2 instance.

## Starting the GUI
TO invoke the SDAccel GUI, enter the following command:
```bash
sdx
```
After you confirm that the GUI has opened successfully, close the GUI.

## Running SW Emulation
Execute the following commands to run the SW Emulation step for the SDAccel `helloworld` example:

```bash
cd $HOME/aws-fpga/SDAccel/examples/xilinx_2018.2/getting_started/host/helloworld_c/
make clean
make check TARGETS=sw_emu DEVICES=$AWS_PLATFORM all
```

## Running HW Emulation

Execute the following commands to run the HW Emulation step for the SDAccel `helloworld` example:

```bash
cd $HOME/aws-fpga/SDAccel/examples/xilinx_2018.2/getting_started/host/helloworld_c/
make clean
make check TARGETS=hw_emu DEVICES=$AWS_PLATFORM all
```

## Building for F1 Deployment

* Execute the following commands to build the FPGA binary for the SDAccel `helloworld` example:

```bash
cd $HOME/aws-fpga/SDAccel/examples/xilinx_2018.2/getting_started/host/helloworld_c/
make clean
make TARGETS=hw DEVICES=$AWS_PLATFORM all
```

The build process will generate the host and FPGA binaries.  
1. Host binary: `./helloworld`  
2. FPGA binary: `./xclbin/vector_addition.hw.xilinx_aws-vu9p-f1_4ddr-xpr-2pr_4_0.xclbin`

Create an Amazon FPGA Image (AFI) from the `.xclbin` file with the `create_sdaccel_afi.sh` script by using either of the following methods:
* Create an image locally, after installing the AWS CLI on your machine, or by
* Upload the `.xclbin` to an AWS instance, and run the `create_sdaccel_afi.sh script` there.

In this tutorial, you will upload everything to an AWS EC2 instance.


# 3. Uploading and Executing on F1

This section covers the following steps:
 - Upload your FPGA binary (built on-premise) to the AWS cloud
 - Create an AFI from the `.xclbin` file
 - Compile the host program on the F1 instance
 - Execute your accelerated application on the F1 instance

## Uploading your FPGA binary and host program to AWS

1. Create a tarball with the necessary files:
```bash
tar cvfz helloworld.tgz Makefile src xclbin/vector_addition.hw.xilinx_aws-vu9p-f1_4ddr-xpr-2pr_4_0.xclbin
```
2. Start an AWS F1 instance
3. Upload the tarball to your AWS F1 instance
```bash
scp -i ~/<AWS key pairs>.pem <xclbin file> centos@<public IP address of EC2 instance>:/home/centos/.
```
>**NOTE**: Alternatively, you can transfer files using an AWS S3 bucket.

## Creating the Amazon FPGA Image

1. Ssh to your AWS F1 instance.
```bash
ssh -i <AWS key pairs.pem> -ssh centos@<public IP address of EC2 instance> 22
```
2. Extract your tarball.
```bash
mkdir helloworld
cd helloworld
tar xvfz ../helloworld.tgz
```    
3. Setup the SDAccel environment.
```bash
cd $AWS_FPGA_REPO_DIR                                         
source sdaccel_setup.sh
```
4. Create the AWS FPGA binary and AFI from the `.xclbin` file.
```bash
cd $HOME/helloworld/xclbin
$SDACCEL_DIR/tools/create_sdaccel_afi.sh \
    -xclbin=<xclbin file name>.xclbin \
    -s3_bucket=<bucket-name> \
    -s3_dcp_key=<dcp-folder-name> \
    -s3_logs_key=<logs-folder-name>
```

## Compiling the host application
Use the Makefile to compile the host application.
```bash
cd $HOME/helloworld
make exe
```
This will create the `helloworld` program.

## Executing the acceleration application on F1

* After the AFI status changes to **available**, you are ready to execute on F1:
```bash
sudo sh
source /opt/xilinx/xrt/setup.sh   
./helloworld
```

This module explained how you can develop your FPGA binary on-premise, create an AFI, and execute your accelerated application on an AWS F1 instance.

<hr/>
<p align="center"><b>
<a href="README.md">Return to the 5-steps getting started guide</a>
</b></p>
<br>
<hr/>
<p align="center"><sup>Copyright&copy; 2018 Xilinx</sup></p>
