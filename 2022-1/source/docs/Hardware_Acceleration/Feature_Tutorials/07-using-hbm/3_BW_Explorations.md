<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Hardware Acceleration Tutorials</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

## HBM Bandwidth Explorations

In this section, you will observe achievable bandwidth using one HBM master port. You will also explore access to single or multiple Pseudo channels as various transaction sizes are initiated by the kernel master port.

- The topology, for example, M0 to single PC0 directly across the switch or M0 to a group PC0-1 or M0 to a group PC0-3.
- The number of bytes in the transaction vary from 64 bytes to 1024 bytes,
- Addressing used: sequential/linear accesses or random accesses,
- Use of the Random Access Memory Attachment (RAMA) IP to achieve better results.The RAMA IP is specifically designed to assist HBM-based designs with non-ideal
traffic masters and use cases. For more information, refer to <a href="https://www.xilinx.com/support/documentation/ip_documentation/rama/v1_1/pg310-rama.pdf"> RAMA LogiCORE IP Product Guide</a>



This section, via the above different configurations analyze enough data so that the developers will understand and make better decisions for their designs.

If your application is memory bound, it's always beneficial to access 64-bytes of data whether it's DDR or HBM. For this project, datawidth is set to 512 bits by default using `dwidth` variable in Makefile. You can experiment with smaller data width by changing this variable. Additionally, performance measured is based on M_AXI interface memory performance read-only and write performance is not measured in this section. The measured bandwidth is using C++ std::chrono to record the time just before kernel enqueues and just after the queue finish command. The bandwidth is reported in GB/s achieved.

The kernel ports in1,in2, and out are connected to all the HBM channels. In this scenario, each kernel port will have access to all the HBM channels. The application should implement this connectivity only if the application requires accessing all the channels. HBM memory subsystem will attempt to give the kernel the best access to all the memories connected to, say, kernel port in1 to M11 or M12 of the HBM subsystem. The application will experience extra latency to access the Psuedo channels on the extremes, say PC0 or PC31, from the middle master M12. Due to this, the application may require more outstanding transaction settings on AXI interfaces connected to kernel ports.

In this module, all the kernel ports are connected to all the Psudeo channels for simplicity.

Let's start with Bandwidth experiments using sequential accesses first.

### Sequential Accesses

In this step, first, you will build the xclbin that can support transaction size of, say 64 bytes, 128 bytes, 256 bytes,512 bytes, 1024 bytes. Next, you can explore achievable bandwidth accessing a single Pseudo channel of HBM (256MB), two Psuedo channels (512MB), and four Psuedo channels (1024MB)


Here is an example of building the application using following target for Master 0 accessing PC0 as shown below. (Don't run this command)

`make build TARGET=hw memtype=HBM banks=0_31 dsize=256 addrndm=0  txSize=64 buildxclbin=1`

The project provides the following flexibility to run an application using arguments, as shown below.

- dsize=256 will access only a single Pseudo channel, because the datasize on the host size is 256 MB
- txSize=64 will queue each command equivalent of 64 bytes from kernel port. Since each transfer is 64 bytes, this will be equivalent to a Burst length of 1. txSize=128 will be identical to Burst Length of 2, and so on.
- banks0_31 configures kernel's AXI master ports connect to all the banks. During the build, Makefile will create the HBM_connectivity.cfg file in the respective build directory. Refer to `mem_connectivity.mk` for more information.  You can also create your custom connectivity by updating in_M0, in_M1, and out_M2 variables
- addrndm=0 will ensure the address generated is sequential when the kernel is run. As seen previously, this is an argument to the kernel passed down from the host code.


The above build command will create the xclbin under <Project>/build/HBM_addSeq_allBanks_d512_txSize64

You can run the following command to generate the builds for txSize of 64,128,256,512,1024 bytes.

`make build_without_rama`    # This command is already executed in the first module

- If the machine doesn't have enough resources to launch six jobs in parallel, you can run the above command one by one, as shown below

    `make noramajob-64 noramajob-128 noramajob-256 noramajob-512 noramajob-1024`


To run the application with the above build created for txSize of 64,128,256,512,1024 bytes AND accessing 1,2,4 Pseudo channels (using dsize argument)

`make all_hbm_seq_run`

The above target will generate the output file `<Project>/makefile/Run_SequentialAddress.perf` file with the following data

```
Addr Pattern   Total Size(MB) Transaction Size(B) Throughput Achieved(GB/s)

Sequential     256 (M0->PC0)             64                     13.0996
Sequential     256 (M0->PC0)             128                    13.0704
Sequential     256 (M0->PC0)             256                    13.1032
Sequential     256 (M0->PC0)             512                    13.0747
Sequential     256 (M0->PC0)             1024                   13.0432

Sequential     512 (M0->PC0_1)           64                     13.1244
Sequential     512 (M0->PC0_1)           128                    13.1142
Sequential     512 (M0->PC0_1)           256                    13.1285
Sequential     512 (M0->PC0_1)           512                    13.1089
Sequential     512 (M0->PC0_1)           1024                   13.1097

Sequential     1024 (M0->PC0_3)          64                     13.148
Sequential     1024 (M0->PC0_3)          128                    13.1435
Sequential     1024 (M0->PC0_3)          256                    13.1506
Sequential     1024 (M0->PC0_3)          512                    13.1539
Sequential     1024 (M0->PC0_3)          1024                   13.1454

```

This use case shows the maximum results when using one kernel master, M0 to access HBM. The table above shows the measured bandwidth in GB/s achieved.

The top 5 rows show the point to point accesses, ie, 256 MB accesses, with the Transaction size variation. The bandwidth is consistent around 13 GB/s.

The next ten rows show a grouping of 2 pseudo channels and 4 pseudo channels, ie, 512 MB and 1024 MB, respectively, and the bandwidth is constant.

#### Conclusion: The bandwidth achieved for sequential accesses is mostly independent of the topology and is constant at about 13 GB/s.


### Random Accesses

We are using the same topologies as the previous step but using an addressing scheme using random addresses within the selected range.

To run all the variations like in the previous step, You can also use the following Makefile target to run the application. There is no need to rebuild the xclbins again.

`make all_hbm_rnd_run`

The above target will generate the output file `<Project>/makefile/Run_RandomAddress.perf` file with the following data.

```
Addr Pattern   Total Size(MB) Transaction Size(B)  Throughput Achieved(GB/s)

Random         256 (M0->PC0)             64                     4.75379
Random         256 (M0->PC0)             128                    9.59893
Random         256 (M0->PC0)             256                    12.6164
Random         256 (M0->PC0)             512                    13.1338
Random         256 (M0->PC0)             1024                   13.155

Random         512 (M0->PC0_1)           64                     0.760776
Random         512 (M0->PC0_1)           128                    1.49869
Random         512 (M0->PC0_1)           256                    2.71119
Random         512 (M0->PC0_1)           512                    4.4994
Random         512 (M0->PC0_1)           1024                   6.54655

Random         1024 (M0->PC0_3)          64                     0.553107
Random         1024 (M0->PC0_3)          128                    1.07469
Random         1024 (M0->PC0_3)          256                    1.99473
Random         1024 (M0->PC0_3)          512                    3.49935
Random         1024 (M0->PC0_3)          1024                   5.5307

```

The top 5 rows show the point to point accesses, ie 256 MB accesses, with a Transaction size variation. The bandwidth drops compared to the top 5 rows in the previous step when the address pattern was sequential. You can still experience decent bandwidth for larger transaction sizes, though.

The bandwidth drops compared to the top 5 rows from 13GB/s using the sequential accesses at the previous step. You can still experience better bandwidth for larger transaction sizes than 64 bytes though, this is simply explained because when accessing 128 bytes or more, then, only the first access is random the next accesses in the transaction are sequential, so the memory is better utilized, efficiency-wise.

When the master is addressing 2 or 4 PCs to access a larger range, the bandwidth will drop significantly. So it's important to observe that a single M_AXI connected to 1 PC will provide better bandwidth than connected to multiple PCs.


Let's use the specific example of Row 13, the transaction size is 256 bytes and using a 1 GB of randomly accessed data - i.e. utilizing PC0-3. We can see the performance is ~2 GB/s. If this was a real design need, it would be advantageous to change the microarchitecture of said design to use 4 M_AXI to access 4 individual PC in an exclusive manner. This means that the kernel code would have to check the index/address it wished to access and then exclusively use one of the pointer arguments (translating to one of the 4 M_AXI) to make this memory access. As you might have already understood the access range is now 256 MB per pointer/M_AXI, which basically means that we fall back to a use case where we have one master accessing one PC, and this is exactly the situation in Row 3. As a result, this would provide 12+ GB/s of bandwidth using 4 interfaces but with only one utilized at a time. You could try to further improve the situation by making 2 parallel accesses using those 4 M_AXI but this means that the part of the design providing the indexes/addresses need to provide 2 in parallel, which might be a challenge too.



#### Conclusion: The bandwith is higher when accessing a single Pseudo Channel over 256 MB data (or less) compared to accessing multiple Pseudo Channels.



### Random Accesses with RAMA IP

In this step, we are using the same topologies as the previous step, but now we are using RAMA IP to improve the overall bandwidth. This step will require the generation of new xclbins.

The v++ linker requires a tcl file to connect the RAMA IP to the Axi Master ports. Refer to the file `./makefile/rama_post_sys_link.tcl` for more information

The Makefile creates the cfg-rama.ini file shown below and configures the v++ linking phase using `--config cfg-rama.ini` option.
```
[advanced]
param=compiler.userPostSysLinkTcl=<Project>/makefile/rama_post_sys_link.tcl
```

To build all the xclbins, run the following target.

`make build_with_rama`   # This command is already executed in the first module

- If the machine doesn't have enough resources to launch six jobs in parallel, you can run the above command one by one, as shown below

    `make ramajob-64 ramajob-128 ramajob-256 ramajob-512 ramajob-1024 -j6`

To run all the variations like in the previous step, You can also use the following Makefile target to build and run the application.

    `make all_hbm_rnd_rama_run`

The above target will generate the output file `<Project>/makefile/Run_RandomAddressRAMA.perf` file with the following data.

```
Addr Pattern   Total Size(MB) Transaction Size(B)  Throughput Achieved(GB/s)

Random         256 (M0->PC0)             64                     4.75415
Random         256 (M0->PC0)             128                    9.59875
Random         256 (M0->PC0)             256                    12.6208
Random         256 (M0->PC0)             512                    13.1328
Random         256 (M0->PC0)             1024                   13.1261

Random         512 (M0->PC0_1)           64                     6.39976
Random         512 (M0->PC0_1)           128                    9.59946
Random         512 (M0->PC0_1)           256                    12.799
Random         512 (M0->PC0_1)           512                    13.9621
Random         512 (M0->PC0_1)           1024                   14.1694

Random         1024 (M0->PC0_3)          64                     6.39984
Random         1024 (M0->PC0_3)          128                    9.5997
Random         1024 (M0->PC0_3)          256                    12.7994
Random         1024 (M0->PC0_3)          512                    13.7546
Random         1024 (M0->PC0_3)          1024                   14.0694

```

The top 5 rows show the point to point accesses, i.e. 256 MB accesses, with a transaction size variation. The bandwidth achieved is very similar to the previous step without RAMA IP.
The next ten rows with access to 512 MB and 1024MB respectively show a significant increase in achieved bandwidth compared to the previous step when configuration didn't utilised  RAMA IP.



#### Conclusion: The RAMA IP significantly improves memory access efficiency in cases where the required memory access exceeds 256 MB (one HBM pseudo-channel)



# Summary

Congratulations! You have completed the tutorial.

In this tutorial, you learned it's relatively easy to migrate a DDR-based application to HBM based application using v++ flow. You also experimented with how the HBM based application throughput varies based on the address patterns and the overall memory being accessed by the kernel.

<p align="center"><b><a href="README.md">Return to Start of Tutorial</a></b></p>

<p align="center"><sup>Copyright&copy; 2020-2022 Xilinx</sup></p>
