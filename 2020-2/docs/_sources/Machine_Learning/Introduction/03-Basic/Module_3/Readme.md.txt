3.3 Setting cross-compilation environment
-----------------------
User could use either Windows or Linux OS to download the board image and burn it to SD card. But this module could only be done on Linux X86 server.
* Download the [sdk-2020.1.0.0.sh](https://www.xilinx.com/bin/public/openDownload?filename=sdk-2020.1.0.0.sh) script on the host. The ```sdk``` file is generated through ```petalinux-build --sdk``` command, and applied to setup the sysroot headers, libs and include files for cross compilation of applications running on the embedded platforms.
* Run the script to install the cross-compilation system dependency and sysroot.
   ```
   $./sdk-2020.1.0.0.sh
   ```
  Follow the prompts to install, note that the read-write permissions is required for the installation path. The installation will take around 10mins. By default it will be installed on the directory of ``~/petalinux_sdk``, which user could modify to other location if prefer to.
* When the installation completes, follow the prompts and execute the following command to install the embedded ARM cross-compilation environment on X86 server.
   ```
   $source [SDK_INSTALLATION_PATH]/environment-setup-aarch64-xilinx-linux
   ```
  Note that the command needs to be re-executed each time when a new terminal interface is opened.
* Now we need to add some AI related libs and includes to the existing sysroot by simply download the Vitis AI denpendencies [vitis_ai_2020.1-r1.2.0.tar.gz](https://www.xilinx.com/bin/public/openDownload?filename=vitis_ai_2020.1-r1.2.0.tar.gz) and extract it to the sysroot.
   ```
   $tar -xzvf vitis_ai_2020.1-r1.2.0.tar.gz -C ~/petalinux_sdk/sysroots/aarch64-xilinx-linux
   ```

* The host setup is completed and we could take refinedet as an example to do the cross-compile in the AI Library.
    ```
    $cd ~/Vitis-AI/Vitis-AI-Library/overview/samples/refinedet
    $bash -x build.sh
    ```
  You will find four execuable files generated after compilation. Please follow the instruction on [Module 4](../Module_4) for next step.
  <p align="left">
  <img width="720" height="158" src="images/cross-compile.png">
  </p>

<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020-2021 Xilinx</sup></p>
