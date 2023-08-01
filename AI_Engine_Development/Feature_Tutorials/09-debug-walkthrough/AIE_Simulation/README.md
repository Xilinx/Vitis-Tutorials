<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# AI Engine Simulation Debug Walkthrough

## Introduction

The AI Engine simulator (aiesimulator) models the timing and resources of the AI Engine array, while using transaction-level SystemC models for the NoC and double-data rate (DDR) memory. This allows for faster performance analysis of your AI Engine applications and accurate estimation of the AI Engine resource use with cycle-approximate timing information. This takes into account the hardware constraints, such as heap/stack sizes and proram memory size for verifying the design and keeping track of those detailed timing information makes the simulation slow and resource intensive.

The AI Engine simulator allows stepping through the AI Engine compiler-generated assembly code which aids in code optimization; however, source level visibility could be limited due to compiler optimization. This section discusses about some important features (listed below) that ease the debugging of the design at AIE Simulation level.  

Before starting this tutorial:

* It is expected that you have run the steps to set the environment variables as described in [Introduction](../README.md#Introduction).
* Create a system project manually using the steps mentioned in [Port the Command Line project to the Vitis IDE System Project](../CreateIDESystemProj.md), or download the Vitis exported project as instructed in [Download the Vitis IDE Project](../README.md#Download-Vitis-IDE-project).
* Open the Vitis IDE tool by typing `vitis` in the console, set the workspace, and the IDE opens. Click **File**->**Import**, select the **Vitis exported project zip file**, browse to the above zip file, and click `Next`. Enable the check box, **System Projects**, and click **Finish**.

## Features

<table style="width:100%">

<tr>
<td>
 <a href="./README.md#Build-and-simulate-in-Vitis-IDE">Build and Simulate in the Vitis IDE</a>
</td>
<td>
Demonstrates how to use the Vitis IDE to build and simulate an AI Engine design.
</td>
</tr>
	
<tr>
<td>
<a href="./README.md#Debug-using-printf">Debug Using printf</a>
</td>
<td>
Explains how to add `printf` statement, and view the output in the console.
</td>
</tr>

<tr>
<td>
<a href="./README.md#Debug-using-Vitis-IDE-debugger">Debug Using the Vitis IDE Debugger</a>
</td>
<td>
Explores different debugging techniques that uses the Vitis IDE debugger.
</td>
</tr>

<tr>
<td>
<a href="./README.md#Enabling-profile-and-trace-options">Enabling Profile and Trace Options</a>
</td>
<td>
Explains how to enable profile and trace options in the Vitis IDE. Explores different event trace dump options like VCD and WDB.
</td>
</tr>

<tr>
<td>
<a href="./README.md#Deadlock-detection">Deadlock Detection</a>
</td>
<td>
Shows a practical deadlock scenario and some simulator options to identify the deadlock.
</td>
</tr>

<tr>
<td>
<a href="./README.md#Visualizing-deadlock-in-Vitis-Analyzer">Visualizing Deadlock in the Vitis Analyzer</a>
</td>
<td>
Explains how to visualize deadlock using trace information in the Vitis Analyzer, and identify the root cause.
</td>
</tr>

<tr>
<td>
<a href="./README.md#Debugging-memory-access-violations">Debugging Memory Access Violations</a>
</td>
<td>
Introduces an Out of Bound read access and explains how to identify the violations using the AI Engine simulator options.
</td>
</tr>

<tr>
<td>
<a href="./README.md#Single-kernel-debug">Single Kernel Debug</a>
</td>
<td>
Explains how to debug a single kernel design using a pipeline view in the Vitis IDE.
</td>
</tr>

<tr>
<td>
<a href="./README.md#Design-performance-debug">Design Performance Debug</a>
</td>
<td>
Shows how to calculate kernel latency and throughput using profile information.
</td>
</tr>

</table>

# Section 1

## Build and Simulate in the Vitis IDE

1. In the Vitis IDE toolbar, click the arrow next to the manage configurations button, and select **Emulation-AIE**. This sets the build target for the AI Engine simulation.

    ![build target](./Images/Build_target.PNG)
2. Right-click the *[aiengine]* domain project, select **C/C++ Build Settings**, choose **Compiler**, and add `-O0` to the Pre Processor Options. This improves the debug visibility.
![build setting](./Images/Build_setting.PNG)

3. Right-click the *[aiengine]* domain project, and select the **Build** option. Once the build completes, you see the **Compilation Complete** and **Build Finished** messages in the console.

4. Right-click the *[aiengine]* domain project, and select the **Run As → Launch AIE Emulator** option to start the simulation for the aiesimulation target. When the simulation completes, you see the following output in the console:
![aie simulator output](./Images/aiesimulator_output.PNG)
5. The aiesimulator output files from the design are located at `${PROJECT}/Emulation-AIE/aiesimulator_output/data`. Verify the output files `${PROJECT}/Emulation-AIE/aiesimulator_output/data/output_upscale.txt` and `${PROJECT}/Emulation-AIE/aiesimulator_output/data/output_data_shuffle.txt` against the golden files `${PROJECT}/data/golden_upscale.txt` and `${PROJECT}/data/output_data_shuffle.txt` to ensure that the design's I/O functionalities are correct. The Vitis IDE supports compare with the feature to compare two files. Highlight the files to be compared, then right-click one of highlighted files, and select **compare with** > **each other with transformation** . Click the **Predefined filters** icon(as shown below) > Enable **Remove timestamp** checkmark and click **OK** twice. Selecting **each other with transformation** is required because AI Engine simulation inserts a timestamp for each output data. For example, compare `${PROJECT}/data/golden_upscale.txt` and `${PROJECT}/Emulation-AIE/data/output_upscale.txt`.
![predefined filter](./Images/predefined_filter.PNG)

# Section 2

## Debug Using printf

The simplest form of tracing is to use a formatted `printf()` statement in the code for printing debug messages. Visual inspection of intermediate values and addresses can help you understand the progress of program execution. No additional include files are necessary for using `printf()` other than standard C/C++ includes (`stdio.h`). You can add `printf()` statements to your code to be processed during the aiesimulation or hardware emulation, and remove them or comment them out for hardware builds.

1. Open the source file, `src/kernels/upscale.cc`. Add the following lines after `int_t = *IniIter++;`:

    ```
    float * print_ptr = (float*)&in_t;
    for(int pp=0;pp<16;pp++)
        printf("Iteration-%d -> Vector-%d -> value = %f\n",i,pp,print_ptr[pp]);
    ```

    >**NOTE:** Adding `printf()` statements to your AI Engine kernel code will increase the compiled size of the AI Engine program. Be careful that the compiled size of your kernel code does not exceed the per-AI Engine processor memory limit of 16 KB.

2. Rebuild the *[aiengine]* project.
3. To enable the `printf()` function, it is required to specify `--profile` in run configuration.
![debug print(f)](./Images/debug_printf.PNG)
4. Click **Apply** and **Run**.
5. Observe the following `print` statements in the console:

    ```
    Configuring PL-Interface for graph mygraph...
    Set iterations for the core(s) of graph mygraph
    Enabling core(s) of graph mygraph 
    Waiting for core(s) of graph mygraph to finish execution ...
    Iteration-0 -> Vector-0 -> value = 0.000000
    Iteration-0 -> Vector-1 -> value = 0.000000
    ...
    ...
    Iteration-0 -> Vector-13 -> value = 0.000000
    Iteration-0 -> Vector-14 -> value = 0.000000
    Iteration-0 -> Vector-15 -> value = 0.000000 
    Iteration-1 -> Vector-0 -> value = 0.000000
    Iteration-1 -> Vector-1 -> value = 3.141500
    Iteration-1 -> Vector-2 -> value = 6.283000
    ```

6. Revert the changes in the source file to exercise other debug features.

# Section 3

## Debug Using the Vitis IDE Debugger

This section walks you through a debug methodology that uses the Vitis IDE debugger. You can learn how to invoke the debugger, add breakpoints, view intermediate values, etc.

1. After the design is built for the aiesimulation target, right-click the *[aiengine]* domain project, and select **Debug As** → **Launch AIE Emulator**.
2. This gets you to the debug mode in the Vitis IDE and waits in `o_buffer_main.h` waiting to acquire the lock to read the input data.
3. Open the source file, `peak_detect.cc`, and place the breakpoint at line 33 by double-clicking the blue space beside the line number in the source editor.

    >**NOTE:**
    >
    >1. You should see the stack trace corresponding to the all the kernel functions mapped to a particular tile. For example, in this case, the kernel `peak_detect` mapped to the tile [25,0]. So, when you select the `core[25,0]` in the `Debug` window, then only the breakpoint you set in the `peak_detect.cc` appears to be active. If not, it appears as red cross sign. Similarly for other tiles and kernel function breakpoints.
    >2. Sometimes you might see the yellow warning symbol at the breakpoint you placed. That indicates that the breakpoint is planted at some other line in the same source code. This is because the compiler optimized the particular operation, and the breakponts can only be placed at lines that are not optimized by the compiler.
    >
    >       ![breakpoint placement](./Images/breakpoint_placement.PNG)
    >
    >* `1` -> Selecting the core[25,0] in the debug window.
    >* `2` -> Indicates the current location of the pointer during the debug (explained more in later steps).
    >* `3` -> Breakpoint placed at a line that is optimized by compiler.
    >* `4` -> Breakpoint placed correctly which can be used to debug. Note a small blue tick mark.
    >

4. Notice the `resume`,`pause`,`terminate`,`disconnect`,`step-in`,`step-over`, and `step-return` options in the taskbar after entering debug mode.
5. You should be able to view the variables, breakpoints, registers, and set expressions to analyze the design at each iteration during the debug session.
![debug analysis view](./Images/debug_analysis_view.PNG)

6. After placing the breakpoint in `peak_detect.cc`, click the **Resume** button. You can see the debugger stops at that particular line. Hovering your mouse on any particular variable gives you the value at that particular instance of the run. For example, the address value of the `InIter` is `0x38000`.
![address of variable](./Images/address_of_variable.PNG)

7. Use the **Memory** monitor window to get the values at the address location, `0x38000`. Click the **+** button, and add the address `0x38000` to the memory monitor. You should see the values equal to the input values in `data/inx.txt`. Also, based on the location of breakpoint, you should be able to see the similar values in the **variable** view. If that variable is not visible in a particular iteration (due to compiler optimizations), you should still be able to track the value by adding in **Expressions**.

8. Click the **Expressions** and **Add new expression**, and type `vin`. You should see the initial zeros which matches with the values you see in the memory inspector. In each iteration, 16 values of 4-bytes (int32) are sent as an input in one iteration.

9. Now, click the resume button again, and you should see the following next set of 16 values in the `vin`.
![memory variable](./Images/memory_variable.PNG)

10. Hover you mouse on `OutIter_1`, get the memory address, and validate the values. As the `OutIter_1` is just a passthrough operation from the input to the output, you should see the same values as in `InIter`.

### Exercise Step

1. Click the **Instructor stepping mode** icon (click twice; this is a bug in the Vitis IDE) in the **Debug** window, and observe the disassembly view of the kernel code. You should be able to place breakpoints in this, and use `step-into` control to view the intermediate values.

2. Look at the **Register** view, and observe the changes in the register values, especially the program counter (PC) at each step.
![diassembly view](./Images/disassembly_view.PNG)>

3. Once you are done with the debugging, remove the breakpoint by double-clicking the breakpoint itself, and select the top-level project (in this case, PeakDetect) and the **Resume** button. You can see all cores goes into the `Disabled` state and also the `core(s) are done executing` message in the console.
4. Click the **Disconnect** button, and navigate to the **Design** mode from top right corner.

>**NOTE:** During the debug process, if any of the kernel status in the stack trace is shown as `Lock stall` or `Memory stall`, you might need to consider selecting the other core, and click `Resume` manually. This is becuase in the debug mode it is expected to run each individual core manually.

### Limitations

1. There are maximum four breakpoints available for each tile. Program under debug is stopped at first line of `main()`. This does not impact those available breakpoints.
2. Expect a non-sequential execution when stepping through the source code.
3. Due to compiler optimization, some variable values are stored in registers. `N/A` is shown in **variables** view for those optimized variables values.

# Section 4

## Enabling Profile and Trace Options

This section walks you through a flow to enable profiling and trace in the Vitis IDE. Using this, you should be able to get performance metrics and generate trace data which can be visualized in the Vitis Analyzer.

1. Right-click the *[aiengine]* domain project, and select **Run As** → **Run Configurations**.
2. Under **Main** tab, select **AI Engine Emulator**, and click **AI Engine Emulator for PeakDetect Emulation-AIE**.
3. Enable the check box **Generate Trace**, and leave the default options **VCD** with the filename *foo*.
4. Enable the **Generate Profile** check box, and leave the other default options under that.

    >**NOTE:**  You can also try enablling profile generation for selected tiles.
    >![profile trace run config](./Images/Profie_trace_runconfig.PNG)

    Launch the aiesimulation by selecting **Run As → Launch AIE Emulator**.<br />
5. Once the Run completes, right-click `${PROJECT}/Emulation-AIE/aiesimulator_output/default.aierun_summary`, and select **Open with** -> **Vitis Analyzer Report**. This opens the following Vitis Analyzer window.
![profile trace in vitis analyzer](./Images/profile_trace_va.PNG)

6. Click the **Profile** -> select the **Summary** and **Profile Details** corresponding to all the tiles ([24,0],[25,0],[25,1]), and observe the cycle count, instruction count, and program memory size. More information about the profile details is explained in the [Design Performance Debug](./README.md#Design-performance-debug) section.

7. Click **Trace**, and observe the trace events corresponding to all the tiles. For example, select the core `[25,0]` corresponding to the kernel `peak_detect`. Expand the kernel function and zoom in to check the input and output values.
![trace kernel io](./Images/trace_kernel_io.PNG)

    >**NOTE:** If the VCD file generated during AI Engine simulation is too large, it takes too much time for the Vitis Analyzer to analyze the VCD and open the trace view. Alternatively, you can do an online analysis of the VCD when running the AI Engine simulator using the WDB and CTF files. To generate this, you need to choose **Online** option instead of **VCD** in step 3.

### Exercise Step

You should be able to add arguments `--dump-vcd=foo`, `--online -wdb` in the **Run As** -> **Run configurations** -> **Arguments** tab, and open the event trace in the Vitis Analyzer using the `default.aierun_summary` file. You can open the `.wdb` file directly from **File** -> **%Fill%** in the Vitis Analyzer.

 ![argument wbd](./Images/argument_wdb.PNG)

# Section 5

## Deadlock Detection

AI Engine designs can run into simulator hangs. A common cause is insufficient input data for the requested number of graph iterations, a mismatch between production and consumption of stream data, cyclic dependency with stream, cascade stream or asynchronous buffers, or the wrong order of blocking protocol calls (acquisition of async buffers, read/write from streams).

This topic walks you through the practical scenario of deadlock during aiesimulation and different the simulator options that helps debugging.

1. Open the `src/kernels/data_shuffle.cc`, and comment out the line 24.
2. Compile the design by rebuilding the *[aiengine]* domain project.
3. Run the aiesimulation by right-clicking the *[aiengine]* domain project and **Run As** → **Launch AIE Emulator**, and observe the hang.
4. You can wait for few seconds to confirm the hang, and click the icon located in bottom right corner that shows background operations in progress view and the kill the simulation process.
![simulation hang](./Images/simulation_hang.PNG)
5. The AI Engine simulator provides an option to exit the simulation if all active cores in stalled state after the time period (in ns).
6. For example, add `--hang-detect-time=60` in the **Run configurations** -> **Arguments**, and rerun the aiesimulation. You can observe the simulation exits smoothly with the following information in the console.

    ```
    Enabling core(s) of graph mygraph
    [WARNING]: T=580800 ps All Cores in Hang State 
    Waiting for core(s) of graph mygraph to finish execution ...
    [WARNING]: T=653600 ps All Cores in Hang State 
    [WARNING]: T=1846400 ps All Cores in Hang State 
    [WARNING]: T=1906400 ps All Cores in Hang State for 60ns
    |---------------- Core Stall Status ----------------|
    (24,1) -> Stream stall ->  SS0  at T=1702400 ps
    (25,1) -> Lock stall ->  Lock_West  at T=1748800 ps
    (25,2) -> Lock stall ->  Lock_East  at T=1846400 ps
    |---------------------------------------------------|
    [CRITICAL WARNING]: Closing Simulator 
    Exiting!
    ```

7. Revert the changes in the source file to exercise other debug features.

More information about how to visualize the deadlock using the stream stalls and lock stalls in the Vitis Analyzer is explained in [Visualizing Deadlock in the Vitis Analyzer](./README.md#Visualizing-deadlock-in-Vitis-Analyzer).

 > **NOTE:**
 >
 >1. The hang detect time should be selected approximately based on the complexity of the kernels, graph, and also on the number of iterations the graph is running.
 >2. If the simulator option, `--simulation-cycle-timeout=cycles`, is also specified, care should be taken that the `--hang-detect-time` should be less than the the timeout. Also note that the timeout is in cycles.
 >3. To convert the cycles to the `ns`, you need to consider the AI Engine clock frequency.

# Section 6

## Visualizing Deadlock in the Vitis Analyzer

This topic is a continuation of the [Deadlock Detection](./README.md#Deadlock-detection) section and walks you through the visualization of the deadlock (Lock stall and stream stall) in the Vitis Analyzer.

1. After simulating with the `--hang-detect-time=60` option, open the `default.aierun_summary` file in the Vitis Analyzer.
2. Click **Trace**, and expand the `data_shuffle` kernel function. It ran for only two iterations and went into a stream stall.
![stream stall](./Images/stream_stall.PNG)
3. Expand the `peak_detect` kernel function, and observe the lock stall after three iterations.
![lock stall](./Images/lock_stall.PNG)
The kernel `data_shuffle` expects more stream data from the kernel `peak_detect` which it cannot provide. So, the stream port in the `data_shuffle` went into the stream stall, and the kernel hangs. It cannot accept more inputs from buffer port which causes the back pressure on `peak_detect` kernel leading to lock stall, which is the potential root-cause for the deadlock.

# Section 7

## Debugging Memory Access Violations

Memory access violations occur when a kernel is reading or writing out of bounds of an object or reading uninitialized memory. This can manifest itself in multiple ways, such as a simulator crash or hang. This debug feature helps to find out of range memory access from each tile during the AI Engine emulation runtime; however, this option impacts runtime performance negatively. The 'out of range' memory access indicates valid address assignment within each section. There could be certain addresses that are not assigned between sections.

The aiesimulator option `--enable-memory-check` helps to find out these out of range memory accesses.

Besides using the simulator option, it is necessary to first identify an invalid address from the design. To do this:

1. In a Linux terminal where a valid Vitis installation/setup is done, issue the following command to list a specific tile's valid memory addresses and sizes assigned by the AI Engine compiler:

    For example, to get valid memory addresses of `peak_detect` kernel:

    ```
    # For Vitis IDE project
    cd ${PROJECT_PATH}/Emulation-AIE/Work/aie/25_0/Release
    readelf -S 25_0
    ```

2. The output of the `readelf` command is as follows.

    ```
    readelf -S 25_0
    There are 33 section headers, starting at offset 0x47484:

    Section Headers:
      [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
      [ 0]                   NULL            00000000 000000 000000 00      0   0  0
      [ 1] .shstrtab         STRTAB          00000000 00268f 00016c 00      0   0  1
      [ 2] .strtab           STRTAB          00000000 0027fb 000872 00      0   0  0
      [ 3] .symtab           SYMTAB          00000000 00306d 000410 10      2  41  0
      [ 4] .bss.DMb.16       NOBITS          00029e00 000294 000200 00  WA  0   0  1
      [ 5] .bss.DMb.16       NOBITS          0002c000 000294 000200 00  WA  0   0  1
      [ 6] .bss.DMb.16       NOBITS          00031e00 000294 000200 00  WA  0   0  1
      [ 7] .data.DMb.4       PROGBITS        000357e0 000294 000020 00  WA  0   0  1
      [ 8] .data.DM_bankA.4  PROGBITS        00035c00 0002b4 000024 00  WA  0   0  1
      [ 9] .bss.DMb.16       NOBITS          00038000 0002d8 000200 00  WA  0   0  1
      [10] .bss.DMb.16       NOBITS          0003a000 0002d8 000200 00  WA  0   0  1
      [11] .bss.DMb.16       NOBITS          0003c000 0002d8 000200 00  WA  0   0  1
      [12] .text             PROGBITS        00000000 0002d8 000102 00  AX  0   0  1
      [13] .text             PROGBITS        00000110 0003da 0002d8 00  AX  0   0  1
      [14] .text             PROGBITS        000003f0 0006b2 0003de 00  AX  0   0  1
      [15] .text             PROGBITS        000007d0 000a90 0000be 00  AX  0   0  1
      [16] .text             PROGBITS        00000890 000b4e 000094 00  AX  0   0  1
      [17] .debug_line       PROGBITS        00000000 00347d 00169b 00      0   0  0
      [18] .debug_info       PROGBITS        00000000 004b18 00a78c 00      0   0  0
      [19] .debug_abbrev     PROGBITS        00000000 00f2a4 000755 00      0   0  0
      [20] .debug_loc        PROGBITS        00000000 00f9f9 005581 00      0   0  0
      [21] .debug_frame      PROGBITS        00000000 014f7a 000d54 00      0   0  0
      [22] .debug_ranges     PROGBITS        00000000 015cce 0017b8 00      0   0  0
      [23] .debug_str        STRTAB          00000000 017486 023195 00      0   0  0
      [24] .debug_pubnames   PROGBITS        00000000 03a61b 00ad75 00      0   0  0
      [25] .debug_pubtypes   PROGBITS        00000000 045390 00208c 00      0   0  0
      [26] .tctmemstrtab     STRTAB          00000000 000be2 000a78 00      0   0  0
      [27] .tctmemtab        LOPROC+0x123456 00000000 04741c 000068 08     26   0  0
      [28] .tctmemtab        LOPROC+0x123467 00000000 00165a 000068 08     26   0  0
      [29] .stackinfo        LOPROC+0x123458 00000000 0016c2 000010 10      0   0  0
      [30] .rtstab           LOPROC+0x123469 00000000 0016d2 0000a9 0d      0   0  0
      [31] .eoltab           LOPROC+0x123470 00000000 00177b 0008f4 0c      0   0  0
      [32] .chesstypean[...] LOPROC+0x123468 00000000 00206f 000620 10      0   0  0
    Key to Flags:
      W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
      L (link order), O (extra OS processing required), G (group), T (TLS),
      C (compressed), x (unknown), o (OS specific), E (exclude),
      D (mbind), p (processor specific)
    ```

    Here, the flags `WA` and `AX` indicates, this section will be loaded into tile memory.

3. Add the memory read violation to the kernel code by opening `src/kernels/peak_detect.cc`, and change the line-26 to `v_in = *(InIter+8500000500)`.
4. Build the *[aiengine]* domain project, add the `--enable-memory-check` option to the **Run Configurations**, and run the aiesimulation.
5. Observe the following messages in the console.

    ```
    Waiting for core(s) of graph mygraph to finish execution ...
    660 ns [ERROR] tl.aie_logical.aie_xtlm.math_engine.array.tile_25_1.cm.proc: dme_lda_n_out access out of boundary! address = 0x000013d20 prog_cntr = 0x0462
    660 ns [ERROR] tl.aie_logical.aie_xtlm.math_engine.array.tile_25_1.cm.proc: dme_ldb_n_out access out of boundary! address = 0x000013d00 prog_cntr = 0x0462

    Warning: (W545) sc_stop has already been called
    In file: sc_simcontext.cpp:1051
    In process: tl.aie_logical.aie_xtlm.math_engine.array.tile_25_1.cm.proc.dm_access_adr_check @ 660 ns
    660 ns [ERROR] tl.aie_logical.aie_xtlm.math_engine.array.tile_25_1.cm.proc: dmo_lda_n_out access out of boundary! address = 0x000013d30 prog_cntr = 0x0462
    660 ns [ERROR] tl.aie_logical.aie_xtlm.math_engine.array.tile_25_1.cm.proc: dmo_ldb_n_out access out of boundary! address = 0x000013d10 prog_cntr = 0x0462
    ```

    The address `0x000013d30` is out of range from the valid addresses you see from the `readelf` command.
6. The AI Engine simulation generates the `${PROJECT_PATH}/Emulation-AIE/AIESim_Guidance.json` file which can be viewed when the `${PROJECT_PATH}/Emulation-AIE/aiesimulator_output/default.aierun_summary` file is opened in the Vitis Analyzer.
![aiesim guidance](./Images/aiesim_guidance.PNG)

7. The **PC** column in the **Memory Violations** tab helps redirecting to the kernel function that has a memory violation.

    >**NOTE:** Currently, if the violation impacts the API, e.g., `aie::mul` in this case, clicking the `PC` might not redirect you to the exact kernel function. A general recommendation in these kind of cases where memory violations cannot be identified is to use the `x86simulator` with the `valgrind` option as explained in [Memory Access Violation and Valgrind Support for the x86simulator](../X86_Simulation/README.md#memory-access-violation-and-valgrind-support).

8. Revert the changes in source code to exercise other debug features.

# Section 8

## Single Kernel Debug

The AI Engine Pipeline view in the Vitis IDE allows you to correlate instructions executed in a specific clock cycle with the labels in the Disassembly view. The underlying AI Engine pipeline is exposed in debug mode using the pipeline view. The Vitis IDE only supports the pipeline view for graphs containing single kernels.

That is, the pipeline view is supported when your graph has AI Engine kernels targeted to a single AI Engine tile. There can only be only one single kernel in a graph or multiple kernels mapped to a single tile for the pipeline view to be enabled. So to get the pipleline view, do the following source code changes to the Peakdetector design.

1. Clone the git repository, and copy and replace all files from `{GIT_TUTORIAL_PATH}/AIE_Simulation/src/kernels/` with the files in `${PROJECT_PATH}/src/kernels/`. It is recommended to make a copy of the original files before replacing.
    > **NOTE:** These source changes are done to accommodate the kernels in a single tile. As the original sources contains stream connections in between two kernels and two stream connections to PLIO; they cannot fit in a single AI Engine tile.

2. Also, replace the `{GIT_TUTORIAL_PATH}/AIE_Simulation/src/graph.h` with the `${PROJECT_PATH}/src/graph.h`.

3. Observe the runtime ratios in the `graph.h`. This change from the original design makes all the kernels to be placed in a single AI Engine tile.

4. Rebuild the design, and observe the graph view in the Vitis Analyzer. Observe the single buffer between two kernels instead of ping-pong buffers. This is because now two kernels are mapped to a single AI Engine core and ping-pong buffers are not required.

5. To enable the Pipeline view on graphs with a single kernel, select **Generate Profile** from the Debug configurations.

6. Enter the debug, as explained in the [Debug Using Vitis IDE Debugger](./README.md#Debug-using-Vitis-IDE-debugger) section, to observe only a single core in the **Debug** window. The  **Pipeline View** automatically comes up as soon as IDE enters the debug mode.
![pipeline view](./Images/pipeline_view.PNG) 

7. Click the **Step Over** button, and observe the changes in the pipeline view as well as the **Cycle Count**,**Program Counter** in Runtime statistics. The main advantage of the pipeline view is when you want to observe how many cycles a particular instruction takes.

8. Once the debug is done, hit the **Resume** button to complete the simulation or the **Terminate** button to exit the debugger smoothly.

9. Revert all the source changes to exercise the other debug features.

# Section 9

## Design Performance Debug

To estimate the design performance during the AI Engine simulation, it is necessary to analyze the profile results carefully. This section walks you through some topics that are most commonly used to assess how your kernel is performing overall.

Refer to the Section 4 [Enabling the Profile and Trace Options](./README.md#Enabling-the-profile-and-trace-options) to understand how to enable profiling in the Vitis IDE.

1. After running the AI Engine Simulation, open the `default.aierun_summary` file in the Vitis Analyzer, and click the **Profile**.
2. You can click the **Summary** corresponding to each tile in the landing page, and observe the cycle count, instruction count, and program memory.
3. Now, under the **Function Reports**, click the **Total Function Time** to observe the following table at the bottom for the `data_shuffle` kernel function.
![profile function time](./Images/profile_function_time.PNG)

    * The `data_shuffle` kernel function took 2,305 cycles for seven iterations, i.e., ~329 cycles for one iteration which is the **Avg Function Time**.
    * The `main` function is added by the compiler and different from the `main()` function in the `graph.cpp` file. This function took 1,461 cycles in total which includes the time to transfer control back and forth between each graph iteration, lock stalls, etc.
    * The `_main_init` runs once for all graph iterations, and it took 181 cycles.
    * The `_cxa_finalize` function took 41 cycles to call the destructors of the global c++ objects.
    * The `_fini` function executes the program terminating instructions, and it took 28 cycles.

4. If you click the AI Engine Simulation **Summary**, you can notice the AI Engine Frequency as `1250 MHz` i.e., `0.8ns`, i.e., `1` cycle = `0.8 ns`
    Now, the data_shuffle function took `329` cycles for `1` iteration, i.e., `329 * 0.8 ~= 264 ns`.

5. Try to match these valuess with the trace information. Click **Trace**, and zoom in to the period of one iteration (between two `main()` function calls as follows), and add a marker and drag it to the end of the kernel function.
![trace function time](./Images/trace_function_time.PNG)

    The difference between the starting time and end time of the kernel function for one iteration matches with the 264 ns seen in the profiling results.

### Calculating the Graph Throughput Using Graph Output

1. From the trace information in the run_summary in the Vitis Analyzer, navigate to the output port for which you want to calculate the throughput (`upscale` kernel in this case). Add a marker at the start of the first output sample as highlighted as follows. Then click the `Go to last time` icon, and observe the cursor moves to the end of the last iteration. Now, click the **previous transition** icon to go the start of the last iteration. Add one more marker at the end, and observe the time difference as `2273.600 ns`.
![trace graph throughput](./Images/trace_graph_throughput.PNG)
2. The number of bytes transferred is `128 samples * 4 Bytes * 7 iterations` = `3584` bytes.
3. Throughput = 3584/2273 * e-9 ~= 1.5 GBPS.

    Theoratically, AI Engine can transfer four bytes per cycle (in this case, 0.8 ns). So, to transfer `3584` bytes of data, it requires 896 cycles (896 * 0.8 = 716 ns). So, the theoratical througput is 5 GBPS.

## Support

GitHub issues will be used for tracking requests and bugs. For questions, go to [support.xilinx.com](https://support.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
