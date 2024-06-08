# Step-by-Step integration to run External Traffic Generators with Emulation Flow 

## Step-1:  ADF Graph Modifications
To use external traffic generators for any kind of simulation, you need to make modifications to the graph code, specifically the graph.h file. This file contains the PLIO constructors, which are used to connect the graph to the programmable logic.

1. Navigate to `aie/src` and open `graph.h`.
2. Notice that the `#ifdef EXTERNAL_IO` is used and the lines of code under it do not have the data file in the PLIO constructors. This is needed for the external traffic generator to work properly, because the data file takes precedence.
3. Take a note of the names (first argument) of the PLIO constructors. These will be used to hook up the external traffic generators and the same name should be used for creating the ports in the external traffic generators. 
4. Close graph.h.

## Step-2: Update the host code to only control the graph
Navigate inside sw/host.cpp and notice that the `#ifndef EXTERNAL_IO` is used and the lines of code under it is used to control the PL data movers alongwith the graph. The code outside it is only used to control the graph as the PL data movers are replaced with external source and sink. 

## Step-3: Linking for Hardware Emulation

In a standard simulation, the various XO files of data movers and PL processing streaming kernel (mm2s, polar_clip, s2mm) would be created and linked to the AI Engine array, leaving no room for the external traffic generators. A configuration file for such a simulation can be seen in system.cfg:

```BASH
[connectivity]
nk=mm2s:1:mm2s
nk=s2mm:1:s2mm
nk=polar_clip:1:polar_clip

sc=mm2s.s:ai_engine_0.in_interpolator
sc=ai_engine_0.clip_in:polar_clip.in_sample
sc=polar_clip.out_sample:ai_engine_0.in_classifier
sc=ai_engine_0.out_classifier:s2mm.s
```

To work with external traffic generators in hardware emulation, introduce hooks in the PL and replace the mm2s/s2mm XOs with the the sim IPC master and slave hooks. For that purpose, AMD provides a complete set of pre built XO files with various bit widths located inside `$XILINX_VITIS/data/emulation/XO/`:

```BASH
sim_ipc_axis_master_NNN.xo with NNN in 8, 16, 32, 64, 128, 256, 512 bits
sim_ipc_axis_slave_NNN.xo with NNN in 8, 16, 32, 64, 128, 256, 512 bits
```
In this tutorial, there are 32-bit interfaces. 

The configuration file is the one in system_etg.cfg. Note that the XO instance names "in_interpolator" for sim_ipc_axis_master_32 and "out_classifier" for sim_ipc_axis_slave_32 should match with that of the external ports in the external traffic generator code that becomes a key which will be used by the external process to refer to a particular stream port. 

```BASH
[connectivity]
nk=sim_ipc_axis_master_32:1:in_interpolator
nk=sim_ipc_axis_slave_32:1:out_classifier
nk=polar_clip:1:polar_clip
sc=in_interpolator.M00_AXIS:ai_engine_0.in_interpolator
sc=ai_engine_0.out_interpolator:polar_clip.in_sample
sc=polar_clip.out_sample:ai_engine_0.in_classifier
sc=ai_engine_0.out_classifier:out_classifier.S00_AXIS
```

## Step-4: Writing the External Traffic Generator 
The overall goal of the external traffic generator is to send or receive data to or from the AI Engine array through a specific port. The sender can generate data on the fly or read it from a file. The receiver can keep the data and save it somewhere, or process it in a function. This example design implements sender and receiver as external sources.

Here, the external traffic generators have taken the place of the PL data movers mm2s and s2mm respectively i.e. interfaced with the input port of the interpolator (in_interpolator) and the output port of the classifier (out_classifier).  You can go inside `TrafficGenerator/` for the script analysis in Python, MATLAB and CPP.  

<details>
  <summary>Python</summary>

### Python

#### 1. Instantiating the XTLM Utilies

You need to create the sender and receiver objects for the AIE input PLIO/output PLIO that will make sure to instantiate the XTLM utilities for IPC based communication while sending or receiving the traffic. Open the script inside `TrafficGenerator/Python/xtg_aie.py` for the script analysis.

```BASH
in_interpolator = aie_input_plio("in_interpolator", 'int16')
out_classifier = aie_output_plio("out_classifier", 'int32')
```
Here the first parameter `in_interpolator` is a string value that should match PLIO names in the graph PLIO constructors. The second parameter is the AIE kernel datatype with which sender/receiver is interfaced. For more details on datatype mapping in Python based external traffic generators, see <UG1393 link> 

#### 2. Transmitting the data using send_data (data_val, tlast) API

You can prepare the list of data values and send them using send_data API call. The API expects data values in the list as the first parameter and TLAST value to be driven as the second.  

```BASH
in_interpolator.send_data(in_interpolator_data, True)
```
Here the first parameter `in_interpolator_data` is the list of cint16 (16-bit) real and imag values expected by the AIE kernel. The second parameter is the TLAST value as `True`

#### 3. Receiving the data using receive_data_with_size API(expected_data_size)

In order to get the received data values from the classifier, use receive_data_with_size(exp_data_size) API call. This API needs expected data size (in bytes) as an argument. The classifier expects total 4096 bytes of data in 4 iterations i.e. 1024 bytes each time receive data is called. 

```BASH 
out_classifier_data = out_classifier.receive_data_with_size(1024)
```

This API is a blocking API and it will wait till expected bytes is received at the output port.

For more details on Python based APIs, refer [Writing Traffic Generators in Python](https://docs.amd.com/r/en-US/ug1393-vitis-application-acceleration/Writing-Python-Traffic-Generators)
</details>

<details>
  <summary>Matlab</summary>

### Matlab

#### 1. Instantiating the XTLM Utilies

You can create the sender and receiver objects for the AIE that will make sure to instantiate the XTLM utilies for IPC based communication while sending or receiving the traffic. You can check lines 7-13. 

```BASH
in_interpolator = aie_input_plio("in_interpolator", 'int16')
out_classifier = aie_output_plio("out_classifier", 'int32')
```
#### 2. Transmitting the data using send_data (data_val, tlast) API

You can prepare the list of data values and send them using send_data API call. See lines <> in the script. The API expects data values list as the first parameter and TLAST value as the second.  

```BASH
in_interpolator.send_data(in_interpolator_data, True)
```
Here the first parameter `in_interpolator_data` is the list of int16 values expected by the AIE kernel. The second parameter is the TLAST value as `True`

#### 3. Receiving the data using receive_data_with_size API(expected_data_size)

In order to get the received data values from the classifier, use receive_data_with_size(exp_data_size) API call. This API needs expected data size (in bytes) as an argument. 

```BASH 
out_classifier_data = out_classifier.receive_data_with_size(1024)
```

This API is a blocking API and it will wait till specified data i.e. total 4096 bytes is received in four iterations at the output port. Once received the specified data size, you can see the data values in the `out_classifier_data` list.

Once the data is received in the list, you can dump it in a file for comparing with the golden output or you can process the data further into some other function based on the application. Here, in this design we are dumping the output of a classifier into a file as it is the final output of the design. 

For more details on MATLAB APIs, refer [Writing Traffic Generators in MATLAB](https://docs.amd.com/r/en-US/ug1393-vitis-application-acceleration/Writing-Traffic-Generators-in-MATLAB)
</details>

<details>
  <summary>CPP</summary>

### CPP

#### 1. Instantiating the XTLM Utilities

You can create the sender and receiver objects for the AIE that will make sure to instantiate the XTLM utilies for IPC based communication while sending or receiving the traffic.

```BASH
    xtlm_ipc::axis_master in_interpolator("in_interpolator");
	xtlm_ipc::axis_slave out_classifier("out_classifier");

```
#### 2. Transmitting the data using send_data (data_val, tlast) API

You can prepare the list of data values and send them using send_data API call. See lines <> in the script. The API expects data values list as the first parameter and TLAST value as the second.  

```BASH
in_interpolator.send_data(interpolator_byte_array, true);
```

Here the first parameter `interpolator_byte_array` is the data values in the form of byte array. The second parameter is the TLAST value as `True`

#### 3. Receiving the data using receive_data_with_size API(expected_data_size)

In order to get the received data values from the classifier, use receive_data_with_size(exp_data_size) API call. This API needs expected data size (in bytes) as an argument. 

```BASH 
out_classifier.receive_data_with_size(data_out_cls, 1024)
```

This API is a blocking API and it will wait till specified data i.e. total 4096 bytes is received in four iterations at the output port. Once received the specified data size, `out_data_cls` byte array. You can convert the byte array into user data type using the conversion APIs. For conversion APIs, refer [Writing Traffic Generators in C++](https://docs.amd.com/r/en-US/ug1393-vitis-application-acceleration/General-Purpose-C-API)

Once the data is received in the list, you can dump it in a file for comparing with the golden output or you can process the data further into some other function based on the application. The output of the classifier is validated against the golden output (classifier_golden.txt). 

</details>

## Step-5: Run the Traffic Generator with Emulation Flow 

You need to trigger the external TG script and the emulation process paralley to establish the connection. You can launch them using `make` command or launch the external process & emulation process on seperate terminals for better analysis of two seperate process. Here are the steps - 

### Launching the external script
Open terminal 1 and use the script inside ``scripts/`` folder to launch the external traffic generator. This script does the required setup to point to the XTLM IPC libary from where all the APIs are imported. You can use following commands based on Pyhton/Matlab/CPP. 

```BASH
./scripts/etg_py.sh --> For Python Based

or 

./scripts/etg_matlab.sh --> For Matlab Based

or

./scripts/etg_cpp.sh --> For CPP Based
```
### Launching the Emulation process 
Once launched the external script in terminal 1, open terminal 2 and simultaneously on you can run the Emulation as below: 

<details>
  <summary>HW Emulation</summary>

#### HW Emulation

```BASH
cd package.hw_emu &
./launch_hw_emu.sh -g --> For launching the waveform view in GUI 

or 

cd package.hw_emu &
./launch_hw_emu.sh
```
##### Viewing HW Emulation Results in the XSIM Waveform Viewer

Once you launch the hw emulation using ``-g`` flag as mentioned above, you can open the waveform as below: 

</details>

<details>
  <summary>SW Emulation</summary>

#### SW Emulation

```BASH
setenv XCL_EMULATION_MODE sw_emu
./host.exe a.xclbin 
```
</details>

#### Make Utitlity to run all the flows 

1. To compile and run sw_emu with external traffic generators --> ``make run TARGET=sw_emu PLATFORM=<vck190> EXTO=true TRAFFIC_GEN=<PYTHON/MATLAB/CPP>``
2. To compile and run sw_emu without external traffic generators --> ``make run TARGET=sw_emu PLATFORM=<vck190> EXTIO=false``
3. To compile and run hw_emu with external traffic generators --> ``make run TARGET=hw_emu PLATFORM=<vck190> EXTO=true TRAFFIC_GEN=<PYTHON/MATLAB/CPP>``
4. To compile and run hw_emu without external traffic generators --> ``make run TARGET=hw_emu PLATFORM=<vck190> EXTO=true TRAFFIC_GEN=<PYTHON/MATLAB/CPP>``

