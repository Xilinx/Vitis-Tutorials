<table>
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2019.2 Vitis™ Application Acceleration Development Flow Tutorials</h1>
   <a href="https://github.com/Xilinx/SDAccel-Tutorials/branches/all">See SDAccel™ Development Environment 2019.1 Tutorials</a>
   </td>
 </tr>
 <tr>
 <td align="center"><h1>Optimizing Accelerated FPGA Applications: Bloom Filter Example
 </td>
 </tr>
</table>

# 1. Evaluating the Original Application

## Application

Document filtering is a process where a system monitors a stream of incoming documents, classifies them according to their content, and selects those that are deemed relevant to a specific user or topic. The ability to filter out irrelevant information and organize relevant information into meaningful categories is important in a world where big data is playing an increasingly central role. Document filtering is used extensively in the everyday querying, retrieval, and analysis of information that helps to identify relevant documents of interest.

 In practical scenarios, the number of documents to be searched for an event can be very large and because the monitoring of events must run in real time, a smaller execution time is required for processing all the documents. In this tutorial, you compute a score for each document, which represents the document relevance. Performance of this application is measured in execution time and throughput for processing all the documents in the database.

 The user's interest is represented by a search array, which contains words of interest and has a weight associated with it, which indicates the prominence of the word based on user's interest. While monitoring the incoming stream of documents, you want to find the weight associated with words stored in the database. A native implementation would access the database for every word in the document to check if a word is present in the database and if present, retrieve the weight of the word. A more optimized approach would be using space-efficient bloom filter in your cache that can report whether a word is present in the database, which reduces the number of expensive database queries.

The bloom filter is a hash-table-based data structure that can be used to test whether an element is present in the dataset. False positive matches are possible, but false negatives are not; in other words, a query returns either "possibly in set" or "definitely not in set". The advantage of using a bloom filter is that it is space-efficient and reduces the number of database queries drastically for data which is not present in the database. A bloom filter is also useful in applications for implementing search engines and database management systems, such as Cassandra and Postgres where it can reduce the number of disk queries thereby increasing performance.

The false-positive rate (the number of cases where the bloom filter incorrectly reports that the data might be present in the database) can be reduced to less than 1% with less than 10 bits per element independent of the size or number of elements in the set.

Depending on the false-positive rate required, the number of hash-functions (number of elements in the set and number of bits in the bloom filter) can be adjusted.

The following figure shows an example of a bloom filter representing the set {x, y, z}. The colored arrows show the positions in the bit array that each set element is mapped to. The element 'w' is not in the set {x, y, z} because it hashes to a one bit-array position containing 0. In the following figure, the number of elements are 18 and the number of hash functions compute for each element is 3.

![](./images/bloom.PNG)

## Implementation

In this example, each document consists of an array of words where each word is a 32-bit unsigned integer comprised of a 24-bit word ID and an 8-bit representing the frequency. The search array consists of words of interest to the user. It represents a smaller set of 24-bit word IDs, where each word ID has a weight associated with it, determining the importance of the word. If you navigate to `bloom` directory and then navigate to the `cpu_src` directory and look at line 65 in the `main.cpp` file, you can see that the bloom filter size is 64 KB, which is implemented as `1L<<bloom_size` where `bloom_size` is defined as 14 in header file `sizes.h` thereby (2^14)*4B = 64 KB.

The score for each document is obtained by the cumulative product of multiplying the weight of word ID with its frequency. The greater the score, the more relevant the document matching the search array.

Example:

* **Search Array**: [{word_1,10},{word_3,20},{word_6,30}]
* **Document**: [{word_1,20} ,{word_2,40},{word_3,50}]
* **Compute Score for document**:  20x10 + 0x40 + 50x20 = 1200

In this step, you will build and run this application to create baseline performance data for the original, non-accelerated application.

## Build the C Application

Navigate to the `cpu_src` directory and run **make** to generate the executable file

  The command compiles the C source code and builds the `host` executable. The executable requires the number of documents as an input argument.
>**TIP:** The `Makefile` used in this lab is detailed and contains many steps and variables. For a discussion of the structure and contents of the Makefile, refer to [Understanding the Makefile](./HowToRunTutorial.md).

## Run the C Application and Generate the Golden Result

In this step, run the original C application with the number of documents as the argument and generate the golden output files for comparison purposes using the following commands.

   ```
   cd bloom/design/cpu_src/build
   ./host cpu 100000
   ```

The generated output compute scores are stored in the `cpu_profile_score` array in the host code, which represents the outputs for the total number of documents specified.

## Profile the Application

Because you only have the function `runOnCPU` to accelerate, you will run the executable and evaluate the execution time of the function.

1. Extract the profile result.

   ```
   gprof host gmon.out> gprofresult.txt
   ```

2. To view the Profile Summary report, open the `gprofresult.txt` file in a text editor. You should see results similar to the following table.

   Each sample counts as 0.01 seconds.

   The flat profile report of the individual sub-functions is as follows.
   >**NOTE:** The performance profile will vary based on your machine and machine load.

   | % Time | Cumulative Seconds | Self Seconds | Total Calls  | ms/Call  | ms/Call  | Name                         |  
   |--------:|-----------:|----------:|----------:|----------:|----------:|:------------------------------|  
   | 48.06  |     8.76  |   8.76   |   802078720  |  0.00  |  0.00   | MurmurHash2                 |
   | 24.93  |     13.30 |   4.54   |    1   |   4.54   |   13.30 | runOnCPU              |
   | 21.14 | 17.15 | 3.85 | 1 | 2.85 | 4.88 | setupData |

   You can see that the application spends almost half of time in the sub-function `MurmurHash2`, which computes the hash values of words in each document. The `MurmurHash2` sub-function is called by `runOnCPU` and `setupData` functions as part of the `main` function.

   The following table shows a high-level view of the functions inside the `main` function as part of the call graph for the previous report `gprofresults.txt`.

    | % Time |         Self              |  Children | Called | Name
   |--------:|-----------:|----------:|----------:|:------------|
   | 72.9 | 4.54 | 8.76 | 1 | runOnCPU |
   | 26.8  | 3.85 | 1.03 | 1 | setupData |

   From a `main` function standpoint, you can see that the CPU spends almost 73% of time in the `runOnCPU` function. In the `runOnCPU` function, there is only one child call, which is the `MurmurHash2` function. Based on execution times from table, you can deduce that around 66% **((8.76/(8.76+4.54))** of time is spent by calls to `MurmurHash2`. Therefore, accelerating the `runOnCPU` function would significantly increase the performance of the application.

## Determine the Maximum Achievable Throughput

In most FPGA accelerated systems, the maximum achievable throughput is limited by the PCIe® bus. The PCIe performance is influenced by many different aspects, such as the motherboard, drivers, targeted platform, and transfer sizes. The Vitis core development kit provides a utility, `xbutil`, and you can run the `xbutil dmatest` command to measure the maximum PCIe bandwidth it can achieve. The throughput on your design target cannot exceed this upper limit.

## Establish Overall Acceleration Goals

In this tutorial, your goal is to process 100,000 documents (1.6 GB). On the CPU, it takes 11.22 ms to process 100 documents. Each document contains about 3500 words (unsigned int) in average. This means you are achieving a throughput of approximately **(3500\*100)\*4B/11.2 ms =  124.78 MB/s**. Because your goal is to process 100,000 documents (1.6 GB), if you run the host code executable with 100,000 documents, you can see that the execution time is 11.74s.

You want to improve the application throughput so that the execution time takes less than one second because you should be able to compute the scores of documents in real time. With this goal in mind, the first thing to do is to check whether this goal is achievable with the FPGA. Having a 0.6 second execution time for processing 100,000 documents implies that the throughout required is 2.67 GB/s as you are processing documents the size of 1.6 GB. First, determine if this throughput is less than the maximum achievable by the kernel and also whether the throughput goal is within the bounds of maximum achievable throughput of an Alveo Data Center accelerator card.

As explained in [Step 3: Identify Device Parallelization Needs](https://www.xilinx.com/html_docs/xilinx2019_2/vitis_doc/Chunk1821279816.html#kjk1555544737506) in the [Methodology for Accelerating Applications with the Vitis Unified Software Platform](https://www.xilinx.com/html_docs/xilinx2019_2/vitis_doc/Chunk1821279816.html#wgb1568690490380), the maximum throughput achievable from kernel can be approximated as:

**Thw = (Frequency / Computational Intensity) = (Frequency * max(VINPUT, VOUTPUT) / VOPS)**

where:

* **VINPUT**, **VOUTPUT** represent the volume of input and output data processed respectively.
* **VOPS** represents the volume of operations processed on the input and output data.
* **Computational Intensity** of a function is the ratio of the total number of operations to the maximum amount of input and output data.  In this application, the volume of input data processed is greater than the volume of output data generated (as document score is computed only once per document). Each input word is only used once for computing the document score, hence the computational intensity is 1.

**Thw = (Frequency\*1)samples**

Because each sample is 4 bytes of data, the maximum throughput of kernel is: **Thw = (300MHz)\*4B = 1.2GB/s**.

Analyzing the nature of parallelism within the code, you can see that the compute score of each document is independent of other documents. Within each document, the compute consists of two parts: computing hash function of words and computing the document score based on hash values. The first part of computing hash functions can be done in parallel for each word and the second part is a reduction step where you reduce the values of words based on the hash values computed in the first part.

One of the ways you can achieve parallelism is by widening the datapath of CU so you can process more words in parallel, making your CU more powerful. The other way is to replicate CUs without widening the datapath of CUs to compute the score for multiple documents in parallel. You can follow either approach or take a hybrid approach for best results. In this tutorial, you using the replicating CUs approach as it is easier to code and upi can still achieve your expected throughput.

Because your target throughput is 2.67 GB/s, you can replicate the CUs to achieve the required throughput. In the later optimizations of this tutorial, you notice that there are external memory stalls, which prevent the kernel from reaching its maximum achievable throughput. Hence, you will keep the number of CUs as 4. One thing to note while replicating CUs is that the resources are scaled linearly with CUs. If the resource utilization exceeds resources on the FPGA, the design cannot be built on the FPGA. Therefore, after running a single CU, you need to estimate the maximum number of CUs that can fit in the device.

The throughput goal is also within the bounds of throughput that can be achieved on the PCIe interface on an Alveo Data Center accelerator card. This tutorial will walk you through a predictable process for achieving that goal.

## Next Step

You have identified the functions from the original application that are targets for acceleration and established the performance goals. In the following labs, you will create a baseline of the original bloom filter function running in hardware and perform a series of host and kernel code optimizations to meet your performance goals. You will begin by [creating a Vitis core development kit application](./baseline_fpga.md) from the original application.

You will be using hardware emulation runs for measuring performance in each step. As part of the final step, you can run all these steps in hardware to demonstrate how the performance was improved at each step.
</br>
<hr/>
<p align="center"><b><a href="/docs/vitis-getting-started/">Return to Getting Started Pathway</a> — <a href="./README.md">Return to Start of Tutorial</a></b></p>

<p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>
