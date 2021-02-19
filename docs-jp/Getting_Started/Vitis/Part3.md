<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis™ 入門チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/tree/2020.1">2020.1 チュートリアルを参照</a></td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# Vitis フロー 101 – パート 3: vector-add 例の紹介

このチュートリアルで使用される例は、vector-add アプリケーションです。この例はシンプルなので、複雑なアルゴリズムの検討に煩わされることなく、FPGA アクセラレーションの主な概念に集中できます。

## vector-add カーネルのソース コード

このチュートリアルでは、ハードウェア アクセラレータ (カーネルとも呼ばれる) は C++ で記述されています。Vitis フローでは、Verilog または VHDL でコード記述されたカーネルもサポーされます。vector-add カーネルの Verilog RTL バージョンを使用した例 については、[ここ](https://github.com/Xilinx/Vitis_Accel_Examples/tree/master/rtl_kernels/rtl_vadd)を参照してください。

C++ を使用すると、ハードウェア アクセラレータの記述は 20 行未満のコードに収まり、Vitis コンパイラを使用して FPGA に簡単かつ効率的にインプリメンテーションできます。

```cpp
extern "C" {
  void vadd(
    const unsigned int *in1, // Read-Only Vector 1
    const unsigned int *in2, // Read-Only Vector 2
    unsigned int *out,       // Output Result
    int size                 // Size in integer
  )
  {

#pragma HLS INTERFACE m_axi port=in1 bundle=aximm1
#pragma HLS INTERFACE m_axi port=in2 bundle=aximm2
#pragma HLS INTERFACE m_axi port=out bundle=aximm1

    for(int i = 0; i < size; ++i)
    {
      out[i] = in1[i] + in2[i];
    }
  }
}
```

このシンプルな例では、C++ カーネルの 2 つの重要な側面を取り上げています。

1. Vitis では、名前修飾問題を回避するために C++ カーネルを `extern “C”` として宣言する必要があります。
2. Vitis コンパイル プロセスの結果は、ソース コードでのプラグマを使用することで制御されます。

このほか、vector-add カーネルの機能が簡単にわかるようになっています。vadd 関数は 2 つの入力ベクター (in1 および in2) を読み取り、単純な for ループを使用してそれらを out ベクターに追加します。size パラメーターは、入力ベクターと出力ベクターのエレメント数を示します。

プラグマは、関数パラメーターを個別のカーネル ポートにマップするために使用されます。2 つの入力パラメーターを異なる入力ポートにマッピングすることで、カーネルは両方の入力を並列に読み取ることができるようなります。この入門チュートリアルでは詳細を説明しませんが、通常はハードウェア アクセラレータのインターフェイス要件を考慮することが重要です。これらの要件によって、達成可能な最大のパフォーマンスが変わります。

Vitis 資料には、[C++ カーネルのコーディングに関する考慮事項](https://japan.xilinx.com/html_docs/xilinx2020_2/vitis_doc/devckernels.html#rjk1519742919747)に関する全体的な情報と、[プラグマのリファレンスガイド](https://japan.xilinx.com/html_docs/xilinx2020_2/vitis_doc/tfo1593136615570.html)が含まれます。

## ホスト プログラムのソース コード

ホスト プログラムのソース コードは C/C++ で記述され、標準 OpenCL API を使用してハードウェア アクセラレーションされた vector-add カーネルとやりとりします。

* このチュートリアルの `src` ディレクトリにある [`host.cpp`](./example/src/host.cpp) ファイルを開きます。

このシンプルな例のソース コードには、4 つの主なステップがあります。

* **ステップ 1:** OpenCL 環境が初期化されます。このセクションでは、ホストが接続されたザイリンクス デバイスを検出し、ファイルから FPGA バイナリ (.xclbin ファイル) を読み込んで、最初に検出されたザイリンクス デバイスにプログラムします。次に、コマンド キューとカーネル オブジェクトが作成されます。すべての Vitis アプリケーションには、このセクションのコードと非常によく似たコードが含まれます。

* **ステップ 2:** アプリケーションは、カーネルとデータを共有するために必要な 3 つのバッファー (各入力に 1 つ、出力に 2 つ) を作成します。データセンター プラットフォームでは、4K ページ境界にアライメントされたメモリをより効率的に割り当てることができます。エンベデッド プラットフォームでは、連続的なメモリ割り当てを実行する方が効率的です。これらのいずれかを達成するには、バッファー作成時にザイリンクス ランタイムにホスト メモリを割り当てさせるのが最も簡単な方法です。これには、バッファーを作成するときに `CL_MEM_ALLOC_HOST_PTR` フラグを使用し、割り当てられたメモリをユーザー空間ポインターにマッピングします。

```cpp
    // Create the buffers and allocate memory   
    cl::Buffer in1_buf(context, CL_MEM_ALLOC_HOST_PTR | CL_MEM_READ_ONLY,  sizeof(int) * DATA_SIZE, NULL, &err);

    // Map host-side buffer memory to user-space pointers
    int *in1 = (int *)q.enqueueMapBuffer(in1_buf, CL_TRUE, CL_MAP_WRITE, 0, sizeof(int) * DATA_SIZE);
```

>**注記**: アプリケーションが明示的にホスト メモリを割り当て、バッファーの作成時に対応するポインターを再利用する方法もよく使用されます。この例でこの方法を使用するのは、データセンターとエンベデッド プラットフォームの両方で最もポータブルで効率的な方法だからです。

* **ステップ 3:** ホスト プログラムはカーネルの引数を設定してから、3 つの操作 (2 つの入力ベクターをデバイス メモリへ転送、カーネルを実行、最後に結果をホスト メモリへ転送) をスケジュールします。これらの操作は、ステップ 1 で宣言したコマンド キューに格納されます。これら 3 つの関数呼び出しはノンブロッキングであることに留意してください。コマンドはキューに入れられ、ザイリンクス ランタイムがそれらをデバイスに送信します。この例のホスト コードで使用されるキューは順序付きキューであるため、これらのコマンドは指定された順序で実行されます。ただし、キューが順不同なこともあり、準備ができたときにノンブロッキング呼び出しが順番通りではなく、順不同に実行されることもあります。`q.finish()` への呼び出しは、すべてのエンキューされたコマンドが完了するまで待機する必要があります。

```cpp
    // Set kernel arguments
    krnl_vector_add.setArg(0, in1_buf);
    krnl_vector_add.setArg(1, in2_buf);
    krnl_vector_add.setArg(2, out_buf);
    krnl_vector_add.setArg(3, DATA_SIZE);

    // Schedule transfer of inputs to device memory, execution of kernel, and transfer of outputs back to host memory
    q.enqueueMigrateMemObjects({in1_buf, in2_buf}, 0 /* 0 means from host*/);
    q.enqueueTask(krnl_vector_add);
    q.enqueueMigrateMemObjects({out_buf}, CL_MIGRATE_MEM_OBJECT_HOST);

    // Wait for all scheduled operations to finish
    q.finish();
```

* **ステップ 4:** キューに入れられたすべての操作が完了すると、`q.finish()` への呼び出しが返されます。この場合、カーネルの結果を含む出力バッファーがホスト メモリに戻され、ソフトウェア アプリケーションで安全に使用できることを意味します。ここでは、単にプログラムが終了する前に、結果が予測値と比較されます。

この例は、OpenCL API を使用してハードウェア アクセラレータとやりとりする最も簡単な方法を示しています。通常どおり、その他の情報は [Vitis の資料](https://japan.xilinx.com/html_docs/xilinx2020_2/vitis_doc/devhostapp.html#vpy1519742402284)を参照してください。

## 次の手順

ここまででカーネルとホスト プログラムの両方のソース コードを理解したはずです。[**パート 4**](./Part4.md) では、この例をコンパイルして実行する方法を説明します。

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
