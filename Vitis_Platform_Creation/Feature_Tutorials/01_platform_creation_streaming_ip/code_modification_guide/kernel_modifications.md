<table class="sphinxhide" width="100%">
 <tr>
   <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Platform Creation Tutorials</h1>
   </td>
 </tr>
</table>

This tutorial lists the changes required for the vadd kernel. Note that this is only one example and there are also other ways to implement the same functions.

#### 1. Include necessary file headers.

Add following 'include' commands at the beginning of the file, right before the 'define' command.

***OLD***:
```
#define DATA_SIZE 4096
```

***NEW***:
```
#include "ap_int.h"
#include "ap_axi_sdata.h"

#define DATA_SIZE 4096
```

#### 2. Define the AXI stream data type and add the dds process function
This function simply read the data into the kernel and then sends it out through another port. 

Define a new 'pkt' type before the kernel function definition.

***OLD***
```
extern "C" {
/*
    Vector Addition Kernel
    ...
...
```

***NEW***
```
typedef ap_axis<15, 0, 0, 0> pkt;

static void dss_process( int *wave, hls::stream<pkt> &s_in) {
    for (int i = 0; i < 1024; i++) 
    {
        #pragma HLS PIPELINE II = 1
        pkt value = s_in.read();
        wave[i] = value.data;
    }
}
extern "C" {
/*
    Vector Addition Kernel
    ...
```

#### 3. Add more ports to the kernel functions

Modify the kernel function to include more ports for interacting with the IP inside platform.

***OLD***
```
extern "C" {
/*
    Vector Addition Kernel
    ...
extern "C" {
void krnl_vadd(uint32_t* in1, uint32_t* in2, uint32_t* out, int size) {
```

***NEW***
```
extern "C" {
/*
    Vector Addition Kernel
    ...
void krnl_vadd(uint32_t* in1, uint32_t* in2, uint32_t* out, int size, int *wave_out, hls::stream<pkt> &dds_in) {
```

#### 4. Add the declaration of the two ports

***OLD**
```
#pragma HLS INTERFACE m_axi port = in1 bundle = gmem0
#pragma HLS INTERFACE m_axi port = in2 bundle = gmem1
#pragma HLS INTERFACE m_axi port = out bundle = gmem0
```

***NEW***

```
#pragma HLS INTERFACE m_axi port = in1 bundle = gmem0
#pragma HLS INTERFACE m_axi port = in2 bundle = gmem1
#pragma HLS INTERFACE m_axi port = out bundle = gmem0
#pragma HLS INTERFACE m_axi port = wave_out bundle = gmem2
#pragma HLS INTERFACE axis port = dds_in
```

#### 4. Add a piece of logic to call the dds process function



***OLD***
```
store_result(out, out_stream, size);
}
```

***NEW***
```
store_result(out, out_stream, size);
dss_process(wave_out,dds_in);
}
```

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
