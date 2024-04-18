Vector Addition
===============

This is a simple example of vector addition. The kernel uses HLS Dataflow which allows the user to schedule multiple task together to achieve higher throughput.

Vitis kernel can have one s_axilite interface which will be used by host application to configure the kernel. All the global memory access arguments are associated to m_axi(AXI Master Interface) as below:

.. code:: cpp	

   #pragma HLS INTERFACE m_axi port = in1 bundle = gmem0
   #pragma HLS INTERFACE m_axi port = in2 bundle = gmem1
   #pragma HLS INTERFACE m_axi port = out bundle = gmem0

Multiple interfaces can be created based on the requirements. For example when multiple memory accessing arguments need access to global memory simultaneously, user can create multiple master interfaces and can connect to different arguments.

Usually data stored in the array is consumed or produced in a sequential manner, a more efficient communication mechanism is to use streaming data as specified by the STREAM pragma, where FIFOs are used instead of RAMs.

Vector addition in kernel is divided into 4 sub-tasks(read input 1, read input 2 , compute_add and write) which are then performed concurrently using ``Dataflow``.

.. code:: cpp

   #pragma HLS dataflow
       load_input(in1, in1_stream, size);
       load_input(in2, in2_stream, size);
       compute_add(in1_stream, in2_stream, out_stream, size);
       store_result(out, out_stream, size);
