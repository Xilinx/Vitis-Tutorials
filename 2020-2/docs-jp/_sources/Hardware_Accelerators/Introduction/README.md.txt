<p align="right"><a href="https://xilinx.github.io/Vitis-Tutorials/master/docs/index.html">English</a> | <a>日本語</a></p>
<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis ™ - Alveo U50 のアクセラレーション チュートリアル</h1><a href="https://japan.xilinx.com/products/design-tools/vitis.html">xilinx.com の Vitis™ 開発環境を参照</a></td>
 </tr>
</table>

### Vitis ハードウェア アクセラレータ チュートリアルの概要

このチュートリアルは、3 つのセクションに分割されます。

* [**セクション 1**](./01-Workflows/README.md) (~ 10 分):
  + Vitis とホスト/カーネルの実例の概要
    * Vitis がどのようにして困難な部分を受け持ち、ユーザーがアプリケーション コードに集中できるようにしているかをお見せします。
* [**セクション 2**](./02-System_Setup/README.md) (Alveo U50 カードが既にインストール済みの場合は数分):
  + Vitis 開発ツールを設定します。
  + サーバーにインストールされたカードを見つけて確認します。
* [**セクション 3**](./03-Algorithm_Acceleration/README.md) (数時間):
  + Cholesky アルゴリズムを理解し、まず CPU 上で実行します。

  + コードを分類し直して、ホストとカーネル プログラムの両方を作成します。

  + ホストとカーネルをバインドする API を確認します。

  + 複数のモジュールにわたってカーネルにインクリメンタル最適化を適用して、スループットを向上させます。最後に、最適化されたアクセラレータを使用する Alveo カードをプログラミングし、パフォーマンスを確認します。

  + [**入門**](./03-Algorithm_Acceleration/docs/cpu_src/README.md)

    * Cholesky アルゴリズムの紹介
    * CPU バージョンの実行

  + [**Vitis モジュール 1**](./03-Algorithm_Acceleration/docs/module1_baseline/README.md)

    * 最初のカーネル デザインを使用し、パフォーマンス ベースラインを基準点として取得
    * Vitis を GUI モードまたは <code>make</code> で実行
    * Vitis アナライザーを実行して、アプリケーションのタイムラインを視覚化
    * Vitis HLS を実行して、カーネル コードのパフォーマンスとリソースを確認

  + [**Vitis モジュール 2**](./03-Algorithm_Acceleration/docs/module2_pipeline/README.md) (<code>PIPELINE</code> と <code>INTERFACE</code> の影響に焦点を当てた短いモジュール)

    * HLS の <code>PIPELINE</code> プラグマによる命令の並列化を理解
    * <code>INTERFACE</code> プラグマを適用して物理ポート アダプターを管理

  + [**Vitis モジュール 3**](./03-Algorithm_Acceleration/docs/module3_datatype/README.md)

    * <code>double</code> データ型よりもハードウェア効率の高い C++ の <code>float</code> データ型を使用するようにデザインを変更
    * Vitis、Vitis アナライザー、および Vitis HLS を実行

  + [**Vitis モジュール 4**](./03-Algorithm_Acceleration/docs/module4_dataflow/README.md)

    * <code>DATAFLOW</code> タスク並列最適化プラグマを適用
    * Vitis、Vitis アナライザー、および Vitis HLS を実行 (特定のデータフロー波形の表示を含む)
    * U50 カードで実行

</br><hr/>

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
<p align="center"><sup>この資料は 2021 年 2 月 8 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
