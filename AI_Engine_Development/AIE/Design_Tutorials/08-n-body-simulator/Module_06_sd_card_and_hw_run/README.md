<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AMD Versal™ Adaptive SoC AI Engine Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See AMD Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See AMD Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Building the Design

*Estimated time: 6 minutes*

```
make all
```

or

```
v++ -t hw                                                                         \
    --platform xilinx_vck190_base_202310_1                                        \
    --package.defer_aie_run                                                       \
    -p ../Module_04_xclbin/build/build_dir.hw.xilinx_vck190_base_202310_1/hpc.xclbin \
    ../Module_02_aie/build/libadf.a                                               \
    --package.out_dir ./build                                                     \
    --package.rootfs $(COMMON_IMAGE_VERSAL)/rootfs.ext4                           \
    --package.kernel_image $(COMMON_IMAGE_VERSAL)/Image                           \
    --package.sd_file $(EMBEDDED_EXEC_SCRIPT)                                     \
    --package.boot_mode=sd                                                        \
    --package.image_format=ext4                                                   \
    --package.sd_file ../Module_05_host_sw/build/ps_app_animate.exe               \
    --package.sd_file ../Module_05_host_sw/build/ps_app.exe                       \
    --package.sd_file ../data/m2s_i.txt                                           \
    --package.sd_file ../data/input_j.txt                                         \
    --package.sd_file ../data/s2m_golden_i_k0.txt                                 \
    --package.sd_file ../data/s2m_golden_i_k1.txt                                 \
    --package.sd_file ../data/s2m_golden_i_k2.txt                                 \
    --package.sd_file ../data/s2m_golden_i_k3.txt                           
```

## SD Card Image Generation

After compiling and linking your kernel code to build the XCLBIN, you need to package the device binary, along with the host applications, to build a package that can be booted and run on the hardware device. The `v++ -p` step, packages the final product into an `sd_card.img` file at the end of the `v++` compile and link process.

## Booting the VCK190 Board

Flash the `sd_card.img` to an sd card and boot your VCK190 production board.

**Step 1.** Ensure your board is powered OFF.

**Step 2.** Use an SD card writer (such as balenaEtcher) to flash the `build/packet_hw/sd_card.img` file an SD card.

**Step 3.** Plug the flashed SD card into the top slot of the VCK190 board.

**Step 4.** Set the switch SW1 Mode\[3:0\]=1110 = OFF OFF OFF ON.

**Step 5.** Connect your computer to the VCK190 board using the included USB cable.

**Step 6.** Open a TeraTerm terminal and select the correct COM port. Set the port settings to the following:

```
Port: <COMMXX>
Speed: 115200
Data: 8 bit
Parity: none
Stop Bits: 1 bit
Flow control: none
Transmit delay: 0 msec/char 0 msec/line
```

**Step 7.** Power ON the board.

**Step 8.** Wait until you see the `root@versal-rootfs-common-<tools-version>` Linux command prompt. Press enter a few times to get past any `xinit` errors.

**Step 9.** Run the following commands into the TeraTerm terminal:

```
cd /mnt/sd-mmcblk0p1
export XILINX_XRT=/usr
```

## Running the Design on Hardware

Once the Linux command line displays, go to the `/mnt/sd-mmcblk0p1` partition and you should see the files you packaged into the `sd_card.img`. From here run the functional test:

```
./ps_app.exe --timesteps 1
```

You can also review the `run.log` for the messages written by the host application.

Next profile and compare the execution times for the C++ N-Body Simulator running on the A72 processor vs. the AI Engine N-Body Simulator design.

*Estimated Time: 2 minutes*

```
./ps_app.exe --timesteps 1 --profile
```

The output on the console should be something like this:

```
Initializing ADF API...
[INFO]: Cout Level set to [INFO]:
[INFO]: Log Level set to [MESSAGE]:
[INFO]: argv[0]=./ps_app.exe
[INFO]: argv[1]=--timesteps
[INFO]: argv[2]=1
[INFO]: argv[3]=--profile
==============================
Welcome to the NBODY Simulator
==============================
NUM PARTICLES: 12800
NUM TIMESTEPS: 1
==============================
[INFO]: Profiling turned on ...
[INFO]: Elapsed time for NBody Simulator executed in host application is 123.954 seconds
[INFO]: Starting iteraton 0 ...
[INFO]: Elapsed time for NBody Simulator executed in AI Engine is 0.004657468 seconds
[INFO]: TEST PASSED
```

Now run the executable to generate data to create the particle animation in the next module:

```
./ps_app_animate.exe --timesteps 300
```

Create `data` directory in `Module_07_results/`. Copy the `animation_data.txt` from the board to the `Module_07_results/data/` folder if you want to create an animation out of it.

```
scp animation_data.txt <user>@<ip-address>:/<project-dir>/Module_07_results/data/.
```

## References

* [Vitis Compiler Command](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/v-Command)

## Next Steps

After running the design on hardware, you can compare the performance between the Python, C++, and AI Engine NBody Simulators in the next module, [Module 07 - Results](../Module_07_results).


### Support

GitHub issues will be used for tracking requests and bugs. For questions go to [support.xilinx.com](http://support.xilinx.com/).



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
