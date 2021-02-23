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
<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>Versal カスタム プラットフォーム作成チュートリアル</h1>
    </td>
 </tr>
</table>

**バージョン: 2020.2**

このモジュールでは、VCK190 評価ボードのアクセラレーション アプリケーションを実行するカスタム プラットフォームを作成します。

通常の Vitis アクセラレーション プラットフォーム設計は、ベースのブータブル デザインのビルドから開始してから、プラットフォームに必要なコンポーネントとプロパティを追加します。エラーの発生を防ぐため、各ステップを次の手順に進む前に検証する必要があります。次の図はこのワークフローを示しています。

![Vitis プラットフォーム作成の手順](images/platform_creation_steps.svg)

Vivado からは、プラットフォーム作成者がプラットフォームをすばやくビルドできるように、拡張可能なプラットフォームのデザイン例が提供されています。CIPS、NOC、DDR コントローラー、およびプラットフォーム プロパティのプリセットも追加されています。これにより、手順 0 と手順 1 のプラットフォーム作成プロセスが効率的になります。プラットフォーム ハードウェア デザインにコンポーネントを追加する場合は、デザイン例の作成後に追加できます (オプション)。

PetaLinux プロジェクトに XSA をインポートし、その他いくつかオプションを設定して、ソフトウェア コンポーネントを準備します。ソフトウェアおよびハードウェアのコンポーネントをビルドした後、プラットフォームをパッケージします。

各手順ごとに生成されたファイルをテストして、問題なく動作することを確認します。頻繁にテストをすることで、エラーが発生した場合にその原因を絞り込みやすくなります。最後に、このプラットフォームでテスト アプリケーションをいくつか実行し、このカスタマイズ済みのプラットフォームをテストします。

次の目次リンクを使用して、これらの手順を実行してください。

- [手順 0: ベースのブータブル デザインの作成](./step0.md)
- [手順 1: Vitis プラットフォームのハードウェア設定](./step1.md)
- [手順 2: PetaLinux でのソフトウェア コンポーネントの作成](./step2.md)
- [手順 3: Vitis プラットフォームの作成](./step3.md)
- [手順 4: プラットフォームのテスト](./step4.md)
- [よくある質問](faq.md)

## 要件

このチュートリアル デザインは、VCK190 プロダクション ボードおよび VCK190 ES1 ボードをサポートしています。

- Vitis 2020.2 は VCK190 プロダクション ボードをサポートしています。このボードを使用するために追加のライセンスや設定は必要ありません。
- VOK190 ES1 ボードを使用する場合は、Vitis 2020.2 で追加の EA ライセンス設定が必要です。

このデザインには、インターネットにアクセスできる Linux ホストマシンが必要です。Linux OS では、Vitis ソフトウェアと PetaLinux がサポートされる必要があります。

## 参考資料

- [『Vitis 統合ソフトウェア プラットフォーム: アプリケーション アクセラレーション開発』 (UG1393)](https://japan.xilinx.com/html_docs/xilinx2020_2/vitis_doc/index.html)
  - [プラットフォーム作成の一般規則](https://japan.xilinx.com/html_docs/xilinx2020_2/vitis_doc/create_embedded_platforms.html#rjs1596051748503)
- [Vitis エンベデッド プラットフォーム ソース GitHub リポジトリ](https://github.com/Xilinx/Vitis_Embedded_Platform_Source)

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
<p align="center"><sup>この資料は 2021 年 2 月 8 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
