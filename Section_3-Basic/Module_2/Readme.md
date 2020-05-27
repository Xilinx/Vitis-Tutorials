3.2 Setting cross-compilation environment
-----------------------
Before we move to the board environment, some work need to be operated on the host side.
* Download the [sdk.sh](https://www.xilinx.com/bin/public/openDownload?filename=sdk.sh) on host
* Install the cross-compilation system environment, follow the prompts to install. 
   ```
   $./sdk.sh
   ```
  Note that the read-write permissions is required for the installation path. The installation will take around 10mins.
* When the installation completes, follow the prompts and execute the following command.
   ```
   $source [SDK_INSTALLATION_PATH]/environment-setup-aarch64-xilinx-linux
   ```
  Note that the command need to be re-executed each time when a new terminal interface open up.
* Download the Vitis AI denpendencies [vitis_ai_2019.2-r1.1.0.tar.gz](https://www.xilinx.com/bin/public/openDownload?filename=vitis_ai_2019.2-r1.1.0.tar.gz) and extract it to the sysroot.
   ```
   $tar -xzvf vitis_ai_2019.2-r1.1.0.tar.gz -C ~/petalinux_sdk/sysroots/aarch64-xilinx-linux
   ```
* Update the glog to v0.4.0
	* Download the glog package and extract it.
		```
		$cd [SDK_INSTALLATION_PATH]
		$curl -Lo glog-v0.4.0.tar.gz https://github.com/google/glog/archive/v0.4.0.tar.gz
		$tar -zxvf glog-v0.4.0.tar.gz
		$cd glog-0.4.0
		```
	* Build and repack it for the PetaLinux system.
		```
		$mkdir build_for_petalinux
		$cd build_for_petalinux
		$unset LD_LIBRARY_PATH; source ~/petalinux_sdk/environment-setup-aarch64-xilinx-linux
		$cmake -DCPACK_GENERATOR=TGZ -DBUILD_SHARED_LIBS=on -DCMAKE_INSTALL_PREFIX=$OECORE_TARGET_SYSROOT/usr ..
		$make && make install
		$make package
		```
* Cross compile the samples in the AI Library, taking refinedet as an example.
    ```
    $cd ~/Vitis-AI/Vitis-AI-Library/overview/samples/refinedet
    $bash -x build.sh
    ```
  You will find four execuable files generated after compilation.
  <p align="left">
  <img width="720" height="158" src="images/cross-compile.png">
  </p>
