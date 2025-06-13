<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ In-Depth Tutorials</h1>
    </td>
 </tr>
</table>


# Package the desing and generate a SD card

The packaging step prepares and assembles boot artifacts, linux image and file system, and host applications and deploy them to a SD card image or folder.
For convenience with Windows OS, the formatting is set to fat32 so the SD card can be accessed natively by Windows.

To package the design, call Makefile from top folder:
```
make package
```



## Next step
  - [Run on hardware](../README.md#run-on-hardware)



<p class="sphinxhide" align="center"><sub>Copyright © 2020–2022 Xilinx, Inc</sub></p>
<p class="sphinxhide" align="center"><sub>Copyright © 2022–2025 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
