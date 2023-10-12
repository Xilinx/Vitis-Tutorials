<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Platform Creation Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    </td>
 </tr>
</table>

# Step 3: Run Application on the Platform

## Application 1: Run a PL Acceleration Application

You will create an acceleration application with PL kernel running on the platform created in step 2 and run it on this custom-VCK190 board.

1. Creating Vector Addition Application
   - Go to `WorkSpace` directory created in step1
   - Run Vitis by typing `vitis -w .` in the console. `-w` is to specify the workspace. `.` means the current workspace directory. Close the welcome tab after Vitis launches.
   - Go to example view by clicking the example button and click **Simple Vector Addition**.
      ![example.PNG](./images/step3/example.PNG)
   - Click **Create Application from Template**. The project creation wizard would pop up. 
      - Input the **System project name** as `vadd` and use the default location for **System project location**. Then click **Next**.
      - Select **custom_platform** platform, click **Next**.
         >Note If the platform component is not in the current workspace user could click **+** button to add the platform component.
      - Set **Kernel Image** to ```xilinx-versal-common-v2023.2/Image```.
      - Set **Root FS** to ```xilinx-versal-common-v2023.2/rootfs.ext4```.
      - Set **Sysroot** to ```xilinx-versal-common-v2023.1/sysroots/cortexa72-cortexa53-xilinx-linux```. Then click **Next**.
      - Review the summary of your `vadd` system project and click **Finish**.

      >Note: In the platform selecting page if the platform you want to use is not in the current list, you can click **+** button to add the platform to the list.

   After seconds the vadd system project, vadd host component and vadd kernel component will be ready in the component view. 

2. Building Vector Addition Application

   Vadd system project supports three targets: Software emulation, Hardware emulation and Hardware. User could select the target accordingly following steps below. 

   - Go to flow navigator and select vadd system project.

      ![build_application.PNG](./images/step3/build_application.PNG)

   - Click **Build All** under the target
   - In the Build Component setup dialog please enable all the components. Then click **OK**.

      ![build_application.PNG](./images/step3/build_component.PNG)

   After compilation is finished. Users can locate the `sd_card.img` by expanding the Output directory. This provides a logical view of the output. The actual path of `sd_card.img` file is located in `WrokSpace/vadd/build/<TARGET>/package/package/` directory. For example, if user targets hardware emulation, the `sd_card.img` file is in located in `WrokSpace/vadd/build/hw_emu/package/package/`. Users can also view the complete path of the `sd_card.img` file by simply hovering the mouse pointer over the image file.

3. Running Hardware Emulation 

   After the compilation for hardware emulation is finished, please follow steps below to run hardware emulation.
   - Click **Start Emulator** under **HARDWARE EMULATION** target. The Start Emulator setup page would be popped up. Then click **Start**.
      ![build_application.PNG](./images/step3/start_emulator.PNG)
   - Click **Run** after the emulator boots up to terminal
      ![build_application.PNG](./images/step3/emulation_result.PNG)
   - Click **Stop emulator** to stop emulation after emulation is finished.
      ![build_application.PNG](./images/step3/stop_emulator.PNG)
   

4. Running the Application on Hardware

   1. Copy `WrokSpace/vadd/build/hw/package/package/sd_card.img` to local if you build the project on a remote server or virtual machine.

   2. Program the `sd_card.img` to a SD card. Refer to [AR#73711](https://www.xilinx.com/support/answers/73711.html) for detailed steps.

      > **NOTE:** The programmed SD card has two partitions. The FAT32 partition with boot components and the EXT4 partition with Linux root file system. By default, the Windows system cannot see the contents of EXT4 partition.

      > **NOTE:** Eject the SD card properly from the system after programming it.

   3. Insert the SD card, and boot the VCK190 board with SD boot mode (SW1[4:1] = "1110": OFF, OFF, OFF, ON) and power on.

      > **NOTE:** Refer to [VCK190 Evaluation Board User Guide](https://www.xilinx.com/support/documentation/boards_and_kits/vck190/ug1366-vck190-eval-bd.pdf) for details about boot mode.

   4. Connect to UART console.

   5. Launch the test application from UART console.

   <details>
   <summary><strong>Use the following steps to run the application</strong></summary>

     You will need to log in with user `petalinux` first and set up a new password (it is then also the sudo password):

     - Log into the system

         ```bash
         petalinux login:petalinux
         You are required to change your password immediately (administrator enforced).
         New password:
         Retype new password:
         petalinux:~$ sudo su
         We trust you have received the usual lecture from the local System
         Administrator. It usually boils down to these three things:
               #1) Respect the privacy of others.
               #2) Think before you type.
               #3) With great power comes great responsibility.
         Password:
         petalinux:/home/petalinux#
         ```

     - Go to auto mounted FAT32 partition and run the application as:

         ```
         petalinux:/home/petalinux# cd /run/media/sd-mmcblk0p1
         petalinux:/home/petalinux# ./simple_vadd krnl_vadd.xclbin
         ```
   </details>  

   6. Expected print on UART console

   <details>
   <summary><b>Show Log</b></summary>

   ```
   root@petalinux:/run/media/mmcblk0p1# ./vadd binary_container_1.xclbin
   EXE: /run/media/mmcblk0p1/simple_vadd
   [XRT] WARNING: The xrt.ini flag "opencl_summary" is deprecated and will be removed in a future release.  A summary file is generated when when any profiling is enabled, so please use the appropriate settings from "opencl_trace=true", "device_counter=true", and "device_trace=true."
   [XRT] WARNING: The xrt.ini flag "opencl_device_counter" is deprecated and will be removed in a future release.  Please use the equivalent flag "device_counter."
   INFO: Reading krnl_vadd.xclbin
   Loading: 'krnl_vadd.xclbin'
   [   74.394840] zocl-drm amba_pl@0:zyxclmm_drm: zocl_create_client: created KDS client for pid(577), ret: 0
   [   74.395731] zocl-drm amba_pl@0:zyxclmm_drm: zocl_destroy_client: client exits pid(577)
   [   74.401000] zocl-drm amba_pl@0:zyxclmm_drm: zocl_create_client: created KDS client for pid(577), ret: 0
   Trying to program device[0]: edge
   [   74.937477] [drm] skip kind 29(AIE_RESOURCES) return code: -22
   [   74.938038] [drm] found kind 8(IP_LAYOUT)
   [   74.938641] [drm] found kind 9(DEBUG_IP_LAYOUT)
   [   74.939375] [drm] skip kind 25(AIE_METADATA) return code: -22
   [   74.939688] [drm] found kind 7(CONNECTIVITY)
   [   74.940087] [drm] found kind 6(MEM_TOPOLOGY)
   [   74.940907] [drm] Memory 0 is not reserved in device tree. Will allocate memory from CMA
   [   74.948647] [drm] Memory 1 is not reserved in device tree. Will allocate memory from CMA
   [   74.963753] cu_drv CU.2.auto: cu_probe: CU[0] created
   [   74.974174] cu_drv CU.2.auto:  ffff000803cbac10 xrt_cu_intr_thread: CU[0] start
   [   74.989334] [drm] zocl_xclbin_read_axlf f4f049d5-183a-e265-264d-ecfa34a51343 ret: 0
   [   75.037345] [drm] bitstream f4f049d5-183a-e265-264d-ecfa34a51343 locked, ref=1
   [   75.038188] zocl-drm amba_pl@0:zyxclmm_drm:  ffff000800323c10 kds_add_context: Client pid(577) add context CU(0xffffffff) shared(true)
   [   75.041054] zocl-drm amba_pl@0:zyxclmm_drm:  ffff000800323c10 kds_del_context: Client pid(577) del context CU(0xffffffff)
   [   75.042096] [drm] bitstream f4f049d5-183a-e265-264d-ecfa34a51343 unlocked, ref=0
   [   75.094803] [drm] bitstream f4f049d5-183a-e265-264d-ecfa34a51343 locked, ref=1
   [   75.095393] zocl-drm amba_pl@0:zyxclmm_drm:  ffff000800323c10 kds_add_context: Client pid(577) add context CU(0xffffffff) shared(true)
   Device[0]: program successful!
   [   75.188269] zocl-drm amba_pl@0:zyxclmm_drm:  ffff000800323c10 kds_add_context: Client pid(577) add context CU(0xffffffff) shared(true)
   [   75.192218] zocl-drm amba_pl@0:zyxclmm_drm:  ffff000800323c10 kds_add_context: Client pid(577) add context CU(0x0) shared(true)
   TEST PASSED
   [   78.347703] zocl-drm amba_pl@0:zyxclmm_drm:  ffff000800323c10 kds_del_context: Client pid(577) del context CU(0xffffffff)
   [   78.349191] zocl-drm amba_pl@0:zyxclmm_drm:  ffff000800323c10 kds_del_context: Client pid(577) del context CU(0x0)
   [   78.354584] zocl-drm amba_pl@0:zyxclmm_drm:  ffff000800323c10 kds_del_context: Client pid(577) del context CU(0xffffffff)
   [   78.355356] [drm] bitstream f4f049d5-183a-e265-264d-ecfa34a51343 unlocked, ref=0
   [   78.462912] zocl-drm amba_pl@0:zyxclmm_drm: zocl_destroy_client: client exits pid(577)
   ```

   </details>

   >**NOTE**: The `[   35.116279] [drm]` messages are print from XRT for debugging purpose. It only shows up on UART. It will now print on ssh. This debugging message can be turned off by turning down the system dmesg reporting level.

   ```
   echo "4" > /proc/sys/kernel/printk
   ```


## Application 2: Run an AIE + PL Acceleration Application Project

To verify the platform functionality, you will create a project with AIE + PL kernel and PS application, and run it on this custom-VCK190 board.

1. Creating AIE + PL Application
   - Go to `WorkSpace` directory created in step1
   - Run Vitis by typing `vitis -w .` in the console. `-w` is to specify the workspace. `.` means the current workspace directory. Close the welcome tab after Vitis launches.
   - Go to example view by clicking the example button and click **AI Engine, PL and PS System Design**.
      ![example.PNG](./images/step3/example.PNG)
   - Click **Create Application from Template**. The project creation wizard would pop up. 
      - Use the default setting for **System project name** and **System project location**. Then click **Next**.
      - Select **custom_platform** platform, click **Next**.
         >Note If the platform component is not in the current workspace user could click **+** button to add the platform component.
      - Set **Kernel Image** to ```xilinx-versal-common-v2023.2/Image```.
      - Set **Root FS** to ```xilinx-versal-common-v2023.2/rootfs.ext4```.
      - Set **Sysroot** to ```xilinx-versal-common-v2023.1/sysroots/cortexa72-cortexa53-xilinx-linux```. Then click **Next**.
      - Review the summary of your `vadd` system project and click **Finish**.

      >Note: In the platform selecting page if the platform you want to use is not in the current list, you can click **+** button to add the platform to the list.

   After seconds the vadd system project, vadd host component and vadd kernel component will be ready in the component view. 

2. Building AIE + PL Application

   Vadd system project supports three targets: Software emulation, Hardware emulation and Hardware. User could select the target accordingly following steps below. 

   - Go to flow navigator and select vadd system project.

      ![build_application.PNG](./images/step3/build_application.PNG)

   - Click **Build All** under the target
   - In the Build Component setup dialog please enable all the components. Then click **OK**.

      ![build_application.PNG](./images/step3/build_component.PNG)

   After compilation is finished. Users can locate the `sd_card.img` by expanding the Output directory. This provides a logical view of the output. The actual path of `sd_card.img` file is located in `WrokSpace/aie_sys_design/build/<TARGET>/package/package/` directory. For example, if user targets hardware emulation, the `sd_card.img` file is in located in `WrokSpace/aie_sys_design/build/hw_emu/package/package/`. Users can also view the complete path of the `sd_card.img` file by simply hovering the mouse pointer over the image file.

3. Running Hardware Emulation 

   After the compilation for hardware emulation is finished, please follow steps below to run hardware emulation.

   - Click **Start Emulator** under **HARDWARE EMULATION** target. The Start Emulator setup page would be popped up. Then click **Start**.
      ![build_application.PNG](./images/step3/start_emulator.PNG)
   - Click **Run** after the emulator boots up to terminal
      ![build_application.PNG](./images/step3/aie_emulation.PNG)
   - Click **Stop emulator** to stop emulation after emulation is finished.
      ![build_application.PNG](./images/step3/stop_emulator.PNG)      
   
   **What Just Happened?**

   Vitis runs PS emulation in QEMU, PL and AIE emulation in XSIM. They can communicate with each other. When running the emulation, Vitis downloads executable and xclbin to Remote Working Directory and launch it.

4. Running the Application on Hardware
   After the compilation for hardware is finished, please follow steps below to run on hardware.

   1. Copy the `sd_card.img` from `WrokSpace/aie_sys_design/build/hw/package/package/` directory to local, if the project is run on a server or virtual machine.

      > **NOTE:** Vitis will generate images with aie_debug enabled and disabled. The image with aie_debug disabled will run freely; the image with aie_debug enabled will halt AI Engine and wait for the debugger to connect to it.

   2. Program `sd_card.img` to the SD card. Refer to [AR#73711](https://www.xilinx.com/support/answers/73711.html) for detailed steps.

      > **NOTE:** The programmed SD card has two partitions: the FAT32 partition with boot components and the EXT4 partition with Linux root file system. By default, the Windows system cannot see the contents of EXT4 partition.

      > **NOTE:** Eject the SD card properly from the system after programming it.

   3. Insert the SD card, and boot the VCK190 board with SD boot mode (SW1[4:1] = "1110": OFF, OFF, OFF, ON) and power on.

      > **NOTE:** Refer to [VCK190 Evaluation Board User Guide](https://www.xilinx.com/support/documentation/boards_and_kits/vck190/ug1366-vck190-eval-bd.pdf) for details about boot mode.

   4. Launch test application from the UART console.

      <details>
      <summary><strong>Use the following steps to run the application</strong></summary>

      You will need to login with user `petalinux` first, and set up a new password (it is then also the sudo password):

      - Log into the system.

            ```bash
            petalinux login:petalinux
            You are required to change your password immediately (administrator enforced).
            New password:
            Retype new password:
            petalinux:~$ sudo su
            We trust you have received the usual lecture from the local System
            Administrator. It usually boils down to these three things:
                  #1) Respect the privacy of others.
                  #2) Think before you type.
                  #3) With great power comes great responsibility.
            Password:
            petalinux:/home/petalinux#
            ```

      - Go to the auto mounted FAT32 partition and run the application :

            ```
            petalinux:/home/petalinux# cd /run/media/sd-mmcblk0p1
            petalinux:/home/petalinux# ./plaie binary_container_1.xclbin
            ```
      </details> 

   5. Expected print on UART console.

   <details>
   <summary><b>Show Log</b></summary>

      ```
      root@petalinux:/mnt/sd-mmcblk0p1# ./plaie binary_container_1.xclbin
      Initializing ADF API...
      [   39.786629] zocl-drm axi:zyxclmm_drm: zocl_create_client: created KDS client for pid(697), ret: 0
      [   39.795569] zocl-drm axi:zyxclmm_drm: zocl_destroy_client: client exits pid(697)
      [   39.813998] zocl-drm axi:zyxclmm_drm: zocl_create_client: created KDS client for pid(697), ret: 0
      [   40.245746] [drm] found kind 29(AIE_RESOURCES)
      [   40.245765] [drm] found kind 8(IP_LAYOUT)
      [   40.250535] [drm] found kind 9(DEBUG_IP_LAYOUT)
      [   40.254738] [drm] found kind 25(AIE_METADATA)
      [   40.259429] [drm] found kind 7(CONNECTIVITY)
      [   40.263937] [drm] found kind 6(MEM_TOPOLOGY)
      [   40.268417] [drm] Memory 0 is not reserved in device tree. Will allocate memory from CMA
      [   40.280809] [drm] Memory 1 is not reserved in device tree. Will allocate memory from CMA
      [   40.296964] cu_drv CU.2.auto: cu_probe: CU[0] created
      [   40.302294] cu_drv CU.3.auto: cu_probe: CU[1] created
      [   40.307630] cu_drv CU.4.auto: cu_probe: CU[2] created
      [   40.312842] cu_drv CU.2.auto:  ffff000804e4ac10 xrt_cu_intr_thread: CU[0] start
      [   40.320432] cu_drv CU.3.auto:  ffff00080271e810 xrt_cu_intr_thread: CU[1] start
      [   40.321470] [drm] zocl_xclbin_read_axlf 8e1f87ed-725e-07da-c6b3-6786f6df2f1f ret: 0
      [   40.327753] cu_drv CU.4.auto:  ffff000802696410 xrt_cu_intr_thread: CU[2] start
      XAIEFAL: INFO: Resource group Avail is created.
      XAIEFAL: INFO: Resource group Static is created.
      XAIEFAL: INFO: Resource group Generic is created.
      [   40.390527] [drm] bitstream 8e1f87ed-725e-07da-c6b3-6786f6df2f1f locked, ref=1
      [   40.390547] zocl-drm axi:zyxclmm_drm:  ffff00080085d410 kds_add_context: Client pid(697) add context Domain(0) CU(0xffffffff) shared(true)
      [   40.410331] zocl-drm axi:zyxclmm_drm:  ffff00080085d410 kds_del_context: Client pid(697) del context Domain(0) CU(0xffffffff)
      Input memory virtual addr 0x0xffffb91ea000x
      Input memory virtual addr 0x0xffffb91e9000x
      in_bohdl0 in_bohdl1 sync[   40.421650] [drm] bitstream 8e1f87ed-725e-07da-c6b3-6786f6df2f1f unlocked, ref=0
      started
      in_bohdl0 in_bohdl1 sy[   40.429988] [drm] bitstream 8e1f87ed-725e-07da-c6b3-6786f6df2f1f locked, ref=1
      nc done
      Output memory virtual a[   40.439461] zocl-drm axi:zyxclmm_drm:  ffff00080085d410 kds_add_context: Client pid(697) add context Domain(0) CU(0xffffffff) shared(true)
      ddr 0x0xffffb91e8000x
      [   40.464816] zocl-drm axi:zyxclmm_drm:  ffff00080085d410 kds_add_context: Client pid(697) add context Domain(0) CU(0x1) shared(true)
      run pl_mm2s_1
      [   40.478710] zocl-drm axi:zyxclmm_drm:  ffff00080085d410 kds_add_context: Client pid(697) add context Domain(0) CU(0x0) shared(true)
      run pl_mm2s_2
      [   40.492018] zocl-drm axi:zyxclmm_drm:  ffff00080085d410 kds_add_context: Client pid(697) add context Domain(0) CU(0x2) shared(true)
      run pl_s2mm
      graph init. This does nothing be[   40.506280] zocl-drm axi:zyxclmm_drm:  ffff00080085d410 kds_del_context: Client pid(697) del context Domain(0) CU(0x1)
      cause CDO in boot PDI already con[   40.519845] zocl-drm axi:zyxclmm_drm:  ffff00080085d410 kds_del_context: Client pid(697) del context Domain(0) CU(0x0)
      figures AIE.
      graph run
      graph end[   40.533494] zocl-drm axi:zyxclmm_drm:  ffff00080085d410 kds_del_context: Client pid(697) del context Domain(0) CU(0xffffffff)

      mm2s_1 completed with status(4[   40.547611] zocl-drm axi:zyxclmm_drm:  ffff00080085d410 kds_del_context: Client pid(697) del context Domain(0) CU(0x2)
      )
      mm2s_2 completed with status(4)
      s2mm completed with status(4)
      TEST PASSED
      Releasing host Buffers...
      Releasing remaining XRT objects...
      [   40.561044] [drm] bitstream 8e1f87ed-725e-07da-c6b3-6786f6df2f1f unlocked, ref=0
      [   40.578375] zocl-drm axi:zyxclmm_drm: zocl_destroy_client: client exits pid(697)
      ```
   </details>






Congratulations now you've created a custom platform that PL acceleration kernel and AIE applications can work on it.

## Next Steps

If user need to do iteration for your project, you could go through the [Iteration Guidelines](./Iteration_guideline.md) to do iterations.


[In next chapter](./faq.md), we'll list some frequently encountered problems and frequently asked questions. Please take a look if you'd like to understand more of platform creation.


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>

