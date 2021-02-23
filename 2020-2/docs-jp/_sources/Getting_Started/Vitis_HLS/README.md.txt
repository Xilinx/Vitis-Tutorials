<p align="right"><a href="https://xilinx.github.io/Vitis-Tutorials/master/docs/index.html">English</a> | <a>日本語</a></p>
<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis™ アプリケーション アクセラレーション チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/tree/2020.1">2020.1 チュートリアルを参照</a></td>
 </tr>
</table>
<!--
# Copyright 2021 Xilinx Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
-->

# Vitis HLS の解析および最適化

## 概要

Vitis 高位合成 (HLS) は、Vitis アプリケーション アクセラレーション開発フローの重要な部分です。このツールは、C/C++ および OpenCL コードをカーネルにコンパイルして、ザイリンクス デバイスのプログラマブル ロジック (PL) でのアクセラレーションに使用できるようにします。つまり、高位合成を実行して Vitis ツール用にハードウェア カーネルをコンパイルするツールです。

> **ヒント:** Vitis HLS を使用して C/C++ コードから Vivado IP を生成することもできますが、このチュートリアルでは実行しません。Vitis の XO カーネルと Vivado RTL IP の生成は、似ていますが、大きな違いもいくつかあります。このチュートリアルは、Vitis HLS ツールの一般的な概要を示すものです。

このチュートリアルでは、Vitis HLS ツールの GUI を使用してハードウェア カーネルをビルド、解析、最適化します。Vitis ツールでは Vitis カーネル フローを使用します。詳細は、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) の Vitis HLS フローの [Vitis カーネル フローのイネーブル](https://www.xilinx.com/cgi-bin/docs/rdoc?v=2020.2;t=vitis+doc;d=vitis_hls_process.html;a=uiy1584905571731)を参照してください。

### 開始前の確認事項

このチュートリアルでは、次を使用します。

* BASH Linux シェル コマンド。
* 2020.2 Vitis コア開発キット リリースおよび xilinx\_u200\_xdma\_201830\_2 プラットフォーム。必要であれば、ほかのバージョンおよびプラットフォームを使用するように変更することもできます。

> **重要:**
>
> * 例を実行する前に、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの[インストール](https://www.xilinx.com/html_docs/xilinx2020_2/vitis_doc/acceleration_installation.html#vhc1571429852245)の手順に従って、Vitis コア開発キットをインストールしてください。
> * ザイリンクス Alveo™ データセンター アクセラレータ カードでアプリケーションを実行する場合は、各 Alveo カードのリンク先の [\[資料] タブ](https://www.xilinx.com/products/boards-and-kits/alveo.html)に表示される資料で説明されるように、カードとソフトウェア ドライバーを正しくインストールしてください。

### Vitis を実行する環境の設定

Vitis を実行する環境を設定するには、次のスクリプトを実行して、特定のコマンド シェルで実行する環境を設定します。

```bash
source <Vitis_install_path>/Vitis/2020.2/settings64.sh
source /opt/xilinx/xrt/setup.sh
```

*注記: .csh スクリプトも提供されていますが、このチュートリアルでは bash シェルが使用されていることを前提としています。*

インストールしたデータセンター プラットフォームまたはエンベデッド プラットフォームのディレクトリを指定するには、次の環境変数を設定します。

```bash
export PLATFORM_REPO_PATHS=<path to platforms>
```

*注記: Ubuntu ディストリビューションによっては、Vitis を正しく設定するために LIBRARY\_PATH もエクスポートする必要があります。*

```bash
export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu
```

詳細は、[ザイリンクス アンサー 73698](https://www.xilinx.com/support/answers/73698.html) を参照してください。

### チュートリアル リファレンス ファイルの入手

1. リファレンス ファイルを入手するには、ターミナルに `git clone https://github.com/Xilinx/Vitis-Tutorials` と入力します。
2. `Getting_Started/Vitis_HLS` ディレクトリに移動し、`reference-files` ディレクトリにアクセスします。

## 次の手順

演習は、次の順序で実行します。

* [Vitis HLS プロジェクトの作成](./new_project.md)
* [高位合成の実行および結果の解析](./synth_and_analysis.md)
* [最適化テクニックの使用](./optimization_techniques.md)
* [DATAFLOW 最適化の確認](./dataflow_design.md)

<!--

1. [Creating the Vitis HLS Project](./new_project.md) - Create the project to specify the source code and testbench.

2. [Running High-Level Synthesis and Analyzing Results](./synth_and_analysis.md) - Simulate and synthesize the design, and analyze the results.

3. [Using Optimization Techniques](./optimization_techniques.md) - Try different optimization techniques to achieve the initiation interval (II)=1.

4. [Reviewing the Dataflow Optimization](./dataflow_design.md) - Add the Dataflow optimization to achieve even better results.

-->
</br><hr/>
<p align="center" class="sphinxhide"><b><a href="./README.md">メイン ページに戻る</a> &mdash; <a href="./../README.md">入門コースの初めに戻る</a></b></p>
<p align="center" class="sphinxhide"><sup>Copyright&copy; 2021 Xilinx</sup></p>
<p align="center"><sup>この資料は 2021 年 1 月 22 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
