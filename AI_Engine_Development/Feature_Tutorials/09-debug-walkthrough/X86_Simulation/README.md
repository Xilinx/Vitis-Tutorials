<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# X86 Simulation Debug Walkthrough

## Introduction

The x86simulator supports faster simulation to help verify the kernel's functionalities. It applies systemC model instead of RTL model to achieve faster build and run time. The Hardware constraints such as heap/stack sizes and program memory size are not verified in software emulator.

Before starting this tutorial, 

* It is expected that you have run the steps to set the environment variables as described in [Introduction](../README.md#Introduction)
* Create a system project manually using the steps mentioned in [Port command line project to Vitis IDE system project](../CreateIDESystemProj.md) or download the Vitis exported project as instructed in [Download Vitis IDE project](../README.md#Download-Vitis-IDE-project)
* Open the Vitis IDE tool by typing `vitis` in the console and set the workspace and the IDE opens. Click on `File->Import` and select the `Vitis exported project zip file` and browse to the above zip file and click `Next`. Enable the check box `System Projects` and click `Finish`.

## Features

<table style="width:100%">

<tr>
<td>
 <a href="./README.md#Build-and-simulate-in-Vitis-IDE">Build and simulate in Vitis IDE</a>
</td>
<td>
Demonstrates how to use Vitis IDE to build and simulate AI Engine design
</td>
</tr>

<tr>
<td>
<a href="./README.md#Debug-using-printf">Debug using printf()</a>
</td>
<td>
Demonstrates how to add formatted printf() to print debug messages
</td>
</tr>

<tr>
<td>
<a href="./README.md#Debug-using-printf-with-Vector-datatypes">Debug using printf() with Vector datatypes</a>
</td>
<td>
Demonstrates how to print vector output data value via printf()
</td>
</tr>

<tr>
<td>
<a href="./README.md#Debug-using-Vitis-IDE-debugger">Debug using Vitis IDE debugger</a>
</td>
<td>
Demonstrates how to use Vitis IDE debugger to debug AI Engine design
</td>
</tr>
 
<tr>
<td>
<a href="./README.md#x86simulator-options-for-debugging">x86simulator options for debugging</a>
</td>
<td>
Demonstrates how to use the x86simulator options file for debugging
</td>
</tr>

<tr>
<td>
<a href="./README.md#Data-dump">Data dump</a>
</td>
<td>
Demonstrates how to use data dump feature with practical scenario
</td>
</tr>
 
<tr>
<td>
<a href="./README.md#Deadlock-detection">Deadlock detection</a>
</td>
<td>
Demonstrates how to debug deadlock scenarios in x86simulation
</td>
</tr>

<tr>
<td>
<a href="./README.md#Trace-report-in-file">Trace report in file</a>
</td>
<td>
Demonstrates how to visualize the trace report in file
</td>
</tr>
                                          
<tr>
<td>
<a href="./README.md#Trace-report-in-output-console">Trace report in output console</a>
</td>
<td>
Demonstrates how to visualize the trace report in output console during run-time
</td>
</tr>

<tr>
<td>
<a href="./README.md#Memory-access-violation-and-valgrind-support">Memory access violation and valgrind support</a>
</td>
<td>
Demonstrates how to debug memory access violations in AI Enigne design using Valgrind support
</td>
</tr>

<tr>
<td>
<a href="./README.md#Using-GDB-debugger-in-command-line">Using GDB debugger in command line</a>
</td>
<td>
Demonstrates about debugging in command-line using GDB debugger
</td>
</tr>
 
<tr>
<td>
<a href="./README.md#x86simulation-on-command-line">x86simulation on command line</a>
</td>
<td>
Demosntrates how to run x86simulation on command line
</td>
</tr>
 
<tr>
<td>
<a href="./README.md#x86simulation-with-GDB">x86simulation with GDB</a>
</td>
<td>
demonstrates how to use GDB duirng x86simulation
</td>
</tr>
 
<tr>
<td>
<a href="./README.md#x86simulator-Using-GDB-server">x86simulator Using GDB server</a>
</td>
<td>
Demonstrates how to use GDB server to debug design.
</td>
</tr>

</table>

### Section - 1 
## Build and simulate in Vitis IDE

a) In the Vitis IDE -> tool bar, click on the arrow next to manage configurations button and select the 'Emulation-SW' as shown below. This sets the build target for x86 simulation.
><img src="./Images/Build_target.PNG" width="600" height="300">                                                      
b) Right-click on the *[aiengine]* domain project and select the **C/C++ Build Settings** , choose **Compiler** and add `-O0` to the Pre Processor Options. This improves the debug visibility.
><img src="./Images/Build_setting.PNG" width="500" height="300"> 
c) Now right-click on the *[aiengine]* domain project and select **Build** option. Once the build completes, you see the **Compilation Complete** and **Build Finished** messages in console.<br />
d) Right-click on the *[aiengine]* domain project and select **Run As → Launch SW Emulator** option to start simulation for x86simulation target. When the simulation completes you see the below output in the console.<br />
><img src="./Images/x86simulator_output.PNG" width="350" height="110">     

e) X86simulator output files from design are located at `${PROJECT}/Emulation-SW/x86simulator_output/data`. Verify the output files `${PROJECT}/Emulation-SW/x86simulator_output/data/output_upscale.txt` and `${PROJECT}/Emulation-SW/x86simulator_output/data/output_data_shuffle.txt` against golden files `${PROJECT}/data/golden_upscale.txt` and `${PROJECT}/data/output_data_shuffle.txt` to ensure that the design's I/O functionalities are correct. Vitis IDE supports compare with feature to compare two files, highlight two files to be compared then right click one of highlighted file and select **compare with** then **Each other**. For example, Compare `${PROJECT}/data/golden_upscale.txt` and `${PROJECT}/Emulation-SW/data/output_upscale.txt`.

                                                                     
### Section - 2
## Debug using printf

The simplest form of tracing is to use a formatted `printf()` statement in the code for printing debug messages. Visual inspection of intermediate values and addresses can help you understand the progress of program execution. You can add `printf()` statements to your code to be processed during x86 simulation and remove them or comment them out for hardware builds. To help identify which kernel is printing which line the `X86SIM_KERNEL_NAME` macro can be useful.

This section talks about adding `printf()` statement in the source code, compile and run `x86simulator` and check the output in console.

a) From Vitis IDE, browse to the *[aiengine]* domain project → expand **src** → **kernels** → **click** on peak_detect.cc.<br />
b) Add `#include <adf/x86sim/x86simDebug.h>` at the beginning of the source file and `printf("%s: %s, %d\n", __FUNCTION__, X86SIM_KERNEL_NAME, __LINE__);` after for loop.<br />
c) To Compile the project, Right click on *[aiengine]* domain project and select **'Build Project'**. <br />
d) To Run the project, **Right click** → **Run As** → **Launch SW Emulator**.<br />
e) The expected result is as shown below.
><img src="./Images/printf_support.PNG" width="650" height="400">
f) Remove the added printf statement from peak_detect.cc file, to use it for other features.

### Section - 3
## Debug using printf with Vector datatypes
x86simulator supports printing vector output data value via printf(). This section talks about visualizing vector values using x86simulator.

a) Add below lines in the peak_detect.cc 
```
int32_t* print_ptr = (int32_t*)&v_in;
for(int pp=0;pp<16;pp++)
    printf("Iteration-%d -> Vector-%d -> value = %d\n",i,pp,print_ptr[pp]);
```
b) Re-compile the project either by right-clicking on the project → **Build project**, or by hitting the *build* button in the task bar.<br />
c) Now, run the x86simulation and observe the print statements in the console as shown below.
><img src="./Images/vector_printf.PNG" width="630" height="400">


### Section - 4                                 
## Debug using Vitis IDE debugger

This section walks you through a debug methodology that uses Vitis IDE debugger. You can learn how to invoke the debugger, add breakpoints, view intermediate values e.t.c

a) After the design is built for x86simulation target, Right-click on the *[aiengine]* domain project and select **Debug as** → **Launch SW Emulator**.
b) This gets you to the debug mode in Vitis IDE and waits in graph.cpp file(after *main()* function).<br />
c) Now, open any source file from Explorer window and add the break-point of interest. For example, open the *peak_detect.cc* source file and add the break-point at line-26.<br />
d) Observe different debug functionalities/controls available. e.g. step-in, step-over, step-return, resume, terminate, disconnect e.t.c.<br />
><img src="./Images/debug_controls.PNG" width="300" height="250">
e) Press `F8` or `Resume` button in the tool bar. Observe the simulation stops at the user-defined break-point as shown below.<br />
><img src="./Images/break_point.PNG">
f) You can inspect the array value `v_in` `(aie::vector<int32,16>)` from Variables view. Double click the variables view to enlarge the area. You can restore it back to the original size by double-clicking again. Expand the variable `v_in` and continue expanding to  `(Vin → data → __elems_ → __elems_[0] → val → VBitDataBase<32,true,false> → data →  0 → [])` . If you click on that, it shows value '16' at the bottom (based on the iteration in your case). <br />
><img src="./Images/variables_view.PNG" width="600" height="400">
g) You can either continue stepping for all iterations or remove the break-point and hit **Run**button in taskbar. It completely runs the simulation for all iterations. Once you are done with debugging, you can click on disconnect button and switch back to Design mode.<br />

### Section - 5
## x86simulator options for debugging

The table below lists some x86simulator options which are used for debugging. For the complete set of x86simulator options refer to the UG1076 - [Simulator Options](https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment/Simulator-Options)

|Option |Description|
|:----|:----|
|--dump|Enables snapshots of data traffic on kernel ports.|
|--trace|Enables trace of kernel stall events.|
|--valgrind|Runs the simulator under Valgrind to detect access violations.|
 
All x86simulator supported features allow users to debug designs without using the debugger and do not require any instrumentation of kernel code.

One obvious way to specify these options is, to add them in Run configurations in Vitis IDE. To do this, Right-click on the *[aiengine]* domain project and select **Run As** → **Run configurations** and add the options in Arguments tab as shown below.
><img src="./Images/run_configurations.PNG" width="600" height="400">

Another way to enable feature is to update the configuration file, `${PROJECT_PATH}/Emulation-SW/Work/options/x86sim.options` from `no` to `yes` for the selected feature.
 
```
# For setting input directory path : define input-dir
input-dir=.
# For setting output directory path : define output-dir
output-dir=./x86simulator_output
# For Timeout : define timeout as integer value
timeout=
# For Snapshots : define dump=yes
dump=no
# For Gdb debugging : define gdb=yes
gdb=no
# For running Valgrind : define valgrind=yes
valgrind=no
# For running Valgrind and debugging via Gdb server : define valgrind-gdb=yes
valgrind-gdb=no
# For overriding default options to valgrind : define --valgrind-args with options
valgrind-args=
# For Stopping on deadlock : define disable-stop-on-deadlock=yes
disable-stop-on-deadlock=no
# For Trace : define trace=yes
trace=no
# For Trace print : define trace-print=yes
trace-print=no
# For Enabling handshake protocol with external testbench : define enable-handshake-ext-tb=yes
enable-handshake-ext-tb=no
# For generating valid x86sim.aierun_summary in SW_EMU flow: define pkg-dir=<absolute path to work directory>
pkg-dir= <Path to work directory>

```
### Data dump
This feature allows users to dump and inspect data traffic at kernel ports with data types. Let us examine how this feature is helpful.

a) Open the `src/kernels/data_shuffle.cc` in *[aiengine]* domain project and comment out the line-24.<br />
b) Build the project and open the `${PROJECT_PATH}/Emulation-SW/Work/options/x86sim.options` file and update the dump feature from `no` to `yes`.<br />
c) Right-click on the *[aiengine]* domain project and **Run As** → **Launch SW Emulator**.<br />
d) Once the simulation is completed, you can observe the below messages in the console.<br />
 
```
Processing './x86simulator_output/dump/x86sim_dump.data'
 
File                   Port direction  Port type  Data type  Kernel or platform port
---------------------  --------------  ---------  ---------  -----------------------
mygraph_in_out_0.txt   out             window     int32      mygraph.in.out[0]     
mygraph_p_d_in_0.txt   in              window     int32      mygraph.p_d.in[0]     
mygraph_u_s_out_0.txt  out             window     float      mygraph.u_s.out[0]    
mygraph_out0_in_0.txt  in              window     float      mygraph.out0.in[0]    
mygraph_d_s_out_0.txt  out             window     int32      mygraph.d_s.out[0]    
mygraph_out1_in_0.txt  in              window     int32      mygraph.out1.in[0]    
mygraph_p_d_out_2.txt  out             window     float      mygraph.p_d.out[2]    
mygraph_u_s_in_0.txt   in              window     float      mygraph.u_s.in[0]     
mygraph_u_s_in_1.txt   in              stream     int32      mygraph.u_s.in[1]     
mygraph_d_s_in_1.txt   in              stream     int32      mygraph.d_s.in[1]     
mygraph_p_d_out_1.txt  out             stream     int32      mygraph.p_d.out[1]    
mygraph_p_d_out_0.txt  out             window     int32      mygraph.p_d.out[0]    
mygraph_d_s_in_0.txt   in              window     int32      mygraph.d_s.in[0]     
 
Wrote './x86simulator_output/dump/mygraph_in_out_0.txt'
Wrote './x86simulator_output/dump/mygraph_p_d_in_0.txt'
Wrote './x86simulator_output/dump/mygraph_u_s_out_0.txt'
Wrote './x86simulator_output/dump/mygraph_out0_in_0.txt'
Wrote './x86simulator_output/dump/mygraph_d_s_out_0.txt'
Wrote './x86simulator_output/dump/mygraph_out1_in_0.txt'
Wrote './x86simulator_output/dump/mygraph_p_d_out_2.txt'
Wrote './x86simulator_output/dump/mygraph_u_s_in_0.txt'
Wrote './x86simulator_output/dump/mygraph_u_s_in_1.txt'
Wrote './x86simulator_output/dump/mygraph_d_s_in_1.txt'
Wrote './x86simulator_output/dump/mygraph_p_d_out_1.txt'
Wrote './x86simulator_output/dump/mygraph_p_d_out_0.txt'
Wrote './x86simulator_output/dump/mygraph_d_s_in_0.txt'
Simulation completed successfully returning zero

```
**Note** : The `Port type` column in the above console should show the type as *buffer* but in 2023.1 release it shows as *window* port.

Observe that one text file per each port of each kernel is generated using the `--dump` feature and the filenames are in the format of `<graph-name>_<sub-graph-class-name>_<sub-graph-instance-name>_<kernel-index>_[in]/[out]_index.txt` for graph input/output files.

e) Open the `${PROJECT_PATH}/Emulation-SW/x86simulator_output/dump/mygraph_in_out_0.txt` file and note the `Iteration` and `snapshot` values recorded in that file. This matches with the dimensions(buffer size) specified in the graph code per iteration.
 
f) Similarly, you can open the input/output of all the kernels in a graph and observe the intermediate outputs as well as interface ports.
 
### Deadlock detection
 
AI Engine designs can run into simulator hangs. A common cause is insufficient input data for the requested number of graph iterations, mismatch between production and consumption of stream data, cyclic dependency with stream, cascade stream or asynchronous buffers, or wrong order of blocking protocol calls (acquisition of async buffers, read/write from streams).

By default, x86simulation detects the deadlock(if any) and the messages that appear in the console guides you on debugging further.

#### Scenario-1
 
g) For example, open the `src/kernels/data_shuffle.cc` and comment out the line-24.

h) Compile the design by re-building the *[aiengine]* domain project.

i) Run x86simulation by right-clicking on the *[aiengine]* domain project and **Run As** → **Launch SW Emulator**.

j) Observe that the x86simulator detects error and output messages on console. In addition to that the file `${PROJECT_PATH}/Emulation-SW/x86simulator_output/simulator_state_post_analysis.dot` is generated.
 
```
x86simulator --pkg-dir=./Work --i=..
INFO: Reading options file './Work/options/x86sim.options'.
x86simulator: Detected deadlock
Deadlock diagnosis:
  1. main() is waiting on kernel 'mygraph.p_d'
     because Node 'mygraph.p_d' is blocked while writing port 'mygraph.p_d.out[0]'
  2. Node 'mygraph.p_d' is blocked while writing port 'mygraph.p_d.out[0]'
     because Unable to write port 'mygraph.p_d.out[0]'
  3. Unable to write port 'mygraph.p_d.out[0]'
     because Node 'mygraph.d_s' is blocked while reading port 'mygraph.d_s.in[1]'
  4. Node 'mygraph.d_s' is blocked while reading port 'mygraph.d_s.in[1]'
     because Data unavailable from port 'mygraph.d_s.in[1]'
  5. Data unavailable from port 'mygraph.d_s.in[1]'
     because Node 'strmbrdcst_i3_po1' is blocked while reading port 'mygraph.p_d.out[1]'
  6. Node 'strmbrdcst_i3_po1' is blocked while reading port 'mygraph.p_d.out[1]'
     because Data unavailable from port 'mygraph.p_d.out[1]'
  7. Data unavailable from port 'mygraph.p_d.out[1]'
     because Node 'mygraph.p_d' is blocked while writing port 'mygraph.p_d.out[0]'
Consider rerunning x86simulator with the --trace option.
Wrote ./x86simulator_output/simulator_state_post_analysis.dot
Simulation completed successfully returning zero

```
This is the textual representation of the deadlock path (starting to the end). To get the pictorial representation of the same, you need to use `dot` application.

k) Locate the `${PROJECT_PATH}/Emulation-SW/x86simulator_output/simulator_state_post_analysis.dot` file path in your terminal.

l) Issue the command `dot -Tpng simulator_state_post_analysis.dot > simulator_state_post_analysis.png` and open the file.

><img src="./Images/dot_file.PNG" width="200" height="400">
 
The paths in the red indicate the root cause of the deadlock. In this design, if you observe carefully, Observe the graph path 'n3-c7-n2-c6-n5', the edge `c7` is not sending enough data to the edge `c6`. From our graph code, `in[1]` is the stream input of the kernel `data_shuffle`. This kernel expects stream data every iteration. However, the producing kernel sends 1 stream output every 16 input samples. This in-turn caused the kernel to stop functioning and the complete design went into the deadlock situation. Hence the path from node `n3` to `n5` also shown as red.  
 
m) Revert the changes you have done on `src/kernels/data_shuffle.cc`
 
#### Scenario-2

n) Empty the file `data/inx.txt`. Make sure to back-up the file before emptying it. 
o) Repeat the steps (h)-(l) and observe the deadlock path now.
><img src="./Images/dot_file.PNG" width="200" height="400">
 
In this case, due to the insufficient input data to fill the input buffer, the kernel went into the hang state waiting for the input data.

p) Make sure you revert the changes for other exercises.
 
**Important**

 Absence of deadlock for x86 simulation does not mean absence of deadlock in SystemC simulation. X86 simulation does not model timing and resource constraints and thus there are fewer causes of deadlock. On the other hand, if x86 simulation deadlocks, SystemC simulation deadlocks as well, so it is beneficial to fix the deadlock in x86 simulation before proceeding with SystemC simulation.

### Trace report in file
 
Trace capability is used for debugging simulation hangs without the need for instrumenting kernel code or using gdb. 

Consider the scenario-1 in the [Deadlock detection](./README.md#Deadlock-detection) section where the stream data from the producer kernel doesn't match with the consumer kernel. Now let's see how to visualize the information using the trace feature of x86simulator.

q) Make the changes to the source code as mentioned in Deadlock detection section, Scenario-1 and build the project.<br />
r) Now, right-click on the *[aiengine]* domain project and select **Run As** → **Run Configurations**.<br />
s) In the Arguments tab, add `--trace` to the program arguments. 
```
--pkg-dir=./Work --i=.. --trace
```
t) Click **Apply** and **Run**

u) Once the run completes, you can see below information in the console.
 ```
 Processing './x86simulator_output/trace/x86sim_event_trace.data'
10% complete
50% complete
70% complete
80% complete
100% complete
Wrote './x86simulator_output/trace/x86sim_event_trace.data.txt'
Wrote ./x86simulator_output/simulator_state_post_analysis.dot
Simulation completed successfully returning zero
```
v) Open the `${PROJECT_PATH}/Emulation-SW//x86simulator_output/trace/x86sim_event_trace.data.txt` file and observe the trace events.

w) Observe the events corresponding to all the kernels. Say, for `mygraph.d_s`. Go to the last but one timestamp on that kernel and locate the `stream stall begin in[1]` followed by `thread terminated`, as the deadlock happened. 

x) You can also add `–dump` to the simulator options and open the `.txt` files and observe the `snapshot` and `iteration` values to understand how many samples of data got processed.

For more details on the kind of events that are recorded, refer to the UG1076 - [Trace Report](https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment/Trace-Report)


 ### Trace report in output console

This is in continuation to the [Trace report in file](#./README.md#Trace-report-in-file) topic. Here, we try to look at the trace events in the console, rather than opening the file `x86sim_event_trace.data.txt`. 

**Note**: Make sure you are still using the design changes that are done as part of exercising the deadlock situation.

y) Make sure the *[aiengine]* domain project is successfully built for Emulation-SW

z) Right-click on the *[aiengine]* domain project and select **Run As** → **Run Configurations**.

a1) In the Arguments tab, add `--trace-print` to the program arguments. 

b1) Click **Apply** & **Run**.

c1) Observe the output in the console that has timestamp, internal name of the kernel and event type.

```
x86simulator --pkg-dir=./Work --i=.. --trace-print
INFO: Reading options file './Work/options/x86sim.options'.
    351664               strmbrdcst_i3_po1                    thread begin 0
    565999               strmbrdcst_i3_po1              stream stall begin 7
    353987                   platformIn_i0                    thread begin 0
    381566                  platformOut_i1                    thread begin 0
    426106                  platformOut_i2                    thread begin 0
    524589                          ker_i3                    thread begin 0
    746420                          ker_i3        kernel wait on run begin 0
```
Here you can notice that the output in console is not as polished as the file generated by --trace. But this is useful when your design runs for long time and you wish to see the event while the simulation is running.

### Section - 6                                                        
## Memory access violation and valgrind support

Memory access violations occur when a kernel is reading or writing out of bounds of an object or reading uninitialized memory. This can manifest itself in multiple ways like a simulator crash or hang. The `x86simulator --valgrind` option will find memory access violations in kernel source code. 

**Note**: Valgrind needs to be installed for this feature to work. AMD recommends using Valgrind version 3.16.1. This option allows detection of memory access violations in kernel source code using `x86simulator` with Valgrind. The following kinds of access violations can be detected.

* Out-of-bounds write
* Out-of-bounds read
* Read of uninitialized memory

**Setup Environment Variables**
`x86simulator` requires `VALGRIND_HOME`, `VALGRIND_LIB` and `PATH` environmental variables to be configured per your host computer configuration. Exit out of Vitis IDE, setup following environmental variables and relaunch Vitis IDE. For example,
```
export PATH=<Install_Path>/valgrind/3.16.1/:$PATH
export VALGRIND_HOME=<Install_Path>/valgrind/3.16.1/
export VALGRIND_LIB=<Install_Path>valgrind/3.16.1/lib/
```
a) After re-launching the Vitis IDE tool, enable the valgrind feature either by updating the configuration file, `${PROJECT_PATH}/Emulation-SW/Work/options/x86sim.options` or adding `--valgrind` to run configuration and click on **Apply** then **Close**.<br />
b) Run x86simulation by right-clicking on the *[aiengine]* domain project and select **Run As** → **Launch SW Emulator**.<br />
c) With no violations in kernel code, the valgrind messages looks as shown below in the console.

```
x86simulator --pkg-dir=./Work --i=.. --valgrind
It is recommended to use Valgrind version 3.14.0 or higher.
==21151== Memcheck, a memory error detector
==21151== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==21151== Using Valgrind-3.16.1 and LibVEX; rerun with -h for copyright info
==21151== Command: ./Work/pthread/sim.out
==21151==
INFO: Reading options file './Work/options/x86sim.options'.
==21151==
==21151== HEAP SUMMARY:
==21151==     in use at exit: 183,522 bytes in 530 blocks
==21151==   total heap usage: 179,047 allocs, 178,517 frees, 9,342,021 bytes allocated
==21151==
==21151== For a detailed leak analysis, rerun with: --leak-check=full
==21151==
==21151== For lists of detected and suppressed errors, rerun with: -s
==21151== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
d) Add memory read violation to the kernel code and try to see the valgrind messages.<br />
e) Open `src/kernels/peak_detect.cc` and change the line-26 to `v_in = *(InIter+8500000500)`<br />
f) Build the *[aiengine]* domain project and run the x86simulation. This time observe the console carefully for below messages in the output console.<br />

```
x86simulator --pkg-dir=./Work --i=.. --valgrind
..
..
..
==15387==    by 0x411D64: operator= (vbit_vector.h:237)
==15387==    by 0x411D64: operator= (vbit_vector.h:507)
==15387==    by 0x411D64: v16int32::operator=(v16int32 const&) (me_chess_types.h:2654)
==15387==    by 0x411A60: aie::detail::vector<int, 16u>::operator=(aie::detail::vector<int, 16u> const&) (vector.hpp:304)
==15387==    by 0x40C382: peak_detect(adf::io_buffer<int, adf::direction::in, adf::io_buffer_config<adf::extents<4294967295u>, adf::locking::sync, adf::addressing::linear, adf::margin<0u> > >&, adf::io_buffer<int, adf::direction::out, adf::io_buffer_config<adf::extents<4294967295u>, adf::locking::sync, adf::addressing::linear, adf::margin<0u> > >&, output_stream<int>*, adf::io_buffer<float, adf::direction::out, adf::io_buffer_config<adf::extents<4294967295u>, adf::locking::sync, adf::addressing::linear, adf::margin<0u> > >&) (peak_detect.cc:20)
..
..
==15387== For lists of detected and suppressed errors, rerun with: -s
==15387== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
/proj/xbuilds/SWIP/2023.1_0427_0137/installs/lin64/Vitis/2023.1/aietools/bin/x86simulator: line 374: 15387 Segmentation fault      valgrind $VALGRIND_ARGS $X86SIM_PROG

```
Valgrind points out the file that has out of bound read access with line number as shown above.

**Exercise step**

g) Add below lines of code that creates an uninitialized variable in any kernel code and see how the Valgrind helps identifying the issue.
```
int demo;
printf("%s, %d\n", __FUNCTION__, demo);
```

### Section - 7                                                        
## Using GDB debugger in command line
The x86 simulation features that works in Vitis IDE, also works in command line. Vitis IDE provides GUI based debug support to control the flow by setting the break points and viewing the intermediate values while execution. It is also possible to do the similar debugging for command line projects using GDB capabilities. This section walks through different steps to debug the code using GDB.

To start with, first locate the necessary files to compile and run the simulation as a standalone and then add the GDB capabilities step-by-step.

Before beginning, it is expected to set the environment variables as described in [Introduction](../README.md#Introduction). Also assumed that you have cloned the git repository.

### x86simulation on command line

a) Go to the tutorial directory and locate the `Makefile`.<br />
b) Do `make aie` in the linux terminal. This command runs the compilation.<br />
c) Do `make sim` to simulate the AI Engine graph for x86sim target.

### x86simulation with GDB
This topic walks you through running the x86simulator with GDB.<br />
d) Running the `x86simulator` with the `--gdb` command line switch, breaks immediately before entering `main()` in `graph.cpp`. This pauses execution before any AI Engine kernels have started because the graph has not been run. To exit GDB type `quit` or `help` for more commands.

Now, update the configuration file, `${PROJECT_PATH}/Work/options/x86sim.options` by changing the `gdb=no` to `gdb=yes` then do `make sim` or directly issue command `x86simulator --gdb` to launch x86simulator.

```
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib64/libthread_db.so.1".
INFO: Reading options file './Work/options/x86sim.options'.
[New Thread 0x7ffff595b700 (LWP 38956)]
[New Thread 0x7ffff515a700 (LWP 38957)]
[New Thread 0x7ffff4959700 (LWP 38958)]
[New Thread 0x7fffeffff700 (LWP 38959)]
[New Thread 0x7fffef7fe700 (LWP 38960)]
 
Thread 1 "sim.out" hit Temporary breakpoint 1, main () at ../../aie/graph.cpp:7
7      mygraph.init();
```
e)  After `x86simulator` is launched successfully with gdb, set up a breakpoint by `break` command.
```
(gdb) break data_shuffle
Breakpoint 2 at 0x410f47: file ./../.././aie/kernels/data_shuffle.cc, line 9.
```
f) Type `c` and continue execution until break point is hit.
```
(gdb) c
Continuing.
[New Thread 0x7fffeeffd700 (LWP 39520)]
[New Thread 0x7fffee7fc700 (LWP 39521)]
[New Thread 0x7fffedffb700 (LWP 39522)]
[Switching to Thread 0x7fffedffb700 (LWP 39522)]
 
Thread 9 "sim.out" hit Breakpoint 2, data_shuffle (from_prev=..., outmax=0x7fffedffad78, out_shift=...)
    at ./../.././aie/kernels/data_shuffle.cc:9
9       auto InIter = aie::begin_vector<8>(from_prev);
```
g) Type `info locals` to examine local variables and `info stack` to observe call stack.<br />
```
(gdb) info stack
#0  data_shuffle (from_prev=..., outmax=0x7fffedffad78, out_shift=...) at ./../.././aie/kernels/data_shuffle.cc:9
#1  0x0000000000411814 in b5_kernel_wrapper (arg0=..., arg1=0x4a26b0, arg2=...) at wrap_data_shuffle.cc:6
#2  0x000000000042180a in x86sim::Kernel_b5_data_shuffle::invokeKernel (this=0x4a5a90) at PthreadSim.cpp:82
#3  0x00007ffff7b4293f in x86sim::IMEKernel::execute() () from /proj/xbuilds/SWIP/2023.1_0427_0137/installs/lin64/Vitis/2023.1/aietools/lib/lnx64.o/libx86sim.so
#4  0x00007ffff7b59564 in ?? () from /proj/xbuilds/SWIP/2023.1_0427_0137/installs/lin64/Vitis/2023.1/aietools/lib/lnx64.o/libx86sim.so
#5  0x00007ffff7483dff in std::execute_native_thread_routine (__p=0x4a8890) at ../../../.././libstdc++-v3/src/c++11/thread.cc:80
#6  0x00007ffff7bc6ea5 in start_thread () from /lib64/libpthread.so.0
#7  0x00007ffff6be096d in clone () from /lib64/libc.so.6
```
h) You can also set the break point of your interest in any kernel source code at any line. For example, use `break ./../.././aie/kernels/data_shuffle.cc:21` sets the break point at line:21 in `data_shuffle.cc` source code.<br />
i) You can remove all breakpoints by typing `delete` and continue execution until end of the program. Issue `quit` to exit out of gdb.
```
(gdb) delete
Delete all breakpoints? (y or n) y
(gdb) c
Continuing.
[Thread 0x7fffeeffd700 (LWP 39520) exited]
[Thread 0x7fffee7fc700 (LWP 39521) exited]
[Thread 0x7fffedffb700 (LWP 39522) exited]
[Thread 0x7fffef7fe700 (LWP 38960) exited]
[Thread 0x7fffeffff700 (LWP 38959) exited]
[Thread 0x7ffff4959700 (LWP 38958) exited]
[Thread 0x7ffff515a700 (LWP 38957) exited]
[Thread 0x7ffff595b700 (LWP 38956) exited]
[Inferior 1 (process 37514) exited normally]
```
Below are some of the useful GDB commands.

|GDB Command |Description|
|:----|:----|
|break <kernel_function_name>|Pause execution at specified .|
|continue|Causes the debugger to run to completion.|
|delete|Delete all breakpoints.|
|finish|Exits the current function call but keeps the simulation paused.|
|info locals|Shows the current status of local variables within the scope of the function call shown in the call stack.|
|info stack|Shows a track of the function call stack at the current breakpoint.|
|print <local_variable_name>|Prints the current value of a single variable.|

### x86simulator Using GDB server
This feature uses GDB server to debug and requires two terminals working together. One terminal for `x86simulator` and the other terminal for `GDB server`.<br />
j) In Terminal-1, update the configuration file, `${PROJECT_PATH}/Work/options/x86sim.options` by changing the `valgrind-gdb=no` to `valgrind-gdb=yes` and issue command `x86simulator` or directly issue command `x86simulator --valgrind-gdb` directly.
```
>>x86simulator --valgrind-gdb
==44263== Memcheck, a memory error detector
==44263== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==44263== Using Valgrind-3.16.1 and LibVEX; rerun with -h for copyright info
==44263== Command: ./Work/pthread/sim.out
==44263==
==44263== (action at startup) vgdb me ...
==44263==
==44263== TO DEBUG THIS PROCESS USING GDB: start GDB like this
==44263==   /path/to/gdb ./Work/pthread/sim.out
==44263== and then give GDB the following command
==44263==   target remote | /tools/baton/valgrind/3.16.1/lib/valgrind/../../bin/vgdb --pid=44263
==44263== --pid is optional if only one valgrind process is running
==44263==
```
k) In Terminal-2, setup the Vitis tool and Valgrind tool environment variables as described in [Memory access violation and valgrind support](./README.md#Memory-access-violation-and-valgrind-support).<br />
l) Navigate to the project directory and issue the command `gdb ./Work/pthread/sim.out`. 
```
>>gdb ./Work/pthread/sim.out
GNU gdb (GDB) 11.2
Copyright (C) 2022 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-pc-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<https://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.
 
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from ./Work/pthread/sim.out...
```
m) Issue command `target remote | <VALGRIND_INSTALL_PATH/valgrind/3.16.1/bin/vgdb` in Terminal-2.
```
(gdb) target remote | /tools/baton/valgrind/3.16.1/bin/vgdb
Remote debugging using | /tools/baton/valgrind/3.16.1/bin/vgdb
relaying data between gdb and process 44263
warning: remote target does not support file transfer, attempting to access files from local filesystem.
Reading symbols from /lib64/ld-linux-x86-64.so.2...
(No debugging symbols found in /lib64/ld-linux-x86-64.so.2)
0x0000000004001140 in _start () from /lib64/ld-linux-x86-64.so.2
(gdb)
```
n) From now, you can follow the steps (e)-(i) and set the break-point of interest, observe local values, stack values e.t.c and finally once the gdb server exited normally, you can observe the Terminal-1 for valgrind runs completion and overall status summaries.
```
INFO: Reading options file './Work/options/x86sim.options'.
==44263==
==44263== HEAP SUMMARY:
==44263==     in use at exit: 183,522 bytes in 530 blocks
==44263==   total heap usage: 179,047 allocs, 178,517 frees, 9,341,190 bytes allocated
==44263==
==44263== For a detailed leak analysis, rerun with: --leak-check=full
==44263==
==44263== For lists of detected and suppressed errors, rerun with: -s
==44263== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
**Exercise step**

Insert an out of bound read access error as mentioned in [Memory access violation and valgrind support](./README.md#Memory-access-violation-and-valgrind-support) and perform the steps from (j)-(n). Observe the violation using the --valgrind-gdb in Terminal-1.

## Support

GitHub issues will be used for tracking requests and bugs. For questions go to [support.xilinx.com](https://support.xilinx.com/).


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
