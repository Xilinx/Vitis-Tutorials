3.2 Setting cross-compilation environment
-----------------------
Before we move to the board environment, some work needs to be done on the host side. You could use either Windows or Linux OS to download the board image and burn it to SD card. But this module could only be done on Linux X86 server.
* Download the [sdk.sh](https://www.xilinx.com/bin/public/openDownload?filename=sdk.sh) script on the host. The sdk.sh file is generated through ```petalinux-build --sdk``` command, and applied to setup the sysroot headers, libs and include files for cross compilation of applications running on the embedded platforms.
* Run the script to install the cross-compilation system dependency and sysroot.
   ```
   $./sdk.sh
   ```
  Follow the prompts to install, note that the read-write permissions is required for the installation path. The installation will take around 10mins. By default it will be installed on the directory of ``~/petalinux_sdk``, which user could modify to other location if prefer to.
* When the installation completes, follow the prompts and execute the following command to install the embedded ARM cross-compilation environment on X86 server.
   ```
   $source [SDK_INSTALLATION_PATH]/environment-setup-aarch64-xilinx-linux
   ```
  Note that the command needs to be re-executed each time when a new terminal interface is opened.
* Now we need to add some AI related libs and includes to the existing sysroot by simply download the Vitis AI denpendencies [vitis_ai_2019.2-r1.1.0.tar.gz](https://www.xilinx.com/bin/public/openDownload?filename=vitis_ai_2019.2-r1.1.0.tar.gz) and extract it to the sysroot.
   ```
   $tar -xzvf vitis_ai_2019.2-r1.1.0.tar.gz -C ~/petalinux_sdk/sysroots/aarch64-xilinx-linux
   ```
* The last step would be update the glog to v0.4.0, as the Vitis AI dependency is higher than the petalinux default glog version. This step could be ignored after Vitis AI 1.2 release.
	* Download the glog package and extract it.
		```
		$cd [SDK_INSTALLATION_PATH]
		$curl -Lo glog-v0.4.0.tar.gz https://github.com/google/glog/archive/v0.4.0.tar.gz
		$tar -zxvf glog-v0.4.0.tar.gz
		$cd glog-0.4.0
		```
	* Build and repack it for the embedded Linux system.
		```
		$mkdir build_for_petalinux
		$cd build_for_petalinux
		$unset LD_LIBRARY_PATH; source ~/petalinux_sdk/environment-setup-aarch64-xilinx-linux
		$cmake -DCPACK_GENERATOR=TGZ -DBUILD_SHARED_LIBS=on -DCMAKE_INSTALL_PREFIX=$OECORE_TARGET_SYSROOT/usr ..
		$make && make install
		$make package
		```
* The host setup is completed and we could take refinedet as an example to do the cross-compile in the AI Library.
    ```
    $cd ~/Vitis-AI/Vitis-AI-Library/overview/samples/refinedet
    $bash -x build.sh
    ```
  You will find four execuable files generated after compilation. Please follow the instruction on [Module 3](https://gitenterprise.xilinx.com/swm/Vitis-In-Depth-Tutorial/tree/master/Machine_Learning_Tutorial/Section_3-Basic/Module_3) for next step.
  <p align="left">
  <img width="720" height="158" src="images/cross-compile.png">
  </p>

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
