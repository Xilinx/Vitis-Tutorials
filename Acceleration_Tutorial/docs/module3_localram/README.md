Code, files and instructions for **module 3** (same flow as in [<b>module 1</b>](../module1_baseline) to setup the Vits GUI or run make)

The kernel source code is same as in module 2.

The difference is in the Vitis v++ configuration file which is now assigning local block of RAM within the Xilinx device logic array for faster access:
 + By default host in Alveo cards connections are assigned to the DDRs available on the cards.  Here since we target a U50 that does not have DDR but HBM, connection go to that memory.  It is however possible to use smaller memories that are part of the logic array.
