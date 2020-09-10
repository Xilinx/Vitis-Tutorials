<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ アプリケーション アクセラレーション開発フロー チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">2019.2 Vitis アプリケーション アクセラレーション開発フロー チュートリアル</a></td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# makefile の理解

この演習に必要なソース ファイルはすべて `design` フォルダーに含まれます。`design` フォルダーには、元のアプリケーション コードを含む `cpu_src`、アクセラレーション アプリケーション コードを含む `src`、`makefile` の 3 つのフォルダーが含まれます。

`src` フォルダーには、さらに 6 つのフォルダーが含まれ、各手順のソース ファイルが含まれます。次のディレクトリは、チュートリアルの各手順に該当しています。

| チュートリアルの手順| 関連ディレクトリ
|----------|----------
| C アプリケーションから Vitis コア開発キット アプリケーションを作成| `src/baseline`
| メモリ転送を最適化| `src/localbuf`
| 固定小数点データ型を使用して最適化| `src/fixedpoint`
| データフローを使用して最適化| `src/dataflow`
| 順不同キューと複数のカーネルを使用| `src/multicu`
| 複数の計算ユニットを含む QDMA を使用| `src/qdma`

makefile の実行時に TARGET を設定して、ソフトウェア エミュレーション、ハードウェア エミュレーション、Alveo データセンター アクセラレータ カードのハードウェアを実行できます。

## デザインのビルド

デザインをビルドするには、次のコマンドを使用します。

```
make build TARGET=<sw_emu/hw_emu/hw> STEP=<baseline/localbuf/fixedpoint/dataflow/multicu/> SOLUTION=1
```

チュートリアルに基づいてコードをアップデートするのに使用可能な `src` ディレクトリに加え、`reference-files` ディレクトリも提供されています。makefile ターゲットに `SOLUTION=1` を設定すると、`reference-files` ディレクトリのファイルを直接使用できます。

たとえば、ベースライン演習用にハードウェア エミュレーションの xclbin をビルドするには、次のコマンドを使用します。

```
make build TARGET=hw_emu STEP=baseline SOLUTION=1
```

## デザインの実行

デザインをエミュレーションまたはハードウェアで実行するには、次のコマンドを使用します。

```
make run TARGET=<sw_emu/hw_emu/hw> STEP=<baseline/localbuf/fixedpoint/dataflow/multicu> SOLUTION=1 [NUM_FRAMES=1]
```

NUM\_FRAMES オプションは、ホスト アプリケーションの実行に使用される入力ビデオ フレーム数を定義します。ソフトウェアおよびハードウェア エミュレーションでは、NUM\_FRAMES を **1** に設定してエミュレーション時間を削減します。ハードウェア実行では、フル ビデオ ファイルが使用されるので、このオプションは使用しないでください。

たとえば、ベースライン手順のハードウェア エミュレーションを実行するには、次のコマンドを使用します。

```
make run TARGET=hw_emu STEP=baseline SOLUTION=1 NUM_FRAMES=1
```

## プロファイルおよびタイムライン トレース レポートの生成

デザインのパフォーマンスを解析するには、レポートを確認する必要があります。レポートの詳細は、「アクセラレーション アプリケーションのビルドおよび実行の基本概念」チュートリアルの[プロファイルおよびトレース レポートの生成](../Pathway3/ProfileAndTraceReports.md)演習を参照してください。

## プロファイルおよびタイムライン トレースの表示

プロファイル サマリ レポートとタイムライン トレースを表示するには、次のコマンドを使用します。

```
make view_run_summary TARGET=hw_emu STEP=multicu
```

makefile は、次のコマンドを使用してプロファイル サマリ レポートとタイムライン トレース レポートを開きます。

```
vitis_analyzer run_summary
```

## クリーンアップ

前に生成したビルドをクリーンアップするには、次のコマンドを使用します。

```
make clean TARGET=hw_emu STEP=baseline
```

これにより、baseline バージョンのデザイン用にハードウェア エミュレーションで生成されたファイルが削除されます。</br>

<hr/>
<p align="center" class="sphinxhide"><b><a href="/docs/vitis-getting-started/README.md">入門コースの初めに戻る</a> &mdash; <a href="/docs/convolution-tutorial/README.md">チュートリアルの初めに戻る</a></b></p>
<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
