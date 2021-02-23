<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>Vitis AI Tutorials</h1>
  </td>
 </tr>
 <tr>
 <td align="center"><h1>RFModulation-Recognition-with-Vitis-AI</h1>
 </td>
 </tr>
</table>


## Current Status:

- Tested with Vitis AI 1.3
- Tested in hardware on ZCU102, ZCU104, VCK190, and Alveo U50.

## Introduction

FPGAs are often used as part of the signal processing chain in wireless signal applications. In these applications, digital data is encoded, modulated, and transmitted over a wireless channel to a receiver.
Normally the receiver has some a priori knowledge of the signal so that it can be properly received and decoded.
Without this knowledge, being able to reliably detect and decode the signal can be extremely difficult.
Here we discuss using Deep Neural Networks to perform automatic modulation recognition so that the receiver may be able to detect and demodulate the signal without this explicit knowledge of the modulation type and encoding method.

## Overview

Transmitting digital information using wireless signals requires the use of a digital modulation technique, often employing the use of shift keying and complex/quadrature signal encoding. Some of the more common modulation types include Amplitude Shift-Keying (ASK), Phase Shift-Keying (PSK), Quadrature Phase Shift-Keying (QPSK), and Quadrature Amplitude Modulation (QAM). Each has different requirements around decoding and demodulation.

## Dataset

Signal impairments and distortions are expected and normal for these over-the-air signals, and are a necessary part of any dataset.
These impairments include fading, frequency offsets, and gaussian white noise **[1]**.
The overall net effect of these impairments is a lower Signal to Noise Ratio (SNR).
The lower the SNR, the harder it is to receive a signal without any errors.
Likewise, as the number of possible symbols increases, the room for detection errors becomes smaller.
Here, as in **[1]**, a dataset from DeepSig **[2]** is used, which contains both synthetic simulated channel effects as well as over-the-air recordings of 24 digital and analog modulation tyes.

Research **[1]** has shown that the accuracy of neural network estimation on this dataset varies significantly, trending higher as the overall SNR increases.
Figure 1 gives an example.

<img src="media/image1.png" style="width:3.84971in;height:2.88728in" alt="Chart, line chart Description automatically generated" />

**Figure 1: Sample accuracy vs SNR**

## Sequential Model

Various attempts at using a Deep Learning Architecture to perform modulation recognition have been attempted.
Here we use a resnet based model simialr to whats described in **[1]**.
This model contains 6 reset stacks. Each resenet stack (shown below) consists of 2 resent blocks followed by a max pooling layer.


<img src="media/image2.png" style="width:4.426in;height:4.6038in" alt="Diagram Description automatically generated" />

**Figure 2: Resnet stack used for RFMR tutorial**

Research continues into other types of networks including Recurrent Neural Networks (RNNs).

## Vitis-AI

Vitis-AI is the software tool made by Xilinx used for Machine Learning Inferrnce applications.
It supports different frameworks such as Caffe, TensorFlow, and PyTorch
An overview of the components in Vitis-AI is
shown in figure 3.

<img src="media/image3.jpeg" style="width:4.14151in;height:3.564in" />

**Figure 3: Vitis-AI overview**

This tutorial uses a Jupyter Notbook running in the Vitis-AI TensorFlow2 python environment. This will walk through downloading and analyzing the data set, defining and training the keras model, and importing into the Vitis-AI tools. In the Vitis-AI tools we quantize the floating point model to 8 bits, perform quantization fine tuning,  and compile to target running on the Xininx Deep Learning Processor (DPU).
Evaluations of model accuracy are performed before and after quantization, and the compilation step includes details on the final implementation.

## Step-by-step tutorial

### 1.0 Prerequisites and Setup
- Host machine with Ubuntu 16.04, 18.04 or other linux Vitis-AI supported distributions
- Vitis-AI installed on your host machine. Follow the instructions from https://github.com/Xilinx/Vitis-AI
- ZCU102 or ZCU104 Xilinx evaluation boards, or Alveo U50 Acceleration card.  Note: If you would like to use the VCK190 board you will need to apply for the early access lounge, contact your local sales team for more information.
- Setup  VART on your target board according to the instructions on https://github.com/Xilinx/Vitis-AI/tree/master/demo/VART

### 2.0 Start Vitis AI and Launch Jupyter Notebook

Launch the gpu (shown below) or cpu docker container. Note if you use the cpu docker container training and quantization will take much longer.

```bash <Path to Vitis-AI Install>/Vitis-AI/docker_run.sh xilinx/vitis-ai-gpu:latest```

Once the docker container launches enable the conda tensroflow environment and launch the Jupyter notebook server

~~~
conda activate vitis-ai-tensorflow2
jupyter notebook --no-browser --ip=0.0.0.0 --NotebookApp.token='' --NotebookApp.password=''
~~~

You can now point you web browser to the address shown by the jupter notebook server, and open up the notebook file vai_2018_RadioML_VAI_keras.ipynb
This wil take you step by step through the following
- Download the Data set
- Define Keras Model
- Train Model
- Evaluate Model
- Quantize Model
- Compare accuracy of floating point and quantized models
- Optional Quantization Fine Tuning
- Compile model for DPU to create dpu xmodel file

### 3.0 Running on Target Board
Copy following files from the Host machine to the ZCU104, ZCU102, or Alveo U50 boards:
- vai_c_output/dpu.rfClassification_0.elf
- boardSw/test_accuracy.py
- boardSw/test_performance.py
- rf_classes.npy
- rf_snrs.npy
- rf_input.npy

For the ZCU102 or ZCU104 boards, you can use a tool such as WinSCP to connect from a laptop to the board. You will need to first set the appropraite ip address on the board.
For example: ifconfig -eth0 192.155.50.187

#### Performance Test

To run the perforamce test with 4 threads on 1000 RFSamples, enter the following at target board serial or ssh terminal prompt:

```python3 test_performance.py 4 rfClassification.xmodel 1000```

The RF frames are read from the file rf_input.npy, and the performance number will be reported. Each RF frame is a set of 1024 I and Q Samples
Results shown below are using a ZCU104 board with 2 4096DPUS at 300Mhz (2.45 Peak INT8 TOPs).
You can experiment with differnet number of threads and image counts.

<img src="media/perf.png">


#### Accuracy Test

To run the accuracy test, enter the following at target board serial or shh terminal prompt:

``` python3 test_accuracy.py rfClassification_0.xmodel```

The RF frames, snrs, and modulation classes are read from the files rf_input.npy, rf_snrr.npy, rf_classes.npy.
The Top1 accuracy should be close to what was measure for the quantized or quantized fine tuned model. As we saw when evaluating the models in the Juptyer notebook, the classification is more accuate for higher SNRs. The results below show ZCU104 accuracy on quantized model w/o fine tuning.

<img src="media/accuracy.png">


## Future Work

This tutorial is meant as a starting point for users trying to work with RF Modulation Recognition in Xilinx devices. Research and Development continues in this area, and future implementations may include RNNs.
Please file a github issue if there are problems, or post on forums.xilinx.com for feature requests for future versions.

### References:

**[1]** Over-the-Air Deep Learning Based Radio Signal Classification,
IEEE JOURNAL OF SELECTED TOPICS IN SIGNAL PROCESSING, VOL. 12, NO. 1,
FEBRUARY 2018

**[2]** <https://www.deepsig.ai/datasets>

**[3]**  Xilinx Vitis-AI: <https://github.com/Xilinx/Vitis-AI>

Copyright 2020 Xilinx Inc.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License.

<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020-2021 Xilinx</sup></p>
