<table class="sphinxhide" width="100%">
 <tr width="100%">
   <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Platform Creation Tutorials</h1>
   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# Vitis Export To Vivado

***Version: Vitis 2023.1***

## Introduction
The Vitis export to Vivado is the new feature release in  Vivado 2023.1 / Vitis 2023.1 suite which enables bi-directional hardware hand-offs between the Vitis tools and the Vivado design suite to improve developer productivity. User can do the hardware design development in the vivado design suite and use Vitis tool to do the software development like, XRT, AIE development, PL kernels development in the Vitis tool. Earlier to this release, for any change in the hardware design user needs to export the extensible xsa and go to Vitis environment and carry out the further development flow in the Vitis. This flow provides the flexibility to the user to do the hardware design development in the Vivado design which includes h/w design development, synthesis, implementation and timing closure in Vivado and other development tasks like XRT, AIE development, application development, HLS kernel development etc in the Vitis tool. In 2023.1, the flow supports hardware development only (i.e. TARGET=HW).

The tutorial describes about the flow which starts from the Vivado Design Suite. The hardware design to be developed in the Vivado Design and export extensible hardware platform (.xsa) from the Vivado Design Suite that forms the basis of many differentiated Vitis hardware applications. The extensible platform is imported in to Vitis environment to perform development of AI Engine Graph application and additional PL kernels for the system design. Compile AIE graph and PL kernels. Link the compiled output, extensible platform to export VMA during the v++ linking phase using the new v++ switch. Import the VMA file into the Vivado by new TCL API and later design development work can be continued in Vivado design. Design flow section shared the complete design flow. 

To use Vitis export to Vivado flow, new v++ switch and 3 TCL APIs are introduced. The tutorial helps user to understand the following new switch and TCL APIs in the flow:

The new v++ switch introduced in this flow to export Vitis Metadata is:
```bash
1. v++ -l --export_archive
```
New Vivado TCL APIs introduced in this flow to import and remove the VMA from Vivado are:
```bash
2. vitis:: import_archive
3. vitis:: remove_archive_hierarchy
4. vitis:: remove_archive
```
<B>IMPORTANT:</B>Before beginning the tutorial make sure you have installed the Vitis 2023.1 and Vivado 2023.1 software. 

This tutorial demonstrates the hardware design development using TCL based flow. User can try any other design methodology which supports BDC (Block Design Container) as the flow supports BDC based designs only. 


## Objectives
After completing the tutorial, user would be able to do the following:
 1. Export the extensible platform (.xsa) from the hardware design developed in the Vivado Design Suite.
 2. PL Kernel development and compilation in the Vitis environment.
 3. AIE graph development and compilation in the Vitis Environment.
 4. Export Vitis-Metadata-Archive (VMA) using the new v++ command --export_archive.
 5. Import VMA in Vivado design suite using the new TCL API.
 6. Generate fixed.xsa


## DESIGN FILES
--------------

1. TCL files used to create Vivado project is in the `vivado_impl` directory:

[Flat.tcl](./vivado_impl/flat.tcl)

[create_bdc.tcl](./vivado_impl/create_bdc.tcl)

[flat_pre_bdc.tcl](./vivado_impl/flat_pre_bdc.tcl)

2. AIE files for AIE development

[Graph.cpp](./aie/graph.cpp)

[Graph.h](./aie/graph.h)

[include.h](./aie/include.h)

[kernels.h](./aie/kernels.h)

3. PL_KERNELS for PL Kernel development

[mm2s.cpp](./pl_kernels/mm2s.cpp)

[polar_clip.cpp](./pl_kernels/polar_clip.cpp)

[s2mm.cpp](./pl_kernels/s2mm.cpp)

4. Configuration Files 

[system.cfg](./vitis_impl/system.cfg)


## DESIGN FLOW: 
This tutorial covers two use cases:
1. Modification(s) in the hardware design after the VMA import (second iteration onwards) is in Vivado only. No change affecting the AIE-PL boundary.
2. Modification(s) are related to the AIE-PL boundary and Vitis development after the VMA import (second iteration onwards). In this use case, modifications related to vivado are also supported. 

The complete flow (from hardware design creation to export .xclbin) is divided into seven steps as shown in below figure:

![Flow_Diagram](./images/flow_dig.png)

### Use Case 1 : Vitis Export to Vivado flow with changes related to Vivado only after importing VMA.

#### Step-1: To create the hardware platform using the TCL flow. TCL files are shared in vivado_impl folder.

##### Command to generate the vivado project through TCL and export xsa

```bash
vivado -mode batch -source ./vivado_impl/flat.tcl
```
OR use the make command to generate xsa.
```bash
make xsa
```
Output: Generated Extensible XSA is located in the director `vivado_impl/flat_wa.xsa` and `flat` folder contains vivado project flat.

#### Step-2: After exporting XSA, need to generate libadf.a and PL Kernels (.xo) before running the v++ linker to export vma file.

##### 2.1: Compile AIE graph and generate libadf.a

Command to execute 2.1 :
```bash
v++ -c --mode aie --platform ../vivado_impl/flat_wa.xsa ./aie/graph.cpp 
```
OR use the make command to generate AIE compiled output
```bash
make adf
```

Output: Generated `libadf.a` is located in the directory `aie\libadf.a`

##### 2.2: Compile PL kernels to generate .xo file

Command to execute 2.2:

We are using 3 PL kernels. In this step, generate xos for all 3 kernels. Please find the details below:

1. Generate kernel mm2s.xo 
```bash
v++ -c -k mm2s -f ./vivado_impl/flat_wa.xsa -s -o mm2s.xo ./pl_kernels/mm2s.cpp 
```

2. Generate kernel polar_clip.xo
```bash
v++ -c -k polar_clip -f ./vivado_impl/flat_wa.xsa -s -o polar_clip.xo ./pl_kernels/polar_clip.cpp
```

3. Generate kernel s2mm.xo
```bash
v++ -c -k s2mm -f ./vivado_impl/flat_wa.xsa -s -o s2mm.xo ./pl_kernels/s2mm.cpp
```

OR

All 3 XOS can be generated using the make command:
```bash
make xos
```

Output: Generated .xo file are located in the directory `pl_kernels\mm2s.xo`,  `pl_kernels\polar_clip.xo` and `pl_kernels\s2mm.xo`

##### 2.3: Export `vma` file using the v++ linker using the switch `--export_archive` and linking the extensible.xsa, libadf.a, xos and system.cfg:

Command to export vma:
```bash
v++ -l --platform ../vivado_impl/flat_wa.xsa  ../pl_kernels/*.xo ../aie/libadf.a --save-temps --export_archive --config system.cfg -o vitis_impl/flat_hw.vma
```
Here, *.xo represents all PL kernel.xo files and path

OR use the make command to generate vma: 

```bash
make vma
```
Output: Generated .vma file in `vitis_impl`

#### Step-3: Import vma file in the vivado project 

1. Open the vivado project from the project directory `./vivado_impl/flat/flat.xpr`.

![Vivado-1](./images/Step_3_1.png)

2. From the TCL console, use the TCL API to import the VMA: vitis::import_archive ./vitis_imple/flat_wa.xsa

![TCL-1](./images/Step_3_2.png)

3. After running TCL API to import the VMA, the modified design shall have ExtensibleRegion_VMA.bdc as shown below:

![image](./images/Step_3_3.png)

4. ExtensibleRegion_vma.bd shall be seen as:

![image](./images/Step_3_4.png)

#### Step-4: Changes related to Vivado design Only
Review of the VMA section can be done here. The bdc shows which kernels are used in the Vitis region and connections. User can review PL-AIE connection also as per the design requirement.
In this step, developer is free to do all design analysis and modification as doing before in vivado. Please refer the [UG1393](https://docs.xilinx.com/search/all?query=Vitis+Unified+Software+Platform+Documentation%253A+Application+Acceleration+Development+(UG1393)&content-lang=en-US) to understand the design guidelines and limitations under section "Vitis Export Flow Guidelines and Limitations"

<B> Note: Any changes related to imported VMA BDC can only be done through Vitis. It is read only. To perform any changes in the Vivado, enusre that latest VMA is imported into the Vivado.</B>

<I> To prgress the vivado changes, user needs to ensure that latest VMA is imported into the Vivado because latest VMA is imported with the exported XSA. If any changes are done after the exporting XSA and before importing VMA will be lost as after importing the VMA new *_vma.bdc will be generated. That bdc carries the vivado design which was exported in the XSA. </I>

#### Step-5: If there are no changes required in the Vitis Environment, this step can be skipped. Refer to Use Case 2, for more details. 

#### Step-6: Generate fixed xsa.

In this step, user shall run the design through implementation run and timing closure. User is free to make all design changes as mentioned in step-4 and changes in the timing constraint is allowed to meet timing. If user faces any implementation and timing violation or optimization issues, resolve it in this step and take the design to closure as before. After design closure in vivado is done, fixed.xsa can be generated by using the TCL API: `write_hw_platform -fixed ./vivado_impl/flat_fixed.xsa`

![image](./images/step_6.png)

Output: Fixed xsa `flat_fixed.xsa` is in the folder `vivado_impl`

#### Step-7: Generate XCLBIN
Like previous flows, fixed.xsa can be used for many purposes like application development running on Petalinux/Yocto or XRT or baremetal flows. Later, generate the xcilbin using the v++  package command:
`v++ -p -t hw --platform ./vivado_impl/flat_fixed.xsa -o flat_hw.xclbin ./aie/libadf.a`
This step will generate .xclbin without having any software details like rootfs etc.
To run the design on hardware, user need to add the software details while packaging. Please refer the Building and Pacakging section in [UG1393](https://docs.xilinx.com/search/all?query=Vitis+Unified+Software+Platform+Documentation%253A+Application+Acceleration+Development+(UG1393)&content-lang=en-US)


### Use Case 2 : Vitis Export to Vivado flow with changes related to Vitis + Vivado after importing VMA.

This use case covers the scenario when user wants to make changes in the Vivado and Vitis. For making the changes in the Vivado, user can progress as shared in step-4 of usecase-1. For changes to be done in Vitis, import the latest VMA into Vivado as shared in step-5:

#### Step-5: Changes related to Vitis design 
This step supports any modification in the Vitis Region design which may or may not impact the boundary of the PL-AIE region, developer shall remove the imported VMA from the existing Vivado Design. To remove the imported VMA from Vivado design, following two commands can be used:

1. vitis::remove_archive_hierarchy
2. vitis::remove_archive   

In this use case, it is considered that there is/are change(s) in the Vitis design. To execute the change, need to remove the imported vma from the vivado project. Follow below steps to execute that:
1. Remove the imported vma using the TCL API `vitis::remove_archive_hierarchy` to remove the imported vma only. Implement the changes in the vivado, repeat step 1 to export the updated xsa. Repeat step-2 to use the latest xsa to link updated AIE compiled output, PL kernels output and system.cfg. Export the updated vma. Repeat step-3, to import the modified vma in the design.

In this example, we plan to add one more polar clip IP in the Vitis region. Since .xo has been already generated in previous steps (Use Case-1), we can directly modify connections in the system.cfg file as shown below:

![image](./images/step_9.png)


In the above system.cfg, we have added one more polar_clip kernel. The new polar_clip_2 kernel is concatenated with the formal polar_clip_1 kernel. 

After using `vitis::remove_archive_hierarchy` command, the updated bd will look as shown below. In which added PL IPs and connections are intact and only vma is removed.

![image](./images/Step-10.png)

After Running the command, the modified VMA region is shown as:

![image](./images/Step-11.png)

2. Remove the imported vma using the TCL API `vitis::remove_archive` to remove the imported vma with changes done in vivado after vma import. After this command, user need to start from step 1, export extensible.xsa again with hardware changes and repeat the steps as shared in use case 1. The use of this command helps the developer in case there is a need to knock off all the changes or configurations done during the hardware development in Vivado and start from the last VMA import (in case, considering multiple VMA are imported). 

Repeat step 6 and 7 from use case -1 to generate fixed.xsa and xclbin files.

## Summary:

In this tutorial, user will learn the following after completing the tutorial:
 1. Start the design in Vivado using BDC methodology and export extensible.xsa.
 2. Compilation of AIE graph and PL kernels xo.
 3. Link the compiled output, system.cfg, extensible.xsa and export Vitis Metadata Archive.
 4. Import Vitis Metadata into Vivado and progress the platform development in Vivado.
 5. Generate Fixed.xsa from Vivado.

To read more about the flow [UG1393](https://docs.xilinx.com/search/all?query=Vitis+Unified+Software+Platform+Documentation%253A+Application+Acceleration+Development+(UG1393)&content-lang=en-US) (Chapter-19: Managing Vivado Synthesis,Implementation, and Timing Closure).

## Support:
GitHub issues will be used for tracking requests and bugs. For questions go to support.xilinx.com.

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>








