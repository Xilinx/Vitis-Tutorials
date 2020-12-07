# Section 3: Basic Use Case

## 3.1 "Hello World" Preparation

The pre-built [DPU](https://github.com/Xilinx/Vitis-AI/tree/v1.1/DPU-TRD) platform which is dedecated for machine learning applications is provided in this module, user could simply retrieve it and write it into SD card to prepare the running environment.


* Prerequisites

  * Terminal emulator, e.g. [Tera Term](https://osdn.net/projects/ttssh2)

  * SD Card Image Writer tool, e.g. [Etcher](https://etcher.io/)

* Download the pre-built DPU board image from the link below

  * [ZCU104 prebuilt image](https://www.xilinx.com/bin/public/openDownload?filename=xilinx-zcu104-dpu-v2020.1-v1.2.0.img.gz)
  
  **To improve the user experience, the Vitis AI Runtime packages, VART samples, Vitis-AI-Library samples and
models have been built into the board image. Therefore, user does not need to install Vitis AI
Runtime packages and model package on the board separately. However, users can still install
the model or Vitis AI Runtime on their own image or on the official image by following these
steps.**

* Choose an unpartitioned SD card of size 8GB or greater. Use Etcher to burn the image file onto the SD card.
  * Etcher launches, as shown in the following figure.
  
  <p align="left">
  <img width="452" height="232" src="images/etcher1.png">
  </p>  
  
  * Browse to the location of the image.You can select a .zip or .gz compressed file.
  * Etcher tries to detect the SD drive. Verify the drive designation and the image size.
  * Click 'Flash' and wait till the operation is complete

  <p align="left">
  <img width="452" height="232" src="images/etcher2.png">
  </p>

* Afterwards, please get the ZCU104 board ready with one serial cable and ethernet cable.
  * Insert the SD card with the image into the destination board.
  * Plug in the power and boot the board using the serial port to operate on the system.
  * Find out the IP information of the board using the serial port.
    ```
    root@xilinx-zcu104-2020_1:~# ifconfig
    ```
    <p align="left">
    <img width="600" height="308" src="images/IP_IDT.png">
    </p>
  * Now the board is ready to be operated by using SSH. Please follow the instruction on [Module 2](../Module_2) for next step.
  <p align="left">
  <img width="720" height="660" src="images/ZCU104.png">
  </p>

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
