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

# Tiling Parameter Programming

***Version: Vitis 2025.1***

## Introduction

>**IMPORTANT**: Before beginning the tutorial make sure you have installed the AMD Vitis™ 2025.1 software. The Vitis release includes all the embedded base platforms including the VEK280 base platform that is used in this tutorial. In addition, ensure you have downloaded the Common Images for Embedded Vitis Platforms from [this link](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html).

The ‘common image’ package contains a prebuilt Linux kernel and root file system that can be used with the AMD Versal™ board for embedded design development using the Vitis software platform.

Before starting this tutorial, run the following steps:

1. Go to the directory where you have unzipped the Versal Common Image package.
2. In a Bash shell, run the ``/Common Images Dir/xilinx-versal-common-v2025.1/environment-setup-cortexa72-cortexa53-amd-linux`` script. This script sets up the SDKTARGETSYSROOT and CXX variables. If the script is not present, you must run the ``/Common Images Dir/xilinx-versal-common-v2025.1/sdk.sh``.
3. Set up your ROOTFS and IMAGE to point to the ``rootfs.ext4`` and Image files located in the ``/Common Images Dir/xilinx-versal-common-v2025.1`` directory.
4. Set up your PLATFORM_REPO_PATHS environment variable to ``$XILINX_VITIS/base_platforms``.

This tutorial targets VEK280 board for 2025.1 version.

Data generation for this tutorial requires [Python 3](https://www.python.org/downloads/). The following packages are required:

- os
- sys
- numpy
- For graphics generation
  - matplotlib
  - PIL
- For Buffer Descriptor index extraction:
  - jmespath
  - json



## Objectives

After completing this tutorial, you will be able to:

- Program buffer descriptors using tiling parameters.
- Understand what are the limits of this BD parametrization.
- Use tensor buffer stream concept to access input and output buffers from a kernel.

This tutorial is based on a simple passthrough that allows simple data visualization at the output.

## Prerequisite knowledge

To follow this tutorial you need to understand the architecture of the _AI Engine-ML_ as well as the basics of buffer descriptor programming:

- **AI Engine ML Architecture:**: [am020](https://docs.xilinx.com/r/en-US/am020-versal-aie-ml)
- **Programming Buffer Descriptors with Tiling parameters:** [UG1603](https://docs.xilinx.com/r/en-US/ug1603-ai-engine-ml-kernel-coding)


## Tutorial Overview

This tutorial is divided in multiple sections:

- [Documentation Examples](DocumentationExamples.md) In this sections all examples exposed in UG1603 are shown in action.
- [Memory Tiles](MemoryTiles.md) In this section some more examples are shown, with the limitations and how to override them.
- [External Memory](ExternalMemory.md) In this section we will show how to program the external memory using the _external_buffer_ API.
- [Memory Modules](MemoryModules.md) In this section we will show how to program the DMAs of the memory modules as well as the tensor buffer stream concept.

The directories for the sources and the generated files are different for all sections of the tutorial:

| | Documentation Examples | Memory Tiles | External Memory | Memory Modules |
| :---: | :---: | :---: | :---: | :---: |
|Source Code Directory | *src_doc* | *src_memtile* | *src_extbuf* | *src_memmodule* |
| Aie Compilation Work Directory | *doc_WorkAie* | *memtile_WorkAie* | *extbuf_WorkAie* | *memmodule_WorkAie |
| X86 Compilation Work Directory | *doc_WorkX86* | *memtile_WorkX86* | *extbuf_WorkX86* | *memmodule_WorkX86 |
| Aie Compilation Work Directory | *doc_aiesimulator_output* | *memtile_aiesimulator_output* | *extbuf_aiesimulator_output* | *memmodule_aiesimulator_output |
| X86 Compilation Work Directory | *doc_x86simulator_output* | *memtile_x86simulator_output* | *extbuf_x86simulator_output* | *memmodule_WorkX86 |

The *make* utilities is used to compile and simulate the various sections:

Data generation for all sections:
```shell
make clean data
```

AIE compilation and simulation for some testcases in a specific section:

```shell
make SECTION=[doc | memtile | extbuf | memmodule] [T1] [T2] [T3] [T4] aie aiesim
```

For __doc__ section, all testcases are compiled. For others _Tn_ will compile _n-th_.testcase.

This is the same behaviour for x86 compilation and simulation:

```shell
make SECTION=[doc | memtile | extbuf | memmodule] [T1] [T2] [T3] [T4] x86 x86sim
```



## Basics of Tiling Parameter Programming

### Introduction

There are multiple levels of memory in the AI Engine-ML architecture:

- Memory Module attached to the AI Engine-ML (64kB). Access from the processor or using internal DMA with dedicated address generators (2xMM2S, 2xS2MM).
- memory tile (512 kB): 1 or 2 per column. Access with internal DMAs with dedicated address generator (6xS2MM, 6xMM2S).
- External memory: DDR or HBM which is also addressed by DMAs located in the PL-AIE array interface.

Data in these memories can be seen as a single dimension set of data, but depending on the memory the maximum dimensionality can be 3 (AIE Tile DMA, Interface Tile DMA) or 4 (Memory Tile DMA). All these DMA BDs of the can be programmed from within the graph. Memory Modules buffer access can also be accessed using complex schemes using tensor buffer stream concept.

This programming process is done through what we call *Tiling Parameters*. These parameters describe how the data should be accessed in the memory on a tile basis. Dimension 0 is the dimesion where the data are contiguous in memory. The overall dimension of the buffer is specified and then the tile dimension and the number of tiles to read in each dimension.


### Tiling parameter structure

The `tiling_parameters`structure is defined as follows:

- `buffer_dimension`: Buffer dimensions in buffer element type. buffer_dimension[0] is the fast-moving dimension and is contiguous in memory. When this variable is left unspecified, the dimensions of the associated buffer object will be used. On dimension 0, it should be a multiple of 32 bits.
- `tiling_dimension`: dimensions of the basic Tile in buffer element type. On dimension 0, it should be a multiple of 32 bits.
- `offset`: Multi-dimensional offset w.r.t. buffer starting element. Assuming buffer dimension is specified. Offset can be negative in all but the last dimension.
- `tile_traversal`: Vector of traversing_parameters. tile_traversal[i] represents i-th loop of inter-tile traversal, where i=0 represents most inner loop and i=N-1 represents most outer loop.
- `packet_port_id`: Output port id of the connected pktsplit or the input port id of the connected pktmerge, useful for packet switching.
- `boundary_dimension`: When the data read access is outside the boundary dimension, the read value is replaced by zero (0).


One can see in this parameters all the global parameters of the transfer, but the order of the memory access is specified by the `traversing_parameters`:


- `dimension`: The dimension of the inter-tile traversing loop.
- `stride`: stride represents the distance in terms of buffer element type between consecutive inter-tile traversal.
- `wrap`: wrap represents the number of steps before wrapping the current traversing loop and incrementing to the next traversing loop. The traversal stops when the last wrap completes.

Actually a vector of `traversing_parameters` is describing the access order. This allows the user to specify any dimension-wise order, starting with dimension 3 if the data must be ordered that way.

Some more details on tiling parameters can be found here: [Tiling Parameters Programming](TilingParametersProgramming.md).

### Buffer Descriptors

The underlying DMA control structure are actually Buffer Descriptors (BD). This tutorial is not intended to give you all tips and tricks about buffer descriptors, but a minimum knowledge is necessary to understand some limitations in tiling parameters programming.

Some details on buffer descriptors can be found here: [BufferDescriptors](DeepDiveBufferDescriptors.md)

## Utilities

In order to be able to view the Tiling Parameter Programming effects on data, there are 2 Python utilities that are used to generate data and to visualize tiles (up to 3D).
 
### CreateNDData.py

 This script is used to generate data so that it is easy to see the dimensions and coordinates of each data. For example 2D data where all dimension length are less than 10 will be:
 ```
 0  1  2  3  4  5  6  7
10 11 12 13 14 15 16 17
20 21 22 23 24 25 26 27
30 31 32 33 34 35 36 37
40 41 42 43 44 45 46 47
50 51 52 53 54 55 56 57
60 61 62 63 64 65 66 67
70 71 72 73 74 75 76 77
```
As you can see, the unit digit is the column number and the 10s digit is the row number.

Now If there are 3 dimensions, the 100s digit will be the layer number and so on. If one dimension length is between 10 and 99, two digits will be used in each dimension to represent the index. Here are the generated data for dimensions: 16 8 and 2:

```
Layer = 0
    0     1     2     3     4     5     6     7     8     9    10    11    12    13    14    15
  100   101   102   103   104   105   106   107   108   109   110   111   112   113   114   115
  200   201   202   203   204   205   206   207   208   209   210   211   212   213   214   215
  300   301   302   303   304   305   306   307   308   309   310   311   312   313   314   315
  400   401   402   403   404   405   406   407   408   409   410   411   412   413   414   415
  500   501   502   503   504   505   506   507   508   509   510   511   512   513   514   515
  600   601   602   603   604   605   606   607   608   609   610   611   612   613   614   615
  700   701   702   703   704   705   706   707   708   709   710   711   712   713   714   715

        Layer = 1
        10000 10001 10002 10003 10004 10005 10006 10007 10008 10009 10010 10011 10012 10013 10014 10015
        10100 10101 10102 10103 10104 10105 10106 10107 10108 10109 10110 10111 10112 10113 10114 10115
        10200 10201 10202 10203 10204 10205 10206 10207 10208 10209 10210 10211 10212 10213 10214 10215
        10300 10301 10302 10303 10304 10305 10306 10307 10308 10309 10310 10311 10312 10313 10314 10315
        10400 10401 10402 10403 10404 10405 10406 10407 10408 10409 10410 10411 10412 10413 10414 10415
        10500 10501 10502 10503 10504 10505 10506 10507 10508 10509 10510 10511 10512 10513 10514 10515
        10600 10601 10602 10603 10604 10605 10606 10607 10608 10609 10610 10611 10612 10613 10614 10615
        10700 10701 10702 10703 10704 10705 10706 10707 10708 10709 10710 10711 10712 10713 10714 10715
```

In these case I specified a base of 10 and 100. A base of 16 or 256 could have been chosen if the display was in hexadecimal.

Here is the usage of this script:
```SHELL
Usage: CreateNDData NbDataPerLine Base Dimensions LengthDim0 [LengthDim1 ...]
        NbDataPerLine: Number of data printed per line.
                       This is important to have files compliant with the bitwidth of your PLIOs
        Base:          Numerical Base with which is compute the increment from row to row (10, 100, 16, ...).
        Dimensions:    Number of dimensions of the data.
        LengthDim0:    Length on Dimension 0.
        LengthDim...:  Length on Dimension 1, 2, ....
```

Output must be redirected to a file to be stored.

### GetTiles.py

`GetTiles.py` is used to visualize the tiles of the data. It is a simple script that reads the data from a file and displays the tiles. Here is the usage of this script:
```SHELL
Usage: GetTiles filename TileDim NCols [ NRows [ NLayers [ NImages ]]] [ Tile# | Tile_start Tile_end]
       filename: input file containing the matrix (without timestamps)
       TileDim: 1D 2D 3D 4D or 1Do 2Do 3Do or 1DO 2DO 3DO for optimized display 
                1Do 2Do 3Do: start ellipsis notation at 50 columns
                1DO 2DO 3DO: start ellipsis notation at 30 columns
       NCols: number of columns of each Tile in the matrix
       NRows: number of rows of each Tile in the matrix
       NLayers: number of layers of each Tile in the matrix
       NImages: number of images of each Tile in the matrix
       Tile#:  display the Tile with the given index  ('*' for all tiles)
       Tile_start Tile_end: display the Tiles in the given range
```

The original file should not have timestamps. For X86 simulation you can use the raw output of the simulator, for AIE simulations, you should use the files which are pre-pended with `NoStamps_'.

For 4D tiles, use 3D and interpret the sequential tiles as the 4th dimension.

Display optimization is just a question of adding ellipsis for large number of columns instead of printing all columns. Standard display of a 256 element vector:

```Shell
  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36  37  38  39  40  41  42  43  44  45  46  47  48  49  50  51  52  53  54  55  56  57  58  59  60  61  62  63  64  65  66  67  68  69  70  71  72  73  74  75  76  77  78  79  80  81  82  83  84  85  86  87  88  89  90  91  92  93  94  95  96  97  98  99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255
```
 Optimized display of the same 256 element vector:
 ```SHELL
   0   1   2   3   4   5   6   7   8   9    ...    246 247 248 249 250 251 252 253 254 255
```

### CompilerReport.py

After AI Engine compilation with hardware target (not x86 simulation), all tiling parameters are converted into Buffer Descriptor initialization. In order to know which BD are initialized the python script `CompilerReport.py`can be used giving the AIE Work directory as parameter. 

This can be used to extract various information about the implementation solution of the AIE compiler, mapper, router. Script can be used to extract DMA channels, BD numbers and Tiling parameters:
```shell
Usage: ExtractBDNumbers.py WorkDirectory option1 [option2 [...]]
        -bd_memtile: extract Memory Tile BDs
        -bd_memory: extract Memory Module BDs
        -bd_interface: extract Interface Tile Channels
        -bd_all: extract all BDs
        -tiling: extract tilings and buffer size
```


The Makefile contains a target (*extractBDs*) that will extract all BD numbers from the AI Engine Compiler report.

Fo example you can compile some testcases on the 'memmodule' section and display the buffer desriptors as follows:

```shell
make SECTION=memtile T2 T3 aie extractBDs
```

This will display the following:
```shell
============= Memory Tile Buffer Descriptors =============

Array Column 11    Memory Tile Row 0
        Input  Channel 0:  Buffer Descriptors: [0, 1]
        Output Channel 0:  Buffer Descriptors: [2, 3]

Array Column 12    Memory Tile Row 0
        Input  Channel 0:  Buffer Descriptors: [0, 1]
        Output Channel 0:  Buffer Descriptors: [2, 3]

Array Column 12    Memory Tile Row 1
        Input  Channel 0:  Buffer Descriptors: [0, 1]
        Output Channel 0:  Buffer Descriptors: [2, 3, 4, 5, 6, 7, 8, 9]

============= Memory Module Buffer Descriptors =============

Array Column 11    Memory Tile Row 0
        Output Channel 0:  Buffer Descriptors: [0, 1]
        Input  Channel 0:  Buffer Descriptors: [2, 3]

Array Column 11    Memory Tile Row 1
        Input  Channel 0:  Buffer Descriptors: [0, 1]

Array Column 12    Memory Tile Row 1
        Output Channel 0:  Buffer Descriptors: [0, 1]

============= Interface Tile Buffer Descriptors =============

 No Buffer Descriptors
```

These are all the BD indexes initialized in the Memory Tiles and the Memory Modules.

## Conclusion

In this tutorial you learnt how to:

- Declare and use shared buffers (memory tiles).
- Declare and use external buffers (external memory).
- Program DMA Buffer Descriptors using Tiling Parameters.
- Assign Read and Write access schemes to Memory Tiles, External Memory and Memory Modules.
- Use Tensor Buffer Stream to program complex scheme access to a buffer from within a kernel.


# Support

GitHub issues will be used for tracking requests and bugs. For questions, go to [support.xilinx.com](https://support.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2025 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>


