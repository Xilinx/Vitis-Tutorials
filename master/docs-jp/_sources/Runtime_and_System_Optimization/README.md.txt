<p align="right"><a href="https://xilinx.github.io/Vitis-Tutorials/master/docs/index.html">English</a> | <a>日本語</a></p>
<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis ™ - ランタイムおよびシステムの最適化チュートリアル</h1><a href="https://japan.xilinx.com/products/design-tools/vitis.html">xilinx.com の Vitis ™開発環境を参照</a></td>
 </tr>
</table>

### 設計チュートリアル

最適化されたアクセラレーション アプリケーションを開発する手法には、アプリケーションのアーキテクチャとハードウェア カーネルの開発の 2 つの段階があります。最初の段階では、どのソフトウェア関数を FPGA カーネルでアクセラレーションするか、どれくらいの並列処理が達成可能か、どのようにコード記述するかなど、アプリケーション アーキテクチャに関する重要事項を決定します。第 2 段階では、ソース コードを構築し、必要なコンパイラ オプションとプラグマを適用して、最適なパフォーマンス ターゲットを達成するのに必要なカーネル アーキテクチャを作成して、カーネルをインプリメントします。次の例は、この設計手法を実際のアプリケーションで使用する方法を示しています。

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>チュートリアル</b><td width="65%" align="center"><b>説明</b></tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/01-host-code-opt/README.md">XRT ホスト コードの最適化</a></td>
 <td>CPU ホスト コードを最適化して、ハードウェア アクセラレータとランタイム ソフトウェアの相互関係を最大限に活用する方法を説明します。</td>
 </tr>
 <tr>
 <td align="center"><a href="./Design_Tutorials/02-ivas-ml/README.md">IVAS を使用したストリーミング ビデオ解析</a></td>
 <td>Vitis および Vitis AI 1.2 で、ストリーミング ビデオを解析するためのザイリンクス IVAS フレームワークを使用したリファレンス プラットフォームをデモします。</td>
 </tr>
 </table>

### 機能チュートリアル

 <table style="width:100%">
 <tr>
 <td width="35%" align="center"><b>チュートリアル</b><td width="65%" align="center"><b>説明</b></tr>
 <tr>
 <td align="center"><a href="./Feature_Tutorials/01-mult-ddr-banks/README.md">複数 DDR バンクの使用</a></td>
 <td>複数の DDR を使用してカーネルとグローバル メモリ間のデータ転送を向上する方法を説明します。</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/02-using-multiple-cu/README.md">複数の計算ユニットの使用</a></td>
 <td>FPGA のカーネル インスタンス数を増加する柔軟なカーネル リンク プロセスを使用して、統合したホスト カーネル システムの並列処理を改善する方法を説明します。</td>
 </tr>
  <tr>
 <td align="center"><a href="./Feature_Tutorials/03-controlling-vivado-implementation/README.md">Vivado インプリメンテーションの制御</a></td>
 <td>プロジェクトをインプリメントする際に Vivado&reg; ツール フローを制御する方法を説明します。</td>
 </tr>
 </table>
<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
<p align="center"><sup>この資料は 2021 年 2 月 8 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
