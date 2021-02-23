<p align="right"><a href="https://xilinx.github.io/Vitis-Tutorials/master/docs/index.html">English</a> | <a>日本語</a></p>
<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis™ アプリケーション アクセラレーション開発フロー チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/tree/2020.1">2020.1 Vitis アプリケーション アクセラレーション開発フロー チュートリアル</a></td>
 </tr>
</table>

# アクセラレーション FPGA アプリケーションの最適化: ブルーム フィルターの例

## 概要

FPGA 上でアプリケーションをアクセラレーションする手法は、複数の段階で構成されます。

- **アプリケーションの設計**: どのソフトウェア関数を FPGA カーネルにマップするか、どれくらい並列処理が必要か、どのように適用するかなど、アプリケーションのアーキテクチャに関する重要事項を決定します。
- **パフォーマンス目標を達成するためのアクセラレータの開発**: パフォーマンス目標を達成できるカーネル アーキテクチャを作成するため、ソース コードを変更し、プラグマを適用して、カーネルをインプリメントします。
- **ホスト コードの最適化**: アプリケーションのアクセス パターン、データの動き、CPU および FPGA のアイドル時間を調べ、パフォーマンス目標を満たすためにコードをアップデートします。

このチュートリアルでは、ベースライン アプリケーションから開始し、アプリケーションをプロファイリングしてハードウェア アクセラレーションの可能性を調べます。このチュートリアルのアプリケーションでは、検索プロファイルに基づいてユーザーの関心に一致するドキュメントを見つけるため、ドキュメントの入力ストリームを検索します。

ブルーム フィルター アプリケーションは、構造化されていない電子メールやテキスト ファイルのデータを検索し、特定のユーザーに関連したドキュメントを検索して通知を送信するなど、データ分析で使用されます。

## 開始前の確認事項

> **ヒント**: このチュートリアルには、約 2 時間かかります。

このチュートリアルでは、次を使用します。

* BASH Linux シェル コマンド。
* 2020.1 Vitis コア開発キット リリースおよび xilinx\_u200\_xdma\_201830\_2 プラットフォーム。必要であれば、ほかのバージョンおよびプラットフォームを使用するように変更することもできます。

このチュートリアルでは、設計したアクセラレータを FPGA 上で実行するので、ザイリンクス Alveo™ U200 データセンター アクセラレータ カードがこのチュートリアルを実行するために設定されていることが必要です。アクセラレータを実行するのに必要な複数の `xclbin` ファイルを生成するには 6 ～ 7 時間かかる可能性があるので、U200 カード用に生成済みの `xclbin` ファイルが提供されています。これらの生成済みファイルを使用するには、ハードウェア カーネルのビルド時またはハードウェアでのアクセラレータの実行時に、`SOLUTION=1` 引数を追加する必要があります。

> **重要:**
>
> * 例を実行する前に、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの[インストール](https://japan.xilinx.com/html_docs/xilinx2020_2/vitis_doc/acceleration_installation.html#vhc1571429852245)の手順に従って、Vitis コア開発キットをインストールしてください。
> * Alveo カードでアプリケーションを実行する場合は、[Alveo ポートフォリオ](https://japan.xilinx.com/products/boards-and-kits/alveo.html) ページの手順に従って、カードとソフトウェア ドライバーを正しくインストールしてください。

### チュートリアル リファレンス ファイルの入手

1. チュートリアルの内容を入手するには、ターミナルに `git clone http://github.com/Xilinx/Vitis-Tutorials` と入力します。

2. `Hardware_Accelerators/Design_Tutorials/02-bloom` ディレクトリに移動します。

   * `cpu_src` には、変更前のソース コードが含まれます。
   * `images` に、このチュートリアルの図を示します。
   * この演習で使用するコマンドは、`makefile` ディレクトリの `Makefile` で説明されています。異なるプラットフォームをターゲットとする場合は、`PLATFORM` 変数を使用します。
   * `reference_file` には、パフォーマンスを向上するために変更されたカーネルおよびホストに関連するファイルが含まれます。

3. 次のように、大きなファイルをコピーして抽出します。

   ```
   wget https://japan.xilinx.com/bin/public/openDownload?filename=xclbin_save.tar.gz -O xclbin_save.tar.gz
   tar -xvzf  xclbin_save.tar.gz
   ```

   **ヒント:** `xclbin_save` には、`make run` コマンドで `SOLUTION=1` を設定してハードウェア上で実行する際に使用可能な `xclbin` ファイルが含まれます。

### チュートリアルの概要

* [元のアプリケーションの概要](1_overview.md)
* [アクセラレーション パフォーマンスの体験](2_experience-acceleration.md)
* [デバイス アクセラレーション アプリケーションの構築](3_architect-the-application.md)
* [カーネルのインプリメント](4_implement-kernel.md)
* [ホストとカーネル間のデータの動きを解析](5_data-movement.md)
* [複数 DDR バンクの使用](6_using-multiple-ddr)

<!--
1. [Overview of the Original Application](1_overview.md): Provides a brief overview of the Bloom filter application with some examples of how this application is used in real-world scenarios.
2. [Experience Acceleration Performance](2_experience-acceleration.md): Profile the Bloom filter application and evaluate which sections are best suited for FPGA acceleration. You will also experience the acceleration potential by running the application first as a software-only version and then as an optimized FPGA-accelerated version.
3. [Architecting the Application](3_architect-the-application.md): In this lab, the original C++ based application computes scores for the documents using a Bloom filter. This lab also discusses setting realistic performance goals for an accelerated application. At the end of this lab, you will have a specification of the kernel based on the [Methodology for Accelerating Applications with the Vitis Software Platform](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=methodologyacceleratingapplications.html#wgb1568690490380).
4. [Implementing the Kernel](4_implement-kernel.md): Implement the kernel based on the specification from the previous lab, and run the compute part of the algorithm on the FPGA.
5. [Analyze Data Movement Between Host and Kernel](5_data-movement.md): Analyze the performance results of the kernel you generated. Working with a predefined FPGA accelerator, you learn how to optimize data movements between the host and FPGA, how to efficiently invoke the FPGA kernel, and how to overlap computation on the CPU and FPGA to maximize application performance.
6. [Using Multiple DDR Banks](6_using-multiple-ddr): Configure multiple DDR banks to improve the kernel performance.
-->
<hr/>
<p align= center class="sphinxhide"><b><a href="../../../README.md">メイン ページに戻る</a> &mdash; <a href="../../README.md/">ハードウェア アクセラレータ チュートリアルの初めに戻る</a></b></p></br><p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
<p align="center"><sup>この資料は 2021 年 2 月 8 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
