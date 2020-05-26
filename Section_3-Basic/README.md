Section 3
=======================

Xilinx has provided pre-built board image to show quick demos how AI could help in various application scenario.
The following introduction will show step by step guidance to run the AI demo on ZCU104 evaluation board.

3.1 Retrieve pre-built DPU platform
-----------------------
* Prerequisites
  * Terminal emulater (https://osdn.net/projects/ttssh2)
  * Win32 Disk Imager (https://sourceforge.net/projects/win32diskimager)
 
* Download the pre-built DPU board image from the link below

  * [ZCU104](https://www.xilinx.com/bin/public/openDownload?filename=xilinx-zcu104-dpu-v2019.2-v2.img.gz) 

* Choose an unpartitioned SD card of size 8GB or greater. Use Win32DiskImager to burn the image file onto the SD card.
  * Unzip the image file ``xilinx-zcu104-dpu-v2019.2-v2.img.gz`` using windows extractor
  * Browse to the location of the unzipped image in the Win32 utility
  * Choose the correct SD card under 'Device'
  * Select 'Write' to the SD card, click 'Yes' at the prompt to continue writing and wait till the operation is complete
  
  <p align="left">
  <img width="720" height="482" src="Module_1/images/win32.png">
  </p>
  
* Afterwards, please get the ZCU104 board ready with one serial cable and ethernet cable.
  * Insert the SD card with the image into the destination board.
  * Plug in the power and boot the board using the serial port to operate on the system.
  * Set up the IP information of the board using the serial port. 
    ```
    root@xilinx-zcu104-2019_2:~# ifconfig eth0 [IP_OF_BOARD] 
    ```
  * Now the board could be operated by using SSH.
  <p align="left">
  <img width="720" height="660" src="Module_1/images/ZCU104.png">
  </p>

3.2 Setting cross-compilation environment
-----------------------
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
  <img width="720" height="158" src="Module_2/images/cross-compile.png">
  </p>
  
3.3 Setting up board environment & Demo run
-----------------------
* Before get hands on to the board, there are some dependencies need to be prepared in advance. The corresponding script is provided in the Module_3 repository. Copy the prerequisite files and comipled project to evaluation board.
    ```
    $cd [DOWNLOAD_PATH]/Board_Dependency
    $./get_dependency_zcu104.sh
    $cp [SDK_INSTALLATION_PATH]/glog-0.4.0/build_for_petalinux/glog-0.4.0-Linux.tar.gz .
    $cd ..
    $scp -r Board_Dependency root@[IP_OF_BOARD]:~/
    $scp -r ~/Vitis-AI/Vitis-AI-Library/overview root@[IP_OF_BOARD]:~/
    $ssh root@[IP_OF_THE_BOARD]
    ```
 * Now we could operate on board through ethernet connection.
   * Install the dependencies
   ```
   root@xilinx-zcu104-2019_2:~# cd Board_Dependency
   root@xilinx-zcu104-2019_2:~/Board_Dependency# ./install_dependency_zcu104.sh
   ```
   * Run the demo
   ```
   root@xilinx-zcu104-2019_2:~/Board_Dependency# cd ../overview/samples/refinedet
   root@xilinx-zcu104-2019_2:~/overview/samples/refinedet# ./test_performance_refinedet refinedet_pruned_0_96 test_performance_refinedet.list
   ```
   <p align="left">
   <img src="Module_3/images/demo_single_thread.png">
   </p>
   Note that user could also use parameters to define the number of threads and running time as below. For more detailed instruction, please refer to the Readme file within the refinedet folder
   
   ```
   root@xilinx-zcu104-2019_2:~/overview/samples/refinedet# ./test_performance_refinedet refinedet_pruned_0_96 test_performance_refinedet.list -t 8 -s 60
   -t: <num_of_threads>
   -s: <num_of_seconds>
   ```
   <p align="left">
   <img src="Module_3/images/demo_multi_threads.png">
   </p>
