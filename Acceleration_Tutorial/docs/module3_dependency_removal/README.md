Code and files for **module 3**

The DEPENDENCE pragma is used to provide additional information that can overcome loop-carry dependencies and allow loops to be pipelined (or pipelined with lower initiation intervals).

The Vitis high-level synthesis tool automatically detects the following dependencies:

+ Within loops (loop-independent dependence), or
+ Between different iterations of a loop (loop-carry dependence).

These dependencies impact when operations can be scheduled, especially during function and loop pipelining.

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

     :warning: IMPORTANT: Specifying a false dependency, when in fact the dependency is not false, 
     is likely to result in incorrect hardware. 
     Be sure dependencies are correct (either true or false)!
