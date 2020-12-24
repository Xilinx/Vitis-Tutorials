<table>
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>Versal ACAP AI Engine A to Z Bare-metal Flow</h1>
   </td>
 </tr>

</table>

## Introduction

In this section of the tutorial, you will learn how to add PL kernels in HLS into the system project and build the whole system.

### Step 1. Modify the Graph for Use in Hardware Build

You now have a working application to be run on the AI Engine array. What you need now is to modify the AI Engine graph to be used in hardware and connect the AI Engine array to the PL using the Vitis compiler (V++).

1. Open the `project.cpp` file and replace the following line:
```
simulation::platform<1,1> platform("data/input.txt", "data/output.txt");
```
with

```
PLIO *in0 = new PLIO("DataIn1", adf::plio_32_bits,"data/input.txt");
PLIO *out0 = new PLIO("DataOut1",adf::plio_32_bits, "data/output.txt");
simulation::platform<1,1> platform(in0, out0);
```

2. The main function in `project.cpp` will not be used in the hardware run, so you need to add a switch (`#ifdef __AIESIM__`) so that main will not be taken into account for the hardware build.

3. Change the Active build configuration to Hardware and rebuild the project.

### Step 2. Add PL Kernels

In this example, HLS kernels are used which bridge between memory and the AXI4-Stream interface to input and output data from memory.
* The `mm2s` kernel reads data from memory and inputs it to the AI Engine array.
* The `s2mm` kernel receives output data from the AI Engine array and writes it to memory.

1. Open the Vitis 2020.2 IDE and select the same workspace as the AI Engine application project. Right-click the ***simple_application_system*** project and select ***Add Hw Kernel Project***.

2. Name the project ***hw-kernels*** and click ***Finish*** to create the project.

3. Right-click the ***hw-kernel project*** and click ***import sources***. Browse into the ***src*** folder and select the ***mm2s.cpp*** and ***s2mm.cpp*** files.

4. In the ***hw-kernels.prj*** page, click on the lightning icon (***Add HW*** function) icon and select both functions (`mm2s` and `s2mm`) as hardware functions.


### Step 3. Configure Hardware Linking Project

Now that you have imported the kernels, you need to tell the Vitis linker how to connect everything together.

1. In the ***simple_application_system_hw_link.prj*** page, enable ***Export hardware (XSA)***.

![](images/hw_link_cfg1.png)

2. Now you need to tell the Vitis compiler about the connectivity of the system. This step is done using a configuration file.
Create a `system.cfg` file with a text editor and add the following lines.
```
[connectivity]
stream_connect=mm2s_1.s:ai_engine_0.DataIn1
stream_connect=ai_engine_0.DataOut1:s2mm_1.s
```

Note that as per the [Versal ACAP AI Engine Programming Environment User Guide (UG1076)](https://www.xilinx.com/cgi-bin/docs/rdoc?t=vitis+doc;v=2020.2;d=yii1603912637443.html), the naming convention for the Compute Units (or kernel instances) are `<kernel>_#`, where # indicates the CU instance. Thus the CU names built corresponding to the kernels `mm2s` and `s2mm` in your project are respectively `mm2s_1` and `s2mm_1`.
The `stream_connect` option is defined as `<compute_unit_name>.<kernel_interface_name>:<compute_unit_name>.<kernel_interface_name>`.
For example, to connect the AXI4-Stream interface of the `mm2s_1` (compute unit name) called `s` (kernel interface name) to the `DataIn1` (interface name) input of the graph in the `ai_engine_0` (compute unit name) IP you need the following option: `stream_connect=mm2s.s:ai_engine_0.DataIn1`.

3. Right-click the ***simple_application_system_hw_link*** and click ***import sources***. Select the ***system.cfg*** file created and add it to the ***simple_application_system_hw_link*** folder.

      ![](images/hw_link_cfg2.png)

4. In the ***simple_application_system_hw_link.prj*** page, right-click the binary container and click ***Edit v++ options***.

      ![](images/hw_link_cfg3.png)

Add the following option to link your configuration file:
```
--config ../system.cfg
```

  ![](images/hw_link_cfg4.png)


### Step 4. Build the System

1. Select the ***simple_application_system*** project and click on the hammer icon to build it. The compilation process takes some time to finish. The underlying AI Engine application project, hardware kernel project, and hardware linking project are compiled one after another. The system should build successfully with no error.

      ![](images/system_build.png)

2. You can open the generated Vivado project in `<workspace>/simple_system_hw_link/Hardware/binary_container_1.build/link/vivado/vpl/prj` to take a look at the compilation result.
You can see that the Vitis compiler added the two HLS IP (`mm2s` and `s2mm`) and connected them to the memory (NoC) and to the AI Engine IP.

      ![](images/vivado_prj.png)
      ![](images/vivado_prj2.png)


In the next step, you will create a PS bare-metal application and run the system with it.


<p align="center"><b><a href="./02-aie_application_creation.md">Return to Step 2</a> — <a href="./04-ps_application_creation_run_all.md">Go to Step 4</a></b></p>


<p align="center">© Copyright 2020 Xilinx, Inc.</p>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

<p align="center"><sup>XD018</sup></p>
