<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ アプリケーション アクセラレーション開発フロー チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">2019.2 Vitis アプリケーション アクセラレーション開発フロー チュートリアル</a></td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# はじめてのプログラム

## 概要

Vitis コア開発キット アプリケーションには、ホスト CPU で実行され、ザイリンクス FPGA で実行される 1 つまたは複数のアクセラレータと通信するソフトウェア プログラムが含まれます。このチュートリアルでは、ホストおよびアクセラレータ用のコードを記述し、Vitis コンパイラを使用してアプリケーションをビルドしてから、そのアプリケーションを実行してプロファイルする方法について説明します。このチュートリアルで使用されるアクセラレータは、ベクター加算関数です。

このチュートリアルで使用される makefile には、ホストおよびカーネルをコンパイル、リンク、実行するためのコマンド セットが含まれます。makefile からは、結果を表示するプロファイル機能を有効にすることもできます。

## 開始前の確認事項

このチュートリアルでは、次を使用します。

* BASH Linux シェル コマンド。
* 2020.1 Vitis コア開発キット リリースおよび xilinx\_u200\_xdma\_201830\_2 プラットフォーム。必要であれば、ほかのバージョンおよびプラットフォームを使用するように変更することもできます。

> **重要:**
>
> * 例を実行する前に、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの[インストール](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=vhc1571429852245.html)の手順に従って、Vitis コア開発キットをインストールしてください。
> * ザイリンクス Alveo™ データセンター アクセラレータ カードでアプリケーションを実行する場合は、[Alveo ポートフォリオ](https://japan.xilinx.com/products/boards-and-kits/alveo.html) ページの手順に従って、カードとソフトウェア ドライバーを正しくインストールしてください。

### チュートリアル リファレンス ファイルの入手

1. リファレンス ファイルを入手するには、ターミナルに `git clone https://github.com/Xilinx/Vitis-Tutorials` と入力します。
2. `my-first-program` ディレクトリに移動し、`reference-files` ディレクトリにアクセスします。

## 次の手順

演習は、次の順序で実行します。

* [はじめての C++ カーネルの記述](./cpp_kernel.md)
* [はじめてのホスト プログラムの記述](./host_program.md)
* [アプリケーションのコンパイル、リンク、実行](./building_application.md)
* [アプリケーションのプロファイリング](./profile_debug.md)

<!--
1. [Coding My First C++ Kernel](./my-first-program/cpp_kernel.md): Create the hardware kernel to run on the accelerator card.
2. [Coding My First Host Program](./my-first-program/host_program.md): Create the host program code, including making the required API calls to run the kernel.
3. [Compiling and Linking the Application](./my-first-program/building_application.md): Build the host program and hardware kernel.
4. [Profiling the Application](./my-first-program/profile_debug.md): Profile and optimize the application design.

-->
</br><hr/>
<p align="center" class="sphinxhide"><b><a href="/README.md">メイン ページに戻る</a> &mdash; <a href="/docs/vitis-getting-started/README.md">入門コースの初めに戻る</a></b></p></br><p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>

<p align="center"><sup>この資料は表記のバージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>