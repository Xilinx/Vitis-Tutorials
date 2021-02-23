<p align="right"><a href="https://xilinx.github.io/Vitis-Tutorials/master/docs/index.html">English</a> | <a>日本語</a></p>
<table width="100%">
 <tr width="100%">
    <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ - セクション 2 - システム設定</h1><a href="https://japan.xilinx.com/products/design-tools/vitis.html">xilinx.com の Vitis™ 開発環境を参照</a></td>
 </tr>
</table>
チュートリアルの Vitis 最適化部分に進むには、環境とツールを設定する必要があります。次の手順に従って、Vitis および Alveo U50 アクセラレータカードをインストールして設定してください。

## Vitis ソフトウェア プラットフォームのインストール

[Vitis 統合ソフトウェア開発プラットフォーム](https://japan.xilinx.com/products/design-tools/vitis/vitis-platform.html)からは、エッジ、クラウド、ハイブリッド計算アプリケーションをアクセラレーションする統合プログラミング モデルが提供されます。アプリケーションを開発するには、[ザイリンクス ダウンロード センター](https://japan.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vitis.html)から Vitis コア開発キットをダウンロードしてインストールする必要があります。このチュートリアルでは、Vitis 2020.1 を使用します。

**<u>注記:</u>** Vitis コア開発キットは、Alveo データセンター アクセラレータ カードと同じサーバーにインストールする必要はありません。開発フローのみが必要な場合は、カードをインストールせずにシステムにインストールできますが、Vitis と連動させるには、Alveo カード用に XRT およびターゲット開発プラットフォームをインストールする必要があります。

<details><summary><b>代替ソリューション</b></summary>
<p>また、<a href="https://www.nimbix.net/alveo/">Nimbix</a> などのクラウド サーバー上でアプリケーションを開発および展開することもできます。これには、ハードウェア アクセラレーション コードを開発し、ホスト サーバーおよびインストールされたアクセラレータ カード上で実行するアプリケーションを運用するために必要なすべてが含まれます。</p>
<p><b>[Alveo]</b> → <b>[U50]</b> → <b>[Xilinx Vitis Unified Software Platform 2020.1]</b> をクリックし、Nimbix でコンピュート デスクトップを起動します。また、FPGA を搭載したデスクトップを起動する前に、マシン タイプのプルダウン ウィンドウで [Xilinx U50] を選択してください。</p>
<p>このチュートリアルでは、Vitis の起動を開始する前に、環境変数を Nimbix Desktop の Terminal に設定するか、ソース コードと一緒に提供されている makefile コマンドを実行する必要があります。</p>
<pre><code>export PATH=/opt/xilinx/Vivado/2020.1/tps/lnx64/gcc-6.2.0/bin:$PATH</code></pre>
</details>

## Alveo U50 アクセラレータ カードのインストール

Alveo U50 アクセラレータ カードの詳細は、[Alveo U50 製品ページ](https://japan.xilinx.com/products/boards-and-kits/alveo/u50.html)を参照してください。Alveo U50 データセンター アクセラレータ カードに精通するために必要なすべてのリソースへのリンクと、インストール パッケージ、および RedHat/CentOS と Ubuntu オペレーティング システムの両方の手順が **\[Getting Started]** タブに表示されます。

サーバーへの Alveo U50 アクセラレータ カードのインストール方法は、『Alveo U50 データセンター アクセラレータ カード インストール ガイド』 ([UG1370](https://japan.xilinx.com/cgi-bin/docs/bkdoc?k=accelerator-cards;v=latest;d=ug1370-u50-installation.pdf)) の手順を参照してください。このガイドには、カードの立ち上げおよび検証を含む最新のハードウェアおよびドライバー ソフトウェアのインストール手順のほか、Alveo U50 カードを使用してハードウェア アクセラレーションされたアプリケーションを運用する手順も含まれます。

<details><summary><b>代替ソリューション</b></summary>
<p>また、<a href="https://www.nimbix.net/alveo/">Nimbix</a> などのクラウド サーバー上でアプリケーションを開発および展開することもできます。これには、ハードウェア アクセラレーション コードを開発し、ホスト サーバーおよびインストールされたアクセラレータ カード上で実行するアプリケーションを運用するために必要なすべてが含まれます。</p>
    <p><b>[Alveo]</b> → <b>[U50]</b> → <b>[Xilinx Vitis Unified Software Platform 2020.1]</b> をクリックし、Nimbix でコンピュート デスクトップを起動します。また、FPGA を搭載したデスクトップを起動する前に、マシン タイプのプルダウン ウィンドウで [Xilinx U50] を選択してください。</p>
    <p>このチュートリアルでは、Vitis の起動を開始する前に、環境変数を Nimbix Desktop の Terminal に設定するか、ソース コードと一緒に提供されている makefile コマンドを実行する必要があります。</p>
    <pre><code>export PATH=/opt/xilinx/Vivado/2020.1/tps/lnx64/gcc-6.2.0/bin:$PATH</code></pre>
</details>

## Vitis を実行する環境の設定

Vitis ソフトウェア プラットフォームを実行する環境を設定するには、次のスクリプトを実行して、特定のコマンド シェルで実行する環境を設定します。

~~~
#setup XILINX_VITIS and XILINX_VIVADO variables
source <Vitis_install_path>/Vitis/2020.1/settings64.sh
#setup XILINX_XRT
source /opt/xilinx/xrt/setup.sh
~~~

Alveo U50 アクセラレータ カードのターゲット開発プラットフォームのディレクトリを指定するには、次の環境変数を設定します。

~~~
export PLATFORM_REPO_PATHS=<path to platforms>
~~~

## Alveo U50 アクセラレータ カードの検証

サーバーに Alveo U50 アクセラレータ カードをインストールすると、アプリケーションをホスト サーバーおよびアクセラレータ カードで運用できるようになります。インストールされた Alveo U50 カードを検証するには、『Alveo U50 データセンター アクセラレータ カード インストール ガイド』 ([UG1370](https://japan.xilinx.com/cgi-bin/docs/bkdoc?k=accelerator-cards;v=latest;d=ug1370-u50-installation.pdf)) の第 5 章「カードの立ち上げおよび検証」の手順に従ってください。

**xbutil validate** は、指定した Alveo カードでさまざまなテスト セットを実行して、正しくインストールされるかどうかを検証するコマンドです。検証を実行するには、XRT の環境を設定する必要があります。

~~~
#setup XILINX_XRT
source /opt/xilinx/xrt/setup.sh
#validate Alveo card
xbutil validate
~~~

Alveo U50 カードが正しくインストールされていれば、xbutil validate を実行後に、次に示すような実行されたテストのサマリが表示されます。

~~~
INFO: Found 1 cards
INFO: Validating card[0]: xilinx_u50_gen3x16_xdma_201920_3
INFO: == Starting AUX power connector check:
AUX power connector not available. Skipping validation
INFO: == AUX power connector check SKIPPED
INFO: == Starting PCIE link check:
INFO: == PCIE link check PASSED
INFO: == Starting SC firmware version check:
INFO: == SC firmware version check PASSED
INFO: == Starting verify kernel test:
INFO: == verify kernel test PASSED
INFO: == Starting DMA test:
Host -> PCIe -> FPGA write bandwidth = 11933.1 MB/s
Host <- PCIe <- FPGA read bandwidth = 11966.5 MB/s
INFO: == DMA test PASSED
INFO: == Starting device memory bandwidth test:
...........
Maximum throughput: 52428 MB/s
INFO: == device memory bandwidth test PASSED
INFO: == Starting PCIE peer-to-peer test:
P2P BAR is not enabled. Skipping validation
INFO: == PCIE peer-to-peer test SKIPPED
INFO: == Starting memory-to-memory DMA test:
M2M is not available. Skipping validation
INFO: == memory-to-memory DMA test SKIPPED
INFO: Card[0] validated successfully.
INFO: All cards validated successfully.
~~~

[**セクション 3**](../03-Algorithm_Acceleration/README.md) に進んでください。

</br><hr/>

<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
<p align= center class="sphinxhide"><b><a href="../../../README.md">メイン ページに戻る</a> &mdash; <a href="../../README.md/">ハードウェア アクセラレータ チュートリアルの初めに戻る</a></b></p></br>
<p align="center"><sup>この資料は 2021 年 2 月 8 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
