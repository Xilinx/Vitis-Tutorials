<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ Getting Started Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# Vitis HLS Analysis and Optimization

***Version: Vitis 2025.2***

## Introduction

Vitis High-Level Synthesis (HLS) enables you to compile C/C++ code into RTL code for implementation in the programmable logic (PL) region of AMD devices. It is the tool that performs high-level synthesis of the C/C++ code and exports it as either a Vivado IP (`.zip`) for use in the Vivado Design Suite and the Embedded Software development flow, or as a Vitis kernel (`.xo`) for use in the Vitis acceleration flow and heterogeneous system designs. Although the Vitis kernel and Vivado IP flows are similar, there are some fundamental differences as explained in [Introduction to Vitis HLS Components](https://docs.amd.com/r/en-US/ug1399-vitis-hls/Introduction-to-Vitis-HLS-Components). This tutorial demonstrates the Vitis unified IDE flow for the bottom-up development of HLS designs.

### Before You Begin

The labs in this tutorial use:

* BASH Linux shell commands.
* Vitis core development kit release.

>**IMPORTANT:**  
>
> * Before running any of the examples, make sure you have installed the Vitis Software Platform as described in Vitis release notes [Installing the Vitis](https://docs.amd.com/r/en-US/ug1742-vitis-release-notes/Vitis-Software-Platform-Installation).

### Setup the environment to run Vitis

To configure the environment to run Vitis, run the following scripts which set up the environment to run in a specific command shell.

```bash
source <Vitis_install_path>/settings64.sh
source /opt/xilinx/xrt/setup.sh
```

> **Note**: `.csh` scripts are also provided, but this tutorial assumes a bash shell is used.

To specify the location of any platforms you have installed, set the following environment variable:

```bash
export PLATFORM_REPO_PATHS=<path to platforms>
```

### Accessing the Tutorial Reference Files

1. To access the reference files, type the following into a terminal: `git clone https://github.com/Xilinx/Vitis-Tutorials`.
2. Navigate to the `Getting_Started/Vitis_HLS` directory, and then access the `reference-files` directory.
3. You may open the HLS project by the following command if you've already sourced the settings script to setup Vitis (Click **Update** if a **Update Workspace** window pops up after executing the command). But we strongly recommend that you read each step in [Next Steps](#next-steps) carefully to clearly understand how to create a project from scratch and perform simulation, synthesis and optimization.

   ```code
   vitis -w workspace
   ```

4. The [Makefile](./reference-files/Makefile) is provided as a reference if you'd like to explore the CLI flow.

## Next Steps

This tutorial describes how to create an HLS component using the Vitis Unified IDE:

1. [Creating the Vitis HLS Project](./unified_ide_project.md) - Create the project to deine the HLS component.
2. [Running High-Level Synthesis and Analyzing Results](./unified-synth_and_analysis.md) - Simulate and synthesize the design, and analyze the results.
3. [Using Optimization Techniques](./unified-optimization_techniques.md) - Try different optimization techniques to achieve the initiation interval (II)=1.
4. [Reviewing the Dataflow Optimization](./unified-dataflow_design.md) - Add the Dataflow optimization to achieve even better results.

<hr class="sphinxhide"></hr>

<p class="sphinxhide" align="center">
<a class="sphinxhide" href="./README.md">Return to Main Page</a> — <a class="sphinxhide" href="./../README.md">Return to Getting Started Pathway</a>
</p>

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
