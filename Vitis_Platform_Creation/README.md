<table class="sphinxhide" width="100%">
 <tr>
   <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AMD Vitis™ Platform Creation Tutorials</h1>
   </td>
 </tr>
</table>


# Vitis Platform Creation Tutorials

## What is a Platform

Before going to platform creation, Let's get an initial idea of platform.

![image](XPFM.PNG)

Briefly speaking, a platform is a package that contains the HPFM（`.xsa`）file and SPFM (software components). When you input the `.xsa` file and software components, the Vitis IDE tool will package them together and generate a platform `.xpfm` file, while the `.xsa` file is designed and exported from the AMD Vivado tool. Software components are prepared by AMD and ready to use for evaluation. Software components customization is also supported with Petalinux, if needed.

### Platform Creation Example Quick Access

- For a first experience of platforms, refer to [Vitis Platform Quick Start](../Getting_Started/Vitis_Platform/)
- For **ZYNQMP** device family, refer to [ZCU104 platform tutorial](./Design_Tutorials/02-Edge-AI-ZCU104/)
- For **Versal** device family, refer to [VCK190 platform tutorial](./Design_Tutorials/03_Edge_VCK190/)
- For **DFX** platform, refer to [DFX platform tutorial](./Design_Tutorials/04_Edge_VCK190_DFX/)
- For **SOM** series, refer to [KV260 platform tutorial](./Design_Tutorials/01-Edge-KV260/)
- For **PetaLinux** customization, refer to [PetaLinux customization](./Feature_Tutorials/02_petalinux_customization/)

## Platform Creation Tutorials

The tutorials under the Vitis Platform Creation category help you learn how to develop an extensible platform for your own board, or customize the Vitis platform on Xilinx demo boards.

- The **[Design Tutorials](./Design_Tutorials/)** showcase end-to-end workflow for creating the Vitis extensible platforms from scratch for different device families and boards.
- The **[Feature Tutorials](./Feature_Tutorials/)** highlight specific features and flows that help develop the platform.


### Design Tutorials

<table style="width:100%">
 <tr>
  <th width="21%" align="center"><b>Tutorial</b></td>
  <th width="8%" align="center"><b>Device Family</b></td>
  <th width="8%" align="center"><b>Board</b></td>
  <th width="6%" align="center"><b>Platform Type</b></td>
  <th width="12%" align="center"><b>IDE Flow</b></td>
  <th width="45%" align="center"><b>Design Target</b></td>
 </tr>

 <tr>
  <td align="center"><a href="../Getting_Started/Vitis_Platform/README.md">Versal Platform Creation Quick Start</a></td>
  <td>Versal AI Core</td>
  <td>VCK190</td>
  <td>Flat</td>
  <td>Vivado &<br>Vitis Unified IDE</td>
  <td><b>Highlights</b>: simplest Vitis Platform creation and usage flow.
      <ul>
        <li><b>Hardware design</b>: Using Vivado Customizable Example Design tempalte to quick start.  </li>
        <li><b>Software design</b>: Using `createdts` and Common Image to quick start. </li>
        <li><b>Verification</b>: Vector Addition.  </li>
      </ul>
      <b>Note</b>: This design flow is applicable to most AMD demo boards.
  </td>
 </tr>

 <tr>
  <td align="center"><a href="./Design_Tutorials/03_Edge_VCK190/README.md">Create a Vitis Platform for Custom Versal Boards</a></td>
  <td>Versal AI Core</td>
  <td>VCK190</td>
  <td>Flat</td>
  <td>Vivado &<br>Vitis IDE</td>
  <td><b>Highlights</b>: Platform design flow for custom boards.
      <ul>
        <li><b>Hardware design</b>: Using Vivado Customizable Example Design (device part based) to create the hardware and do further customizations. </li>
        <li><b>Software design</b>: Using `createdts` and Common Image to quick start. </li>
        <li><b>Verification</b>: Vector Addition</li>
      </ul>
      <b>Note</b>: This tutorial uses VCK190 board as a custom board. The design doesn't use any of its presets.
  </td>
 </tr>

 <tr>
  <td align="center"><a href="./Design_Tutorials/04_Edge_VCK190_DFX/README.md">Versal DFX Platform Creation Tutorial</a></td>
  <td>Versal AI Core</td>
  <td>VCK190</td>
  <td>DFX</td>
  <td>Vivado &<br>Vitis IDE</td>
  <td>
    <b>Highlights</b>: Design flow for Vitis DFX (Dynamic Function eXchange) Platform.
      <ul>
        <li><b>Hardware design</b>: Using Vivado Customizable Example Design for DFX platform and doing further customizations. </li>
        <li><b>Software design</b>: Using `createdts` and Common Image to quick start. </li>
        <li><b>Verification</b>: Vector Addition</li>
      </ul>
  </td>
 </tr>

 <tr>
  <td align="center"><a href="./Design_Tutorials/02-Edge-AI-ZCU104/README.md">Create Vitis Platforms for Zynq UltraScale+ MPSoC</a></td>
  <td>Zynq UltraScale+ MPSoC</td>
  <td>ZCU104</td>
  <td>Flat</td>
  <td>Vivado &<br>Vitis IDE</td>
  <td>
    <b>Highlights</b>: Creating a Vitis platform for Zynq UltraScale+ MPSoC from scratch.
      <ul>
        <li><b>Hardware design</b>: Creating the hardware design from scratch without any help from Vivado example design templates. </li>
        <li><b>Software design</b>: Using `createdts` and Common Image to quick start. </li>
        <li><b>Verification</b>: Vector Addition and Vitis-AI</li>
      </ul>
  
  </td>
 </tr>

  <tr>
  <td align="center"><a href="./Design_Tutorials/01-Edge-KV260/README.md">Custom Kria SOM Platform Creation Example</a></td>
  <td>ZYNQ UltraScale+ MPSoC</td>
  <td>KV260</td>
  <td>Flat</td>
  <td>Vivado &<br>Vitis IDE</td>
  <td>
      <b>Highlights</b>: Kria SOM Platform creation and usage flow
      <ul>
        <li><b>Hardware design</b>: Creating from scratch </li>
        <li><b>Software design</b>: Using Common Image and showing device tree overlay (DTBO) creation flow</li>
        <li><b>Verification</b>: Vector Addition and Kria SOM application loading procedure</li>
      </ul>
  </td>
 </tr>
</table>

### Feature Tutorials

These tutorials illustrate various platform features and how you can incorporate them into your own custom platforms.

<table style="width:100%">
 <tr>
  <th width="21%" align="center"><b>Tutorial</b></td>
  <th width="8%" align="center"><b>Device Family</b></td>
  <th width="8%" align="center"><b>Board</b></td>
  <th width="6%" align="center"><b>Platform Type</b></td>
  <th width="12%" align="center"><b>IDE Flow</b></td>
  <th width="45%" align="center"><b>Design Target</b></td>
 </tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/01_platform_creation_streaming_ip/">Incorporating Stream Interfaces</a></td>
  <td>Generic, but using Versal AI Core as example</td>
  <td>VCK190</td>
  <td>Flat</td>
  <td>Vivado &<br>Vitis IDE</td>
  <td>
      <b>Highlights</b>:
      <ul>
        <li>Adding custom IP into the platform hardware</li>
        <li>Using AXI Stream IP in platform and kernel</li>
      </ul>
  </td>
 </tr>

  <tr>
    <td align="center"><a href="./Feature_Tutorials/02_petalinux_customization/">PetaLinux Building and System Customization</a></td>
    <td>ZYNQ UltraScale+ MPSoC and Versal AI Core</td>
    <td>ZCU104 and VCK190</td>
    <td>Flat</td>
    <td>Vivado &<br>Vitis IDE</td>
    <td>
      <b>Highlights</b>: Customize the software components with PetaLinux
    </td>
  </tr>

  <tr>
    <td align="center"><a href="./Feature_Tutorials/03_Vitis_Export_To_Vivado/">Hardware Design Fast Iteration with Vitis Export to Vivado</a></td>
    <td>Versal AI Core</td>
    <td>VCK190</td>
    <td>Block Design Container</td>
    <td>Vivado &<br>Vitis IDE</td>
    <td>
      <b>Highlights</b>:
      <ul>
        <li>Skip creating the platform before v++ linking</li>
        <li>Using Vivado to do design implementation and timing closure</li>
        <li>Fast iteration for hardware design</li>
      </ul>
    </td>
  </tr>
  
  <tr>
    <td align="center"><a href="./Feature_Tutorials/04_platform_validation/">Hardware Design Validation</a></td>
    <td>Versal AI Core</td>
    <td>VCK190</td>
    <td>Flat & Block Design Container</td>
    <td>Vivado &<br>Vitis IDE</td>
    <td>
      <b>Highlights</b>:
      <ul>
        <li>Link kernel with XSA directly</li>
        <li>Validation against hardware platform interfaces</li>
        <li>Validate the hardware design with bare-metal application</li>
      </ul>
    </td>
  </tr>


</table>

## Abbreviation

- **XSA**   : Vivado exported archive file that contains hardware information required for Vitis and PetaLinux
- **DFX**   ：Dynamic Function eXchange
- **SOM**   : System-on-Modules
- **DTB**   : Device Tree Binary
- **DTBO**  : Device Tree Binary Overlay




<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
