3.4 CIFAR10 Classification using Vitis AI and TensorFlow
-----------------------

This module introduces the Vitis AI TensorFlow design process and illustrates how to go from a Python description of the network model to running a compiled model on the Xilinx&reg; DPU accelerator. Please visit the following link for more [Design Tutorials](https://gitenterprise.xilinx.com/swm/Vitis-In-Depth-Tutorial/tree/v1.3/Machine_Learning/Design_Tutorials)

The application code in this example design is written in Python and uses the Unified APIs and VART runtime that were introduced in Vitis&trade; AI v1.3.

We will run the following steps:

1. Training and evaluation of a customized version of the DenseNet network using TensorFlow Keras.
2. Conversion of the HDF5 format Keras checkpoint into a TensorFlow compatible checkpoint.
3. Removal of the training nodes and conversion of the graph variables to constants (..often referred to as 'freezing the graph').
4. Evaluation of the frozen model using the CIFAR-10 test dataset.
5. Quantization of the floating-point frozen model.
6. Evaluation of the quantized model using the CIFAR-10 test dataset.
7. Compilation of the quantized model to create the ```.xmodel``` file ready for execution on the DPU accelerator IP.
8. Download and execution of the application on an evaluation board.


## The CIFAR-10 Dataset

CIFAR-10 is a publicly available dataset that contains a total of 60k RGB images each of which are 32pixels x 32pixels x8bits per color channel. The small image size of 32 x 32 means that it is not very useful for real-world applications, but the CIFAR-10 dataset makes a good starting point for studying machine learning. The complete dataset of 60k images is normally divided into 50k images for training and 10k images for testing/validation.

There are a total of 10 mutually exclusive classes (or labels):


| Class index | Class name |
| :---------: | ---------- |
|      0      | airplane   |
|      1      | automobile |
|      2      | bird       |
|      3      | cat        |
|      4      | deer       |
|      5      | dog        |
|      6      | frog       |
|      7      | horse      |
|      8      | ship       |
|      9      | truck      |


## The DenseNetX CNN

The DenseNet architecture was first proposed by Huang et al in their paper ['Densely Connected Convolutional Networks'](https://arxiv.org/pdf/1608.06993.pdf) The code provided in the ``DenseNetX.py`` python script is based on version 5 of the paper and includes the Bottleneck layers and Compression factor, so strictly speaking is a DenseNet-BC implementation. It also includes some minor modifications to make it compatible with the Vitis AI quantizer and compiler. In particular, the order of the BatchNorm, RelU activation and Convolution layers has been altered from BN->ReLU->Conv to Conv->BN->ReLU.

The authors of the original paper used a Stochastic Gradient Descent (SGD) optimizer whereas the training script in this example (``train.py``) uses RMSProp - the code required for SGD optimization is also provided, just uncomment it and then comment out the RMSProp optimizer, like this:

```python
    '''
    Optimizer
    RMSprop used in this example.
    SGD  with Nesterov momentum was used in original paper
    '''
    opt = SGD(lr=learnrate, momentum=0.9, nesterov=True)
    #opt = RMSprop(lr=learnrate)
```


The code in ``DenseNetX.py`` is set for the CIFAR10 dataset, so the first convolutional layer uses a 3x3 kernel with a stride length of 1 and the first max pooling layer is ommitted:


```python
    # Use this for CIFAR-10, CIFAR-100
    # first convolutional layer + BN + ReLU (Imagenet style)
    net = Conv2D((2*k),3,strides=1,use_bias=False,kernel_initializer='he_uniform',kernel_regularizer=l2(weight_decay),padding='same')(input_layer)
    net = BatchNormalization()(net)
    net = Activation('relu')(net)
```


In the original DenseNet paper, the first convolutional layer was set for a 7x7 kernel, stride lengths of 2 and a max pooling layer was included for the ImageNet dataset:


```python
    # Use this for IMAGENET
    # first convolutional layer + BN + ReLU
    net = Conv2D((2*k), 7, strides=2, use_bias=False)(input_layer)
    net = BatchNormalization()(net)
    net = Activation('relu')(net)

    # max pooling layer
    net = MaxPooling2D(3, 2)(net)
```

**Note**: You must adjust this code according to the size of your input data.

A picture of the complete network can be found at ./img/model.png.


### DenseNet-121, 169, 201 and 264 Naming

The authors of the original paper labelled the architectures they created for ImageNet as DenseNet-121, 169, 201 and 264.  (Reference 1, Table 1). It can be useful to understand how they arrived at that nomenclature.

They used the parameter 'L' to indicate the depth of their networks but counted only the convolutional and fully-connected layers, they did not include the pooling, batch norm or activation layers.

For example, DenseNet-121 is made up of:

+ the first convolutional layer = 1
+ 1 dense block of 6 1x1 and 3x3 convolutional layers = 6 x 2 = 12.
+ 1 transition layer with 1 convolutional layer = 1.
+ 1 dense block of 12 1x1 and 3x3 convolutional layers = 12 x 2 = 24.
+ 1 transition layer with 1 convolutional layer = 1.
+ 1 dense block of 24 1x1 and 3x3 convolutional layers = 24 x 2 = 48.
+ 1 transition layer with 1 convolutional layer = 1.
+ 1 dense block of 16 1x1 and 3x3 convolutional layers = 16 x 2 = 32.
+ 1 classification layer which contains 1 fully-connected layer = 1


The sum of 1 + 12 + 1 + 24 + 1 + 48 + 1 + 32 + 1 = 121


The DenseNetX model provided in this tutorial has 3 dense blocks each of which have 16 sets of 1x1 and 3x3 convolutions. The 3 dense blocks are created by the `convlayers` argument which must be provided as a Python list:

```python
 model = densenetx(input_shape=(input_height,input_width,input_chan),classes=10,theta=0.5,drop_rate=0.3,k=12,convlayers=[16,16,16])
```


## Implementing the Design

This section will lead you through the steps necessary to run the design in hardware.

### Preparing the Host Machine and Target Board

The host machine has several requirements that need to be met before we begin. You will need:

  + An Ubuntu 16.04 or 18.04 x86 host machine with internet access to download files.

  + Optionally, a GPU card suitable for training (a trained checkpoint is provided for those who wish to skip the training step).

  + The environment is supposed to be ready at this step. If not, please follow the setup instructions provided in [Module_2](../Module_2) and in [Module_3](../Module_3).

### Downloading the Design and Setting up the Workspace

This repository should be downloaded to the host machine as a zip file and then unzipped to a folder, or cloned using the ``git clone`` command from a terminal.

Open a linux terminal, cd into the repository folder then into the 'files' folder. Start the Vitis AI docker - if you have a GPU in the host system, it is recommended that you use the GPU version of the docker container. If you intend running the model training, you will definitely need the GPU docker container. If you are going to skip the training phase, then the CPU docker container will be sufficient.

As part of the [Setting up the host](https://www.xilinx.com/html_docs/vitis_ai/1_2/jck1570690043273.html) procedure, you will have cloned or downloaded The Vitis AI repository to the host machine. In the Vitis AI folder of that repo there is a shell script called docker_run.sh that will launch the chosen docker container. Open a terminal on the host machine and cd into the enter the following commands (note: start *either* the GPU or the CPU docker container, but not both):


```shell
# navigate to densenet tutorial folder
cd <path_to_densenet_design>/DenseNetX

# to start GPU docker
<path_to_Vitis-AI>/docker_run.sh xilinx/vitis-ai-gpu:latest

# ..or to start CPU docker
<path_to_Vitis-AI>/docker_run.sh xilinx/vitis-ai-cpu:latest
```

The docker container will start and you should see something like this in the terminal:


```shell
==========================================

__      ___ _   _                   _____
\ \    / (_) | (_)            /\   |_   _|
 \ \  / / _| |_ _ ___ ______ /  \    | |
  \ \/ / | | __| / __|______/ /\ \   | |
   \  /  | | |_| \__ \     / ____ \ _| |_
    \/   |_|\__|_|___/    /_/    \_\_____|

==========================================

Docker Image Version:  latest
Build Date: 2020-12-25
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

Now run the environment setup script:  `source ./0_setenv.sh`

This will set up all the environment variables (..mainly pointers to folder and files..) most of which you can edit as required. It will also create the folders for the logs and the trained keras checkpoint.

The 0_setenv.sh script also activates the 'vitis-ai-tensorflow' TensorFlow conda environment, so you should now see that the terminal prompt looks like this:


```shell
(vitis-ai-tensorflow) Vitis-AI /workspace$
```

### Step 1: Training Your Model

:pushpin: Training takes a considerable time, between 8-12 hours depending on the GPU. You can either:

+ Reduce the number of epochs by editing the ``export EPOCHS=160`` line in the 0_setenv.sh shell script. Obviously, less epochs of training will have a negative impact on the final accuracy.
+ Skip the training phase altogether and use the pretrained Keras checkpoint available in keras_model.zip. In this case, you can copy the k_model.h5 file inside this zip archive to the ./files/build/keras_model folder. You can then skip the remaining parts of Step 1 and go directly to Step 2.


To run step 1: ``source ./1_train.sh``

The training process is defined by the train.py Python script. First, the CIFAR10 dataset is downloaded and pre-processed by the datadownload.py script which is called by train.py. All images undergo simple pre-processing before being used for training, evaluation and quantization calibration. The images are normalized to bring all pixel values into the range 0 to 1.0 by dividing them by 255.

The images will also undergo augmentation in the form of random rotation, horizontal flipping (i.e. around the *vertical* axis), shifting up and down, shearing and zooming:


```python
data_augment = ImageDataGenerator(rotation_range=10,
                                  horizontal_flip=True,
                                  height_shift_range=0.1,
                                  width_shift_range=0.1,
                                  shear_range=0.1,
                                  zoom_range=0.1)

train_generator = data_augment.flow(x=x_train,
                                    y=y_train,
                                    batch_size=batchsize,
                                    shuffle=True)
```

At the end of training, the accuracy of the trained model will be calculated using the .evaluate method:


```python
scores = model.evaluate(x=x_test, y=y_test, batch_size=50, verbose=0)
print ('Evaluation Loss    : ', scores[0])
print ('Evaluation Accuracy: ', scores[1])
```


After training has finished, the trained Keras checkpoint will be found in the ./files/build/keras_model folder as an HDF5 file called k_model.h5.

*Note: Any error messages relating to CUPTI can be ignored.*


### Step 2: Converting the Keras HDF5 Checkpoint to a TensorFlow Frozen Graph

To run step 2: ``source ./2_keras2tf.sh``

The Vitis AI tools cannot operate directly on Keras checkpoints and require a TensorFlow compatible frozen graph as the input format. The 2_keras2tf.sh shell script will create the frozen graph in two steps:

1. The HDF5 file is converted to a TensorFlow checkpoint.
2. The TensorFlow checkpoint is converted to a 'frozen graph' in binary protobuf format.

The output .pb file is generally known as a 'frozen graph' since all variables are converted into constants and graph nodes associated with training such as the optimizer and loss functions are stripped out.

After this step is completed, there should be a protobuf file called 'frozen_graph.pb' in the ./files/build/freeze folder.


### Step 3: Evaluating the Frozen Graph

To run step 3: ``source ./3_eval_frozen.sh``

This is an optional step as the frozen graph is still in floating-point format and should give almost identical accuracy results as the evaluation done during the training phase (step 1). All 10k images of CIFAR10 test images are passed through the frozen model and the accuracy is calculated.


### Step 4: Quantizing the Frozen Graph

To run step 4: ``source ./4_quant.sh``

The DPU accelerator IP executes all calculations in 8bit integer format, so we must quantize our floating-point frozen graph. This is done by the Vitis AI tools, in particular by the 'vai_q_tensorflow quantize' command. This command can be seen in the 4_quant.sh script and has several arguments that we must provide values for:


| Argument               | Description                                                    |
| ---------------------- | -------------------------------------------------------------- |
| `--input_frozen_graph` | path and name of the input  .pb frozen graph                   |
| `--input_fn`           | Name of input function used in calibration pre-processing      |
| `--output_dir`         | Name of the output folder where the quantized models are saved |
| `--input_nodes`        | Name(s) of the input nodes                                     |
| `--output_nodes`       | Name(s) of the output nodes                                    |
| `--input_shapes`       | Shape(s) of the input nodes                                    |
| `--calib_iter`         | Number of calibration iterations                               |


*Note: Any error messages relating to ./bin/ptxas can be ignored.*

Most of the arguments are self-explanatory but special mention needs to be made of --input_fn and --calib_iter.

We require a sample set of data to calibrate the quantization process. This data will be passed through the model in one forward pass and so must be processed in exactly the same way as the data is pre-processed in training...the function pointed to be the --input_fn argument will need to contain all of the pre-processing steps.

The images for the calibration are created by the tf_gen_images.py python script and then stored in the ./files/build/quantize/images folder along with a text file which lists those images. This folder will be deleted after quantization is finished to reduce occupied disk space.

The image_input_fn.py Python script contains a single function called calib_input (..hence we set --input_fn to image_input_fn.calib_input in the 4_quant.sh shell script..) which opens the images with OpenCV, flips them to RGB from BGR as the model was trained on RGB images and then normalizes them to have all pixels in the range 0 to 1.0, exactly as was done in training and evaluation.

The number of images generated for use in calibration is set by the CALIB_IMAGES environment variable in the 0_setenv.sh script. Care should be taken that the number of calibration iterations (--calib_iter) multiplied by the calibration batch size (set in the image_input_fn.py script) does not exceed the total number of available images (CALIB_IMAGES).

Once quantization has completed, we will have the quantized deployment model (deploy_model.pb) and the evaluation model (quantize_eval_model.pb) in the ./files/build/quantize folder.


### Step 5: Evaluating the Quantized Model

To run step 5: ``source ./5_eval_quant.sh``

This is an optional, but *highly* recommended step. The conversion from a floating-point model where the values can have a very wide dynamic range to an 8bit model where values can only have one of 256 values almost inevitably leads to a small loss of accuracy. We use the quantized evaluation model to see exactly how much impact the quantization has had.

The exact same Python script, eval_graph.py, that was used to evaluate the frozen graph is used to evaluate the quantized model.


### Step 6: Compiling the Quantized Model

To run step 6: ``source ./6_compile.sh``

The DPU IP is a soft-core IP whose only function is to accelerate the execution of convolutional neural networks. It is a co-processor which has its own instruction set - those instructions are passed to the DPU in Xmodel file format.

The Vitis AI compiler will convert, and optimize where possible, the quantized deployment model to a set of micro-instructions and then output them in an Xmodel file.

The generated instructions are specific to the particular configuration of the DPU. The DPU's parameters are contained in a arch.json file which needs to be created for each target board - see the [Vitis AI User Guide](https://www.xilinx.com/html_docs/vitis_ai/1_3/zmw1606771874842.html) for details.

In the specific case of the ZCU104 and the prepared SDcard image file that we used back in the 'Preparing the host machine and target board' section, the arch.json file is included in the docker container and its location is passed to the vai_c_tensorflow command via the --arch argument.

Once compile is complete, the Xmodel file will be stored in the ./build/compile folder.


### Step 7: Running the Application on the Board

To run step 7: ``source ./7_make_target.sh``


This final step will copy all the required files for running on the board into the ./build/target folder. The entire target folder will be copied to the ZCU104 SDcard. The 7_make_target.sh script also creates 10000 images from the CIFAR10 test set - the application code will preprocess and classify these images.

Copy it to the /home/root folder of the flashed SD card, this can be done with ```scp``` command.

  + If the ZCU104 is connected to the same network as the host machine, the target folder can be copied using scp.

  + The command will be something like ``scp -r ./target root@192.168.1.227:~/``  assuming that the ZCU104 IP address is 192.168.1.227 - adjust this and the path to the target folder as appropriate for your system.

  + If the password is asked for, insert 'root'.


With the target folder copied to the SD Card and the ZCU104 booted, you can issue the command for launching the application - note that this done on the ZCU104 board, not the host machine, so it requires a connection to the ZCU104 such as a serial connection to the UART or an SSH connection via Ethernet.

The application can be started by navigating into the target folder and then issuing the command ``python3 app_mt.py``. The application will start and after a few seconds will show the throughput in frames/sec.

```shell
root@xilinx-zcu104-2020_2:~/target# python3 app_mt.py
Command line options:
 --image_dir :  images
 --threads   :  1
 --model     :  model_dir/densenetx.xmodel
Pre-processing 10000 images...
Starting 1 threads...
FPS=453.83, total frames = 10000 , time=22.0348 seconds
output buffer length: 10000
Correct: 9145 Wrong: 855 Accuracy: 0.9145
```

For better throughput, the number of threads can be increased like this:

```shell
root@xilinx-zcu104-2020_2:~/target# python3 app_mt.py -t 5
Command line options:
 --image_dir :  images
 --threads   :  5
 --model     :  model_dir/densenetx.xmodel
Pre-processing 10000 images...
Starting 5 threads...
FPS=792.44, total frames = 10000 , time=12.6192 seconds
output buffer length: 10000
Correct: 9145 Wrong: 855 Accuracy: 0.9145
```


## Accuracy & Performance Results

The floating-point post-training and frozen graph evaluations can be compared to the INT8 post-quantization model and actual results obtained by the hardware model running on the ZCU104 board:


| Post-training (Float) | Frozen Graph (Float) | Quantized Model (INT8) | Hardware model (INT8) |
| :-------------------: | :------------------: | :--------------------: | :-------------------: |
|        92.94%         |        93.03%        |         92.66%         |       91.45  %        |


The approximate throughput (in frames/sec) for various batch sizes is shown below:


| Threads | Throughput (fps) |
| :-----: | :--------------: |
|    1    |      478.47      |
|    2    |      708.57      |
|    3    |      790.29      |
|    4    |      820.57      |
|    5    |      829.56      |

Now, you could jump to [Module_5](../Module_5) for a more practical use case.

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
