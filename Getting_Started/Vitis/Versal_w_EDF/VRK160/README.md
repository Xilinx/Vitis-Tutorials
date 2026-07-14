<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Getting Started with Versal™ RF Series with Vitis™ Unified IDE</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

***Version: Vitis 2026.1 and Vivado 2026.1***

Welcome to Vitis Getting Started!

This tutorial showcases the important steps to build the AIE kernel dedicated for Versal RF Series architecture along with a Host application and running the design on the hardware and hardware emulation.

This tutorial uses pre-built vrk160_base.xpfm from Vitis installation path to compile AIE kernels.

The pre-built VRK160 platform has:

   - Base part: PS and PS-to-NoC-DDR connectivity

   - Extensible part: PL and AIE regions

The base part will serve as the foundation to generate the EDF WIC image. For more information about AMD EDF, please refer to the official [AMD Wiki page](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/3250585601/AMD+Embedded+Development+Framework+EDF#Introduction-to-the-AMD-Embedded-Development-Framework).  The extensible part will later be used to link with your custom kernels, ensuring your development flow is fully aligned with the AMD EDF methodology.

   >Note: This CED design based platforms enables segmented configuration by default. The PS-NoC-to-LPDDR is used to initialize the LPDDR memory and provide access to it during system bring-up. For more details about segmented configuration, please refer to [UG1273](https://docs.amd.com/r/en-US/ug1273-versal-acap-design/Segmented-Configuration).

 Then, you'll develop the AIE kernels and link them with the extensible XSA to produce a fixed XSA. Finally, you'll develop the acceleration application based on this fixed XSA.

Please go through the Makefile provided in this tutorial to better understand the Vitis tool flow.

This tutorial is aligned with the AMD Embedded Development Framework (EDF). For more information about AMD EDF, please refer to the official [AMD Wiki page](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/3250585601/AMD+Embedded+Development+Framework+EDF#Introduction-to-the-AMD-Embedded-Development-Framework)

To build this tutorial:
1. Source Vitis 2026.1:

   ```
   source <path_to_vitis_install>/settings64.sh
   ```
1. [Download](../README.md) the EDF Images.
2. Set up the sysroot

    ```
    source <path-to-design>/yocto_artifacts/amd-cortexa72-common_meta-edf-app-sdk/sdk.sh -d ./yocto_artifacts/ -y
    ```
3. To compile the binaries for HW EMU flow

    ```
    make all
    ```

4. To compile the binaries for HW only flow

    ```
    make sd_card
    ```
4. Run on VRK160 board

    - Boot the board using the QSPI BIN file downloaded in the previous step, following the instructions outlined in chapter `How to boot a board using the pre-built Images: OSPI Boot` in  [AMD WDF Wiki Page](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/3258155011/AMD+EDF+Getting+started+-+Discovery+and+Evaluation+AMD+Versal+device+portfolio#AMDEDFGettingstarted-DiscoveryandEvaluationAMDVersaldeviceportfolio-How-to-boot-a-board-using-the-pre-built-Images%3A-OSPI-Boot).
    - Program the `edf-platform-disk-image-amd-cortexa72-common.rootfs.wic.xz` to a SD card. Refer to the chapter of `Writing the EDF Linux® disk image (wic) to the secondary boot media : SD card ` in  [AMD WDF Wiki Page](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/3258155011/AMD+EDF+Getting+started+-+Discovery+and+Evaluation+AMD+Versal+device+portfolio#AMDEDFGettingstarted-DiscoveryandEvaluationAMDVersaldeviceportfolio-How-to-boot-a-board-using-the-pre-built-Images%3A-OSPI-Boot).

      > **NOTE:** Eject the SD card properly from the system after programming it.

    - Insert the SD card, and boot the VRK160 board with QSPI boot mode and power on.

    - Connect to UART console.

    - Launch the test application from UART console.

    <details>
    <summary><strong>Use the following steps to run the application</strong></summary>

     You will need to log in with user `amd-edf` first and set up a new password (it is then also the sudo password):

    - Log into the system

         ```bash
         amd-edf login:amd-edf
         You are required to change your password immediately (administrator enforced).
         New password:
         Retype new password:
         amd-edf:~$ sudo su
         We trust you have received the usual lecture from the local System
         Administrator. It usually boils down to these three things:
               #1) Respect the privacy of others.
               #2) Think before you type.
               #3) With great power comes great responsibility.
         Password:
         amd-edf:/home/amd-edf#
         ```

    - Use SCP to download the application and other files required to the current folder. Required files are listed as below:

        - Application: `host.exe`
        - DTBO: `gmio_async_xrt.dtbo`
        - XCLBIN: `gmio_async_xrt.xclbin`
        - PDI: `gmio_async_xrt.pdi`

       Use the scp command to transfer files to the current working directory. For example, to download the application file:

        ```
        amd-edf:/home/amd-edf# scp  <user_name>@<IP of host where IDE is running on>:<path to workspace>/host.exe .
        ```
        >Note: Using an SD card to copy the files to your board also works.

    - Run the application

        ```
        amd-edf:/home/amd-edf# ls
        amd-edf:/home/amd-edf# host.exe gmio_async_xrt.dtbo gmio_async_xrt.xclbin gmio_async_xrt.pdi
        amd-edf:/home/amd-edf# fpgautil -b gmio_async_xrt.pdi  -o gmio_async_xrt.dtbo
        amd-edf:/home/amd-edf# ./host.exe gmio_async_xrt.xclbin
        ```

   </details>

    - Expected print on UART console

    <details>
    <summary><b>Show Log</b></summary>

    ```
    amd-edf:/home/amd-edf# fpgautil -b gmio_async_xrt.pdi  -o gmio_async_xrt.dtbo
    amd-edf:/home/amd-edf# ./host.exe gmio_async_xrt.xclbin
    INFO: Load the pdi and dtbo using fpgautil
    [94715.900]Loading PDI from DDR
    [94718.686]Monolithic/Master Device
    [94729.654]18.282 ms: PDI initialization time
    [94735.607]+++Loading Image#: 0x0, Name: aie_image, Id: 0x18800000
    [94746.718]---Loading Partition#: 0x0, Id: 0x0
    [95247.524] 493.289 ms for Partition#: 0x0, Size: 22896 Bytes
    [95256.884]Subsystem PDI Load: Done
    Time taken to load BIN is 2656.000000 Milli Seconds
    BIN FILE loaded through FPGA manager successfully
    XAIEFAL: INFO: Resource group Avail is created.
    XAIEFAL: INFO: Resource group Static is created.
    XAIEFAL: INFO: Resource group Generic is created.
    input bo address800100000
    input bo memory group2
    input bo memory size100000
    output bo address800200000
    output bo memory group2
    output bo memory size100000
    mem cpy start
    mem cpy done
    before reset
    after reset
    run start
    Read Cycles: d1fec
    Throughput of the graph: 1523.84 MB/s
    Number of AIE cycles consumed in the  iteration is: d288c
    transfer competed
    TEST PASSED
    INFO: Embedded host run completed.
    ```
    </details>


<p class="sphinxhide" align="center"><sub>Copyright © 2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
