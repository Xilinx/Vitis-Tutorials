
# Hardware Run

To run the design on hardware, please ensure the design was run with below command
```
make all TARGET=hw
```
The above command will generate "pack_out_dir" containing all the file required for the hardware run

### Steps to run the design on Hardware

#### 1. Connect the VCK190 board and set the SD boot mode. Please refer to [UG1366](https://docs.amd.com/r/en-US/ug1366-vck190-eval-bd) to get more details on VCK190 board

#### 2.  Flash the SD Card   
Use the balenaEtcher/similar tools to flash the SD card. Plugin the SD card to the VCK190 board to initate the boot. 

#### 3.  Running the design and application on the VCK190   
Once the Linux boot is complete, use the below commands to run the host.exe and a.xclbin
versal-rootfs-common-20242:/mnt# 

```
sudo su
cd /run/media/mmcblk0p1
./host.exe a.xclbin
```

- Check the results

```
OUTPUT[0]=30
OUTPUT[1]=70
OUTPUT[2]=110
OUTPUT[3]=150
TEST PASSED

```
<p align="center">
<img src="support_files/HW_results.png" width="1300" />
</p>

With the above results on the hardware run, we have reached to the end of this tutorial. 


<p class="sphinxhide" align="center"><sub>Copyright © 2025 Advanced Micro Devices, Inc</sub></p>
<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
