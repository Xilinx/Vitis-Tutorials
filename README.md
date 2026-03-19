<p class="sphinxhide" align="right"><a>English</a> | <a href="docs-jp/README.md">日本語</a></p>
<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ In-Depth Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
    </td>
  </tr>
</table>

## Unlocking a New Design Experience For All Developers

The AMD [Vitis™ Unified Software Platform](https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html) is a development environment for developing designs that include FPGA fabric, Arm® processor subsystems, and AI Engines. The Vitis tools work in conjunction with AMD Vivado™ ML Design Suite to provide a higher level of abstraction for design development. Learn how to use Vitis to implement a fully end-to-end application using software-defined flows.

## Where to Start

If you are new to the Vitis software platform and want to start with the basics, or just want to get a quick overview of what Vitis can offer, look at the tutorials under [Getting Started](./Getting_Started/README.md). From there, explore other tutorials on different topics.

Otherwise, if you are looking for a specific tutorial for the desired device or platform, or are interested in a special application or feature, you can select a tutorial from the topics as listed under the [Tutorials](#tutorials).

In this repository, tutorials are organized by topic based on function and application. Each topic includes two sections:

- **Feature Tutorials** focus on specific features or flows, such as AI Engine, embedded software, HLS, or platform creation. These features may not be essential for every design but are valuable for certain use cases.
- **Design Tutorials** cover higher-level concepts and design flows. They guide you through detailed examples or reference designs and showcase more complex, complete applications.

## How to Get Help

- Check the [FAQ](./FAQs.md).
- For questions about the Vitis software platform, visit the [Vitis Forum](https://support.xilinx.com/s/topic/0TO2E000000YKXhWAO/vitis).
- For questions or issues about tutorials, create an [Issue](https://github.com/Xilinx/Vitis-Tutorials/issues).

## How to Download the Repository

To get a local copy of the [Vitis-Tutorials](https://github.com/Xilinx/Vitis-Tutorials) repository, clone it to your local system by executing the following command:

```git
git clone https://github.com/Xilinx/Vitis-Tutorials.git
```

The default branch always reflects the latest Vitis release. If you need a different version, run `git checkout <branch>` after cloning the repository to switch to the appropriate branch.

Alternatively, you can download the repository as a ZIP file. This includes only the selected branch and is smaller than a full clone. To download a ZIP file of a specific branch, do one of the following:
- From a browser, select the desired branch. Next, click the green `Code` button and select `Download ZIP`.
- From a terminal, execute the following command. The following uses the 2025.2 branch as an example.

```code
wget https://github.com/Xilinx/Vitis-Tutorials/archive/refs/heads/2025.2.zip && unzip 2025.2.zip 
```

If you already know exactly which tutorials or folders you need, `git sparse-checkout` is an option to download only those parts. Please refer to Git documentation for usage details.

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
            <td colspan="4" height="24" align="center">
                <strong> <a href="./AI_Engine_Development/AIE-MLv2/">AI Engine Development on AIE-MLv2 :new: </a></strong>
            </td>
        </tr>
        <tr>
            <td colspan="4" height="48">
                Learn how to target, develop, and deploy advanced algorithms using Versal AIE-MLv2 architecture in conjunction with PL IP/kernels and software applications running on the embedded processors.
            </td>
        </tr>
        <tr>
            <td height="24">
                <strong>Feature Tutorials</strong>
            </td>
            <td colspan="3" height="24">
                <strong>Design Tutorials</strong>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE-MLv2/Feature_Tutorials/01-aie-mlv2_a_to_z/">A to Z AIE Bare-metal Flow for VEK385 :new:</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE-MLv2/Design_Tutorials/01-Radio-ML">Radio-ML on AIE-MLv2 :new:</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE-MLv2/Design_Tutorials/02-Channelizer-Using-Vitis-Libraries/">Channelizer on AIE-MLv2 using Vitis Libraries :new:</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE-MLv2/Design_Tutorials/06-farrow_filter/">Fractional Delay Farrow Filter on AIE-MLv2 :new:</a>
            </td>                                  
        </tr>     
    </tbody>
</table>

<table border="0" width="100%">
    <tbody>
        <tr>
            <td colspan="4" height="24" align="center">
                <strong> <a href="./AI_Engine_Development/AIE-ML/">AI Engine Development on AIE-ML </a></strong>
            </td>
        </tr>
        <tr>
            <td colspan="4" height="48">
                Learn how to target, develop, and deploy advanced algorithms using Versal AIE-ML architecture in conjunction with PL IP/kernels and software applications running on the embedded processors.
            </td>
        </tr>
        <tr>
            <td colspan="2" height="24">
                <strong>Feature Tutorials</strong>
            </td>
            <td colspan="2" height="24">
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
            <td> 
                <a href="./AI_Engine_Development/AIE-ML/Design_Tutorials/02-Prime-Factor-FFT">Prime Factor FFT-1008 on AIE-ML </a>               
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
                <a href="./AI_Engine_Development/AIE-ML/Design_Tutorials/03-AIE-ML-lenet_tutorial">AIE-ML LeNet Tutorial </a>              
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE-ML/Design_Tutorials/04-AIE-API-based-FFT-for-many-instances-applications">AIE API based FFT for Many Instances Applications</a>
            </td>            
        </tr>
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE-ML/Feature_Tutorials/05-AI-engine-versal-integration">Versal Integration for HW Emu and HW</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE-ML/Feature_Tutorials/13-aie-ml-performance-analysis">AIE-ML Performance Analysis </a>     
            </td>                
            <td>
                <a href="./AI_Engine_Development/AIE-ML/Design_Tutorials/05-Softmax-Function">Softmax Function on AIE-ML</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE-ML/Design_Tutorials/06-farrow_filter">Migrating Farrow Filter from AIE to AIE-ML</a>              
            </td> 
        </tr>        
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE-ML/Feature_Tutorials/20-aiecompiler-features">AIE Compiler Features </a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE-ML/Feature_Tutorials/06-gemm/">Matrix Compute with Vitis Libraries</a>
            </td>                
            <td>
                <a href="./Vitis_System_Design/Design_Tutorials/01-Versal_Custom_Thin_Platform_Extensible_System">Versal Custom Thin Platform Extensible System</a>           
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE-ML/Design_Tutorials/07-Channelizer-Using-Vitis-Libraries">Polyphase Channelizer on AIE-ML using Vitis Libraries</a>                     
            </td>             
        </tr>            
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE-ML/Feature_Tutorials/07-tiling-parameters">Tiling Parameter Programming</a>   
            </td>         
            <td>   
            </td>                
            <td>
                <a href="./AI_Engine_Development/AIE-ML/Design_Tutorials/08-MNIST-ConvNet">MNIST ConvNet on AIE-ML</a>         
            </td>
            <td>   
            </td>             
        </tr>           
    </tbody>
</table>

<table border="0" width="100%">
    <tbody>
        <tr>
            <td colspan="4" height="24" align="center">
                <strong> <a href="./AI_Engine_Development/AIE/">AI Engine Development on AIE</a></strong>
            </td>
        </tr>
        <tr>
            <td colspan="4" height="48">
                Learn how to target, develop, and deploy advanced algorithms using a Versal AI Engine array in conjunction with PL IP/kernels and software applications running on the embedded processors.
            </td>
        </tr>
        <tr>
            <td colspan="2" height="24">
                <strong>Feature Tutorials</strong>
            </td>
            <td colspan="2" height="24">
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
            <td>
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/02-super_sampling_rate_fir">Super Sampling Rate FIR Filters</a>
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
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/03-beamforming">Beamforming Design</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/04-Polyphase-Channelizer">Polyphase Channelizer</a>
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
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/05-Prime-Factor-FFT">Prime Factor FFT </a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/06-fft2d_AIEvsHLS">2D-FFT</a>
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
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/07-firFilter_AIEvsHLS">FIR Filter</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/08-n-body-simulator">N-Body Simulator</a>
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
                 <a href="./AI_Engine_Development/AIE/Design_Tutorials/09-ddc_chain">Digital Down-conversion Chain</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/10-GeMM_AIEvsDSP">Versal GeMM Implementation</a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/11-ai-engine-emulation-waveform-analysis">Versal Emulation Waveform Analysis</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/13-aie-performance-analysis">AIE Performance and Deadlock Analysis</a>
            </td>
            <td>
                <a href="./Vitis_System_Design/Design_Tutorials/01-Versal_Custom_Thin_Platform_Extensible_System">Versal Custom Thin Platform Extensible System</a>                    
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/11-Bilinear_Interpolation">Bilinear Interpolation</a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/14-implementing-iir-filter">Implementing an IIR Filter on the AIE</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/15-post-link-recompile">Post-Link Recompile of an AI Engine Application</a>
            </td>
            <td >
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/12-IFFT64K-2D">64K IFFT Using 2D Architecture</a>                     
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/13-FFT-DFT-on-AIE">FFT and DFT on AI Engine</a>
            </td>  
        </tr>
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/17-RTL-IP-with-AIE-Engines">Using RTL IP with AI Engines</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/18-aie_a_to_z_custom_linux_platform">AI Engine A-to-Z Flow for Linux</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/14-Bitonic-Sorting">Bitonic SIMD Sorting on AI Engine</a>                     
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/15-farrow_filter">Fractional Delay Farrow Filter</a>
            </td> 
        </tr>
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/20-aiecompiler-features">AIE Compiler Features</a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/21-two_tone_filter">Two Tone Filter</a>
            </td>
            <td>                   
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/16-1M-Point-FFT-32Gsps">1M Point float FFT @ 32 Gsps </a>
            </td>                
            <td>
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/17-Hough-Transform">System Partitioning of a Hough Transform </a>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/23-AIE_independent_graphs/">AIE Independent Graphs </a>
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/24-aie_pl_interface">RTL / AI Engine interfacing Examples</a>   
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/18-MUSIC-Algorithm">MUSIC Algorithm </a>
            </td> 
            <td>
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/19-Softmax-Function">Softmax Function on AI Engine </a>            
            </td>
        </tr>     
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/25-AIE-kernel-optimization/">AIE Kernel Optimization </a>         
            </td>
            <td>   
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/26-gentle_intro_to_aie_kernel_programming/">AI Engine Kernel Programming :new: </a>                
            </td>
            <td>
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/20-TDM-Mixer/">Time-Division Multiplexed Mixer </a>
            </td> 
            <td>
                <a href="./AI_Engine_Development/AIE/Design_Tutorials/21-Back-Projection-SAR/">Back-Projection SAR on AIE </a>                        
            </td>
        </tr>  
        <tr>
            <td height="24">
                <a href="./AI_Engine_Development/AIE/Feature_Tutorials/27-system-timeline/">System Timeline Tutorial :new: </a>         
            </td>
            <td>              
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
            <td colspan="4" height="24" align="center">
                <strong> <a href="./Vitis_System_Design/">Vitis System Design </a></strong>
            </td>
        </tr>
        <tr>
            <td colspan="4" height="48">
                Learn how to develop applications using the Vitis Unified IDE and CLI with tutorials that focus on System design flows and their use cases.
            </td>
        </tr>
        <tr>
            <td colspan="1" height="24">
                <strong>Feature Tutorials</strong>
            </td>
            <td colspan="3" height="24">
                <strong>Design Tutorials</strong>
            </td>
        </tr>
        <tr>
            <td height="24">
                <a href="./Vitis_System_Design/Feature_Tutorials/01-Vitis_Functional_Simulation/">Vitis Functional Simulation </a>            
            </td>
            <td>
                <a href="./Vitis_System_Design/Design_Tutorials/01-Versal_Custom_Thin_Platform_Extensible_System/">Versal Custom Thin Platform Extensible System</a>    
            </td>
            <td>
                <a href="./Vitis_System_Design/Design_Tutorials/02-Versal_Vitis_Subsystem_Flow/">Versal Custom Platform Integration using Vitis Subsystem </a>
            </td>
            <td>
                <a href="./Vitis_System_Design/Design_Tutorials/03-Versal_AI_Edge_Gen2_Design_Flow/">Versal AI Edge Gen2 Design flow with Vitis Unified IDE :new: </a>
            </td>            
        </tr>     
        <tr>
            <td height="24">
                <a href="./Vitis_System_Design/Feature_Tutorials/02-Vitis_Subsystem_Simulation/">Vitis Subsystem Simulation </a>     
            </td>
            <td>
                <a href="./Vitis_System_Design/Design_Tutorials/04-Versal_AIE_Advanced_Flow_VSS_VMA_NPM/">Flexible AIE Development Flow :new: </a>        
            </td>
            <td>
                <a href="./Vitis_System_Design/Design_Tutorials/05-Versal_Segmented_configuration_Petalinux_Flow/">VCK190 Segmented Configuration with Petalinux :new: </a>
            </td>
            <td>
            </td>            
        </tr>   
    </tbody>
</table>

<table border="0" width="100%">
    <tbody>
        <tr>
            <td colspan="4" height="24" align="center">
                <strong> <a href="./Embedded_Software/">Vitis Embedded Software Development  </a></strong>
            </td>
        </tr>
        <tr>
            <td colspan="4" height="48">
                Introduce Vitis embedded design flows, learn the Vitis Unified IDE for developing embedded software applications targeted towards AMD embedded processors.
            </td>
        </tr>
        <tr>
            <td height="24">
                <strong>Getting Started</strong>
            </td>
            <td colspan="3" height="24">
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
                <a href="./Embedded_Software/Feature_Tutorials/02-Debugging">Vitis Embedded Software Debugging Guide</a> 
            </td>
            <td>
                <a href="./Embedded_Software/Feature_Tutorials/03-vitis_classic_to_unified_migration">Migrating from classic Vitis IDE to Vitis Unified IDE  </a>            
            </td>
        </tr>  
        <tr>
            <td height="24"> </td>
            <td>
                <a href="./Embedded_Software/Feature_Tutorials/04-vitis_scripting_flows">Vitis Embedded Scripting Flow</a>            
            </td>            
            <td> 
                <a href="./Embedded_Software/Feature_Tutorials/05-vitis_version_control/">Vitis Version Control :new: </a>
            </td>
            <td>           
            </td>            
        </tr>  
    </tbody>
</table>

<table border="0" width="100%">
    <tbody>
        <tr>
            <td colspan="4" height="24" align="center">
                <strong> <a href="./Vitis_HLS/">Vitis HLS</a></strong>
            </td>
        </tr>
        <tr>
            <td colspan="2" height="48">
                Vitis High-Level Synthesis (HLS) lets you compile C/C++ code into RTL code. These tutorials offer a broader introduction to the Vitis HLS flows and use cases.
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
                <a href="./Vitis_HLS/Feature_Tutorials/02-Beamformer_Analysis">HLS Micro-Optimization Tutorial using Beamformer IP</a>
            </td>
            <td>
                <a href="./Vitis_HLS/Design_Tutorials/02-Beamformer">Adaptive Beamforming for Radar</a>
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
                <td height="24">
                    <a href="./Vitis_Platform_Creation/Design_Tutorials/04_Edge_VCK190_DFX">Versal Custom DFX Platform Creation Tutorial</a>
                </td>
                <td>
                  <a href="./Vitis_Platform_Creation/Feature_Tutorials/03_Vitis_Export_To_Vivado/">Vitis Export to Vivado Flow </a>
                </td>
            </tr>
            <tr>
                <td height="24"> </td>
                <td>
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
                    <a href="./Developer_Contributed/02-AIE_DSP_with_Makefile_and_GUI">DSP Design on AI Engine with GUI and Makefile Flows</a>
                </td>
                <td height="24">
                    <a href="./Developer_Contributed/03-HLS_Code_Optimization">Vitis HLS Optimization Techniques on Embedded Boards</a>
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


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
