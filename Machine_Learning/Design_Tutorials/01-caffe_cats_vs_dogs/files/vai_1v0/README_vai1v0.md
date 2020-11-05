<table>
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>Vitis AI Tutorials</h1>
   </td>
 </tr>
 <tr>
 <td align="center"><h3> Quantization and Pruning of AlexNet CNN trained in Caffe with Cats-vs-Dogs dataset</h3>
 </td>
 </tr>
</table>


### Current status

1. Tested with Vitis AI 1.0 and 1.1

2. Tested in hardware on ZCU102 (both 1.0 and 1.1)


# 1 Introduction

This Deep Learning (DL) tutorial shows you how to train, prune, and quantize a modified version of the AlexNet convolutional neural network (CNN) with the Kaggle [Dogs vs. Cats](https://www.kaggle.com/c/dogs-vs-cats) dataset in order to deploy it on the Xilinx&reg; [ZCU102](https://www.xilinx.com/products/boards-and-kits/ek-u1-zcu102-g.html) board. You will use the [Caffe](http://caffe.berkeleyvision.org) environment available from [Vitis AI](https://developer.xilinx.com/en/get-started/ai.html), which is a set of optimized IP, tools libraries, models and example designs valid for AI inference on both Xilinx edge devices and Alveo cards.

Once the selected CNN has been correctly trained in Caffe, the ``caffemodel`` file of floating point weights is then quantized by the Vitis AI Quantizer that creates an 8-bit integer representation (named "INT8") from which the Vitis AI Compiler generates the ``elf`` file of micro instructions for the Deep Processor Unit (DPU). The final application is executed at run time - via C++ APIs - on the target board to make predictions that can be compared against the simulation reference results.

The Dogs vs. Cats dataset is composed of only two classes of objects to be classified: cats and dogs. It contains 25000 RGB images that have been organized into the following three databases (all the images are randomly shuffled before forming the database):
- ``train_lmdb``: 20000 images (resized to 256x256) in the LMDB database for the forward/backward training process.
- ``valid_lmdb``:  4000 images (resized to 256x256) in the LMDB database for the validation step during training process.
- ``test``: 1000 images (resized to 227x227) in plain JPEG format for the top-1 prediction measurements, once the CNN is trained.

AlexNet is a well-known CNN that works with images 227x227x3 in size. It is described in the
[Practitioner Bundle book](https://www.pyimagesearch.com/deep-learning-computer-vision-python-book) by [Dr. Adrian Rosebrock](https://www.linkedin.com/in/adrian-rosebrock-59b8732a/) from [PyImageSearch](https://www.pyimagesearch.com/) where it is modeled and trained in Keras/TensorFlow. The model adopted in this tutorial was manually translated into ``.prototxt`` files and trained with Caffe from scratch. Moreover some layers were organized differently:
- "Local Response Normalization" (LRN) layers were replaced by "Batch Normalization" (BN) layers;
- the "ReLU" activation layer was placed after the BN layer, and not before;
- the number of BN and "DROPOUT" layers were also reduced.

:pushpin: **Note:** The Xilinx [Vitis AI Optimizer](https://www.xilinx.com/html_docs/vitis_ai/1_0/zfz1565215654494.html), as known as "pruning", requires a license fee and can be accessed only at its [lounge](https://www.xilinx.com/member/ai_optimizer.html) page. Therefore the pruning tool is not included in this tutorial, although all the shell scripts to prune the CNN and related log files are available in the [pruning](files/pruning/alexnetBNnoLRN/) folder.


# 2 Prerequisites

- An Ubuntu 16.04 host PC with Python 3.6  and its package ``python3.6-tk`` installed (this last one installed with ``sudo apt-get install python3.6-tk``);

- [Vitis AI stack release 1.0](https://github.com/Xilinx/Vitis-AI/tree/v1.0) from [www.github.com/Xilinx](https://www.github.com/Xilinx). In particular, refer to the [Vitis AI User Guide UG1414 v1.0](https://www.xilinx.com/support/documentation/sw_manuals/vitis_ai/1_0/ug1414-vitis-ai.pdf) for the installation guidelines and note that you need to download the two containers available from [docker hub](https://hub.docker.com/r/xilinx/vitis-ai/tags):
  - **tools container** with tag ``tools-1.0.0-cpu``, here ``cpu`` means that this environment runs on the host PC only with CPU support (in other words without any GPU need). Note also that UG1414 explains how to build your own container with GPU support;
  - **runtime container** with tag ``runtime-1.0.1-cpu``, note that you need this container only once to prepare SD card content of the target board, then you will compile all the applications directly on the target board itself, which means you do not cross-compile them from the host PC.

- Vitis AI Evaluation board [ZCU102](https://www.xilinx.com/products/boards-and-kits/ek-u1-zcu102-g.html) with its [image file](https://www.xilinx.com/bin/public/openDownload?filename=xilinx-zcu102-dpu-v2019.2.img.gz), which contains a pre-built working design for the ZCU102 with the [DPU-v2](https://github.com/Xilinx/Vitis-AI/tree/master/DPU-TRD).

- With few changes, explained in the last part of the following section, this tutorial can work also with [Vitis AI stack release 1.1](https://github.com/Xilinx/Vitis-AI).

- Familiarity with Deep Learning principles.

- Familiarity with Caffe (here is the official [online Caffe tutorial](http://caffe.berkeleyvision.org/tutorial/)).


## 2.1  Dos-to-Unix Conversion

In case you might get some strange errors during the execution of the scripts, you have to pre-process -just once- all the``*.sh`` shell and the python ``*.py`` scripts with the [dos2unix](http://archive.ubuntu.com/ubuntu/pool/universe/d/dos2unix/dos2unix_6.0.4.orig.tar.gz) utility.


# 3 Before starting

You have to know few things about [Docker](https://docs.docker.com/) in order to run the Vitis AI smoothly.

In the following of this document, it is assumed that you have cloned the [Vitis AI stack release 1.0](https://github.com/Xilinx/Vitis-AI) in your working directory ``<WRK_DIR>``, for example a folder named ``~/ML/Vitis-AI``.

To list the currently available docker images run:
```bash
docker images # to list the current docker images available in the host pc
```
and you should see something like in the following text:
```text
REPOSITORY                             TAG                 IMAGE ID            CREATED             SIZE
xilinx/vitis-ai                        tools-1.0.0-gpu     034a766d9641        3 hours ago         12GB
xilinx/vitis-ai                        runtime-1.0.0-cpu   af058b4c48b3        6 weeks ago         11.9GB
xilinx/vitis-ai                        tools-1.0.0-cpu     7b7550aac2e6        6 weeks ago         8.35GB
```

To launch the docker container with Vitis AI tools - to do all the steps from CNN training to generation of the ELF file for the DPU - based on CPU (or GPU), execute the following commands from the ``<WRK_DIR>`` folder:

```bash
cd <WRK_DIR> # you are now in Vitis_AI subfolder
./docker_run.sh xilinx/vitis-ai:tools-1.0.0-cpu  # only CPU
#./docker_run.sh xilinx/vitis-ai:tools-1.0.0-gpu # alternatively: only GPU
```

To launch the docker container with Vitis AI runtime - to compile the whole application for the target board - from the ``<WRK_DIR>`` folder execute the following command:

```bash
cd <WRK_DIR> # you are now in Vitis_AI subfolder
./runtime/docker_run.sh xilinx/vitis-ai:runtime-1.0.0-cpu
```

Note that both the two containers map the shared folder ``/workspace`` with the file system of the Host PC from where you launch the above command, which is ``<WRK_DIR>`` in your case. This shared folder enables you to transfer files from the Host PC to the docker container and vice versa.

The docker container do not have any graphic editor, so it is recommended that you work with two terminals and you point to the same folder, in one terminal you use the docker container commands and in the other terminal you open any graphic editor you like.



Note that docker does not have an automatic garbage collection system as of now. You can use this command to do a manual garbage collection:
```
docker rmi -f $(docker images -f "dangling=true" -q)
```


## 3.1 Install Missing Packages on the Vitis AI Tools Container

You have to modify the Vitis AI tools container. Here are the commands:
```bash
./docker_run.sh xilinx/vitis-ai:tools-1.0.0-cpu # enter into the docker Vitis AI 1.0 tools image
#./docker_run.sh xilinx/vitis-ai:1.1.56 # enter into the docker Vitis AI 1.1 tools image    
sudo su # you must be root
conda activate vitis-ai-caffe     # as root, enter into Caffe (anaconda-based) virtual environment
conda install libprotobuf==3.10.1 # only for Vitis AI == 1.0
conda install protobuf==3.10.1    # only for Vitis AI == 1.0
conda install pycairo==1.18.2     # for Vitis AI >= 1.0
pip install lmdb==0.98            # for Vitis AI >= 1.0
conda deactivate
exit # to exit from root
conda activate vitis-ai-caffe # as normal user, enter into Caffe (anaconda-based) virtual environment
```
Note that if you exit from the current  Vitis AI tools docker image you will lose all the installed packages, so to save all changes just open a new terminal and run the following commands:

```bash
sudo docker ps -l # To get the Docker CONTAINER ID
```

you will see the following text (the container ID might have a different number):

```text
CONTAINER ID        IMAGE                             COMMAND                CREATED     STATUS       NAMES
5310263294ba        xilinx/vitis-ai:tools-1.0.0-cpu   "/etc/login.sh bash"   something   something    something
```

now save the modified docker image:

```bash
sudo docker commit -m"new image: added keras 2.2.4 and pandas, seaborn, matplotlib, imutils" \
       5310263294ba xilinx/vitis-ai:tools-1.0.0-cpu
```

Assuming you have renamed this project ``VAI-Caffe-ML-CATSvsDOGS`` and placed it in the directory named ``<WRK_DIR>/tutorials/`` so that it is two levels below the ``Vitis-AI`` folder, you can launch the modified tools container by running the following commands:
```bash
cd <WRK_DIR>
./docker_run.sh xilinx/vitis-ai:tools-1.0.0-cpu
#../docker_run.sh xilinx/vitis-ai:tools-1.0.0-gpu # just in case you have GPU support
cd /workspace/tutorials/VAI-Caffe-ML-CATSvsDOGS
```

### Copy caffe-xilinx.zip

Now, enter into the [AI Model Zoo README.md](https://github.com/Xilinx/AI-Model-Zoo/blob/master/README.md) file and download the [caffe-xilinx.zip](https://www.xilinx.com/bin/public/openDownload?filename=caffe-xilinx-1.1.zip) archive.
Unzip it at the same level of this project directory so that both ``VAI-Caffe-ML-CATSvsDOGS`` and ``caffe-xilinx`` repositories are placed in ``<WRK_DIR>/tutorials/``. Leave it as it is, you do not need to compile and build anything.

You should see something as illustrated in the screenshot of Figure 0:

![figure0](files/doc/images/vai_page.png)

*Figure 0: Entering into the Vitis AI tools container (screenshot taken from Vitis AI 1.2 release).*

You will use only the following python2.7 scripts placed in the folder ``caffe-xilinx/tools/extra``:
- ``caffe-xilinx/tools/extra/parse_log.py``
- ``caffe-xilinx/tools/extra/extract_seconds.py``

Note that you need to modify line number 166 of code ``parse_log.py`` to make it compatible with python >= 3.6:
```python
print 'Wrote %s' % output_filename
```
into
```python
print ("Wrote %s" % output_filename)
```


## 3.2 The Vitis AI 1.0 Runtime Container

This docker image can support both **Vitis AI Library** (named **AI SDK** prior to Vitis) and **DNNDK** cross compiler. In particular:

1. Both **Vitis AI library** and  **Vitis AI for Zynq/ZU+** use the same cross compiler from Petalinux. In the previous DNNDK releases, there was no cross compilation at all and the compilation was done directly on the target board.

2. **Vitis AI samples** uses new unified APIs for both Zynq/ZU+ and Alveo platforms in this release, while **DNNDK samples** still use legacy DNNDK Level1 APIs for back compatibility with previous DNNDK releases (for example the 3.1).

3. **Vitis AI samples** included in the runtime docker do not belong to **Vitis AI Library** although their names seem related. In fact **Vitis AI Library** is designed to be higher level for quick AI application development, this library also provides APIs and optimized routines for pre- and post-processing.

Assuming your project is placed in the directory named ``<WRK_DIR>/tutorials/VAI-Caffe-ML-CATSvsDOGS``, which is two levels below the ``Vitis_AI`` repository, you can launch the runtime container by running the following commands:
```bash
cd <WRK_DIR>
./docker_run.sh xilinx/vitis-ai:runtime-1.0.0-cpu
```


## 3.3 Prepare the SD Card for the Target Board

Chapter 2 of [Vitis AI UG1414 v1.0](https://www.xilinx.com/support/documentation/sw_manuals/vitis_ai/1_0/ug1414-vitis-ai.pdf) contains all the necessary information to setup the target board and to prepare the SD card content. In particular, remember to copy (with ``scp``) to the target board both the ``xilinx_vai_board_package`` folder from the runtime container (located in ``/opt/vitis_ai/``) and the ``dnndk_samples_zcu102`` folder from the ``mpsoc`` folder of Vitis AI.

At the end of this procedure you should see something as illustrated in the screenshot of Figure 1:

![figure1](files/doc/images/zcu102_packages.png)

*Figure 1: Screenshot of ZCU102 terminal with folders copied from the runtime container.*

Note  that the folder [zcu102/common](files/deploy/alexnetBNnoLRN/zcu102/common) of this repository is a copy of the folder [mpsoc/dnndk_samples_zcu102/common](https://github.com/Xilinx/Vitis-AI/tree/master/mpsoc/dnndk_samples_zcu102/common), just for your comfort.



## 3.4 Changes for Vitis AI 1.1

There are few differences between Vitis AI 1.0 and 1.1 releases for what concerns the edge devices:

1. you have to use the proper Docker Image for Vitis AI 1.1

  ```
  xilinx/vitis-ai           1.1.56                            798f6eaea389        3 months ago        9.5GB
  ```

  which is different from the images associated with 1.0:

  ```
  xilinx/vitis-ai           tools-1.0.0-cpu                   37ff1cd99ecb        3 months ago        8.59GB
  xilinx/vitis-ai           tools-1.0.0-gpu                   1b45847f369d        3 months ago        12GB
  ```

2. You have to follow the Vitis AI 1.1 instructions for [Setting Up the Evaluation Boards](https://www.xilinx.com/html_docs/vitis_ai/1_1/yjf1570690235238.html).

3. Starting from Vitis AI 1.1 release there is no more Docker Runtime Container, and you can cross compile the ``elf`` files directly from the host PC to the target board. You have to execute all the instructions of [Legacy DNNDK examples](https://www.xilinx.com/html_docs/vitis_ai/1_1/ump1570690283280.html) to setup ``petalinux/2019.2`` and all the DNNDK application files and libraries, so that you can finally run everything on your target board.
In the following of this tutorial it is assumed that ``petalinux`` is installed into ``/opt/petalinux/2019.2`` of your host PC.

Then, the Vitis AI 1.1 Caffe flow is basically the same of Vitis AI 1.0.

A part for the above changes, all the rest of the flow -with python and shell script files- used in this tutorial with Vitis AI 1.0 works also with  Vitis AI 1.1.


# 4 Project Directory Structure

In Caffe, ``.prototxt`` files cannot use Linux environmental variables, only relative pathnames. This project therefore assumes the following fixed subdirectories, placed under ``/workspace/tutorials/VAI-Caffe-ML-CATSvsDOGS/``:

- ``caffe/code`` contains all the Python scripts;

- ``caffe/models`` contains the solver, training, and deploy``.prototxt`` files;

- ``deploy`` contains the files for quantization of either the baseline (``quantiz``) or pruned (``pruned``) CNN, plus the files for ZCU102 run-time execution (``zcu102/baseline``, ``zcu102/pruned``, and ``zcu102/test_images`` respectively);

- ``input`` contains the following:
  - [LMDB](https://en.wikipedia.org/wiki/Lightning_Memory-Mapped_Database) databases for the Caffe phases of training and validation;
  - JPEG images for testing the top-1 accuracy;
  - other JPEG images for calibration during the quantization process.


#  5 How To Get The Dogs vs. Cats Dataset

The dataset cannot be hosted in this repository because of its large size. To obtain the dataset, you need to register on the [Kaggle website](https://www.kaggle.com), setting up a username and password. The registration procedure involves receiving confirmation codes by email and smartphone. When you are registered, you can download the [dogs-vs-cats.zip](https://www.kaggle.com/biaiscience/dogs-vs-cats/download) archive and put it at the same level of ``input`` and ``caffe`` sub-folders.

Note that you do not need to use the original ``test`` archive, because it does not have labeled images and is therefore not useful for this tutorial.


# 6 Python and Shell Scripts

The shell script [run_all_alexnet.sh](files/run_all_alexnet.sh) does all the job for the AlexNet CNN in this order:

- it sets the environmental variables required by this project by calling  [set_prj_env_variables.sh](files/caffe/set_prj_env_variables.sh); note that this script must be called mandatory prior to any other of the remaining scripts;

- it sets the project directories by launching  [set_the_CATSvsDOGS_prj.py](files/set_the_CATSvsDOGS_prj.py) that organizes the images taken from the ``train.zip`` archive, note that this archive must be placed at the same level of folders ``input`` ``log`` and ``caffe``;

- it runs the whole Caffe flow by calling  [caffe_flow_AlexNet.sh](files/caffe/caffe_flow_AlexNet.sh) script;

- it launches the Vitis AI Quantizer  [vaiq_alexnetBNnoLRN.sh](files/deploy/alexnetBNnoLRN/quantiz/vaiq_alexnetBNnoLRN.sh) and Compiler [vaic_alexnetBNnoLRN.sh](files/deploy/alexnetBNnoLRN/quantiz/vaic_alexnetBNnoLRN.sh) scripts.

In order of execution, the Python scripts that compose the Caffe design flow are listed below. They enable you to create the datasets, train your CNN with a training and validation LMDB database, and finally make predictions on JPEG images. They all are launched by [caffe_flow_AlexNet.sh](files/caffe/caffe_flow_AlexNet.sh).

a) [1_write_cats-vs-dogs_images.py](files/caffe/code/1_write_cats-vs-dogs_images.py): this script creates  subfolders ``test``, ``train``, ``val`` and  ``calib``. The ``calib`` folder is needed only for quantization. You only need to execute it once.  

b) [2a_compute_mean.py](files/caffe/code/2a_compute_mean.py): this script computes the three mean values for the ``train_lmdb`` database. You only need to execute it once.  

c) [2b_create_lmdb.py](files/caffe/code/2b_create_lmdb.py): this script creates the LMDB databases ``input/lmdb/train_lmdb`` and ``input/lmdb/valid_lmdb`` for the training step. You only need to execute it once.

d) [3_read_lmdb.py](files/caffe/code/3_read_lmdb.py): this script can be used to debug the first two scripts.

e) [4_training.py](files/caffe/code/4_training.py): this script launches the real training process in Caffe, given certain ``solver`` and CNN description ``.prototxt`` files. To be used for any trial of training.

f) [5_plot_learning_curve.py](files/caffe/code/5_plot_learning_curve.py): this script is launched at the end of the training to plot the learning curves of accuracy and loss.

g) [6_make_predictions.py](files/caffe/code/6_make_predictions.py): this script is launched at the end of the training to measure the average prediction accuracy achieved by the CNN. You need to have the ``scikit`` (classic ML) library installed.

The script  [check_dpu_runtime_accuracy.py](files/deploy/alexnetBNnoLRN/zcu102/baseline/check_dpu_runtime_accuracy.py) is not part of the Caffe flow and it has to be launched only when the CNN is running on the DPU of ZCU102 board, to compute the effective top-1 accuracy of the AI inference. Doing this allows you to make a fair comparison between the top-1 accuracy values if simulated by the ``.caffemodel`` in Caffe on the _host_ PC, or if estimated by Vitis AI Quantizer on the _host_ PC or if measured at run time on the _target_ ZCU102 board.



# 7 AlexNet Caffe Training Files

## 7.1 CNN Model Description
To describe the CNN in Caffe, you need a ``.prototxt`` text file which shows the type of layers and how they are connected, plus some specific actions to be done only during the training or validation phases, indicated as TRAIN and TEST respectively. You also need to set the ``batch_size`` during the TRAIN and TEST phases: 128 (or also 256, depending on the memory of your GPU card) and 80 respectively. During the TRAIN phase, all the parameters of the CNN are updated by the ``Adam`` solver, every ``batch_size`` number of images.

The model giving the best top-1 prediction results in previous experiments is [train_val_2_alexnetBNnoLRN.prototxt](files/caffe/models/alexnetBNnoLRN/m2/train_val_2_alexnetBNnoLRN.prototxt). Associated with it, you also have the [deploy_2_alexnetBNnoLRN.prototxt](files/caffe/models/alexnetBNnoLRN/m2/deploy_2_alexnetBNnoLRN.prototxt), which is needed to compute the prediction accuracy on the 1000 images in the ``test`` subfolder (the same that will be used at run time on the ZCU102).

There is another model, [q_train_val_2_AalexnetBNnoLRN.prototxt](files/caffe/models/alexnetBNnoLRN/m2/q_train_val_2_alexnetBNnoLRN.prototxt), which will be applied later during the quantization process of the baseline CNN. It is exactly the same as [train_val_2_alexnetBNnoLRN.prototxt](files/caffe/models/alexnetBNnoLRN/m2/train_val_2_alexnetBNnoLRN.prototxt), but the LMDB database of the TRAIN phase has been replaced by the calibration images.


## 7.2 Caffe Training Process Solver

In Caffe, the solver file defines the optimization method (that is, ``SGD``, or ``Adam``, or ``Nesterov``), the number of iterations, and the policy for changing the learning rate during the various iterations. It also says if a CPU or GPU is being used for computation.

The solver file is named [solver_2_alexnetBNnoLRN.prototxt](files/caffe/models/alexnetBNnoLRN/m2/solver_2_alexnetBNnoLRN.prototxt), and contains the settings for the training of the AlexNet model that have proved to be optimal during the previous experiments.

## 7.3 Prediction Accuracy Performance

The output of each process running on the host side is captured and stored into a single unified log file, named [logfile_run_alexnet_host.txt](files/log/logfile_run_alexnet_host.txt); in the following of this section some fragments of such file are reported.

After training is executed, the CNN has a top-1 average accuracy of 92.2% (with 2000 iterations) computed on the validation dataset:

```
... 2541 solver.cpp:384] Iteration 20000, loss = 0.0335872
... 2541 solver.cpp:424] Iteration 20000, Testing net (#0)
... 2541 solver.cpp:523] Test net output #0: accuracy = 0.94725
... 2541 solver.cpp:523] Test net output #1: loss = 0.199691 (* 1 = 0.199691 loss)
... 2541 solver.cpp:523] Test net output #2: top-1 = 0.94725
```

When making predictions with the 1000 test images, the average top-1 prediction accuracy is 93%:
```
                precision recall  f1-score   support

cat             0.95      0.92      0.94       500
dog             0.92      0.96      0.94       500

accuracy                            0.94      1000
macro avg       0.94      0.94      0.94      1000
weighted avg    0.94      0.94      0.94      1000

SKLEARN Accuracy = 0.94

```
Since  GPUs have varying random states, and you might not achieve exactly the same numerical results.


# 8 Quantization of the AlexNet


## 8.1 Input and Output .prototxt Files

The Vitis AI Quantizer tool needs the following input files:
- ``float.prototxt``: this is the description text file of the floating point CNN model;
- ``float.caffemodel``: this is the  weights file of the CNN in floating point;
- ``calibration dataset``: this is a subset of the images used in the original training, containing about 1000 pictures in this case study.

When the quantization is done, two output files are generated. These become the inputs to the  Vitis AI Compiler:
- ``deploy.prototxt``: this is the new description text file of the quantized CNN model.
- ``deploy.caffemodel``: this is the file with fixed point quantized weights (note that this is not a standard Caffe format).

Preparing the input ``.prototxt`` files requires the following steps.

a) Take the weights file generated after the Caffe training process (``snapshot_2_alexnetBNnoLRN__iter_20000.caffemodel``), and rename it simply ``float.caffemodel``.

b) Take the description file used in the Caffe training process ([train_val_2_alexnetBNnoLRN.prototxt](files/caffe/models/alexnetBNnoLRN/m2/train_val_2_alexnetBNnoLRN.prototxt)), and rename it simply ``float.prototxt``.

c) Make the following further changes to the ``float.prototxt`` file:
- remove the ``Datatype`` layers for the original TRAIN phase;
- add an ``ImageData`` type layer with the calibration images folder for the new TRAIN phase;
- on the bottom, add one ``Accuracy`` layer to compute top-1 prediction accuracy.

For your reference, the above changes were already made in  [q_train_val_2_alexnetBNnoLRN.prototxt](files/caffe/models/alexnetBNnoLRN/m2/q_train_val_2_alexnetBNnoLRN.prototxt) file.


## 8.2 Quantization Flow

The effective quantization flow is composed mainly of five steps.

1. Quantize the CNN floating point model to INT8 by using the Vitis AI Quantizer (see script [vaiq_alexnetBNnoLRN.sh](files/deploy/alexnetBNnoLRN/quantiz/vaiq_alexnetBNnoLRN.sh)).

2. Compile the INT8 CNN model by using the Vitis AI Compiler  (see script [vaic_alexnetBNnoLRN.sh](files/deploy/alexnetBNnoLRN/quantiz/vaic_alexnetBNnoLRN.sh)).

3. Edit the [main.cc](files/deploy/alexnetBNnoLRN/zcu102/baseline/src/top2_main.cc) program application.

4. Compile the hybrid (CPU + DPU) application with [crosscompile_alexnet.sh](files/deploy/alexnetBNnoLRN/zcu102/ crosscompile_alexnet.sh) shell script.
  - In case of Vitis AI 1.1 run the following:
  ```bash
  unset LD_LIBRARY_PATH    #required by petalinux for Vitis >= 1.1
  sh /opt/petalinux/2019.2/environment-setup-aarch64-xilinx-linux # set petalinux environment of Vitis AI 1.1
  cd <WRK_DIR>/tutorials/VAI-Caffe-ML-CATSvsDOGS/files
  cd deploy/zcu102
  source crosscompile_alexnet.sh
  cd ..
  tar -cvf zcu102.tar ./zcu102 # to be copied on the SD card
  ```
  - In case of Vitis AI 1.0 run the following:
  ```bash
  cd <WRK_DIR> # you are now in Vitis_AI subfolder
  ./runtime/docker_run.sh xilinx/vitis-ai:runtime-1.0.0-cpu
  cd <WRK_DIR>/tutorials/VAI-Caffe-ML-CATSvsDOGS/files
  cd deploy/zcu102
  source crosscompile_alexnet.sh
  cd ..
  tar -cvf zcu102.tar ./zcu102 # to be copied on the SD card
  ```

5. Assuming you have transferred the ``zcu102.tar`` archive from the host to the target board with ``scp`` utility, now you can run the hybrid application (from the _target_ board side). See below the command line example for the AlexNet case:
   ```
   tar -xvf zcu102.tar
   cd zcu102/baseline
   source run_all.sh
   ```

This command  also runs the script [check_dpu_runtime_accuracy.py](files/deploy/alexnetBNnoLRN/zcu102/baseline/check_dpu_runtime_accuracy.py) to check the top-1 prediction accuracy that AI Inference DPU achieves at run time on the test images. This is the most important step, because you can now see the real average accuracy of your CNN system working at run time.

The estimated top-1 average accuracy after quantization (computed on the validation dataset) can be seen in one of the last lines of the captured [log file](files/log/logfile_run_alexnet_host.txt), 91.8% is achieved (with only a 0.4% drop in comparison with the floating point model):

```
... ] Test iter: 50/50, loss = 0.46556
... ] Test iter: 50/50, top-1 = 0.92
... ] Test Results:
... ] Loss: 0.201669
... ] accuracy = 0.9464
... ] loss = 0.201669 (* 1 = 0.201669 loss)
... ] top-1 = 0.9464
... ] Test Done!
```

In step 2, Vitis AI Compiler says that there is one kernel task running on the DPU (named ``alexnetBNnoLRN_0`` and implementing the CONV, ReLU, BN, and FC layers) and another kernel (named ``alexnetBNnoLRN_1``) running the top-2 and SoftMax layers in software on the ARM CPU of ZCU102, as illustrated by below fragment of [log file](files/log/logfile_run_alexnet_host.txt):
```
   Kernel Name : alexnetBNnoLRN_0
---------------------------------------------------------
   Kernel Type : DPUKernel
     Code Size : 0.18MB
    Param Size : 55.59MB
 Workload MACs : 2262.34MOPS
  IO Mem Space : 0.22MB
    Mean Value : 106, 116, 124,

  Input Node(s)   (H*W*C)
      conv1(0) : 227*227*3
  Output Node(s)  (H*W*C)
        fc8(0) :   1*1*2

   Kernel Name : alexnetBNnoLRN_1
-----------------------------------------------------------
   Kernel Type : CPUKernel
  Input Node(s)  (H*W*C)
          loss :  1*1*2
  Output Node(s) (H*W*C)
          loss :  1*1*2
```


## 8.4  Main  Application
This is the file that has to be compiled on the ARM CPU. It controls the DPU hardware accelerator using the DNNDK DPU APIs. Two variants are available: [top2_main.cc](files/deploy/alexnetBNnoLRN/zcu102/baseline/src/top2_main.cc) to compute the top-2 prediction accuracy, and [fps_main.cc](files/deploy/alexnetBNnoLRN/zcu102/baseline/src/fps_main.cc) to compute the maximum throughput measured in frames per second (fps).

The variants only differ in a few ``printf`` lines, which are used in the first file but commented out in the second file. They print the top-2 accuracies for each input image to be classified, as seen in the example below:
```c++
// from void TopK(const float *d, int size, int k, vector<string> &vkind) {
// ...
      pair<float, int> ki = q.top();
      printf("[Top]%d prob = %-8f  name = %s\n", i, d[ki.second], vkind[ki.second].c_str());
// ...

// from void classifyEntry(DPUKernel *kernelconv){
// ...
	    cout << "DBG imread " << baseImagePath + images.at(ind) << endl;
// ...
```

If you change the way this information is printed in the ``stdout``, you must also change the Python script [check_dpu_runtime_accuracy.py](files/deploy/alexnetBNnoLRN/zcu102/baseline/check_dpu_runtime_accuracy.py) accordingly, because it acts essentially as a text parser of the ``logfile_top2_alexnetBNnoLRN.txt`` captured at run time.

Also in the first lines of the [top2_main.cc](files/deploy/alexnetBNnoLRN/zcu102/baseline/src/top2_main.cc) file, you need to report the kernel name, input node, and output node that were generated by the Vitis AI Compiler in the [log file](files/log/logfile_run_baseline_target.txt):
```c++
#define KERNEL_CONV "alexnetBNnoLRN_0"
#define CONV_INPUT_NODE "conv1"
#define CONV_OUTPUT_NODE "fc2"
```

## 8.5 DNNDK DPU APIs

The three routines in [top2_main.cc](files/deploy/alexnetBNnoLRN/zcu102/baseline/src/top2_main.cc) that make use of DPU APIs are ``main()``, ``classifyEntry()`` and ``run_alexnetBNnoLRN()``. These routines are described below:
- ``main()`` opens the DPU device, loads the convolutional kernel, and finally destroys and closes it when the classification routine is terminated just before the return command.
- ``classifyEntry()`` is the classification routine. It creates a task on the DPU for the convolutional kernel it has to run (in general, one task for each separate kernel you might have). It does this in a multi-threading way, where the number of parallel threads is an argument you have to pass to the executable running on the ARM CPU using ``argc/argv``.
- ``run_alexnetBNnoLRN()`` does most of the work, as shown in the following code snippet:
```
void run_alexnetBNnoLRN(DPUTask *taskConv, Mat img) {
    assert(taskConv );

    int channel = dpuGetOutputTensorChannel(taskConv, CONV_OUTPUT_NODE);
    float *softmax = new float[channel];
    float *FCResult = new float[channel];
    _T(dpuSetInputImage2(taskConv, CONV_INPUT_NODE, img));

    _T(dpuRunTask(taskConv));

    _T(dpuGetOutputTensorInHWCFP32(taskConv, CONV_OUTPUT_NODE, FCResult, channel));
    _T(CPUCalcSoftmax(FCResult, channel, softmax));
    _T(TopK(softmax, channel, 2, kinds));

    delete[] softmax;
    delete[] FCResult;
}
```

## 8.6 Performance of the Quantized Baseline AlexNet on ZCU102

At the end of this quantization procedure, when the AI Inference DPU runs the ``alexnetBNnoLRN`` on the ZCU102 to make predictions on the 1000 test images, the following performance is reported as illustrated by the following fragments of [baseline logfile](files/log/logfile_run_baseline_target.txt):

- 154 fps with six threads

 ```
 ./fps_alexnetBNnoLRN 6
 now running ./fps_alexnetBNnoLRN 6
 total image : 1000
 [Time]6382428us
 [FPS]156.68
```

- 92% top-1 average accuracy

 ```
 number of total images predicted  999
 number of top1 false predictions  61
 number of top1 right predictions  938
 top1 accuracy = 0.94
 ```

# 9. Pruning the AlexNet

The pruning reference documentation can be found in the [Vitis AI Optimizer UG1431](https://www.xilinx.com/html_docs/vitis_ai/1_0/thf1576862844211.html). This section gives instructions to effectively prune the AlexNet CNN.


## 9.1 Introduction

Pruning is a technique to remove redundant or less useful weights and output channels from a CNN layer to reduce or compress the overall number of operations. The aim is to reduce the number of operations and increase the frames per second; you might not need it if your CNN is already optimized by design. This can however be detrimental to the average top-1 accuracy: the final result is ultimately a trade-off between the desired compression and the effective accuracy to sustain a certain target frame rate.

There are usually two types of pruning: fine and coarse. Fine pruning selectively kills either the weights or the output features with the smallest values from a channel. To achieve higher frame rates from a fine-compressed CNN, the hardware accelerator must be enabled to perform zero-skipping (that is, skipping all the multiplications with zero values). Zero-skipping requires a proper hardware architecture and organization of non-zero data (usually with [run-length coding](https://en.wikipedia.org/wiki/Run-length_encoding)) in the internal memory of the hardware accelerator; otherwise, there would be no performance gain from fine pruning.

The Xilinx Vitis AI Optimizer applies coarse pruning, which involves removing a complete output channel. In this case, any hardware accelerator can gain from it.

However, this invasive kind of pruning can affect the average accuracy. It is therefore important to apply the pruning in an iterative manner: for example, by compressing the CNN by only 10% and then performing fine-tuning (which can be a complete training process) to recover the probable accuracy drop. If you work carefully and apply this process for 7-8 steps (or even more), you can arrive at 70-80% of compression with a negligible top-1 average accuracy decrease. This iterative process can take a lot of time, especially if you are using a large database.

At the end of the pruning process, you get a new  floating point ``.caffemodel`` file of a size probably reduced by 40-60% (depending on the CNN) in comparison with the original ``.caffemodel`` file of the baseline (non-pruned) CNN. To run it on the ZCU102 board, you need to apply quantization using the output files generated by pruning (with some minor but important manual editing) as the input file to quantization.

## 9.2 Prepare the Input Files for Pruning

Before you begin, you need to have the following files in the ``files/pruning/alexnetBNnoLRN/`` working directory:

- [config.prototxt](files/pruning/alexnetBNnoLRN/config.prototxt): Use this file to set the number of GPU devices and test iterations, as well as the Caffe model description, weights files, and compression ratio you want to achieve. In reality, seven files like this are indeed adopted, and each one applies the weights generated by the previous pruning trial to increment the compression by 10%.

- [solver.prototxt](files/pruning/alexnetBNnoLRN/solver.prototxt): This is the same solver of your original ``.caffemodel``, just renamed (for example, the same [solver_2_alexnetBNnoLRN.prototxt](files/caffe/models/alexnetBNnoLRN/m2/solver_2_alexnetBNnoLRN.prototxt) that was already adopted during the training process). In general, you can also try to reduce the amount of iterations, depending on your specific case. For this CNN, 12000 iterations per each 10% of pruning step are good enough, instead of using the 20000 iterations as in the original training.

- [train_val.prototxt](files/pruning/alexnetBNnoLRN/train_val.prototxt): This is the same description file of your original ``.caffemodel``, but renamed. For example, it is the same as the [train_val_2_alexnetBNnoLRN.prototxt](files/caffe/models/alexnetBNnoLRN/m2/train_val_2_alexnetBNnoLRN.prototxt). Note that you need to edit [train_val.prototxt](files/pruning/alexnetBNnoLRN/train_val.prototxt) to add top-1 layer at its end.

- ``float.caffemodel``. This is the same weights file of your original ``.caffemodel``, only renamed (for example, the same ``snapshot_2_alexnetBNnoLRN__iter_20000.caffemodel``).


## 9.3 Pruning Flow

In previous experiments, pruning the AlexNet required seven steps of 10% compression each time. The flow can be explained by looking at the [pruning_flow.sh](files/pruning/alexnetBNnoLRN/pruning_flow.sh) shell script:

- ``analysis (ana)`` has to be executed only once at the beginning, and generates a hidden text file, ``.ana.regular``, that is then reused by all following trials. This process can take a lot of time, so it is recommended to comment out the related line in the shell script after you have executed it once (assuming you are not changing the input files).

- Seven steps  of ``compress / finetune`` actions, each one compressing the previously compressed CNN by 10% more. In particular, ``compress`` is responsible for heuristically selecting the channels to kill, while ``finetune`` performs a retrain to restore the top-1 accuracy at the previous value if possible.

- The final action is ``transform``, which transforms the intermediate sparse CNN model into the effective output ``.caffemodel`` of the compressed CNN (``transform.caffemodel``).

In the ``transform`` step, you need to complete the following steps:

1. Take the [final.prototxt](files/pruning/alexnetBNnoLRN/regular_rate_0.7/final.prototxt) generated by the seventh step of ``compress-finetune``.
2. Take the final finetuned ``.caffemodel`` named ``regular_rate_0.7/snapshot/_iter_12000.caffemodel``. This is also illustrated in the [pruning_flow.sh](files/pruning/alexnetBNnoLRN/pruning_flow.sh) shell script.

The command to prune the whole CNN is as follows:
 ```
 cd <WRK_DIR>/tutorials/VAI-Caffe-ML-CATSvsDOGS/files/
 source pruning/pruning_flow.sh
 ```

**:warning: IMPORTANT:** Check or change the pathnames inside the ``solver``, ``train_val``, and ``config*.prototxt`` files mentioned above.

## 9.4 Pruning Process Outputs

After seven rounds of ``compress`` and ``finetune`` two output files are generated from the three input files (``float.caffemodel``,  [solver.prototxt](files/pruning/alexnetBNnoLRN/solver.prototxt) and [train_val.prototxt](files/pruning/alexnetBNnoLRN/train_val.prototxt)). The output files are ``transformed.caffemodel`` and  [final.prototxt](files/pruning/alexnetBNnoLRN/regular_rate_0.7/final.prototxt). These become the input files to the next quantization process.

The compressed AlexNet now has ~87% less operations and ~70% less weights than the original baseline CNN, as reported in the ``logfile_compress7_alexnetBNnoLRN.txt``.

After the last ``finetune`` step, the estimated top-1 average prediction accuracy is  ~95%, as reported in the  ``logfile_finetune7_alexnetBNnoLRN.txt`` log file. In this case, the top-1 accuracy is measured on the validation dataset. To measure the effective top-1 average accuracy on the DPU at run time, you need to quantize the CNN you just pruned.

In conclusion, as reported in ``logfile_stat_alexnetBNnoLRN_original.txt``, the original baseline floating point CNN model has the following complexity:
 ```
 ... net_counter.cpp:114] Total operations: 2153918368
 ... net_counter.cpp:115] Total params:        3764995
 ```
whereas, as reported in ``logfile_stat_alexnetBNnoLRN_pruned.txt``, the pruned CNN has:
 ```
 ... net_counter.cpp:114] Total operations: 641045830
 ... net_counter.cpp:115] Total params:        482197
 ```


# 10 Quantization of the Pruned AlexNet

## 10.1 Pruned Quantization Flow

The process is exactly the same at that explained in the [Quantization of the AlexNet](README.md#8-quantization-of-the-alexnet) section. The only difference is that the input files are now named as follows:

- ``transformed.caffemodel``: The output of the ``transform`` step from the pruning process.

- ``q_final.prototxt``: Generated by manually editing the same [final.prototxt](files/pruning/alexnetBNnoLRN/regular_rate_0.7/final.prototxt) that was produced at the end of the pruning process.

You also need to replace the training LMDB database with the calibration images, and add the three mean values. Pruning is not a deterministic process, so every pruning trial can create a different ``final.prototxt`` file, and in that case you have to re-edit a new ``q_final.prototxt`` file (the [q_final.prototxt](files/deploy/alexnetBNnoLRN/pruned/q_final.prototxt) is solely for documentation).

To compile with the Vitis AI quantizer and compiler tools, call the two shell scripts, [vaiq_pruned_alexnetBNnoLRN.sh](files/deploy/alexnetBNnoLRN/pruned/vaiq_pruned_alexnetBNnoLRN.sh) and [vaic_pruned_alexnetBNnoLRN.sh](files/deploy/alexnetBNnoLRN/pruned/vaic_pruned_alexnetBNnoLRN.sh), with the following commands:
 ```
 cd <WRK_DIR/tutoriala/VAI-Caffe-ML-CATSvsDOGS/files/
 source deploy/alexnetBNnoLRN/vaiq_pruned_alexnetBNnoLRN.sh 2>&1 | tee deploy/alexnetBNnoLRN/pruned/rpt/logfile_vaiq_pruned_alexnetBNnoLRN.txt
 source deploy/alexnetBNnoLRN/vaic_pruned_alexnetBNnoLRN.sh 2>&1 | tee deploy/alexnetBNnoLRN/pruned/rpt/logfile_vaic_pruned_alexnetBNnoLRN.txt
 ```

At the end of the Quantization process on the pruned AlexNet the top-1 accuracy is 0.9564 as illustrated in the following fragment of [logfile](files/log/logfile_run_alexnet_host.txt):
```
... ] Test iter: 50/50, loss = 0.129312
... ] Test iter: 50/50, top-1 = 0.98
... ] Test Results:
... ] Loss: 0.253436
... ] accuracy = 0.9564
... ] loss = 0.253436 (* 1 = 0.253436 loss)
... ] top-1 = 0.9564
... ] Test Done!
```


Now cross-compile the hybrid (CPU + DPU) application with [crosscompile_alexnet.sh](files/deploy/alexnetBNnoLRN/zcu102/crosscompile_alexnet.sh) shell script.
   - In case of Vitis AI 1.1 run the following:
   ```bash
   unset LD_LIBRARY_PATH    #required by petalinux for Vitis >= 1.1
   sh /opt/petalinux/2019.2/environment-setup-aarch64-xilinx-linux # set petalinux environment of Vitis AI 1.1
   cd <WRK_DIR>/tutorials/VAI-Caffe-ML-CATSvsDOGS/files
   cd deploy/zcu102
   source crosscompile_alexnet.sh
   cd ..
   tar -cvf zcu102.tar ./zcu102 # to be copied on the SD card
   ```
   - In case of Vitis AI 1.0 run the following:
   ```bash
   cd <WRK_DIR> # you are now in Vitis_AI subfolder
   ./runtime/docker_run.sh xilinx/vitis-ai:runtime-1.0.0-cpu
   cd <WRK_DIR>/tutorials/VAI-Caffe-ML-CATSvsDOGS/files
   cd deploy/zcu102
   source crosscompile_alexnet.sh
   cd ..
   tar -cvf zcu102.tar ./zcu102 # to be copied on the SD card
   ```

Assuming you have transferred the ``zcu102.tar`` archive from the host to the target board with ``scp`` utility, now you can run the hybrid application (from the _target_ board side). See below the command line example for the AlexNet case:
    ```
    tar -xvf zcu102.tar
    cd zcu102/pruned
    source run_all_pruned.sh
    ```

## 10.3 Performance of the Quantized Pruned AlexNet on ZCU102

At the end of the pruning and quantization procedures, when the AI Inference DPU runs the AlexNet CNN on the ZCU102, the following performance was reported:
- 435 fps with six threads,
- 95% average top-1 accuracy,

as shown in the [logfile_run_pruned_target.txt](files/log/logfile_run_pruned_target.txt) log file.

## 11. AlexNet Summary  ##

## 11.1 Summary of the AlexNet Design Flow

Congratulations! You have completed the Caffe training of AlexNet using the Dogs vs. Cats database. You then applied Xilinx Vitis AI tools to quantize the original CNN to get a baseline reference. You also have seen how to prune (and quantize again) the new optimized CNN: the CNN is optimized in the sense that it has fewer output channels than its baseline version.

By running the ``.elf`` files of either the baseline or the pruned CNNs on the ZCU102 target board, you have measured a frame rate improvement from 154 fps (baseline) to 389 fps (pruned) with an average top-1 accuracy of 92% and 95%  respectively for the baseline and pruned CNNs.

See below for a summary of the most important steps you have completed to arrive at this point.

1. You have downloaded the Dogs vs. Cats dataset and organized it in the proper way to train AlexNet with Caffe, making predictions on 1000 testing images to get the average top-1 accuracy: 20000 and 4000 images respectively in the LMDB training and validation databases, plus 200 images (from the training dataset) to compose the calibration dataset to be used during the quantization process. You have applied the following Python scripts:
  - [1_write_cats-vs-dogs_images.py](files/caffe/code/1_write_cats-vs-dogs_images.py)
  - [2a_compute_mean.py](files/caffe/code/2a_compute_mean.py)
  - [2b_create_lmdb.py](files/caffe/code/2b_create_lmdb.py)

2. You have trained the CNN with 20000 iterations by applying the [4_training.py](files/caffe/code/4_training.py) Python script  and the  [train_val_2_alexnetBNnoLRN.prototxt](files/caffe/models/alexnetBNnoLRN/m2/train_val_2_alexnetBNnoLRN.prototxt) and [solver_2_alexnetBNnoLRN.prototxt](files/caffe/models/alexnetBNnoLRN/m2/solver_2_alexnetBNnoLRN.prototxt) input ``.prototxt`` files. You have also plotted the learning curves of the training process with the [5_plot_learning_curve.py](files/caffe/code/5_plot_learning_curve.py) Python script.

3. The floating point weights file ``float.caffemodel`` generated in the previous step together with the CNN deploy model ([deploy_2_alexnetBNnoLRN.prototxt](files/caffe/models/alexnetBNnoLRN/m2/deploy_2_alexnetBNnoLRN.prototxt)) have then been used to make predictions on the 1000 testing images. You have achieved an average top-1 accuracy of ~93%. All the above steps can be run with the single shell script [caffe_flow_AlexNet.sh](files/caffe/caffe_flow_AlexNet.sh).

4. You have then quantized this baseline CNN with  vitis AI quantizer and compiler tools on the host PC by applying the [vaiq_alexnetBNnoLRN.sh](files/deploy/alexnetBNnoLRN/quantiz/vaiq_alexnetBNnoLRN.sh) and [vaic_alexnetBNnoLRN.sh](files/deploy/alexnetBNnoLRN/quantiz/vaic_alexnetBNnoLRN.sh) shell scripts to the files generated in step 3, ``float.caffemodel`` and [q_float.prototxt](files/caffe/models/alexnetBNnoLRN/m2/q_train_val_2_alexnetBNnoLRN.prototxt), where the latter file is the [train_val_2_alexnetBNnoLRN.prototxt](files/caffe/models/alexnetBNnoLRN/m2/train_val_2_alexnetBNnoLRN.prototxt) edited to replace the LMDB training database with the calibration images and to add in the bottom the top-1 accuracy layer.

5. You have copied to the target ZCU102 board the [fps_main.cc](files/deploy/alexnetBNnoLRN/zcu102/baseline/src/fps_main.cc) application file and the ``.elf`` DPU kernel generated by Vitis AI compiler in the previous step. Then, you have compiled the hybrid application, with the ARM CPU executing the SoftMax and top-2 software routines while the DPU hardware accelerator executes the FC, CONV, ReLU, and BN layers of the CNN.

6. You have measured an effective frame rate of 156fps and an average top-1 accuracy of 94% (this last one using the [check_dpu_runtime_accuracy.py](files/deploy/alexnetBNnoLRN/zcu102/baseline/check_dpu_runtime_accuracy.py) Python script). This ends the implementation flow of the baseline ``alexnetBNnoLRN`` from the concept to the run-time execution on the ZCU102 target board.

7. You have seen how the CNN can be optimized by applying pruning to reduce the number of output channels, and consequently the overall number of operations the DPU has to complete. You have applied the iterative flow described in the [pruning_flow.sh](files/pruning/alexnetBNnoLRN/pruning_flow.sh) shell script together with seven variances of the same [config.prototxt](files/pruning/alexnetBNnoLRN/config.prototxt) configuration file to the following input files:
  - [solver.prototxt](files/pruning/alexnetBNnoLRN/solver.prototxt): The same solver [solver_2_alexnetBNnoLRN.prototxt](files/caffe/models/alexnetBNnoLRN/m2/solver_2_alexnetBNnoLRN.prototxt) adopted during the training process, with edited pathnames and 12000 iterations instead of 20000.
  - [train_val.prototxt](files/pruning/alexnetBNnoLRN/train_val.prototxt): The same description file adopted during the training process, renamed from [train_val_2_alexnetBNnoLRN.prototxt](files/caffe/models/alexnetBNnoLRN/m2/train_val_2_alexnetBNnoLRN.prototxt) with some editing to add top-1 accuracy layer at its end.
  - ``float.caffemodel``, the same weights file of your original ``.caffemodel`` (``snapshot_3_alexnetBNnoLRN__iter_20000.caffemodel``).

8. The pruning process generated the following output files, which then became inputs to the next and final quantization step:
     - ``transformed.caffemodel``: A ``.caffemodel`` binary file much smaller in size than the starting ``float.caffemodel``.
     - [final.prototxt](files/pruning/alexnetBNnoLRN/regular_rate_0.7/final.prototxt): A ``.prototxt`` file detailing how many channels every layer has after pruning.

9. You have edited the [final.prototxt](files/pruning/alexnetBNnoLRN/regular_rate_0.7/final.prototxt) file to replace the LMDB training database with the calibration images, adding the top-1 accuracy layers in the bottom to get the new [q_final.prototxt](files/deploy/alexnetBNnoLRN/pruned/q_final.prototxt) file. You have applied the Vitis AI quantizer and compiler tools on the _host_ PC, by applying the [vaiq_pruned_alexnetBNnoLRN.sh](files/deploy/alexnetBNnoLRN/pruned/vaiq_pruned_alexnetBNnoLRN.sh) and [vaic_pruned_alexnetBNnoLRN.sh](files/deploy/alexnetBNnoLRN/pruned/vaic_pruned_alexnetBNnoLRN.sh) shell scripts to the  [q_final.prototxt](files/deploy/alexnetBNnoLRN/pruned/q_final.prototxt) and ``transformed.caffemodel`` files.

10. As in step 5, you have moved to the _target_ ZCU102 board and compiled the hybrid application there. You have measured a frame rate of 435 fps with an average top-1 accuracy of 95%.




# References

- [BVLC Caffe GitHub page](https://github.com/BVLC/caffe)

- [Designing Deep Learning Neural Networks using Caffe](https://www.researchgate.net/publication/308895193_Designing_Deep_Learning_Neural_Networks_using_Caffe)

- [Deep learning tutorial on Caffe technology : basic commands, Python and C++ code](https://christopher5106.github.io/deep/learning/2015/09/04/Deep-learning-tutorial-on-Caffe-Technology.html)

- [IBM Getting Started with Caffe](https://www.ibm.com/support/knowledgecenter/en/SS5SF7_1.5.3/navigation/pai_getstarted_caffe.html)

- [Xilinx Vitis AI Caffe Examples](https://www.xilinx.com/html_docs/vitis_ai/1_0/vdy1575039492108.html)

- [Getting Started with Caffe (PDF)](http://on-demand.gputechconf.com/gtc/2015/webinar/deep-learning-course/getting-started-with-caffe.pdf)

- [A Practical Introduction to Deep Learning with Caffe and Python](http://adilmoujahid.com/posts/2016/06/introduction-deep-learning-python-caffe/)

- [Using Caffe with your own dataset](https://medium.com/@alexrachnog/using-caffe-with-your-own-dataset-b0ade5d71233)

- [Caffe Tutorial PDF Slides](http://graphics.cs.cmu.edu/courses/16-824/2016_spring/slides/caffe_tutorial.pdf)
