Code and files for **module 2**

## Pipelining for Throughput
The C code high-level synthesis is conservative by default, for example loop body instructions are entirely executed at each iteration instead of a staggered execution.  That latter style of execution is enabled by the PIPELINE pragma, it reduces the initiation interval (II) for a function or loop by allowing the concurrent execution of the different operations.
A pipelined function or loop can then process new inputs every <N> clock cycles, where <N> is the II of the loop or function. The default II for the PIPELINE pragma is 1, which processes a new input every clock cycle. You can also specify the initiation interval through the use of the II option.

Pipelining a loop allows its operations to be implemented in a concurrently as shown in the following animated figure. By default there are 3 clock cycles between each input read (so II=3), and it requires 12 clock cycles fully execute the loop compared to 6 when the pragma is used.

![Pipeline](../images/anim_pipeline.gif)

If the Vivado high-level synthesis tool cannot create a design with the specified II, it issues a warning and creates a design with the lowest possible II.
You can then analyze this design with the warning message to determine what steps must be taken to create a design that satisfies the required initiation interval.

To enable the pragma in the C source, insert it within the body of the function or loop.
```cpp
 #pragma HLS pipeline II=<int> enable_flush rewind
```
II=int specifies the desired number of clock cycles between each  initiation interval for the pipeline. The HLS tool tries to meet this request. Based on data dependencies, the actual result might have a larger initiation interval. The default II is 1.
enable_flush: Optional keyword that implements a pipeline that will flush and empty if the data valid at the input of the pipeline goes inactive.

TIP: This feature is only supported for pipelined functions: it is not supported for pipelined loops.
    rewind: Optional keyword that enables rewinding, or continuous loop pipelining with no pause between one loop iteration ending and the next iteration starting. Rewinding is effective only if there is one single loop (or a perfect loop nest) inside the top-level function. The code segment before the loop:
        Is considered as initialization.
        Is executed only once in the pipeline.
        Cannot contain any conditional operations (if-else).
TIP: This feature is only supported for pipelined loops; it is not supported for pipelined functions.



In the following example function foo is pipelined with an initiation interval of 1:
```cpp
void foo { a, b, c, d} {
  #pragma HLS pipeline II=1
  ...
}
```
