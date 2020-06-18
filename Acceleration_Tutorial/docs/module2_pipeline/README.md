Code and files for **module 2**

## Pipelining for throughput improvement
The C code transformation that the high-level synthesis operates is conservative by default.  This affects loops that only work on one element at a time.  As a result, the operators that a loop uses in its body might not be used at each iteration of the loop.
