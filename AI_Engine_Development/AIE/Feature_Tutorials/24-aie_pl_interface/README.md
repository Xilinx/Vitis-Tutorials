<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ AI Engine Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
        </br>
      <a href="https://www.amd.com/en/products/software/vitis-ai.html">See Vitis™ AI Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# RTL / AI Engine interfacing Examples 

***Version: Vivado / Vitis 2026.1***

## Introduction

This tuorial is demonstrating how you can connect the AMD Versal™ AI Engine to the programmable logic (PL) using AXI compliant AXI4-Stream interfaces. 

In most examples, connectivity between the PL and the AI Engine uses high‑level synthesis (HLS) kernels connected to the double data rate (DDR) memory. This is because the default VCK190 base platform includes only infrastructure blocks. To accelerate part of an application, you minght want to connect the AI Engine to your existing RTL design. To achieve this, you need knowledge of the AXI-Stream protocol and its use for AI Engine connections in the Vitis acceleration flow. Your RTL might require some modification or bridging from the existing RTL. For example, to communicate with the AI Engine, the RTL needs to have a compliant AXI4-Stream interface (it needs to be able to handle back pressure when `tready` is Low).
This tutorial shows multiple ways for connecting RTL blocks with AXI4-Stream present in a custom platform to the AI Engine using the Vitis Acceleration flow.
  
---
## Part 1 - Connecting RTL AXI4-Stream Interfaces (included in Block Design) to the AI Engine

### Platform

In this first example, we are creating a custom platform from a Vivado project with a Block Design which includes RTL blocks with AXI4-Stream master and slave interfaces connected to the AI Engine.

### Hardware Platform Creation
In this example, we assume you are creating the vivado platform starting from the Versal extensible platform configurable example design (CED) included in AMD Vivado™.

![Versal Extensible Platform CED](./images/26.1_versal_extensible_example.jpg)

Then from the Versal extensible platform cCED we are adding custom IPs to the block design. The first one, `AXI4S_Counter` is a counter which generates data on a 64-bit AXI4-Stream interface. The second one, `dummy Sink`, is an AXI4-Stream slave which accepts any data and drops it (`tready` is always High). The AXI4-Stream interfaces of these two IP blocks are not connected to any slave or master interfaces in Vivado. We will connect them to the AI Engine using the V++ linker in the Vitis flow.

![Custom IPs](./images/261_custom_IPs.jpg)

>***Note:*** The AI Engine expects AXI4-Stream-compliant interfaces connected to its AXI4-Stream interfaces. Although the following guide focuses on designing Video IPs, it also contains considerations useful for designing any AXI4-Stream IP.
>
> [UG934 - AXI4-Stream Video IP and System Design](https://docs.amd.com/r/en-US/ug934_axi_videoIP/AXI4-Stream-Video-IP-and-System-Design-Guide)

For the V++ linker to detect the two AXI4-Stream interfaces, add them to the platform properties. Assign each a unique SP tag.

Use the following Tcl commands:
```
set_property PFM.AXIS_PORT {M00_AXIS {type "M_AXIS" sptag "master_axi_1" is_range "false"}} [get_bd_cells /AXI4S_Counter_0]
set_property PFM.AXIS_PORT {S00_AXIS {type "S_AXIS" sptag "slave_axi_1" is_range "false"}} [get_bd_cells /dummy_sink_0]
```
Or you can do this through the Vivado GUI using the Platform tab.

![Platform settings](./images/261_AXIS_pfm_settings.jpg)

> ***Note***: The preceding flow assumes that the RTL AXI4-Stream interfaces are part of the block design (BD). In some designs, the RTL exists outside the BD. In that case, you can simply add an interface port to the BD set as AXI4-Stream (xilinx.com:interface:axis_rtl:1.0). Then add an IP in the BD to pass through all interface nets. The option appears in the **Part 2**.

You can generate the Vivado Platform with the following make command:
```
make vivado_platform
```
The Vivado project generates under `Vivado/build/custom_pfm_strmIn_strmOut`.

### Vitis V++ Link
In this example, you have an AI Engine application, a simple finite impulse response (FIR) filter, with one input programmable logic input/output (PLIO) and one output PLIO. Connect these two PLIOs to the custom register transfer level (RTL) AXI4-Stream interfaces using the V++ linker in the Vitis acceleration flow. 

In the AI Engine application, you declare the two PLIOs interfaces as follows:
```
sig_i =  input_plio::create("PLIO_i_0",plio_64_bits, "data/sig_i.txt" );
sig_o = output_plio::create("PLIO_o_0",plio_64_bits, "data/sig_o.txt" );
```
You can see that the input and output PLIOs are called PLIO_i_0 and PLIO_o_0, respectively.


In the binary container settings, you can define the connectivity between the PLIOs and the RTL AXI4-Stream interfaces (referred by their unique sptag):
```
[connectivity]
sc=master_axi_1:ai_engine_0.PLIO_i_0
sc=ai_engine_0.PLIO_o_0:slave_axi_1
```

The Vitis project can be generated using the following make command:
```
make vitis_project
```

If you open the generated Vivado project, which is located under Vitis/workspace_1/system_project/build/hw_emu/hw_link/binary_container_1/binary_container_1/vivado/vpl/prj/, you can see that the RTL AXI4-Stream interfaces have been connected to the AI Engine.

![VPP Link Result](./images/261_vpp_link_phase_1.jpg)

While you can run this application on Hardware, there is no way to verify the good behaviour of the design. In the following part 3, the platform is modified to add ILAs to the custom RTL AXI4-Stream interfaces so you are able to monitor the activity on the streams in Hardware.

### Hardware Emulation

Another way to verify the good behaviour of the design is to run through Hardware emulation and looking at the waveforms.
To run the Hardware Emulation and visualize the waveforms of the AXI4-Stream interfaces, go through the following steps:
1. Open the workspace_1 in Vitis IDE.
```
vitis -w Vitis/workspace_1/
```

2. In the flow navigator, make sure the component system project is selected and click on *Start Emulator* under *Harware Emulation*.

![Start Emulator](./images/start_emu.jpg)

3. In the pop-up window, enable *Show Waveform* and click *Start*. This starts the emulator and launch the Vivado tool to visualize the simulation waveforms.

![Enable waveforms](./images/start_emu_2.jpg)

4. In the Vivado window, add the M00_AXIS interface from the AXI4S_Counter_0 IP and the S00_AXIS from the dummy_sink_0 IP to the waveform window

![Waveforms](./images/26.1_hw_emu.jpg)

5. Run the simulation for at least 30us.

![Waveforms](./images/26.1_hw_emu_2.jpg)

6. In the Vitis IDE, click Run in the flow navigator to start the AI Engine

![Waveforms](./images/26.1_hw_emu_3.jpg)

5. After ~15us of simulation time, you see transactions on the AXI4-Stream interfaces from and to the AI Engine demonstrating the good behaviour of the design.

![Waveforms](./images/26.1_hw_emu_4.jpg)

---
## Part 2 - Connecting RTL AXI4-Stream interfaces (NOT included in Block Design) to the AI Engine
In this part, the design from **Part 1** is replicated but the AXI4S_Counter and the dummy Sink used in the previous part are now instantiated in an RTL top-level, which also instantiates the Block Design that contains the AI Engine.
As the V++ linker is only able to work inside a BD, add an IP block inside it to which the linker can connect.

### Hardware Platform
In this example, the Vivado design consists of a top-level RTL file, which instantiates the AXI4S_Counter and the dummy Sink IPs used in the previous part as well as a BD. The BD was generated from the CED and contains all the required elements for the Vitis acceleration flow.

In the BD, two instances of the AXI4-Stream Register slice IP are added. They are configured in such way that they only act as a pass-through thus can be considered as wires. But they are required to have AXI4-Stream interfaces to which the V++ linker can connect the AI Engine. One side of these IP instances (M00_AXIS for one and S00_AXIS for the other) is connected to the external ports of the BD (which are then connected to the AXI4S_Counter and the dummy Sink IPs inside the top level). The other interfaces are left uncorrected and added to the plaform, similar to what was done in **Part 1**.

```
# Add AXI4-Stream interfaces to Platform
set_property PFM.AXIS_PORT {M_AXIS {type "M_AXIS" sptag "master_axi_1" is_range "false"}} [get_bd_cells /axis_register_slice_0]
set_property PFM.AXIS_PORT {S_AXIS {type "S_AXIS" sptag "slave_axi_1" is_range "false"}} [get_bd_cells /axis_register_slice_1]
```

![Block Design](./images/261_BD_RTL_out.jpg)

To build the HW design, run the following command:
```
make vivado_platform RTL_OUT_BD=1
```
The Vivado project is generated under Vivado/build/custom_pfm_strmIn_strmOut_RTL_out

### Vitis V++ Link
As the sptag for the master and slave interfaces are the same to what was used in **Part 1**, there is no change to the Vitis project. It can be built using the following command:
```
make vitis_project RTL_OUT_BD=1
```

If you open the generated Vivado project, which is located under `Vitis/workspace_2/system_project/build/hw_emu/hw_link/binary_container_1/binary_container_1/vivado/vpl/prj/`,  you can see that the RTL AXI4-Stream interfaces have been connected to the AI Engine.

![VPP Link Result](./images/261_BD_linked_design_RTL_out.jpg)

### Hardware Emulation

You can verify the good behaviour of the design running through Hardware emulation and looking at the waveforms, using the same steps as in **Part 1**.
To run the Hardware Emulation and visualize the waveforms of the AXI4-Stream interfaces, go through the following steps:

1. Open the workspace_2 in Vitis IDE.
```
vitis -w Vitis/workspace_2/
```

2. In the flow navigator, make sure the component system project is selected and click on **Start Emulator** under **Harware Emulation**.

![Start Emulator](./images/start_emu.jpg)

3. In the pop-up window, enable **Show Waveform** and click **Start**. This starts the emulator and launch the Vivado tool to visualize the simulation waveforms.

![Enable waveforms](./images/start_emu_2.jpg)

4. Add the AXIS interface signals (tready, tvalid, tdata) from the AXI4S_Counter_0 IP and the S00_AXIS from the dummy_sink_0 IP to the waveform window and run the simulation for 30us.

5. In the Vitis IDE, click Run in the flow navigator to start the AI Engine

![Waveforms](./images/26.1_hw_emu_3.jpg)

6. After ~10us of simulation time, you see transactions on the AXI4-Stream interfaces from and to the AI Engine demonstrating the good behaviour of the design.

---
## Part 3 - Connecting Monitored RTL Interfaces to AI Engine

### Creating the design
In some cases, it can be useful to add debug capabilities to the interfaces to monitor the data that is going in and out of the array. One way of adding the debug capabilities is to add the ILAs directly into the Vivado design. This is what is done in this part using the design from **Part 1**:

![Custom IPs with ILAs](./images/261_custom_IPs_ILAs.jpg)

While the Vivado contains a slight change, the Vitis project can be similar to the one from **Part 1**.

To build the full project, including the Vivado HW design and the Vitis project, run the following command:
```
make all INCLUDE_ILAS=1
```

### Running the Design in Hardware

To run the design and observe the traces from the ILAs, open Vivado.

1. Prepare the VCK190 by connecting the UART/JTAG USB cable to your compute and set the boot mode to JTAG.

2. Power on the VCK190.

3. Open Vivado and open the Vivado HW manager.

4. Connect to the target and program the device using the generated `BOOT.BIN` (`Vitis/workspace_3/system_project/build/hw/package/package/BOOT.BIN`) and use the `.ltx` file from the V++ linker generated Vivado project (`Vitis/workspace_3/system_project/build/hw/hw_link/binary_container_1/binary_container_1/vivado/vpl/prj/prj.runs/impl_1/custom_pfm_strmIn_strmOut_bd_wrapper.ltx`).

![Program device](./images/261_program_device.jpg)

5. Open the ILA view and configure the ILA to trigger when M00_AXIS tready and tvalid are high. Change the number of captured window to two and set the trigger position in window setting to 10 and run the trigger for the ILA.

![ILA configuration](./images/261_configure_ILAs.jpg)

6. The ILA should trigger and you should see an activity on the AXI4-Streams.

![ILA Output](./images/261_ILA_output.jpg)

---
## Part 4 - Monitoring the AI Engine interfaces using V++ Link

### Creating the design
Another way to add ILA to monitor the activity to/from the AI Engine is to add ILA from V++ link. In this part, we will use the same vivado design from **Part 1** and only add debug options to v++ to add the ILAs to the AI Engine interfaces.

To build the full project, including the Vivado HW design and the Vitis project, run the following command:
```
make all ILA_VPP=1
```

The only difference with the project from **Part 1** are the following line to the config file for the v++ link stage
```
[debug]
aie.chipscope=PLIO_i_0
aie.chipscope=PLIO_o_0
```
![CFG file](./images/252_cfg_vpp_ila.jpg)

If we open the Vivado design generated from v++ (Vitis/workspace_4/system_project/build/hw/hw_link/binary_container_1/binary_container_1/vivado/vpl/prj/prj.xpr) we can see one ILA added in the Vitis region monitoring the input and output PLIOs of the AI Engine:

![CFG file](./images/261_design_vpp_ila.jpg)

### Running the Design in Hardware

To run the design and observe the traces from the ILAs, open Vivado.

1. Prepare the VCK190 by connecting the UART/JTAG USB cable to your compute and set the boot mode to JTAG.

2. Power on the VCK190.

3. Open Vivado and open the Vivado HW manager.

4. Connect to the target and program the device using the generated `BOOT.BIN` (`Vitis/workspace_4/system_project/build/hw/package/package/BOOT.BIN`) and use the `.ltx` file from the V++ linker generated Vivado project (`Vitis/workspace_4/system_project/build/hw/hw_link/binary_container_1/binary_container_1/vivado/vpl/prj/prj.runs/impl_1/debug_nets.ltx`).

5. Open the ILA view and run auto trigger. You should see activity going in and out of the AI Engine

---
## Part 5 - Broadcasting Data to the AI Engine and the Programmable 
In some cases, the same data might need to be processed by both the AI Engine and the PL so you might need a way to broadcast the data to both the domains.
On important thing to keep in mind with working with compliant AXI4-Stream interfaces (as the AI Engine expects an AXI4-Stream compliant interface) is that the master and slave interfaces need to handle the handshaking protocol (with tvalid and tready). Thus, when connecting multiple slaves to a single master is not as simple as connecting the two slaves to the same master interface (as there would be two tready signals). The master would need to handle the logic between the two tready from the slaves.
Another option is to use an IP that would handle this for you. In the Vivado catalog, the AXI4-Stream Broadcaster IP is intended for this type of use case.

### Creating the design
In this example, the same design as **Part 1** with the AXI4S_Counter and dummy Sink custom RTL IPs is used. However, in this case, the source, the AXI4S_Counter, is connected to an AXI4-Stream broacaster IP to stream the data to both the PL and the AI Engine domains. A second dummy sink instance is added to simulate the connectivity with the PL.
The connectivity between one of the AXI4-Stream interfaces of the AXI4-Stream Broadcaster IP and the second dummy sink is added directly to the Vivado design as shown below.

![Vivado Design](./images/261_hw_design_AXIS_broadcast.jpg)

Similar to **Part 1**, the HW design as 2 AXI4-Stream interfaces left unconnected, one master and one slave interface. The only difference is that the unconnected master interface is from the AXI4-Stream Broadcaster IP.
However, same SP Tags are given to the two interfaces as in **Part 1**. Thus, you can use the exact same Vitis project.
```
set_property PFM.AXIS_PORT {M00_AXIS {type "M_AXIS" sptag "master_axi_1" is_range "false"}} [get_bd_cells /axis_broadcaster_0]
set_property PFM.AXIS_PORT {S00_AXIS {type "S_AXIS" sptag "slave_axi_1" is_range "false"}} [get_bd_cells /dummy_sink_0]
```

To build the full project including the Vivado HW design and the Vitis project, run the following command:
```
make all AXI4S_BROADCAST=1
```

### Hardware Emulation

You can verify the good behaviour of the design running through Hardware emulation and looking at the waveforms, using the same steps as in **Part 1**.
To run the Hardware Emulation and visualize the waveforms of the AXI4-Stream interfaces, go through the following steps:

1. Open the workspace_5 in Vitis IDE.
```
vitis -w Vitis/workspace_5/
```

2. In the flow navigator, make sure the component system project is selected and click on **Start Emulator** under **Hardware Emulation**.

3. In the pop-up window, enable **Show Waveform** and click **Start**. This starts the emulator and launches the Vivado tool to visualize the simulation waveforms.

4. Add the AXIS interface signals (tready, tvalid, tdata) from the AXI4S_Counter_0 IP and the S00_AXIS from the dummy_sink_0 IP to the waveform window and run the simulation for 30us.

5. In the Vitis IDE, click Run in the flow navigator to start the AI Engine

6. You see transactions on the AXI4-Stream interfaces from the AXI4-Stream broadcaster to the AI Engine, the AXI4-Stream broadcaster to the PL and from the AI Engine to the PL demonstrating the good behaviour of the design.

![HW emulation waveforms](./images/261_hw_emu_AXIS_broadcast.jpg)

<hr class="sphinxhide"></hr>

<p class="sphinxhide" align="center"><sub>Copyright © 2024–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/legal/copyright.html">Terms and Conditions</a></sup></p>
