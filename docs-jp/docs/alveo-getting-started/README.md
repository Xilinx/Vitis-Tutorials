<table>
 <tr>
   <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2019.2 Vitis™ アプリケーション アクセラレーション開発フローのチュートリアル</h1><a href="https://github.com/Xilinx/SDAccel-Tutorials/branches/all">SDAccel™ 開発環境 2019.1 チュートリアルを参照</a></td>
 </tr>
 <tr>
 <td align="center"><h1>入門コース</h1>
 </td>
 </tr>
</table>

# Alveo データセンター アクセラレータ カード入門

## 概要

このチュートリアルでは、ザイリンクス Alveo™ データセンター アクセラレータ カードを理解し、インストールを実行するのに使用可能なさまざまなリソースについて説明します。

サーバーに Alveo データセンター アクセラレータ カードをインストールする方法については、[Alveo ページ](http://japan.xilinx.com/alveo)を参照してください。このページには、Alveo データセンター アクセラレータ カードについて理解し、RedHat/CentOS および Ubuntu オペレーティング システムにインストールする方法を知るのに必要なすべてのリソースへのリンクが含まれます。

サーバーに Alveo データセンター アクセラレータ カードをインストールすると、アプリケーションを運用して、ホスト サーバーとアクセラレータ カードで実行されるようにできます。ただし、アプリケーションを開発するには、Vitis コア開発キットをインストールする必要があります。Vitis コア開発キットには、ホスト アプリケーション用に最適化されたコンパイラ、ハードウェア カーネル用のクロスコンパイラ、優れたデバッグ環境、およびプロファイラーなどが含まれ、パフォーマンスのボトルネックを見つけてアプリケーションを最適化できるようになっています。Vitis コア開発キットは、Alveo データセンター アクセラレータ カードと同じサーバーにインストールする必要はありません。  カードがインストールされていないシステムにもインストールできます。

そのほか、[AWS](https://aws.amazon.com/ec2/instance-types/f1/) または [Nimbix](https://www.nimbix.net/alveo/) などのクラウド サーバーでアプリケーションを開発および運用することもできます。どちらにもハードウェア アクセラレーション コードを開発、シミュレーション、デバッグ、およびコンパイルするために必要なものがすべて含まれています。

## Alveo データセンター アクセラレータ カードのインストール

Alveo データセンター アクセラレータ カードをインストールする前に、[QuickTake ビデオ](https://japan.xilinx.com/video/fpga/getting-started-with-alveo-u200-u250.html)からハードウェアとドライバー ソフトウェア インストールの概要と、Alveo データセンター アクセラレータ カードのホーム ページの簡単な説明をご覧になってください。

Alveo データセンター アクセラレータ カードのインストール方法は、『Alveo データセンター アクセラレータ カード入門』 ([UG1301](https://japan.xilinx.com/cgi-bin/docs/bkdoc?k=accelerator-cards;v=latest;d=j_ug1301-getting-started-guide-alveo-accelerator-cards.pdf)) を参照してください。この資料には、最新のハードウェアおとびドライバー ソフトウェア インストール方法と、Alveo データセンター アクセラレータ カードを使用したハードウェア アクセラレーションされたアプリケーションを運用する方法が含まれます。また、次も含まれます。

* 最低限のシステム要件
* 検証されたサーバーのリスト
* Alveo データセンター アクセラレータ カードの概要
* カードの立ち上げおよび検証ステップ

アプリケーションを社内開発する場合は、[インストール](https://japan.xilinx.com/html_docs/xilinx2019_2/vitis_doc/vhc1571429852245.html)の詳細なインストール方法に従って、Vitis コア開発キットをインストールします。

<!--For developing applications for AWS, see one of the following tutorials:

* [Getting Started on AWS F1 with SDAccel and C/C++ Kernels](/docs/aws-getting-started/CPP)
* [Getting Started on AWS F1 with SDAccel and RTL Kernels](/docs/aws-getting-started/RTL)-->
## 次のステップ

ソフトウェア アプリケーションのビルド、ハードウェア プラットフォームのビルド、エミュレーションの実行、レポートの表示、ハードウェアでの実行など、Vitis コア開発キットを使用した[アクセラレーションされたアプリケーションのビルドおよび実行の基本概念](../Pathway3/README.md)を理解</br>

<hr/>
<p align= center><b><a href="../../README.md">メイン ページに戻る</a> &mdash; <a href="../vitis-getting-started/README.md">入門コースの初めに戻る</a></b></p></br><p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>

この資料は表記のバージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。
