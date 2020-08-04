<table>
 <tr>
   <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2019.2 Vitis™ アプリケーション アクセラレーション開発フローのチュートリアル</h1><a href="https://github.com/Xilinx/SDAccel-Tutorials/branches/all">SDAccel™ 開発環境 2019.1 チュートリアルを参照</a></td>
 </tr>
 <tr>
 <td align="center"><h1>はじめてのプログラム</h1>
 </td>
 </tr>
</table>

# 1\. はじめての C++ カーネルの記述

この演習では、Vitis™ コア開発キット カーネルのコード要件について説明します。確認するのは、`reference-files/src` フォルダーに含まれる[ベクター加算カーネル](./reference-files/src/vadd.cpp)ファイルです。

前述したように、Vitis コア開発キット アプリケーションには、ホスト CPU で実行され、ザイリンクス FPGA で実行される 1 つまたは複数のアクセラレータと交信するソフトウェア プログラムが含まれます。ホスト プログラムは API 呼び出しを使用してデータを FPGA から出し入れして、アクセラレータと交信します。データ転送は、ホスト メモリから FPGA アクセラレーション カードのデバイス メモリにデータをコピーすると発生します。デバイス メモリは、異なるメモリ バンクにさらに分割されます。アクセラレータ (カーネルとも呼ばれる) には、これらのメモリ バンクに接続可能で、ホスト プログラムと共有するデータにアクセス可能なメモリ インターフェイスが含まれます。

この演習では、次を実行します。

1. カーネルに必要なインターフェイス要件について学びます。
2. 関数引数がどのようにインターフェイスへマップされるかを理解します。
3. Vitis コア開発キットで VADD 関数を編集してカーネルで使用可能にします。

## カーネルのインターフェイス要件

関数をカーネルに変更する前に、まずカーネル インターフェイス要件について理解する必要があります。C++ 関数はすべて特定のインターフェイス要件を満たしていないと、Vitis コア開発キットでカーネルとして使用できません。すべてのカーネルに、次のハードウェア インターフェイスが必要です。

- 制御レジスタ (カーネルを開始および停止) にアクセスし、スカラー引数を渡すのに使用される 1 つの [AXI4-Lite](https://japan.xilinx.com/products/intellectual-property/axi.html#details) スレーブ インターフェイス。
- 次のインターフェイスの少なくとも 1 つ (カーネルには両方のインターフェイスを使用可能):
  - メモリと通信するための [AXI4 マスター](https://japan.xilinx.com/products/intellectual-property/axi.html#details) インターフェイス。
  - カーネル間やホストとカーネル間でデータを転送するための [AXI4-Stream](https://japan.xilinx.com/products/intellectual-property/axi.html#details) インターフェイス。

カーネルは、次の機能および条件で実行できるようになります。

- スカラー引数が AXI4-Lite スレーブ インターフェイスを介してカーネルに渡さる。
- ポインター引数が AXI4-Lite スレーブ インターフェイスを介してカーネルに渡される。
- ポインター データがグローバル メモリ (DDR または PLRAM) に含まれる。
- カーネルが 1 つまたは複数の AXI4 メモリ マップド インターフェイスを介して、グローバル メモリ内のポインター データにアクセス。
- カーネルがカーネルの AXI4-Lite インターフェイスの制御信号を介してホスト プログラムにより開始される。

## VADD の例

### C コードの構造

次のコードは、[vadd.cpp](./reference-files/src/vadd.cpp) の VADD 関数のカーネル シグネチャを示しており、ポインターとスカラー引数のみが使用されています。

```C++
        void vadd(
                const unsigned int *in1, // Read-Only Vector 1 - Pointer arguments
                const unsigned int *in2, // Read-Only Vector 2 - Pointer arguments
                unsigned int *out,       // Output Result      - Pointer arguments
                int size                 // Size in integer    - Scalar arguments
                )
        {
```

### 関数引数のマップ

**重要:** C/C++ で記述されたカーネルには、デフォルトでは関数引数の転送に使用される物理インターフェイスに関する固有の前提はありません。コンパイラは、コードに組み込まれたプラグマに従って、どの物理インターフェイスを使用して各ポート用に生成するか決定します。関数が有効な C/C++ カーネルとして処理されるようにするには、各関数引数に有効なインターフェイス プラグマが必要です。

#### ポインター引数

関数インターフェイスの 1 つの引数は、2 つのカーネル ポートになります。このため、ポインター引数にはそれぞれ 2 つのインターフェイス プラグマが必要です。

* 1 つ目のポートでは、カーネルがグローバル メモリのデータにアクセスします。このポートは、AXI マスター インターフェイス (`m_axi`) にマップする必要があります。
* 2 つ目のポートでは、データのベース アドレスがホスト プログラムによりカーネルに渡されます。このポートは、カーネルの AXI4-Lite スレーブ インターフェイス (`s_axilite`) にマップする必要があります。

VADD カーネルの場合、引数 `in1`、`in2`、および `out` に次のプラグマが必要です。

```C++
#pragma HLS INTERFACE m_axi     port=in1 offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi     port=in2 offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi     port=out offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=in1              bundle=control
#pragma HLS INTERFACE s_axilite port=in2              bundle=control
#pragma HLS INTERFACE s_axilite port=out              bundle=control
```

AXI マスター ポートの特性化には、`m_axi` インターフェイス プラグマが使用されます。

* `port`: AXI メモリマップド インターフェイスにマップされる引数の名前を指定します。
* `offset=slave`: ポインターのベースのアドレスがカーネルの AXI -Liteスレーブ インターフェイスを介して使用できるようになったことを示します。
* `bundle`: `m_axi` インターフェイスの名前を指定します。この例の場合、3 つの引数が `gmem` という 1 つの AXI インターフェイスにマップされています。

AXI4-Lite ポートの特性化には、`s_axilite` インターフェイス プラグマが使用されます。

#### スカラー

関数インターフェイスのスカラー引数は、カーネルの AXI4-Lite インターフェイス (`s_axilite`) にマップされる必要のあるポートになります。関数の `return` は同じように AXI4-Lite インターフェイスにマップされる必要があります。

VADD デザインの場合、引数 `size` および `return` 値に次のプラグマが必要です。

```C++
#pragma HLS INTERFACE s_axilite port=size              bundle=control
#pragma HLS INTERFACE s_axilite port=return            bundle=control
```

カーネルには AXI4-Lite インターフェイス 1 つのみを含めることができるので、すべての `s_axilite` プラグマが同じ `bundle` 名を使用する必要があります。この例の場合、AXI4-Lite インターフェイスに `control` という名前を付けています。

HLS INTERFACE プラグマの詳細は、[HLS プラグマ](https://japan.xilinx.com/html_docs/xilinx2019_2/vitis_doc/Chunk538726301.html#okr1504034364623)を参照してください。

### C リンケージの使用

インターフェイス プラグマを追加するだけでなく、関数を extern "C" { ... } で囲んで、カーネルとしてコンパイルされるようにする必要があります。extern "C" を使用すると、コンパイラ/リンカーで C の命名規則および呼び出し規則が使用されます。

```C++
extern "C" {
void vadd(
        const unsigned int *in1, // Read-Only Vector 1
        const unsigned int *in2, // Read-Only Vector 2
        unsigned int *out,       // Output Result
        int size                 // Size in integer
        )
 {

#pragma HLS INTERFACE m_axi     port=in1  offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi     port=in2  offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi     port=out  offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=in1               bundle=control
#pragma HLS INTERFACE s_axilite port=in2               bundle=control
#pragma HLS INTERFACE s_axilite port=out               bundle=control
#pragma HLS INTERFACE s_axilite port=size              bundle=control
#pragma HLS INTERFACE s_axilite port=return            bundle=control
```

これで、Vitis ツールチェーンを使用して `vadd` 関数がカーネルにコンパイルできるようになりました。

## 次のステップ

このチュートリアルの次のステップは、[ホスト プログラムの記述](./host_program.md)です。</br>

<hr/>
<p align="center"><b><a href="../../docs/vitis-getting-started/">入門コースの初めに戻る</a> &mdash; <a href="./README.md">チュートリアルの初めに戻る</a></b></p>
<p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>
