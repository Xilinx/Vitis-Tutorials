<table>
 <tr>
   <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2019.2 Vitis™ アプリケーション アクセラレーション開発フローのチュートリアル</h1><a href="https://github.com/Xilinx/SDAccel-Tutorials/branches/all">SDAccel™ 開発環境 2019.1 チュートリアルを参照</a></td>
 </tr>
 <tr>
 <td align="center"><h1>アクセラレーション FPGA アプリケーションの最適化: ブルーム フィルターの例</td>
 </tr>
</table>

# Makefile について

この演習に必要なソース ファイルは `design` フォルダーにあります。`design` フォルダーには、元のアプリケーション コードが含まれている `cpu_src`、アクセラレートされたアプリケーション コードが含まれている `src`、そして `makefile` の 3 つのサブディレクトリがあります。

`src` フォルダーには 5 つのサブフォルダーがあって、各ステップのソース ファイルが含まれています。チュートリアルの各ステップで使用するディレクトリは次のようになっています。

| チュートリアルのステップ| ディレクトリ
|----------|----------
| C アプリケーションから Vitis コア開発キット アプリケーションを作成| [src/baseline\_fpga](./design/src/baseline_fpga)
| メモリ転送の最適化| [src/localbuf](./design/src/localbuf)
| データフローを使用した最適化| [src/dataflow](./design/src/dataflow)
| アウト オブ オーダー キューおよび複数のカーネルの使用| [src/multicu](./design/src/multicu)
| 複数 DDR バンクを使用した最適化| [src/multiddr](./design/src/multiddr)

makefile ターゲットは、ソフトウェア エミュレーション、ハードウェア エミュレーション、Alveo データセンター アクセラレータ カードでのハードウェアを実行するために使用できます。

## デザインを構築するための makefile ターゲット

デザインを構築するには次のコマンドを使用します。

```
make build TARGET=<sw_emu/hw_emu/hw> STEP=<baseline/localbuf/dataflow/multicu/multiddr>
```

チュートリアルに基づいてコードをアップデートするために使用できる `src` ディレクトリだけでなく、`reference-files` ディレクトリも提供されています。この `reference-files` ディレクトリにあるファイルを直接使用して makefile ターゲットに `NUM_DOCS=100` と設定します。`NUM_DOCS` はこのアプリケーションを実行してスコアを計算するドキュメント数を表しています。

たとえば、ベースラインの演習でハードウェア エミュレーション xclbin を構築するには、次のコマンドを使用します。

```
make build TARGET=hw_emu STEP=baseline NUM_DOCS=100
```

## デザインを実行するための makefile ターゲット

エミュレーションまたはハードウェアでデザインを実行するには、次のコマンドを使用します。

```
make run TARGET=<sw_emu/hw_emu/hw> STEP=<baseline/localbuf/dataflow/multicu/multiddr> NUM_DOCS=100
```

`NUM_DOCS` オプションは、ホスト アプリケーション実行用に実行するドキュメントの数を定義します。ソフトウェアおよびハードウェア エミュレーションの場合は、エミュレーション時間を短縮するため、`NUM_DOCS` を **100** に設定することを推奨します。

たとえば、ベースライン ステップ用にハードウェア エミュレーションを実行するには次のコマンドを使用します。

```
make run TARGET=hw_emu STEP=baseline NUM_DOCS=100
```

## プロファイルおよびタイムライン トレース レポートを生成するための makefile ターゲット

デザインのパフォーマンスを解析するにはレポートを確認する必要があります。これらのレポートの詳細は、「アクセラレートされたアプリケーションの構築および実行の基本コンセプト」の[プロファイルおよびトレース レポートの生成](../Pathway3/ProfileAndTraceReports.md)の演習を参照してください。

## プロファイルおよびタイムライン トレース レポートを確認するための makefile ターゲット

次のコマンドを使用して、プロファイル サマリとタイムライン トレースのレポートを確認します。

```
make view_report TARGET=hw_emu STEP=baseline
```

## 消去するための makefile ターゲット

以前生成したビルドを消去するにするには、次のコマンドを使用します。

```
make clean TARGET=hw_emu STEP=baseline
```

これで、ベースライン バージョンのデザイン用にハードウェア エミュレーションで生成されたファイルが消去されます。</br>

<hr/>
<p align="center"><b><a href="../../docs/vitis-getting-started/README.md">入門ガイドの最初に戻る</a> &mdash; <a href="./README.md">チュートリアルの最初に戻る</a></b></p>
<p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>
