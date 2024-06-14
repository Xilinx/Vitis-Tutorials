# Vitis-Tutorials ChangeLog

## 2024.1 (Jun. 14th, 2024)
### New Tutorials 
- AI_Engine_Development/AIE/Design_Tutorials/16-1M-Point-FFT-32Gsps/
- AI_Engine_Development/AIE/Design_Tutorials/17-Hough-Transform/
- AI_Engine_Development/AIE/Feature_Tutorials/22-Performance-Validation-in-Analysis-View/ 
- AI_Engine_Development/AIE-ML/Feature_Tutorials/22-Performance-Validation-in-Analysis-View/ 

### Tutorial Updates
- The tutorials under Hardware_Acceleration category are in "regression" mode, meaning we will run regression tests on 2024.1 and newer versions, but will not make any feature updates other than bug fixes.
- Update tutorials to use the Vitis Unified CLI
- Provide equivalent python scripts for GUI-based tutorials

<details open>
    <summary>2023.2 Release</summary>

## 2023.2_update (Mar. 22, 2024)
### New Tutorials
- AI_Engine_Development/AIE/Design_Tutorials/14-Bitonic-Sorting/
- AI_Engine_Development/AIE/Design_Tutorials/15-farrow_filter/ 

## 2023.2_update (Feb. 19, 2024)
### New Tutorials
- AI_Engine_Development/AIE-ML/Design_Tutorials/03-AIE-ML-lenet_tutorial/
- AI_Engine_Development/AIE/Design_Tutorials/13-FFT-DFT-on-AIE/
- AI_Engine_Development/AIE/Feature_Tutorials/21-two_tone_filter/ 

## 2023.2_update (Jan. 31, 2024)
### New Tutorials
- Vitis_HLS/Design_Tutorials/02-Beamformer/
- Vitis_HLS/Feature_Tutorials/01-using_code_analyzer/
- Vitis_HLS/Feature_Tutorials/02-Beamformer_Analysis/
- AI_Engine_Development/AIE-ML/Design_Tutorials/02-Prime-Factor-FFT/
- AI_Engine_Development/AIE/Design_Tutorials/12-IFFT64K-2D/
- Embedded_Software/Feature_Tutorials/02-Debugging/

## 2023.2_update (Jan. 11, 2024)
### New Tutorials
- Embedded_Software/Feature_Tutorials/01-user_managed_mode/
- Embedded_Software/Feature_Tutorials/03-vitis_classic_to_unified_migration/
- Embedded_Software/Feature_Tutorials/04-vitis_scripting_flows/

## 2023.2 (Nov. 1st, 2023)
### New Tutorials 
- AI_Engine_Development/AIE-ML/Design_Tutorials/01-AIE-ML-programming-and-optimization/
- AI_Engine_Development/AIE/Feature_Tutorials/20-aiecompiler-features/ 
- Embedded_Software/Getting_Started/

### Tutorial Updates
- Update existing AIE feature tutorials to work with AIE-ML
    - AI_Engine_Development/AIE-ML/Feature_Tutorials/02-using-gmio/
    - AI_Engine_Development/AIE-ML/Feature_Tutorials/03-rtp-reconfiguration/
    - AI_Engine_Development/AIE-ML/Feature_Tutorials/04-packet-switching/
    - AI_Engine_Development/AIE-ML/Feature_Tutorials/05-AI-engine-versal-integration/
    - AI_Engine_Development/AIE-ML/Feature_Tutorials/20-aiecompiler-features/
- Tutorials that contain GUI design flow have been updated to work with the Unified IDE, which has become the default GUI in Vitis 2023.2 
- Updated tutorials to use the Vitis Unified CLI

### EoU Improvements
- Add an additional level under AI_Engine_Development to host tutorials targeting AIE and AIE-ML.
    - AI_Engine_Development/AIE
    - AI_Engine_Development/AIE-ML
</details>

<details close>
    <summary>2023.1 Release</summary>

## 2023.1_update (Sep. 21th, 2023)
### New Tutorials 
- AI_Engine_Development/Design_Tutorials/05-Prime-Factor-FFT/
- AI_Engine_Development/Design_Tutorials/09-ddc_chain/
- AI_Engine_Development/Design_Tutorials/11-Bilinear_Interpolation/

## 2023.1_update (Jul. 27th, 2023)
### New Tutorials 
- Vitis_Platform_Creation/Feature_Tutorials/04_platform_validation/
### Tutorial Updates
- Revamping the AI Engine debug walkthrough tutorial by using a smaller design for fast iteration and adding more features. 
    - AI_Engine_Development/Feature_Tutorials/09-debug-walkthrough/
### EoU Improvements
- Rewrite the README pages to help users locate tutorials and get help
    - AI_Engine_Development/README
    - Vitis_Platform_Creation/README

## 2023.1_update (Jun. 12th, 2023)
### New Tutorials 
- Vitis_Platform_Creation/Feature_Tutorials/03_Vitis_Export_To_Vivado/

### Tutorial Updates
- Provided an updated description and screenshots to help users leverage the new Vitis Unified IDE for design development.
    - Getting_Started/Vitis_HLS/
    - AI_Engine_Development/Feature_Tutorials/05-AI-engine-versal-integration/

## 2023.1 (May 24th, 2023)
### New Tutorials 
- AI_Engine_Development/Design_Tutorials/04-Polyphase-Channelizer/

### Tutorial Updates
- Updated the description and included screenshots to guide users on analyzing design performance using the Analysis view in Vitis Unified IDE. This replaces the deprecated Vitis Analyzer.
    - AI_Engine_Development/Feature_Tutorials/13-aie-performance-analysis/
    - AI_Engine_Development/Feature_Tutorials/14-implementing-iir-filter/
    - Hardware_Acceleration/Design_Tutorials/01-convolution-tutorial/
    - Hardware_Acceleration/Design_Tutorials/02-bloom/
    - Hardware_Acceleration/Design_Tutorials/06-cholesky-accel/
    - Hardware_Acceleration/Design_Tutorials/07-host-code-opt/
    - Hardware_Acceleration/Feature_Tutorials/02-mixing-c-rtl-kernels/
    - Hardware_Acceleration/Feature_Tutorials/04-mult-ddr-banks/
- Provided an updated description and screenshots to help users leverage the new Vitis Unified IDE for design development.
    - Getting_Started/Vitis_Platform/
- Update license to MIT, as well as legal attribution in code. 

### EoU Improvements
- Some of the tutorials have been restructured to improve user experience.
</details>

<details close>
    <summary>2022.2 Release</summary>

## 2022.2_update (Mar 1st, 2023)
### New Tutorials 
- Developer_Contributed/02-AIE_DSP_with_Makefile_and_GUI/
- Developer_Contributed/03-HLS_Code_Optimization/   
    
## 2022.2 (Nov 2nd, 2022)
### New Tutorials 
- Vitis_Platform_Creation/Design_Tutorials/04_Edge_VCK190_DFX/ 
- AI_Engine_Development/Design_Tutorials/10-GeMM_AIEvsDSP/
- AI_Engine_Development/Feature_Tutorials/19-aie_external_io_sv/

### Tutorial Updates
- Hardware_Acceleration/Design_Tutorials/10-get_moving_with_alveo/
    - moved from Hardware_Acceleration/Introduction/

### EoU Improvements
- New look of the landing page and a new FAQs page of Vitis-Tutorials. 
- New Introduction page of Vitis_Platform_Creation and Hardware_Acceleration to make searching for tutorials with specific flows easier and help users choose the proper tutorials for their needs.
- Some of the tutorials have been restructured to improve user experience.

</details>

<details close>
    <summary>2022.1 Release</summary>

## 2022.1_update (Sep 15th, 2022)
### New Tutorials 
- AI_Engine_Development/Feature_Tutorials/18-aie_a_to_z_custom_linux_platform/
- Hardware_Acceleration/Design_Tutorials/09-sssp-application/
- Hardware_Acceleration/Feature_Tutorials/10-p2p-card-to-card/
### Tutorial Updates
- AI_Engine_Development/Feature_Tutorials/17-RTL-IP-with-AIE-Engines/
   - moved from AI_Engine_Development/Design_Tutorials/04-custom-platform-emulation

## 2022.1 (May 6th, 2022)
### EoU Improvements
- New Introduction page of AI Engine Development category to make searching for tutorials with specific flows easier and help users choose the proper tutorials for their needs.
- New CHANGELOG page of the Vitis-Tutorials repository. Revision history information has also been added to several tutorials. 
- Some of the tutorials on platform creation and AIE development have been restructured to improve user experience.
- Add Vitis IDE support for some of the AIE feature tutorials, the supported tutorials can be downloaded as an example template to be added to a project and running in Vitis IDE.
### New Tutorials 
- Getting_Started/Vitis/Vitis_Platform/
- Vitis_Platform_Creation/Feature_Tutorials/02_platform_creation_petalinux_component/	
- AI_Engine_Development/Feature_Tutorials/16-external-traffic-generator-aie/
- Hardware_Acceleration/Design_Tutorials/08-alveo_aurora_kernel/
- Hardware_Acceleration/Feature_Tutorials/09-using-ethernet-on-alveo/
### Tutorial Updates
- Developer_Contributed/01-Versal_Custom_Thin_Platform_Extensible_System/  
    - Add lpddr4 to the platform; Add Yocto support; Add HW_Emu support. 
- Hardware_Acceleration/Design_Tutorials/03-rtl_stream_kernel_integration/ 
    - Introduce XRT Native API to replace OpenCL API.
- AI_Engine_Development/Feature_Tutorials/14-implementing-iir-filter/
    - Add part II 

</details>



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
