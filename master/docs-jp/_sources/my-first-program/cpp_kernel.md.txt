<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ アプリケーション アクセラレーション開発フロー チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">2019.2 Vitis アプリケーション アクセラレーション開発フロー チュートリアル</a></td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# 1\. はじめての C++ カーネルの記述

この演習では、Vitis™ コア開発キット カーネルのコード要件を説明します。`reference-files/src` フォルダーに含まれる単純なベクター加算カーネル `vadd.cpp` を見てみます。

前述のように、Vitis コア開発キット アプリケーションには、ホスト CPU で実行され、ザイリンクス FPGA で実行される 1 つまたは複数のアクセラレータと通信するソフトウェア プログラムが含まれます。ホスト プログラムは、API 呼び出しを使用してデータを FPGA と転送し、アクセラレータと通信します。データ転送は、ホスト メモリから FPGA アクセラレーション カードのデバイス メモリにデータをコピーすると発生します。デバイス メモリは、複数のメモリ バンクに分割されます。アクセラレータ (カーネルとも呼ばれる) には、これらのメモリ バンクに接続可能で、ホスト プログラムと共有するデータにアクセス可能なメモリ インターフェイスが含まれます。

この演習では、次を実行します。

1. カーネルに必要なインターフェイス要件について学びます。
2. 関数引数がどのようにインターフェイスにマップされるかを理解します。
3. VADD 関数を Vitis コア開発キットでカーネルと互換するように編集します。

## カーネルのインターフェイス要件

関数をカーネルに変更する前に、まずカーネル インターフェイス要件を理解する必要があります。C++ 関数を Vitis コア開発キットでカーネルとして使用するには、特定のインターフェイス要件を満たす必要があります。すべてのカーネルに、次のハードウェア インターフェイスが必要です。

- 制御レジスタ (カーネルを開始および停止) にアクセスし、スカラー引数を渡すのに使用される 1 つの [AXI4-Lite](https://japan.xilinx.com/products/intellectual-property/axi.html#details) スレーブ インターフェイス。
- 次のインターフェイスの少なくとも 1 つ (カーネルには両方のインターフェイスを使用可能):
  - メモリと通信するための [AXI4 マスター](https://japan.xilinx.com/products/intellectual-property/axi.html#details) インターフェイス。
  - カーネル間またはホストとカーネル間でデータを転送するための [AXI4-Stream](https://japan.xilinx.com/products/intellectual-property/axi.html#details) インターフェイス。

カーネルを実行するには、次の条件に従っている必要があります。

- スカラー引数が AXI4-Lite スレーブ インターフェイスを介してカーネルに渡される。
- ポインター引数が AXI4-Lite スレーブ インターフェイスを介してカーネルに渡される。
- ポインター データがグローバル メモリ (DDR または PLRAM) に含まれる。
- カーネルが 1 つまたは複数の AXI4 メモリ マップド インターフェイスを介して、グローバル メモリ内のポインター データにアクセスする。
- カーネルがカーネルの AXI4-Lite インターフェイスの制御信号を介してホスト プログラムにより開始される。

## VADD の例

### C コードの構造

次のコードに、ポインターとスカラー引数のみを使用する `vadd.cpp` の VADD 関数のカーネル シグネチャを示します。

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

**重要:** C/C++ で記述されたカーネルは、デフォルトで関数引数の転送に使用される物理インターフェイスに自動的にマップされます。また、コンパイラでは、コードに組み込まれたプラグマから各ポート用に生成する物理インターフェイスが決定されます。関数が有効な C/C++ カーネルとして処理されるようにするには、有効なインターフェイス (デフォルトまたはプラグマで指定) が必要です。

#### ポインター引数

関数インターフェイスの 1 つの引数は、2 つのカーネル ポートになります。このため、ポインター引数にはそれぞれ 2 つのインターフェイス プラグマが必要です。

* 1 つ目のポートでは、カーネルがグローバル メモリのデータにアクセスします。このポートは、AXI マスター インターフェイス (`m_axi`) にマップする必要があります。
* 2 つ目のポートでは、データのベース アドレスがホスト プログラムによりカーネルに渡されます。このポートは、カーネルの AXI4-Lite スレーブ インターフェイス (`s_axilite`) にマップする必要があります。
* デフォルトでは、引数 `in1`、`in2`、および `out` は、最初のポートは `m_axi` インターフェイスにマップされ、2 番目のポートは `s_axilite` インターフェイスにマップされます。

このデフォルトの割り当ては、引数 `in1`、`in2`、および `out` に次のプラグマを指定するのと同じです。このデフォルトの割り当てを変更するには、次のプラグマを指定します。

```C++
#pragma HLS INTERFACE m_axi     port=in1 offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi     port=in2 offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi     port=out offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=in1              bundle=control
#pragma HLS INTERFACE s_axilite port=in2              bundle=control
#pragma HLS INTERFACE s_axilite port=out              bundle=control
```

`m_axi` インターフェイス プラグマは、AXI マスター ポートの特性を指定するために使用します。

* `port`: AXI メモリマップド インターフェイスにマップされる引数の名前を指定します。
* `offset=slave`: カーネルの AXI4-Lite スレーブ インターフェイスを介してポインターのベースのアドレスを使用できることを示します。
* `bundle`: `m_axi` インターフェイスの名前を指定します。この例の場合、3 つの引数が `gmem` という 1 つの AXI インターフェイスにマップされています。

`s_axilite` インターフェイス プラグマは、AXI4-Lite ポートの特性を指定するために使用します。

#### スカラー

関数インターフェイスのスカラー引数は、カーネルの AXI4-Lite インターフェイス (`s_axilite`) にマップされる必要のあるポートになります。関数の `return` 値も同様に AXI4-Lite インターフェイスにマップされる必要があります。

VADD デザインでは、引数 `size` および `return` 値は自動的に `s_axilite` インターフェイスにマップされます。デフォルトの割り当てを変更するには、次のプラグマを指定します。

```C++
#pragma HLS INTERFACE s_axilite port=size              bundle=control
#pragma HLS INTERFACE s_axilite port=return            bundle=control
```

カーネルには AXI4-Lite インターフェイス 1 つのみを含めることができるので、すべての `s_axilite` プラグマで同じ `bundle` 名を使用する必要があります。この例では、AXI4-Lite インターフェイスに `control` という名前を使用します。

HLS INTERFACE プラグマの詳細は、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの [HLS プラグマ](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=hlspragmas.html)を参照してください。

### C リンケージの使用

インターフェイス プラグマを追加するのに加え、関数を extern "C" { ... } で囲んでカーネルとしてコンパイルされるようにする必要があります。extern "C" を使用すると、コンパイラ/リンカーで C の命名規則および呼び出し規則が使用されます。

```C++
extern "C" {
void vadd(
        const unsigned int *in1, // Read-Only Vector 1
        const unsigned int *in2, // Read-Only Vector 2
        unsigned int *out,       // Output Result
        int size                 // Size in integer
        )
 {
```

これで、Vitis ツールチェーンを使用して `vadd` 関数がカーネルにコンパイルできるようになりました。

## 次の手順

次の手順では、[ホスト プログラムを記述](./host_program.md)します。</br>

<hr/>
<p align="center" class="sphinxhide"><b><a href="/docs/vitis-getting-started/README.md">入門コースの初めに戻る</a> &mdash; <a href="/docs/my-first-program/README.md">チュートリアルの初めに戻る</a></b></p>
<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
