# Section 2: System Setup

In this tutorial, we'll use Vitis 2020.1 and Vitis-AI v1.2 for demonstration. The tutorial will be updated when Vitis tools have updates. Vitis and Vitis-AI has a wide range of OS support list, from RHEL/CentOS 7.4-7.8 to Ubuntu 16.04/18.04. This tutorial is tested in Ubuntu 18.04.1 and CentOS 7.6.

| Type                    | Version                       |
| ----------------------- | ----------------------------- |
| Vitis Software Platform | 2020.1                        |
| Vitis-AI                | v1.2                          |
| PetaLinux               | 2020.1                        |
| Host OS                 | Ubuntu 18.04.1 and CentOS 7.6 |

The host OS can be a natively installed OS, or installed in a virtual machine. It needs to have at least 100GB hard disk to install all the tools, models and datasets. 16GB DDR memory is preferred for development flow.



## Vitis Software Platform Setup

<img src = "./images/xilinx-vitis.png" align = "right" >

### Vitis Core Tools

1. Download  Vitis Software Platform 2020.1 from [Xilinx Download Center](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vitis/2020-1.html).

2. Install Vitis dependent packages

   1. Ubuntu: Install GLX 1.3

      ```bash
      sudo add-apt-repository ppa:xorg-edgers/ppa
      sudo apt-get update
      sudo apt-get install libgl1-mesa-glx
      sudo apt-get install libgl1-mesa-dri
      sudo apt-get install libgl1-mesa-dev
      sudo add-apt-repository --remove ppa:xorg-edgers/ppa
      ```

   2. CentOS: Install EPEL and kernel headers

      ```bash
      # Install EPEL
      sudo yum install epel-release
      ```



3. Install Vitis Software Platform ([Full User Guided](https://www.xilinx.com/html_docs/xilinx2020_1/vitis_doc/juk1557377661419.html))

   1. Ensure "Devices > Install devices for Alveo and Xilinx Edge acceleration platforms" is selected during installation.



### PetaLinux

Note: PetaLinux is only needed for Platform Development for Customization Flow.

1. Download PetaLinux from [Xilinx Download Center](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-design-tools/2020-1.html)

2. Install PetaLinux dependencies (Full list: [UG1144](https://www.xilinx.com/support/documentation/sw_manuals/xilinx2020_1/ug1144-petalinux-tools-reference-guide.pdf) Table 2)

   1. Ubuntu

      ```
      sudo apt install iproute gcc g++ net-tools libncurses5-dev zlib1g:i386 libssl-dev flex bison libselinux1 xterm autoconf libtool texinfo zlib1g-dev gcc-multilib build-essential screen pax gawk python
      ```

   2. CentOS

      ```
      sudo yum install iproute gcc gcc-++ net-tools libncurses5-devel zlib-devel openssl-devel flex bison libselinux xterm autoconf libtool texinfo SDL-devel glibc-devel glibc glib2-devel automake screen pax libstdc++ gawk python
      ```



3. Install PetaLinux

   ```
   ./petalinux-v2020.1-final-installer.run
   ```

4. Setup PetaLinux requirements

   1. Ubuntu: change shell from dash to bash

      ```
      sudo dpkg-reconfigure dash
      ```




### Load Vitis, XRT, and PetaLinux environment

   ```bash
   source /tools/Xilinx/Vitis/2020.1/settings64.sh
   source /opt/xilinx/xrt/setup.sh
   source /opt/petalinux-v2020.1/settings.sh
   ```



---

## Vitis AI Setup

<img src = "./images/vitis-ai.png" align = "right" >

The Vitis™ AI development environment is Xilinx’s development platform for AI inference on Xilinx hardware platforms, including both edge devices and Alveo cards. It consists of optimized IP, tools, libraries, models, and example designs. It is designed with high efficiency and ease of use in mind, unleashing the full potential of AI acceleration on Xilinx FPGA and ACAP.

Please visit <https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html> for more information about Vitis AI.

Vitis AI is released as [GitHub repository](https://github.com/Xilinx/Vitis-AI) and Docker.

Follow the instructions below to setup the Vitis AI environment step by step.

### Host Requirement

Please use x86 processor based Ubuntu or CentOS as the host operating system. If you have a nVidia graphics card with CUDA support, it will be very helpful to reduce the running time.

Git package is needed at the host machine. For Ubuntu, use following command to install:
~~~
sudo apt update
sudo apt install git
~~~

For CentOS, use following command to install:
~~~
sudo yum install git
~~~

Docker environment is needed at host side. For docker installation without GPU support, please refer to following link:
* Ubuntu: <https://docs.docker.com/engine/install/ubuntu>
* CentOS: <https://docs.docker.com/engine/install/centos>

For docker installation with nVidia GPU support, please refer to following link:
* Ubuntu and CentOS: <https://github.com/NVIDIA/nvidia-docker>

### Get Vitis AI Git Repository

Go to your working directory, say ~/workspace, then use following command to get the Vitis AI repository.
~~~
git clone https://github.com/Xilinx/Vitis-AI.git
~~~
After that, you will get the Vitis AI directory *~/workspace/Vitis-AI*.

### Prepare Docker

There are two docker available for user to choose: CPU docker is for users without a nVidia graphics card; GPU docker is for those have one. The CPU docker can be pulled [Docker Hub](https://hub.docker.com/r/xilinx/vitis-ai/tags) or you can build it with the provided Dockerfile. For GPU docker, you have to build it by yourself with the provided Dockerfile.

#### CPU Docker
To get the CPU docker from Docker Hub, use following command:
~~~
docker pull xilinx/vitis-ai:latest
~~~

To build it by yourself, use following comamnd:
~~~
~/workspace/Vitis-AI/docker/docker_build_cpu.sh
~~~

In either way, you could finally get the docker image *vitis-ai:latest* settled on your host.

#### GPU Docker
You have to build the docker using following comamnd:
~~~
~/workspace/Vitis-AI/docker/docker_build_gpu.sh
~~~

After that, you could get the docker image *vitis-ai-gpu:latest* on your host.

<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020-2021 Xilinx</sup></p>
