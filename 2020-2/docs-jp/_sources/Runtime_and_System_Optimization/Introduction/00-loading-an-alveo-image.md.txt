<p align="right"><a href="../../../../README.md">English</a> | <a>日本語</a>
<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis ™ - ランタイムおよびシステムの最適化<br/>例 0: Alveo イメージの読み込み</h1><a href="https://japan.xilinx.com/products/design-tools/vitis.html">xilinx.com の Vitis ™開発環境を参照</a></td>
 </tr>
</table>

## 概要

最初の例では、Alveo™ データセンター アクセラレータ カードにイメージを読み込みます。  システムに電源を投入すると、Alveo カードによりシェルが初期化されます。  このシェルによりホスト PC との接続がインプリメントされますが、ロジックのほとんどの部分はデザインをビルドできるようにブランクのキャンバスとなっています。このロジックをアプリケーションで使用するには、まずコンフィギュレーションする必要があります。

また、一部の演算には本質的にレイテンシの面でコストがかかります。  FPGA のコンフィギュレーションは、アプリケーション フローの中で最も時間のかかる作業です。  コストがどれぐらいかを知るため、イメージを読み込んでみます。

## キー コード

この例では、XRT 用に OpenCL ランタイム API を初期化し、コマンド キューを作成して、Alveo カードの FPGA をコンフィギュレーションします。  これは通常、1 回だけの操作です。カードがコンフィギュレーションされると、電源がオフになるまで、または別のアプリケーションでリコンフィギュレーションされるまで、コンフィギュレーションされたままになります。  1 つのカードに複数の独立したアプリケーションがハードウェアを読み込もうとすると、最初のアプリケーションが制御を解放するまでは次のアプリケーションはブロックされます。複数の独立したアプリケーションで、カード上で実行中の同じイメージを共有することはできます。

まず、次に示すように、ヘッダーを含める必要があります。  このアプリケーションで参照される完全なコードは、ファイル `design_source/sw_src/00_load_kernels.cpp` を参照してください。

```cpp
// Xilinx OpenCL and XRT includes
#include"xcl2.hpp"

#include <CL/cl.h>
```

この 2 つのうち、必要なのは `CL/cl.h` のみです。`xcl2.hpp` は、必要な初期化関数のラッパーとしてザイリンクスから提供されているヘルパー関数のライブラリです。

適切なヘッダーを含めたら、次のコードに示すように、コマンド キューを初期化し、バイナリ ファイルを読み込んで、FPGA にプログラムする必要があります。  これは、どのプログラムにもある時点で含める必要のある定型コードです。

```cpp
// This application will use the first Xilinx device found in the system
std::vector<cl::Device> devices = xcl::get_xil_devices();
cl::Device device               = devices[0];

cl::Context context(device);
cl::CommandQueue q(context, device);

std::stringdevice_name      = device.getInfo<CL_DEVICE_NAME>();
std::stringbinaryFile       = xcl::find_binary_file(device_name, argv[1]);
cl::Program::Binaries bins  = xcl::import_binary_file(binaryFile);

devices.resize(1);
cl::Program program(context, devices, bins);
```

このワークフローをまとめると、次のようになります。

1. システムでザイリンクス デバイスを検出し、番号を付けます。  ここではデバイス 0 をターゲット カードと想定できますが、マルチアクセラレータ カード システムでは、デバイスを指定する必要があります。
2. OpenCL コンテキストおよびコマンド キューを初期化します。
3. Alveo ボードをターゲットとするバイナリ ファイルを読み込みます。  これらの例ではコマンド ラインでファイル名を渡しますが、ファイル名はコードに記述するか、アプリケーションごとに処理することもできます。
4. FPGA をプログラムします。

44 行目で、プログラム操作が実際にトリガーされます。プログラム中に、ランタイムにより Alveo カードの現在の設定がチェックされます。  既にプログラムされている場合は、デバイスのメタデータを xclbin から読み込んだ後に戻ることができますが、  プログラムされていない場合は、ここでデバイスをプログラムします。

## アプリケーションの実行

XRT ランタイムが初期化されたら、ビルド ディレクトリから次のコマンドを実行してアプリケーションを実行します。

`./00_load_kernels alveo_examples`

プログラムにより、次のようなメッセージが表示されます。

```
-- Example 0: Loading the FPGA Binary --

Loading XCLBin to program the Alveo board:

Found Platform
Platform Name: Xilinx
XCLBIN File Name: alveo_examples
INFO: Importing ./alveo_examples.xclbin
Loading: ?./alveo_examples.xclbin?

FPGA programmed, example complete!

-- Key execution times --
OpenCL Initialization : 1624.634 ms
```

FPGA の初期化に 1.6 秒かかっています。このカーネルの読み込み時間には、ディスク I/O、PCIe レイテンシ、コンフィギュレーション オーバーヘッドなど、多数の操作が含まれます。通常は、アプリケーションの起動時に FPGA をコンフィギュレーションするか、あらかじめコンフィギュレーションしておきます。既に読み込まれているビットストリームを使用して、このアプリケーションをもう一度実行します。

```
-- Key execution times --
OpenCL Initialization : 262.374 ms
```

0.26 秒は 1.6 秒よりもかなり良い結果です。ディスクからファイルを読み出して解析し、FPGA に読み込まれた xclbin ファイルが正しいことを確認する必要がありますが、全体的な初期化時間は大幅に短縮されています。

## 追加演習

この演習に、追加で次のことを試してみてください。

1. **xbutil** ユーティリティを使用してボードをクエリします。  どの xclbin ファイルが読み込まれましたか。
2. もう一度 **xbutil** ユーティリティを使用し、FPGA にどのカーネルが存在するかを確認します。  FPGA のプログラム前後で違いがあるかを調べます。

## 学習ポイント

- FPGA コンフィギュレーションはコストのかかる操作です。  必要になる前に余裕をもって FPGA を初期化しておくのが理想的です。  これは、アプリケーションのほかの初期化タスクを実行しているときに別のスレッドで実行するか、専用システムのシステム ブート時に実行することも可能です。
- FPGA が読み込まれると、その後の読み込み時間は大幅に短縮されます。
- これで、FPGA にイメージを読み込めるようになったので、次に例を実行してみます。

[**例 1:** 単純なメモリ割り当て](./01-simple-memory-allocation.md)

<p align="center"><sup>Copyright&copy; 2019-2021 Xilinx</sup></p>
<p align="center"><sup>この資料は 2021 年 1 月 22 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
