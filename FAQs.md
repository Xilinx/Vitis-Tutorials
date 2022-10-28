<p align="right"><a>English</a> | <a href="docs-jp/README.md">日本語</a></p>
<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ In-Depth Tutorials</h1>
    </td>
 </tr>
</table>

# Frequently Asked Questions
- [How are the tutorials organized?](#how-are-the-tutorials-organized)
- [What is the release schedule of Vitis-Tutorials?](#what-is-the-release-schedule-of-vitis-tutorials)
- [What is the Git branching strategy?](#what-is-the-git-branching-strategy)
- [How to contribute to Vitis tutorials?](#how-to-contribute-to-vitis-tutorials)


## How are the tutorials organized?
The Vitis tutorials are organized by topics, Each topic contains two sections. Feature_Tutorials and Design_Tutorials. 
* [Getting Started](./Getting_Started) : Overview of Vitis including tutorials showcasing Vitis HLS, Vitis Libraries and platform creation. 
* [AI Engine Development](./AI_Engine_Development) : Learn how to target, develop, and deploy advanced algorithms using a Versal AI Engine array in conjunction with PL IP/kernels and software applications running on the embedded processors.
* [Hardware Acceleration](./Hardware_Acceleration) : Learn how to use the Vitis to build, analyze, and optimize an accelerated algorithm developed in C++, OpenCL, and even low-level hardware description languages (HDLs) like Verilog and VHDL.
* [Vitis Platform Creation](./Vitis_Platform_Creation) : Learn how to develop a Vitis platform for your own board, or customize the platform on Xilinx demo boards.
* [Developer Contributed](./Developer_Contributed) : Developer contributed tutorials from inside and outside Xilinx.

## What is the release schedule of Vitis-Tutorials?
Vitis-Tutorials release follows the release of Vitis tools, the new version of tutorials usually are available in 2 weeks after the Vitis tool release. Some new tutorials will be released asynchronously with the release of the tools.

## What is the Git branching strategy?
The default branch of Vitis-Tutorials always follows the latest released Vitis tool version. We don't keep a master or main branch, if you need to run tutorials on a different Vitis version, please checkout the branch matches the tool version after cloning the repository. 

## How to contribute to Vitis tutorials?
Contributions to Vitis-Tutorials are welcome. Please send your PRs to the branch that matches the version of the tool you created and verified the tutorial. Check <a href="./Developer_Contributed/README.md">here</a> for details. 


<p align="center"><sup>Copyright&copy; 2020–2022 Xilinx</sup></p>
