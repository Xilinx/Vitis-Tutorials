<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AMD Versal™ Adaptive SoC AI Engine Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See AMD Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See AMD Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Building the Design

*Estimated time: 4 hours*

```
make all
```
or
```
v++ -l                                     \
    -t hw                                  \
    --platform xilinx_vck190_base_202310_1 \
    --save-temps                           \
    -g                                     \
    --optimize 2                           \
    --hls.jobs 8                           \
    --config ./conn.cfg                    \
    --clock.defaultFreqHz 150000000        \
    --temp_dir ./build/_x_temp.hw.xilinx_vck190_base_202310_1               \
    --report_dir ./build/reports/_build.hw.xilinx_vck190_base_202310_1/hpc  \
    --advanced.param compiler.userPostSysLinkOverlayTcl=./post_sys_link.tcl  \
    -o './build/build_dir.hw.xilinx_vck190_base_202310_1/hpc.xclbin'        \
    ../Module_03_pl_kernels/build/_x_temp.hw.xilinx_vck190_base_202310_1/packet_sender.xo   \
    ../Module_03_pl_kernels/build/_x_temp.hw.xilinx_vck190_base_202310_1/m2s_x2.xo          \
    ../Module_03_pl_kernels/build/_x_temp.hw.xilinx_vck190_base_202310_1/packet_receiver.xo \
    ../Module_03_pl_kernels/build/_x_temp.hw.xilinx_vck190_base_202310_1/s2m_x4.xo          \
    ../Module_02_aie/build/libadf.a
```

## Full System Design

The AMD Vitis Linker (`v++ -l`) is used to link multiple kernel objects (XO), together with the hardware platform XSA file, to produce the device binary XCLBIN file.

Review the `conn.cfg` file. It creates an instance of each PL kernel described previously and provides the connection scheme between them and the AI Engine graph. At the end of the file, there are Vivado™ tool options specified to close timing and run the design at 300 MHz.

## Design Implementation

The following image was taken from the Vivado project for the entire design. It depicts the hardware implementation determined by the place-and-route on the adaptive SoC device.

![alt text](images/Nbody_Implemented_design.png)

## References

* [Beamforming Tutorial - Module_04 - AI Engine and PL Integration](https://github.com/Xilinx/Vitis-Tutorials/tree/master/AI_Engine_Development/Design_Tutorials/03-beamforming)

* [Vitis Compiler Command](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/v-Command)

## Next Steps

After linking the AI Engine design with the PL datamovers, you are ready to create the host software in the next module, [Module 05 - Host Software](../Module_05_host_sw).


### Support

GitHub issues will be used for tracking requests and bugs. For questions go to [support.xilinx.com](http://support.xilinx.com/).



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
