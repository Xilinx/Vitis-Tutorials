# Debugging Linux Applications

***Version: Vitis 2023.2***

The Vitis IDE provides a familiar environment for debugging Linux applications for a target platform. This section covers the following Linux application debug features:

- Setting breakpoints
- Stepping through program execution
- Viewing program variables, memory contents, and the stack 

The following diagram illustrates the general Linux application debug flow. The Vitis Debugger an integrated debugger supporting Zynq®-7000 SoC, Zynq UltraScale+ MPSoC, and MicroBlaze™ and Versal™ processors.

![](images/Linux_debug_flow.png)


**Note:** Refer to the [Appendix](#appendix) section for supplementary information about creating a hardware design and building software boot images.  

## Limitations

Xen aware and OS aware debugging is not supported in Vitis Unified IDE 2023.2. If users require this feature then it is recommended that Vitis Classic 2023.2 is used.

## Getting Started

Linux debug setup is a mandatory step. Setup allows you to create platform and application projects in the Vitis IDE.

1. Open a terminal and navigate to your desired work directory.
2. Copy **scripts/petalinux_build.sh** to your desired working directory; for example, `linux_debug/petalinux_prj`.
3. Source the PetaLinux tool.
4. Follow the steps in the [PetaLinux Build](#petalinux-build) section to complete the PetaLinux build and configuration manually.
1. 
   **Note:** You can skip the manual steps by using the automated `petalinux_build.sh` script to generate Linux images. However, make sure to select the appropriate configuration whenever prompted.

5. Navigate to `zcu102` in your Vivado project and copy `mpsoc_preset_wrapper.xsa` to your desired Vitis work directory (in the following example, it is `vitis_work_dir`) as `zcu102.xsa`.

    ```
    $ cd zcu102/mpsoc_preset_wrapper.xsa
    $ cp mpsoc_preset_wrapper.xsa vitis_work_dir/zcu102.xsa
    ```

    **Note:** If you create a custom ZCU102 XSA by following the steps in [Vivado Design](#vivado-design), you can use it as it is here.

6. Create the sysroot

    ```
    $ cd xilinx-zcu102-2023.2/images/linux
    $ ./sdk.sh
    ```
**Note:** When prompted for the `Enter target directory for SDK`, enter the path to `vitis_work_dir`

7. Copy **scripts/unified_workspace.py** to your Vitis work directory (in this example, `vitis_work_dir`) as you did for `zcu102.xsa`. Copy **src/linux_test_application.c** to `vitis_work_dir/src`. These files are required to set up the Vitis workspace. Your Vitis work directory should be structured like the example below after downloading these files.

    ```
    vitis_work_dir
    |
    |_ zcu102.xsa
    |
    |_ unified_workspace.py
    |
    |_ sysroot
    |  |
    |  |_ cortexa72-cortexa53-xilinx-linux
    |  |
    |  |_ x86_64-petalinux-linux
    |
    |_ src
      |
      |_ linux_test_application.c

    ```

8. Run the `Vitis-workspace.py` script for workspace setup in Vitis Unified CommandLine Interface (CLI). After successful execution of the script, a Vitis worskpace named `linux_debug_ws` will be created with a ZCU102 based platform and a Linux test application.

    ```
    vitis_work_dir
    |
    |_ zcu102.xsa
    |
    |_ vitis-workspace.py
    |
    |_ src
    |   |
    |   |_ linux_test_application.c
    |
    |_ linux_debug_ws

    ```

9. Launch the Vitis IDE **linux_debug_ws** workspace. 


## Error 1: Failed in Validating the test connection


1. In the Vitis workspace under `FLOW`, select the `Build` and click the `Open Settings` icon to open the `Launch Configurations`.
2. Select `Debug` icon


   ![](images/TCF-connection.PNG)

### Setting Up the Ethernet Connection

Select the settings based on your connection

## Connecting to Physical Board


- Set your host machine's Ethernet address to a static IP (suggested: 192.168.1.10).
- Using the serial terminal, configure the board's IP address to 192.168.1.11:

  ```
  ifconfig eth0 192.168.1.11
  ```
  **Note:** This can be any address other than the one that the host is configured to on the same Subnet LAN.

- Ping the host machine using the terminal to verify connectivity.

  ```
  ping 192.168.1.10 –c 1
  ```
## Connecting to QEMU

Petalinux supports the Quick EMUlation (QEMU) emulator. This is useful for users how dont have access to Physical board. Users can use the steps below to boot Linux on the QEMU. Users can pass `qemu-args` to pass the network information. The hardware used in this tutorial is for a ZCU102 which has the GEM enabled at GEM3. This command will look slightly different GEM is used.

The hostfwd definitions along with user network arguments i.e  `-net nic -net nic -net nic -net nic,vlan=1 -net user,vlan=1) hostfwd=tcp:<host address>:<host port used for forwarding>-<guest address>:<guest port>`

```
petalinux-boot --qemu --kernel --qemu-args "-net nic -net nic -net nic -net nic,netdev=gem3 -netdev user,id=gem3,hostfwd=tcp:127.0.0.1:1540-10.0.2.15:1534"
```

**Note** Use Ctrl A + x to exit the QEMU

### Setting Up the TCF Agent


## Physical Board Linux Agent

- **Vitis -> Target Connections**.
- Right click on **Linux TCF Agent** option and select **New Target**.
- Change the host IP address to localhost or 127.0.0.1. 
- change the Port to 1540
- Click **OK**.

![](images/Linux-Agent.PNG)

## QEMU Linux Agent

- **Vitis -> Target Connections**.
- Right click on **Linux TCF Agent** option and select **New Target**.
- Change the host IP address to 192.168.1.11. 
- Click **OK**.

![](images/Linux-Agent-QEMU.PNG)

## Error 2: Multithread Application Debug

Enable the `_ERROR5_` macro in the application source file `linux_test_application.c` and disable `_ERROR3_` and `_ERROR6_` as shown below.

```
//#define _ERROR3_
#define _ERROR5_
//#define _ERROR6_
```

The goal of multithread application debugging is to make the application thread-safe. You need to debug the application and identify the locations to protect read/write operations.

2. Under `FLOW`, highlight the `linux_test_app`, and Build.
3. Under `FLOW`, highlight the `linux_test_app`, and Debug to launch a Launch Configuration. 
4. Select, the Target Connection; either `physical_zcu102`, or `qemu_zcu102` and the Work Directory to /home/petalinux`

![](images/launch_config.PNG)

4. The code will stop at the program entry. There are various debugging techniques such as breakpoints, stepping, and so on to identify the data that needs to be protected. This example will use the breakpoint inside thread function.
5. Put a breakpoint at below two lines.

    ```
    printf("\n Job %d started\n", counter);
    .......
    .......
    printf("\n Job %d finished\n", counter);
    ```
5. Click **Resume** and check the Job ID printed in the console. Here, the job to be done is not thread-safe.
6. Open the **Variables** view to track the counter variable value.
7. Use a synchronization mechanism (mutex, for example) to ensure that two or more concurrent threads do not simultaneously execute read/writes.

8. Uncomment the mutex code lines listed below from the application to make it thread-safe.

    ```
    ............................
    ............................

    #ifdef _ERROR5_
    void* doSomeThing(void *arg)
    {
      //  pthread_mutex_lock(&lock);
      .................
      .................
      // pthread_mutex_unlock(&lock);
      .................
    }
    #endif
    ............................
    ............................
    #ifdef _ERROR5_
    /* if (pthread_mutex_init(&lock, NULL) != 0)
        {
            printf("\n mutex init failed\n");
            return 1;
        }
    */
    ...............
    ...............
    ...............
    //    pthread_mutex_destroy(&lock);
    ```
9. Rebuild the application.
10. Relaunch the `Launch Configuration`
11. Observe the output.


## Error 3 : Dynamic Memory Allocation

Enable the `_ERROR6_` macro in the application source file `linux_test_application.c` and disable `_ERROR3_` and `_ERROR5_` as shown below.

```
//#define _ERROR3_
//#define _ERROR5_
#define _ERROR6_
```
The goal of dynamic memory allocation debugging is to understand memory management.

1. Under `FLOW`, highlight the `linux_test_app`, and Build.
2. Under `FLOW`, highlight the `linux_test_app`, and Debug to launch a Launch Configuration. 
3. Select, the Target Connection; either `physical_zcu102`, or `qemu_zcu102` and the Work Directory to /home/petalinux`

![](images/launch_config.PNG)

4. The code will stop at the program entry.
5. Use debugging techniques such as the Variables view, step over, breakpoints, and so on to identify the incorrect memory assignment.
6. Place a breakpoint at the following lines.

    ```
    *ptr = i + 1;
    ptr++;
    ```         
7. Click **Resume** and let the tool stop at the breakpoint.
8. Open **Variables** view to check the **ptr** value. Observe that a value is not correctly assigned to a pointer.
9. Comment out the following code lines:

    ```
    #ifdef _ERROR6_
    ................
    ................
    ................
    //ptr = i + 1;
    //ptr++;
    ................
    ```

10. Uncomment the following line of code to make the appropriate pointer assignment.

    ```
    #ifdef _ERROR6_
    ................
    ................
    ................
    ................
    ptr[i] = i + 1;
    ................
    ```

11. Rebuild the application.
12. Relaunch the `Launch Configuration`
13. Observe the output.

## Appendix

[Vivado Design](#Vivado-Design) explains how to use a Vivado example design to create a Vivado project. This set of steps gives you the XSA file from Vivado to export. You can skip the manual steps and use this Tcl script to create an XSA directly.

[PetaLinux Build](#petalinux-build) explains how to create a PetaLinux project, and how to configure and build boot images. 

### Vivado Design

**Note**: You can skip these steps by running the [automation steps](#automation-steps) instead. 

#### Creating the Block Design

The first step is to create a block design. You can create this according to your specific requirements, but here you will create a Zynq UltraScale+ MPSoC based example project using a template, as explained below.

1. On the Welcome screen, navigate to **Quick Start** and click **Open Example Project**.

   ![](images/Create-Example-Project.PNG)

2. The Create Example Project wizard will be launched. Click **Next**.

3. In the Select Project Template window, click **Refresh** to install templates.

   ![](images/Install-Update_Example-Designs.PNG)
 
4. You will now be able to see the **Zynq Ultrascale+ MPSoC Design Presets** template. Select the template and click **Next**.

   ![](images/ZynqMPSoC-Preset-Template.PNG)

6. Update the Project Name to "zcu102" and update the path.

7. In the next window, select **Zynq UltraScale+ MPSoC ZCU102 Evaluation Board** and click **Next**. Here, you can see two preset designs. Select the second option, **Processing System and Programmable Logic (PS+PL) with GPIO and Block RAM**, and click **Finish**.

   ![](images/PS-PL.PNG)

  The block design you have created for the example project is shown in the following figure.

  ![](images/ZynqMP-Block-Design.PNG)

#### Generating the Bitstream
The preset example creates an HDL wrapper and also generates output products. You can directly perform synthesis and implementation, a
nd you can generate a bitstream. To generate a bitstream, click **Generate Bitstream**. 

#### Exporting the XSA

After successful bitsream generation, navigate to **File > Export > Export Hardware** to launch the Export Hardware Platform wizard. 
Click **Next** and export the hardware with the **Include bitstream** option. Enter the desired XSA name and complete the wizard.

![](images/Include-Bitstream.PNG)


#### Automation Steps

Launch the Vivado Tcl console. Source `vivado_design.tcl`. 

### PetaLinux Build

It is required to configure the PetaLinux project to support Linux application debug. Run the following commands for each step listed below. Users can use the BSP for their board. However, the steps below uses ZynqMP template, and the MACHINE_NAME is updated. This would be a more generic flow

1. Create the PetaLinux project:

    ```
    petalinux-create -t project --template -n xilinx-zcu102-2023.2
    ```

2. Configure the hardware description:

    ```
    cd xilinx-zcu102-2023.2
    petalinux-config --get-hw-description <path to XSA>
    ```

3. Configure the DTG Options 


    - Select `DTG Options`: 

        ```
        (zcu102-rev1.0) MACHINE_NAME
        ```

3. Configure the root file system to enable TCF agent:

    ```
    petalinux-config -c rootfs
    ```

    - Select `Filesystem Packages`: 

        ```
        Filesystem Packages ---> 
        ```

    - Scroll down to select `misc`.

        ```
        misc --->
        ```

    - Scroll down to select `tcf-agent`. Ensure that the `tcf-agent` is enabled.

        ```
        [*] tcf-agent 
        [ ] tcf-agent-dev 
        [ ] tcf-agent-dbg
        ```

4. Go back to the `Filesystem Packages` menu and click **console**.

    ```
    console --->
    ```

5. Navigate to `network`.

    ```
    network --->
    ```

6. Click into the `dropbear` submenu and make sure that `dropbear` is enabled:

    ```
    [*] dropbear
    ```

7. Go back to the previous menu to select OpenSSH and ensure that `openssh-sftp-server` is enabled.

    ```
    [ ] openssh 
    [ ] openssh-ssh 
    [ ] openssh-sftp 
    [*] openssh-sftp-server 
    [ ] openssh-keygen 
    [ ] openssh-dbg 
    [ ] openssh-dev 
    [ ] openssh-misc 
    [ ] openssh-sshd 
    [ ] openssh-scp
    ```

8. Save the configuration and exit. 

9. Build: 

    ```
    petalinux-build
    ```

10. Sysroot:

    ```
    petalinux-package --boot --u-boot
    petalinux-build --sdk
    petalinux-package --sysroot
    ```
**Note:** You can skip all the above steps by using the `petalinux_build.sh` script to build the Linux images. 


<p class="sphinxhide" align="center"><sub>Copyright © 2020–2024 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>