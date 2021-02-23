<p align="right"><a href="https://xilinx.github.io/Vitis-Tutorials/master/docs/index.html">English</a> | <a>日本語</a></p>
<table class="sphinxhide">

 <tr>
   <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis™ アプリケーション アクセラレーション開発フロー チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/tree/2020.1">2020.1 Vitis アプリケーション アクセラレーション開発フロー チュートリアル</a></td>
 </tr>
</table>

# ビデオたたみ込みフィルター アプリケーションのアクセラレーション

このチュートリアルでは、ザイリンクス Alveo データセンター アクセラレータ カードを使用してアクセラレーションされた計算量の多いアプリケーションについて説明します。FPGA 上で実行する特定のカーネルを設計し、パフォーマンスのためのホスト側アプリケーションの最適化について簡単に説明します。カーネルはスループットが最大になるように設計され、ホスト アプリケーションは、ホストと FPGA カードの間でデータが効率的に転送されるように最適化されます。ホスト アプリケーションは、複数のカーネル呼び出しに対処できるようにデータ転送を重複させて、データ移動の遅延をなくします。このチュートリアルでは、**Vitis HLS を使用してビルド可能なハードウェア カーネルのパフォーマンスを簡単に見積もる方法と、その見積もりがどれくらい正確で実際のハードウェア パフォーマンスに近いか**についても説明します。

## アクセラレーションの概要

1 つ目の演習は、ビデオ フィルターをザイリンクス Alveo アクセラレータ カードに移植することで達成可能なアクセラレーション パフォーマンスをすばやく体験できるように設計されています。Alveo シリーズ カードは、データセンター アプリケーションをアクセラレーションするために設計されています。ただし、このチュートリアルはいくつかの簡単な変更を加えると、ほかのアクセラレータ カードにも適用できます。

この最初の演習で実行する手順は次のとおりです。

- Vitis アプリケーション アクセラレーション開発フローを設定
- ハードウェア最適化済みのアクセラレータを実行し、そのパフォーマンスをアプリケーションのベースラインと比較

この演習では、CPU のパフォーマンスと比較すると、かなりパフォーマンスを向上できることを示します。このチュートリアルの次の演習では、2D たたみ込みカーネルとホスト側アプリケーションのさまざまな最適化および設計手法を使用して、このようなパフォーマンスを達成する方法について説明します。

### GitHub リポジトリのクローンおよび Vitis ツールの設定

次のコマンドを使用して、リポジトリをクローンします。

```bash
git clone https://github.com/Xilinx/Vitis-Tutorials.git
```

ローカル インストールごとに、次のコマンドを使用して、アプリケーション ビルドおよびランタイム環境を設定します。

```bash
source <XILINX_VITIS_INSTALL_PATH>/settings64.sh
source <XRT_INSTALL_PATH>/setup.sh
```

### アプリケーション パフォーマンスのベースライン

ソフトウェア アプリケーションは、解像度 1920x1080 の高解像度 (HD) ビデオフレーム/画像を処理します。一連の画像にたたみ込みを実行し、パフォーマンス結果のサマリを表示します。これは、ベースライン ソフトウェアのパフォーマンスを測定するために使用されます。次のように、チュートリアルを指定する環境変数を repo のパスを基準に設定してください。

```bash
export CONV_TUTORIAL_DIR=/VITIS_TUTORIAL_REPO_PATH/Hardware_Accelerators/Design_Tutorials/01-convolution-tutorial
```

**VITIS\_TUTORIAL\_REPO\_PATH** は、クローン後にユーザーが配置した git repo のローカル パスです。

**注記**: このチュートリアルのすべての演習で `CONV_TUTORIAL_DIR` 変数が正しく設定されるようにしてください。

アプリケーションを実行して、次のようにパフォーマンスを測定します。

```bash
cd $CONV_TUTORIAL_DIR/sw_run
./run.sh
```

次に示すような結果が表示されます。CPU のスループットを書き留めておきます。

```bash
----------------------------------------------------------------------------
Number of runs    : 60
Image width       : 1920
Image height      : 1080
Filter type       : 6

Generating a random 1920x1080 input image
Running Software version on 60 images

CPU  Time         :    28.0035 s
CPU  Throughput   :    12.7112 MB/s
----------------------------------------------------------------------------
```

## FPGA アクセラレーション アプリケーションの実行

### ホスト アプリケーションの起動

FPGA アクセラレーションされたビデオたたみ込みフィルターを使用するアプリケーションを起動します。アプリケーションは、実際の FPGA カードで実行されます (システム実行とも呼ばれます)。

```bash
cd $CONV_TUTORIAL_DIR
make run
```

結果のサマリは、次のようになります。

```bash
----------------------------------------------------------------------------
Xilinx 2D Filter Example Application (Randomized Input Version)

FPGA binary       : ../xclbin/fpgabinary.hw.xclbin
Number of runs    : 60
Image width       : 1920
Image height      : 1080
Filter type       : 3
Max requests      : 12
Compare perf.     : 1

Programming FPGA device
Generating a random 1920x1080 input image
Running FPGA accelerator on 60 images
Running Software version
Comparing results

Test PASSED: Output matches reference

FPGA Time         :     0.4240 s
FPGA Throughput   :   839.4765 MB/s
CPU  Time         :    28.9083 s
CPU  Throughput   :    12.3133 MB/s
FPGA Speedup      :    68.1764 x
----------------------------------------------------------------------------
```

### 結果

ホスト アプリケーション コンソールの出力から、FPGA アクセラレーション カーネルが 68x の係数で CPU のみのインプリメンテーションを上回ることがわかります。CPU よりもパフォーマンスが大きく向上します。次の演習では、このパフォーマンスによって 1080p の解像度で 3 つ以上の HD ビデオ チャンネルを並列処理する方法を説明します。このチュートリアルでは、C++ で記述されたカーネルとホスト アプリケーションをビルドして、このようなパフォーマンス向上を達成する方法について説明します。  ホスト アプリケーションは、OpenCL API とザイリンクス ランタイム (XRT) を使用して、このカスタム ビルド ハードウェア カーネルの計算能力を効果的に引き出します。

---------------------------------------


<p align="center"><b>次の演習モジュール: <a href="./lab1_app_introduction_performance_estimation.md">ビデオたたみ込みフィルター: 概要とパフォーマンス見積もり</a><p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p></b></p>
<p align="center"><sup>この資料は 2021 年 2 月 8 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
