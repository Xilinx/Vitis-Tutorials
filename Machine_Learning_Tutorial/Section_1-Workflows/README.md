# Vitis and Vitis-AI Workflow

The Vitis unified software platform enables the development of embedded software and accelerated applications on heterogeneous Xilinx platforms including FPGAs, SoCs, and Versal ACAPs. It provides a unified programming model for accelerating Edge, Cloud, and Hybrid computing applications. 

![vitis_stack](images/vitis_stack.png)

Vitis software platform has a layered architecture. It helps engineers to focus on their own target but easy to exchange work results. For example, there are commonly three types of workflows targeting different persona for Vitis and Vitis AI evaluation and development.

| Workflow | Target Persona | Highlights in the Flow |
|-----------|-----------------|------------------------|
| Evaluation Flow | Data Scientists | Use your industry standard tools plus Xilinx adapters to evaluate acceleration performance on Xilinx platforms. Minimal toolset requirement. No need to install Vitis or PetaLinux. |
| Development Flow | Application Developers | Explore the possibilities with Vitis acceleration on Xilinx platforms. No need to install PetaLinux. |
| Customization Flow | Hardware engineers and BSP engineers | Advanced customization for more input and output interfaces, embedded Linux kernel and root file system configurations, etc. |

In big teams, engineers are focused in one area. In small teams or for independent developers, one person may act as multiple personas. This tutorial considered the above two situations. Please refer to each chapter introduction whether you're the target reader of this chapter.