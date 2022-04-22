##############################################
Vitis HLS
##############################################



.. sidebar:: Generating Vivado IP from C/C++ code

   Vitis HLS can also be used to generate Vivado IP from C/C++ code, but that flow is not the subject of this tutorial. Although similar, there are some significant differences between producing Vitis XO kernels and Vivado RTL IP. However, you can use this tutorial as a general introduction to the Vitis HLS tool.



Vitis High-Level Synthesis (HLS) is a key part of the Vitis application acceleration development flow. The tool is responsible for compiling C/C++ and OpenCL code into a kernel for acceleration in the programmable logic (PL) region of Xilinx devices. Thus, it is the tool that compiles the hardware kernels for the Vitis tools by performing high-level synthesis.


In this tutorial, you will work through the Vitis HLS tool GUI to build, analyze, and optimize a hardware kernel. You are working through the Vitis kernel flow in the Vitis tool. For more information, refer to `Enabling the Vitis Kernel Flow <https://www.xilinx.com/cgi-bin/docs/rdoc?v=2021.1;t=vitis+doc;d=vitis_hls_process.html;a=uiy1584905571731>`_ in the Vitis HLS Flow of the Vitis Unified Software Platform Documentation (UG1416).



***************************************
Before You Begin
***************************************

The labs in this tutorial use:

* BASH Linux shell commands.
* 2021.1 Vitis core development kit release and the **xilinx_u200_gen3x16_xdma_1_202110_1** platform. If necessary, it can be easily ported to other versions and platforms.


.. warning:: Before you run any examples

   * Before running any of the examples, make sure you have installed the Vitis core development kit as described in `Installation <https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/acceleration_installation.html#vhc1571429852245>`_ in the Application Acceleration Development flow of the Vitis Unified Software Platform Documentation (UG1416).
   * If you run applications on the Xilinx® Alveo™ Data Center accelerator cards, ensure the card and software drivers have been correctly installed by following the instructions To complete installation, follow the instructions on the `Alveo Product Documentation tab  <https://www.xilinx.com/products/boards-and-kits/alveo.html>`_.


***************************************************
Set Up The Environment to Run Vitis
***************************************************

To configure the environment to run Vitis, run the following scripts which set up the environment to run in a specific command shell.


.. code-block:: bash

   source <Vitis_install_path>/Vitis/2021.1/settings64.sh
   source /opt/xilinx/xrt/setup.sh


.. note:: ``.csh`` scripts are also provided, but this tutorial assumes a bash shell is used.


To specify the location of any Data-Center or Embedded platforms you have installed, set the following environment variable:

.. code-block:: bash

   export PLATFORM_REPO_PATHS=<path to platforms>


.. note:: On some Ubuntu distributions, to properly set up Vitis, you must also export ``LIBRARY_PATH``.

.. code-block:: bash

   export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu



.. sidebar::

   For more information see <Xilinx AR 73698 <https://www.xilinx.com/support/answers/73698.html>`_.


**********************************************************
Access the Tutorial Reference Files
**********************************************************

1. To access the reference files, type the following into a terminal: ``git clone https://github.com/Xilinx/Vitis-Tutorials``.
2. Navigate to the ``Getting_Started/Vitis_HLS`` directory, and then access the ``reference-files`` directory.


****************************************
Next Steps
****************************************

.. toctree::
   :maxdepth: 3

   Creating a Vitis HLS Project <./new_project>
   Running High-Level Synthesis and Analyzing Results <./synth_and_analysis>
   Using Optimization Techniques <./optimization_techniques>
   Reviewing the DATAFLOW Optimization <./dataflow_design>

