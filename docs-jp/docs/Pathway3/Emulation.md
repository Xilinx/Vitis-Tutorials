<table>
 <tr>
   <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2019.2 Vitis™ アプリケーション アクセラレーション開発フローのチュートリアル</h1><a href="https://github.com/Xilinx/SDAccel-Tutorials/branches/all">SDAccel™ 開発環境 2019.1 チュートリアルを参照</a></td>
 </tr>
 <tr>
 <td align="center"><h1>アクセラレーション アプリケーションのビルドおよび実行の基本概念</h1>
 </td>
 </tr>
</table>

# 2\. ソフトウェアおよびハードウェア エミュレーションの実行

## 概要

エミュレーションは、デザインのデバッグおよび最適化に不可欠なプロセスです。Vitis コア開発キットには、ソフトウェアおよびハードウェア エミュレーションの両方が含まれます。

このチュートリアルでは、ソフトウェアおよびハードウェア エミュレーションの両方を実行する手順について説明します。

## 開始前の注意点

この演習の前に、[アプリケーションのビルド](./BuildingAnApplication.md)演習を確認して、実行してください

## ソフトウェアおよびハードウェア エミュレーションの実行

アプリケーションのデバッグおよび最適化に役立つように、Vitis コア開発キットでは、ソフトウェアおよびハードウェア エミュレーションの両方でデザインを実行できるようになっています。ソフトウェア エミュレーションとハードウェア エミュレーションのそれぞれで、さまざまなレベルでデザインに関する特定の情報を取得できます。詳細は、この後説明します。エミュレーションをおよび最適化の詳細は、[アプリケーションのプロファイル、最適化、およびデバッグ](https://japan.xilinx.com/html_docs/xilinx2019_2/vitis_doc/wzc1553475252001.html)を参照してください。

* ソフトウェア エミュレーション (`sw_emu`) では、ホスト アプリケーション コードとカーネル コードの両方が x86 プロセッサで実行できるようコンパイルされます。これにより、高速なビルドおよび実行ループを使用した反復アルゴリズムによる改善が可能になります。このターゲットは、構文エラーを特定し、アプリケーションと共に実行されるカーネル コード ソース レベルのデバッグを実行し、システムの動作を検証するのに便利です。RTL カーネルでは、C モデルが関連付けられている場合にソフトウェア エミュレーションがサポートされます。

* ハードウェア エミュレーション (`hw_emu`) では、カーネル コードがハードウェア モデル (RTL) にコンパイルされ、専用シミュレータで実行されますが、残りのシステムでは C シミュレータが使用されます。ビルドおよび実行にかかる時間は長くなりますが、詳細でサイクル精度の高いカーネル アクティビティが表示されます。このターゲットは、FPGA に含まれるロジックの機能をテストして、最初のパフォーマンス見積もりを取得する場合に便利です。

## エミュレーションを実行するための要件

エミュレーションを実行するには、次を実行しておく必要があります。

1. デザインをビルドします。
2. エミュレーション コンフィギュレーション ファイルを生成します。
3. XCL\_EMULATION\_MODE 環境変数を設定します。

### デザインのビルド

エミュレーションを実行する前に、必要なエミュレーション モード (ソフトウェアまたはハードウェア) でアプリケーションをビルドします。エミュレーション モードは `-t <build_target>` という `v++` コマンド オプションでハードウェアをビルドする際に指定します。

* \* ソフトウェア エミュレーションを実行するには、\<*build\_target*> を **sw\_emu** に設定してハードウェアをビルドする必要があります。
* 同様に、ハードウェア エミュレーションを実行するには、\<*build\_target*> を **hw\_emu** に設定してハードウェアをビルドする必要があります。

ホスト ソフトウェアおよびハードウェアのビルド方法および \<*build\_target*> の指定方法については、[アプリケーションのビルド](./BuildingAnApplication.md)演習を参照してください。

> **重要:** サンプルを実行する前に、インストールに示すように Vitis コア開発キットをインストールしておく必要があります。
>
> ```bash
> #setup Xilinx Vitis tools. XILINX_VITIS and XILINX_VIVADO will be set in this step.
> source <VITIS_install_path>/settings64.sh
> #Setup runtime. XILINX_XRT will be set in this step
> source <XRT_install_path>/setup.sh
> ```

### エミュレーション コンフィギュレーション ファイルの生成

アプリケーションをビルドして特定のエミュレーション モードをターゲットにしたら、エミュレーション コンフィギュレーション ファイルを生成する必要もあります。

エミュレーション コンフィギュレーション ファイル (`emconfig.json`) は、`emconfigutil` コマンドを使用して生成され、エミュレーション中にザイリンクス ランタイム (XRT) ライブラリで使用される情報を含みます。`emconfigutil` コマンドは、Vitis コア開発キットのインストールに含まれます。

必要な `emconfig.json` を生成するには、最低でも `-f` または `-–platform` オプションを使用してターゲット プラットフォームを指定する必要があります。指定する *platform\_target* は、ホストおよびハードウェア ビルド中に使用されるものと同じである必要があります。1 つの `emconfig.json` ファイルをソフトウェアおよびハードウェア エミュレーションの両方に使用できます。

```
emconfigutil -–platform <platform_target>
```

オプションのリストは、`emconfigutil --help` を実行するか、[Vitis 環境リファレンス マニュアル](https://japan.xilinx.com/html_docs/xilinx2019_2/vitis_doc/yxl1556143111967.html)を参照してください。

### XCL\_EMULATION\_MODE 環境変数の設定

最後に、ソフトウェアまたはハードウェア エミュレーションのいずれかを実行する前に、XCL\_EMULATION\_MODE 環境変数を設定します。この変数は、必要なエミュレーションを正しく実行するためにランタイム ライブラリで使用されます。

* ソフトウェア エミュレーションの場合は、変数を `sw_emu` に設定します。

  ```
  export XCL_EMULATION_MODE=sw_emu
  ```

* ハードウェア エミュレーションの場合は、変数を `hw_emu` に設定します。

  ```
  export XCL_EMULATION_MODE=hw_emu
  ```

## ソフトウェア エミュレーションを実行

わかっていることを使用し、すべてをまとめて、提供されているソース ファイルの手順に従ってソフトウェア エミュレーションを実行します。

1. *build\_target* を `sw_emu` に設定してソフトウェアおよびハードウェアが ビルドされるようにします。

   > **注記:** ホスト ソフトウェアおよびハードウェアのビルド方法は、[アプリケーションのビルド](./BuildingAnApplication.md)演習を参照してください。

2. エミュレーション コンフィギュレーション ファイルを生成します。  
生成した `emconfig.json` ディレクトリにある必要がありますので、ホスト実行ファイルと同じディレクトリに移動してから、次のコマンドを実行します。

   ```bash
   emconfigutil --platform xilinx_u200_xdma_201830_2
   ```

   platform\_target は xilinx\_u200\_xdma\_201830\_2 です。これは、ソフトウェアおよびハードウェア ビルド プロセス中に使用した platform\_target と同じである必要があります。

3. XCL\_EMULATION\_MODE 環境変数を `sw_emu` に設定してソフトウェア エミュレーションで実行します。

   ```bash
   export XCL_EMULATION_MODE=sw_emu
   ```

4. コンフィギュレーション ファイル (`emconfig.json`) を生成して XCL\_EMULATION\_MODE を設定したら、次のコマンドでホスト プログラムとカーネルをソフトウェア エミュレーション モードで実行します。

   ```bash
   ./host mmult.sw_emu.xilinx_u200_xdma_201830_2.xclbin
   ```

   このコマンドにより Vitis アナライザーで使用可能な `run.summary` ファイルも作成され、関連レポートを表示できるようになります。

ソフトウェア エミュレーションが問題なく実行されると、コンソールには次のように表示されます。

```
Loading: 'mmult.sw_emu.xilinx_u200_xdma_201830_2.xclbin'
TEST PASSED
```

## ハードウェア エミュレーションの実行

ソフトウェア エミュレーションを実行したので、同じデザインのハードウェア エミュレーションを実行し、ソフトウェアおよびハードウェア エミュレーションの違いを確認します。

1. *build\_target* を hw\_emu に設定してソフトウェアおよびハードウェアが ビルドされるようにします。

   > **注記:** ホスト ソフトウェアおよびハードウェアのビルド方法は、[アプリケーションのビルド](./BuildingAnApplication.md)演習を参照してください。

2. エミュレーション コンフィギュレーション ファイルを生成します。

   > **注記:** ファイルが既に「ソフトウェア エミュレーションの実行」セクションで生成されている場合、この手順は飛ばすことができます。

   生成した `emconfig.json` ディレクトリにある必要がありますので、ホスト実行ファイルと同じディレクトリに移動してから、次のコマンドを実行します。

   ```bash
   emconfigutil --platform xilinx_u200_xdma_201830_2
   ```

   この場合、platform\_target は xilinx\_u200\_xdma\_201830\_2 です。  これは、ソフトウェアおよびハードウェア ビルド プロセス中に使用した platform\_target と同じである必要があります。

3. XCL\_EMULATION\_MODE 環境変数を `hw_emu` に設定してハードウェア エミュレーションで実行します。

   ```bash
   export XCL_EMULATION_MODE=hw_emu
   ```

4. コンフィギュレーション ファイル (`emconfig.json`) を生成して XCL\_EMULATION\_MODE 変数を設定したら、次のコマンドでホスト プログラムとカーネルをハードウェア エミュレーション モードで実行します。

   ```bash
   ./host mmult.hw_emu.xilinx_u200_xdma_201830_2.xclbin
   ```

ハードウェア エミュレーションが問題なく実行されると、コンソールには次のように表示されます。

```
Loading: 'mmult.hw_emu.xilinx_u200_xdma_201830_2.xclbin'
INFO: [Vitis-EM 01] Hardware emulation runs simulation underneath. Using a large data set will result in long simulation times. It is recommended that a small dataset is used for faster execution. This flow does not use cycle accurate models and hence the performance data generated is approximate.
TEST PASSED
INFO: [Vitis-EM 22] [Wall clock time: 19:16, Emulation time: 0.00651464 ms] Data transfer between kernel(s) and global memory(s)
mmult_1:m_axi_gmem-DDR[1]          RD = 2.000 KB               WR = 1.000 KB
```

## まとめ

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

## 次のステップ

エミュレーションを実行したら、プロファイル サマリとタイムライン トレース レポートを生成して、さらにデバッグおよび最適化をします。これらのレポートを生成する方法は、[プロファイルおよびトレース レポートの生成](./ProfileAndTraceReports.md)演習を参照してください。</br>

<hr/>
<p align="center"><b><a href="../../docs/vitis-getting-started/README.md">入門コースの初めに戻る</a></b> &mdash; <b><a href="./README.md">チュートリアルの初めに戻る</a></b></p>
<p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>
