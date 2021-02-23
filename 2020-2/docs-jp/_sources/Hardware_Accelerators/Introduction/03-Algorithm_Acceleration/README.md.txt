<p align="right"><a href="https://xilinx.github.io/Vitis-Tutorials/master/docs/index.html">English</a> | <a>日本語</a></p>
<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ - アルゴリズム アクセラレーション</h1><a href="https://japan.xilinx.com/products/design-tools/vitis.html">xilinx.com の Vitis™ 開発環境を参照</a></td>
 </tr>
</table>

> **_このセクションで必要なもの:_** <code>./cpu\_src</code> の CPU バージョンのアルゴリズムを実行する導入リファレンス モジュール 1 つ、<code>./docs</code> ディレクトリの Alveo U50 モジュール 4 つ、各モジュールのローカル readme ファイルの手順

* [**はじめに — CPU 実行**](./docs/cpu_src/README.md): アルゴリズムの C++ インプリメンテーション
  + C++ の非アクセラレーション バージョンの Cholesky アルゴリズムを実行
* [**モジュール 1**](./docs/module1_baseline/README.md): デザインを設定し、パフォーマンスの**ベースライン**を確立
  + ザイリンクス デバイスにインプリメントされたカーネルへの接続に役立つホスト OpenCL API を理解
  + ソフトウェア レベル (sw\_emu) およびハードウェア レベル (hw\_emu) 両方でエミュレーションを使用して結果を確認
  + Vitis アナライザーでタイムライン トレースを視覚化して、パフォーマンスを評価
  + Vitis HLS を起動して、カーネルの最適化を確認
* [**モジュール 2**](./docs/module2_pipeline/README.md): このバージョンのコードで明示的に <code>**PIPELINE**</code> および <code>**INTERFACE**</code> 指示子を適用
  + これらのプラグマとそのデザインへの影響について説明
* [**モジュール 3**](./docs/module3_datatype/README.md): <code>double</code> 型のデータ型を <code>float</code> に変更
  + ハードウェア エミュレーションを実行してから、Vitis アナライザーおよび Vitis HLS を実行
  + デザインをインプリメントするのに必要な物理リソースおよびパフォーマンスへの影響を確認
* [**モジュール 4**](./docs/module4_dataflow/README.md): <code>double</code> に戻し、タスク並列処理プラグマで結果を向上
  + タスク並列処理最適化の <code>**DATAFLOW**</code> プラグマをイネーブルにするようにコードを変更
  + Vitis アナライザーを使用してパフォーマンスの向上を評価
  + Vitis HLS を使用して、dataflow で作成された新しいマイクロ アーキテクチャを確認
  + バイナリ (xclbin) を生成して、カードをプログラムして実際のパフォーマンスを測定

  </br><hr/>

  <p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
  <p align= center class="sphinxhide"><b><a href="../../../README.md">メイン ページに戻る</a> &mdash; <a href="../../README.md/">ハードウェア アクセラレータ チュートリアルの初めに戻る</a></b></p></br>
  <p align="center"><sup>この資料は 2021 年 2 月 8 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
  日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
