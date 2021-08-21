# Work with Networks in Vitis AI Docker

This module introduces how to use networks and scripts from Vitis AI model zoo to do following tasks in Vitis AI docker:

- prepare dataset
- evaluate pretrained float model and quantized model, including test images and test accuracy
- retrain the model
- use Vitis AI toolchain to generate files for deployment

We will use Caffe RefineDet network as example in this tutorial. More examples with other framework will be added in the future.

## Prerequisite

- Internet connection is avaliable
- Vitis AI docker succesfully starts
- Conda environment 'vitis-ai-caffe' is enabled by `conda activate vitis-ai-caffe`
- ZCU104 board is up and running with Vitis AI release image

## Download Network Package from Vitis AI Model Zoo

[Vitis AI model zoo](https://github.com/Xilinx/Vitis-AI/tree/master/AI-Model-Zoo) provides a varity of networks pre-trained in Caffe/TensorFlow/DarkNet, covering different applications including but not limited to image classification, ADAS/AD, video surveillance and etc. Several mainstream networks are further optimized using [Xilinx Optmizer](https://www.xilinx.com/member/ai_optimizer.html) to achieve much higher performance.

In each network packages, float models and quantized models are provided together with training and evaluation scripts. We use Caffe [RefineDet](https://arxiv.org/abs/1711.06897) (No. 12 model in the Model Zoo) to build people detection application.

RefineDet is an one-stage objection detection network that could achieve better accuracy than two-stage networks before
publication but still maintains comparable speed performance of one-stage networks.

RefineDet can be seen as an improved version of SSD with addition of anchor refinement module (ARM), object detection m
and transfer connection block (TCB) to improve accuracy. 

The detailed introduction about network itself is beyond the scope of this module, please refer to the original paper. 

### Get RefineDet Model Package
After the git clone the whole repository, go to the module 5 folder (Vitis-In-Depth-Tutorial/Machine_Learning_Tutorial/Section_3-Basic/Module_5) and run `sh get_network.sh` to download RefineDet network package, which is trained with people subset selected from COCO2014 and optimized to reduce 96% of the computation complexity (GOP).

If no error occurs, your workspace in module 5 folder will be like below
```
Module_5/
|-- cf_refinedet_coco_480_360_0.96_5.08G
|   |-- codeco_480_360_0.96_5.08G
|   |-- code_bak
|   |-- data
|   |-- float
|   |-- labelmap.prototxt
|   |-- quantizedototxt
|   `-- readme.md
|-- cf_refinedet_coco_baseline_125G
|   |-- refinedet_baseline.prototxtco_baseline_125G
|   `-- solver.prototxtaseline.prototxt
|-- get_network.sh
|-- network_training.mdg.md
|-- README.mdts
|-- reference_scripts
|   `-- code
`-- res_output.jpg

```

Go to folder `cf_refinedet_coco_480_360_0.96_5.08G` and follow the steps described below to start the real works.

```
cf_refinedet_coco_480_360_0.96_5.08G/
├── code
│   ├── gen_data
│   │   ├── convert_coco2voc_like.py
│   │   ├── create_data.py
│   │   └── get_dataset.sh
│   ├── test
│   │   ├── demo
│   │   │   ├── COCO_val2014_000000209468.jpg
│   │   │   ├── COCO_val2014_000000425226.jpg
│   │   │   └── res_output.jpg
│   │   ├── demo.py
│   │   ├── evaluation_py2.py
│   │   └── test.py
│   └── train
│       ├── convert_coco2voc_like.py
│       ├── create_data.py
│       ├── solver.prototxt
│       └── train.sh
├── data
├── float
│   ├── test.prototxt
│   ├── trainval.caffemodel
│   └── trainval.prototxt
├── labelmap.prototxt
├── quantized
│   ├── deploy.caffemodel
│   ├── deploy.prototxt
│   ├── quantize_test.prototxt
│   ├── quantize_train_test.caffemodel
│   └── quantize_train_test.prototxt
└── readme.md
```

## Modify Scripts to be Python3 Compatible

Vitis AI docker is built with Python3 environment while Caffe is developed originally with Python2. Hence Minor modificaiton in several scripts are needed to be Python3 compatible. Ready-to-use scripts are prepared and located in folder `reference scripts`, which will be copied into code folder for simplicity when running "get_network.sh".

Detailed modifications are as below

- create_annoset.py

  change `print` to `print()`

  change the command and path to call Caffe prebuilt binaries

- /code/gen_data/create_data.py

  change `print` to `print()`

  change `import commands` to `import subprocess`

  change `commands.getoutput()` to `subprocess.getoutput()`

  change the path to the script `create_annoset.py` in line 122 according to your environment

 - /code/test/test.py

   change `print` to `print()`

   change `xrange` to `range`

   comment line 91 to let script use python env in conda

   change GPU ID in line 171 if nessaray, default value is 3

## Prepare Customized COCO Dataset

COCO is a large-scale object detection, segmentation, and captioning dataset including 330K images (>200K labeled) in 91 object types, which are used widely in ML/AI researches. For detailed information about the dataset, please refer to [COCO official website](https://cocodataset.org/#home).

If you would like to retrain the network, evaluate mAP before directly deploying the network on board, the COCO dataset used by this network needs to be prepared with necessary process.

Related scripts are provided in folder `cf_refinedet_coco_480_360_0.96_5.08G/code/gen_data`.

```
gen_data/
|-- convert_coco2voc_like.py
|-- create_annoset.py
|-- create_data.py
`-- get_dataset.sh
```

Use `sh get_dataset.sh` to download and generate the dataset which only contains **one class** *person* seleted from COCO2014. If you already have COCO2014 in the hard drive, please comment the download code in `get_dataset.sh` and put COCO2014 files into folder `cf_refinedet_coco_480_360_0.96_5.08G/data` for simplicity, in which the expected file structure is shown as below

```
coco2014/
      |->train2014/
      |->val2014/
      |->instances_train2014.json
      |->instances_val2014.json
```

If the script finishes without error, the generated LMBD files and images will have following structure
```
data/
|-- coco2014
|   |-- Annotations
|   |-- Images
|   |-- instances_train2014.json
|   |-- instances_val2014.json
|   |-- train2014.txt
|   |-- train_image2anno.txt
|   |-- val2014.txt
|   `-- val_image2anno.txt
|-- coco2014_lmdb
|   |-- train2014_lmdb
|   `-- val2014_lmdb
|-- link_480_360
|   |-- train2014_lmdb -> ../../data/coco2014_lmdb/train2014_lmdb
|   `-- val2014_lmdb -> ../../data/coco2014_lmdb/val2014_lmdb
`-- test_name_size.txt
```
If the script finishes with out error, the md5sum values of train and val mdb files are as below:

|                 File Name                  |           Md5sum Value           |
| :----------------------------------------: | :------------------------------: |
| data/coco2014_lmdb/train2014_lmdb/data.mdb | 39891c36467f22498a98cbd431e43657 |
|  data/coco2014_lmdb/val2014_lmdb/data.mdb  | e99d3ad2ce3d5ffc864fa8d41b01d4b0 |




## From Official RefineDet to Model Zoo Version

The model zoo package provides Xilinx version RefineDet with modifications on model struture to make it compatible with Vitis AI, and also network pruning optimization by Vitis AI optimizer.

The **[Journey from Original RefineDet](/Machine_Learning/Introduction/03-Basic/Module_5/network_training.md)** documents the steps in detail of how to start from original model and arrive at the baseline version (not optimized, 125GOPs) of that in the model zoo package (optimized, 5GOPs model used in this example, 25GOPs and 10GOPs models also proivded in the model zoo). The pretrained baseline model is provided here and could be used in following parts.

In advanced section, there will be a seperate module to documents the workflow of AI optimizer and the steps to prune the model from baseline to optimized version.

## Work with Vitis AI Toolchain

Vitis AI toolchain is used to quantize float 32bit network model into INT8 model with optimization and then compile it into instructions which run efficiently on the DPU in FPGA. The toolchain could support Caffe, DarkNet and TensorFlow framework.

Comprehensive introduction of Vitis AI toolchain can be found in [Vitis AI User Documentation
](https://www.xilinx.com/html_docs/vitis_ai/1_2/zkj1576857115470.html).Here we only use RefineDet as example to demostrate Caffe workflow. The following steps are based on network package from model zoo (cf_refinedet_coco_480_360_0.96_5.08G), but it could be easily used with baseline model with some changes on file paths in scripts, prototxt and commands.

### <span id = "tool">Evaluate Model</span>

In the network package, both float model and quantized model are provided together with evaluation script to run single images or test mAP over whole validation dataset. These scripts work with baseline model as well.

### Run single image

Go to folder `cf_refinedet_coco_480_360_0.96_5.08G/code/test` and run the network on single image with command `python demo.py`. The output image will be stored in `output/res_output.jpg`. If you would like to use different image, please modify the image path in the `demo.py`.

![Inference result](./res_output.jpg)

### Test mAP

The network package provides scripts to varify the mAP of the pretrained network (both float model and quantized model). To use the scripts, please make sure the evaluation dataset are correctly prepared and file paths in function 'parse_args' (line 54 to 85 in `test.py`)  are modified according to your environment.

By default, the script will evaluate float model (trainval.caffemodel and test.prototxt) stored under folder `cf_refinedet_coco_480_360_0.96_5.08G/float`.

Use command `python test.py` to run the evaluation which will takes around 30~60 minutes depending on your CPU/GPU capability. If no error occurs, it will display the mAP result as below:
```
evaluate 21634 images
person AP: 0.6120284761439228
mAP: 0.612028476144
```
It's also easy to evaluate quantized model in the same way, just change the prototxt file and caffemodel file in the script into `quantize_test.prototxt` and `quantize_train_test.caffemodel`. After evaluation is finished the result will be displayed.
```
evaluate 21634 images
person AP: 0.6112254774886302
mAP: 0.611225477489
```

The float baseline model could also be evaluated using the same script, just change the prototxt file and caffemodel file into `refinedet_baseline.prototxt` and `refinedet_baseline.caffemodel` in folder `cf_refinedet_coco_baseline_125G`.


## Deploy Model

### Quantize Float Model

The ready-to-use quantized models are already provided in the package. If you would like to try the quantization steps by yourself, detailed explaination could be found in [Vitis AI User Documentation
](https://www.xilinx.com/html_docs/vitis_ai/1_2/uim1570695919827.html).

For Caffe example, the `vai_q_caffe` will be used to execute quantization with following commands:

```
vai_q_caffe quantize -model trainval.prototxt \
                     -weights trainval.caffemodel \
                     -calib_iter 20 \
                     -auto_test  \
                     -test_iter 2100 \
                     -output_dir quantization \
                     -gpu 0 \
                     -ignore_layers arm_priorbox
```

Plesse note that the 'arm_priorbox' (which concats all previous priorbox layer together) needs to be ignored because it will not run on DPU and quantization will affect bounding box results. Generated quantized model will be stored in the folder spcified by `-output_dir`, which in this case is `quantization`.

```
I0527 01:37:05.329720  9006 net_test.cpp:212] Test iter: 2100/2100
I0527 01:37:05.755471  9006 net_test.cpp:259] Test Results:
I0527 01:37:05.755494  9006 net_test.cpp:260]     Test net output #0: detection_eval = 0.610996
I0527 01:37:05.757798  9006 net_test.cpp:398] Test Done!
I0527 01:37:06.175303  9006 decent_q.cpp:361] Start Deploy
I0527 01:37:06.209132  9006 decent_q.cpp:369] Deploy Done!
--------------------------------------------------
Output Quantized Train&Test Model:   "quantization/quantize_train_test.prototxt"
Output Quantized Train&Test Weights: "quantization/quantize_train_test.caffemodel"
Output Deploy Weights: "quantization/deploy.caffemodel"
Output Deploy Model:   "quantization/deploy.prototxt"
```

Using the same step and command, we could also generate quantized model for baseline model and use in next step compilation. Please make sure right models are used as you wish and output files are saved properly.

### Compile Quantized Model

To deploy quantized model on board, we use `vai_c_caffe` to generate elf file by compiling deploy.prototxt and deploy.caffemodel. Here we could use files in both folder `cf_refinedet_coco_480_360_0.96_5.08G/quantized` (provided in the network package) or `quantization` (generated by ourselves, please check where you put it).

Before compilation, in the any deploy.prototxt, please make sure all the layers after the layer block `odm_conf` are deleted as they are not needed for deployment and compilation .

Run below command in the quantized model folder to compile the model in to elf file which targeting DPU on ZCU104 released board image

```
vai_c_caffe --prototxt    deploy.prototxt \
            --caffemodel  deploy.caffemodel \
            --arch        /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU104/ZCU104.json \
            --output_dir  board_file_zcu104_5G \
            --net_name    refinedet_5G \
            --options     "{'save_kernel':''}"
```

Please note that you could also specify the path for prototxt, caffemodel and output_dir as you wish.

During compilation, the compiler will give following output which will show useful informations including kernel name, input tensor names, input tensor shapes, output tensor names and output tensor shapes. With `save_kernel` option used, these informations are saved in `refinedet_kernel.info` in `board_file_zcu104_5G`.

```
**************************************************
* VITIS_AI Compilation - Xilinx Inc.
**************************************************

Kernel topology "refinedet_5G_kernel_graph.jpg" for network "refinedet_5G"
kernel list info for network "refinedet_5G"
                               Kernel ID : Name
                                       0 : refinedet_5G

                             Kernel Name : refinedet_5G
--------------------------------------------------------------------------------
                             Kernel Type : DPUKernel
                               Code Size : 0.14MB
                              Param Size : 0.32MB
                           Workload MACs : 5084.69MOPS
                         IO Memory Space : 3.35MB
                              Mean Value : 104, 117, 123,
                      Total Tensor Count : 53
                Boundary Input Tensor(s)   (H*W*C)
                               data:0(0) : 360*480*3

               Boundary Output Tensor(s)   (H*W*C)
                            arm_loc:0(0) : 1*1*48960
                           arm_conf:0(1) : 1*1*24480
                            odm_loc:0(2) : 1*1*48960
                           odm_conf:0(3) : 1*1*24480

                        Total Node Count : 52
                           Input Node(s)   (H*W*C)
                              conv1_1(0) : 360*480*3

                          Output Node(s)   (H*W*C)
                              odm_loc(0) : 1*1*48960
                             odm_conf(0) : 1*1*24480
                              arm_loc(0) : 1*1*48960
                             arm_conf(0) : 1*1*24480

```
After compilation finished, elf file, kernel info file and kernel graph files are generated and saved.

```
board_file_zcu104_5G
|-- dpu_refinedet_5G.elf
|-- refinedet_5G_kernel.info
`-- refinedet_5G_kernel_graph.gv
```

Using the same step and command in the folder containing the deploy model for baseline model, we could also generate elf file for baseline model

```
board_file_zcu104_125G
|-- dpu_refinedet_125G.elf
|-- refinedet_125G_kernel.info
`-- refinedet_125G_kernel_graph.gv
```


## Test on Board

Before starting this part, please make sure:

- the zcu104 board is running with Vitis AI 1.1 release board image and Vitis AI Library/VART are installed correctly based on previouse modules

- host and zcu104 board are in same network. In our example, board IP (TARGET_IP) is 10.176.18.133

### Transfer Compiler Output to Board

Tranfer the compiler output `board_file_zcu104_5G` and `board_file_zcu104_125G` to board and place it under `~/overview/samples/refinedet` with scp command

`scp -r /PATH_TO/board_file_zcu104_5G root@10.176.18.133:~/overview/samples/refinedet`

`scp -r /PATH_TO/board_file_zcu104_125G root@10.176.18.133:~/overview/samples/refinedet`


### Connect to Board

`ssh root@10.176.18.133`

enter password root to connect to the board.

Go to refinedet example application folder

`cd /overview/samples/refinedet`

### Create Model File Folder

Copy model file folder example into current folder

`cp /usr/share/vitis_ai_library/models/refinedet_pruned_0_96/ refinedet_test_5G/`

`cp /usr/share/vitis_ai_library/models/refinedet_pruned_0_96/ refinedet_test_125G/`

Copy elf files generated by ourselves into folder `refinedet_test_5G` and ` refinedet_test_125G/`

`cp board_file_zcu104_5G/dpu_refinedet_5G.elf refinedet_test_5G/refinedet_5G.elf`

`cp board_file_zcu104_125G/dpu_refinedet_125G.elf refinedet_test_125G/refinedet_125G.elf`


### Run the application

- Performance test

  For example we could test performance in 60s with 4 threads with following command

  `./test_performance_refinedet refinedet_test_5G/ test_performance_refinedet.list -t 4 -s 60`

  ```
  I0513 08:32:18.886174 21870 benchmark.hpp:175] writing report to <STDOUT>
  I0513 08:32:18.951648 21870 benchmark.hpp:201] waiting for 0/60 seconds, 4 threads running
  I0513 08:32:28.951963 21870 benchmark.hpp:201] waiting for 10/60 seconds, 4 threads running
  I0513 08:32:38.952139 21870 benchmark.hpp:201] waiting for 20/60 seconds, 4 threads running
  I0513 08:32:48.952313 21870 benchmark.hpp:201] waiting for 30/60 seconds, 4 threads running
  I0513 08:32:58.952564 21870 benchmark.hpp:201] waiting for 40/60 seconds, 4 threads running
  I0513 08:33:08.953136 21870 benchmark.hpp:201] waiting for 50/60 seconds, 4 threads running
  I0513 08:33:18.953428 21870 benchmark.hpp:210] waiting for threads terminated
  FPS=235.63
  E2E_MEAN=16964.4
  DPU_MEAN=14459.7
  ```

  `./test_performance_refinedet refinedet_test_125G/ test_performance_refinedet.list -t 4 -s 60`

  ```
  I0513 08:34:38.699662  3631 benchmark.hpp:175] writing report to <STDOUT>
  I0513 08:34:38.773613  3631 benchmark.hpp:201] waiting for 0/60 seconds, 4 threads running
  I0513 08:34:48.773798  3631 benchmark.hpp:201] waiting for 10/60 seconds, 4 threads running
  I0513 08:34:58.773970  3631 benchmark.hpp:201] waiting for 20/60 seconds, 4 threads running
  I0513 08:35:08.774128  3631 benchmark.hpp:201] waiting for 30/60 seconds, 4 threads running
  I0513 08:35:18.775049  3631 benchmark.hpp:201] waiting for 40/60 seconds, 4 threads running
  I0513 08:35:28.775233  3631 benchmark.hpp:201] waiting for 50/60 seconds, 4 threads running
  I0513 08:35:38.775465  3631 benchmark.hpp:210] waiting for threads terminated
  FPS=18.2661
  E2E_MEAN=218638
  DPU_MEAN=216079
  ```

From comparison we can see huge performance improvement between baseline model and pruned model which is brought by AI optimizer. The non-linear FPS improvement ratio vs compression ratio is because of

- network efficiency on DPU decreases at some extent after network size decreases
- DPU time percentage significantly decreases in whole application time. Software processing and scheduling overhead increase will affect end-to-end FPS performance

In the other module in advanced section, we will introduce how to use AI optimizer to achieve network optimization to reduce network from 125G to 5G.

## Summary

In this module, we use a Caffe detection network (RefineDet) introduce following items:

- how to use pre-trained model from Vitis AI model zoo, generate dateset and test it with provided scripts
- how to start from original network, make necessary modification, conduct trainning and get to the version provide in the model zoo
- how to quantize and compile the model with ready-to-use Vitis AI toolchain inside docker
- how to deploy the network with Vitis AI library and run on the board

Hope you enjoy it and let's move to [next module](../Module_6/) .

<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020-2021 Xilinx</sup></p>
