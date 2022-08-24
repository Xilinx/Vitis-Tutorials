<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Versal® ACAP AI Engine Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
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
    --platform xilinx_vck190_base_202110_1 \
    --save-temps                           \
    -g                                     \
    --optimize 2                           \
    --hls.jobs 8                           \
    --config ./conn.cfg                    \
    --clock.defaultFreqHz 300000000        \
    -I../Module_03_pl_kernels/Vitis_Libraries/utils/L1/include              \
    -I../Module_03_pl_kernels/Vitis_Libraries/utils/L1/include/hw           \
    --temp_dir ./build/_x_temp.hw.xilinx_vck190_base_202110_1               \
    --report_dir ./build/reports/_build.hw.xilinx_vck190_base_202110_1/hpc  \
    --vivado.synth.jobs 8 --vivado.impl.jobs 8                              \
    -o './build/build_dir.hw.xilinx_vck190_base_202110_1/hpc.xclbin'        \
    ../Module_03_pl_kernels/build/_x_temp.hw.xilinx_vck190_base_202110_1/packet_sender.xo   \
    ../Module_03_pl_kernels/build/_x_temp.hw.xilinx_vck190_base_202110_1/m2s_x2.xo          \
    ../Module_03_pl_kernels/build/_x_temp.hw.xilinx_vck190_base_202110_1/packet_receiver.xo \
    ../Module_03_pl_kernels/build/_x_temp.hw.xilinx_vck190_base_202110_1/s2m_x4.xo          \
    ../Module_02_aie/build/libadf.a
```

## Full System Design

The Vitis Linker (`v++ -l`) is used to link multiple kernel objects (XO), together with the hardware platform XSA file, to produce the device binary XCLBIN file.

Review the `conn.cfg` file. It creates an instance of each PL kernel described previously and provides the connection scheme between them and the AI Engine graph. At the end of the file, there are vivado options specified to close timing and run the design at 300 MHz.

## Design Implementation

The following image was taken from the Vivado project for the entire design. It depicts the hardware implementation determined by the place-and-route on the ACAP device.

![alt text](images/Design%20Implementation.png)

## References

* [Beamforming Tutorial - Module_04 - AI Engine and PL Integration](https://github.com/Xilinx/Vitis-Tutorials/tree/master/AI_Engine_Development/Design_Tutorials/03-beamforming)

* [Vitis Compiler Command](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Vitis-Compiler-Command)

## Next Steps

After linking the AI Engine design with the PL datamovers, you are ready to create the host software in the next module, [Module 05 - Host Software](../Module_05_host_sw).


# Support

GitHub issues will be used for tracking requests and bugs. For questions go to [support.xilinx.com](http://support.xilinx.com/).

# License

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.

You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )



Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

<p align="center"> XD068 | &copy; Copyright 2021 Xilinx, Inc.</p>
