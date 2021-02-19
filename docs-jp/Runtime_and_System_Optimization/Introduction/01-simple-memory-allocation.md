<p align="right"><a href="../../../../README.md">English</a> | <a>日本語</a>
<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis ™ - ランタイムおよびシステムの最適化<br/>例 1: 単純なメモリ割り当て</h1><a href="https://japan.xilinx.com/products/design-tools/vitis.html">xilinx.com の Vitis ™開発環境を参照</a></td>
 </tr>
</table>

## 概要

読み込んだ FPGA イメージには、非常に単純なベクター加算コアが含まれています。  任意の長さのバッファー 2 つを入力として使用し、同じの長さバッファーを出力として生成します。  「ベクター加算コア」という名前からわかるように、このコアは 2 つの値を加算します。

提供されているコードは、FPGA 用に最適化されていません。  次のコードに示すアルゴリズムを直接 FPGA ファブリックに読み込んだのとほぼ同じです。  特に効率が良いわけではありません。インプリメンテーション ツールである程度の最適化は実行されますが、コードに変更を加えることにより、それよりもより良い結果が得られます。このコードでは、1 クロックごとに加算を 1 回ずつ処理することは可能ですが、1 回に 32 ビット出力を 1 つしか処理できません。

```cpp
void vadd_sw(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t size)
{
    for (int i = 0; i < size; i++) {
       c[i] = a[i] + b[i];
    }
}
```

この時点では、このコードがプロセッサに勝ることはあり得ません。  FPGA ファブリックのクロックは、CPU クロックよりもかなり低速です。  この資料のはじめに紹介した比喩に戻ってたとえると、モノレールの各車両に 1 人しか乗客を乗せていないのと同じ状況です。  また、PCIe 上でのデータ転送、DMA のセットアップなどのオーバーヘッドもあります。次のいくつかの例では、この関数の入力および出力のバッファーを効率よく管理する方法を見ていきます。  Alveo データセンター アクセラレータ カードでのアクセラレーションの効果が見られるのはその後になります。

## キー コード

この例では、実際に FPGA 上で演算を実行してみます。  カード上で演算を実行するには、次の操作を実行する必要があります。

1. カードとのデータの送受信に使用するバッファーを割り当て、データを挿入します。
2. ホスト メモリ空間と Alveo グローバル メモリとの間でこれらのバッファーを転送します。
3. カーネルを実行してこれらのバッファーのデータを処理します。
4. カーネルでの処理結果をホスト メモリ空間に戻し、プロセッサからアクセスできるようにします。

これらの操作のうちカード上で実行されるのは 1 つだけです。メモリ管理はアプリケーションのパフォーマンスに大きく影響するので、まずメモリ管理から見てみます。

アクセラレーションに関する作業を実行したことがない場合、メモリの割り当てに `malloc()` または `new` への標準呼び出しを使用してしまいがちです。  この例では、これらの標準呼び出しを使用して、ホストと Alveo カードとの間でデータを転送する一連のバッファーを割り当てます。  4 つのバッファーを割り当てます。加算の入力に使用する 2 つの入力バッファー、Alveo で使用する出力バッファー、および `vadd` 関数のソフトウェア インプリメンテーションに使用する追加のバッファーです。  これにより、Alveo 用のメモリの割り当て方法と、プロセッサの実行効率に与える影響を見ることができます。

バッファーは次に示すように割り当てられています。  この例では、`BUFSIZE` は 24 MiB (`uint32_t` 型の 6 × 1024 × 1024) です。  ここに示していないコードは、前の例とまったく同じか、機能的に同等のコードです。

```cpp
uint32_t* a =new uint32_t[BUFSIZE];
uint32_t* b =new uint32_t[BUFSIZE];
uint32_t* c =new uint32_t[BUFSIZE];
uint32_t* d =new uint32_t[BUFSIZE];
```

これにより、**ページング方式**を使用する、**境界に揃えられていない**、**仮想**メモリが割り当てられます。  特に、「境界に揃えられていない」ことが後で問題になります。

バッファーを割り当て、初期テスト ベクター値を挿入したら、Alveo グローバル メモリに転送します。  これには、`CL_MEM_USE_HOST_PTR` フラグを使用して OpenCL バッファー オブジェクトを作成します。  これにより、API に対して、バッファーを割り当てる代わりにポインターが提供されます。これ自体は悪くありませんが、ポインターを割り当てる設定はしていないので、パフォーマンスの劣化を招きます。

次のコードには、割り当て済みのバッファーを OpenCL バッファー オブジェクトにマップするコードが含まれています。

```cpp
std::vector<cl::Memory> inBufVec, outBufVec;
cl::Buffer a_to_device(context,
                       static_cast<cl_mem_flags>(CL_MEM_READ_ONLY |
                                                 CL_MEM_USE_HOST_PTR),
                       BUFSIZE * sizeof (uint32_t),
                       a,
                       NULL);
cl::Buffer b_to_device (context,
                       static_cast<cl_mem_flags(CL_MEM_READ_ONLY |
                                                CL_MEM_USE_HOST_PTR),
                       BUFSIZE * sizeof(uint32_t),
                       b,
                       NULL);
cl::Buffer c_from_device(context,
                        static_cast<cl_mem_flags>(CL_MEM_WRITE_ONLY |
                                                  CL_MEM_USE_HOST_PTR),
                        BUFSIZE * sizeof(uint32_t),
                        c,
                        NULL);
inBufVec.push_back(a_to_device);
inBufVec.push_back(b_to_device);
outBufVec.push_back(c_from_device);
```

このコードでは、API で認識される `cl::Buffer` オブジェクトを割り当て、前の段階で割り当てたバッファーからポインター `a`、`b`、`c` を渡します。  `CL_MEM_READ_ONLY` および `CL_MEM_WRITE_ONLY` は、ランタイムでこれらのバッファーのカーネルからのアクセス権を指定します。  つまり、`a` および `b` はホストからカードに書き込まれますが、カーネルから見るとこれらは**読み取り専用**です。  その後、`c` がカードからホストにリードバックされます。  カーネルから見ると、これは**書き込み専用**です。  さらに、これらのバッファー オブジェクトをベクターに追加して、一度に複数のバッファーを転送できるようにします。実質的にはベクターにポインターを追加しているだけで、データ バッファーそのものを追加しているわけではありません。

次に、入力バッファーを Alveo カードに転送します。

```cpp
cl::Event event_sp;
q.enqueueMigrateMemObjects(inBufVec, 0, NULL, &event_sp);
clWaitForEvents(1, (const cl_event *)&event_sp);
```

このコード抜粋では、主要なイベントは 108 行目の enqueueMigrateMemObjects() への呼び出しです。  バッファーのベクターに 0 を渡しており、これがホストからデバイスへの転送であることを示します。`cl::Event` オブジェクトも渡します。

ここで、同期化について少し説明しておきましょう。転送をエンキューするということは、その転送をランタイムの「やるべきことリスト」に追加することですが、ランタイムはその転送が完了するのを待つわけではありません。  `cl::Event` オブジェクトを登録することにより、今後いつでもそのイベントが発生したときに待機するようにできます。  通常ここは待機するところではありませんが、さまざまな操作にかかった時間を簡単に表示できるようにするため、コード内のさまざまな時点に挿入しています。  これによりアプリケーションにわずかなオーバーヘッドが追加されますが、これは演習であって、最大パフォーマンスを得るために最適化する例ではありません。

ここで、カーネルに何を渡すのかをランタイムに指示する必要があるので、次のコードに示すようにします。  引数リストは次のとおりです。

`(uint32_t*a, uint32_t*b, uint32_t*c, uint32_t size)`

この場合、`a` は引数 0、`b` は引数 1、のようになります。

```cpp
krnl.setArg(0, a_to_device);
krnl.setArg(1, b_to_device);
krnl.setArg(2, c_from_device);
krnl.setArg(3, BUFSIZE);
```

次に、コマンド キューにカーネル自体を追加し、実行が開始するようにします。  通常は、転送とカーネルの間に同期化するのではなく、連続実行されるようにエンキューします。  カーネルの実行をコマンド キューに追加する行は、次のようになります。

```cpp
q.enqueueTask(krnl, NULL, &event_sp);
```

この時点で待機しない場合は、`cl::Event` オブジェクトではなく `NULL` を渡します。

最後に、カーネルが処理を完了したら、メモリをホストに戻し、CPU から新しい値にアクセスできるようにします。  これは、次のように実行します。

```cpp
q.enqueueMigrateMemObjects(outBufVec, CL_MIGRATE_MEM_OBJECT_HOST, NULL, &event_sp);
clWaitForEvents(1, (const cl_event *)&event_sp);
```

この例では、同期化を待機します。  これらのエンキュー関数を呼び出すときに、エントリを**ノンブロッキング**方式でコマンド キューに配置するということに注意してください。  転送をエンキューした直後にバッファーにアクセスしようとすると、リードバックは完了しています。

例 0 から FPGA コンフィギュレーションを除外すると、カーネルを実行するために新しく追加するのは次のものです。

1. 通常の方法でバッファーを割り当てます。  これより良い方法がありますが、アクセラレーションを初めて試す多くの人がこの方法を使用します。
2. 割り当て済みバッファーを cl::Buffer オブジェクトにマップします。
3. 入力バッファー (a および b) の Alveo デバイス グローバル メモリへの移動をエンキューします。
4. カーネル引数、バッファーおよびスカラー値の両方を設定します。
5. カーネルを実行します。
6. カーネルの結果を CPU のホスト メモリにリードバックし、読み出しの完了を同期化します。

これが実際のアプリケーションであれば、同期化は 1 回のみ必要です。  前述のように、この例では、ワークフローのさまざまな操作のタイミングをより良くレポートするため、いくつかの同期化を使用しています。

## アプリケーションの実行

XRT ランタイムが初期化されたら、ビルド ディレクトリから次のコマンドを実行してアプリケーションを実行します。

`./01_simple_malloc alveo_examples`

プログラムにより、次のようなメッセージが表示されます。

```
-- Example 1: Vector Add with Malloc() --

Loading XCLBin to program the Alveo board:

Found Platform
Platform Name: Xilinx
XCLBINFile Name: alveo_examples
INFO: Importing ./alveo_examples.xclbin
Loading: ./alveo_examples.xclbin
Running kernel test with malloc()ed buffers
WARNING: unaligned host pointer 0x154f7909e010 detected, this leads to extra memcpy
WARNING: unaligned host pointer 0x154f7789d010 detected, this leads to extra memcpy
WARNING: unaligned host pointer 0x154f7609c010 detected, this leads to extra memcpy

Simple malloc vadd example complete!

--------------- Key execution times ---------------
OpenCL Initialization:              247.371 ms
Allocating memory buffer:           0.030 ms
Populating buffer inputs:           47.955 ms
Software VADD run:                  35.706 ms
Map host buffers to OpenCL buffers: 64.656 ms
Memory object migration enqueue:    24.829 ms
Set kernel arguments:               0.009 ms
OCL Enqueue task:                   0.064 ms
Wait for kernel to complete:        92.118 ms
Read back computation results:      24.887 ms
```

境界に揃えられていないポインターに関する警告がいくつか表示されています。  割り当てに関して何もしていないので、Alveo カードとの間で転送されるバッファーはどれも、Alveo DMA エンジンで必要な 4 KiB 境界に揃えられていません。  そのため、転送の前にバッファーの内容を境界に揃うようにコピーする必要がありますが、この操作には時間がかかります。

ここからは、これらの数値に注意します。  複数の実行間でレイテンシに多少の変動はありますが、各エリアの差に注目します。ベースラインは次のとおりです。

| 操作| 例 1
|----------|:----------:
| OCL の初期化| 247.371 ms
| バッファーの割り当て| 30 µs
| バッファーへの値の挿入| 47.955 ms
| ソフトウェア VADD| 35.706 ms
| バッファーのマップ| 64.656 ms
| バッファーの書き出し| 24.829 ms
| カーネル引数の設定| 9 µs
| カーネルの実行時間| 92.118 ms
| バッファーの読み込み| 24.887 ms
| ΔAlveo→CPU| -418.228 ms
| ΔAlveo→CPU (アルゴリズムのみ)| -170.857 ms

## 追加演習

この演習に、追加で次のことを試してみてください。

- 割り当てるバッファーのサイズを変えてみます。  バッファー サイズと個々の操作のタイミングとの間にどのような関係がありますか。どれも同じレートでスケーリングしますか。
- 各ステップ間の同期化を削除すると、実行時間にどのように影響しますか。
- Alveo からホストに最後にバッファーをコピーした後の同期化を削除すると、どうなりますか。

## 学習ポイント

- ここでもまた、FPGA コンフィギュレーション「税」を支払う必要があります。  これを補うため、実行時間を CPU よりも 250 ms 以上短縮する必要があります。  1 つのバッファーを処理するだけのこの単純な例では、CPU を上回るスピードを達成するのは無理です。
- 単純に割り当てられたメモリは、メモリのコピーに時間がかかるので、アクセラレータに移動するには適しません。  この後の例で、この影響を調べてみます。
- OpenCL はコマンド キューで機能します。  同期化の方法およびタイミングは開発者しだいですが、CPU がバッファーのデータにアクセスする前に同期しているようにするため、Alveo グローバル メモリからバッファーをリードバックするときに注意が必要です。

[**例 2:** 境界に揃えられたメモリ割り当て](./02-aligned-memory-allocation.md)

<p align="center"><sup>Copyright&copy; 2019-2021 Xilinx</sup></p>
<p align="center"><sup>この資料は 2021 年 1 月 22 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
