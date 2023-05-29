<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Hardware Acceleration Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Section 2 - Setting up the Environment

> **_In this section:_**  _Setting up the environment for development._

To proceed with this tutorial, set up the environment and the tools. Ensure that you have installed all the tool dependencies.

## Prerequisites

 - [Vitis 2023.1](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vitis.html)
 - [Xilinx RunTime (XRT) 2023.1](https://xilinx.github.io/XRT/)
 - [Alveo U50 packages](https://www.xilinx.com/products/boards-and-kits/alveo/u50.html)

## Setting up the Vitis&trade; Environment

To be able to work with the libraries, you must set up the AMD Vitis™ environment first. For instructions to set up the Vitis environment, refer to the [Vitis online documentation](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Setting-Up-the-Environment-to-Run-the-Vitis-Software-Platform). Following are some example scripts to set up Vitis tools and XRT:

```
  $ source <Vitis Tool Installation Path>/Xilinx/Vitis/2023.1/settings64.sh
  $ source <XRT Installation Path>/xrt/setup.sh
  $ export PLATFORM_REPO_PATHS=<Platform Installation Path>/xilinx/platforms
  $ export DEVICE=xilinx_u50_gen3x16_xdma_5_202210_1
```

## Downloading the Libraries

You must clone the Vitis Libraries from [Vitis_Libraries](https://github.com/Xilinx/Vitis_Libraries) into the local path.

```
  $ git clone https://github.com/Xilinx/Vitis_Libraries.git
  $ cd Vitis_Libraries/
  $ git checkout main 
```

After the download is completed, browse the subfolders to open the Graph library.

The Vitis Graph Library is an open-sourced Vitis library written in C++ for accelerating graph applications in a variety of use cases. It covers three levels of acceleration: the module level (L1), the pre-defined kernel level (L2), and the asynchronous software level (L3).

The folder structure is shown below:

<details>  <summary><b> Vitis Libraries Folder Structure </b></summary>
 <pre><code>  
 Vitis_Libraries/
├── Jenkinsfile
├── LICENSE.txt
├── README.md
├── blas/
├── codec/
├── data_analytics/
├── data_compression/
├── data_mover/
├── database/
├── dsp/
├── graph/
├── hpc/
├── motor_control/
├── quantitative_finance/
├── security/
├── solver/
├── sparse/
├── ultrasound/
├── utils/
└── vision/
 </code></pre>
</details>


<details>  <summary><b> Graph Library Structure </b></summary>
 <pre><code>  
graph/
├── Jenkinsfile
├── L1
│   ├── README.md
│   ├── include
│   ├── tests
├── L2
│   ├── README.md
│   ├── benchmarks
│   ├── include
│   ├── meta
│   ├── tests
├── L3
│   ├── README.md
│   ├── demo
│   ├── include
│   ├── lib
│   ├── meta
│   ├── src
│   ├── tests
├── LICENSE.txt
├── README.md
├── docs
│   ├── Doxyfile_L2
│   ├── Doxyfile_L3
│   ├── Makefile
│   ├── Makefile.sphinx
│   ├── _build
│   ├── _static
│   ├── _templates
│   ├── benchmark.rst
│   ├── conf.py
│   ├── guide_L1
│   ├── guide_L2
│   ├── guide_L3
│   ├── images
│   ├── index.rst
│   ├── overview.rst
│   ├── plugin
│   ├── release.rst
│   ├── tutorial.rst
├── ext
│   ├── make_utility
│   ├── xcl2
├── library.json
├── plugin
    ├── README.md
 </code></pre>
</details>


<details>  <summary><b> Algorithms implemented by the Vitis Graph Library  </b></summary>  
 <br>
 
  - **Similarity analysis**: Cosine Similarity, Jaccard Similarity.
  - **Classification**: k-nearest Neighbor, maximal independent set.
  - **Centrality analysis**: PageRank.
  - **Pathfinding**: Single Source Shortest Path (SSSP), Multi-Sources Shortest Path (MSSP), Minimum Spanning Tree, and Estimated Diameter.
  - **Connectivity analysis**: Weakly Connected Components and Strongly Connected Components.
  - **Community detection**: Louvain Modularity (From 22.1, Louvain API can support large-scale graphs), Label Propagation, and Triangle Count.
  - **Search**: Breadth-First Search and 2-Hop Search.
  - **Graph Format**: Renumber, Calculate Degree, and Format Conversion between CSR and CSC.
 
</details>

## Setting up the Vitis Libraries

Run the following command to set the Vitis Library path for compiling with graph:

```
  $ export VITIS_LIBS_PATHS=<Vitis Libraries local path>/Vitis_Libraries
```

## Setting Options

When compiling the host code on an Ubuntu system, if you encounter an error such as ```cannot find crtn.o: No such file or directory```, set up the environment as shown below to resolve it.

```
  $ export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu
```

When compiling host code, if you encounter an error such as ```terminate called after throwing an instance of 'std::runtime_error' what()..```, set up the environment as shown below to resolve it.

```
  $ export LC_ALL="C"
```

## Next

Proceed to [**Section 3**](../03-Application/README.md).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
