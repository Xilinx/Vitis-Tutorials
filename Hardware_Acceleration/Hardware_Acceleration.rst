#######################################
Hardware Acceleration
#######################################

.. sidebar:: More Information

    See Vitis™ Development Environment on `xilinx.com <https://www.xilinx.com/products/design-tools/vitis.html>`_



The tutorials under the Hardware Acceleration category help you learn how to use the Vitis core development kit to build, analyze, and optimize an accelerated algorithm developed in C++, OpenCL, and even low-level hardware description languages (HDLs) like Verilog and VHDL. You may also learn how to optimize the CPU side of your application for efficient memory allocation, how to sequence system-level events, and more. 

* The :doc:`Feature Tutorials <./docs/Feature_Tutorials/Feature_Tutorials>` illustrate specific features or flows of Vitis, some features may not be required by all designs but are still useful for some use cases.
* The :doc:`Design Tutorials <./docs/Design_Tutorials/Design_Tutorials>` illustrate higher-level concepts or design flows, walk through specific examples or reference designs, and more complex and complete designs or applications.

These tutorials target different boards including Alveo™ Data Center acceleration cards or MPSoC Evaluation Boards like ZCU104. The table below lists the tutorials available, and the features and flows showcased in all of these tutorials. The various columns correspond to specific features/flows supported in these tutorials and will help you identify tutorials that showcase specific flows and features that you are interested in.






************************************************
Feature Tutorials
************************************************


.. toctree::
   :maxdepth: 3
   :caption: Feature Tutorials
   :hidden:

   Feature Tutorials <./docs/Feature_Tutorials/Feature_Tutorials>







.. csv-table:: Feature Tutorials
   :header: "Tutorial","Board / Platform","Kernel","XRT APIs","Libraries Used","Highlighted Features","GUI Flow"
   :widths: 35, 10, 10, 10, 10, 15, 10

   ":doc:`Getting Started with RTL Kernels <./docs/Feature_Tutorials/01-rtl_kernel_workflow/README>`","U200","C/C++","Native","","RTL kernel","Yes "
   ":doc:`Mixing C and RTL <./docs/Feature_Tutorials/02-mixing-c-rtl-kernels/README>`","U200","RTL + C/C++","OpenCL","","Mixed C++ and RTL kernel"," "
   ":doc:`Dataflow Debug and Optimization <./docs/Feature_Tutorials/03-dataflow_debug_and_optimization/README>`","HLS Part","C/C++","HLS","","HLS design and analysis","Yes "
   ":doc:`Using Multiple DDR Banks <./docs/Feature_Tutorials/04-mult-ddr-banks/README>`","U200","C/C++","Native","","DDR bank assignment"," "
   ":doc:`Using Multiple Compute Units <./docs/Feature_Tutorials/05-using-multiple-cu/README>`","U200","C/C++","OpenCL","","Multiple Compute Units"," "
   ":doc:`Controlling Vivado Implementation <./docs/Feature_Tutorials/06-controlling-vivado-implementation/README>`","U200","C/C++","OpenCL","","Use Vivado in the Vitis flow"," "
   ":doc:`Optimizing for HBM <./docs/Feature_Tutorials/07-using-hbm/README>`","U200 / U50","C/C++","OpenCL","","HBM and RAMA IP"," "
   ":doc:`Host Memory Access <./docs/Feature_Tutorials/08-using-hostmem/README>`","U250","C/C++","OpenCL","","Use Host Memory"," "
   ":doc:`Using GT Kernels and Ethernet IPs on Alveo <./docs/Feature_Tutorials/09-using-ethernet-on-alveo/README>`","U200","RTL","Native","","Ethernet Subsystem"," "
   ":doc:`Enabling FPGA to FPGA P2P Transfer using Native XRT C++ API <./docs/Feature_Tutorials/10-p2p-card-to-card/README>`","U50 / U200","C/C++","Native","","PCIe P2P"," "






************************************************
Design Tutorials
************************************************


.. toctree::
   :maxdepth: 3
   :caption: Design Tutorials
   :hidden:

   Design Tutorials <./docs/Design_Tutorials/Design_Tutorials>




.. csv-table:: Feature Tutorials
   :header: "Tutorial","Board / Platform","Kernel","XRT APIs","Libraries Used","Highlighted Features","GUI Flow"
   :widths: 35, 10, 10, 10, 10, 15, 10

   ":doc:`Convolution Example <./docs/Design_Tutorials/01-convolution-tutorial/README>`","U200","C/C++","OpenCL","Vision","Performance analysis and optimization","Yes "
   ":doc:`Bloom Filter Example <./docs/Design_Tutorials/02-bloom/README>`","U200","C/C++","OpenCL","","Performance analysis and optimization"," "
   ":doc:`RTL Systems Integration Example <./docs/Design_Tutorials/03-rtl_stream_kernel_integration/README>`","U50 / 55C / U200 / U250 / U280","RTL + C/C++","Native","Vision","Mixed kernel"," "
   ":doc:`Traveling Salesperson Problem <./docs/Design_Tutorials/04-traveling-salesperson/README>`","U200","C/C++","HLS","","HLS design and analysis","Yes "
   ":doc:`Bottom RTL Kernel Design Flow Example <./docs/Design_Tutorials/05-bottom_up_rtl_kernel/README>`","U50 / 55C / U200 / U250 / U280","RTL + C/C++","Native","","RTL kernel","Yes "
   ":doc:`Cheleskey Algorithm Acceleration <./docs/Design_Tutorials/06-cholesky-accel/README>`","U200","C/C++","OpenCL","","Performance analysis and optimization"," "
   ":doc:`XRT Host Code Optimization <./docs/Design_Tutorials/07-host-code-opt/README>`","U200","C/C++","OpenCL","","Host code optimization"," "
   ":doc:`Aurora Kernel on Alveo <./docs/Design_Tutorials/08-alveo_aurora_kernel/README>`","U50 / 55C / U200 / U250 / U280","RTL + C/C++","Native","","GT kernel"," "
   ":doc:`Single Source Shortest Path Application <./docs/Design_Tutorials/09-sssp-application/README>`","U50","C/C++","OpenCL","Graph","Vitis Library","Yes "
   ":doc:`Get Moving with Alveo <./docs/Design_Tutorials/10-get_moving_with_alveo/README>`","U200","C/C++","OpenCL","Vision","System Optimization",""