<p align="right"><a href="../../../../README.md">English</a> | <a>日本語</a>
<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis ™ - ランタイムおよびシステムの最適化<br/>例 2: 境界に揃えられたメモリ割り当て</h1><a href="https://japan.xilinx.com/products/design-tools/vitis.html">xilinx.com の Vitis ™開発環境を参照</a></td>
 </tr>
</table>

## 概要

前の例では、単純にメモリを割り当てただけですが、DMA エンジンでは、バッファーが 4 KiB のページ境界に揃えられていることが必要です。  バッファーが境界に揃えられていない場合、ランタイムによりバッファーがその内容が境界に揃うようにコピーされます。

これは時間のかかる操作ですが、具体的にどの程度の時間がかかるのでしょうか。  そして、境界に揃えられたメモリはどのように割り当てることができるでしょうか。

## キー コード

この例は、バッファーを割り当てた例 1 のコードを 4 行変更するだけなので、比較的短いです。  境界に揃えられたメモリを割り当てるにはさまざまな方法がありますが、ここでは POSIX 関数 `posix_memalign()` を使用します。   この変更では、先ほどの割り当てを次に示すコードに置き換えます。また、`memory` のコード例には示されていない追加ヘッダーを含める必要もあります。

```cpp
uint32_t*a,*b,*c,*d = NULL;
posix_memalign((void**)&a, 4096, BUFSIZE*sizeof(uint32_t));
posix_memalign((void**)&b, 4096, BUFSIZE*sizeof(uint32_t));
posix_memalign((void**)&c, 4096, BUFSIZE*sizeof(uint32_t));
posix_memalign((void**)&d, 4096, BUFSIZE*sizeof(uint32_t));
```

`posix_memalign()` への呼び出しでは、前述のとおり、要求されたアライメント (この例では 4 KiB) を渡します。

変更はこれだけです。  CPU ベースラインの VADD 関数によってのみ使用されるバッファー `d` も含め、すべてのバッファーの割り当てが変更されています。  これがアクセラレータと CPU の両方のランタイム パフォーマンスに与える影響を確認してみましょう。

## アプリケーションの実行

XRT ランタイムが初期化されたら、ビルド ディレクトリから次のコマンドを実行してアプリケーションを実行します。

`./02_aligned_malloc alveo_examples`

プログラムにより、次のようなメッセージが表示されます。

```
-- Example 2: Vector Add with Aligned Allocation --

Loading XCLBin to program the Alveo board:

Found Platform
Platform Name: Xilinx
XCLBIN File Name: alveo_examples
INFO: Importing ./alveo_examples.xclbin
Loading: './alveo_examples.xclbin'
Running kernel test with aligned virtual buffers

Simple malloc vadd example complete!

--------------- Key execution times ---------------
OpenCL Initialization:                 256.254 ms
Allocating memory buffer:              0.055 ms
Populating buffer inputs:              47.884 ms
Software VADD run:                     35.808 ms
Map host buffers to OpenCL buffers :   9.103 ms
Memory object migration enqueue:       6.615 ms
Set kernel arguments:                  0.014 ms
OCL Enqueue task:                      0.116 ms
Wait for kernel to complete:           92.110 ms
Read back computation results:         2.479 ms
```

一見したところでは、パフォーマンスがかなり向上しているようです。  これらの結果を例 1 の結果と比較して、どのように変化したかを確認します。  次の表に詳細を示します。わかりやすくするため、実行ごとの変動は含めていません。

| 操作| 例 1| 例 2| Δ1→2
|----------|:----------:|:----------:|:----------:
| OCL の初期化| 247.371 ms| 256.254 ms| \-
| バッファーの割り当て| 30 µs| 55 µs| 25 µs
| バッファーへの値の挿入| 47.955 ms| 47.884 ms| \-
| ソフトウェア VADD| 35.706 ms| 35.808 ms| \-
| バッファーのマップ| 64.656 ms| 9.103 ms| -55.553 ms
| バッファーの書き出し| 24.829 ms| 6.615 ms| -18.214 ms
| カーネル引数の設定| 9 µs| 14 µs| \-
| カーネルの実行時間| 92.118 ms| 92.110 ms| \-
| バッファーの読み込み| 24.887 ms| 2.479 ms| -22.408 ms
| ΔAlveo→CPU| -418.228 ms| -330.889 ms| 87.339 ms
| ΔAlveo→CPU (アルゴリズムのみ)| -170.857 ms| -74.269 ms| 96.588 ms

良い結果です。  たった 4 行のコードを変更しただけで、実行時間を約 100 ms 短縮できました。それでもまだ CPU のほうが高速ですが、メモリの割り当て方法を少し変えただけで、非常に大きな改善が見られました。  これで、アライメントに必要なのはメモリ コピーだけになりました。バッファーを割り当てるとき、数マイクロ秒余分に時間をかけてバッファーが境界に揃えられるようすると、後でこれらのバッファーが使用されるときにその何十倍もの時間を節約できます。

このユース ケースでは、予想どおり、ソフトウェアの実行時間は同じです。  割り当てられたメモリのアライメントを変更しているだけで、ユーザー空間のメモリ割り当ては通常のものです。

## 追加演習

この演習に、追加で次のことを試してみてください。

- 割り当てるバッファーのサイズを変えてみます。  前の例から導かれた関係は、この例でも有効ですか。
- 境界が揃えられたメモリを割り当てる OCL API ではない別の方法でも試してみます。  実行ごとの変動以上の違いはありますか。

## 学習ポイント

- 境界に揃えられていないメモリを使用すると、パフォーマンスが低下します。  Alveo カードと共有するバッファーは、常に境界に揃えるようにします。

改善が見られたので、  今度は OpenCL API を使用してメモリを割り当ててみましょう。

[**例 3:** XRT を使用したメモリ割り当て](./03-xrt-memory-allocation.md)

<p align="center"><sup>Copyright&copy; 2019-2021 Xilinx</sup></p>
<p align="center"><sup>この資料は 2021 年 1 月 22 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
