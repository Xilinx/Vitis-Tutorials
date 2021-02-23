<p align="right"><a href="../../../../README.md">English</a> | <a>日本語</a>
<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis ™ - ランタイムおよびシステムの最適化 - 例 6: OpenMP の使用</h1><a href="https://japan.xilinx.com/products/design-tools/vitis.html">xilinx.com の Vitis ™開発環境を参照</a></td>
 </tr>
</table>

## 概要

例 5 で演習は終わったと思ったかもしれませんが、そうではありません。   前述のように、vadd がプロセッサよりも高速になることはありません。  vadd は単純すぎるからです。CPU はデータを転送する必要がなく、ローカル キャッシュを使用できるので、スピードでは常に勝ります。

前のセッションでは良い結果が得られましたが、それはあくまで机上の結果にすぎません。  マーケティングの面では  ここで終わりにしたいところですが、  エンジニアリングの面では、お伝えすべきことがまだあります。

## キー コード

アルゴリズムが単純な場合、アクセラレータが CPU を上回ることはありません。  OpenMP を使用してプロセッサ ループを並列処理してみましょう。CPU コードにヘッダー omp.h を含め、コード code:06\_openmp に示すように OpenMP プラグマを適用します。

```cpp
void vadd_sw(uint32_t* a, uint32_t *b, uint32_t* c, uint32_t size)
{
#pragma omp parallel for
      for (inti = 0; i < size; i++) {
          c[i] = a[i] + b[i];
      }
}
```

やることはそれだけです。  GCC に渡す必要のあるコマンド ライン フラグがいくつかありますが、それらは CMake で自動的に処理されるので (OpenMP がインストールされている場合)、開発者は直接ビルドして実行できます。  この例のコードは、それ以外は例 5 のコードと同じです。

## アプリケーションの実行

XRT ランタイムが初期化されたら、ビルド ディレクトリから次のコマンドを実行してアプリケーションを実行します。

`./06_wide_processor alveo_examples`

プログラムにより、次のようなメッセージが表示されます。

```
-- Example 6: VADD with OpenMP --

Loading XCLBin to program the Alveo board:

Found Platform
Platform Name: Xilinx
XCLBIN File Name: alveo_examples
INFO: Importing ./alveo_examples.xclbin
Loading: ’./alveo_examples.xclbin’

-- Running kernel test with XRT-allocated contiguous buffers and wide VADD (16 values/clock), with software OpenMP

OCL-mapped contiguous buffer example complete!

--------------- Key execution times ---------------
OpenCL Initialization:              253.898 ms
Allocate contiguous OpenCL buffers: 907.183 ms
Map buffers to userspace pointers:  0.307 ms
Populating buffer inputs:           1188.315 ms
Software VADD run :                 157.226 ms
Memory object migration enqueue :   1.429 ms
Wait for kernel to complete :       618.231 ms
```

| 操作| 例 5| 例 6| Δ5→6
|----------|:----------:|:----------:|:----------:
| ソフトウェア VADD| 1166.471 ms| 157.226 ms| -1009.245 ms
| ハードウェア VADD (合計)| 692.172 ms| 618.231 ms| -73.94 ms
| ΔAlveo→CPU| -503.402 ms| 461.005 ms| 964.407 ms

アクセラレータの実行時間が変動しているのは、これらのテストが仮想クラウド環境で実行されていることが主な理由ですが、それはこの演習の焦点ではありません。

## 追加演習

この演習に、追加で次のことを試してみてください。

- ベクター加算でプロセッサのスピードを上回ることができるかどうか試してみます。
- OpenMP のさまざまなプラグマを試してみます。ハードウェア アクセラレータを上回るには、CPU コアがいくつ必要ですか。

## 学習ポイント

- 繰り返しになりますが、単純な O(N) では CPU を上回ることはできません。

それでも落胆する必要はありません。  それでは、実際の例を見てみましょう。

**[例 7: Vitis ビジョンを使用したイメージ サイズの変更](./07-image-resizing-with-vitis-vision.md)**

<p align="center"><sup>Copyright&copy; 2019-2021 Xilinx</sup></p>
<p align="center"><sup>この資料は 2021 年 1 月 22 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
