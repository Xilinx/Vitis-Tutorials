<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ アプリケーション アクセラレーション チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">2019.2 Vitis アプリケーション アクセラレーション開発フロー チュートリアル</a></td>
 </tr>
 <tr>
 <td align="center"><h1></h1>
 </td>
 </tr>
</table>

# Vitis HLS の解析および最適化

## 概要

Vitis 高位合成 (HLS) は、Vitis アプリケーション アクセラレーション開発フローの重要な部分です。このツールは、C/C++ および OpenCL コードをカーネルにコンパイルして、ザイリンクス デバイスのプログラマブル ロジック (PL) でのアクセラレーションに使用できるようにします。つまり、高位合成を実行して Vitis ツール用にハードウェア カーネルをコンパイルするツールです。

このチュートリアルでは、Vitis HLS ツールの GUI を使用してハードウェア カーネルをビルド、解析、最適化します。Vitis ツールでは Vitis カーネル フローを使用します。詳細は、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) の Vitis HLS フローの [Vitis カーネル フローのイネーブル](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=creatingnewvitishlsproject.html;a=uiy1584905571731)を参照してください。

### 開始前の確認事項

このチュートリアルでは、次を使用します。

* BASH Linux シェル コマンド
* 2020.1 Vitis コア開発キット リリースおよび xilinx\_u200\_xdma\_201830\_2 プラットフォーム。必要であれば、ほかのバージョンおよびプラットフォームを使用するように変更することもできます。

> **重要:**
>
> * 例を実行する前に、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの[インストール](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=vhc1571429852245.html)の手順に従って、Vitis コア開発キットをインストールしてください。
> * ザイリンクス Alveo™ データセンター アクセラレータ カードでアプリケーションを実行する場合は、各 Alveo カードのリンク先の [[資料] タブ](https://japan.xilinx.com/products/boards-and-kits/alveo.html)に表示される資料で説明されるように、カードとソフトウェア ドライバーを正しくインストールしてください。

### チュートリアル リファレンス ファイルの入手

1. リファレンス ファイルを入手するには、ターミナルに `git clone https://github.com/Xilinx/Vitis-Tutorials` と入力します。
2. `vitis_hls_analysis` ディレクトリに移動し、`reference-files` ディレクトリにアクセスします。

## 次のステップ

演習は、次の順序で実行します。

* [Vitis HLS プロジェクトの作成](./new_project.md)
* [高位合成の実行および結果の解析](./synth_and_analysis.md)
* [最適化テクニックの使用](./optimization_techniques.md)
* [DATAFLOW 最適化の確認](./dataflow_design.md)
* [Vitis ツールでの HLS カーネルの使用](./using_the_kernel.md)

<!--

1. [Creating the Vitis HLS Project](./new_project.md) - Create the project to specify the source code and testbench.

2. [Running High-Level Synthesis and Analyzing Results](./synth_and_analysis.md) - Simulate and synthesize the design, and analyze the results.

3. [Using Optimization Techniques](./optimization_techniques.md) - Try different optimization techniques to achieve the initiation interval (II)=1.

4. [Reviewing the Dataflow Optimization](./dataflow_design.md) - Add the Dataflow optimization to achieve even better results.

5. [Using the HLS Kernel in the Vitis Tool](./using_the_kernel.md) - Use the Vitis integrated design environment (IDE) to create a new application project with the host and kernel code.

-->
</br><hr/>
<p align="center" class="sphinxhide"><b><a href="./README.md">メイン ページに戻る</a> &mdash; <a href="../../docs/vitis-getting-started/README.md">入門コースの初めに戻る</a></b></p></br><p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>

<p align="center"><sup>この資料は表記のバージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>