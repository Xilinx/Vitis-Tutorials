<p align="right"><a>English</a> | <a href="docs-jp/README.md">日本語</a></p>
<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AMD Vitis™ In-Depth Tutorials</h1>
    </td>
 </tr>
</table>



<div align="center">
    <a href="http://www.youtube.com/watch?v=4JijWoV6lH4">
    <img src="./Getting_Started/Vitis/images/intro_video_new.png">
    </a>
</div>
<p align="center">
Visit more Vitis developer videos on <a href="https://www.youtube.com/channel/UCkzIS3hJplxSbVRxRQJW4Ow">Adaptive Computing Developer YouTube Channel</a>
</p>

## Unlocking a New Design Experience For All Developers

The [Vitis software platform](https://www.xilinx.com/products/design-tools/vitis/vitis-platform.html) is a development environment for developing designs that include FPGA fabric, Arm® processor subsystems, and AI Engines. The Vitis tools work in conjunction with AMD Vivado™ ML Design Suite to provide a higher level of abstraction for design development. Learn how to use Vitis to implement a fully end-to-end application using software-defined flows.

## Where to Start

If you are new to the Vitis software platform and want to start with the basics, or just want to get a quick overview of what Vitis can offer, look at the tutorials under [Getting Started](./Getting_Started/README.md). From there, explore other tutorials on different topics.

Otherwise, if you are looking for a specific tutorial for the desired device or platform, or are interested in a special application or feature, you can select a tutorial from the topics as listed under the [Tutorials](#tutorials).

In this repository, tutorials are divided into different topics by function and application with each topic containing 2 sections.

- **Feature Tutorials** illustrate specific features or flows of Vitis, Libraries, XRT and platforms, some features may not be required by all designs but are still useful for some use cases.
- **Design Tutorials** illustrate higher-level concepts or design flows, walk through specific examples or reference designs, and more complex and complete designs or applications.

## How to Get Help

- Check the [FAQ](./FAQs.md).
- For questions about the Vitis software platform, visit the [Vitis Forum](https://support.xilinx.com/s/topic/0TO2E000000YKXhWAO/vitis).
- For questions or issues about tutorials, create an [Issue](https://github.com/Xilinx/Vitis-Tutorials/issues).

## How to Download the Repository

To get a local copy of the [Vitis-Tutorials](https://github.com/Xilinx/Vitis-Tutorials) repository, clone it to your local system by executing the following command:

```git
git clone https://github.com/Xilinx/Vitis-Tutorials.git
```

The default branch is always consistent with the most recently released version of the Vitis software platform. If you need to run a tutorial on a different version, after you clone the repository, use the `git checkout <branch>` command to specify a branch that matches the tool version you are using.

Alternatively, you can also download repository contents as a ZIP file. The downloaded ZIP file will contain *only* the selected branch, and its overall size will be smaller than a cloned repository.

To download a ZIP file of a specific branch, do one of the following:

- From a browser, select the desired branch. Next, click the green `Code` button and select `Download ZIP`.
- From a terminal, execute the following command. The following uses the 2024.1 branch as an example.

  ```
  wget https://github.com/Xilinx/Vitis-Tutorials/archive/refs/heads/2024.1.zip && unzip 2024.1.zip 
  ```

## Release Notes

[Change Log](./CHANGELOG.md)

## Tutorials

<table border="0" width="100%">
    <tbody>
        <tr>
            <td colspan="3" height="24" align="center">
                <strong> <a href="./Getting_Started">Getting Started</a> </strong>
            </td>
        </tr>
        <tr>
            <td colspan="3" height="48">
                Start here! Learn the basics of the Vitis programming model by putting together your very first application. No experience necessary!
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./Getting_Started/Vitis">Vitis Introduction</a>
            </td>
            <td>
                <a href="./Getting_Started/Vitis_HLS">Vitis HLS Introduction</a>
            </td>
        </tr>
        <tr>
            <td>
                <a href="./Getting_Started/Vitis_Libraries">Vitis Libraries Introduction</a>
            </td>
            <td>
                <a href="./Getting_Started/Vitis_Platform/">Vitis Platform Introduction </a>
            </td>
        </tr>
        <tr>
            <td>
                <a href="./Embedded_Software/Getting_Started">Vitis Unified IDE for Embedded Design</a>
            </td>
            <td>
            </td>
        </tr>        
    </tbody>
</table>

<table border="0" width="100%">
    <tbody>
        <tr>
            <td colspan="3" height="24" align="center">
                <strong> <a href="./AI_Engine_Development/AIE-ML/">AI Engine Development on AIE-ML </a></strong>
            </td>
        </tr>
        <tr>
            <td colspan="3" height="48">
                Learn how to target, develop, and deploy advanced algorithms using Versal AIE-ML architecture in conjunction with PL IP/kernels and software applications running on the embedded processors.
            </td>
        </tr>
        <tr>
            <td colspan="2" height="24">
                <strong>Feature Tutorials</strong>
            </td>
            <td height="24">
                <strong>Design Tutorials</strong>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE-ML/Feature_Tutorials/01-aie_a_to_z">A to Z Bare-metal Flow </a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE-ML/Feature_Tutorials/02-using-gmio">Using GMIO with AIE-ML </a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE-ML/Design_Tutorials/01-AIE-ML-programming-and-optimization">AIE-ML Programming </a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE-ML/Feature_Tutorials/03-rtp-reconfiguration">Runtime Parameter Reconfiguration</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE-ML/Feature_Tutorials/04-packet-switching">Packet Switching</a>
            </td>                
            <td> 
                <a href="./AI_Engine_Development/AIE-ML/Design_Tutorials/02-Prime-Factor-FFT">Prime Factor FFT-1008 on AIE-ML </a>               
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE-ML/Feature_Tutorials/05-AI-engine-versal-integration">Versal Integration for HW Emu and HW</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE-ML/Feature_Tutorials/20-aiecompiler-features">AIE Compiler Features </a>
            </td>                
            <td>
                <a href="./AI_Engine_Development/AIE-ML/Design_Tutorials/03-AIE-ML-lenet_tutorial">AIE-ML LeNet Tutorial </a>
            </td>
        </tr>        
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE-ML/Feature_Tutorials/13-aie-ml-performance-analysis">AIE-ML Performance Analysis </a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE-ML/Feature_Tutorials/22-Performance-Validation-in-Analysis-View">Performance Validation in Analysis View :new: </a>   
            </td>                
            <td>
                <a href="./AI_Engine_Development/AIE-ML/Design_Tutorials/04-AIE-API-based-FFT-for-many-instances-applications">AIE API based FFT for Many Instances Applications :new:</a>
            </td>
        </tr>   
        <tr>
            <td height="24">
                <a href="./Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System">Versal Custom Thin Platform Extensible System</a>    
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE-ML/Design_Tutorials/06-farrow_filter/">Migrating Farrow Filter from AIE to AIE-ML :new:</a>              
            </td>                
            <td>
                <a href="./AI_Engine_Development/AIE-ML/Design_Tutorials/05-Softmax-Function">Softmax Function on AIE-ML :new:</a>
            </td>
        </tr>           
    </tbody>
</table>

<table border="0" width="100%">
    <tbody>
        <tr>
            <td colspan="3" height="24" align="center">
                <strong> <a href="./AI_Engine_Development/AIE/">AI Engine Development on AIE</a></strong>
            </td>
        </tr>
        <tr>
            <td colspan="3" height="48">
                Learn how to target, develop, and deploy advanced algorithms using a Versal AI Engine array in conjunction with PL IP/kernels and software applications running on the embedded processors.
            </td>
        </tr>
        <tr>
            <td colspan="2" height="24">
                <strong>Feature Tutorials</strong>
            </td>
            <td height="24">
                <strong>Design Tutorials</strong>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/05-AI-engine-versal-integration">Versal Integration for HW Emu and HW</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/02-using-gmio">Using GMIO with AIE</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/01-aie_lenet_tutorial">LeNet Tutorial</a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/03-rtp-reconfiguration">Runtime Parameter Reconfiguration</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/04-packet-switching">Packet Switching</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/02-super_sampling_rate_fir">Super Sampling Rate FIR Filters</a>
            </td>
        </tr>
        <tr>
            <td height="24"> 
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/01-aie_a_to_z">A to Z Bare-metal Flow</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/06-versal-system-design-clocking-tutorial">Versal System Design Clocking</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/03-beamforming">Beamforming Design</a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/07-AI-Engine-Floating-Point">Using Floating-Point in the AI Engine</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/08-dsp-library">DSP Library Tutorial</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/06-fft2d_AIEvsHLS">2D-FFT</a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/09-debug-walkthrough">Debug Walkthrough Tutorial</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/10-aie-dsp-lib-model-composer">AIE DSP Library and Model Composer</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/07-firFilter_AIEvsHLS">FIR Filter</a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/11-ai-engine-emulation-waveform-analysis">Versal Emulation Waveform Analysis</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/12-axis-traffic-generator">AXIS External Traffic Generator</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/08-n-body-simulator">N-Body Simulator</a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/13-aie-performance-analysis">AIE Performance and Deadlock Analysis</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/14-implementing-iir-filter">Implementing an IIR Filter on the AIE</a>
            </td>
            <td>
                 <a href="./AI_Engine_Development/AIE/Design_Tutorials/10-GeMM_AIEvsDSP">Versal GeMM Implementation</a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/15-post-link-recompile">Post-Link Recompile of an AI Engine Application</a>
            </td>
            <td height="24">
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/16-external-traffic-generator-aie">Python and C++ External Traffic Generators</a>
            </td>
            <td>
                 <a href="./AI_Engine_Development/AIE/Design_Tutorials/04-Polyphase-Channelizer">Polyphase Channelizer </a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/17-RTL-IP-with-AIE-Engines">Using RTL IP with AI Engines</a>
            </td>
            <td height="24">
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/18-aie_a_to_z_custom_linux_platform">AI Engine A-to-Z Flow for Linux</a>
            </td>
            <td>
                 <a href="./AI_Engine_Development/AIE/Design_Tutorials/05-Prime-Factor-FFT">Prime Factor FFT </a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/19-aie_external_io_sv">Using Verilog Traffic Generators in AIE Simulation</a>
            </td>
            <td>
                 <a href="./Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System">Versal Custom Thin Platform Extensible System</a>
            </td>
            <td>
                 <a href="./AI_Engine_Development/AIE/Design_Tutorials/09-ddc_chain">Digital Down-conversion Chain </a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/20-aiecompiler-features">AIE Compiler Features </a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/21-two_tone_filter">Two Tone Filter </a>            
            </td>
            <td>
                 <a href="./AI_Engine_Development/AIE/Design_Tutorials/11-Bilinear_Interpolation">Bilinear Interpolation </a>
            </td>
        </tr>
        <tr>
            <td height="24">
                 <a href="./AI_Engine_Development/AIE/Feature_Tutorials/22-Performance-Validation-in-Analysis-View">Performance Validation in Analysis View :new: </a>   
            </td>                 
            <td>
                 <a href="./AI_Engine_Development/AIE/Feature_Tutorials/24-aie_pl_interface">RTL / AI Engine interfacing Examples :new: </a>          
            </td>
            <td>
                 <a href="./AI_Engine_Development/AIE/Design_Tutorials/12-IFFT64K-2D">64K IFFT Using 2D Architecture </a>
            </td>
        </tr>
        <tr>
            <td colspan="3" height="24" align="center">
                <strong> Design Tutorials </strong>
            </td>
        </tr>        
        <tr>
            <td height="24">
                 <a href="./AI_Engine_Development/AIE/Design_Tutorials/13-FFT-DFT-on-AIE">FFT and DFT on AI Engine </a>                    
            </td>
            <td >
                 <a href="./AI_Engine_Development/AIE/Design_Tutorials/14-Bitonic-Sorting">Bitonic SIMD Sorting on AI Engine </a>                     
            </td>
            <td>
                 <a href="./AI_Engine_Development/AIE/Design_Tutorials/15-farrow_filter">Fractional Delay Farrow Filter </a>
            </td>          
        </tr>
        <tr>
            <td height="24">                   
                 <a href="./AI_Engine_Development/AIE/Design_Tutorials/16-1M-Point-FFT-32Gsps">1M Point float FFT @ 32 Gsps :new: </a>
            </td>                
            <td>
                 <a href="./AI_Engine_Development/AIE/Design_Tutorials/17-Hough-Transform">System Partitioning of a Hough Transform :new: </a>
            </td>              
            <td>
                 <a href="./AI_Engine_Development/AIE/Design_Tutorials/18-MUSIC-Algorithm">MUSIC Algorithm :new: </a>
            </td>            
        </tr>
        <tr>
            <td height="24">                   
                 <a href="./AI_Engine_Development/AIE/Design_Tutorials/19-Softmax-Function"> Softmax Function on AI Engine :new: </a>
            </td>                
            <td>
            </td>              
            <td>
            </td>            
        </tr>                             
    </tbody>
</table>

<table border="0" width="100%">
    <tbody>
        <tr>
            <td colspan="3" height="24" align="center">
                <strong> <a href="./Embedded_Software/">Vitis Embedded Software Development  </a></strong>
            </td>
        </tr>
        <tr>
            <td colspan="3" height="48">
                Introduce Vitis embedded design flows, learn the Vitis Unified IDE for developing embedded software applications targeted towards AMD embedded processors.
            </td>
        </tr>
        <tr>
            <td height="24">
                <strong>Getting Started</strong>
            </td>
            <td colspan="2" height="24">
                <strong>Feature Tutorials</strong>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./Embedded_Software/Getting_Started">Getting Started in Vitis Unified IDE</a>
            </td>
            <td> 
                <a href="./Embedded_Software/Feature_Tutorials/01-user_managed_mode">User Managed Mode</a>
            </td>
            <td>
                <a href="./Embedded_Software/Feature_Tutorials/03-vitis_classic_to_unified_migration">Migrating from classic Vitis IDE to Vitis Unified IDE  </a> 
            </td>
        </tr>  
        <tr>
            <td height="24"> </td>
            <td>
                <a href="./Embedded_Software/Feature_Tutorials/02-Debugging">Vitis Embedded Software Debugging Guide</a>            
            </td>
            <td> 
                <a href="./Embedded_Software/Feature_Tutorials/04-vitis_scripting_flows">Vitis Embedded Scripting Flow</a>
            </td>
        </tr>  
    </tbody>
</table>

<table border="0" width="100%">
    <tbody>
        <tr>
            <td colspan="3" height="24" align="center">
                <strong> <a href="./Vitis_HLS/">Vitis HLS</a></strong>
            </td>
        </tr>
        <tr>
            <td colspan="3" height="48">
                Vitis High-Level Synthesis (HLS) lets you compile C/C++ code into RTL code. These tutorials offer a broader introduction to the Vitis HLS flows and use cases.
            </td>
        </tr>
        <tr>
            <td colspan="2" height="24">
                <strong>Feature Tutorials</strong>
            </td>
            <td height="24">
                <strong>Design Tutorials</strong>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./Vitis_HLS/Feature_Tutorials/01-using_code_analyzer">Using Code Analyzer from Vitis Unified IDE</a>
            </td>
            <td>
                <a href="./Vitis_HLS/Feature_Tutorials/02-Beamformer_Analysis">HLS Micro-Optimization Tutorial using Beamformer IP</a>
            </td>
            <td>
                <a href="./Vitis_HLS/Design_Tutorials/01-Polynomial_Vectorization">Polyvec NTT Tutorial using Code Analyzer :new: </a>
            </td>
        </tr>
        <tr>
            <td height="24">
            </td> 
            <td>
            </td> 
            <td>
                <a href="./Vitis_HLS/Design_Tutorials/02-Beamformer">Adaptive Beamforming for Radar:
Floating-Point QRD+WBS in an FPGA</a>
            </td>
        </tr>          
    </tbody>
</table>

<table border="0" width="100%">
    <tbody>
        <tr>
            <td colspan="3" height="24" align="center">
                <strong> <a href="./Vitis_Platform_Creation">Vitis Platform Creation</a></strong>
            </td>
        </tr>
        <tr>
            <td colspan="3" height="48">
                Learn how to build custom platforms for Vitis to target your own boards built with Xilinx devices, and how to modify and extend existing platforms.
            </td>
        </tr>
        <tr>
            <td height="24">
                <strong>Design Tutorials</strong>
            </td>
            <td height="24">
                <strong>Feature Tutorials</strong>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./Vitis_Platform_Creation/Design_Tutorials/02-Edge-AI-ZCU104">Custom Platform Creation on MPSoC</a>
            </td>
            <td>
                <a href="./Vitis_Platform_Creation/Feature_Tutorials/01_platform_creation_streaming_ip">Incorporating Stream Interfaces</a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./Vitis_Platform_Creation/Design_Tutorials/03_Edge_VCK190">Custom Platform Creation on Versal</a>
            </td>
            <td>
                <a href="./Vitis_Platform_Creation/Feature_Tutorials/02_petalinux_customization">PetaLinux Building and System Customization</a>
            </td>
        </tr>
            <tr>
                <td>
                    <a href="./Vitis_Platform_Creation/Design_Tutorials/01-Edge-KV260">Custom Platform Creation on KV260</a>
                </td>
                <td height="24">
                    <a href="./Vitis_Platform_Creation/Feature_Tutorials/03_Vitis_Export_To_Vivado">Hardware Design Fast Iteration with Vitis Export to Vivado</a>
                </td>
            </tr>
            <tr>
                <td>
                    <a href="./Vitis_Platform_Creation/Design_Tutorials/04_Edge_VCK190_DFX">Versal Custom DFX Platform Creation Tutorial</a>
                </td>
                <td height="24">
                  <a href="./Vitis_Platform_Creation/Feature_Tutorials/04_platform_validation">Versal Extensible Hardware Design Validation</a>
                </td>
            </tr>
        </tbody>
    </table>

<table border="0" width="100%">
        <tbody>
            <tr>
                <td colspan="2" height="24" align="center">
                    <strong> <a href="./Developer_Contributed">Vitis Developer Contributed Tutorials</a></strong>
                </td>
            </tr>
            <tr>
                <td colspan="2" height="48">
                    Check out tutorials that other developers shared! We welcome your contribution, you may share end-to-end designs, tips
                    and tricks, or designs and examples that can help Xilinx users.
                </td>
            </tr>
            <tr>
                <td height="24">
                    <a href="./Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System">Versal Custom Thin Platform
                        Extensible System</a>
                </td>
                <td height="24">
                    <a href="./Developer_Contributed/02-AIE_DSP_with_Makefile_and_GUI">DSP Design on AI Engine with GUI and Makefile Flows</a>
                </td>
            </tr>
            <tr>
                <td height="24">
                    <a href="./Developer_Contributed/03-HLS_Code_Optimization">Vitis HLS Optimization Techniques on Embedded Boards</a>
                </td>
                <td height="24">
                </td>
            </tr>
        </tbody>
    </table>

<table border="0" width="100%">
    <tbody>
        <tr>
            <td colspan="2" height="24" align="center">
                <strong> <a href="./Hardware_Acceleration">Hardware Acceleration</a> </strong>
            </td>
        </tr>
        <tr>
            <td colspan="2" height="48">
                Learn how to use the Vitis core development kit to build, analyze, and optimize an accelerated algorithm developed in C++, OpenCL, and even Verilog and VHDL.
            </td>
        </tr>
        <tr>
            <td height="24">
                <strong>Feature Tutorials</strong>
            </td>
            <td height="24">
                <strong>Design Tutorials</strong>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./Hardware_Acceleration/Feature_Tutorials/01-rtl_kernel_workflow">Getting Started with RTL Kernels</a>
            </td>
            <td>
                <a href="./Hardware_Acceleration/Design_Tutorials/01-convolution-tutorial">Convolution Example</a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./Hardware_Acceleration/Feature_Tutorials/02-mixing-c-rtl-kernels">Mixing C and RTL</a>
            </td>
            <td>
                <a href="./Hardware_Acceleration/Design_Tutorials/02-bloom">Bloom Filter Example</a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./Hardware_Acceleration/Feature_Tutorials/03-dataflow_debug_and_optimization">Dataflow Debug and Optimization</a>
            </td>
            <td>
                <a href="./Hardware_Acceleration/Design_Tutorials/03-rtl_stream_kernel_integration">RTL Systems Integration Example</a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./Hardware_Acceleration/Feature_Tutorials/04-mult-ddr-banks">Using Multiple DDR Banks</a>
            </td>
            <td>
                <a href="./Hardware_Acceleration/Design_Tutorials/04-traveling-salesperson">Traveling Salesperson Problem</a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./Hardware_Acceleration/Feature_Tutorials/05-using-multiple-cu">Using Multiple Compute Units</a>
            </td>
            <td>
                <a href="./Hardware_Acceleration/Design_Tutorials/05-bottom_up_rtl_kernel">Bottom RTL Kernel Design Flow Example</a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./Hardware_Acceleration/Feature_Tutorials/06-controlling-vivado-implementation">Controlling Vivado Implementation</a>
            </td>
            <td>
                <a href="./Hardware_Acceleration/Design_Tutorials/06-cholesky-accel">Cholesky Algorithm Acceleration</a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./Hardware_Acceleration/Feature_Tutorials/07-using-hbm">Optimizing for HBM</a>
            </td>
            <td>
                <a href="./Hardware_Acceleration/Design_Tutorials/07-host-code-opt">XRT Host Code Optimization</a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./Hardware_Acceleration/Feature_Tutorials/08-using-hostmem">Host Memory Access</a>
            </td>
            <td>
                <a href="./Hardware_Acceleration/Design_Tutorials/08-alveo_aurora_kernel">Aurora Kernel on Alveo</a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./Hardware_Acceleration/Feature_Tutorials/09-using-ethernet-on-alveo">Using GT Kernels and Ethernet IPs on Alveo</a>
            </td>
            <td>
                <a href="./Hardware_Acceleration/Design_Tutorials/09-sssp-application">Single Source Shortest Path Application</a>
            </td>
        </tr>
    <tr>
            <td height="24">
                <a href="./Hardware_Acceleration/Feature_Tutorials/10-p2p-card-to-card">P2P Transfer using Native XRT C++ API</a>
            </td>
            <td>
                <a href="./Hardware_Acceleration/Design_Tutorials/10-get_moving_with_alveo">Get Moving with Alveo</a>
            </td>
        </tr>
    </tbody>
</table>

## Other Vitis Tutorial Repositories

<table border="0" width="100%">
    <tbody>
        <tr>
            <td width="257">
                <strong>Tutorial Repository</strong>
            </td>
            <td>
                <strong>Description</strong>
            </td>
        </tr>
        <tr>
            <td>
                <a href="https://github.com/Xilinx/Vitis-HLS-Introductory-Examples">Introductory examples for Vitis HLS</a>
            </td>
            <td>
                This repository contains introductory examples for Vitis HLS that demonstrate specific scenarios related to coding styles and optimization methods. They can help you get started with coding and optimization using Vitis HLS.
            </td>
        </tr>
        <tr>
            <td>
                <a href="https://github.com/Xilinx/Vitis_Accel_Examples">Vitis Acceleration Examples</a>
            </td>
            <td>
                This repository illustrates specific scenarios related to host code and kernel programming through small working examples. They can get you started with Vitis acceleration application coding and optimization.
            </td>
        </tr>
        <tr>
            <td>
                <a href="https://github.com/xilinx/Vitis-AI-Tutorials">Machine Learning Tutorials</a>
            </td>
            <td>
                The repository helps to get you the lay of the land working with machine learning and the Vitis AI toolchain on Xilinx devices. It illustrates specific workflows or stages within Vitis AI and gives examples of common use cases.
            </td>
        </tr>
        <tr>
            <td>
                <a href="https://github.com/xilinx/Embedded-Design-Tutorials">Embedded Design Tutorials</a>
            </td>
            <td>
                Learn how to build and use embedded operating systems and drivers on Xilinx Adaptive SoCs and the MicroBlaze™ soft processor. These tutorials cover open-source operating systems and bare-metal drivers available from Xilinx, compilers, debuggers, and profiling tools for traditional SoC software development.
            </td>
        </tr>
        <tr>
            <td>
                <a href="https://github.com/Xilinx/Vitis_Model_Composer">Vitis Model Composer Tutorials</a>
            </td>
            <td>
                Learn rapid design exploration using Vitis Model Composer. Vitis Model Composer provides a library of performance-optimized blocks for design and implementation of DSP algorithms on Xilinx devices. The Vitis Model Composer AI Engine, HLS and HDL libraries within the Simulink™ environment, enable the rapid design exploration of an algorithm and accelerate the path to production.
            </td>
        </tr>
    </tbody>
</table>


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2024 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
