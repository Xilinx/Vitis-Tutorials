<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ アプリケーション アクセラレーション開発フロー チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">2019.2 Vitis アプリケーション アクセラレーション開発フロー チュートリアル</a></td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# アクセラレーション FPGA アプリケーションの最適化: たたみ込みの例

## 概要

最適化されたアクセラレーション アプリケーションを開発して必要なパフォーマンス目標を達成する手法には、アプリケーションの設計とアクセラレータの開発の 2 つの主な段階があります。

* 第 1 段階では、どのソフトウェア関数を FPGA カーネルでアクセラレーションするか、どれくらい並列処理が達成可能か、どのようにコード記述するかなど、アプリケーション アーキテクチャに関する重要事項を決定します。
* 第 2 段階では、ソース コードを構築し、必要なコンパイラ オプションとプラグマを適用して、パフォーマンス ターゲットを達成するのに必要なカーネル アーキテクチャを作成して、カーネルをインプリメントします。

このチュートリアルでは、ベースライン アプリケーションから開始し、アプリケーションをプロファイリングしてハードウェア アクセラレーションの可能性を調べます。このアプリケーションは、多くのオーディオ/ビデオ形式を再生、トランスコード、多重化および分離、フィルター可能な 2D たたみ込みフィルターを使用して、マルチチャネル RGB ビデオ ストリームを処理します。その後、ホスト プログラムおよびカーネルの両方でさまざまな最適化を実行します。このチュートリアルでは、次の最適化手法を使用します。

* メモリ転送の最適化
* 固定小数点型の適用
* データフローおよびストリーム
* ループ最適化

このチュートリアルでは、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの [Vitis 統合ソフトウェア プラットフォームでのアプリケーション アクセラレーション手法](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=methodologyacceleratingapplications.html)の手順に従って、CPU ベースのアプリケーションを最適化された FPGA アクセラレーション デザインに移行します。このチュートリアルを実行する際にこの資料も確認すると、より理解が深まります。

## 開始前の確認事項

このチュートリアルでは、手順を実行するマシンに FFmpeg フレームワークがインストールされている必要があります。ダウンロードして次のコマンドを実行します。

* CentOS の場合:

  ```
  sudo yum localinstall --nogpgcheck https://download1.rpmfusion.org/free/el/rpmfusion-free-release-7.noarch.rpm
  sudo yum install ffmpeg
  ```

* Ubuntu の場合:

  ```
  sudo apt update
  sudo apt install ffmpeg
  ffmpeg -version
  ```

このチュートリアルでは、次を使用します。

* BASH Linux シェル コマンド。
* 2020.1 Vitis コア開発キット リリースおよび xilinx\_u200\_xdma\_201830\_2 プラットフォーム。必要であれば、ほかのバージョンおよびプラットフォームを使用するように変更することもできます。
* 多くの手順および変数を含む `Makefile`。`Makefile` の構造および内容については、[makefile の理解](./HowToRunTutorial.md)を参照してください。

> **重要:**
>
> * 例を実行する前に、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの[インストール](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=vhc1571429852245.html)の手順に従って、Vitis コア開発キットをインストールしてください。
> * ザイリンクス Alveo™ データセンター アクセラレータ カードでアプリケーションを実行する場合は、[Alveo ポートフォリオ](https://japan.xilinx.com/products/boards-and-kits/alveo.html) ページの手順に従って、カードとソフトウェア ドライバーを正しくインストールしてください。

### チュートリアル リファレンス ファイルの入手

1. リファレンス ファイルを入手するには、ターミナルに `git clone http://github.com/Xilinx/Vitis-Tutorials` と入力します。
2. `convolution-tutorial` ディレクトリに移動し、`design` ディレクトリにアクセスします。

## 次の手順

次の演習では、既存のアプリケーションを FPGA アクセラレーションされたアプリケーションとして最適化するためのベスト プラクティスを説明します。このチュートリアルでは、複数の演習に分けて手法を示します。演習は、次の順序で実行することをお勧めします。

* [元のアプリケーションの評価](RunOriginalCode.md)
* [C アプリケーションから Vitis コア開発キット アプリケーションを作成](baseline.md)
* [メモリ転送の最適化](localbuf.md)
* [固定小数点データ型を使用した最適化](fixedtype.md)
* [データフローを使用した最適化](dataflow.md)
* [順不同キューと複数の計算ユニットの使用](multi-CU.md)
* [複数の計算ユニットを使用した QDMA ストリーミングの使用](qdma.md)
* [ハードウェアでのアクセラレータの実行](RunOnHardware.md)

<!--

1. [Evaluating the Original Application](RunOriginalCode.md): In this lab, the original C-based application is used to process the filtered video stream. This lab also discusses setting realistic performance goals for an accelerated application.
2. [Creating a Vitis Core Development Kit Application from the C Application](baseline.md): Convert the original C code into a host program and hardware kernel where the kernel is called by the host using the OpenCL™ API.
3. [Optimizing Memory Transfers](localbuf.md): Learn methods for optimizing the hardware kernel for improved memory access. You will learn how to use local cache to make efficient use of the FPGA bandwidth.
4. [Optimizing Using Fixed Point Data Types](fixedtype.md): Understand how data types affect design performance.
5. [Optimizing with Dataflow](dataflow.md): Improve the compute efficiency of your kernel, applying dataflow and streaming to improve the data-path in your kernel.
6. [Using Out-of-Order Queues and Multiple Compute Units](multi-CU.md): Modify the OpenCL API calls in the host program to allow for out-of-order task execution, and increase parallelism within the accelerator by synthesizing multiple kernels to perform the work.
7. [Using QDMA Streaming with Multiple Compute Units](qdma.md): Modify the design to use multiple CUs with streaming functionality based on the output from the previous lab.
8. [Running the Accelerator in Hardware](RunOnHardware.md): All the previous steps have been run in Hardware Emulation mode. Here you run the application on FPGA-based acceleration hardware.

-->
</br><hr/>
<p align="center" class="sphinxhide"><b><a href="./README.md">メイン ページに戻る</a> &mdash; <a href="/docs/vitis-getting-started/README.md">入門コースの初めに戻る</a></b></p></br><p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>

<p align="center"><sup>この資料は表記のバージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>