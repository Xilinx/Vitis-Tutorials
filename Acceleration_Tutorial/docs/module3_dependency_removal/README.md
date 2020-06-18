Code and files for **module 3**

The DEPENDENCE pragma is used to provide additional information that can overcome loop-carry dependencies and allow loops to be pipelined (or pipelined with lower intervals).

The Vivado High-Level Synthesis (HLS) tool automatically detects the following dependencies:

    Within loops (loop-independent dependence), or
    Between different iterations of a loop (loop-carry dependence).

These dependencies impact when operations can be scheduled, especially during function and loop pipelining.

    Loop-independent dependence: The same element is accessed in the same loop iteration.
```cpp
    for (i=0;i<N;i++) {
     A[i]=x;
     y=A[i];
    }
```
    Loop-carry dependence: The same element is accessed in a different loop iteration.
```cpp
    for (i=0;i<N;i++) {
     A[i]=A[i-1]*2;
    }
```
Under certain complex scenarios automatic dependence analysis can be too conservative and fail to filter out false dependencies. Under some circumstances, such as variable dependent array indexing, or when an external requirement needs to be enforced (for example, two inputs are never the same index), the dependence analysis might be too conservative. The DEPENDENCE pragma allows you to explicitly specify the dependence and resolve a false dependence.
IMPORTANT: Specifying a false dependency, when in fact the dependency is not false, can result in incorrect hardware. Be sure dependencies are correct (true or false) before specifying them.
