<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>Vitis AI Tutorials</h1>
   </td>
 </tr>
 <tr>
 <td align="center"><h3> Profiling a CNN Using DNNDK or VART with Vitis AI</h3>
 </td>
 </tr>
</table>

# Current Status

* Tested with Vitis AI 1.3.
* Tested in hardware on ZCU102

###Date: 14 Jan 2021

# 1 Introduction

This tutorial shows you different ways to profile a CNN application running on the ZCU102 target board with Vitis&trade; AI 1.3, which is a set of optimized IP, tools, libraries, models and example designs valid for AI inference on both Xilinx edge devices and Alveo&trade; Data Center accelerator cards.

For more information, see the following sites:
* [Vitis AI product page](https://developer.xilinx.com/en/get-started/ai.html)
* [AI Edge Platform product page](https://www.xilinx.com/products/design-tools/ai-inference/edge-ai-platform.html)
* [Alveo Data Center Accelerator Card product page](https://www.xilinx.com/products/boards-and-kits/alveo.html)

In order to follow this tutorial, you must have already trained and quantized your CNN, whether you selected Caffe or TensorFlow.
In fact compiling, running and debugging the C++ (or Python) application on the embedded system composed by the Deep Processor Unit (DPU)
and the ARM CPU is almost independent from the adopted ML framework.

Note that the previous release of this tutorial -based on Vitis AI 1.2- is available [here](https://github.com/Xilinx/Vitis-AI-Tutorials/tree/Profiling-DNNDK-Examples).

# 2 Prerequisites

- Ubuntu 16.04 host PC with Python 3.6.

- The entire repository of [Vitis AI stack release 1.3](https://github.com/Xilinx/Vitis-AI) from [www.github.com/Xilinx](https://www.github.com/Xilinx).

-  Accurate reading of [Vitis AI User Guide UG1414 v1.3](https://www.xilinx.com/support/documentation/sw_manuals/vitis_ai/1_3/ug1414-vitis-ai.pdf). In particular:

  1. "Vitis AI Overview" in Chapter 1 with DPU naming and guidelines to download the tools container available from [docker hub](https://hub.docker.com/r/xilinx/vitis-ai/tags) and the Runtime Package for edge (MPSoC) devices.
  2. "Installation and Setup" instructions of Chapter 2 for both host and target;
  3. "Quantizing the Model" in Chapter 4 and "Compiling the Model" in Chapter 5.
  4. "Programming with VART" APIs in Chapter 6.

- The [ZCU102](https://www.xilinx.com/products/boards-and-kits/ek-u1-zcu102-g.html) evaluation board with its [image file](https://www.xilinx.com/bin/public/openDownload?filename=xilinx-zcu102-dpu-v2020.2-v1.3.0.img.gz), which contains a pre-built working design for the ZCU102 with the DPUCZDX8G (renamed shortly as "DPUv2" in the following).

- In this tutorial you will use FCN8 and AlexNet CNNs and their ``elf`` files that were respectively generated in the Vitis AI 1.2 Tutorials:
  - [FCN8 and UNET Semantic Segmentation with Keras and Xilinx Vitis AI](https://github.com/Xilinx/Vitis-AI-Tutorials/tree/VAI-KERAS-FCN8-SEMSEG)
  - [Quantization and Pruning of AlexNet CNN trained in Caffe with Cats-vs-Dogs dataset](https://github.com/Xilinx/Vitis-AI-Tutorials/tree/VAI-Caffe-ML-CATSvsDOGS)

- Familiarity with Deep Learning principles.


## 2.1 Dos-to-Unix Conversion

In case you might get some strange errors during the execution of the scripts, you have to pre-process -just once- all the``*.sh`` shell and the python ``*.py`` scripts with the [dos2unix](http://archive.ubuntu.com/ubuntu/pool/universe/d/dos2unix/dos2unix_6.0.4.orig.tar.gz) utility.
In that case run the following commands from your Ubuntu host PC (out of the Vitis AI docker images):
```bash
sudo apt-get install dos2unix
cd <WRK_DIR> #your working directory
for file in $(find . -name "*.sh"); do
dos2unix ${file}
done
```

## 2.2 Working Directory

In the following of this document, it is assumed that you have cloned the [Vitis AI stack release 1.3](https://github.com/Xilinx/Vitis-AI) and this is your working directory ``<WRK_DIR>`` (for example in my case I renamed it shortly as ``~/ML/VAI1v3``).

This tutorial repository is then cloned and placed in a ``tutorial`` sub-folder below the ``<WRK_DIR`` directory and then renamed as ``VAI-Profiling-DNNDK-VART``.


## 2.3 Before starting with Vitis AI 1.3

To list the currently available docker images run:
```bash
docker images # to list the current docker images available in the host pc
```
and you should see something like in the following text:
```text
REPOSITORY            TAG                               IMAGE ID            CREATED             SIZE
xilinx/vitis-ai-gpu   latest                            1bc243fc037a        41 minutes ago      19GB
```

To launch the docker container with Vitis AI tools - to do all the steps from CNN training to generation of the ELF file for the DPU - based on CPU (or GPU), execute the following commands from the ``<WRK_DIR>`` folder:

```bash
cd <WRK_DIR> # you are now in Vitis_AI subfolder
./docker_run.sh xilinx/vitis-ai-gpu:1.3

#if you want to use tensorflow
conda activate vitis-ai-tensorflow

#if you want to use caffe
conda activate vitis-ai-caffe

```

Note that the container maps the shared folder ``/workspace`` with the file system of the Host PC from where you launch the above command, which is ``<WRK_DIR>`` in your case.
This shared folder enables you to transfer files from the Host PC to the docker container and vice versa.

The docker container does not have any graphic editor, so it is recommended that you work with two terminals and you point to the same folder, in one terminal you use the docker container commands and in the other terminal you open any graphic editor you like.

Note that docker does not have an automatic garbage collection system as of now. You can use this command to do a manual garbage collection:
```
docker rmi -f $(docker images -f "dangling=true" -q)
```

## 2.4 DNNDK Setup

Before you start the tutorial, you have to follow and execute the Step1 and Step2 instructions of the [DNNDK](https://github.com/Xilinx/Vitis-AI/blob/master/demo/DNNDK/README.md) section to setup ``petalinux_sdk`` and the ZCU102 SD card image.


**Important**: In GitHub you cannot store a file larger than 25MB, so all the ``.elf`` and ``.so`` files were compressed with ``gzip``. Before running any script, you must manually uncompress those files.


# 3.0 Q&A about Profiling and Multithreading

<details><summary>Does the multithreading execution of kernels running in parallel deliver deterministic results?</summary>
</p>
  The latency through the DPU (assuming it has data available and the system is not memory bound) should be somewhat deterministic. The latency from when you start a thread to the time when it completes is not necessarily deterministic as it may depend on the number of threads launched, system utilization, and other settings. There is also the capability with the DNNDK APIs to set ``core affinity`` (which DPU will execute which task) as well as ``priority`` (which task takes priority), so if you have a higher priority task, you can set the priority higher for that task.</details>
</p>
<details><summary>Is there any other way to find out the optimum number of threads or only testing?</summary>
</p>
 It seems that most of the time, for the 3 B4096 DPUs on the ZCU102 board, 6 threads is a good number.  That said, it depends on the number of DPUs available, the time it takes to execute the model, and the software loading of the system.  Once you have the multithreaded application setup, it should be pretty easy to vary this number at runtime.</details>
</p>
<details><summary>What does one "kernel" mean?</summary>
</p>
  A kernel is the instantiation of a task (for example a certain CNN) on the DPU.</details>
</p>
<details><summary>How do the kernels communicate to each other, or do they run completely independent to each other?</summary>
</p>
  The kernels are totally independent from each other.</details>
</p>
<details><summary>How are the overall processing steps divided up in kernels?</summary>
</p>
  The kernel or DPU task (or "runner") is the CNN model.  When you compile the model it becomes a kernel, so the processing steps are those contained within the model.</details>
</p>
<details><summary>Does one frame map to one kernel, or is one frame processed by multiple kernels?</summary>
</p>
  One frame maps to one kernel, though perhaps it is possible that you could have multiple inputs to a CNN model and you could possibly be inputting multiple frames or other data to the various inputs. It is also possible that you could segment a model into multiple kernels, though you would need to compile each one individually, then take the output from that kernel and feed it to the next. You could theoretically create a pipeline in this case that uses multiple DPUs, each which execute a portion of the model and are fed by the output of the previous kernel.</details>
</p>
<details><summary>Is there any method to define what is being processed within one kernel?</summary>
</p>
  You have the ability to define what is in the CNN model. If you want to only process a portion of the model, then make that the output layer or input layer when you quantize it with ``vai_q_*`` and compile it with ``vai_c_*`` (where ``*`` means either ``caffe`` or ``tensorflow``).</details>
</p>
<details><summary>What can I do with the profiling results?</summary>
</p>
  Once the fine-grained profiling is done over one specific CNN model, the Vitis AI compiler (``vai_c_*``) does not currently offer open parameters/options for the CNN performance tuning in the ``elf`` file. However, if you are not satisfied with the performance delivered by the DPU core, you can try to modify the DPU configurations in order to obtain better performance. For example, you can try applying more advanced DPU architectures from B1152 to B4096, or applying ``high RAM usage``.  Refer to [Configurate the DPU](https://github.com/Xilinx/Vitis-AI/blob/v1.2.1/DPU-TRD/prj/Vivado/README.md) for more details. Otherwise, if the DPU core offers enough performance, you can try to modify the DPU configurations with lower logic resources, which will be beneficial for other subsystems to be implemented later in the FPGA.</details>


# 4 DPU Profiling with DNNDK C++ APIs

There are at least three possible **profiling methods** to measure the throughput performance of the embedded system composed by the ARM CPU and the DPU IP core:
* manually profiling only the CNN APIs called by the ARM CPU,
* automatically profiling all the CNN layers running on the DPU IP core (which is called **fine-grained profiling**),
* manually computing the elapsed time - with image pre-processing and data loading operation included

The first and third profiling methods require a different compilation flag from the second method: ``--options    "{'mode':'normal'}"`` (for methods 1 and 3) and  ``--options    "{'mode':'debug'}"`` (for method 2) in the ``vai_c`` script used to generate the DPU ``elf`` file from the quantized CNN. This is illustrated in the following fragments of code, respectively for a CNN quantized with ``vitis-ai-caffe`` or ``vitis-ai-tensorflow`` anaconda environments:

```bash
# conda activate vai_caffe
vai_c_caffe  --prototxt=${model_dir}/deploy.prototxt     \
     --caffemodel=${model_dir}/deploy.caffemodel \
     --output_dir=${output_dir}                  \
     --net_name=${CNN}                           \
     --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU102/arch.json \
     --options    "{'mode':'normal', 'save_kernel':''}"
#    --options    "{'mode':'debug'}"

# conda activate vai_tensorflow
vai_c_tensorflow \
       --frozen_pb=${model_dir}/deploy_model.pb \
       --output_dir=${output_dir}               \
       --net_name=${CNN}                        \
       --arch /opt/vitis_ai/compiler/arch/DPUCZDX8G/ZCU102/arch.json \
       --options    "{'mode':'normal'}"
#      --options    "{'mode':'debug'}"       
```

In both the first and second methods, the image preprocessing CPU overhead is not taken in account. In fact, in the current application, the ARM CPU runs the preprocessing in software, which is surely not an efficient solution being it very slow. In real life scenario an hardware accelerator would do that in the MPSoC fabric with much smaller latency, typically using
the [Vitis Vision library](https://github.com/Xilinx/Vitis_Libraries/tree/master/vision) based on Open-CV functions optimized for the FPGA.

While the results measured by method 1 and 2 should be quite in agreement (note that method 2 is the most precise), the results of method 3 should be worst because of the overhead of the ARM CPU (running in software the tasks of file I/O operations). Note also that all those results must be measured with a single thread execution.

## 4.1 Profiling AlexNet CNN for Image Classification

In the following of this Section you will see how profiling the ``AlexNet`` CNN trained with Caffe on the ``Dogs vs. Cats`` dataset for image classification with RGB images of size 227x227x3, as illustrated in the [UG1336](https://github.com/Xilinx/Vitis-AI-Tutorials/tree/VAI-Caffe-ML-CATSvsDOGS).The same concepts are valid also for any other CNN.

To save time, you can run profiling using one input image, as its results do not depend on the amount of input images. Due to that the folder ``test_image`` contains only one image, differently from the archive ``test_images.tar.gz``.

Make an archive of the ``alexnet_zcu102`` folder and then copy ``alexnet_zcu102.tar`` to your ZCU102 target board with ``scp`` utility: for example, assuming your target board have static IP address value of ``192.168.1.40``, the command will be ``scp ./alexnet_zcu102.tar root@192.168.1.40:~/`` (password is ``root``).

All the results of next subsections are obtained by running the following commands (they are all contained in the script ``run_all.sh``) directly from the ZCU102 board:

  ```bash
  # extract the archive
  cd ~
  mv alexnet_zcu102.tar ~/DNNDK/
  cd ~/DNNDK
  tar -xvf alexnet_zcu102.tar
  cd alexnet_zcu102
  # crosscompile the C++ applications with DNNDK APIs  
  bash ./crosscompile_alexnet.sh
  # extract the test input images
  tar -xvf test_images.tar
  # run baseline CNN
  cd baseline
  bash ./run_all_baseline.sh
  # run pruned CNN
  cd ../pruned
  bash ./run_all_pruned.sh  
  ```

Log files were captured for your comfort and placed in the ``alexnet_zcu102/log/`` folder as a reference of what you should see during the processing.


### 4.1.1 First Method

In the first profiling method the DPU elapsed time is measured "manually" with the following fragment of C++ code in the ``run_CNN()`` subroutine from [fps_main_method1.cc](files/alexnet_zcu102/baseline/src/fps_main_method1.cc):
```
#define SHOWTIME

#ifdef SHOWTIME
#define _T(func)                                                              \
        auto _start = system_clock::now();                                    \
        func;                                                                 \
        auto _end = system_clock::now();                                      \
        auto duration = (duration_cast<microseconds>(_end - _start)).count(); \
        string tmp = #func;                                                   \
        tmp = tmp.substr(0, tmp.find('('));                                   \
        cout << "[TimeTest]" << left << setw(30) << tmp;                      \
        cout << left << setw(10) << duration << "us" << endl;                 \
#else
#define _T(func) func;
#endif

...
void run_CNN(DPUTask *taskConv, Mat img)
{
  // Get the output Tensor
  int8_t *outAddr = (int8_t *)dpuGetOutputTensorAddress(taskConv, CONV_OUTPUT_NODE);
  // Get size of the output Tensor
  int size = dpuGetOutputTensorSize(taskConv, CONV_OUTPUT_NODE);
  // Get channel count of the output Tensor
  int channel = dpuGetOutputTensorChannel(taskConv, CONV_OUTPUT_NODE);
  // Get scale of the output Tensor
  float out_scale = dpuGetOutputTensorScale(taskConv, CONV_OUTPUT_NODE);
  ...
  _T(dpuSetInputImage2(taskConv, CONV_INPUT_NODE, img));
  ...
  _T(dpuRunTask(taskConv));
  ...
  // Calculate softmax on CPU and show TOP5 classification result
  _T(dpuRunSoftmax(outAddr, softmax, channel, size/channel, out_scale));
  TopK(softmax, channel, 5, kinds);
  ...
}

...

void classifyEntry(DPUKernel *kernelConv)
{

...
#define DPU_MODE_NORMAL 0
#define DPU_MODE_PROF   1
#define DPU_MODE_DUMP   2

    /* Create DPU Tasks for CONV  */
    DPUTask *taskConv = dpuCreateTask(kernelConv, DPU_MODE_NORMAL); // profiling not enabled
    //DPUTask *taskConv = dpuCreateTask(kernelConv, DPU_MODE_PROF); // profiling enabled
    //enable profiling
    //int res1 = dpuEnableTaskProfile(taskConv);
    //if (res1!=0) printf("ERROR IN ENABLING TASK PROFILING FOR CONV KERNEL\n");

...
}
```

Be sure to have generated the ``dpu_*.elf`` file -after the CNN quantization process- with the ``vai_c_caffe`` compiler using the following flag ``--options "{'mode':'normal'}"``.

As reported in the [logfile_target_dnndk_baseline.txt](files/alexnet_zcu102/log/logfile_target_dnndk_baseline.txt), at run time execution you will see something like this for each input image (1 us = 1e-6 s):

```text
./fps_alexnetBNnoLRN_method1 1
now running ./fps_alexnetBNnoLRN_method1 1
total image : 1
[TimeTest]dpuSetInputImage              467       us
[TimeTest]dpuRunTask                    11105     us
[TimeTest]dpuRunSoftmax                 354       us
[TimeTest]TopK                          3         us
```
summing all together this is equivalent to 11.92ms, which corresponds to a frame rate of 83.82Hz.


### 4.1.2 Second Method

The second profiling method is explained in Chapter 8 at Section "Fine-Grained Profiling" on the [Vitis AI User Guide UG1414 v1.2](https://www.xilinx.com/support/documentation/sw_manuals/vitis_ai/1_2/ug1414-vitis-ai.pdf).

This is indeed the real profiling and it requires the ``dpu_*.elf`` file to be generated by the ``vai_c_caffe`` compiler with the following flag ``--options "{'mode':'debug'}"``.  Furthermore, you have to modify the C++ code
in the ``classifyEntry()`` subroutine as illustrated in the next fragment take from [fps_main_method2.cc](files/alexnet_zcu102/baseline/src/fps_main_method2.cc) file:

```
//#define SHOWTIME

void classifyEntry(DPUKernel *kernelConv)
{
...

    //DPUTask *taskConv = dpuCreateTask(kernelConv, DPU_MODE_NORMAL); // profiling not enabled
    DPUTask *taskConv = dpuCreateTask(kernelConv, DPU_MODE_PROF); // profiling enabled
    int res1 = dpuEnableTaskProfile(taskConv);
    if (res1!=0) printf("ERROR IN ENABLING TASK PROFILING FOR CONV KERNEL\n");
...
}
```

After having cross compiled the application and running it on the target board, at run time execution you will see something like this
(which is similar to a [gprof](https://web.eecs.umich.edu/~sugih/pointers/gprof_quick.html) report):

```text
DNNDK] Performance profile - DPU Kernel "alexnetBNnoLRN_0" DPU Task "alexnetBNnoLRN_0-0"
=====================================================================================================
  ID                       NodeName Workload(MOP) Mem(MB) RunTime(ms) Perf(GOPS) Utilization    MB/S
   1                          conv1       210.830    0.26       1.055      199.8        16.3%   245.2
   2                          conv2       895.795    0.70       0.953      940.0        76.5%   734.9
   3                          conv3       299.041    0.95       0.343      871.8        71.0%  2783.5
   4                          conv4       448.561    1.40       0.508      883.0        71.9%  2750.9
   5                          conv5       299.041    0.92       0.349      856.9        69.7%  2635.5
   6                            fc6        75.497   36.09       5.327       14.2         1.2%  6774.3
   7                            fc7        33.554   16.08       2.338       14.4         1.2%  6877.7
   8                            fc8         0.016    0.01       0.014        1.2         0.1%   845.5

                Total Nodes In Avg:
                                All      2262.336   59.15      10.887      207.8        16.9%  5433.1
=====================================================================================================
```

The runtime execution is aligned with method1 -although more accurate (having less overhead)- and shows a runtime execution of 10.87ms, which corresponds to a frame rate of ~92Hz.

Note also that this fine-grained profiling shows you the following parameters:
- ``Workload(MOP)``: Computation workload (MAC indicates two operations);
- ``Mem(MB)``: Memory size for code, parameter, and feature map for this DPU node;
- ``RunTime(ms)``: The execution time in unit of millisecond (ms);
- ``Perf(GOPS)``: The DPU performance in unit of GOP per second, given by ``Workload(MOP)/RunTime(ms)``
- ``Utilization``: The DPU utilization in percent (%);
- ``MB/S``: The average DDR memory access bandwidth, given by ``Mem(MB)/Runtime(ms)``.


### 4.1.3 Third Method

In the third method the DPU elapsed time - including image preprocessing running on ARM CPU - is measured with the following fragment of C++ code in the ``classifyEntry()`` subroutine from [fps_main_method3.cc](files/alexnet_zcu102/baseline/src/fps_main_method3.cc):

```
//#define SHOWTIME

...

#include <chrono>
auto _start = system_clock::now(); //timers
for (auto i = 0; i < threadnum; i++){
workers[i] = thread([&,i]() {

  /* Create DPU Tasks for CONV  */
  DPUTask *taskConv = dpuCreateTask(kernelConv, DPU_MODE_NORMAL); // profiling not enabled

  for(unsigned int ind = i  ;ind < images.size();ind+=threadnum)
    {

      Mat img = imread(baseImagePath + images.at(ind)); //OpenCV read image
      run_CNN(taskConv, img); //this contains the image pre-processing
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

...

DPUTask *taskconv = dpuCreateTask(kernelconv, DPU_MODE_NORMAL); // profiling not enabled
//DPUTask *taskconv = dpuCreateTask(kernelconv, DPU_MODE_PROF); // profiling enabled
//enable profiling
//int res1 = dpuEnableTaskProfile(taskconv);
//if (res1!=0) printf("ERROR IN ENABLING TASK PROFILING FOR CONV KERNEL\n");

```
At run time execution you will see something like this:
```text
...
[Time]17368us
[FPS]57.5772
...
```
The effective frame rate is now 57.57Hz, as it includes also the ARM CPU cycles spent for executing SW routines to load and preprocess the input images.


## 4.2 Increasing AlexNet Performance in Terms of FPS

### 4.2.1 Multithreading

The code adopted for method 3 [fps_main_method3.cc](files/alexnet_zcu102/src/fps_main_method3.cc) is the most efficient to try multithreading experiments, adding more images in input to the DPU, with the hope to increase the data rate in terms of "frames-per-second" or "fps" shortly.

As reported in the  [logfile_target_dnndk_baseline.txt](files/alexnet_zcu102/log/logfile_target_dnndk_baseline.txt), and illustrated in the following fragment:

```
./fps_alexnetBNnoLRN 1
now running ./fps_alexnetBNnoLRN 1
total image : 1000
[Time]14289605us
[FPS]69.9809

./fps_alexnetBNnoLRN 2
now running ./fps_alexnetBNnoLRN 2
total image : 1000
[Time]8427321us
[FPS]118.662

./fps_alexnetBNnoLRN 3
now running ./fps_alexnetBNnoLRN 3
total image : 1000
[Time]7507790us
[FPS]133.195

./fps_alexnetBNnoLRN 4
now running ./fps_alexnetBNnoLRN 4
total image : 1000
[Time]6866431us
[FPS]145.636

./fps_alexnetBNnoLRN 5
now running ./fps_alexnetBNnoLRN 5
total image : 1000
[Time]6563955us
[FPS]152.347

./fps_alexnetBNnoLRN 6
now running ./fps_alexnetBNnoLRN 6
total image : 1000
[Time]6609546us
[FPS]151.296
```

The best performance achieves ~152fps with 5 threads in parallel. This happens because the DPU multithreading environment will instantiate 5 kernels running in parallel and loading images at a different time, thus using the DPU architecture in a more efficient way. This is almost a factor of 3 in terms of performance increase.


### 4.2.2 Pruning

Some CNNs - as ``AlexNet`` - have naturally a high level of redundancy and so they can be optimized with a "pruning" technique, as explained in [UG1336](https://github.com/Xilinx/Vitis-AI-Tutorials/tree/VAI-Caffe-ML-CATSvsDOGS), which means the amount of operations can be greatly reduced by pruning the CNN without detriment of its prediction accuracy. However, there are other CNNs -like ``MobileNet`` - which cannot be further pruned otherwise their intelligence could be destroyed.

When running the "pruned" ``AlexNet`` with 5 threads the frame rate increases to 410.77Hz, as reported in the [logfile_target_dnndk_pruned.txt](files/alexnet_zcu102/log/logfile_target_dnndk_pruned.txt)

```
./fps_alexnetBNnoLRN 5
now running ./fps_alexnetBNnoLRN 5
total image : 1000
[Time]2434441us
[FPS]410.772
```

whereas the average top-1 accuracy is 0.95 (it was 0.94 in the baseline, not-pruned, CNN):

```
number of total images predicted  999
number of top1 false predictions  47
number of top1 right predictions  952

top1 accuracy = 0.95
```


## 4.3 Profiling FCN8 CNN for Semantic Segmentation


In this Section you see how profiling the ``FCN8`` CNN for Semantic Segmentation trained with Keras/TensorFlow on a small dataset (which is part of the [CamVid](http://mi.eng.cam.ac.uk/research/projects/VideoRec/CamVid/))  with RGB images of size 224x224x3, as illustrated in the [UG1445](https://github.com/Xilinx/Vitis-AI-Tutorials/tree/VAI-KERAS-FCN8-SEMSEG).

To save time, you can run profiling using only one input image, as its results do not depend on the amount of input images. Due to that the archive ``test1.tar`` contains only one image, differently from the archive ``test_images.tar``.

Make an archive of the ``fcn8_zcu102`` folder and then copy ``fcn8_zcu102.tar`` to your ZCU102 target board with ``scp`` utility: for example, assuming your target board have static IP address value of ``192.168.1.40``, the command will be ``scp ./fcn8_zcu102.tar root@192.168.1.40:~/`` (password is ``root``).

All the results are obtained by running ``run_all.sh`` script directly from the ZCU102 board:

Log files were captured for your comfort and placed in the ``fcn8_zcu102/log`` folder.



### 4.3.1 Profiling results with method2

An alternative  way to implement the method2 (fine grained profiling), which is also simpler than what done for ``alexnet`` example, is to avoid using the following lines of code (from the [fps_main.cc](files/fcn8_zcu102/fcn8/src/fps_main.cc) C++ application file):

```cpp
DPUTask *taskConv = dpuCreateTask(kernelConv, DPU_MODE_PROF); // profiling enabled
//enable profiling
int res1 = dpuEnableTaskProfile(taskConv);
if (res1!=0) printf("ERROR IN ENABLING TASK PROFILING FOR CONV KERNEL\n");
```

and just use only the following line:

```cpp
DPUTask *taskConv = dpuCreateTask(kernelConv, DPU_MODE_NORMAL); // profiling not enabled
```

then you enable the ``mode = profile`` with the ``dexplorer`` DNNDK utility (running on the target board), as illustrated in this fragment of code taken from the [run_on_zcu102.sh](files/fcn8_zcu102/run_on_zcu102.sh) shell script, and just lunch the application:

```bash
dexplorer -m profile # enable profiling
./dbg_fcn8 1         # launch the application
```

As already done previously for the ``alexnet`` example, the ``dbg_dpu_fcn8.elf`` file was generated by the ``vai_c_tensorflow`` compiler with the flag ``--options "{'mode':'debug'}"``.

After having cross compiled the application and running it on the target board, as reported in the [logfile_target_fcn8.txt](files/fcn8_zcu102/log/logfile_target_fcn8.txt), at run time execution of ``fcn8``
you will see something like this:

```text
[DNNDK] Performance profile - DPU Kernel "fcn8" DPU Task "fcn8-0"
=====================================================================================================
  ID                       NodeName Workload(MOP) Mem(MB) RunTime(ms) Perf(GOPS) Utilization    MB/S
   1       block1_conv1_convolution       173.408    3.22       0.753      230.3        18.7%  4273.2
   2       block1_conv2_convolution      3699.376    3.88       3.251     1137.9        92.6%  1193.5
   3       block2_conv1_convolution      1849.688    2.37       1.646     1123.7        91.5%  1442.8
   4       block2_conv2_convolution      3699.376    2.07       3.248     1139.0        92.7%   636.3
   5       block3_conv1_convolution      1849.688    1.44       1.647     1123.1        91.4%   872.9
   6       block3_conv2_convolution      3699.376    2.11       3.250     1138.3        92.6%   648.9
   7       block3_conv3_convolution      3699.376    1.53       3.249     1138.6        92.7%   472.0
   8       block4_conv1_convolution      1849.688    1.71       1.871      988.6        80.5%   914.9
   9       block4_conv2_convolution      3699.376    3.04       3.719      994.7        81.0%   816.8
  10       block4_conv3_convolution      3699.376    2.75       3.720      994.5        80.9%   739.1
  11           pool4_11_convolution         2.408    0.10       0.030       80.3         6.5%  3483.8
  12       block5_conv1_convolution       924.844    2.45       0.956      967.4        78.7%  2565.3
  13 conv2d_transpose_2_conv2d_transpose    0.226    0.01       0.023        9.8         0.8%   556.6
  14       block5_conv2_convolution       924.844    2.45       0.956      967.4        78.7%  2565.3
  15           pool3_11_convolution         4.817    0.21       0.046      104.7         8.5%  4665.9
  16       block5_conv3_convolution       924.844    2.38       0.957      966.4        78.6%  2487.4
  17              conv6_convolution      1258.815   12.31       3.112      404.5        32.9%  3955.9
  18              conv7_convolution        25.690    0.31       0.088      291.9        23.8%  3488.8
  19 conv2d_transpose_1_conv2d_transpose    9.634    0.13       0.048      200.7        16.3%  2660.8
  20                add_layer_add_1         0.000    0.03       0.029        0.0         0.0%   998.3
  21 conv2d_transpose_3_conv2d_transpose   14.451    0.61       0.281       51.4         4.2%  2169.8

                Total Nodes In Avg:
                                All     32009.303   47.31      32.880      973.5        79.2%  1438.9
=====================================================================================================
```



# 5 Experiments with VART Python and C++ APIs

Vitis AI Run Time - shortly named VART - enables applications to use the unified high-level runtime API for both cloud and edge. In fact the DNNDK API are necessary to support the Legacy DNNDK examples for edge and are not portable at all on the cloud (at the time there was not yet any Vitis AI). If you use VART, the same application - as it is - can be targeted to either an edge or an Alveo card. Furthermore - and much more important - VART API abstract the CNN features at much higher level than the DNNDK API, which means a much simpler C++ code to be managed.  

The first action you have to do about VART is reading the instructions of [Vitis AI VART README.md](https://github.com/Xilinx/Vitis-AI/tree/master/demo/VART/README.md).


## 5.1 AlexNet CNN

You can find the most recent implementation of AlexNet CNN using VART APIs in this Vitis AI 1.3 [01-caffe_cats_vs_dogs ](https://github.com/Xilinx/Vitis-Tutorials/tree/master/Machine_Learning/Design_Tutorials/01-caffe_cats_vs_dogs) tutorial.



## 5.2 FCN8 and FCN8UPS  CNNs

You can find the most recent implementation of FCN8 and FCN8UPS CNNs using VART APIs in this Vitis AI 1.3 [05-Keras_FCN8_UNET_segmentation](https://github.com/Xilinx/Vitis-Tutorials/tree/master/Machine_Learning/Design_Tutorials/05-Keras_FCN8_UNET_segmentation) tutorial.
