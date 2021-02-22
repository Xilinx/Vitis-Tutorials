# Journey from Original RefineDet

This module will explain how to start from the original RefineDet [repo](https://github.com/sfzhang15/RefineDet) and get the model provided in the model zoo. 

## Prerequisite 

- Internet connection avaliable
- Vitis AI docker succesfully starts
- Conda environment 'vitis-ai-caffe' is enabled by `conda activate vitis-ai-caffe`

## RefineDet

RefineDet is an one-stage objection detection network that could achieve better accuracy than two-stage networks before it's publication but still maintains comparable speed performance of one-stage networks.

RefineDet can be seen as an improved version of SSD with addition of anchor refinement module (ARM), object detection module (ODM) and transfer connection block (TCB) to improve accuracy. 

The detailed introduction about network itself is beyond the scope of this module, please refer to the original paper. 

## Adopt RefineDet to Vitis AI

In order to train and deploy the network with Vitis AI toolchain and DPU, original RefineDet needs few modifications to make it compatitble. The RefineDet repository provides scripts based on its own Caffe version and also provide pretrained network with different resolution on VOC or COCO dataset.

Here we will base on Xilinx Caffe, the Xilinx version with support of common networks and our own networks, to show the steps to adopt original network onto Vitis AI. The model we used in last module from Vitis AI model zoo is originated from 320*320 resolution COCO model.

We will brake the work into following two part:

- Fundemental modification to the original network

- Customized modification to covert 320x320 model into 480x360 model


We will use COCO 320x320 model as the example in the rest of the module, please download it from links provided RefineDet author

```
https://drive.google.com/open?id=1BIVvkSQM8DarpeqOpjgPpDxUoFq9ZxKM

http://pan.baidu.com/s/1gf8J7Yr
```
Extract the file package and files will be like this

``` 
--- VGGNet
|   --- coco
|   |   --- refinedet_vgg16_320x320
|   |   |   --- coco_refinedet_vgg16_320x320_final.caffemodel
|   |   |   --- deploy.prototxt
|   |   |   --- multi_test_deploy.prototxt
|   |   |   --- single_test_deploy.prototxt
|   |   |   --- solver.prototxt
|   |   |   --- test.prototxt
|   |   |   --- train.prototxt
|   |   |   --- VGG16_COCO_320.py
```

### Fundemental Modification

In order to make any RefineDet models work with Vitis AI, following basic modifications are needed

**Item 1. Change 'Normalize' layer into 'BatchNorm' layer**

In the original network, Normalize layers are used after conv4_3 and conv5_3 which are not supported by DPU. The solution is to use BatchNorm layer as the alternative.

Please note that the BatchNorm layer will be right after the conv layer (conv4_3 or conv5_3) and before ReLu layer (in place), thus the top and bottom field should all be the name of conv layer (conv4_3 or conv5_3).

Change from `conv4_3_norm` to `conv4_3_bn`

```
layer {
  name: "conv4_3_norm"
  type: "Normalize"
  bottom: "conv4_3"
  top: "conv4_3_norm"
  norm_param {
    across_spatial: false
    scale_filler {
      type: "constant"
      value: 10
    }
    channel_shared: false
  }
}
```

```
layer {
  name: "conv4_3_bn"
  type: "BatchNorm"
  bottom: "conv4_3"
  top: "conv4_3"
  param {
    lr_mult: 1
    decay_mult: 1
  }
  param {
    lr_mult: 1
    decay_mult: 0
  }
  param {
    lr_mult: 1
    decay_mult: 0
  }
  param {
    lr_mult: 0
    decay_mult: 0
  }
  phase: TRAIN
  batch_norm_param {
    scale_filler {
      type: "constant"
      value: 1
    }
    bias_filler {
      type: "constant"
      value: 0
    }
  }
}
```
Change from `conv5_3_norm` to `conv5_3_bn`

```
layer {
  name: "conv5_3_norm"
  type: "Normalize"
  bottom: "conv5_3"
  top: "conv5_3_norm"
  norm_param {
    across_spatial: false
    scale_filler {
      type: "constant"
      value: 8
    }
    channel_shared: false
  }
}
```

```
layer {
  name: "conv5_3_bn"
  type: "BatchNorm"
  bottom: "conv5_3"
  top: "conv5_3"
  param {
    lr_mult: 1
    decay_mult: 1
  }
  param {
    lr_mult: 1
    decay_mult: 0
  }
  param {
    lr_mult: 1
    decay_mult: 0
  }
  param {
    lr_mult: 0
    decay_mult: 0
  }
  phase: TRAIN
  batch_norm_param {
    scale_filler {
      type: "constant"
      value: 1
    }
    bias_filler {
      type: "constant"
      value: 0
    }
  }
}
```

**Item 2. Merge 'train.prototxt' and 'test.prototxt' into 'trainval.prototxt' for simpilicity (Optional)**

In order to montior both loss and mAP during the training process, we could add last four layers from the test.prototxt into the end of the train.protoxt. At the same time, add AnnotatedData layer for TEST phase to get input data for testing. 

Add `multi_gpu_testing: false` into AnnotatedData layer for TEST phase.

An exmaple prototxt after merge is included here, i.e, `refinedet_baseline.prototxt`.

**Item 3. Change the layer types of 'arm_loss', 'odm_loss', 'detection_out' and 'detection_eval' to be the type name in Xilinx Caffe**

In Xilinx Caffe, in order to avoid potential confilct, the layer types using by RefineDet are renamed, which is shown in below table: 

| Layer          |  Original Layer Type | Layer Type in Xilinx Caffe  |
| ----           | ----              | ----                    |
| arm_loss       | MultiBoxLoss      | MultiBoxLossRefine      |
| odm_loss       | MultiBoxLoss      | MultiBoxLossRefine      |
| detection_out  | DetectionOutput   | DetectionOutputRefine   |
| detection_eval | DetectionEvaluate | DetectionEvaluateRefine |


**Item 4. Modify path to the necessary files** 

In the AnnotatedData layer for trainning, please change 
 
    - the path to the folder holding the train LMDB file
   
        data_param {
        source: "../cf_refinedet_coco_480_360_0.96_5.08G/data/coco2014_lmdb/train2014_lmdb/"
        batch_size: 20
        backend: LMDB
        }
 
    - the path to the labelmap file

        label_map_file: "../cf_refinedet_coco_480_360_0.96_5.08G/labelmap.prototxt"

In the AnnotatedData layer for testing, please change 
 
    - the path to the folder holding the train LMDB file

        data_param {
        source: "../cf_refinedet_coco_480_360_0.96_5.08G/data/coco2014_lmdb/val2014_lmdb/"
        batch_size: 10
        backend: LMDB
        multi_gpu_testing: false
        }


    - the path to the labelmap file

        label_map_file: "../cf_refinedet_coco_480_360_0.96_5.08G/labelmap.prototxt"


In the detection_eval layer, please change

    - the path to the name_size file 

        name_size_file: "../cf_refinedet_coco_480_360_0.96_5.08G/data/test_name_size.txt"


**Item 5. Modify solver.prototxt to change hyperparameters** 

Configure the hyperparameters according to your own environment (like GPU memory size) and your own training strategy. Some parameters used are listed here as example.

```
base_lr: 0.01
max_iter: 200000
lr_policy: "multistep"
stepvalue: 80000
stepvalue: 160000
type: "SGD"
gamma: 0.1
momentum: 0.9
weight_decay: 0.0005
snapshot: 5000
solver_mode: GPU
test_initialization: True
eval_type: "detection"
ap_version: "11point"
```


### Customized Modification

To customize the 320x320 COCO model into the 480x360 model provided in Vitis AI model zoo, apart from the fundemental changes mentioned above, extra modifications are needed. 

RefineDet uses TCB, the FPN like structure from layer `conv4_3`, `conv5_3`, `fc7` to `conv 6_2` which constructed by three 2x downsampling and corrsponding three 2x upsampling in between. This structure has the strict requriement that the kernel size (k_w and k_h) of layer `conv4_3` must be the multiple of 8 (2^3). Also, before `conv4_3` there is also three 2x downsampling during feature extraction. If directly change resolution 320x320 to 480x360 on original network, the k_w and k_h of layer `conv4_3` will be 60 and 45, which will result in error for following TCB structure. 

In order to have a 480x360 resolution network, special modificaitons have been made to make k_w and k_h of `conv4_3` to be 64 and 48, which are explained in detailed in item 2 below. 

**Item 1. Change input size from 320x320 to 480x360**

In the AnnotatedData layers, change width and height value in resize_param to be 480 and 360

```
    resize_param {
      prob: 1
      resize_mode: WARP
      height: 360
      width: 480
      interp_mode: LINEAR
      interp_mode: AREA
      interp_mode: NEAREST
      interp_mode: CUBIC
      interp_mode: LANCZOS4
    }
```

**Item 2. Change convlution layers, pooling layer parameters to support new resolution**

If we use original network parameters and structure in 480x360 resolution, the feature map size after conv4_3 will be 60x45, which cannot satisfy the requirement of following TCB structure and result in error. 

In order to arrive at the feature map size of 64x48 after conv4_3, we provide a version of implementation. Please note that this is just one way to achieve the requirment, and that the feature map size or network structure/parameter can be modified in many different ways. 


| Layer                      | Parameter   | Original     | Modified | Feature Map Size | 
| :----:                     | :----:      | :----:       | :----:   | :----:           |
| conv1_1                    | pad         | 1            | 2        | 482x362          |
| conv1_2                    | pad         | 1            | 2        | 484x364          |
| conv2_2                    | pad         | 1            | 2        | 244x184          |
| conv3_1                    | pad         | 1            | 2        | 124x94           |
| conv3_2                    | pad         | 1            | 2        | 126x96           |
| conv3_3                    | kernel_size | 3            | 2        | 64x48            |
|                            | stride      | 1            | 2        |                  |
|                            | pad_w       | 1            | 1        |                  |
|                            | pad_h       | 1            | 0        |                  |
| pool3                      |             |              | delete   |                  |
| conv4_1                    | bottom      | pool3        | conv3_3  | 64x48            |
| conv4_3_bn                 |             | -            | add      |                  |
| pool4                      |             |              | delete   |                  |
| conv5_1                    | kernel_size | 3            | 2        | 32x24            |
|                            | stride      | 1            | 2        |                  |
|                            | pad         | 1            | 0        |                  |
|                            | bottom      | pool4        | conv4_3  |                  |
| conv5_3_bn                 |             | -            | add      |                  |
| pool5                      |             |              | delete   |                  |
| fc6                        | kernel_size | 3            | 2        | 16x12            |
|                            | stride      | 1            | 2        |                  |
|                            | pad         | 3            | 0        |                  |
|                            | bottom      | pool5        | conv5_3  |                  |
| conv5_3_norm               |             |              | delete   |                  |
| TL4_1                      | bottom      | conv5_3_norm | conv5_3  |                  |
| conv4_3_norm               |             |              | delete   |                  |
| TL3_1                      | bottom      | conv4_3_norm | conv4_3  |                  |
| conv4_3_norm_mbox_loc      | bottom      | conv4_3_norm | conv4_3  |                  |
| conv4_3_norm_mbox_conf     | bottom      | conv4_3_norm | conv4_3  |                  |
| conv4_3_norm_mbox_priorbox | bottom      | conv4_3_norm | conv4_3  |                  |
| conv5_3_norm_mbox_loc      | bottom      | conv5_3_norm | conv5_3  |                  |
| conv5_3_norm_mbox_conf     | bottom      | conv5_3_norm | conv5_3  |                  |
| conv5_3_norm_mbox_priorbox | bottom      | conv5_3_norm | conv5_3  |                  |
  

All the above-mentioned modification can be found in the exmaple prototxt `refinedet_baseline.prototxt`.

**Item 3. Change class number from 81 to 2**

There five layers that relate to class number, four convlution layers and one loss layer: 

    - In convolution_param of following layers, change num_output from 243 to 6. 
      P3_mbox_conf
      P4_mbox_conf
      P5_mbox_conf
      P6_mbox_conf
        
    - In odm_loss layer, change num_classes from 81 to 2 

## Train the Modified Network

Use standard `VGG_ILSVRC_16_layers_fc_reduced.caffemodel` as the initial weights and train the modified version from scratch. Get it by running command 

```
wget http://vision.cs.unc.edu/wliu/projects/ParseNet/VGG_ILSVRC_16_layers_fc_reduced.caffemodel
```

Train the network with following command

```
caffe train -solver solver.prototxt -weights VGG_ILSVRC_16_layers_fc_reduced.caffemodel -gpu 0 2>&1 | tee train.log
```


Detection mAP during one experimental training process could be 

```
I0601 08:43:36.417382 17132 net.cpp:280] This network produces output detection_eval
I0601 08:45:59.333092 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.152085
I0601 10:21:55.472414 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.171831
I0601 11:57:21.744807 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.276751
I0601 13:32:53.489629 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.263878
I0601 15:08:20.257485 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.289203
I0601 16:43:48.145087 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.305306
I0601 18:19:49.653108 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.350838
I0601 19:55:23.386104 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.337912
I0601 21:30:57.058233 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.322393
I0601 23:06:30.205420 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.356988
I0602 00:42:07.622771 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.324532
I0602 02:17:28.071298 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.359916
I0602 03:52:34.299418 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.427493
I0602 05:28:08.372817 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.419444
I0602 07:03:43.965183 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.382097
I0602 08:39:05.628803 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.552272
I0602 10:14:16.057716 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.557035
I0602 11:50:01.469343 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.566706
I0602 13:25:44.348400 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.545264
I0602 15:01:17.255231 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.652842
I0602 16:36:42.457309 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.658513
I0602 18:18:52.569782 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.687939
I0602 21:29:55.022497 17132 solver.cpp:901]     Test net output #0: detection_eval = 0.692454
```

The model zoo has provided well trained baseline model as well as other three optimized models in different pruning ratio, which are corrsponding to model 10 to 13 in [model download page](https://github.com/Xilinx/Vitis-AI/tree/master/models/AI-Model-Zoo#model-details). All models could be used to test mAP and deploy onboard by Vitis AI toolchain.  

Continue toolchain content at the [main page](/Machine_Learning/Introduction/03-Basic/Module_5/README.md/#evaluate-model).
