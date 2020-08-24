<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ アプリケーション アクセラレーション開発フロー チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">2019.2 Vitis アプリケーション アクセラレーション開発フロー チュートリアル</a></td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# 2\. C アプリケーションから Vitis コア開発キット アプリケーションを作成

前の演習で、アプリケーションの `convolve_cpu` 関数に大部分の時間を費やしていることがわかりました。パフォーマンスをアクセラレーションするには、この関数をカーネルに移動して、FPGA アクセラレータ カード上で実行されるようにします。

この演習では、C 関数をハードウェア カーネルに変換し、パフォーマンスを評価するプロセスを説明します。また、後の演習で実行する各最適化の結果と比較するため、アクセラレーション アプリケーションのベースライン パフォーマンス値も求めます。最初の FPGA カーネルを固定画像サイズで実行します。カーネルはハードウェア エミュレーションで実行するので、小さい画像サイズを使用します。ここでは、画像サイズを 512x10 に設定します。

パフォーマンスの計測には、プロファイル サマリおよびタイムライン トレース レポートを使用します。これらのレポートの詳細は、「アクセラレーションされたアプリケーションをビルドして実行するための基本的な概念」チュートリアルの[プロファイルおよびトレース レポートの生成](../Pathway3/ProfileAndTraceReports.md)演習を参照してください。

## カーネル コードの変更

次の手順を使用して、C コードをカーネル コードに変換し、`convolve_cpu` 関数をハードウェア アクセラレーションされたカーネルに変換して FPGA で実行します。

> **ヒント:** カーネル ソース ファイルは `reference-files/baseline` フォルダーに含まれています。必要に応じて、リファレンスとしてご利用ください。

1. `cpu_src` フォルダーの `convolve_kernel.cpp` ファイルを選択し、`convolve_fpga.cpp` という名前のコピーを作成します。

2. `convolve_fpga.cpp` ファイルを開き、`convolve_cpu` 関数の名前を `convolve_fpga` に変更します。

3. 次の HLS INTERFACE プラグマを波かっこで囲んで関数の最初に追加します。

        #pragma HLS INTERFACE s_axilite port=return bundle=control
        #pragma HLS INTERFACE s_axilite port=inFrame bundle=control
        #pragma HLS INTERFACE s_axilite port=outFrame bundle=control
        #pragma HLS INTERFACE s_axilite port=coefficient bundle=control
        #pragma HLS INTERFACE s_axilite port=coefficient_size bundle=control
        #pragma HLS INTERFACE s_axilite port=img_width bundle=control
        #pragma HLS INTERFACE s_axilite port=img_height bundle=control

   FPGA カーネルには、ホスト アプリケーションがカーネルをコンフィギュレーションしてカーネルと通信するための AXI4-Lite スレーブ制御インターフェイス (`s_axilite`) が 1 つ必要です。return を含むすべての関数引数は、専用のプラグマを使用してこの `control` インターフェイスにマップする必要があります。

4. 各ポインター引数も専用のプラグマを使用して AXI メモリ マップド マスター ポート (`m_axi`) にマップする必要があります。関数に次のプラグマを追加します。

        #pragma HLS INTERFACE m_axi port=inFrame offset=slave bundle=gmem1
        #pragma HLS INTERFACE m_axi port=outFrame offset=slave bundle=gmem1
        #pragma HLS INTERFACE m_axi port=coefficient offset=slave bundle=gmem2

   AXI マスター ポートは、カーネルがグローバル メモリのデータにアクセスするために使用されます。データのベース アドレス (`offset`) は、前に定義した制御インターフェイス (`slave`) を使用してカーネルに供給されます。プラグマの bundle プロパティを使用すると、インターフェイス ポートに名前を付けることができます。この例では、関数には 3 つのポインター引数が含まれますが、カーネルには `gmem1` と `gmem2` の 2 つのポートが含まれるようになります。`gmem1` ポートは `inFrame` を読み出して `outFrame` を書き込むのに使用され、`gmem2` ポートは `coefficient` を読み出すのに使用されます。

   > **注記:** プラグマの詳細は、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの [Vitis 環境リファレンス資料](https://japan.xilinx.com/html_docs/xilinx2019_2/vitis_doc/yxl1556143111967.html)を参照してください。

5. `convolve_fpga.cpp` ファイルを保存します。

## ハードウェア エミュレーションの実行

1. 次のコマンドを使用して、`makefile` ディレクトリに移動し、ハードウェア エミュレーションを実行します。

   ```
   cd design/makefile
   make run TARGET=hw_emu STEP=baseline NUM_FRAMES=1
   ```

   > **ヒント:** 各演習の変更は、`src` ディレクトリから加えることができます。この演習で実行するすべての最適化の完成コードもこのディレクトリにあります。結果を実行するだけの場合は、コードを変更する必要はありません。これは、コマンドに `SOLUTION=1` を追加するとイネーブルにできます。
   >
   > ```
   > make run TARGET=hw_emu STEP=baseline SOLUTION=1 NUM_FRAMES=1
   > ```

   アプリケーションの実行中と実行後の両方でコンソールに次のテキストが表示されるはずです。

   ```
   Processed 0.02 MB in 3241.206s (0.00 MBps)

   INFO: [Vitis-EM 22] [Wall clock time: 23:56, Emulation time: 9.17236 ms] Data transfer between kernel(s) and global memory(s)
   convolve_fpga_1:m_axi_gmem1-DDR          RD = 167.781 KB             WR = 20.000 KB
   convolve_fpga_1:m_axi_gmem2-DDR          RD = 167.781 KB             WR = 0.000 KB
   ```

   最初の行は、前の実行のクロック時間とエミュレーション時間を表示します。クロック時間は現在のシステム時間で、エミュレーション時間はこの実行に対してエミュレーターがシミュレーションを実行した時間です。その下の行は、FPGA とオンボード FPGA RAM 間で転送されたデータ サイズを表示します。

## ハードウェア エミュレーションのプロファイル サマリの表示

1. 次のコマンドを使用して、プロファイル サマリ レポートを表示します。

   ```
   make view_run_summary TARGET=hw_emu STEP=baseline
   ```

2. プロファイル サマリ レポートを確認します。[Kernel Execution] セクションから、カーネルの実行時間が 3.903 ms であることがわかります。この値は、各最適化段階の後にパフォーマンスの改善を計測するために使用します。

3. [Kernels to Global Memory] セクションに、各転送の平均バイト数が 4 バイトであることが示されます。これは、パフォーマンスに影響します。

4. 転送数は 91024 であり、必要なデータ転送数に対して多すぎます。次のセクションでは、これらを最適化をします。

5. プロファイル サマリ レポートからのパフォーマンス結果を次の表に記録します。数値は異なる場合があります。

   この演習では、次の情報を記録します。

* **時間**: ハードウェア エミュレーション時間。プロファイル サマリ レポートの \[Top Kernel Execution] セクションの \[Duration] から取得します。
* **カーネル**: カーネルの名前。
* **画像サイズ**: 画像のサイズと形。
* **読み出し**: オンボード RAM から読み出されたバイト数。コンソール ウィンドウに表示され、プロファイル サマリ レポートにリストされます。
* **書き込み**: オンボード RAM に書き込まれたバイト数。コンソール ウィンドウに表示され、プロファイル サマリ レポートにリストされます。
* **読み出し平均**: 各メモリ トランザクションの平均読み出しサイズ。プロファイル サマリ レポートの \[Data Transfer] タブの \[Data Transfer: Kernels and Global Memory] セクションから取得します。
* **書き込み平均**: 各メモリ トランザクションの平均書き込みサイズ。プロファイル サマリ レポートの \[Data Transfer] タブの \[Data Transfer: Kernels and Global Memory] セクションから取得します。
* **帯域幅**: (512x10x4)/時間 で計算されます。

| 手順| 画像サイズ| 時間 (HW-EM) (ms)| 読み出し (KB)| 書き込み (KB)| 読み出し平均 (KB)| 書き込み平均 (KB)| 帯域幅 (MBps)
|:----------|:----------|----------:|----------:|----------:|----------:|----------:|----------:
| baseline| 512x10| 3.903| 344| 20.0| 0.004| 0.004| 5.2

[baseline_hwemu_profilesummary]: ./images/191_baseline_hwemu_pfsummary_new_2.JPG "ベースライン バージョンのハードウェア エミュレーションのプロファイル サマリ"
## 次の手順

この演習では、ハードウェア カーネルを作成し、ハードウェア エミュレーション モードでアプリケーションを実行しました。次の演習では、ホスト アプリケーションとカーネル間のデータ転送を検証し、[メモリ転送を最適化](./localbuf.md)するためのストラテジを決定します。</br>

<hr/>
<p align="center" class="sphinxhide"><b><a href="/docs/vitis-getting-started/README.md">入門コースの初めに戻る</a> &mdash; <a href="/docs/convolution-tutorial/README.md">チュートリアルの初めに戻る</a></b></p>
<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
