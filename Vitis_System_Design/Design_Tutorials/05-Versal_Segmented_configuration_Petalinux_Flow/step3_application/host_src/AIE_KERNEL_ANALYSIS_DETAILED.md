# AIE Kernel 功能详细分析和 Host 代码可行性评估

## 1. AIE Kernel 功能分析

### 1.1 Kernel 架构

```
┌─────────────────────────────────────────────────────────┐
│                    AIE Graph (mygraph)                   │
│                                                          │
│  input_plio → first simple → second simple → output_plio│
│                                                          │
│  每次处理: 32 个 cint16 样本                            │
│  迭代次数: 4 次                                          │
│  总处理: 32 × 4 = 128 个样本                            │
└─────────────────────────────────────────────────────────┘
```

**关键配置：**
- `NUM_SAMPLES = 32`：每次迭代处理的样本数
- 两个 `simple` kernel 串联：`first → second`
- 数据类型：`cint16`（复数16位，32位格式：real[15:0], imag[31:16]）

### 1.2 数据变换分析

通过对比 `input.txt` 和 `golden.txt`，可以推断 `simple` kernel 的功能：

#### 输入数据模式（input.txt 前16行）：
```
0 1    → (real=0, imag=1)
2 3    → (real=2, imag=3)
4 5    → (real=4, imag=5)
6 7    → (real=6, imag=7)
8 9    → (real=8, imag=9)
10 11  → (real=10, imag=11)
12 13  → (real=12, imag=13)
14 15  → (real=14, imag=15)
1 0    → (real=1, imag=0)    ← 注意：实部虚部交换
3 2    → (real=3, imag=2)
5 4    → (real=5, imag=4)
7 6    → (real=7, imag=6)
9 8    → (real=9, imag=8)
11 10  → (real=11, imag=10)
13 12  → (real=13, imag=12)
15 14  → (real=15, imag=14)
```

#### 输出数据模式（golden.txt 前16行）：
```
0 2    → (real=0, imag=2)     = (0*2, 1*2)
4 6    → (real=4, imag=6)     = (2*2, 3*2)
8 10   → (real=8, imag=10)   = (4*2, 5*2)
12 14  → (real=12, imag=14)   = (6*2, 7*2)
16 18  → (real=16, imag=18)   = (8*2, 9*2)
20 22  → (real=20, imag=22)   = (10*2, 11*2)
24 26  → (real=24, imag=26)   = (12*2, 13*2)
28 30  → (real=28, imag=30)   = (14*2, 15*2)
2 0    → (real=2, imag=0)     = (1*2, 0*2)    ← 注意：实部虚部交换
6 4    → (real=6, imag=4)     = (3*2, 2*2)
10 8   → (real=10, imag=8)    = (5*2, 4*2)
14 12  → (real=14, imag=12)   = (7*2, 6*2)
18 16  → (real=18, imag=16)   = (9*2, 8*2)
22 20  → (real=22, imag=20)   = (11*2, 10*2)
26 24  → (real=26, imag=24)   = (13*2, 12*2)
30 28  → (real=30, imag=28)   = (15*2, 14*2)
```

### 1.3 Simple Kernel 功能推断

**结论：`simple` kernel 对每个 cint16 样本执行以下操作：**

```cpp
// 伪代码
void simple(cint16 in, cint16 &out) {
    out.real = in.real * 2;  // 实部乘以2
    out.imag = in.imag * 2;  // 虚部乘以2
}
```

**验证：**
- 输入 `(0, 1)` → 输出 `(0, 2)` ✓
- 输入 `(2, 3)` → 输出 `(4, 6)` ✓
- 输入 `(1, 0)` → 输出 `(2, 0)` ✓
- 输入 `(3, 2)` → 输出 `(6, 4)` ✓

**两个 kernel 串联的效果：**
- `first`: `(real, imag) → (real*2, imag*2)`
- `second`: `(real*2, imag*2) → (real*4, imag*4)`

但根据 golden 数据，看起来只乘以了2，所以可能是：
- 两个 kernel 都执行相同的操作，但最终效果是乘以2
- 或者第二个 kernel 是直通（passthrough）

### 1.4 数据流处理流程

```
输入数据 (128 samples)
    ↓
mm2s: 从 DDR 读取，通过 stream 发送
    ↓ [Stream: 32 samples per iteration]
AIE Graph (4 iterations):
    Iteration 0: 处理 samples 0-31
    Iteration 1: 处理 samples 32-63
    Iteration 2: 处理 samples 64-95
    Iteration 3: 处理 samples 96-127
    ↓
    first simple kernel: (real, imag) → (real*2, imag*2)
    ↓
    second simple kernel: 可能也是乘以2，或直通
    ↓
s2mm: 从 stream 接收，写入 DDR
    ↓
输出数据 (128 samples)
```

## 2. Host 代码可行性分析

### 2.1 ✅ 正确的部分

#### 1. 数据大小和迭代配置
```cpp
#define INPUT_SIZE  128    // 正确：32 × 4 = 128
#define OUTPUT_SIZE 128   // 正确：32 × 4 = 128
#define NO_OF_ITERATIONS  4  // 正确：与 graph.run(4) 一致
int sizeIn = INPUT_SIZE;     // 128 samples
int sizeOut = OUTPUT_SIZE;   // 128 samples
```
**✅ 正确**：数据大小与迭代次数匹配

#### 2. 数据加载
```cpp
memcpy(in_bomapped, input_data, input_size_in_bytes);
```
**✅ 正确**：直接使用 step2 的 `input_data` 数组

#### 3. Kernel 启动顺序
```cpp
xrtRunStart(mm2s_rhdl);    // 1. 启动 mm2s
xrtRunStart(s2mm_rhdl);    // 2. 启动 s2mm
xrtGraphRun(ghdl, itr);    // 3. 启动 AIE graph
```
**✅ 正确**：先启动 PL kernel，再启动 AIE graph

#### 4. 同步和等待
```cpp
xrtRunWait(mm2s_rhdl);     // 等待 mm2s 完成
xrtRunWait(s2mm_rhdl);     // 等待 s2mm 完成
xrtBOSync(out_bohdl, XRT_BO_SYNC_BO_FROM_DEVICE, ...);  // 同步输出
```
**✅ 正确**：等待所有 kernel 完成后再同步数据

#### 5. 结果验证
```cpp
for (int i = 0; i < sizeOut; i++) {
    if (out_bomapped[i] != golden[i]) { ... }
}
```
**✅ 正确**：与 step2 的 `golden` 数据比较

### 2.2 ⚠️ 潜在问题和注意事项

#### 1. mm2s/s2mm 的 size 参数

**当前代码：**
```cpp
rval = xrtRunSetArg(mm2s_rhdl, 2, sizeIn);   // sizeIn = 128
rval = xrtRunSetArg(s2mm_rhdl, 2, sizeOut);  // sizeOut = 128
```

**分析：**
- mm2s 会一次性读取 128 个样本并发送到 stream
- Graph 运行 4 次迭代，每次从 stream 读取 32 个样本
- s2mm 会从 stream 接收 128 个样本并写入内存

**✅ 这应该是可行的**，因为：
  - mm2s 和 graph 是异步的，stream 作为缓冲区
  - 只要 mm2s 发送的总数据量 ≥ graph 需要的总数据量即可
  - s2mm 会等待 graph 输出所有数据

**⚠️ 但需要注意：**
  - 确保 stream 缓冲区足够大
  - 确保数据流顺序正确（FIFO）

#### 2. 数据格式一致性

**检查：**
- Host 使用：`int32_t`（32位整数）
- AIE kernel 使用：`cint16`（复数16位，32位格式）
- 格式：`real[15:0], imag[31:16]` = `(imag << 16) | real`

**✅ 格式一致**：
```cpp
// input.txt: "0 1" → 0x00010000
// real=0, imag=1 → (1 << 16) | 0 = 0x00010000 ✓
```

#### 3. Graph 名称

**当前代码：**
```cpp
auto ghdl = xrtGraphOpen(dhdl, uuid, "mygraph");
```

**检查 graph.cpp：**
```cpp
simpleGraph mygraph;  // Graph 实例名称是 "mygraph"
```

**✅ 名称匹配**

#### 4. 数据同步时机

**当前顺序：**
1. 等待 mm2s 完成
2. 等待 s2mm 完成
3. 同步输出 buffer
4. 比较结果

**✅ 顺序正确**：必须等待所有 kernel 完成后再同步和比较

### 2.3 🔍 需要验证的点

#### 1. Simple Kernel 的实际实现

由于 `kernels.cc` 未提供，无法100%确定功能。但从数据推断：
- 最可能：每个样本的实部和虚部都乘以2
- 需要在实际运行中验证

#### 2. 两个 Kernel 串联的效果

- 如果两个 kernel 都乘以2，最终应该是乘以4
- 但 golden 数据显示只乘以2
- 可能：第二个 kernel 是直通，或者有其他逻辑

#### 3. Stream 缓冲和同步

- mm2s 发送 128 个样本到 stream
- Graph 分 4 次每次读取 32 个样本
- 需要确保 stream 有足够的缓冲空间

## 3. 总结

### ✅ Host 代码总体可行

**优点：**
1. 数据大小计算正确（128 samples）
2. 数据格式正确（cint16 格式）
3. Kernel 启动顺序合理
4. 同步机制正确
5. 验证逻辑正确

**需要注意：**
1. 确保 stream 缓冲区足够
2. 验证 simple kernel 的实际功能
3. 在实际硬件/仿真中测试验证

### 📝 建议

1. **添加调试输出**：
   ```cpp
   // 打印前几个输入/输出样本
   for (int i = 0; i < 8; i++) {
       printf("Input[%d] = 0x%08X, Output[%d] = 0x%08X, Golden[%d] = 0x%08X\n",
              i, input_data[i], i, out_bomapped[i], i, golden[i]);
   }
   ```

2. **验证数据转换**：
   - 确认 input.txt → input_data 的转换正确
   - 确认 golden.txt → golden 的转换正确

3. **测试运行**：
   - 在硬件仿真中测试
   - 检查是否有数据不匹配
   - 如果失败，分析错误模式

### 🎯 预期行为

如果一切正常，应该看到：
- mm2s 发送 128 个样本
- Graph 处理 4 次迭代，每次 32 个样本
- s2mm 接收 128 个样本
- 输出与 golden 数据完全匹配
- 测试通过：`TEST PASSED`
