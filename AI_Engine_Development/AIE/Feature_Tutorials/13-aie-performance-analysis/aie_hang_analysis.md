</table>
<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# AI Engine Deadlock Analysis

***Version: Vitis 2023.1***

This tutorial introduces you to some common deadlock scenarios and shows you how to detect deadlocks (design hangs) in different tool flows. The methods introduced to detect and analyze deadlock issues include:

1. Using `event` in AMD Vitis&trade; Analyzer to analyze design hangs.

2. Using waveforms in hardware emulation to check AI Engine input and output activities.

3. Using event APIs to analyze data activities for AI Engine input and output in hardware flows.

4. Using `xbutil` to report AI Engine and AI Engine shim status.

5. Using the `devmem` Linux command to probe AI Engine registers. 

**Note:** The default working directory in this step is `testcase_nofifo_hang`, unless explicitly stated otherwise.

## Common Deadlock Scenarios

A deadlock is usually caused by insufficient FIFOs, or the access rate not matching between multiple FIFOs (or different branches of the same net in stream multicast). The following figure shows some deadlock scenarios:

![Deadlock Scenarios](./images/figure5.PNG)

**Scenario 1:** This scenario occurs when K1 tries to write to FIFO0, but the FIFO is full. K2 is still waiting for data coming from FIFO1 before consuming data from FIFO0. 

**Scenario 2:** This scenario occurs when NET0 multicasts to multiple destinations, and the destinations are connected by stream or cascade stream (FIFO1). The NET0 branch 1 is full because K2 is waiting for data from FIFO1, but K1 is still hungry for data from NET0 branch 0 to produce data for FIFO1. 

**Scenario 3:** This scenario occurs when K1 and K2 are connected by buffers (including RTP buffers) and streams. When K1 is trying to write data to K2 using FIFO0, K2 is still trying to acquire lock for the ping or pong buffer. K1 will not release the lock of the buffer until it finishes its current iteration. 

## AI Engine Deadlock Example and Analysis in AI Engine Simulator 

The example is similar to the one used in [AI Engine Execution and Measurement](./aie_execution_measurement.md), except that it does not have a FIFO for the stream connection:

![Deadlock Example Graph](./images/figure6.PNG)

When the design stalls, `graph::wait()` and `graph::end()` hang. It needs to interrupt graph execution by:

* Using `graph::wait(CYCLE_NUMBER)`: Specifying the number of cycles to wait for the API to return (if the graph does not return after `CYCLE_NUMBER` cycles, this API still returns immediately).
* Using `graph::end(CYCLE_NUMBER)`: Specifying the number of cycles to wait for the graph to be ended (if the graph does not return after `CYCLE_NUMBER` cycles, this API still ends the graph immediately).
* Using the `--simulation-cycle-timeout CYCLE_NUMBER` option for `aiesimulator`.

The `CYCLE_NUMBER` should be large enough for AI Engine simulator to record all the stall events, or for hardware to run into hang status. 

1. In this example, examine `aie/graph.cpp`. We wait for 10000 cycles:

	```
	gr.init();
  	gr.run(4);
  	gr.wait(10000);
	```

2. Run AI Engine simulator using the following command:

	```
	make aiesim
	```

3. Open **Trace** view in Vitis Analyzer by using the following command:

    ```
	vitis_analyzer aiesimulator_output/default.aierun_summary
    ```

	![Trace View](./images/figure7.PNG)

	The hang occurs after the following activities:

	**1:** Kernel `aie_dest1` acquires the lock of read buffer (`buf0`) and write buffer (`buf1`).

	**2:** Kernel `aie_dest1` starts.

	**3:** Kernel hangs in stream stall.

	**4:** S2mm is waiting for kernel `aie_dest1` to release buffer `buf0`. 

### AI Engine Stall Analysis with Vitis Analyzer

Vitis Analyzer can use the event trace from the AI Engine simulation to do stall analysis that shows an overview of the stall status in metrics. It also helps you determine where the stall has happened, and the possible causes.

If you are using Vitis Analyzer to do stall analysis, run the AI Engine simulator with `--online -wdb -ctf` options to generate event trace information in the background:

	aiesimulator --pkg-dir=./Work --online -wdb -ctf

**Note:** For more information about AI Engine stall analysis using Vitis Analyzer in the hardware emulation flow, refer to the _Versal ACAP AI Engine Programming Environment User Guide_ ([UG1076](https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment)).

In Vitis Analyzer, the Performance Metrics view gives an overview of the stalls in the design:

![Performance Metrics View](./images/a.PNG)

Each tile shows percentages for each type of stall. From the metrics table, it can be seen that tile (24,0) has a large percentage of lock stall (98.896%), and tile (25,0) has a large percentage of stream stall (98.380%). These metrics indicate that the design is hanging, and that analysis is required.

In the Graph view of Vitis Analzyer, you can visualize the stalled path in the graph, which gives an indication of where the stall has happened in the design. By understanding the design behavior, it is also possible to estimate the cause of the hang. 

For example, select the stream stall in Trace view, and switch to Graph view. In this design, kernel `k[0]` hangs in stream stall. The full destination port is `gr.k[1]/in`, which means that the destination kernel `k[1]` is not receiving data from the stream. 

![Stream stall in Trace View](./images/b_0.PNG)

![Stream stall in Graph View](./images/b.PNG)

Select the stall in **Lock Stalls** in Trace view, and switch to Graph view. The red path to kernel `k[1]` shows that it is trying to acquire the lock of the input buffer, but without success. The white path shows that kernel `k[0]` is holding the lock of the buffer.

![Lock stall in Trace View](./images/c_0.PNG)

![Lock stall in Graph View](./images/c_0.PNG)

From the above analysis, the cause of the hang is given. The direct resolution, without modifying kernel code, is to increase the FIFO size between the two kernels.

For more information about AI Engine stall analysis feature, refer to the _Versal ACAP AI Engine Programming Environment User Guide_ ([UG1076](https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment)).

## AI Engine Deadlock Detection in the Hardware Emulation Flow

Like AI Engine simulator, the hardware emulation flow can also dump VCD for AI Engine. To dump VCD, write `AIE_DUMP_VCD=foo` in a file and specify this file for the `-aie-sim-options` option of `launch_hw_emu.sh`. 

It is usually helpful to view the input and output of AI Engine in the waveform. The `-g` option can be added to `launch_hw_emu.sh` to launch the XSIM waveform. The command looks like the following:

	
    ```
	./launch_hw_emu.sh -g -aie-sim-options ./sim_options.txt
	```
	
    
When the XSIM GUI pops up, add `ai_engine_0` or the PL kernels' signals to the waveform. Click **Start** in XSIM. In the Linux prompt, run the following command:

	
	```
	cd /run/media/mmcblk0p1
	./host.exe a.xclbin
	```
	

After the PS code completes in Linux, check the input to the AI Engine `S00_AXIS` and the output from the AI Engine `M00_AXIS`:

![XSIM](./images/figure8.PNG)

After the PS receives 104 samples, the design hangs. `TVALID` is always High, indicating that the PL kernel `mm2s` is still trying to send data to the AI Engine, but `TREADY` from the AI Engine turns to Low, and remains Low. There are also no samples from the AI Engine to the PL in the `M00_AXIS` interface of the AI Engine.

Analysis of the VDC file in Vitis Analyzer is similar to analysis in AI Engine simulator. A workaround is to move the VCD file generated by the hardware emulation flow to the working directory and open it:

	
	```
	vitis_analyzer aiesimulator_output/default.aierun_summary
	```
	

## AI Engine Deadlock Detection in the Hardware Flow

If a deadlock does not show in the AI Engine simulator or hardware emulation flows, it might still show in the hardware flow.

The PS code to profile how much data has been transferred for the input and output is shown below:

	```
	event::handle handle = event::start_profiling(*dout, event::io_stream_running_event_count);
	event::handle handle2 = event::start_profiling(*din, event::io_stream_running_event_count);
	if(handle==event::invalid_handle || handle2==event::invalid_handle){
		printf("ERROR:Invalid handle. Only two performance counter in a AIE-PL interface tile\n");
		return 1;
	}

	//kernel run
	auto s2mm_run = s2mm(out_bo, nullptr, OUTPUT_SIZE);//1st run for s2mm has started
	auto mm2s_run = mm2s(in_bo, nullptr, OUTPUT_SIZE);
	gr.run(4);
	// Wait graph for some cycles
	gr.wait(50000); // wait for AIE kernel to complete or at most 50000 cycles

	long long data_out_count = event::read_profiling(handle);
	long long data_in_count = event::read_profiling(handle2);
	event::stop_profiling(handle);
	event::stop_profiling(handle2);
	std::cout<<"Output data received:"<<data_out_count<<std::endl;
	std::cout<<"Input data sent:"<<data_in_count<<std::endl;
	```
	
	
  **Note:** `mm2s` needs to be started after `event::start_profiling`. Otherwise, the data transfer begins after `mm2s` starts, and that happens before `event::start_profiling` and `gr.run(4)`.

The output is similar:

	
	```
	Output data received:0
	Input data sent:104
	```
	
	
From how much data has been transferred for the input and output, the status of the design can be estimated. The `graph.wait(50000)` in the above code can be replaced with `sleep` or `usleep` APIs to wait a certain amount of time depending on the scale of the design.

If necessary, an Integrated Logic Analyzer (ILA) can be inserted to probe the interfaces of the PL kernels to detect the AI Engine and PL kernels' running status.

Refer to [AI Engine Status Analysis](./aie_status_analysis.md) for how to use Vitis Analyzer to understand the AI Engine status in both hardware and hardware emulation.

### Conclusion

After completing this tutorial, you have learned how to detect and analyze design hang issues.

## Appendix (Optional)

This section provides details of other methods of detecting and analyzing AI Engine running status.

### Manual Dump and Register Reading to Detect AI Engine Status in Hardware Emulation and Hardware

- **Using `xbutil` to report graph running status:** The following command can be used to report graph running status:

	```
	xbutil examine -r aie -d 0
	```
	
	The output of above command is as follows:

		```
		----------------------
		[0000:00:00.0] : edge
		----------------------
		Aie
		  Aie_Metadata
		  GRAPH[ 0] Name      : gr
		            Status    : unknown
		    SNo.  Core [C:R]          Iteration_Memory [C:R]        Iteration_Memory_Addresses
		    [ 0]   24:0                24:0                          8324
		    [ 1]   25:0                24:0                          7012
		
		Core [ 0]
		    Column                : 24
		    Row                   : 0
		    Core:
		        Status                : disabled, east_lock_stall
		        Program Counter       : 0x000001ca
		        Link Register         : 0x000000b0
		        Stack Pointer         : 0x0003a0c0
		    DMA:
		        MM2S:
		            Channel:
		                Id                    : 0
		                Channel Status        : idle
		                Queue Size            : 0
		                Queue Status          : okay
		                Current BD            : 0
		
		                Id                    : 1
		                Channel Status        : idle
		                Queue Size            : 0
		                Queue Status          : okay
		                Current BD            : 0
		
		        S2MM:
		            Channel:
		                Id                    : 0
		                Channel Status        : stalled_on_requesting_lock
		                Queue Size            : 0
		                Queue Status          : okay
		                Current BD            : 0
		
		                Id                    : 1
		                Channel Status        : idle
		                Queue Size            : 0
		                Queue Status          : okay
		                Current BD            : 0
		
		    Locks:
		        0                     : acquired_for_read
		        1                     : released_for_read
		        2                     : acquired_for_write
		        3                     : released_for_write
		        4                     : released_for_write
		        5                     : released_for_write
		        6                     : released_for_write
		        7                     : released_for_write
		        8                     : released_for_write
		        9                     : released_for_write
		        10                    : released_for_write
		        11                    : released_for_write
		        12                    : released_for_write
		        13                    : released_for_write
		        14                    : released_for_write
		        15                    : released_for_write
		
		    Events:
		        core                  : 1, 2, 5, 22, 26, 28, 29, 31, 32, 35, 38, 39, 44, 73, 74, 78, 82, 86, 90, 94, 98, 102, 106, 114
		        memory                : 1, 20, 21, 25, 33, 43, 44, 45, 46, 47, 48, 76, 78, 106, 113
		
		Core [ 1]
		    Column                : 25
		    Row                   : 0
		    Core:
		        Status                : disabled, stream_stall_ms0
		        Program Counter       : 0x00000310
		        Link Register         : 0x00000220
		        Stack Pointer         : 0x00029ba0
		    DMA:
		        MM2S:
		            Channel:
		                Id                    : 0
		                Channel Status        : idle
		                Queue Size            : 0
		                Queue Status          : okay
		                Current BD            : 0
		
		                Id                    : 1
		                Channel Status        : idle
		                Queue Size            : 0
		                Queue Status          : okay
		                Current BD            : 0
		
		        S2MM:
		            Channel:
		                Id                    : 0
		                Channel Status        : idle
		                Queue Size            : 0
		                Queue Status          : okay
		                Current BD            : 0
		
		                Id                    : 1
		                Channel Status        : idle
		                Queue Size            : 0
		                Queue Status          : okay
		                Current BD            : 0
		
		    Locks:
		        0                     : released_for_write
		        1                     : released_for_write
		        2                     : released_for_write
		        3                     : released_for_write
		        4                     : released_for_write
		        5                     : released_for_write
		        6                     : released_for_write
		        7                     : released_for_write
		        8                     : released_for_write
		        9                     : released_for_write
		        10                    : released_for_write
		        11                    : released_for_write
		        12                    : released_for_write
		        13                    : released_for_write
		        14                    : released_for_write
		        15                    : released_for_write
		
		    Events:
		        core                  : 1, 2, 5, 22, 23, 24, 28, 29, 31, 32, 35, 38, 39, 41, 44, 73, 74, 75, 76, 78, 79, 80, 82, 83, 84, 86, 87, 88, 90, 91, 92, 94, 95, 96, 98, 99, 100, 102, 103, 104, 106, 114
		        memory                : 1		
		```

		
	**Tip:** If a design hangs in Linux, press **Ctrl+Z** to suspend the design and run command.

	It is seen that `core[0]` (tile_24_1, `aie_dest2`)  is in the status `east_lock_stall`, and `core[1]` (tile_25_1, `aie_dest1`) is in the status `stream_stall_ms0`. That is, `aie_dest1` is trying to write to the consumer `aie_dest2`, while `aie_dest2` is still trying to acquire lock to start.

	**Tip:** Cross-probe between **Graph** and **Array** view in Vitis Analyzer to understand kernels, buffers, and the locations of ports. 
`
- **Using `devmem` to probe AI Engine registers to see AI Engine status:** By using the `devmem` command, you can read AI Engine registers to see the AI Engine internal status. The register reference can be found in the _Versal ACAP AI Engine Register Reference_ ([AM015](https://www.xilinx.com/html_docs/registers/am015/am015-versal-aie-register-reference.html)).

	For example, the core status registers can be found here:

	![core status](./images/figure11.PNG)

	Find the absolute addresses for the kernels in the design. The status of the kernels can be read by running the following command:

		versal-rootfs-common-20231:/run/media/mmcblk0p1# devmem 0x2000C872004
		0x00001000
		versal-rootfs-common-20231:/run/media/mmcblk0p1# devmem 0x2000C072004 
		0x00000200
		```


	Value `0x00001000` indicates that it is `Stream_Stall_MS0`, and value `0x00000200` indicates that it is `Lock_Stall_E`. The analysis of the result is similar to using `xbutil`. 

### Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
