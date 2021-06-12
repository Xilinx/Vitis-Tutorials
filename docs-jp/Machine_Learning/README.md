﻿<table class="sphinxhide">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ 2020.2 / Vitis-AI™ 1.3 - Machine Learning Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis-AI™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

### Introduction to Machine Learning with Vitis AI

Start here! This tutorial series will help to get you the lay of the land working with the
Vitis AI toolchain and machine learning on Xilinx devices.

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
 <tr>
 <td align="center"><a href="../../Machine_Learning/Introduction/README.md">Introduction to Vitis AI</a></td>
 <td>This tutorial puts in practice the concepts of FPGA acceleration of Machine Learning and illustrates how to
 quickly get started deploying both pre-optimized and customized ML models on Xilinx devices.</td>
 </tr>
 </table>

 ### Design Tutorials

These tutorials illustrate end-to-end design concepts or workflows using Vitis AI.

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
 <tr>
 <td align="center">
   <a href="../../Machine_Learning/Design_Tutorials/01-caffe_cats_vs_dogs/README.md">Quantization and Pruning of AlexNet CNN trained in Caffe with Cats-vs-Dogs dataset (UG1336)</a>
 </td>
 <td>Train, prune, and quantize a modified version of the AlexNet convolutional neural network (CNN) with the Kaggle Dogs vs. Cats dataset in order to deploy it on the Xilinx® ZCU102 board.</td>
 </tr>
 <tr>
 <td align="center">
   <a href="../../Machine_Learning/Design_Tutorials/02-MNIST_classification_tf/README.md">MNIST Classification using Vitis? AI and TensorFlow (UG1337)</a>
 </td>
 <td>Learn the Vitis AI TensorFlow design process for creating a compiled ELF file that is ready for deployment on the Xilinx DPU accelerator from a simple network model built using Python. This tutorial uses the MNIST test dataset.</td>
 </tr>
 <tr>
 <td align="center">
   <a href="../../Machine_Learning/Design_Tutorials/03-using_densenetx/README.md">Using DenseNetX on the Xilinx DPU Accelerator (UG1340)</a>
 </td>
 <td>Learn about the Vitis AI TensorFlow design process and how to go from a Python description of the network model to running a compiled model on the Xilinx DPU accelerator.</td>
 </tr>
 <tr>
 <td align="center">
   <a href="../../Machine_Learning/Design_Tutorials/04-Keras_GoogleNet_ResNet/README.md">Deep Learning with Custom GoogleNet and ResNet in Keras and Xilinx Vitis AI (UG1381)</a>
 </td>
 <td>Quantize in fixed point some custom CNNs and deploy them on the Xilinx ZCU102 board, using Keras and the Xilinx7Vitis AI tool chain based on TensorFlow (TF).</td>
 </tr>
 <tr>
 <td align="center">
   <a href="../../Machine_Learning/Design_Tutorials/05-Keras_FCN8_UNET_segmentation/README.md">FCN8 and UNET Semantic Segmentation with Keras and Xilinx Vitis AI (UG1445)</a>
 </td>
 <td>Train the FCN8 and UNET Convolutional Neural Networks (CNNs) for Semantic Segmentation in Keras adopting a small custom dataset, quantize the floating point weights files to an 8-bit fixed point representation, and then deploy them on the Xilinx ZCU102 board using Vitis AI.</td>
 </tr>
 <tr>
 <td align="center">
   <a href="../../Machine_Learning/Design_Tutorials/06-densenetx_DPUv3/README.md">Using DenseNetX on the Xilinx Alveo U50 Accelerator Card (UG1472)</a>
 </td>
 <td>Implement a convolutional neural network (CNN) and run it on the DPUv3E accelerator IP.</td>
 </tr>
 <tr>
 <td align="center">
   <a href="../../Machine_Learning/Design_Tutorials/07-yolov4-tutorial/README.md">Vitis AI YOLOv4</a>
 </td>
 <td>Learn how to train, evaluate, convert, quantize, compile, and deploy YOLOv4 on Xilinx devices using Vitis AI.</td>
 </tr>
 <tr>
  <td align="center">
   <a href="../../Machine_Learning/Design_Tutorials/08-tf2_flow/README.md">TensorFlow2 and Vitis AI design flow</a>
 </td>
 <td>Learn about the TF2 flow for Vitis AI. In this tutorial, you'll be trained on TF2, including conversion of a dataset into TFRecords, optimization with a plug-in, and compiling and execution on a Xilinx ZCU102 board or Xilinx Alveo U50 Data Center Accelerator card.</td>
 </tr>
 <tr>
  <td align="center">
   <a href="../../Machine_Learning/Design_Tutorials/09-mnist_pyt-master/README.md">PyTorch flow for Vitis AI</a>
 </td>
 <td>Learn how to use Vitis AI by using PyTorch. You'll use a simple `get-you-started` example to get started, and then be trained on quantization with a plug-in, and then compiling and execution on a Xilinx ZCU102 board or Xilinx Alveo U50 Data Center Accelerator card.
</td>
 </tr>
 <tr>
  <td align="center">
   <a href="../../Machine_Learning/Design_Tutorials/10-RF_modulation_recognition/README.md">RF Modulation Recognition with TensorFlow 2</a>
 </td>
 <td>Machine learning applications are certainly not limited to image processing! Learn how to apply
 machine learning with Vitis AI to the recognition of RF modulation from signal data.
</td>
 </tr>
 </table>


 ### Feature Tutorials

 Feature tutorials illustrate specific workflows or stages within Vitis AI.

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>Tutorial</b>
 <td width="65%" align="center"><b>Description</b>
 </tr>
 <tr>
 <td align="center"><a href="../../Machine_Learning/Feature_Tutorials/01-freezing_a_keras_model/README.md">Freezing a Keras Model for use with Vitis AI (UG1380)</a></td>
 <td>Freeze a Keras model by generating a binary protobuf (.pb) file.</td>
 </tr>
  <tr>
 <td align="center"><a href="../../Machine_Learning/Feature_Tutorials/02-profiling-example/README.md">Profiling a CNN Using DNNDK or VART with Vitis AI (UG1487)</a></td>
 <td>Profile a CNN application running on the ZCU102 target board with Vitis AI.</td>
 </tr>
  <tr>
 <td align="center"><a href="../../Machine_Learning/Feature_Tutorials/03-edge-to-cloud/README.md">Moving Seamlessly between Edge and Cloud with Vitis AI (UG1488)</a></td>
 <td>Compile and run the same identical design and application code on either the Alveo U50 data center accelerator card or the Zynq UltraScale+™ MPSoC ZCU102 evaluation board. </td>
 </tr>
  <tr>
 <td align="center"><a href="../../Machine_Learning/Feature_Tutorials/04-tensorflow-ai-optimizer/README.md">TensorFlow AI Optimizer Example Using Low-level Coding Style (UG1512)</a></td>
 <td>Use AI Optimizer for TensorFlow to prune an AlexNet CNN by 80% while maintaining the original accuracy.</td>
 </tr>
 </table>

</hr>
<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020-2021 Xilinx</sup></p>
