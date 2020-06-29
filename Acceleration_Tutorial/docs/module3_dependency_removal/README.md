Code, files and instructions for **module 3** (same flow as in [<b>module 1</b>](../module1_baseline) to setup the Vits GUI or run make)

Here are the optimizations applied:
 + The loop are explicitely pipelined as they were in module 2 but the tool would have pipelined these loops by default as we've seen through the module 1 results
 + To circumvent the II violation of 5, we create multiple mult-add that we store in an array called tmp 
 + Then to avoid limited throughput because of port access we use a tmpcol array that we load at the beginning of the algorithm

  <details>
  <summary><b> Click to expand! Learn about the <code>dependence</code> pragma...</b></summary>
  
The dependence pragma provides additional information to overcome loop-carry dependencies and allow loops to be pipelined like we saw in module2 (or pipelined with lower initiation intervals). These dependencies impact the scheduling of operations during high-level synthesis for both function and loop pipelining.

The Vitis high-level synthesis tool automatically detects the following dependencies:

+ Within loops (loop-independent dependence), or
+ Between different iterations of a loop (loop-carry dependence).

Loop-independent dependence: The same element is accessed in the same loop iteration.
```cpp
# A[] is accessed multiple time in the loop body
    for (i=0;i<N;i++) {
     A[i]=x;
     y=A[i];
    }
```
Loop-carry dependence: The same element is accessed in a different loop iteration.
```cpp
# Same element of A[] is accessed across loop iterations 
    for (i=0;i<N;i++) {
     A[i]=A[i-1]*2;
    }
```
Under certain complex scenarios automatic dependence analysis can be too conservative and fail to filter out false dependencies. Under some circumstances, such as variable dependent array indexing, or when an external requirement needs to be enforced (for example, two inputs are never the same index), the dependence analysis might be too conservative. The DEPENDENCE pragma allows you to explicitly specify the dependence and resolve a false dependence.

     IMPORTANT! Specifying a false dependency, when in fact the dependency is not false, 
     is likely to result in incorrect hardware. 
     Be sure dependencies are correct (either true or false)! 
      
</details>



