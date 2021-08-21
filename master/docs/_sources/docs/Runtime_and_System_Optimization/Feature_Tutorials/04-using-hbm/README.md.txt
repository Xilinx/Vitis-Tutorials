<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2021.1 Vitis™ Application Acceleration Development Flow Tutorials</h1>
   <a href="https://github.com/Xilinx/Vitis-Tutorials/tree/2020.2">See 2020.2 Vitis Application Acceleration Development Flow Tutorials</a>
   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# Introduction
This tutorial covers essential steps on migrating an existing application using DDR memory to HBM memory.  The tutorial begins by explaining the structural differences between DDR and HBM, as well as achievable theoretical bandwidth. The second section uses a simple example with necessary steps to migrate an application from DDR to HBM.

## Before You Begin

>**TIP**: This tutorial takes approximately two hours to complete.

The labs in this tutorial use:

* BASH Linux shell commands.
* 2020.2 Vitis core development kit release and the *xilinx_u200_xdma_201830_2* and *xilinx_u50_gen3x16_xdma_201920_3* platforms. If necessary, it can be easily ported to other versions and platforms. Its also expected that you have a Xilinx® Alveo™ Data Center accelerator card that supports DDR and HBM to run this tutorial.

>**IMPORTANT:**
>
>* Before running any of the examples, make sure you have installed the Vitis core development kit as described in [Installation](https://www.xilinx.com/html_docs/xilinx2020_2/vitis_doc/acceleration_installation.html#vhc1571429852245) in the Application Acceleration
Development flow of the Vitis Unified Software Platform Documentation (UG1416).
>* If you run applications on Xilinx® Alveo™ Data Center accelerator cards, ensure the card and software drivers have been correctly installed by following the instructions on the [Alveo Portfolio page](https://www.xilinx.com/products/boards-and-kits/alveo.html).


### Accessing the Tutorial Reference files

1. Enter the command in a terminal: `git clone http://github.com/Xilinx/Vitis-Tutorials`.
2. Navigate to `Runtime_and_System_Optimization/Feature_Tutorials/04-using-hbm` directory, and then access the `reference-files` directory.

### Generating all the xclbins

1. The tutorial provides the results as you go through. But if you intend to generate the xclbins, this can take several hours to generate the multiple `xclbin`. Its recommended to build all of these upfront so that you can focus on tutorial without waiting on these builds. The following steps will build the necessary xclbins for next two modules.
2. Update the `PLATFORM` variable in the `makefile/Makefile` as shown below with the platform intended to be used.
    ```
    ifeq ($(memtype), DDR)
    PLATFORM :=xilinx_u200_gen3x16_xdma_1_202110_1
    else
    PLATFORM :=xilinx_u50_gen3x16_xdma_201920_3
    endif
    ```
3. Run the following command to build three xclbins for module 2 (Migration from DDR to HBM). The expected time to build xclbins is about 7-8 hours or you can run these in parallel.

    ``` cd 04-using-hbm/makefile;  make ddr_addSeq_build  hbm_addSeq_2Banks_build hbm_addSeq_overlap_build```

4. Run the following command to build ten xclbins for module 3 (HBM Bandwidth Results). The expected time to build xclbins is about 5-6 hours since the following command will run parallel jobs by default

    ``` cd 04-using-hbm/makefile;  make build_without_rama build_with_rama```

    If your machine doesn't have enough resources to run multiple build jobs in parallel, you can always run these serially using the following commands but it may take about several hours.

    ```cd 04-using-hbm/makefile;  make noramajob-64 noramajob-128 noramajob-256 noramajob-512 noramajob-1024 ramajob-64 ramajob-128 ramajob-256 ramajob-512 ramajob-1024;```

Please refer to the ./makefile/Makefile for more information.

## Tutorial Overview

1. [HBM Overview](1_overview.md): Provides a brief overview on the structural differences between DDR and HBM and theoretical maximum bandwidth by HBM.

2. [Migration from DDR to HBM](2_Migrating_to_HBM.md): Walks through the steps of migrating an existing DDR based application to HBM

3.  [HBM Bandwidth Explorations](3_BW_Explorations.md): Shows the achievable bandwidth using a single kernel port based on access pattern and transaction size.
###



<p align="center"><b>
Start the next step: <a href="1_overview.md"> HBM Overview
</b></p>
</br>
<hr/>
<p align="center"><b><a href="docs/README.md">Return to Start of Tutorial</a></b></p>

<p align="center"><sup>Copyright&copy; 2020-2021</sup></p>




