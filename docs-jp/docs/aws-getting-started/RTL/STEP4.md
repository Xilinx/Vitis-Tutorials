# Practice and Experiment with RTL Kernels

Apply and practice what you've learned in the earlier steps of this guide by running more hands-on examples. Familiarize yourself with online resources relating to the SDAccel™ environment, and make your way to the [AWS forum](https://forums.aws.amazon.com/forum.jspa?forumID=243) to search for knowledge and find answers.

## Experiment with Other Examples

Running and experimenting with the following three examples of RTL kernels will help you further familiarize yourself with the RTL kernel flow.

#### Example 1: Vector Addition with two Clocks

This example shows vector addition performed by an RTL kernel with two clocks and the use of the `--kernel_frequency` XOCC option.

Download and run [this example](https://github.com/Xilinx/SDAccel_Examples/tree/aws_2018.3/getting_started/rtl_kernel/rtl_vadd_2clks) from the SDAccel GitHub repository.



#### Example 2: Vector Addition with two Kernels

This example shows how create an accelerated design using more than one RTL Kernel. In this example, Vector Addition is performed with two kernels (Kernel_0 and Kernel_1) which perform vector addition. Kernel_1 reads the output from Kernel_0 as one of two inputs.

[Download and run this example](https://github.com/Xilinx/SDAccel_Examples/tree/aws_2018.3/getting_started/rtl_kernel/rtl_vadd_2kernels) from the SDAccel GitHub repository.

#### Example 3: High-Performance Matrix Multiply with precompiled XO file and advanced Vivado™ implementation options

This example implements a high-performance matrix multiplication of two input matrices (A*B=C). The matrix multiplication kernel operates on matrices of type int16 and produces int16 results. Internally, the kernel has a systolic array of 2048 DSP units and is attached to two DDR banks. The DSP array runs at 400 MHz, whereas the logic around the array runs at 300 MHz.

[Download and run this example](https://github.com/Xilinx/SDAccel_Examples/tree/aws_2018.3/acceleration/high_perf_mat_mult) from the SDAccel GitHub repository.


## Get Support and Troubleshoot Issues

The [AWS F1 SDAccel Development forum](https://forums.aws.amazon.com/forum.jspa?forumID=243) is the place to look for answers, share knowledge and get support. Make sure to subscribe to the forum by clicking the **Watch Forum** link in the Available Actions section.



## Learn More about SDAccel

#### SDAccel QuickTake Video Tutorials

* [Fundamental Concepts of Application Host Code](https://www.xilinx.com/video/hardware/concepts-of-application-host-code.html)

* [Introduction to the SDAccel RTL Kernel Wizard](https://www.xilinx.com/video/software/intro-sdaccel-rtl-kernel-wizard.html)

#### Documentation for SDAccel v2018.3

* _SDx Development Environment Release Notes, Installation, and Licensing Guide_ ([UG1238](https://www.xilinx.com/cgi-bin/docs/rdoc?v=replace;d=ug1238-sdx-rnil.pdf))
* _SDAccel Environment User Guide_ ([UG1023](https://www.xilinx.com/cgi-bin/docs/rdoc?v=replace;d=ug1023-sdaccel-user-guide.pdf))
* _SDAccel Environment Optimization Guide_ ([UG1207](https://www.xilinx.com/cgi-bin/docs/rdoc?v=replace;d=ug1207-sdaccel-optimization-guide.pdf))
* _SDAccel Environment Tutorial_ ([UG1021](https://www.xilinx.com/cgi-bin/docs/rdoc?v=replace;d=ug1021-sdaccel-intro-tutorial.pdf))

<hr/>
<p align="center"><b>
<a href="STEP5.md">NEXT: Install and Run SDAccel on your own Machine</a>
</b></p>
<br>
<hr/>
<p align="center"><sup>Copyright&copy; 2018 Xilinx</sup></p>
