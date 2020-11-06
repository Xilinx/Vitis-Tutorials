
<div style="page-break-after: always;"></div>
<table style="width:100%">
  <tr>
    <th width="100%" colspan="6"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>Deep Learning with Custom GoogleNet and ResNet in Keras and Xilinx Vitis AI</h1>
</th>
  </tr>
</table>
</div>

### Current status

1. Tested with Vitis AI 1.0 and 1.1

2. Tested in hardware on ZCU102 (both Vitis AI 1.0 and 1.1) and ZCU104 boards (only with 1.0 on the cifar10)


# Introduction

In this Deep Learning (DL) tutorial, you will quantize in fixed point some custom Convolutional Neural Networks (CNNs) and deploy them on the Xilinx&reg; [ZCU102](https://www.xilinx.com/products/boards-and-kits/ek-u1-zcu102-g.html) or [ZCU104](https://www.xilinx.com/products/boards-and-kits/zcu104.html) boards using [Vitis AI](https://developer.xilinx.com/en/get-started/ai.html), which is a set of optimized IP, tools libraries, models and example designs valid for AI inference on both Xilinx edge devices and Alveo cards.

This tutorial includes:
- four custom CNNs, from the simplest ``LeNet`` and ``miniVggNet`` to the intermediate ``miniGoogleNet`` and the more complex ``miniResNet``, as described in the [custom_cnn.py](files/code/custom_cnn.py) file;
- two different datasets, ``Fashion-MNIST`` and ``CIFAR-10``, each one with 10 classes of objects.

Once the selected CNN has been correctly trained in Keras, the [HDF5](https://www.hdfgroup.org/solutions/hdf5/) file of weights is converted into a TF checkpoint and inference graph file, such frozen graph is then quantized by the Vitis AI Quantizer that creates an INT8 ``pb`` file from which the Vitis AI Compiler generates the ``elf`` file of micro instructions for the Deep Processor Unit (DPU) of the Vitis AI platform. The final C++ application is executed at run time on the ZCU102 target board, which is the default one adopted in this tutorial, see the Appendix for the ZCU104. The top-1 accuracy of the predictions computed at run time is measured and compared with the simulation results.

# Pre-requisites

- An Ubuntu 16.04 host PC with Python 3.6  and its package ``python3.6-tk`` installed (this last one installed with ``sudo apt-get install python3.6-tk``);

- [Vitis AI stack release 1.0](https://github.com/Xilinx/Vitis-AI/tree/v1.0) from [www.github.com/Xilinx](https://www.github.com/Xilinx). In particular, refer to the [Vitis AI User Guide UG1414 v1.0](https://www.xilinx.com/support/documentation/sw_manuals/vitis_ai/1_0/ug1414-vitis-ai.pdf) for the installation guidelines and note that you need to download the two containers available from [docker hub](https://hub.docker.com/r/xilinx/vitis-ai/tags):
  - **tools container** with tag ``tools-1.0.0-cpu``, here ``cpu`` means that this environment runs on the host PC only with CPU support (in other words without any GPU need). Note also that UG1414 explains how to build your own container with GPU support;
  - **runtime container** with tag ``runtime-1.0.1-cpu``, note that you need this container only once to prepare SD card content of the target board, then you will compile all the applications directly on the target board itself, which means you do not cross-compile them from the host PC.

  - Vitis AI Evaluation boards that contain a pre-built working design with the [DPU-v2](https://github.com/Xilinx/Vitis-AI/tree/master/DPU-TRD), either:
    - the (default) [ZCU102](https://www.xilinx.com/products/boards-and-kits/ek-u1-zcu102-g.html) board with its [zcu102 image file](https://www.xilinx.com/bin/public/openDownload?filename=xilinx-zcu102-dpu-v2019.2.img.gz), or
    - the (alternative) [ZCU104](https://www.xilinx.com/products/boards-and-kits/zcu104.html) board with its [zcu104 image file](https://www.xilinx.com/bin/public/openDownload?filename=xilinx-zcu104-dpu-v2019.2.img.gz).


- With few changes, explained in the last part of the following section, this tutorial can work also with [Vitis AI stack release 1.1](https://github.com/Xilinx/Vitis-AI).

- Familiarity with Deep Learning principles.


# Before starting

Here are the few things you have to know about Docker in order to run Vitis AI effectively.

Assuming your working directory ``<WRK_DIR>`` is for example ``$HOME/ML/VAI/Vitis-AI`` (or whatever name), to list the currently available docker images you have to run the following command:
```bash
docker images # to list the current docker images available in your host pc
```
you should see something like this:
```text
REPOSITORY                             TAG                 IMAGE ID            CREATED             SIZE
xilinx/vitis-ai                        tools-1.0.0-gpu     034a766d9641        3 hours ago         12GB
xilinx/vitis-ai                        runtime-1.0.0-cpu   af058b4c48b3        6 weeks ago         11.9GB
xilinx/vitis-ai                        tools-1.0.0-cpu     7b7550aac2e6        6 weeks ago         8.35GB
```

To launch the docker container with VAI tools, to be used from CNN training to generation of the ELF file, for CPU (or GPU) from the ``<WRK_DIR>`` folder:
```bash
cd <WRK_DIR> # you are now in Vitis_AI subfolder
./docker_run.sh xilinx/vitis-ai:tools-1.0.0-cpu  # only CPU
#./docker_run.sh xilinx/vitis-ai:tools-1.0.0-gpu # alternatively: only GPU
```

To launch the docker container with VAI runtime, to compile the whole application for the target board, from the ``<WRK_DIR>`` folder:
```bash
./runtime/docker_run.sh xilinx/vitis-ai:runtime-1.0.0-cpu
```

Note that both the two containers map the shared folder ``/workspace`` with the file system of the Host PC from where you launch the above command, which is ``<WRK_DIR>`` in your case. This shared folder enables you to transfer files from the Host PC to the docker container and vice versa.

The docker container do not have any graphic editor, so it is recommended that you work with two terminals and you point to the same folder, in one terminal you use the docker container commands and in the other terminal you open any graphic editor you like.


Note that docker does not have an automatic garbage collection system as of now. You can use this command to do a manual garbage collection:
```
docker rmi -f $(docker images -f "dangling=true" -q)
```



### Install Missing Packages on the Vitis AI Tools Container

This tutorial requires some packages that were not included in the VAI tools container. Here are the commands to include such packages:
```bash
./docker_run.sh xilinx/vitis-ai:tools-1.0.0-cpu # enter into the docker VAI tools image
sudo su # you must be root
conda activate vitis-ai-tensorflow # as root, enter into Vitis AI TF (anaconda-based) virtual environment
conda install matplotlib
conda install pandas
conda install seaborn
conda install pycairo==1.18.2
# you cannot install next packages with conda, so use pip instead
pip install imutils==0.5.1
pip install keras==2.2.4
conda deactivate
exit # to exit from root
conda activate vitis-ai-tensorflow # as normal user, enter into Vitis AI TF (anaconda-based) virtual environment
```
Note that if you exit from the current Docker Vitis AI tools image you will lose all the installed packages, so to save all changes in a new docker image open a new terminal and run the following commands:

```bash
sudo docker ps -l # To get the Docker CONTAINER ID
```
you will see the following text (the container ID might have a different number):
```text
CONTAINER ID        IMAGE                             COMMAND                CREATED      STATUS     NAMES
5310263294ba        xilinx/vitis-ai:tools-1.0.0-gpu   "/etc/login.sh bash"   something    something  something
```
now save the modified docker image:
```bash
sudo docker commit -m"new image: added keras 2.2.4 and pandas, seaborn, matplotlib, imutils" \
       5310263294ba xilinx/vitis-ai:tools-1.0.0-cpu
```

For the sake of clarity, all the Python3.6 packages used in this tutorial are listed in the [vitis_ai_tf_tools_requirements.txt](files/doc/vitis_ai_tf_tools_requirements.txt). Most of them are already pre-installed in the original container and you only need to install the few missing ones listed previously (nevertheless, if you want to re-install all of them, from the python3.6 virtual environment run the command ``pip install -r ./doc/vitis_ai_tf_tools_requirements.txt``).

You can launch this modified tools container by running the following command from the ``<WRK_DIR>`` folder:
```bash
cd <WRK_DIR> # you are now in Vitis_AI subfolder
./docker_run.sh xilinx/vitis-ai:tools-1.0.0-cpu
#./docker_run.sh xilinx/vitis-ai:tools-1.0.0-gpu # just in case you have GPU support
```
then enter into the VAI tensorflow virtual environment:
```bash
conda activate vitis-ai-tensorflow
```


### The Vitis AI Runtime Container

This docker image can support both **Vitis AI Library** (named **AI SDK** prior to Vitis) and **DNNDK** cross compilers. In particular:

1. Both **Vitis AI library** and  **Vitis AI for Zynq/ZU+** use the same cross compiler from Petalinux. In the previous DNNDK releases, there was no cross compilation at all and the compilation was done directly on the target board.

2. **Vitis AI samples** uses new unified APIs for both Zynq/ZU+ and Alveo platforms in this release, while **DNNDK samples** still use legacy DNNDK Level1 APIs for back compatibility with previous DNNDK releases (for example the 3.1).

3. **Vitis AI samples** included in the runtime docker do not belong to **Vitis AI Library** although their names seem related. In fact **Vitis AI Library** is designed to be higher level for quick AI application development, this library also provides APIs and optimized routines for pre- and post-processing.

You can launch the runtime container by running the following command from the ``<WRK_DIR>`` folder:
```bash
cd <WRK_DIR> # you are now in Vitis_AI subfolder
./docker_run.sh xilinx/vitis-ai:runtime-1.0.0-cpu
```

### Prepare the SD Card for the ZCU102 and ZCU104 Target Boards

Chapter 2 of [Vitis AI UG1414 v1.0](https://www.xilinx.com/support/documentation/sw_manuals/vitis_ai/1_0/ug1414-vitis-ai.pdf) contains all the necessary information to setup the target board and to prepare the SD card content. In particular, remember to copy (with ``scp``) to the target board the ``xilinx_vai_board_package`` folder from the runtime container (located in ``/opt/vitis_ai/``) and the ``dnndk_samples_zcu102`` folder (in case of ZCU102) or the ``dnndk_samples_zcu104`` folder (in case of ZCU104) from the ``mpsoc`` folder of Vitis AI.
Once copied, execute the following commands from the target board:
```bash
cd xilinx_vai_board_package
./install.sh
```

At the end of this procedure you should see something as illustrated in the screenshot of Figure 0 (related to ZCU104 board, but it looks the same also for ZCU102):

![figure0](files/doc/images/zcu102_packages.png)

*Figure 0: Screenshot of ZCU104 terminal with folders copied from the runtime container.*

Note also that either [target_zcu102/common](files/target_zcu102/common) or [target_zcu104/common](files/target_zcu104/common) folder of this repository is a copy respectively of either [mpsoc/dnndk_samples_zcu102/common](https://github.com/Xilinx/Vitis-AI/tree/master/mpsoc/dnndk_samples_zcu102/common) or [mpsoc/dnndk_samples_zcu104/common](https://github.com/Xilinx/Vitis-AI/tree/master/mpsoc/dnndk_samples_zcu104/common) folder, just for your comfort.


### Changes for Vitis AI 1.1

There are few differences between Vitis AI 1.0 and 1.1 releases for what concerns MPSoC devices:

1. you have to use the proper Docker Image for Vitis AI 1.1

  ```
  xilinx/vitis-ai-gpu       latest                            61872b030321        2 weeks ago         14.5GB
  ```

  which is different from the images associated with 1.0:

  ```
  xilinx/vitis-ai           tools-1.0.0-cpu                   37ff1cd99ecb        3 months ago        8.59GB
  xilinx/vitis-ai           tools-1.0.0-gpu                   1b45847f369d        3 months ago        12GB
  ```

2. You have to follow the Vitis AI 1.1 instructions for [Setting Up the Evaluation Boards](https://www.xilinx.com/html_docs/vitis_ai/1_1/yjf1570690235238.html).

3. Starting from Vitis AI 1.1 release there is no more Docker Runtime Container, and you can cross compile the ``elf`` files directly from the host PC to the target board. You have to execute all the instructions of [Legacy DNNDK examples](https://www.xilinx.com/html_docs/vitis_ai/1_1/ump1570690283280.html) to setup ``petalinux/2019.2`` and all the DNNDK application files and libraries, so that you can finally run everything on your target board.
In the following of this tutorial it is assumed that ``petalinux`` is installed into ``/opt/petalinux/2019.2`` of your host PC.


Then, the Vitis AI 1.1 flow is basically the same of 1.0, the only difference is that in the three ``run_*.sh`` scripts you have to replace the line

```bash
python /opt/vitis_ai/compiler/vai_c_tensorflow # vitis-ai 1.0 \
```

with the following line

```bash
vai_c_tensorflow # vitis-ai 1.1 \
```

For miniResNet CNN, you have to change one line in the ``*_tf_main.cc`` files of both ``cifar10`` and ``fmnist`` subdirectories in the [target_zcu102](files/target_zcu102) and [target_zcu104](files/target_zcu104) folders, from
```c++
#define CONV_INPUT_NODE "batch_normalization_1_FusedBatchNorm_1_add"   //for Vitis AI == 1.0
```
to
```c++
#define CONV_INPUT_NODE "batch_normalization_1_FusedBatchNormV3_1_add" //for Vitis AI >= 1.1
```

A part for the above changes, all the rest of the flow (that is, the python and shell script files) used in this tutorial with Vitis AI 1.0 works also with  Vitis AI 1.1.


# The Main Flow

The main flow is composed of seven major steps. The first six steps are executed from the tools container on the host PC by launching one of the scripts [run_all.sh](files/run_all.sh). The seventh step can be executed directly on the target board.
Here is an overview of each step:


1. Organize the data into folders, such as ``train`` for training, ``val`` for validation during the training phase, ``test`` for testing during the inference/prediction phase, and ``cal`` for calibration during the quantization phase, for each dataset. See [Organize the Data](#1-organize-the-data) for more information. From the host PC, run the following command:
```
source ./0_generate_images.sh # generate images of both datasets
```
2. Train the CNNs in Keras and generate the HDF5 weights model. See [Train the CNN](#2-train-the-cnn) for more information. From the host PC, run the following  commands:
```
source ./1_fmnist_train.sh  #only for Fashion-MNISTxilinx/vitis-ai-gpu             latest
source ./1_cifar10_train.sh #only for CIFAR-10
```
3. Convert into TF checkpoints and inference graphs. See [Create TF Inference Graphs from Keras Models](#3-create-tf-inference-graphs-from-keras-models) for more information. From the host PC, run the following  commands:
```
source ./2_fmnist_Keras2TF.sh
source ./2_cifar10_Keras2TF.sh
```

4. Freeze the TF graphs to evaluate the CNN prediction accuracy as the reference starting point. See [Freeze the TF Graphs](#4-freeze-the-tf-graphs) for more information. From the host PC, run the following commands:
```
source ./3a_fmnist_freeze.sh
source ./3b_fmnist_evaluate_frozen_graph.sh
source ./3a_cifar10_freeze.sh
source ./3b_cifar10_evaluate_frozen_graph.sh
```

5. Quantize from 32-bit floating point to 8-bit fixed point and evaluate the prediction accuracy of the quantized CNN. See [Quantize the Frozen Graphs](#5-quantize-the-frozen-graphs) for more information. From the host PC, run the following commands:
```
source ./4a_fmnist_quant.sh
source ./4b_fmnist_evaluate_quantized_graph.sh
source ./4a_cifar10_quant.sh
source ./4b_cifar10_evaluate_quantized_graph.sh
```

6. Write the C++ application and then compile the ``elf`` file for the ZCU102 target board. See [Compile the Quantized Models](#6-compile-the-quantized-models) for more information. From the host PC, run the following commands:
```
source ./5_fmnist_vai_compile.sh
source ./5_cifar10_vai_compile.sh
```

7. Execute the ``elf`` file, during run-time, on the ZCU102 target board to measure the effective top-1 accuracy. See [Build and Run on the ZCU102 Target Board](#7-build-and-run-on-the-zcu102-target-board) for more information. From the target board, run the following command:
```
source ./run_on_target.sh
```

>**:pushpin: NOTE** All explanations in the following sections are based only on the Fashion-MNIST dataset; the commands for the CIFAR-10 dataset are very similar: just replace the sub-string "fmnist" with "cifar10".

>**:pushpin: WARNING** Once launched the shell scripts, sometimes you might get the error:
``` $'\r': command not found```. In that case run the following commands from your Ubuntu host PC (out of the Vitis AI docker images):
```bash
sudo apt-get install dos2unix
cd <WRK_DIR>
for file in $(find . -name "*.sh"); do
  dos2unix ${file}
done
```


# 1 Organize the Data

As Deep Learning deals with image data, you have to organize your data in appropriate folders and apply some pre-processing to adapt the images to  the hardware features of the Vitis AI Platform. The script [0_generate_images.sh](files/0_generate_images.sh) creates the sub-folders: ``train``, ``val``, ``test``, and ``cal`` that are located in the ``dataset/fashion-mnist`` and ``dataset/cifar10`` directories and fills them with 50000 images for training, 5000 images for validation, 5000 images for testing (taken from the 10000 images of the original test dataset), and 1000 images for the calibration process (copied from the training images).

All the images are 32x32x3 in dimensions so that they are compatible with the two different datasets.


## 1.1 Fashion MNIST

The [MNIST](http://yann.lecun.com/exdb/mnist/) dataset is considered the ``hello world`` of DL because it is widely used as a first test to check the deployment flow of a vendor of DL solutions. This small dataset takes relatively less time in the training of any CNN. However, due to the poor content of all its images, even the most shallow CNN can easily achieve from 98% to 99% of top-1 accuracy in Image Classification.

To solve this problem, the [Fashion-MNIST](https://github.com/zalandoresearch/fashion-mnist) dataset has been recently created for the paper [Fashion-MNIST: a Novel Image Dataset for Benchmarking Machine Learning Algorithms](arxiv.org/abs/1708.07747). It is identical to the MNIST dataset in terms of training set size, testing set size, number of class labels, and image dimensions, but it is more challenging in terms of achieving high top-1 accuracy values.

Usually, the size of the images is 28x28x1 (gray-level), but in this case they have been converted to 32x32x3 ("false" RGB images) to be compatible with the "true" RGB format of CIFAR-10.

## 1.2 CIFAR-10

The [CIFAR-10](https://www.cs.toronto.edu/~kriz/cifar.html) dataset is composed of 10 classes of objects to be classified. It contains 60000 labeled RGB images that are 32x32 in size and thus, this dataset is more challenging than the MNIST and Fashion-MNIST datasets. The CIFAR-10 dataset was developed for the paper [Learning Multiple Layers of Features from Tiny Images](https://www.cs.toronto.edu/~kriz/learning-features-2009-TR.pdf).

# 2 Train the CNN

Irrespective of the CNN type, the data is processed, using the following Python code, to normalize it from 0 to 1. Such code has to be mirrored in the C++ application that runs in the ARM&reg; CPU of ZCU102 target board.

```Python
# scale data to the range of [0, 1]
x_train = x_train.astype("float32") / cfg.NORM_FACTOR
x_test  = x_test.astype("float32") / cfg.NORM_FACTOR

# normalize
x_train = x_train -0.5
x_train = x_train *2
x_test  = x_test  -0.5
x_test  = x_test  *2
```


## 2.1 LeNet

The model scheme of ```LeNet``` has 6,409,510 parameters as shown in the following figure:

![figure](files/doc/images/bd_LeNet.png)

Once the training is complete, you will get the average top-1 accuracy as reported in the
[logfile_host_run_all.txt](files/rpt/logfile_host_run_all.txt) logfile.

For more details about this custom CNN and its training procedure, read the "Starter Bundle" of the [Deep Learning for Computer Vision with Python](https://www.pyimagesearch.com/deep-learning-computer-vision-python-book/) books by Dr. Adrian Rosebrock.

## 2.2 miniVggNet

`miniVggNet` is a less deep version of the original `VGG16` CNN customized for the smaller Fashion-MNIST dataset instead of the larger [ImageNet-based ILSVRC](https://machinelearningmastery.com/introduction-to-the-imagenet-large-scale-visual-recognition-challenge-ilsvrc/). For more information on this custom CNN and its training procedure, read [Adrian Rosebrock's post](https://www.pyimagesearch.com/2019/02/11/fashion-mnist-with-keras-and-deep-learning/) from the PyImageSearch Keras Tutorials. ``miniVggNet`` is also explained in the "Practitioner Bundle" of the [Deep Learning for CV with Python](https://www.pyimagesearch.com/deep-learning-computer-vision-python-book/) books.

The model scheme of `miniVggNet` has 2,170,986 parameters as shown in the following figure:

![figure](files/doc/images/bd_miniVggNet.png)

Once the training is complete, you will get the average top-1 accuracy as reported in the
[logfile_host_run_all.txt](files/rpt/logfile_host_run_all.txt) logfile and also illustrated by the learning curves:

![figure](files/doc/images/miniVggNet_plot.png)

## 2.3 miniGoogleNet

`miniGoogleNet` is a customization of the original `GoogleNet` CNN. It is suitable for the smaller Fashion-MNIST dataset, instead of the larger ImageNet-based ILSVRC.

For more information on ``miniGoogleNet``, read the "Practitioner Bundle" of the [Deep Learning for CV with Python](https://www.pyimagesearch.com/deep-learning-computer-vision-python-book/) books by Dr. Adrian Rosebrock.

The model scheme of ```miniGoogleNet``` has 1,656,250 parameters, as shown in the following figure:

![figure](files/doc/images/bd_miniGoogleNet.png)

Once the training is complete, you will get the average top-1 accuracy as reported in the
[logfile_host_run_all.txt](files/rpt/logfile_host_run_all.txt) logfile and also illustrated by the learning curves:

![figure](files/doc/images/miniGoogleNet_plot.png)


## 2.4 miniResNet

`miniResNet` is a customization of the original `ResNet-50` CNN. It is suitable for the smaller Fashion-MNIST small dataset, instead of the larger ImageNet-based ILSVRC.

For more information on ``miniResNet``, read the "Practitioner Bundle" of the [Deep Learning for CV with Python](https://www.pyimagesearch.com/deep-learning-computer-vision-python-book/) books.

The model scheme of ```miniResNet``` has  886,102 parameters, as shown in the following figure:

![figure](files/doc/images/bd_miniResNet.png)

Once the training is complete, you will get the average top-1 accuracy as reported
in the [logfile_host_run_all.txt](files/rpt/logfile_host_run_all.txt) logfile
and also reported by the learning curves:

![figure](files/doc/images/miniResNet_plot.png)

# 3 Create TF Inference Graphs from Keras Models

The script [2_fmnist_Keras2TF.sh](files/2_fmnist_Keras2TF.sh) gets the computation graph of the TF backend representing the Keras model which includes the forward pass and training related operations.

The output files of this process, ``infer_graph.pb`` and ``float_model.chkpt.*``, will be stored in the folder [tf_chkpts](files/tf_chkpts/) (actually empty to save disk space). The generated logfile in the [rpt](files/rpt) folder also contains the TF input and output names that will be needed for [Freeze the TF Graphs](#freeze-the-tf-graphs). For example, in the case of ``miniVggNet``, such nodes are named ``conv2d_1_input`` and ``activation_6/Softmax`` respectively, as reported in the [logfile_host_run_all.txt](files/rpt/logfile_host_run_all.txt) file.

# 4 Freeze the TF Graphs

The inference graph created in [Create TF Inference Graphs from Keras Models](#create-tf-inference-graphs-from-keras-models) is first converted to a [GraphDef protocol buffer](https://www.tensorflow.org/guide/extend/model_files), then cleaned so that the subgraphs that are not necessary to compute the requested outputs, such as the training operations, can be removed. This process is called "freezing the graph".

The following commands generate the frozen graph and use it to evaluate the accuracy of the CNN by making predictions on the images in the `test` folder:
```
source ./3a_fmnist_freeze.sh
source ./3b_fmnist_evaluate_frozen_graph.sh
```
It is important to apply the correct ``input node`` and ``output node`` names in all the shell scripts, as shown in the following example with parameters when related to the ``miniVggNet`` case study:
```
--input_node  conv2d_1_input --output_node activation_6/Softmax
```
This information can be captured by the following python code:
```Python
# Check the input and output name
print ("\n TF input node name:")
print(model.inputs)
print ("\n TF output node name:")
print(model.outputs)
```

With the Fashion-MNIST dataset, the frozen graphs evaluation generates top-1 prediction accuracy as reported in the file [logfile_host_run_all.txt](files/rpt/logfile_host_run_all.txt).

# 5 Quantize the Frozen Graphs

The following commands generate the quantized graph and use it to evaluate the accuracy of the CNN by making predictions on the images in the `test` folder:
```
source ./4a_fmnist_quant.sh
source ./4b_fmnist_evaluate_quantized_graph.sh
```

With the Fashion-MNIST dataset, the quantized graphs evaluation generates top-1 prediction accuracy as reported in the file [logfile_host_run_all.txt](files/rpt/logfile_host_run_all.txt).


# 6 Compile the Quantized Models

The following command generates the ``elf`` file for the embedded system composed by the ARM CPU and the DPU accelerator in the ZCU102 board:
```
source ./5_fmnist_vai_compile.sh
```
This file has to be linked with the C++ application directly on the target board OS environment. For example, in case of ``LeNet`` for Fashion-MNIST, the ``elf`` file is named ``dpu_LeNet_0.elf``. A similar nomenclature is applied for the other CNNs.

Note that the Vitis AI Compiler tells you the names of the input and output nodes of the CNN that will be effectively implemented as a kernel in the DPU, therefore whatever layer remains out of such nodes it has to be executed in the ARM CPU as a software kernel.
This can be easily understood looking at the logfile of this step, for example [logfile_host_run_all.txt](files/rpt/logfile_host_run_all.txt) in case of `LeNet` CNN:
```text
Input Node(s)             (H*W*C)
conv2d_2_convolution(0) : 32*32*3

Output Node(s)      (H*W*C)
dense_2_MatMul(0) : 1*1*10
```

# 7 Build and Run on the ZCU102 Target Board

This section reports only the results related to Fashion-MNIST dataset. The results for CIFAR-10 are similar.

## 7.1 The C++ Application

The C++ code for image classification is almost independent of the CNN type, the only differences being the names of the kernel and the input/output nodes.

For example, in the case of ``miniVggNet``, you have the following fragment of C++ code from [top5_tf_main.cc](files/target_zcu102/fmnist/miniVggNet/src/top5_tf_main.cc):

```c++
#define KERNEL_CONV "miniVggNet_0"
#define CONV_INPUT_NODE "conv2d_1_convolution"
#define CONV_OUTPUT_NODE "dense_2_MatMul"
```

whereas for the ``miniGoogleNet``, the C++ code looks like this:

```c++
#define KERNEL_CONV "miniGoogleNet_0"
#define CONV_INPUT_NODE "conv2d_1_convolution"
#define CONV_OUTPUT_NODE "dense_1_MatMul"
```

``LeNet`` and ``miniResNet`` also have their respective codes, which are:

```c++
#define KERNEL_CONV "LeNet_0"
#define CONV_INPUT_NODE "conv2d_2_convolution"
#define CONV_OUTPUT_NODE "dense_2_MatMul"

#define KERNEL_CONV "miniResNet_0"
#define CONV_INPUT_NODE "batch_normalization_1_FusedBatchNorm_1_add"
#define CONV_OUTPUT_NODE "dense_1_MatMul"
```


It is very important that the C++ code for pre-processing the images executes the same operations that you applied in the Python code of the training procedure. This is illustrated in the following C++ code fragments:

```c++
void normalize_image(const Mat& image, int8_t* data, float scale, float* mean)
{
  for(int i = 0; i < 3; ++i) {
    for(int j = 0; j < image.rows; ++j) {
      for(int k = 0; k < image.cols; ++k) { //BGR  
	       data[j*image.rows*3+k*3+i] = (float(image.at<Vec3b>(j,k)[i])/255.0 - 0.5)*2 * scale;
      }
     }
   }
}

inline void set_input_image(DPUTask *task, const string& input_node, const cv::Mat& image, float* mean)
{
  //Mat cropped_img;
  DPUTensor* dpu_in = dpuGetInputTensor(task, input_node.c_str());
  float scale = dpuGetTensorScale(dpu_in);
  int width = dpuGetTensorWidth(dpu_in);
  int height = dpuGetTensorHeight(dpu_in);
  int size = dpuGetTensorSize(dpu_in);
  int8_t* data = dpuGetTensorAddress(dpu_in);
  normalize_image(image, data, scale, mean);
}
```

>**:pushpin: NOTE** The DPU API apply [OpenCV](https://opencv.org/) functions to read an image file (either ``png`` or ``jpg`` or whatever format) therefore the images are seen as BGR and not as native RGB. All the training and inference steps done in this tutorial threats images as BGR, which is true also for the above C++ normalization routine. If the training was done in the effective RGB format, the C++ code should be changed as:
```cpp
 . . .
      for(int k = 0; k < image.cols; ++k) { //RGB
            data[j*image.rows*3+k*3+2-i] = (float(image.at<Vec3b>(j,k)[i])/255.0 - 0.5)*2 * scale;
     }
 . . .
```


A mismatch at this level would prevent the computation of the correct predictions at run time on the target board.

The following is the C++ code fragment to perform the images classification task:

```c++
vector<string> kinds, images;

void run_CNN(DPUTask *taskConv, Mat img)
{
  // Get channel count of the output Tensor
  int channel = dpuGetOutputTensorChannel(taskConv, CONV_OUTPUT_NODE);

  float *softmax = new float[channel];
  float *FCresult = new float[channel];
  float mean[3] = {0.0f, 0.0f, 0.0f};

  // Set image into Conv Task with mean value
  set_input_image(taskConv, CONV_INPUT_NODE, img, mean);

  //cout << "\nRun MNIST CONV ..." << endl;
  _T(dpuRunTask(taskConv));

  // Get FC result and convert from INT8 to FP32 format
  _T(dpuGetOutputTensorInHWCFP32(taskConv, CONV_OUTPUT_NODE, FCresult, channel));

  // Calculate softmax on CPU and show TOP5 classification result
  CPUCalcSoftmax(FCresult, channel, softmax);
  TopK(softmax, channel, 5, kinds);

  delete[] softmax;
  delete[] FCresult;
}

void classifyEntry(DPUKernel *kernelConv)
{

  /*Load all image names */
  ListImages(baseImagePath, images);
  if (images.size() == 0) {
    cerr << "\nError: Not images exist in " << baseImagePath << endl;
    return;
  } else {
    cout << "total image : " << images.size() << endl;
  }

  /*Load all kinds words.*/
  LoadWords(baseImagePath + "labels.txt", kinds);
  if (kinds.size() == 0) {
    cerr << "\nError: Not words exist in labels.txt." << endl;
    return;
  }

  thread workers[threadnum];
  auto_start = system_clock::now();

  for (auto i = 0; i < threadnum; i++)
  {
  workers[i] = thread([&,i]()
  {

    /*Create DPU Tasks for CONV*/
    DPUTask *taskConv = dpuCreateTask(kernelConv, DPU_MODE_NORMAL); //profiling not enabled

    for(unsigned int ind = i  ;ind < images.size();ind+=threadnum) {
      cout << "\nLoad image : " << images.at(ind) << endl;
      Mat img = imread(baseImagePath + images.at(ind)); //OpenCV read image as BGR!
      run_CNN(taskConv, img);
    }
    // Destroy DPU Tasks & free resources
    dpuDestroyTask(taskConv);
  });
  }

  // Release thread resources.
  for (auto &w : workers) {
    if (w.joinable()) w.join();
  }  

  auto _end = system_clock::now();
  auto duration = (duration_cast<microseconds>(_end - _start)).count();
  cout << "[Time]" << duration << "us" << endl;
  cout << "[FPS]" << images.size()*1000000.0/duration  << endl;

}
```
The only difference between the two C++ files, [top5_tf_main.cc](files/target_zcu102/fmnist/LeNet/src/top5_tf_main.cc) and [fps_tf_main.cc](files/target_zcu102/fmnist/LeNet/src/fps_tf_main.cc), is that in [top5_tf_main.cc](files/target_zcu102/fmnist/LeNet/src/top5_tf_main.cc), the ``printf`` instructions are not commented to allow the prediction accuracies to be printed at run time and captured into a logfile, for example, ``logfile_top5_LeNet.txt`` that will later be post-processed by the python script, ``check_runtime_top5_fashionmnist.py``, to generate the final logfile, for example, ``top5_accuracy_fmnist_LeNet.txt`` with the average top-5 accuracy over the test images dataset.

On the other hand, in [fps_tf_main.cc](files/target_zcu102/fmnist/LeNet/src/fps_tf_main.cc) all the ``printf`` are commented so that the elapsed time of the Vitis AI Platform can be measured accurately. The effective fps throughput depends on the amount of threads set at runtime. The script [run_fps_LeNet.sh](files/target_zcu102/fmnist/LeNet/run_fps_LeNet.sh) tries from one to eight threads and prints the effective fps that is stored in a logfile, for example, in ``fps_fmnist_LeNet.txt``.

>**:pushpin: NOTE** This number is a worst case as the images are loaded and preprocessed by the ARM CPU, which is slower in comparison to the Vitis AI Platform. In a real design, the images would be loaded by a sensor and pre-processed by a low-latency HW accelerator (for more information see the Xilinx&reg; [xfOpenCV](https://github.com/Xilinx/xfopencv) optimized library).

## 7.2 Build the application

Turn on your target board and establish a serial communication with a ``putty`` terminal from Ubuntu or with a ``TeraTerm`` terminal from your Windows host PC.

Ensure that you have an Ethernet point-to-point cable connection with the correct IP addresses to enable ``ssh`` communication in order to quickly transfer files to the target board with ``scp`` from Ubuntu or ``pscp.exe`` from Windows host PC. For example, you can set the IP addresses of the target board to be ``192.168.1.100`` while the host PC is  ``192.168.1.101`` as shown in the following figure:

![figure](files/doc/images/teraterm.png)

Once a ``tar`` file of the [target_zcu102](files/target_zcu102) folder has been created, copy it from the host PC to the target board. For example, in case of an Ubuntu PC, use the following command:
```
scp target_zcu102.tar root@192.168.1.100:/root/
```

## 7.3 Running the four CNNs   

From the target board terminal, run the following commands:
```
tar -xvf target_zcu102.tar
cd target_zcu102
source ./run_fmnist.sh
```

With this command, the ``fmnist_test.tar`` file with the 5000 test images will be uncompressed.
For each CNN, the whole application is then built with the ``make`` utility and finally launched, the effective top-5 classification accuracy is checked a python script like [check_runtime_top5_fashionmnist.py](files/target_zcu102/fmnist/LeNet/check_runtime_top5_fashionmnist.py).
Another script like [run_target.sh](files/target_zcu102/fmnist/LeNet/run_target.sh) then re-compiles the C++ code to measure the effective fps by calling, under the hood, a script like [run_fps_LeNet.sh](files/target_zcu102/fmnist/LeNet/run_fps_LeNet.sh) script.

The  [logfile_zcu102_fmnist.txt](files/rpt/logfile_zcu102_fmnist.txt) contains all the top-1 accuracy and fps performance of the four CNNs on the ZCU102 board.



# Summary

The following [Excel table](files/doc/summary_results.xlsx) summarizes the CNN features for each dataset and for each network in terms of:

- elapsed CPU time for the training process
- number of CNN parameters and number of epochs for the training processed
- TensorFlow output node names
- top-1 accuracies estimated for the TF frozen graph and the quantized graph
- top-1 accuracies measured on ZCU102 at run time execution
- frames per second (fps) -measured on ZCU102 at run time execution- including reading the images with OpenCV function from ARM CPU (while in the real life case these images will be stored into DDR memory and so their access time should be negligible as seen from the DPU IP core).

![figure](files/doc/images/summary_results.png)

Note that in the case of CIFAR-10 dataset, being more sophisticated than the Fashion-MNIST, the top-1 accuracies of the four CNNs are quite different with ``miniResNet`` being the most accurate.


# References
- https://www.pyimagesearch.com/2019/02/11/fashion-mnist-with-keras-and-deep-learning/
- https://www.pyimagesearch.com/deep-learning-computer-vision-python-book/
- https://github.com/Xilinx/Edge-AI-Platform-Tutorials/tree/master/docs/MNIST_tf
- https://www.dlology.com/blog/how-to-convert-trained-keras-model-to-tensorflow-and-make-prediction/
- https://github.com/Tony607/keras-tf-pb
- https://towardsdatascience.com/image-classifier-cats-vs-dogs-with-convolutional-neural-networks-cnns-and-google-colabs-4e9af21ae7a8
- https://blog.keras.io/building-powerful-image-classification-models-using-very-little-data.html
- https://machinelearningmastery.com/how-to-develop-a-convolutional-neural-network-to-classify-photos-of-dogs-and-cats/
- https://medium.com/datadriveninvestor/keras-imagedatagenerator-methods-an-easy-guide-550ecd3c0a92
- https://stats.stackexchange.com/questions/263349/how-to-convert-fully-connected-layer-into-convolutional-layer
- https://www.tensorflow.org/guide/extend/model_files

# Acknowledgements

- Thanks to Dr. Adrian Rosebrock, from [PyImageSearch](https://www.pyimagesearch.com/), for his permission (see [PyImageSearch Permission](#pyimagesearch-permission) Appendix)
to use his python code available in the "Starter Bundle" and "Practitioner Bundle" books of the [Deep Learning for CV with Python](https://www.pyimagesearch.com/deep-learning-computer-vision-python-book/) series.

- Thanks to the ladies Mohana Das and Erin Truax, my fantastic editors.

- Thanks to my colleagues Jane Guo, Parisa Rahimian and Jeff Rizzo, for their final check before publication.

- Thanks to my colleague Andy Luo, sponsor and reviewer of this project.

- Thanks to my colleague Yue Gao for her very-detailed review of this project, which really helped me a lot.

- Thanks to my colleagues Mark Harvey, Giovanni Guasti, Eddy De Waegeneer, Sophie Lemarie, Shuai Zhang, Xiaoming Sun, Fei Liu, Yi Shan, Fan Jiang, Xiao Sheng, Lu Tan, for their support during the development of this project.

[Daniele Bagni](mailto:danieleb@xilinx.com)


# Appendix

## A.1 Build and Run on the ZCU104 Target Board

The ``elf`` files generated for ZCU104 are necessarily different from the ones of ZCU102, because the DPU of the first board is smaller than the DPU of the second board. No changes to the C++ files are needed for these four CNN examples.

Working with ZCU104 board requires just to adopt the [5_cifar10_vai_compile_zcu104.sh](files/[5_cifar10_vai_compile_zcu104.sh) instead of the ZCU102 default one ([5_cifar10_vai_compile.sh](files/[5_cifar10_vai_compile.sh)).

Make a ``tar`` file of the [target_zcu104](files/target_zcu104) folder, copy it from the host PC to the target ZCU104 board. For example, in case of an Ubuntu PC, use the following command (assuming the board IP address is always the same):
```
scp target_zcu104.tar root@192.168.1.100:/root/
cd target_zcu104
source ./run_cifar10.sh
```
The  [logfile_cifar10_zcu104.txt](files/target_zcu104/logfile_cifar10_zcu104.txt) file contains all the top-1 accuracy and fps performance of the four CNNs trained with the CIFAR-10 dataset.  

If you want the results also for Fashion-MNIST dataset -not provided to save space- you have to run the  [5_fmnist_vai_compile_zcu104.sh](files/[5_fmnist_vai_compile_zcu104.sh) to generate the proper ``elf`` files.


## A.2 PyImageSearch Permission

```
From: Adrian at PyImageSearch [mailto:a.rosebrock@pyimagesearch.com]
Sent: Thursday, February 20, 2020 12:47 PM
To: Daniele Bagni <danieleb@xilinx.com>
Subject: Re: URGENT: how to cite / use your code in my new DL tutorials

EXTERNAL EMAIL
Hi Daniele,

Yes, the MIT license is perfectly okay to use. Thank you for asking :-)

All the best,


From: Adrian at PyImageSearch <a.rosebrock@pyimagesearch.com>
Sent: Friday, April 12, 2019 4:25 PM
To: Daniele Bagni
Cc: danny.baths@gmail.com

Subject: Re: how to cite / use your code in my new DL tutorials

Hi Daniele,
Thanks for reaching out, I appreciate it! And yes, please feel free to use the code in your project.
If you could attribute the code to the book that would be perfect :-)
Thank you!
--
Adrian Rosebrock
Chief PyImageSearcher

On Sat, Apr 6, 2019 at 6:23 AM EDT, Daniele Bagni <danieleb@xilinx.com> wrote:

Hi Adrian.

...

Can I use part of your code in my tutorials?
In case of positive answer, what header do you want to see in the python files?

...


With kind regards,
Daniele Bagni
DSP / ML Specialist for EMEA
Xilinx Milan office (Italy)
```



<hr/>
<p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>
