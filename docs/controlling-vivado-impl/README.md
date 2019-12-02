<table>
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2019.2 Vitis™ Application Acceleration Development Flow Tutorials</h1>
   <a href="https://github.com/Xilinx/SDAccel-Tutorials/branches/all">See SDAccel™ Development Environment 2019.1 Tutorials</a>
   </td>
 </tr>
 <tr>
 <td align="center"><h1>Controlling Vivado Implementation</h1>
 </td>
 </tr>
</table>

# Introduction

The Vitis™ compiler creates kernel objects from the source code, links the kernels with the targeted shell, and runs the assembled design through the Vivado® tool implementation flows. It generates the platform file (xclbin) needed to program the FPGA-based acceleration cards. Sometimes, it is essential to use the advanced Vivado synthesis and implementation options to achieve your desired results, including timing closure.

# Tutorial Overview

This tutorial shows you how to control the Vivado tool flows to implement your project and guides you through the steps in the command line flow.

The Vitis core development kit provides two methods to control the Vivado tool flow:

1. When compiling or linking for a system build, you can pass Vivado synthesis and implementation options using config files.

   >**NOTE:** It can take hours to complete a system run.

2. If your application has already been compiled, then you can:

   * Optimize the design with the Vivado Design Suite.
   * Generate a new routed checkpoint.
   * Reuse this checkpoint to generate a new platform file (xclbin).

# Before You Begin

This tutorial uses:

* BASH Linux shell commands
* 2019.2 Vitis core development kit release and the *xilinx_u200_xdma_201830_2* platform.
If necessary, it can be easily ported to other versions and platforms.

>**IMPORTANT:**
>
>* Before running any of the examples, make sure you have installed the Vitis core development kit as described in [Installation](https://www.xilinx.com/html_docs/xilinx2019_2/vitis_doc/vhc1571429852245.html).
>* If you run applications on Xilinx® Alveo™ Data Center accelerator cards, ensure the card and software drivers have been correctly installed by following the instructions in the *Getting Started with Alveo Data Center Accelerator Cards Guide* ([UG1301](https://www.xilinx.com/cgi-bin/docs/bkdoc?k=accelerator-cards;v=latest;d=ug1301-getting-started-guide-alveo-accelerator-cards.pdf)).
>* Before running this lab, you must be familiar with the [Essential Concepts for Building and Running the Accelerated Application](/docs/Pathway3/) tutorial.

## Accessing the Tutorial Reference Files

1. To access the reference files, enter the following into a terminal: `git clone https://github.com/Xilinx/Vitis-Tutorials`.
2. Navigate to the `controlling-vivado-impl` directory, and then access `reference-files/run` directory.

# Using the Configuration Files to Configure Vivado Options

>**NOTE:** In this tutorial, run all instructions from the `reference-files/run` directory.

The `–-xp` switch is paired with parameters to configure the Vivado tools. For instance, the `--xp` switch  can configure optimization, placement, and timing, or set up emulation and compile options. In the command line flow, parameters are specified as `param:<param_name>=<value>`, where:

* `param`: Required keyword.
* `param_name`: Name of a parameter to apply.
* `value`: Appropriate value for the parameter.

In this section, you will use the following configurations as examples to see the flow to control Vivado synthesis and implementation:

* Completely flatten hierarchy during RTL synthesis (specify FLATTEN_HIERARCHY=full).
  * `--xp vivado_prop:run.my_rm_synth_1.{STEPS.SYNTH_DESIGN.ARGS.FLATTEN_HIERARCHY}={full}`

* Use the NoTimingRelaxation directive during the routing step (DIRECTIVE=NoTimingRelaxation).
  * `--xp vivado_prop:run.impl_1.{STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE}={NoTimingRelaxation}`

In this lab, `-–xp` command options are provided in an `design.cfg` file with each option specified on a separate line without the `--xp` switch.

For a full list of parameters and valid values, refer to the [Vitis Environment Reference Materials](https://www.xilinx.com/html_docs/xilinx2019_2/vitis_doc/yxl1556143111967.html).

1. Before running any of the examples, ensure you have set up the Vitis core development kit by running these commands:

   ```bash
   #setup Xilinx Vitis tools, XILINX_VITIS and XILINX_VIVADO will be set in this step. source <VITIS install path>/settings64.sh. for example:
   source /opt/Xilinx/Vitis/2019.2/settings64.sh
   #Setup runtime. XILINX_XRT will be set in this step
   source /opt/xilinx/xrt/setup.sh
   ```

2. Compile kernel with the Vitis compiler and link it into the platform file (xclbin) with the `--xp` command options.

   ```bash
   v++ -t hw --config design.cfg -c -k apply_watermark -o'apply_watermark.hw.xilinx_u200_xdma_201830_2.xo' '../src/krnl_watermarking.cl'
   v++ -t hw -s --config design.cfg -R2 -l -o'apply_watermark.hw.xilinx_u200_xdma_201830_2.xclbin' apply_watermark.hw.xilinx_u200_xdma_201830_2.xo
   ```

3. Open the `design.cfg` file and get familiar with different options. The following list describes some of the command options.

      >**Command Options Descriptions**
      >
      >* `-t hw`: Target system run
      >* `platform=xilinx_u200_xdma_201830_2`: Target the xilinx_u200 platform
      >* `debug=1`: Generate debug info
      >* `max_memory_ports=apply_watermark`: Generate one AXI4 interface for one port
      >* `-c`: Compile the kernel
      >* `-k apply_watermark`: Name the kernel
      >* `../src/krnl_watermarking.cl`: Specify source files
      >* `sp=apply_watermark_1.m_axi_gmem0:DDR[0]`: Specify DDR bank connection
      >* `--config design.cfg`: Set the Vivado Synthesis and Implementation options
      >* `-R2`: Set Report Level to 2 to generate DCPs for each implementation step
      >* `-l`: Link the kernel
      >* `nk=apply_watermark:1:apply_watermark_1`: Specify kernel instance number and name

4. To check that the Vivado synthesis and implementation options are applied, open `_x/logs/link/vivado.log`, and search for *flatten_hierarchy* and *NoTimingRelaxation*. You will find following lines indicating that the options have taken effect:

   * **Command**: `synth_design -top pfm_dynamic -part xcu200-fsgd2104-2-e -flatten_hierarchy full -mode out_of_context`
   * **Command**: `route_design -directive NoTimingRelaxation`

# Optimizing with the Vivado Tool

If your application has already been compiled for a system run, you can optimize the implementation result with various Vivado tool techniques to achieve your goal, such as achieving timing closure. After optimization, write the routed design back to checkpoint file (DCP), and then reuse this checkpoint to finalize a new implementation.

>**NOTE:** Ensure that the Vitis core development kit and Vivado environment versions match between different iterations.

In the optimization stage, Vivado tool techniques apply. For information on Vivado tool usage and optimization, refer to the *Vivado Design Suite Implementation User Guide* ([UG904](https://www.xilinx.com/cgi-bin/docs/rdoc?v=latest;d=ug904-vivado-implementation.pdf)) and *UltraFast Design Methodology Guide for the Vivado Design Suite* ([UG949](https://www.xilinx.com/cgi-bin/docs/rdoc?v=latest;d=ug949-vivado-design-methodology.pdf)).

For example, you might run the Vivado tool in batch mode with a Tcl script, where the Tcl script reads DCP, does optimization, and writes routed DCP.

From the `/reference-file/run` folder, run the following command.

```bash
#make sure you have successfully setup Vitis/Vivado environments as previous part shows.
vivado -mode batch -source opt.tcl
```

The `opt.tcl` file displays the following:

```bash
#DCP files in different stages of Vivado have been written by v++ linker automatically with option "-R2"
open_checkpoint ./_x/link/vivado/vpl/prj/prj.runs/impl_1/pfm_top_wrapper_routed.dcp
#Run post-route physical optimization
phys_opt_design -directive AggressiveExplore
write_checkpoint -force ./_x/link/vivado/routed.dcp
```

# Reuse Checkpoint to Generate Platform File (xclbin)

Next, reuse the `routed.dcp` checkpoint file generated by the `opt.tcl` script to generate a new platform file (xclbin). To do this, add the `--reuse_impl` option to the `v++` command.

```bash
v++ -t hw --config design.cfg -l -o'apply_watermark.hw.xilinx_u200_xdma_201830_2.xclbin' apply_watermark.hw.xilinx_u200_xdma_201830_2.xo --reuse_impl ./_x/link/vivado/routed.dcp
```

From the message in the console, you can see that the steps to generate the implemented design have been skipped, and the bitstream generation starts.

```bash
INFO: [VPL 60-251]   Hardware accelerator integration...
Starting FPGA bitstream generation.
```

# Putting it All Together

The following section summarizes all the steps to control Vivado tools implementation with the `v++` command, optimize design with the Vivado tools, and reuse implemented dcp to generate the platform file. You might use it as a quick reference for commands.

1. Set up the core development kit and working directory.

   ```bash
   #setup Xilinx Vitis tools, XILINX_VITIS and XILINX_VIVADO will be set in this step. source <Vitis install path>/settings64.sh. for example:
   source /opt/Xilinx/Vitis/2019.2/settings64.sh
   #Setup runtime. XILINX_XRT will be set in this step
   source /opt/xilinx/xrt/setup.sh
   ```

2. Generate the platform file (xclbin) with `v++` commands.  

   ```bash
   v++ -t hw --config design.cfg -c -k apply_watermark -o'apply_watermark.hw.xilinx_u200_xdma_201830_2.xo' '../src/krnl_watermarking.cl'
   v++ -t hw -s --config design.cfg -R2 -l -o'apply_watermark.hw.xilinx_u200_xdma_201830_2.xclbin' apply_watermark.hw.xilinx_u200_xdma_201830_2.xo
   ```

3. Optimize the design with the Vivado tool and rewrite the routed DCP.

   ```bash
   vivado -mode batch -source opt.tcl
   ```

4. Reuse the routed DCP to generate the platform file (xclbin).

   ```bash
   v++ -t hw --config design.cfg -l -o'apply_watermark.hw.xilinx_u200_xdma_201830_2.xclbin' apply_watermark.hw.xilinx_u200_xdma_201830_2.xo --reuse_impl ./_x/link/vivado/routed.dcp
   ```

5. This tutorial has provided host code to make it complete to run though various flows. The following commands are references to build the host and run the application.

   ```bash
   #Generate the host executable.
   g++ -I$XILINX_XRT/include/ -I$XILINX_VIVADO/include/ -Wall -O0 -g -std=c++11 -L$XILINX_XRT/lib/ -lOpenCL -lpthread -lrt -lstdc++ -o 'host' '../src/host.cpp'
   #Please set correct XCL_EMULATION_MODE manually if running sw_emu and hw_emu modes
   ./host apply_watermark.hw.xilinx_u200_xdma_201830_2.xclbin ../src/inputImage.bmp ../src/golden.bmp
   ```

# Conclusion

This tutorial illustrated how to use the `v++` command `--xp` option to control Vivado synthesis and implementation, using a simple example to optimize the design with the Vivado tool. It then showed the steps to reuse implemented DCP to generate the platform file (xclbin), skipping the intermediate steps to generate the implemented design.

To run through all flows, the host code and its building commands are also provided for reference.

</br>
<hr/>
<p align= center><b><a href="/README.md">Return to Main Page</a></b></p>
<p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>
