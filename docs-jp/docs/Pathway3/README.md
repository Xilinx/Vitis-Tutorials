<table>
 <tr>
   <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2019.2 Vitis™ アプリケーション アクセラレーション開発フローのチュートリアル</h1><a href="https://github.com/Xilinx/SDAccel-Tutorials/branches/all">SDAccel™ 開発環境 2019.1 チュートリアルを参照</a></td>
 </tr>
 <tr>
 <td align="center"><h1>アクセラレーション アプリケーションのビルドおよび実行の基本概念</h1>
 </td>
 </tr>
</table>

## 概要

次の演習では、Vitis™ コア開発キットを使用してアクセラレーションされたアプリケーションをビルドして実行するための基本的な概念を説明します。

## 開始前の注意点

このチュートリアルでは、次を使用します。

* BASH Linux シェル コマンド
* 2019.2 Vitis コア開発キット リリースおよび xilinx\_u200\_xdma\_201830\_2 プラットフォーム。必要であれば、その他のソフトウェア リリースおよびプラットフォームを使用するように変更できます。

> **重要:**
>
> * 例を実行する前に、[インストール](https://japan.xilinx.com/html_docs/xilinx2019_2/vitis_doc/vhc1571429852245.html)に示すように Vitis コア開発キットをインストールしておく必要があります。
> * ザイリンクス Alveo™ データセンター アクセラレータ カードでアプリケーションを実行する場合は、『Alveo データセンター アクセラレータ カード入門ガイド ([UG1301](https://japan.xilinx.com/cgi-bin/docs/bkdoc?k=accelerator-cards;v=latest;d=j_ug1301-getting-started-guide-alveo-accelerator-cards.pdf)) で説明されるように、カードとソフトウェア ドライバーを正しくインストールしてください。

## チュートリアル リファレンス ファイルの入手

1. リファレンス ファイルを入手するには、ターミナルに `git clone https://github.com/Xilinx/Vitis-Tutorials` と入力します。
2. `Pathway3` ディレクトリに移動し、`reference-files` ディレクトリにアクセスします。

## 次のステップ

演習は、次の順序で実行します。

1. [アプリケーションのビルド](./BuildingAnApplication.md): アプリケーションのホスト プログラムおよびハードウェア カーネルをビルドする方法を説明します。
2. [ソフトウェアおよびハードウェア エミュレーションの実行](./Emulation.md): アプリケーションでハードウェアおよびソフトウェア エミュレーションを実行します。
3. [プロファイルおよびトレース レポートの生成](./ProfileAndTraceReports.md): アプリケーションのパフォーマンスをより理解するためのプロファイリング レポートの生成方法について説明します。
4. [ハードウェアでの実行](./HardwareExec.md): 最後に、Alveo データセンター アクセラレータ カードでアプリケーションを実行します。</br>

<hr/>
<p align= center><b><a href="../../README.md">メイン ページに戻る</a> &mdash; <a href="../../docs/vitis-getting-started/README.md">入門コースの初めに戻る</a></b></p></br><p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>

この資料は表記のバージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。

