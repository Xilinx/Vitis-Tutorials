<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ アプリケーション アクセラレーション開発フロー チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">2019.2 Vitis アプリケーション アクセラレーション開発フロー チュートリアル</a></td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# アクセラレーション アプリケーションのビルドおよび実行の基本概念

## 概要

次の演習では、Vitis コア開発キットを使用してアクセラレーション アプリケーションをビルドして実行するための基本的な概念を説明します。

## 開始前の確認事項

このチュートリアルでは、次を使用します。

* BASH Linux シェル コマンド。
* 2020.1 Vitis コア開発キット リリースおよび xilinx\_u250\_qdma\_201920\_1 プラットフォーム。必要であれば、その他のソフトウェア リリースおよびプラットフォームを使用するように変更できます。

> **重要:**
>
> * 例を実行する前に、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの[インストール](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=vhc1571429852245.html)の手順に従って、Vitis コア開発キットをインストールし、Alveo データセンター アクセラレータ カードを設置してください。
> * ザイリンクス Alveo™ データセンター アクセラレータ カードでアプリケーションを実行する場合は、[Alveo ポートフォリオ](https://japan.xilinx.com/products/boards-and-kits/alveo.html) ページの手順に従って、カードとソフトウェア ドライバーを正しくインストールしてください。

### チュートリアル リファレンス ファイルの入手

1. リファレンス ファイルを入手するには、ターミナルに `git clone https://github.com/Xilinx/Vitis-Tutorials` と入力します。
2. `Pathway3` ディレクトリに移動し、`reference-files` ディレクトリにアクセスします。

## 次の手順

演習は、次の順序で実行します。

* [アプリケーションのビルド](./BuildingAnApplication.md)
* [ソフトウェアおよびハードウェア エミュレーションの実行](./Emulation.md)
* [プロファイルおよびトレース レポートの生成](./ProfileAndTraceReports.md)
* [ハードウェアでの実行](./HardwareExec.md)

<!--
1. [Building an Application](./BuildingAnApplication.md): Learn how to build the host program and hardware kernel for an application.
2. [Running Software and Hardware Emulation](./Emulation.md): Run hardware and software emulation on an application.
3. [Generating Profile and Trace Reports](./ProfileAndTraceReports.md): Learn how to generate profiling reports to better understand the performance of an application.
4. [Executing in Hardware](./HardwareExec.md): Finally, execute an application on the Alveo Data Center accelerator card.
-->
</br><hr/>
<p align="center" class="sphinxhide"><b><a href="/README.md">メイン ページに戻る</a> &mdash; <a href="/docs/vitis-getting-started/README.md">入門コースの初めに戻る</a></b></p></br><p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>

<p align="center"><sup>この資料は表記のバージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>