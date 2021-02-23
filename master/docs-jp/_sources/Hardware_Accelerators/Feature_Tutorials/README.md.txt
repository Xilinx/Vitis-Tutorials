<p align="right"><a href="https://xilinx.github.io/Vitis-Tutorials/master/docs/index.html">English</a> | <a>日本語</a></p>
<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis™ - ハードウェア アクセラレータ チュートリアル</h1><a href="https://japan.xilinx.com/products/design-tools/vitis.html">xilinx.com の Vitis™ 開発環境を参照</a></td>
 </tr>
</table>

### Vitis ハードウェア アクセラレータの概要

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>チュートリアル</b><td width="65%" align="center"><b>内容</b></tr>
 <tr>
 <td align="center"><a href="../Introduction/README.md">Vitis ハードウェア アクセラレータの概要</a></td>
 <td>FPGA アクセラレーションの概念を説明し、Cholesky マトリックス分解アルゴリズムをインプリメントするハードウェア アクセラレータを徐々に最適化する方法を示します。</td>
 </tr>
 </table>

### 設計チュートリアル

最適化されたアクセラレーション アプリケーションを開発する手法には、アプリケーションのアーキテクチャとハードウェア カーネルの開発の 2 つの段階があります。最初の段階では、どのソフトウェア関数を FPGA カーネルでアクセラレーションするか、どれくらいの並列処理が達成可能か、どのようにコード記述するかなど、アプリケーション アーキテクチャに関する重要事項を決定します。第 2 段階では、ソース コードを構築し、必要なコンパイラ オプションとプラグマを適用して、最適なパフォーマンス ターゲットを達成するのに必要なカーネル アーキテクチャを作成して、カーネルをインプリメントします。次の例では、この方法を実際のアプリケーションで使用します。

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>チュートリアル</b><td width="65%" align="center"><b>内容</b></tr>
 <tr>
 <td align="center"><a href="../Design_Tutorials/02-bloom/README.md">ブルーム フィルターの例</a></td>
 <td>カーネルとホスト コードの最適化手法を組み合わせて、データ解析アプリケーションで 10 倍のスピードアップを達成する方法を説明します。</td>
 </tr>
 <tr>
 <td align="center"><a href="../Design_Tutorials/01-convolution-tutorial/README.md">たたみ込みの例</a></td>
 <td>ビデオ ストリームのリアルタイム処理に使用される 2D たたみ込みを解析および最適化するプロセスについて説明します。</td>
 </tr>
 <tr>
 <td align="center"><a href="../Design_Tutorials/03-rtl_stream_kernel_integration/README.md">RTL システム統合の例</a></td>
 <td>フリーランニング RTL カーネル、Vitis ライブラリ関数、カスタム Vitis HLS カーネルを実際のシステムに統合する方法を示します。</td>
 </tr>
 </table>
 
### 機能チュートリアル

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>チュートリアル</b><td width="65%" align="center"><b>内容</b></tr>
 <tr>
 <td align="center"><a href="./01-rtl_kernel_workflow/README.md">RTL カーネル入門</a></td>
 <td>Vitis コア開発キットを使用して RTL カーネルを FPGA にプログラムし、一般的な開発フローでハードウェア エミュレーションをビルドする方法を説明します。</td>
 </tr>
 <tr>
 <td align="center"><a href="./02-mixing-c-rtl-kernels/README.md">C と RTL の混合</a></td>
 <td>RTL および C カーネルを含むアプリケーションと、さまざまなデザイン解析機能を使用する方法を示します。</td>
 </tr>
 </table>

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
<p align="center"><sup>この資料は 2021 年 2 月 8 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
