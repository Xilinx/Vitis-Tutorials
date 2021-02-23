<!--
Copyright 2020 Xilinx Inc.
 
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0
 
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Author: Mark Harvey, Xilinx Inc
-->
<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>Vitis AI Tutorials</h1>
  </td>
 </tr>
 <tr>
 <td align="center"><h1>PyTorch flow for Vitis AI</h1>
 </td>
 </tr>
</table>



### Current Status

+ Tested on ZCU102, Alveo U50
+ Tools used: PyTorch 1.4 & Vitis AI 1.3
+ Dataset: MNIST handwritten digits
+ Network: Custom CNN


## Introduction

This tutorial introduces the user to the Vitis AI TensorFlow design process and will illustrate how to go from a python description of the network model to running a compiled model on a Xilinx evaluation board.

The application code in this example design is written in Python and uses the VART runtime.

We will run the following steps:

+ Training and evaluation of a small custom convolutional neural network using PyTorch 1.4
+ Quantization and evaluation of the floating-point model.
+ Compilation of the quantized model to create the .xmodel files ready for execution on the DPU accelerator IP.
+ Download and run the application on the ZCU102 and Alveo U50 evaluation boards.

This tutorial assumes the user is familiar with Python3, PyTorch and has some knowledge of machine learning principles.

![Flow](files/img/flow.png "Basic flow")



## The MNIST dataset

The MNIST handwritten digits dataset is a publicly available dataset that contains a total of 70k 8bit grayscale images each of which are 28pixels x 28pixels. The complete dataset of 70k images is normally divided into 60k images for training and 10k images for validation. The dataset is considered to be the 'hello world' of machine learning and makes a simple introduction to learn the complete Xilinx Vitis-AI flow. 

![mnist](./files/img/mnist.png "Example MNIST images")


## The Convolutional Neural Network

The convolutional neural network in this design has deliberately been kept as simple as possible and consists of just four layers of 2D convolution interspersed with batch normalization and ReLU activation. The network is described in the common.py python script.

![cnn](./files/img/cnn.png?raw=true "Custom CNN")


## Before You Begin

The host machine has several requirements that need to be met before we begin. You will need:

  + An x86 host machine with a supported OS and either the CPU or GPU versions of the Vitis-AI docker installed - see [System Requirements](https://github.com/Xilinx/Vitis-AI/blob/master/docs/system_requirements.md).

  + The host machine will require Docker to be installed and the Vitis-AI GPU docker image to be built - see [Getting Started](https://github.com/Xilinx/Vitis-AI#getting-started).

  + A GPU card suitable for training is recommended, but the training in this tutorial is quite simple and a CPU can be used.

  + If you plan to use the ZCU102 evaluation board, it should be prepared with the board image as per the [Setup the Target](https://github.com/Xilinx/Vitis-AI/tree/master/setup/mpsoc/VART#step2-setup-the-target) instructions. Hints on how to connect the various cables to the ZCU102 are also available [here](https://www.xilinx.com/html_docs/vitis_ai/1_3/installation.html#yjf1570690235238).

  + For the Alveo U50, follow the [Setup Alveo Accelerator Card with HBM for DPUCAHX8H/L](https://github.com/Xilinx/Vitis-AI/tree/master/setup/alveo/u50_u50lv_u280#setup-alveo-accelerator-card-with-hbm-for-dpucahx8hl) instructions

 
For more details, refer to the latest version of the *Vitis AI User Guide* ([UG1414](https://www.xilinx.com/html_docs/vitis_ai/1_3/zmw1606771874842.html)).


## Setting up the workspace

1. Copy the repository by doing either of the following:

    + Download the repository as a ZIP file to the host machine, and then unzip the archive.
    + From a terminal, use the `git clone` command.

2. Open a linux terminal, `cd` to the repository folder, and then `cd` to the `files` folder.
   
3. Start the Vitis AI GPU docker:

     ```shell
     # navigate to densenet tutorial folder
     cd <path_to_densenet_design>/files

     # to start GPU docker container
     ./docker_run.sh xilinx/vitis-ai-gpu:latest
     ```

  The docker container will start and after accepting the license agreement, you should see something like this in the terminal:

     ```shell
     ==========================================
     
     __      ___ _   _                   _____
     \ \    / (_) | (_)            /\   |_   _|
      \ \  / / _| |_ _ ___ ______ /  \    | |
       \ \/ / | | __| / __|______/ /\ \   | |
        \  /  | | |_| \__ \     / ____ \ _| |_
         \/   |_|\__|_|___/    /_/    \_\_____|
     
     ==========================================

     Docker Image Version:  1.3 
     Build Date: 2020-12-20
     VAI_ROOT: /opt/vitis_ai

     For TensorFlow Workflows do:
          conda activate vitis-ai-tensorflow 
     For Caffe Workflows do:
          conda activate vitis-ai-caffe 
     For Neptune Workflows do:
          conda activate vitis-ai-neptune 
     For PyTorch Workflows do:
          conda activate vitis-ai-pytorch 
     For TensorFlow 2.3 Workflows do:
          conda activate vitis-ai-tensorflow2 
     For Darknet Optimizer Workflows do:
          conda activate vitis-ai-optimizer_darknet 
     For Caffe Optimizer Workflows do:
          conda activate vitis-ai-optimizer_caffe 
     For TensorFlow 1.15 Workflows do:
          conda activate vitis-ai-optimizer_tensorflow 
     For LSTM Workflows do:
          conda activate vitis-ai-lstm 
     Vitis-AI /workspace > 
     ```

>:bulb: *If you get a "Permission Denied" error when starting the docker container, it is almost certainly because the docker_run.sh script is not set to be executable. You can fix this by running the following command:*
>    
>    ```shell
>     chmod +x docker_run.sh
>    ```


Activate the PyTorch python virtual environment with `conda activate vitis-ai-pytorch` and you should see the prompt change to indicate that the environment is active:


```shell
Vitis-AI /workspace > conda activate vitis-ai-pytorch
(vitis-ai-pytorch) Vitis-AI /workspace > 
```

*The remainder of this README describes each single step to implement the tutorial, however a shell script called run_all.sh is provided which will run the complete flow:*

```shell
(vitis-ai-pytorch) Vitis-AI /workspace > source run_all.sh
```

## Step 0 - Training

To run step 0:

```shell
(vitis-ai-pytorch) Vitis-AI /workspace > python -u train.py 2>&1 | tee train.log
```
The `train.py` script will execute the training of the CNN and save the trainined floating-point model as a .pth file called `f_model.pth` into the folder indicated by the `--float_model` command line argument.


The script will first check how many GPUs are available and by default it will select GPU #0 as this is usually the fastest. If you wish to select a different GPU, then modify the following line:

```python
device = torch.device('cuda:0')
```

If no GPU is available, the CPU will be selected as the execution unit.


The complete list of command line arguments of `train.py` are as follows:

|Argument|Default|Description|
|:-------|:-----:|:----------|
|--dset_dir|'dataset'|MNIST dataset will be downloaded to this folder|
|--batchsize|100|Batchsize used in training and validation - adjust for memory capacity of your GPU(s)|
|--epochs|3|Number of training epochs|
|--learnrate|0.001|Initial learning rate for optimizer|
|--float_model|'float_model'|Folder where trained floating-point (fmodel.pth) will be saved|


## Step 1 - Quantization

To run step 1:

```shell
(vitis-ai-pytorch) Vitis-AI /workspace > python -u quantize.py --quant_mode calib 2>&1 | tee quant_calib.log
(vitis-ai-pytorch) Vitis-AI /workspace > python -u quantize.py --quant_mode test 2>&1 | tee quant_test.log
```

The Xilinx DPU family of ML accelerators execute models and networks that have their parameters in integer format so we must convert the trained, floating-point checkpoint into a fixed-point integer checkpoint - this process is known as quantization.

Once quantization is finished, the quantized model can be found in the folder indicated by the `--quant_model` command line argument.


The complete list of command line arguments of `quantize.py` are as follows:

|Argument|Default|Description|
|:-------|:-----:|:----------|
|--dset_dir|'dataset'|MNIST dataset will be downloaded to this folder|
|--float_model|'float_model'|Folder containing trained floating-point model|
|--quant_mode|'calib'|Quantization script mode: 'calib' - quantize, 'test - evaluate quantized model|
|--batchsize|100|Batchsize used in evaluation - adjust for memory capacity of your GPU(s)|
|--quant_model|'quant_model'|Folder where quantized model (CNN_int.xmodel) will be saved|


# Step 2 - Compiling for the target

To run step 2, execute the `compile.sh` shell script with one of the target boards as a command line argument, for example:

```shell
(vitis-ai-pytorch) Vitis-AI /workspace > source compile.sh zcu102
```
The `compile.sh` shell script will compile the quantized model and create an .xmodel file which contains the instructions and data to be executed by the DPU. The script also supports `u50` as a command line argument to target the Alveo U50. The compiled xmodel will be written to the compiled_model folder and named CNN_<board_name>.xmodel.


## Step 3 - Running the application on the target

To prepare the images, xmodel and application code for copying to the selected target, run the following command:

```shell
(vitis-ai-pytorch) Vitis-AI /workspace > python -u target.py --target zcu102
```

The script also supports `u50` as a possible value for the `--target` command line option.


The `target.py` script will do the following:

+ Make a folder named target_<board_name>.
+ Copy the appropriate compiled model to the target_<board_name> folder.
+ Copy the Python application code to the target_<board_name> folder.
+ Convert the MNIST test dataset to PNG image files.
     + the number of images is set by the `--num_images` command line argument which defaults to 10000.


The complete list of command line arguments of `target.py` are as follows:

|Argument|Default|Description|
|:-------|:-----:|:----------|
|--dset_dir|'dataset'|MNIST dataset will be downloaded to this folder|
|--target|'zcu102'|Name of target board - zcu102, u50|
|--num_images|10000|Number of MNIST test samples to convert to PNG images|
|--app_dir|'application'|Folder containing Python application code (app_mt.py)|

### ZCU102

The entire `target_zcu102` folder will be copied to the ZCU102. Copy it to the /home/root folder of the flashed SD card, this can be done in one of several ways:

1. Direct copy to SD Card:

  + If the host machine has an SD card slot, insert the flashed SD card and when it is recognised you will see two volumes, BOOT and ROOTFS. Navigate into the ROOTFS and then into the /home folder.  Make the ./root folder writeable by issuing the command ``sudo chmod -R 777 root`` and then copy the entire `target_zcu102` folder from the host machine into the /home/root folder of the SD card.

  + Unmount both the BOOT and ROOTFS volumes from the host machine and then eject the SD Card from the host machine.

2. With scp command:

  + If the target evaluation board is connected to the same network as the host machine, the `target_zcu102` folder can be copied using scp.

  + The command will be something like ``scp -r ./build/target_zcu102 root@192.168.1.227:~/.``  assuming that the target board IP address is 192.168.1.227 - adjust this as appropriate for your system.

  + If the password is asked for, insert 'root'.


With the `target_zcu102` folder copied to the SD Card and the evaluation board booted, you can issue the command for launching the application - note that this done on the target evaluation board, not the host machine, so it requires a connection to the board such as a serial connection to the UART or an SSH connection via Ethernet.

The application can be started by navigating into the `target_zcu102` folder on the evaluation board and then issuing the command ``python3 app_mt.py -m CNN_zcu102.xmodel``. The application will start and after a few seconds will show the throughput in frames/sec, like this:


```shell
root@xilinx-zcu102-2020_2:~/target_zcu102# python3 app_mt.py -m CNN_zcu102.xmodel 
---------------------------
Command line options:
 --image_dir :  images
 --threads   :  1
 --model     :  CNN_zcu102.xmodel
---------------------------
Pre-processing 10000 images...
Starting 1 threads...
---------------------------
Throughput=3399.69 fps, total frames = 10000, time=2.9414 seconds
Correct:9894, Wrong:106, Accuracy:0.9894
---------------------------
```

The performance can be increased by increasing the number of threads with the `--threads` argument:

```shell
root@xilinx-zcu102-2020_2:~/target_zcu102# python3 app_mt.py -m CNN_zcu102.xmodel --threads 4
---------------------------
Command line options:
 --image_dir :  images
 --threads   :  4
 --model     :  CNN_zcu102.xmodel
---------------------------
Pre-processing 10000 images...
Starting 4 threads...
---------------------------
Throughput=5676.42 fps, total frames = 10000, time=1.7617 seconds
Correct:9894, Wrong:106, Accuracy:0.9894
---------------------------
```

### Alveo U50

Note that these steps need to be run from inside of the Vitis-AI Docker container.

Run the `U50_overlay.sh` script (internet connection required) to download and install the correct overlay. Note that the U50 will need to have been flashed with the correct deployment shell - this should have been done in the [Before You Begin](#before-you-begin) section above. The complete steps to run on the Alveo U50 are as follows:


```shell
Vitis-AI /workspace > source U50_overlay.sh
Vitis-AI /workspace > cd target_u50
Vitis-AI /workspace > /usr/bin/python3 app_mt.py --threads 6
```


```shell
(vitis-ai-pytorch) Vitis-AI /workspace/target_u50 > /usr/bin/python3 app_mt.py --model CNN_u50.xmodel  --threads 6
---------------------------
Command line options:
 --image_dir :  images
 --threads   :  6
 --model     :  CNN_u50.xmodel
---------------------------
Pre-processing 10000 images...
Starting 6 threads...
---------------------------
Throughput=22179.60 fps, total frames = 10000, time=0.4509 seconds
Correct:9894, Wrong:106, Accuracy:0.9894
---------------------------
```



## References

+ [Vitis AI Optimizer User Guide (UG1333)](https://www.xilinx.com/support/documentation/sw_manuals/vitis_ai/1_2/ug1333-ai-optimizer.pdf)

+ [Vitis AI User Guide (UG1414)](https://www.xilinx.com/support/documentation/sw_manuals/vitis_ai/1_2/ug1414-vitis-ai.pdf) 


</hr>
<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020-2021 Xilinx</sup></p>