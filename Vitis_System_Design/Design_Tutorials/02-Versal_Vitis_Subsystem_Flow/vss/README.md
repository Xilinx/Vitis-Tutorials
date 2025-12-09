<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ System Design Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
    </td>
  </tr>
</table>


# Preparing Vitis Subsystem components and building a VSS deliverable

This part of the example will demonstrate adding various AI Engine and PL components to a Vitis Subsystem.
To test and verify the AI Engine and HLS components individually and in a small DSP system, the new Vitis Functional Simulation is used demonstratng how to run in native Matlab or Python.
Both Matlab and Python offers rich libraries and toolboxes for creating test vectors, reference modelling and visualizing the results.

## VSS design description

The design reuses blocks from previous tutorials to make it easier to compare the mechanics for adding and integrating them to a small system.
Components used for the VSS region:

| Component                                         | Type | Description                                                                                                            |
|---------------------------------------------------|------|------------------------------------------------------------------------------------------------------------------------|
| counter                                           | RTL  | Free-running counter to generate input stimuli for datamovers.                                                         |
| [axis1to2](./ip/axis1to2/README.md)                | HLS  | Duplicates one AXI stream into two ports to copy inputs to each AIE FIR filter.                                        |
| [vadd_s](./ip/vadd_s/README.md)                    | HLS  | Vector adder taking complex int16 stream inputs. Used to demonstrate Python VFS.                                       |
| [aie](./ip/aie/README.md)                          | AIE  | Top-level graph consisting of a subgraph with data mover kernels and a subgraph with two FIR filter kernels.           |

The AIE datamover subgraph is connected to the counter and leaves placeholder interfaces so the subtractor from Vitis domain can be connected later.
The AIE filter subgraph have two instances of a 16 tap FIR filter kernel. By feeding same data to both FIR filters and sum the outputs using vadd_s,
we can play around with the filter coefficients to test the combined response.

Conceptual Design used in the lab

![](../documentation/readme_files/design_description.svg)

***Note*** the double dash dotted box surrounding the "vss_top" marking the boundary for the VSS component

Steps in this part:
 - 1. Develop a Vitis Subsystem component
    - 1.1 Inspect the HLS component
      - [vadd_s component](./ip/vadd_s/README.md)
      - [axis1to2 component](./ip/axis1to2/README.md)
    - [1.2 Verify HLS vadd_s with Python VFS](./python/README.md)
    - [1.3 Inspect the AIE graph](./ip/aie/README.md)
      - Adds AIE FIR filter and datamover subgraphs to the top aie graph.
      - A separate testbench for the AIE FIR filter graph is provided to verify the FIR filters.
    - [1.4 Use VFS with Matlab or Python verify AIE Subsystem](./matlab/README.md)
      - Use VFS Matlab for functional simulation of DSP (AIE) components to the design
      - Use VFS Matlab to simulate subsystem setup by passing data between components
    - [1.5 Integrate the subsystem and generate a VSS archive](#Integrating-the-subsystem-and-generate-VSS-archive)
    - [1.6 Verify the VSS using cosimulation in Xsim](./cosim/README.md)

## Integrating the subsystem and generate VSS archive

To integrate the various PL and AIE components, a configuration file is used to declare what components to use and which interfaces to connect.
The syntax is similar to how Vitis connects components to an extensible platform, except here we declare what to be added to a vss archive.
Specifying the VSS component archive require this syntax:

```
vss=amd.com:<vss_library_name>:<vss_component_name>:<version_number>:<list_of_instances>
```

Description of the syntax

| Item                 | Example                       | Description                                                                                                                                                                                               |
|----------------------|--------------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `vss_library_name`   | `my_vss_lib`                   | Identifier name to associate the VSS with a library.                                                                                                                                                      |
| `vss_component_name` | `vss_top`                      | Used as the VSS archive name and concatenated with instance names to provide a unique name when integrating the VSS component into an extensible platform.                                                |
| `version_number`     | `1.0`                          | Version number to keep track of the library/component version.                                                                                                                                            |
| `list_of_instance`   | `counter_0`, `ai_engine_0`     | Comma-separated list of instance names. The names must match kernel names declared with `nk` in the config file. **Note:** The `ai_engine_0` instance name must match the default instance name in the Vivado HW platform. |

[Inspect the config file for the example run](./src/vss_conn.cfg)

The VSS archive is generated with v++ using:

```
v++ --link --mode vss --save-temps --part <part_name> --config ./src/vss_conn.cfg <list_of_xo> <libadf.a> --out_dir <build_folder>
```

Makefiles are provided to build everything from the lab top folder. It will automatically compile all required RTL, AIE and HLS components as required by VSS during linking.

From top folder, run:

```
make vss
```

With the VSS archive, we can verify the subsystem using the cosimulation feature to run in a RTL style testbench under Xsim.

***Optional:*** Follow [1.6 Verify the VSS using cosimulation in Xsim](./cosim/README.md)


Once confident with the subsystem design, the next step is to create a vivado platform to which we can integrate the VSS.

## Navigation helper

- [Next step - Develop Custom Vivado Extensible Platform](../vivado/Vivado.md)
- [Creating AIE graphs and kernels](../ip/aie/README.md)
- [Creating RTL counter](../ip/counter/README.md)
- [Creating HLS AXI Stream splitter](../ip/axis1to2/README.md)
- [Creating HLS vector adder](../ip/vadd_s/README.md)
- [Simulating the VSS Component](../cosim/README.md)
- [Return to top](../../../README.md)


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2025 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
