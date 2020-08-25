<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ アプリケーション アクセラレーション開発フロー チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">2019.2 Vitis アプリケーション アクセラレーション開発フロー チュートリアル</a></td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# Alveo ポートフォリオ入門

## 概要

このチュートリアルでは、Alveo™ データセンター アクセラレータ カードについて学び、インストールを実行するために使用可能なさまざまなリソースを示します。

Alveo ポートフォリオの一部をサーバーにインストールする方法は、https://japan.xilinx.com/alveo を参照してください。このページには、Alveo ポートフォリオについて学ぶために必要なすべてのリソースへのリンクと、RedHat/CentOS および Ubuntu オペレーティング システムへのインストール方法が含まれます。

サーバーに Alveo カードを設置したら、アプリケーションをホスト サーバーおよびアクセラレータ カードで実行するよう運用できますが、アプリケーションを開発するには、Vitis コア開発キットをインストールする必要があります。Vitis コア開発キットには、ホスト アプリケーション用に最適化されたコンパイラ、ハードウェア カーネルのクロス コンパイラ、高度なデバッグ環境、パフォーマンスのボトルネックを見つけてアプリケーションを最適化するプロファイラーが含まれます。Vitis コア開発キットは、Alveo カードと同じサーバーにインストールする必要はありません。カードが設置されていないシステムにもインストールできます。

そのほか、[AWS](https://aws.amazon.com/ec2/instance-types/f1/) または [Nimbix](https://www.nimbix.net/alveo/) などのクラウド サーバーでアプリケーションを開発および運用することもできます。どちらにもハードウェア アクセラレーション コードを開発、シミュレーション、デバッグ、およびコンパイルするために必要なものがすべて含まれています。

## インストール

Alveo ポートフォリオをインストールする前に、ハードウェアおよびドライバー ソフトウェアのインストールの概要と、Alveo の概要を説明する [Alveo U200 および U250 入門 QuickTake ビデオ](https://japan.xilinx.com/cgi-bin/docs/ndoc?t=video;d=fpga/getting-started-with-alveo-u200-u250.html)を参照してください。

https://japan.xilinx.com/products/boards-and-kits/alveo.html の手順に従ってインストールします。最新のハードウェアおよびドライバー ソフトウェアのインストール手順が含まれており、Alveo ポートフォリオを使用してハードウェア アクセラレーションされたアプリケーションを運用できるようになります。次も含まれます。

* 最低限のシステム要件
* 検証されたサーバーのリスト
* Alveo ポートフォリオの概要
* カードの立ち上げおよび検証手順

アプリケーションを開発する場合は、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの[インストール](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=vhc1571429852245.html)の手順に従って、Vitis コア開発キットをインストールしてください。

## 次の手順

ソフトウェア アプリケーションのビルド、ハードウェア プラットフォームのビルド、エミュレーションの実行、レポートの表示、ハードウェアでの実行など、Vitis コア開発キットを使用した[アクセラレーション アプリケーションのビルドおよび実行の基本概念](../Pathway3/README.md)を学びます。</br>

<hr/>
<p align="center" class="sphinxhide"><b><a href="/README.md">メイン ページに戻る</a> &mdash; <a href="/docs/vitis-getting-started/README.md">入門コースの初めに戻る</a></b></p></br><p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>

<p align="center"><sup>この資料は表記のバージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>