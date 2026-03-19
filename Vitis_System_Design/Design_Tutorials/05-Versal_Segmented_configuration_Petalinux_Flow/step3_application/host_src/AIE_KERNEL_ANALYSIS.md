# AIE Kernel 功能分析和 Host 代码评估

## 1. AIE Kernel 功能分析

### 1.1 Kernel 结构

从 `step2_vitis_integration/src/graph.h` 可以看出：

```cpp
class simpleGraph : public adf::graph {
  kernel first;   // 第一个 simple kernel
  kernel second;  // 第二个 simple kernel
  
  // 数据流：input -> first -> second -> output
  adf::connect(in.out[0], first.in[0]);
  connect(first.out[0], second.in[0]);
  connect(second.out[0], out.in[0]);
  
  // 每次处理 NUM_SAMPLES = 32 个 cint16 样本
  dimensions(first.in[0]) = { NUM_SAMPLES };  // 32
  dimensions(first.out[0]) = { NUM_SAMPLES }; // 32
  dimensions(second.in[0]) = { NUM_SAMPLES }; // 32
  dimensions(second.out[0]) = { NUM_SAMPLES }; // 32
}
```

### 1.2 数据流和处理流程

```
DDR Memory (input_data)
    ↓
mm2s (Memory-to-Stream)
    ↓ [Stream: 32 samples per iteration]
AIE Graph:
    input_plio → first simple kernel → second simple kernel → output_plio
    ↓ [Stream: 32 samples per iteration]
s2mm (Stream-to-Memory)
    ↓
DDR Memory (output)
```

**关键参数：**
- `NUM_SAMPLES = 32`：每次迭代处理的样本数
- `NO_OF_ITERATIONS = 4`：总共运行4次迭代
- 数据类型：`cint16`（复数16位整数，每个样本32位：real[15:0], imag[31:16]）

### 1.3 Simple Kernel 的功能

虽然 `kernels.cc` 文件未提供，但从接口可以推断：

```cpp
void simple(adf::input_buffer<cint16> & in, adf::output_buffer<cint16> & out);
```

- **输入**：32个 cint16 样本
- **输出**：32个 cint16 样本
- **处理**：两个 simple kernel 串联处理，具体功能取决于实现（可能是：
  - 直通（passthrough）
  - 简单的数学运算（如加法、乘法）
  - 数据格式转换等）

### 1.4 总数据处理量

- **每次迭代**：处理 32 个样本
- **4次迭代**：总共处理 32 × 4 = **128 个样本**
- **输入大小**：128 个 int32（每个 cint16 样本占 32 位）
- **输出大小**：128 个 int32

## 2. Host 代码合理性分析

### 2.1 ✅ 正确的部分

#### 1. 数据大小计算
```cpp
int sizeIn = INPUT_SIZE;   // 128 (包含所有迭代的数据)
int sizeOut = OUTPUT_SIZE; // 128 (包含所有迭代的数据)
```
**✅ 正确**：INPUT_SIZE 和 OUTPUT_SIZE 已经包含了4次迭代的所有数据（32×4=128）

#### 2. 数据加载
```cpp
memcpy(in_bomapped, input_data, input_size_in_bytes);
```
**✅ 正确**：直接使用 `input_data` 数组，无需额外初始化

#### 3. Graph 运行
```cpp
xrtGraphRun(ghdl, itr);  // itr = 4
```
**✅ 正确**：运行4次迭代，每次处理32个样本

#### 4. 结果验证
```cpp
xrtBOSync(out_bohdl, XRT_BO_SYNC_BO_FROM_DEVICE, output_size_in_bytes, 0);
for (int i = 0; i < sizeOut; i++) {
    if (out_bomapped[i] != golden[i]) { ... }
}
```
**✅ 正确**：
- 同步设备数据到主机
- 与 golden 数据逐样本比较

### 2.2 ⚠️ 潜在问题

#### 1. mm2s 和 s2mm 的 size 参数

**当前代码：**
```cpp
rval = xrtRunSetArg(mm2s_rhdl, 2, sizeIn);   // sizeIn = 128
rval = xrtRunSetArg(s2mm_rhdl, 2, sizeOut);  // sizeOut = 128
```

**分析：**
- mm2s 会一次性读取 128 个样本并发送到 stream
- s2mm 会从 stream 接收 128 个样本并写入内存
- Graph 运行4次，每次处理32个样本

**✅ 这应该是正确的**，因为：
  - mm2s 和 s2mm 是异步运行的
  - Graph 会从 stream 中按需读取数据（每次32个样本，共4次）
  - 只要 mm2s 发送的总数据量 ≥ Graph 需要的总数据量即可

#### 2. 数据同步顺序

**当前代码顺序：**
```cpp
xrtRunStart(mm2s_rhdl);    // 启动 mm2s
xrtRunStart(s2mm_rhdl);    // 启动 s2mm
xrtGraphRun(ghdl, itr);    // 启动 graph
xrtRunWait(mm2s_rhdl);     // 等待 mm2s
xrtRunWait(s2mm_rhdl);     // 等待 s2mm
```

**✅ 正确**：先启动所有 kernel，然后等待完成

#### 3. 内存同步

**当前代码：**
```cpp
xrtBOSync(out_bohdl, XRT_BO_SYNC_BO_FROM_DEVICE, output_size_in_bytes, 0);
```

**✅ 正确**：在比较结果前必须同步设备数据到主机

### 2.3 🔍 需要确认的点

#### 1. Simple Kernel 的实际功能

由于 `kernels.cc` 未提供，无法确定：
- simple kernel 是否只是直通（passthrough）？
- 还是做了某种变换？
- **建议**：查看实际的 kernel 实现，或通过仿真/硬件运行来确定期望输出

#### 2. Golden 数据的准确性

`output.h` 中的 `golden` 数组目前是占位符值，需要根据实际 kernel 行为更新。

#### 3. 数据格式

- 输入/输出都是 `int32_t`（每个 cint16 样本占32位）
- 低16位 = real，高16位 = imaginary
- **✅ 格式正确**

## 3. 总结

### ✅ Host 代码总体上是合理的

1. **数据大小计算正确**：128个样本（32×4）
2. **数据流控制正确**：mm2s → Graph → s2mm
3. **同步机制正确**：等待所有 kernel 完成后再同步和比较
4. **验证逻辑正确**：与 golden 数据逐样本比较

### ⚠️ 需要注意的事项

1. **更新 golden 数据**：根据实际 kernel 行为更新 `output.h` 中的期望值
2. **确认 kernel 功能**：如果可能，查看 `kernels.cc` 的实现
3. **测试验证**：在实际硬件或仿真环境中测试，确认数据流正确

### 📝 建议的改进

如果发现数据不匹配，可以：
1. 添加调试输出，打印前几个输入/输出样本
2. 确认 simple kernel 的实际功能
3. 检查数据格式是否正确（real/imaginary 的位顺序）
