<p align="right"><a href="../../../../README.md">English</a> | <a>日本語</a>
<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis ™ - ランタイムおよびシステムの最適化 - 例 3: XRT を使用したメモリ割り当て</h1><a href="https://japan.xilinx.com/products/design-tools/vitis.html">xilinx.com の Vitis ™開発環境を参照</a></td>
 </tr>
</table>

## 概要

メモリがページ境界に揃うように割り当てることで、パフォーマンスを最初のコンフィギュレーションよりも大幅に改善できました。  別のワークフローとして、OpenCL と XRT によりバッファーが割り当てられるようにし、そのバッファーをユーザー空間のポインターにマップして、アプリケーションで使用できるようにする方法があります。  その方法を使用して、タイミングにどのように影響するかを見てみます。

## キー コード

概念的にはわずかな変更ですが、この例では、例 2 より大きなコード変更が必要になります。  その理由は主に、標準ユーザー空間のメモリ割り当てを使用する代わりに、OpenCL ランタイムによりバッファーが割り当てられるようにするからです。  バッファーが割り当てられたら、バッファー内のデータにアクセスできるように、バッファーをユーザー空間にマップする必要があります。

ここでは、前の例のコードを次のように変更します。

```cpp
std::vector<cl::Memory> inBufVec, outBufVec;
cl::Buffer a_buf(context,
                 static_cast<cl_mem_flags>(CL_MEM_READ_ONLY |
                                           CL_MEM_ALLOC_HOST_PTR),
                 BUFSIZE*sizeof(uint32_t),
                 NULL,
                 NULL);
cl::Buffer b_buf(context,
                 static_cast<cl_mem_flags>(CL_MEM_READ_ONLY |
                                           CL_MEM_ALLOC_HOST_PTR),
                 BUFSIZE*sizeof(uint32_t),
                 NULL,
                 NULL);
cl::Buffer c_buf(context,
                 static_cast<cl_mem_flags>(CL_MEM_WRITE_ONLY |
                                           CL_MEM_ALLOC_HOST_PTR),
                 BUFSIZE*sizeof(uint32_t),
                 NULL,
                 NULL);
cl::Buffer d_buf(context,
                 static_cast<cl_mem_flags>(CL_MEM_READ_WRITE |
                                           CL_MEM_ALLOC_HOST_PTR),
                 BUFSIZE*sizeof(uint32_t),
                 NULL,
                 NULL);
```

プログラムの早い段階で OpenCL バッファーオブジェクトを割り当てており、ユーザー空間のポインターはまだありません。  それでも、これらのバッファー オブジェクトを `MigrateMemObjects()` などの OpenCL 関数に渡すことは可能です。  この時点で補助ストレージが割り当てられますが、そのユーザー空間ポインターはまだありません。

`cl::Buffer` コンストラクターへの呼び出しは、前とほぼ同じように見えます。  変更されているのは 2 点のみで、既存のバッファーを使用する代わりに新しいバッファーを割り当てることをランタイムに指示するため、`CL_MEM_USE_HOST_PTR` フラグではなく `CL_MEM_ALLOC_HOST_PTR` フラグを渡します。  また、新しいバッファーを割り当てているので、ユーザーバッファーにポインターを渡す必要はなく、代わりに `NULL` を渡します。

次に、ソフトウェアですぐに使用するバッファー `a`、`b`、`d` へのユーザー空間ポインターに OpenCL バッファーをマップする必要があります。  この時点では、`c` へのポインターをマップする必要はありません。後で、カーネル実行後にそのバッファーから読み出す必要があるときにマップできます。  これには、次のコードを使用します。

```cpp
uint32_t*a = (uint32_t*)q.enqueueMapBuffer(a_buf,
                                           CL_TRUE,
                                           CL_MAP_WRITE,
                                           0,
                                           BUFSIZE*sizeof(uint32_t));

uint32_t*b = (uint32_t*)q.enqueueMapBuffer(b_buf,
                                           CL_TRUE,
                                           CL_MAP_WRITE,
                                           0,
                                           BUFSIZE*sizeof(uint32_t));
uint32_t*d = (uint32_t*)q.enqueueMapBuffer(d_buf,
                                           CL_TRUE,
                                           CL_MAP_WRITE | CL_MAP_READ,
                                           0,
                                           BUFSIZE*sizeof(uint32_t));
```

マップしたら、通常どおりユーザー空間ポインターを使用してバッファーの内容にアクセスできます。ただし、OpenCL ランタイムではオープンになっているバッファーのカウントが参照されるので、マップするバッファーごとに `enqueueUnmapMemObject()` を呼び出す必要があります。

カーネルの実行フローは同じですが、入力バッファーをデバイスに戻すときに違いが現れます。移動を手動でエンキューする代わりに、バッファーをマップするだけです。  OpenCL ランタイムにより、バッファーの内容が現在 Alveo データセンター アクセラレータ カードのグローバル メモリ内にあることが認識され、バッファーがホストに戻されます。  コーディング スタイルは開発者しだいですが、基本的には、バッファー `c` をホストに戻すには、次のコードで十分です。

```cpp
uint32_t*c = (uint32_t*)q.enqueueMapBuffer(c_buf,
                                           CL_TRUE,
                                           CL_MAP_READ,
                                           0,
                                           BUFSIZE * sizeof(uint32_t));
```

最後に、ランタイムでメモリ オブジェクトが完全に消去されるようにするため、これらのメモリ オブジェクトのマップを解除する必要があります。  これは、前のようにバッファーに `free()` を設定するのではなく、プログラムの最後に実行します。この操作は、コマンド キューが終了する前に実行する必要があります。

```cpp
q.enqueueUnmapMemObject(a_buf, a);
q.enqueueUnmapMemObject(b_buf, b);
q.enqueueUnmapMemObject(c_buf, c);
q.enqueueUnmapMemObject(d_buf, d);
q.finish();
```

この使用モデルの主なワークフローをまとめると、次のタスクを実行する必要があります。

1. CL\_MEM\_ALLOC\_HOST\_PTR フラグを使用してバッファーを**割り当て**ます。
2. 入力バッファーをユーザー空間ポインターに**マップ**します。
3. カーネルを通常どおり実行します。
4. 出力バッファーを**マップ**してホスト メモリに戻します。
5. すべてのバッファーを使用し終えたら、正しく消去されるようにすべてのバッファーの**マップを解除**します。

## アプリケーションの実行

XRT ランタイムが初期化されたら、ビルド ディレクトリから次のコマンドを実行してアプリケーションを実行します。

`./03_buffer_map alveo_examples`

プログラムにより、次のようなメッセージが表示されます。

```
-- Example 3: Allocate and Map Contiguous Buffers --

Loading XCLBin to program the Alveo board:

Found Platform
Platform Name: Xilinx
XCLBIN File Name: alveo_examples
INFO: Importing ./alveo_examples.xclbin
Loading: './alveo_examples.xclbin'
Running kernel test with XRT-allocated contiguous buffers

OCL-mapped contiguous buffer example complete!

--------------- Key execution times ---------------
OpenCL Initialization:              247.460 ms
Allocate contiguous OpenCL buffers: 30.365 ms
Map buffers to userspace pointers:  0.222 ms
Populating buffer inputs:           22.527 ms
Software VADD run :                 24.852 ms
Memory object migration enqueue :   6.739 ms
Set kernel arguments:               0.014 ms
OCL Enqueue task:                   0.102 ms
Wait for kernel to complete :       92.068 ms
Read back computation results :     2.243 ms
```

| 操作| 例 2| 例 3| Δ2→3
|----------|:----------:|:----------:|:----------:
| OCL の初期化| 256.254 ms| 247.460 ms| \-
| バッファーの割り当て| 55 µs| 30.365 ms| 30.310 ms
| バッファーへの値の挿入| 47.884 ms| 22.527 ms| -25.357 ms
| ソフトウェア VADD| 35.808 ms| 24.852 ms| -10.956 ms
| バッファーのマップ| 9.103 ms| 222 µs| -8.881 ms
| バッファーの書き出し| 6.615 ms| 6.739 ms| \-
| カーネル引数の設定| 14 µs| 14 µs| \-
| カーネルの実行時間| 92.110 ms| 92.068 ms| \-
| バッファーの読み込み| 2.479 ms| 2.243 ms| \-
| ΔAlveo→CPU| -330.889 ms| -323.996 ms| -6.893 ms
| ΔFPGA→CPU (アルゴリズムのみ)| -74.269 ms| -76.536 ms| \-

ここでの高速化を期待していたかもしれませんが、特定の操作で高速化は見られず、システム内でレイテンシが移動しただけです。  つまり、別の銀行口座から税金を支払ったようなもので、税金から逃れることはできません。  プロセッサとカーネルのメモリ マップが 1 つに統合されているエンベデッド システムであれば、大きな違いが見られますが、サーバー クラスの CPU では違いは見られません。

この方法でバッファーをあらかじめ割り当てておくのは時間がかかりましたが、通常はアプリケーションのクリティカル パスにバッファーを割り当てるのは好ましくありません。  ただし、この方法では、ランタイムでのバッファーの使用はかなり高速になります。

このメモリにアクセスするのになぜ CPU のほうが高速なのでしょうか。  ここまで説明してきませんでしたが、この API を介してメモリを割り当てると、仮想アドレスが物理アドレスに固定されます。  これにより、CPU と DMA の両方でより効率よくメモリにアクセスできるようになります。  ただし、これにもコストがかかります。割り当てには時間がかかり、小型のバッファーを多数割り当てると使用可能なメモリがフラグメント化されてしまうリスクがあります。

通常は、バッファーはアプリケーションのクリティカル パス以外に割り当てるべきで、この方法を正しく使用すれば、負荷を高パフォーマンス部分から別の部分にシフトできます。

## 追加演習

この演習に、追加で次のことを試してみてください。

- 割り当てるバッファーのサイズを変えてみます。  前の例から導かれた関係は、この例でも有効ですか。
- メモリの割り当てと転送のエンキューに、ほかのシーケンスを試してみます。
- 入力バッファーを変更してカーネルを再実行すると、どうなりますか。

## 学習ポイント

- OpenCL および XRT の API を使用すると、領域によってパフォーマンスが向上しますが、根本的にはアクセラレーション税から逃れることはできません。
- カーネルの実行に 1 番時間がかかっていますが、それは簡単にスピードアップできます。  次の例で、これを見てみます。

[**例 4:** データパスの並列処理](./04-parallelizing-the-data-path.md)

<p align="center"><sup>Copyright&copy; 2019-2021 Xilinx</sup></p>
<p align="center"><sup>この資料は 2021 年 1 月 22 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
