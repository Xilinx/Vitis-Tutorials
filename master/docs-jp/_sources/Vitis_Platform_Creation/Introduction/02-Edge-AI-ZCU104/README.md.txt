<!--
# Copyright 2020 Xilinx Inc.
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
<p align="right"><a href="https://xilinx.github.io/Vitis-Tutorials/master/docs/index.html">English</a> | <a>日本語</a></p>

# ZCU104 での Vitis カスタム エンベデッド プラットフォームの作成

**バージョン: Vitis 2020.2 + Vitis AI 1.3**

このモジュールでは、ZCU104 用のカスタム Vitis エンベデッド プラットフォームを作成します。このプラットフォームでは、**Vitis AI アプリケーション**を含む **Vitis アクセラレーション アプリケーション**を実行できます。もちろん、一般的な**エンベッド ソフトウェア アプリケーション**もこのプラットフォームで実行できます。

ZCU104 は検証済みのザイリンクス評価ボードであり、Vivado にプロセッシング システムのプリセットが含まれているため、前回の概要モジュールで述べた手順 0 をスキップして、ハードウェアのビルドおよび XSA の生成から開始します。

ソフトウェア コンポーネントを準備するため、PetaLinux プロジェクトに XSA をインポートし、いくつかオプションを設定します。ソフトウェアおよびハードウェアのコンポーネントをビルドした後、プラットフォームをパッケージします。最後に、このカスタマイズされたプラットフォームをテストするため、このプラットフォームで Vitis AI アプリケーションを含むテスト アプリケーションをいくつか実行します。

## カスタム プラットフォーム プランニング

作成するプラットフォームでは、最終的に次のアプリケーションを実行できるようにします。

- Vector Addition などの Vitis アクセラレーションの例
- Vitis AI アプリケーション

Vector Addition アプリケーションには、カーネル制御用に 1 x クロック、1 x 割り込み、1 x M\_AXI、メモリ アクセス用に 1 x S\_AXI が必要です。

Vitis AI アプリケーションでは、DPU が RTL カーネルとして統合されます。clk と clk2x の 2 つのクロックが必要です。200 MHz と 400 MHz のクロックを供給でき、タイミング クロージャを簡単に達成できます。割り込みも 1 つ必要です。DPU はメモリを多く必要とする IP なので、プラットフォームには複数の AXI HP インターフェイスを使用する必要があります。

ソフトウェア側では、プラットフォームで XRT、ZOCL パッケージを供給する必要があります。ホスト アプリケーションでは、XRT OpenCL API を使用してカーネルを制御できます。Vitis AI ソフトウェア フレームワークでは、XRT を使用して DPU を制御できます。ZOCL は、アクセラレーション カーネルと通信するカーネル モジュールで、デバイス ツリー ノードが必要なので、追加します。ほかの Vitis AI の依存関係も追加します。これらすべての機能は、ザイリンクスが提供する共通イメージで達成できます。これらの共通機能のほかに、GCC コンパイラを追加してアプリケーションのネイティブ コンパイルを実行し、Vitis AI デモ アプリケーション用の mesa-mega ドライバーを追加し、ネットワークがより高速に動作するようにするため dropbear を openssh に置き換える必要があります。

## 詳細手順チュートリアル

次のページから、プラットフォーム作成手順を示します。各ページで、プラットフォーム作成プロセスの主な手順が 1 つずつ説明されています。

- [手順 1: Vivado ハードウェア デザインの作成および XSA の生成](./step1.md)
- [手順 2: PetaLinux でのソフトウェア コンポーネントの作成](./step2.md)
- [手順 3: Vitis プラットフォームの作成](./step3.md)
- [手順 4: プラットフォームのテスト](./step4.md)

**まずは、[手順 1: Vivado デザイン](./step1.md)から始めます。**

## 参考資料

- [『Vitis 統合ソフトウェア プラットフォーム: アプリケーション アクセラレーション開発』 (UG1393)](https://japan.xilinx.com/html_docs/xilinx2020_2/vitis_doc/index.html)
  - [プラットフォーム作成の一般規則](https://japan.xilinx.com/html_docs/xilinx2020_2/vitis_doc/vcm1596051749044.html)
  - [Vitis 環境の設定](https://japan.xilinx.com/html_docs/xilinx2020_2/vitis_doc/settingupvitisenvironment.html)
  - [ザイリンクス ランタイム (XRT) のインストール](https://japan.xilinx.com/html_docs/xilinx2020_2/vitis_doc/pjr1542153622642.html)
- プラットフォーム例
  - [Vitis エンベデッド プラットフォーム ソース GitHub リポジトリ](https://github.com/Xilinx/Vitis_Embedded_Platform_Source)の [ZCU102](https://github.com/Xilinx/Vitis_Embedded_Platform_Source/tree/2020.2/Xilinx_Official_Platforms/zcu102_base) および [ZCU104](https://github.com/Xilinx/Vitis_Embedded_Platform_Source/tree/2020.2/Xilinx_Official_Platforms/zcu104_base) ベース プラットフォームのソース コード
- [ザイリンクス ランタイム (XRT)](https://xilinx.github.io/XRT/master/html/index.html)
- [Vitis-AI GitHub リポジトリ](https://github.com/Xilinx/Vitis-AI)

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
<p align="center"><sup>この資料は 2021 年 2 月 8 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
