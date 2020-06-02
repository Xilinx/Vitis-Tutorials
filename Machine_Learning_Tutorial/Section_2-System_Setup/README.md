# Vitis and Vitis AI Environment Setup

## Vitis Setup

<img src = "./images/xilinx-vitis.png" align = "right" >

The Vitis unified software platform enables the development of embedded software and accelerated applications on heterogeneous Xilinx platforms including FPGAs, SoCs, and Versal ACAPs. It provides a unified programming model for accelerating Edge, Cloud, and Hybrid computing applications. Leverage integration with high-level frameworks, develop in C, C++, or Python using accelerated libraries or use RTL-based accelerators & low-level runtime APIs for more fine-grained control over implementation — Choose the level of abstraction you need.

Please visit <https://www.xilinx.com/products/design-tools/vitis/vitis-platform.html> for more information about Vitis.

To download and install latest Vitis Core Development Kit, please visit <https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vitis.html>.

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
