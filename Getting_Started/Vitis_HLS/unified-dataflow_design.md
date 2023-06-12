   <table class="sphinxhide" width="100%">
 <tr>
   <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Application Acceleration Tutorials</h1>

   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>


# 4. Reviewing the DATAFLOW Optimization

In the earlier steps, you found different ways to optimize the DCT algortithm so that you could achieve an II=1 with the pipelined loops. In this step, you use the DATAFLOW directive to enable task-level parallelism for functions or loops. For more information, refer to [set_directive_dataflow](https://docs.xilinx.com/r/en-US/ug1399-vitis-hls/set_directive_dataflow) in the Vitis HLS flow.

The DATAFLOW optimization tries to create task-level parallelism between the various functions in the code, on top of the loop-level parallelism where possible.

## Clone Component for Design Exploration

The Vitis Components Explorer provides the **Clone Component** command, which lets you create a new component based on the current state and configuration of an existing component. The clone does not include prior results or reports. 

 ![Clone Dataflow](./images/unified-hls-clone-dataflow.png)

1. Right-click the `dct` component in the Vitis Components Explorer and select **Clone Component**. 

You can specify a new component name to create a clone of the selected component. 

2. After specifying a name, select **OK** to clone the component. 
 
**TIP:** The rule of thumb here would be to clone a component before embarking on some optimization experiment and make your changes in the clone. 

## Adding the Dataflow Directive

3. Add the `Dataflow` directive to the top-function of the design. In the Config File editor, select the Dataflow directive and add `dct`, or edit the `hls_config.cfg` file and add the following line: 
```syn.directive.dataflow=dct```

4. Rerun synthesis and review the reports. 

You should notice the addition of the Dataflow Viewer report as a result of adding the Dataflow directive. However, you need to run C/RTL Co-simulation in order to get greater information from the dataflow design. Examine the Dataflow Viewer report prior to running Co-simulation, and then again after. First you should configure the Co-simulation to perform as you want. 

5.  Open the Config File editor, an select the **C/RTL Co-Simulation** category on the left side. The options for Co-simulation are displayed on the right. 

6.  Select `enable_dataflow_profiling` to turn on the feature.
7.  Select `enable_fifo_sizing` to turn on this feature. 
8.  Select **Run** under C/RTL Co-Simulation in the Flow Navigator. 
 
The DATAFLOW optimization is a dynamic optimization that can only really be understood after C/RTL co-simulation which provides needed performance data. After co-simulation the columns of the Process table are populated with values from the simulation results. 

 ![Dataflow Report](./images/unified-hls-dataflow-report.png)

## View the Dataflow Graph

The Dataflow Viewer graph illustrates the overall topology of the DATAFLOW region and shows what type of channels (FIFO/PIPO) were inferred for communication between the tasks in the DATAFLOW region. Analyzing each channel and process can be useful to resolve issues such as deadlock or poor throughput because of bad FIFO sizing. The icons at the top-right of the Dataflow viewer provide a legend for the diagram and access to settings. 

The DataFlow Viewer displays the functions and the flow through the functions. After running C/RTL co-simulation, the elements of the graph are filled out with performance data, and the Process and Channel tables beneath the graph are also filled out. Without the performance data provided by co-simulation, the graph and tables will have NA values to reflect the missing values. For more information, refer to [Dataflow Viewer](https://docs.xilinx.com/r/en-US/ug1399-vitis-hls/Dataflow-Viewer).

The Dataflow Viewer enables the following throughput analysis options:
* The graph shows the overall topology of the DATAFLOW region and shows what type of channels (FIFO/PIPO) were inferred for communication between the tasks in the DATAFLOW region. Analyzing each channel and process can be useful to resolve issues such as deadlock or poor throughput due to bad FIFO sizing. 
* The co-simulation data helps with the FIFO sizing problem by tracking the maximum size of the FIFO during the course of the simulation and thereby giving the user a good reference point on how to size the FIFOs. In addition, when running co-simulation, automatic deadlock detection can highlight the processes and channels involved in the deadlock allowing the user to quickly narrow the focus and fix the issue. 
* In addition to FIFO sizing, the data reported after co-simulation also provides, on a per process and channel basis, the time spent stalling either waiting for input or blocked from writing output. The graph helps the user understand such issues and manage how the channels are sized to accommodate slow producers versus fast consumers and/or vice-versa. In addition, the graph is useful in understanding how reading from the input in the middle of a DATAFLOW region can impact performance. This is a fairly common scenario where performance can be impacted.  

## Export the Vitis Kernel

Finally, you can export the results of the high-level synthesis as a synthesized Vitis kernel (`.xo`) file or Vivado IP (`.zip`) for use in downstream processes.

1. In the Config File Editor, select the **C-Synthesis** heading and set the `output.format` to either `Generate Vivado IP` or `Generate a Vitis XO`. 
2. In the Flow Navigator select **Run** under C Synthesis.

This time the IP or kernel is exported from the design. The green circle no longer shows the yellow warning triangle. An output has been generated. 

## Summary

In this tutorial:

1. You worked to optimize C/C++ code in the Vitis HLS tool, to synthesize it into RTL code for use in the Vitis application acceleration development flow. 
2. With the code optimized, you exported the compiled kernel object (`.xo`) file for use in a Vitis application project. 

These are the elements of building and accelerating applications and functions using the Vitis and Vitis HLS tools. You can mix the Vitis HLS kernels with RTL kernel objects (`.xo`) and uncompiled C/C++ kernel code (`.c`/`.cpp`) in the Vitis application project to create more complex accelerated applications. 

You have the optimized but uncompiled C++ code (`dct.cpp`) that you saved in an earlier step of this tutorial. You can remove the HLS kernel object and add this optimized C++ code in the Vitis application project if you prefer. In this case, the Vitis IDE will call Vitis HLS as part of the build process when compiling the C++ kernel code.  

</br>
<hr/>
<p align="center" class="sphinxhide"><b><a href="/README.md">Return to Main Page</a> — <a href="./README.md">Return to Start of Tutorial</a></b></p>


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
