
<table>
 <tr>
   <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Debug Walkthrough Tutorial - From Simulation to Hardware</h1>
   </td>
 </tr>
 <tr>
 <td align="center"><h2>AI Engine Debug with X86simulator</h2>
 </td>
 </tr>
</table>

`x86simulator` supports fast emulation execution and with other features to help verify the kernel's functionalities. It applies systemC model instead of RTL model to achieve faster build and run time.

Hardware constraints such as heap/stack sizes and program memory size are not verified in software emulator.

The following steps outline the procedure:

[1. Launch and Verify x86 Emulation in Vitis IDE](#1-Launch-and-Verify-x86-Emulation-in-Vitis-IDE)

[2. Printf() Support](#2-Printf-Support)

[3. Printf() with Vector Data Types](#3-Printf-with-Vector-Data-Types)

[4. Vitis IDE Debugger Support](#4-Vitis-IDE-Debugger-Support)

[5. x86simulator Configuration File](#5-x86simulator-Configuration-File)

[6. x86simulator Features](#6-x86simulator-Features)

[7. Command Line Projects Support](#7-Command-Line-Projects-Support)

[Limitations](#Limitations)


## 1. Launch and Verify Software Emulation in Vitis IDE

### Step 1.1 Select Software Emulation Configuration
<img src="images/se_config.png" width="300">

### Step 1.2 Build Configuration
To improve debug visibility, enable `-O0` in aiengine domain project build configuration.

<img src="images/86_build_cfg.png" width="450">

### Step 1.3 Build with x86 Emulator
<img src="images/se_build0.png" width="300">

### Step 1.4 Run with x86 Emulator
<img src="images/86_run.png" width="450">

Note: Both step 1.2 and 1.3 are right click on aiengine domain project to select x86simulator. Ignore `Launch SW Emulator` from Vitis IDE GUI.

### Step 1.5 Console Displays Emulation Result
<img src="images/86_run1.png" width="450">

### Step 1.6 Verify Run Result
X86 emulator output files from design are located at `${PROJECT}/Emulation-SW/x86simulator_output/data`. Verify the output files `${PROJECT}/Emulation-SW/x86simulator_output/data/dlbf_out[0-7].txt` and `${PROJECT}/Emulation-SW/x86simulator_output/data/ulbf_out[0-3].txt` against golden files `${PROJECT}/data/dlbf_gold[0-7].txt` and `${PROJECT}/data/ulbf_gold[0-3].txt` to ensure that the design's I/O functionalities are correct. Vitis IDE supports `compare with` feature to compare two files, highlight two files to be compared then right click one of highlighted file and select `compare with` then `each other`.
For example, Compare `${PROJECT}/data/ulbf_gold3.txt` and `${PROJECT}/Emulation-SW/data/ulbf_out3.txt`

<img src="images/se_compare.png" width="600">

## 2. `printf` Support
The simplest form of tracing is to use a formatted `printf()` statement in the code for printing debug messages. Visual inspection of intermediate values and addresses can help you understand the progress of program execution. You can add `printf()` statements to your code to be processed during x86 emulation, or AIE emulation, and remove them or comment them out for hardware builds. To help identify which kernel is printing which line the `X86SIM_KERNEL_NAME` macro can be useful.

### Step 2.1 Add `printf()` statement
Select `bf8x8_fst_api.cpp` file in Vitis IDE to be edited. Add `#include <adf/x86sim/x86simDebug.h>` at line 24 and `printf("%s: %s, %d\n", __FUNCTION__, X86SIM_KERNEL_NAME, __LINE__);` statement at line 37 of `bf8x8_fst_api.cpp` file.
`bf8x8_fst_api.cpp` can be found from Vitis IDE's explorer, browse to `beamformer` project, click on `src` then `kernel` to expand the directory, and click on `bf8x8_fst_api.cpp`. The file will be displayed at the center of the Vitis IDE. To use X86SIM_KERNEL_NAME you must include `adf/x86sim/x86simDebug.h` header file in graph kernel code.

### Step 2.2 Compile the Project
Highlight the beamformer project, right-click to enable pull-down menu, and select **Build Project** to build the beamformer project.

### Step 2.3 Run the Project
Highlight the beamformer project, right-click to enable pull-down menu, select **Run As**, and then **Launch SW Emulator** to run the project in Vitis IDE.

### Step 2.4 Expected Result
Expect to observe the output of `printf()` statement displayed at Vitis IDE console window.
<img src="images/86_sw_printf.png" width="600">

## 3. Printf() with Vector Data Types
`x86simulator` supports output array data value via printf().

### Step 3.1 Add printf() in source code
Add these three lines at line 52 of `bf8x8_fst_api.cpp` and recompile the AI Engine domain project.
```bash
int16_t* print_ptr = (int16_t*)&bufa0;
for (int pp=0; pp < 8; pp++)
    printf("vector %d real:%d image:%d\n", pp, print_ptr[pp*2], print_ptr[pp*2+1]);
```
### Step 3.2 Compile and Run `x86simulator`
Follow step 1.3 to build the project and step 1.4 to run `x86simulator`.

### Step 3.3 Expected Result
Array values are displayed at console.

<img src="images/86_printf_array.png" width="600">

Note: With the AI Engine simulator the --profile option is required in order to observe `printf()` outputs but with `x86simulator` no additional options are needed to enable `printf()` calls. This is one of the benefits of the `x86simulator`.

### Step 3.4 Reverse Added printf() Statements
Remove added three lines from line 52 of `bf8x8_fst_api.cpp` for other features.


## 4. Vitis IDE Debugger Support

### Step 4.1 Launch SW Emulator Debugger
<img src="images/86_IDE_debug0.png" width="600">

### Step 4.2 Debug during Run Time
All Vitis IDE debug functionalities are supported, e.g. step in, step over, step return, inspect variable values, set/clear break points... Need to set break points to proper code to perform debug functionalities.
<img src="images/86_IDE_debug1.png" width="900">

### Step 4.3 Inspect Array Values
Double click on `Variables` view to enlarge view area. Double click again to make it back to original size.
To inspect vector values, click on arrow of `bufa0`, arrow of `data`, arrow of `val`, arrow of `data` (where array<v1cint16, 8> is shown, arrow of `__elems_` (where shows `__elems_[0]` to `__elems_[7]`, arrow of `__elems_[0]`, arrow of `val`, arrow of `data`, arrow of `__elems_`, arrow of `__elems_[0]`, arrow of `val`, arrow of `VBitDataBase<16, true, false>`, arrow of `data`, (x)=0. It shows unsigned value of 61038 that translates to signed decimal value of -4498.
Follow above steps and click on `__elems_[1]` instead of `__elems_[0]`, the value is 5022.
<img src="images/86_IDE_debug2.png" width="900">

To cross check, first input data set is `cint16` type and from input file at `$PROJECT_PATH/data/ulbf_cin17.txt`.
```bash
-4498 5022 -4794 -631
-4372 325 2892 -3431
-7067 -6110 3811 1737
-5236 1188 3326 6723
-6130 6024 5845 -1099
...
```

Data generated by `x86simulator --dump` is at `$PROJECT_PATH/Emulation-SW/x86simulator_output/dump/dut_ulbf_bf1_core_7_in_0.txt` with first pair values `-4498 5022`.
```bash
# port: dut.ulbf.bf1.core[7].in[0]
# port_dir: in
# port_type: window
# data_type: cint16
# Iteration 1; snapshot 1
-4498 5022
-4794 -631
-4372 325
2892 -3431
-7067 -6110
3811 1737
-5236 1188
3326 6723
-6130 6024
5845 -1099
...
```

## 5. x86simulator Configuration File
`${PROJECT_PATH}/Emulation-SW/Work/options/x86sim.options` defines all supported features.
```bash
# For Timeout : define timeout as integer value
timeout=no
# For Snapshots : define dump=yes
dump=yes
# For Gdb debugging : define gdb=yes
gdb=no
# For running Valgrind : define valgrind=yes
valgrind=no
# For running Valgrind and debugging via Gdb server : define valgrind-gdb=yes
valgrind-gdb=no
# For overriding default options to valgrind : define --valgrind-args with options
valgrind-args=no
# For Stopping on deadlock : define stop-on-deadlock=yes
stop-on-deadlock=no
# For Trace : define trace=yes
trace=no
# For Trace print : define trace-print=yes
trace-print=no
```

`x86simulator` accepts command line options to enable/disable supported features, type in `x86simulator --help` to list supported command line options.
```bash
bash-4.2$ x86simulator --help
 x86simulator [-h] [--help] [--h] [--pkg-dir=PKGDIR]
 optional arguments:
 -h,--help  --h show this help message and exit
--pkg-dir=PKG_DIR     Set the package directory. ex: Work
--timeout=secs        Terminate simuation after specified number of seconds
--dump                Enable snapshots of data traffic on kernel ports
--gdb                 Invoke from gdb
--valgrind            Run simulator under valgrind to detect access violoations
--valgrind-gdb        Run simulator under valgrind and debug via gdb server
--valgrind-args=ARGS  Override default options for valgrind. Used in conjunction
                      with --valgrind.
--stop-on-deadlock    Stop simulation if deadlock is detected
--trace               Enable trace of kernel stall events
--trace-print         Print kernel stall events during simulation
```

## 6. x86simulator Features
All x86simulator supported features allow users to debug designs without using the debugger and do not require any instrumentation of kernel code. To enable feature, user can  update the configuration file, `${PROJECT_PATH}/Emulation-SW/Work/options/x86sim.options` from `no` to `yes` for the selected feature, or pass in command line option e.g. `x86simulator --dump`. 

### x86simulator Data Dump
This feature allows users to dump and inspect data traffic at kernel ports with data types.

#### Step 6.1.1 Enable Dump Feature
Update the configuration file, `${PROJECT_PATH}/Emulation-SW/Work/options/x86sim.options`, change `dump=no` to `dump=yes`. Or add `--dump` to run configuration and click on `Apply` then `Close`.

<img src="images/86_dump_conf.png" width="600">

#### Step 6.1.2 Run x86simulator
Select aiengine domain project, right click on `Run As` and select `Launch SW Emulator`. x86simulator will be launched and run the aiengine domain project. After x86simulator completes, generated files are at `${PROJECT_PATH}/Emulation-SW/x86simulator_output/dump` directory. Filename are in the format of  `graph-name_sub-graph-class-name_sub-graph-instance-name_kernel-index_[in]/[out]_index.txt` for graph input/output files.

<img src="images/86_sim_dump.png" width="600">

### x86simulator Deadlock Detection
AI Engine designs can run into simulator hangs. A common cause is insufficient input data for the requested number of graph iterations, mismatch between production and consumption of stream data, cyclic dependency with stream, cascade stream or asynchronous windows, or wrong order of blocking protocol calls (acquisition of async window, read/write from streams).

#### Step 6.2.1 Enable Deadlock Detection Feature
Update the configuration file, `${PROJECT_PATH}/Emulation-SW/Work/options/x86sim.options`, change `stop-on-deadlock=no` to `stop-on-deadlock=yes`. Or add `--stop-on-deadlock` to run configuration and click on `Apply` then `Close`.

#### Step 6.2.2 Setup Deadlock Condition
Empty `${PROJECT_PATH}/data/dlbf_din0.txt` file.
Note: Make sure to backup the file content for other feature runs.

#### Step 6.2.3 Run x86simulator
Select aiengine domain project, right click on `Run As` and select `Launch SW Emulator`. x86simulator will be launched and run the aiengine domain project. x86simulator detects error and output messages on console. File `${PROJECT_PATH}/Emulation-SW/x86simulator_output/simulator_state_post_analysis.dot` is generated.

<img src="images/86_deadlock.png" width="600">

#### Step 6.2.4 Converts Generated dot file to png file with `dot` Application
Use `dot` application converts .dot file to .png file, `dot -Tpng simulator_state_post_analysis.dot > simulator_state_post_analysis.png`. Open the `${PROJECT_PATH}/Emulation-SW/x86simulator_output/simulator_state_post_analysis.png`, paths in red indicate the root cause of the deadlock.

<img src="images/86_deadlock1.png" width="600">

**IMPORTANT: Absence of deadlock for x86 simulation does not mean absence of deadlock in SystemC simulation. X86 simulation does not model timing and resource constraints and thus there are fewer causes of deadlock. On the other hand, if x86 simulation deadlocks, SystemC simulation deadlocks as well, so it is beneficial to fix the deadlock in x86 simulation before proceeding with SystemC simulation.**

### x86simulator Trace Report
#### Step 6.3.1 Enable Trace Feature
Update the configuration file, `${PROJECT_PATH}/Emulation-SW/Work/options/x86sim.options`, change `trace=no` to `trace=yes` and `trace-print=no` to `trace-print=yes` Or add `--trace` and `--trace-print` to run configuration and click on `Apply` then `Close`.

Note:
`--trace` option to get a full trace report at the end of the simulation.
`--trace-print` option to get output displayed on the console while the simulation is running.

#### Step 6.3.2 Run x86simulator
Select aiengine domain project, right click on `Run As` and select `Launch SW Emulator`. x86simulator will be launched and run the aiengine domain project. After x86simulator completes, generated files `x86sim_event_trace.data` and `x86sim_event_trace.data.txt` are at `${PROJECT_PATH}/Emulation-SW/x86simulator_output/trace` directory.

<img src="images/86_trace.png" width="600">

#### Step 6.3.3 x86simulator Trace Events supported
1. Start of a kernel iteration.
2. End of a kernel iteration.
3. Start of a stream stall, i.e., the start of a read from a stream port of a kernel that blocks because of lack of data.
4. End of a stream stall, i.e., the point where a read from a stream port that initially blocked, finally returns.
5. Start of a lock stall, i.e., the start of an attempt to acquire a window port where the lock is initially not available.
6. End of a lock stall, i.e., the point in time where an attempt to acquire a window port that initially blocked, finally returns.

#### Differences between `trace` and `trace-print`
Besides output to file versus output to console, The columns of the output of --trace-print contain the following information.
1. Timestamp: It is the same as in x86sim_event_trace.data.txt.
2. Internal name of the kernel (x86sim_event_trace.data.txt uses the user name).
3. Event type.
4. Numeric value whose meaning depends on the event type: It encodes the port that you are waiting on for a lock or stream stall. It encodes the iteration number of a start of an iteration event.

### x86simulator Memory Access Violation and Valgrind
Memory access violations occur when a kernel is reading or writing out of bounds of an object or reading uninitialized memory. This can manifest itself in multiple ways like a simulator crash or hang. It can also cause simulator results to be non-repeatable. The `x86simulator --valgrind` option will find memory access violations in kernel source code.
Note: Valgrind needs to be installed for this feature to work. Xilinx recommends using Valgrind version 3.16.1.
This option allows detection of memory access violations in kernel source code using 'x86simulator' with Valgrind. The following kinds of access violations can be detected.
1. Out-of-bounds write
2. Out-of-bounds read
3. Read of uninitialized memory

#### Step 6.4.1 Setup Environmental Variables
`x86simulator` requires `VALGRIND_HOME`, `VALGRIND_LIB` and `PATH` environmental variables to be configured per your host computer configuration.
Exit out of Vitis IDE, setup following environmental variables and relaunch Vitis IDE.
For example,
```bash
export PATH=${FULL_PATH_TO_VALGRIND}:$PATH
export VALGRIND_HOME=${FULL_PATH_TO_VALGRIND}
export VALGRIND_LIB=${FULL_PATH_TO_VALGRIND_LIB}
```

#### Step 6.4.2 Enable Valgrind Feature
Update the configuration file, `${PROJECT_PATH}/Emulation-SW/Work/options/x86sim.options`, change `valgrind=no` to `valgrind=yes`. Or Add `--valgrind` to run configuration and click on `Apply` then `Close`.

#### Step 6.4.3 Run x86simulator
An example of no invalid memory access message from `valgrind`,
```bash
==150659== Memcheck, a memory error detector
==150659== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==150659== Using Valgrind-3.12.0 and LibVEX; rerun with -h for copyright info
==150659== Command: ./Work/pthread/sim.out
==150659== 
INFO: Reading options file './Work/options/x86sim.options'.
date
==150659== 
==150659== HEAP SUMMARY:
==150659==     in use at exit: 184,181 bytes in 716 blocks
==150659==   total heap usage: 2,239,062 allocs, 2,238,346 frees, 141,993,399 bytes allocated
==150659== 
==150659== For a detailed leak analysis, rerun with: --leak-check=full
==150659== 
==150659== For counts of detected and suppressed errors, rerun with: -v
==150659== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

#### Step 6.4.4 Error Message with an Unintialized Variable
Add these two lines at line 35 of `bf8x8_fst_api.cpp`.
```bash
int demo;
printf("%s, %d\n", __FUNCTION__, demo);
```

#### Step 6.4.5 Expected Errors from `valgrind`
```bash 
==32176== Memcheck, a memory error detector
==32176== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==32176== Using Valgrind-3.12.0 and LibVEX; rerun with -h for copyright info
==32176== Command: ./Work/pthread/sim.out
==32176== 
INFO: Reading options file './Work/options/x86sim.options'.
==32176== Thread 90:
==32176== Conditional jump or move depends on uninitialised value(s)
==32176==    at 0x6486A42: vfprintf (in /usr/lib64/libc-2.17.so)
==32176==    by 0x64908C8: printf (in /usr/lib64/libc-2.17.so)
==32176==    by 0x405A22: bf8x8_fst_api(input_window<cint16>*, input_window<cint16>*, output_stream<cacc48>*) (bf8x8_fst_api.cpp:36)
==32176==    by 0x408504: b88_kernel_wrapper(input_window<cint16>*, input_window<cint16>*, output_stream<cacc48>*) (wrap_bf8x8_fst_api.cpp:5)
==32176==    by 0x46C83D: x86sim::Kernel_b88_bf8x8_fst_api::invokeKernel() (PthreadSim.cpp:56)
==32176==    by 0x55CEF2D: x86sim::IMEKernel::execute() (in ${INSTALL_PATH}/aietools/lib/lnx64.o/libx86sim.so)
==32176==    by 0x55CDD3A: ??? (in ${INSTALL_PATH}/aietools/lib/lnx64.o/libx86sim.so)
==32176==    by 0x5C5FCBE: execute_native_thread_routine (thread.cc:83)
==32176==    by 0x4E3CE24: start_thread (in /usr/lib64/libpthread-2.17.so)
==32176==    by 0x653734C: clone (in /usr/lib64/libc-2.17.so)
==32176==  Uninitialised value was created by a stack allocation
==32176==    at 0x4059E8: bf8x8_fst_api(input_window<cint16>*, input_window<cint16>*, output_stream<cacc48>*) (bf8x8_fst_api.cpp:34)
==32176== 
==32176== Use of uninitialised value of size 8
==32176==    at 0x6485EDB: _itoa_word (in /usr/lib64/libc-2.17.so)
==32176==    by 0x6486F55: vfprintf (in /usr/lib64/libc-2.17.so)
==32176==    by 0x64908C8: printf (in /usr/lib64/libc-2.17.so)
==32176==    by 0x405A22: bf8x8_fst_api(input_window<cint16>*, input_window<cint16>*, output_stream<cacc48>*) (bf8x8_fst_api.cpp:36)
==32176==    by 0x408504: b88_kernel_wrapper(input_window<cint16>*, input_window<cint16>*, output_stream<cacc48>*) (wrap_bf8x8_fst_api.cpp:5)
==32176==    by 0x46C83D: x86sim::Kernel_b88_bf8x8_fst_api::invokeKernel() (PthreadSim.cpp:56)
==32176==    by 0x55CEF2D: x86sim::IMEKernel::execute() (in ${INSTALL_PATH}/aietools/lib/lnx64.o/libx86sim.so)
==32176==    by 0x55CDD3A: ??? (in ${INSTALL_PATH}/aietools/lib/lnx64.o/libx86sim.so)
==32176==    by 0x5C5FCBE: execute_native_thread_routine (thread.cc:83)
==32176==    by 0x4E3CE24: start_thread (in /usr/lib64/libpthread-2.17.so)
==32176==    by 0x653734C: clone (in /usr/lib64/libc-2.17.so)
==32176==  Uninitialised value was created by a stack allocation
==32176==    at 0x4059E8: bf8x8_fst_api(input_window<cint16>*, input_window<cint16>*, output_stream<cacc48>*) (bf8x8_fst_api.cpp:34)
==32176== 
...
bf8x8_fst_api, 0
bf8x8_fst_api, 0
bf8x8_fst_api, 0
bf8x8_fst_api, 0
bf8x8_fst_api, 0
bf8x8_fst_api, 0
bf8x8_fst_api, 0
bf8x8_fst_api, 0
bf8x8_fst_api, 0
bf8x8_fst_api, 0
bf8x8_fst_api, 0
bf8x8_fst_api, 0
==32176== 
==32176== HEAP SUMMARY:
==32176==     in use at exit: 184,181 bytes in 716 blocks
==32176==   total heap usage: 77,657 allocs, 76,941 frees, 7,902,765 bytes allocated
==32176== 
==32176== For a detailed leak analysis, rerun with: --leak-check=full
==32176== 
==32176== For counts of detected and suppressed errors, rerun with: -v
==32176== ERROR SUMMARY: 72 errors from 6 contexts (suppressed: 0 from 0)
```

Note:
1. The error message is generated by `valgrind` application instead of `x86simulator`.
2. Running the x86simulator with the Valgrind option will increase the simulation run time.

#### Step 6.4.6 Reverse Added Error
Remove added lines, line 35 and 36 from `bf8x8_fst_api.cpp` file for other `x86simulator` features.


## 7. Command Line Projects Support
All above `x86simulator` features are supported in command line projects. Since Vitis IDE provides GUI debug support with flow control and multiple views, command line projects need GDB capabilities to fulfill the gap. 

### x86simulator Using GDB

#### Step 7.1.1 Download the project
Clone the project source from git repository and unzip the downloaded zip file.

#### Step 7.1.2 Prepare Makefiles and source code
Use this tutorial's Makefile.sw_emu that configures right target to build.
```bash
cd ${DOWNLOAD_PATH}/AI_Engine_Development/Feature_Tutorials/09-debug-walkthrough
cp Makefile.sw_emu Makefile
```

#### Step 7.1.3 Build Project
```bash
make libadf.a
```

#### Step 7.1.4 Enable GDB to Work with `x86simulator`
Update the configuration file, ${PROJECT_PATH}/Emulation-SW/Work/options/x86sim.options, change `gdb=no` to `gdb=yes` then launch `x86simulator`. Or issue command `x86simulator --gdb` to launch `x86simulator`.
By default, when running the `x86simulator` with the `--gdb` command line switch it breaks immediately before entering main() in graph.cpp. This pauses execution before any AI Engine kernels have started because the graph has not been run. To exit GDB type quit or help for more commands.

Launch x86simulator with gdb, issue command `x86simulator --gdb`.

<img src="images/86_gdb1.png" width="600">

After `x86simulator` is launched successfully with `gdb`, set up a breakpoint by `break` command.

<img src="images/86_gdb2.png" width="600">

Continue execution until a breakpoint is hit. Examine local variables and/or call stack.

<img src="images/86_gdb3.png" width="600">

Clear all breakpoints and continue execution until end of program. Issue `quit` to exit out of gdb.

<img src="images/86_gdb4.png" width="600">


#### Step 7.1.5 GDB Commands
| GDB commands | Descriptions |
| :---| :---|
| break <kernel_function_name> | Pause execution at specified <kernel function name>. |
| continue                     | Causes the debugger to run to completion. |
| delete                       | Delete all breakpoints. |
| finish                       | Exits the current function call but keeps the simulation paused. |
| info locals                  | Shows the current status of local variables within the scope of the function call shown in the call stack. |
| info stack                   | Shows a track of the function call stack at the current breakpoint. |
| print <local_variable_name>  | Prints the current value of a single variable. |


### x86simulator Using GDB server

#### Step 7.2.1 Project preparation
Follow step 7.1.1 to 7.1.3 to prepare project using GDN server.

#### Step 7.2.2 Enable GDB Server to Work with `x86simulator`
Since this feature uses GDB server, requires two terminals working together. One terminal for `x86simulator` and the other terminal for GDB server.

Terminal 1 to launch `x86simulator`. Update the configuration file, ${PROJECT_PATH}/Emulation-SW/Work/options/x86sim.options, change `valgrind-gdb=no` to `valgrind-gdb=yes` then launch `x86simulator`. Or issue command `x86simulator --valgrind-gdb` to launch `x86simulator`.

<img src="images/86_valgrind_gdb0.png" width="600">

#### Step 7.2.3 Launch GDB Server
Terminal 2, follow step 6.4.1 to setup Vitis tool and valgrind tool environmental variables.

```bash
cd ${PROJECT_PATH}
gdb ./Work/pthread/sim.out
```

<img src="images/86_valgrind_gdb1.png" width="600">

#### Step 7.2.4 Using GDB Commands
Issue command `target remote | /opt/rh/devtoolset-6/root/usr/lib64/balgrind../../bin/vgdb` from GDB server terminal to run and debug application.

<img src="images/86_valgrind_gdb2.png" width="600">

Set up a breakpoint at kernel function, `bf8x8_fst_api` and continue to execution until breakpoint is hit.

<img src="images/86_valgrind_gdb3.png" width="600">

Breakpoint hit.

<img src="images/86_valgrind_gdb4.png" width="600">

Issue debug command `info stack` to inspect call stack, `delete` to clear all breakpoints and `c` to complete program execution.

<img src="images/86_valgrind_gdb5.png" width="600">

After program finishes execution from DBG server, valgrind runs to completion and summarizes overall status.

<img src="images/86_valgrind_gdb6.png" width="600">


## Limitations
1. Hardware constraints check is not supported in software emulator. Functionalities verified in software emulator need to check resource constraints in AI Engine emulator before running on the hardware board.
2. Kernels that require retaining state from one invocation (iteration) to the next can use global or static variable to store this state. Variables with static storage class, such as global variables and static variables are a cause of discrepancies between x86 simulation and AI Engine simulation. The root causes is that for x86 simulation, the sources files of all kernels are compiled into a single executable. Recommend using C++ class model to avoid the pitfall of variables with static storage class. Alternatively the storage class of the global or static variable can be changed to thread_local, but just for x86 simulation. In this case, each instance of the kernel has its own copy of the variable in x86 simulation.
3. Graph constructs for timed execution (graph.wait(N), graph.resume() and graph.end(N)) behave differently in the AI Engine simulator and the x86 simulator. For the AI Engine simulator N specifies the processor cycle timeout, whereas for the x86 simulator it specifies a wall clock timeout in milliseconds. Thus, if your test bench uses timed execution the AI Engine simulator and x86 simulator might produce different results, in particular, the amount of output data produced might differ.
4. When running the AI Engine compiler with a target of x86sim the compiler ignores the `--Xchess` option. This means that the x86sim flow does not support kernel-specific  compile options.
5. The files associated with the simulation output(s) may not be written to completely after graph.end(). If your main() program is reading these files—to verify the results of the simulation, recommends a wait of two seconds between graph.end() and opening of the files. In the following example, main() waits for two seconds.
6. If a header file appears in a adf::headers constraint and that header file includes `aie_api/aie.hpp` directly or indirectly, `aiecompiler --target=x86sim` will fail. As a workaround, you can move the include of aie_api/aie.hpp from the kernel header to the kernel source file.
7. Packet Stream connections have a field known as the packet ID. If the source of the packet ID field comes from within the ADF graph, the x86 simulator uses the canonical, zero-based indexing scheme for packet IDs. The first branch on the output of a split node has a packet ID equal to 0 followed by 1, 2, 3, etc. If the source of the packet ID field comes from outside the ADF graph there will be discrepancies between the AI Engine simulator and the x86 simulator. The nature of packet merging means that the x86 simulator and the AI Engine simulator produce non-deterministic results. If an AI Engine on a packet split branch finishes processing data before any of the other branches its data appears on the output of the packet merge first. Exactly which core finishes first is highly dependent on both the kernel code and the incoming data. So any downstream processing blocks must be prepared to handle this behavior.
8. Both synchronous and asynchronous run-time parameters are fully supported in the x86 simulator. However the precise timing and cycle accuracy of when an RTP update occurs differs between the x86 simulator and the AI Engine simulator. Asynchronous RTPs in particular do not affect a kernel on a specific known cycle by their very nature of being asynchronous. This is true of asynchronous RTPs in both the x86 simulator and the AI Engine simulator.


# Support

GitHub issues will be used for tracking requests and bugs. For questions go to [support.xilinx.com](https://support.xilinx.com/).

# License

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0


Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

<p align="center"><sup>XD005 | &copy; Copyright 2021 Xilinx, Inc.</sup></p>
