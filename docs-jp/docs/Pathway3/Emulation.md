<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ アプリケーション アクセラレーション開発フロー チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">2019.2 Vitis アプリケーション アクセラレーション開発フロー チュートリアル</a></td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# 2\. ソフトウェアおよびハードウェア エミュレーションの実行

## 概要

エミュレーションは、デザインのデバッグおよび最適化に不可欠なプロセスです。Vitis コア開発キットでは、ソフトウェアおよびハードウェア エミュレーションの両方を実行できます。

このチュートリアルでは、ソフトウェアおよびハードウェア エミュレーションの両方を実行する手順を説明します。

## 開始前の確認事項

この演習を実行する前に、[アプリケーションのビルド](./BuildingAnApplication.md)演習を見直してください。

## ソフトウェアおよびハードウェア エミュレーションの実行

アプリケーションのデバッグおよび最適化のため、Vitis コア開発キットでは、デザインをソフトウェアおよびハードウェア エミュレーションの両方で実行できます。それぞれで、デザインに関するさまざまなレベルの情報を取得できます。エミュレーションおよび最適化の詳細は、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの[アプリケーションのプロファイル、最適化、およびデバッグ](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=wzc1553475252001.html)を参照してください。

* ソフトウェア エミュレーション (`sw_emu`) では、ホスト アプリケーション コードとカーネル コードの両方が x86 プロセッサで実行できるようコンパイルされます。これにより、ビルドと実行を高速に繰り返して反復アルゴリズムで調整していくことができます。このターゲットは、構文エラーを特定し、アプリケーションと共に実行されるカーネル コードをソース レベルでデバッグし、システムの動作を検証するのに便利です。RTL カーネルでは、C モデルが関連付けられている場合にソフトウェア エミュレーションがサポートされます。

* ハードウェア エミュレーション (`hw_emu`) では、カーネル コードはハードウェア モデルにコンパイルされ、ハードウェア シミュレータで実行されますが、システムの残りの部分には C シミュレータが使用されます。ビルドおよび実行にかかる時間は長くなりますが、詳細でサイクル精度のカーネル アクティビティが表示されます。このターゲットは、FPGA で実行されるロジックの機能をテストして、初期パフォーマンス見積もりを取得する場合に便利です。

## エミュレーションを実行するための要件

エミュレーションを実行するには、次を実行しておく必要があります。

1. デザインをビルドします。
2. エミュレーション コンフィギュレーション ファイル (`emconfig.json`) を生成します。
3. XCL\_EMULATION\_MODE 環境変数を設定します。

### デザインのビルド

エミュレーションを実行する前に、必要なエミュレーション モード (ソフトウェアまたはハードウェア) でアプリケーションをビルドします。エミュレーション モードは、ハードウェアをビルドする際に `v++ -t <build_target>` コマンドで指定します。

* ソフトウェア エミュレーションを実行するには、*\<build\_target>* を **sw\_emu** に設定してハードウェアをビルドする必要があります。
* 同様に、ハードウェア エミュレーションを実行するには、*\<build\_target>* を **hw\_emu** に設定してハードウェアをビルドする必要があります。

ホスト ソフトウェアおよびハードウェア カーネルのビルド方法および *\<build\_target>* の指定方法は、[アプリケーションのビルド](./BuildingAnApplication.md)演習を参照してください。

> **重要:** サンプルを実行する前に、Vitis コア開発キットをインストールしておく必要があります。
>
> ```bash
> #setup Xilinx Vitis tools. XILINX_VITIS and XILINX_VIVADO will be set in this step.
> source <VITIS_install_path>/settings64.sh
> #Setup runtime. XILINX_XRT will be set in this step
> source <XRT_install_path>/setup.sh
> ```

### エミュレーション コンフィギュレーション ファイルの生成

アプリケーションをビルドして特定のエミュレーション モードをターゲットにしたら、エミュレーション コンフィギュレーション ファイルを生成する必要もあります。

エミュレーション コンフィギュレーション ファイル (`emconfig.json`) は、`emconfigutil` コマンドを使用して生成し、エミュレーション中にザイリンクス ランタイム (XRT) ライブラリで使用される情報を含みます。`emconfigutil` ユーティリティは、Vitis コア開発キットのインストールに含まれます。

必要な `emconfig.json` を生成するには、最低でも `-f` または `-–platform` オプションを使用してターゲット プラットフォームを指定する必要があります。指定する *platform\_target* は、ハードウェア ビルドで使用したものと同じである必要があります。1 つの `emconfig.json` ファイルをソフトウェアおよびハードウェア エミュレーションの両方に使用できます。

```
emconfigutil -–platform <platform_target>
```

オプションのリストは、`emconfigutil --help` を実行するか、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの [Vitis 環境リファレンス資料](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=yxl1556143111967.html)を参照してください。

### XCL\_EMULATION\_MODE 環境変数の設定

ソフトウェアまたはハードウェア エミュレーションを実行する前に、XCL\_EMULATION\_MODE 環境変数を設定します。この変数は、必要なエミュレーションを正しく実行するためにランタイム ライブラリで使用されます。

* ソフトウェア エミュレーションの場合は、変数を `sw_emu` に設定します。

  ```
  export XCL_EMULATION_MODE=sw_emu
  ```

* ハードウェア エミュレーションの場合は、変数を `hw_emu` に設定します。

  ```
  export XCL_EMULATION_MODE=hw_emu
  ```

## ソフトウェア エミュレーションを実行

提供されているソース ファイルの手順に従って、ソフトウェア エミュレーションを実行します。

1. ソフトウェアおよびハードウェアが *build\_target* を `sw_emu` に設定してビルドされていることを確認します。

   > **注記:** ホスト ソフトウェアおよびハードウェアのビルド方法は、[アプリケーションのビルド](./BuildingAnApplication.md)演習を参照してください。

2. エミュレーション コンフィギュレーション ファイルを生成します。  
生成した `emconfig.json` ファイルは、ホスト実行ファイルと同じディレクトリに含まれている必要があります。それを確認してから、次のコマンドを実行します。

   ```bash
   emconfigutil --platform xilinx_u200_xdma_201830_2
   ```

   platform\_target は xilinx\_u200\_xdma\_201830\_2 です。これは、ソフトウェアおよびハードウェア ビルド プロセスで指定した platform\_target と同じである必要があります。

3. XCL\_EMULATION\_MODE 環境変数を `sw_emu` に設定してソフトウェア エミュレーションを実行します。

   ```bash
   export XCL_EMULATION_MODE=sw_emu
   ```

4. コンフィギュレーション ファイル (`emconfig.json`) を生成して XCL\_EMULATION\_MODE を設定したら、次のコマンドを使用してホスト プログラムとカーネルをソフトウェア エミュレーション モードで実行します。

   ```bash
   ./host mmult.sw_emu.xilinx_u200_xdma_201830_2.xclbin
   ```

ソフトウェア エミュレーションが問題なく実行されると、コンソールに次のように表示されます。

```
Loading: `mmult.sw_emu.xilinx_u200_xdma_201830_2.xclbin'
TEST PASSED
```

## ハードウェア エミュレーションの実行

ソフトウェア エミュレーションを実行したので、同じデザインのハードウェア エミュレーションを実行し、ソフトウェア エミュレーションとハードウェア エミュレーションの違いを確認します。

1. ソフトウェアおよびハードウェアが *build\_target* を hw\_emu に設定してビルドされていることを確認します。

   > **注記:** ホスト ソフトウェアおよびハードウェアのビルド方法は、[アプリケーションのビルド](./BuildingAnApplication.md)演習を参照してください。

2. エミュレーション コンフィギュレーション ファイルを生成します。

   > **注記:** ファイルが既に「ソフトウェア エミュレーションの実行」セクションで生成されている場合、この手順は飛ばすことができます。

   生成した `emconfig.json` ファイルは、ホスト実行ファイルと同じディレクトリに含まれている必要があります。それを確認してから、次のコマンドを実行します。

   ```bash
   emconfigutil --platform xilinx_u200_xdma_201830_2
   ```

   この場合、platform\_target は xilinx\_u200\_xdma\_201830\_2 です。  これは、ソフトウェアおよびハードウェア ビルド プロセスで指定した platform\_target と同じである必要があります。

3. XCL\_EMULATION\_MODE 環境変数を `hw_emu` に設定してハードウェア エミュレーションを実行します。

   ```bash
   export XCL_EMULATION_MODE=hw_emu
   ```

4. コンフィギュレーション ファイル (`emconfig.json`) を生成して XCL\_EMULATION\_MODE 変数を設定したら、次のコマンドを使用してホスト プログラムとカーネルをハードウェア エミュレーション モードで実行します。

   ```bash
   ./host mmult.hw_emu.xilinx_u200_xdma_201830_2.xclbin
   ```

このコマンドにより、Vitis アナライザーで表示可能な `run_summary` ファイルも生成されます。`run_summary` は、ハードウェア エミュレーションおよびシステム実行で自動的に生成されます。

ハードウェア エミュレーションが問題なく実行されると、コンソールに次のように表示されます。

```
Loading: 'mmult.hw_emu.xilinx_u200_xdma_201830_2.xclbin'
INFO: [Vitis-EM 01] Hardware emulation runs simulation underneath. Using a large data set will result in long simulation times. It is recommended that a small dataset is used for faster execution. This flow does not use cycle accurate models and hence the performance data generated is approximate.
TEST PASSED
INFO: [Vitis-EM 22] [Wall clock time: 19:16, Emulation time: 0.00651464 ms] Data transfer between kernel(s) and global memory(s)
mmult_1:m_axi_gmem-DDR[1]          RD = 2.000 KB               WR = 1.000 KB
```

## 手順のサマリ

エミュレーションの設定および実行に必要な手順をまとめると、次のようになります。

1. コア開発キットを設定します。

   ```bash
   source <Vitis_install_path>/settings64.sh
   source <XRT_install_path>/setup.sh
   cd ./reference-files/run
   ```

2. エミュレーション コンフィギュレーション ファイルを生成します。

   ```bash
   emconfigutil --platform xilinx_u200_xdma_201830_2
   ```

3. ソフトウェア エミュレーションを実行します。

   ```bash
   export XCL_EMULATION_MODE=sw_emu
   ./host mmult.sw_emu.xilinx_u200_xdma_201830_2.xclbin
   ```

4. ハードウェア エミュレーションを実行します。

   ```bash
   export XCL_EMULATION_MODE=hw_emu
   ./host mmult.hw_emu.xilinx_u200_xdma_201830_2.xclbin
   ```

## 次の手順

エミュレーションを実行したら、プロファイル サマリとタイムライン トレース レポートを生成して、さらにデバッグおよび最適化をします。これらのレポートを生成する方法は、[プロファイルおよびトレース レポートの生成](./ProfileAndTraceReports.md)演習を参照してください。</br>

<hr/>
<p align="center" class="sphinxhide"><b><a href="/docs/vitis-getting-started/">入門コースの初めに戻る</a></b> &mdash; <b><a href="/docs/Pathway3/README.md">チュートリアルの初めに戻る</a></b></p>
<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
