3.3 Setting up board environment & Demo run
-----------------------
* Before we get our hands on the board, there are some dependencies that need to be prepared in advance. The corresponding script is provided in the Module_3 repository. Copy the prerequisite files and comipled project to the evaluation board.
    ```
    $cd [DOWNLOAD_PATH]/Board_Dependency
    $./get_dependency_zcu104.sh
    $cp [SDK_INSTALLATION_PATH]/glog-0.4.0/build_for_petalinux/glog-0.4.0-Linux.tar.gz .
    $cd ..
    $scp -r Board_Dependency root@[IP_OF_BOARD]:~/
    $scp -r ~/Vitis-AI/Vitis-AI-Library/overview root@[IP_OF_BOARD]:~/
    $ssh root@[IP_OF_THE_BOARD]
    ```
 * Now we can operate on the board through an ethernet connection.
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
   <img src="images/demo_single_thread.png">
   </p>
   Note that the user could also use parameters to define the number of threads and running time as below. For more detailed instruction, please refer to the Readme file within the refinedet folder.
   
   ```
   root@xilinx-zcu104-2019_2:~/overview/samples/refinedet# ./test_performance_refinedet refinedet_pruned_0_96 test_performance_refinedet.list -t 8 -s 60
   -t: <num_of_threads>
   -s: <num_of_seconds>
   ```
   <p align="left">
   <img src="images/demo_multi_threads.png">
   </p>
   
   Please follow the instruction on [Module 4](https://gitenterprise.xilinx.com/swm/Vitis-In-Depth-Tutorial/tree/master/Machine_Learning_Tutorial/Section_3-Basic/Module_4) for next step.
