<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis™ 入門チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/tree/2020.1">2020.1 チュートリアルを参照</a></td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# Vitis フロー 101 – パート 2: インストール ガイド

Vitis を使用してアプリケーションを開発および運用するには、Vitis 統合ソフトウェア環境、ザイリンクス ランタイム ライブラリ (XRT)、およびプロジェクトで使用されているアクセラレーション カード独自のプラットフォーム ファイルをインストールする必要があります。

## 手順 1 – インストール要件

インストール プロセスを開始する前に、必ず [Vitis のインストール要件](https://japan.xilinx.com/html_docs/xilinx2020_2/vitis_doc/acceleration_installation.html#igz1531201833632)を確認してください。

>**注記**: Windows OS は、Vitis エンベデッド ソフトウェア開発フローでのみサポートされます。Vitis アクセラレーション フローは Linux でのみサポートされます。

## 手順 2 – Vitis のダウンロード

最新版の Vitis は、[ザイリンクス ダウンロード ウェブサイト](https://japan.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vitis.html)からダウンロードできます。必ず Linux 自己解凍型ウェブ インストーラーまたはシングル ファイル インストーラーをダウンロードしてください。

## 手順 3 – ダウンロードしたパッケージのインストール

Vitis、XRT、およびプラットフォーム パッケージをダウンロードしたら、次の順番に指示に従ってインストールします。

1. [Vitis ソフトウェア プラットフォームのインストール](https://japan.xilinx.com/html_docs/xilinx2020_2/vitis_doc/acceleration_installation.html#dhg1543555360045)

2. [ザイリンクス ランタイムおよびプラットフォームのインストール](https://japan.xilinx.com/html_docs/xilinx2020_2/vitis_doc/acceleration_installation.html#pjr1542153622642)

## 手順 4 – プラットフォーム独自のファイルのダウンロードおよびインストール

<details>
<summary><b>エンベデッド プラットフォームについては、ここをクリック</b></summary>
ザイリンクスでは、ZCU102 および ZCU104 カードのベース プラットフォームを提供しています。プラットフォームをインストールする前に、次のパッケージをダウンロードする必要があります。

1. XRT
2. ベース プラットフォーム
3. 共通イメージ

各プラットフォームに必要なパッケージは、対応するダウンロード ページにあります。

* [ZCU102](https://japan.xilinx.com/support/download/index.html/content/xilinx/ja/downloadNav/embedded-platforms.html)

* [ZCU104](https://japan.xilinx.com/support/download/index.html/content/xilinx/ja/downloadNav/embedded-platforms.html)

</details>
<details>
<summary><b>データセンター プラットフォームについては、ここをクリック</b></summary>
ザイリンクスでは、Alveo U200、U250、U50、および U280 データセンター アクセラレーション カード用のベース プラットフォームを提供しています。プラットフォームをインストールする前に、次のパッケージをダウンロードする必要があります。

1. XRT (ザイリンクス ランタイム)

2. 運用ターゲット プラットフォーム

3. 開発ターゲット プラットフォーム

各プラットフォームに必要なパッケージは、対応するダウンロード ページにあります。

* [U200](https://japan.xilinx.com/products/boards-and-kits/alveo/u200.html#gettingStarted)

* [U250](https://japan.xilinx.com/products/boards-and-kits/alveo/u250.html#gettingStarted)

* [U50](https://japan.xilinx.com/products/boards-and-kits/alveo/u50.html#gettingStarted)

* [U280](https://japan.xilinx.com/products/boards-and-kits/alveo/u280.html#gettingStarted)

</details>

## 手順 5 - Vitis を実行する環境の設定

Vitis を実行する環境を設定するには、次のスクリプトを実行して、特定のコマンド シェルで実行する環境を設定します。

```bash
source <Vitis_install_path>/Vitis/2020.2/settings64.sh
source /opt/xilinx/xrt/setup.sh
```

>**注記**: .csh スクリプトも提供されていますが、このチュートリアルでは bash シェルが使用されていることを前提としています。

インストールしたデータセンター プラットフォームまたはエンベデッド プラットフォームのディレクトリを指定するには、次の環境変数を設定します。

```bash
export PLATFORM_REPO_PATHS=<path to platforms>
```

>**注記**: Ubuntu ディストリビューションによっては、Vitis を正しく設定するために LIBRARY\_PATH もエクスポートする必要があります。

```bash
export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu
```

詳細は、[ザイリンクス アンサー 73698](https://japan.xilinx.com/support/answers/73698.html) を参照してください。

## 次の手順

これで Vitis を使用するために必要なものをすべて正しくインストールできました。[**パート 3**](./Part3.md) に進んで、はじめてのサンプル プロジェクトを作成できるようになりました。

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
