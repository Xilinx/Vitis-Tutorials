<table>
 <tr>
   <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2019.2 Vitis™ アプリケーション アクセラレーション開発フローのチュートリアル</h1><a href="https://github.com/Xilinx/SDAccel-Tutorials/branches/all">SDAccel™ 開発環境 2019.1 チュートリアルを参照</a></td>
 </tr>
 <tr>
 <td align="center"><h1>アクセラレーションした FPGA アプリケーションの最適化: たたみ込みの例</td>
 </tr>
</table>

# 2\. C アプリケーションから Vitis コア開発キット アプリケーションを作成

前の演習では、アプリケーション内の大部分の時間を費やしているのが `convolve_cpu` 関数であることがわかりました。パフォーマンスをアクセラレーションするには、この関数をカーネルに移動して、FPGA アクセラレータ カード上で実行されるようにします。

この演習では、C 関数をハードウェア カーネルに変換して、パフォーマンスを評価するプロセスについて説明します。また、アクセラレーションされるアプリケーションのベースライン パフォーマンスの数値も判明させて、この後の演習で追加していく最適化と比較するために使用します。最初の FPGA カーネルを固定画像サイズで実行します。カーネルはハードウェア エミュレーションで実行するので、より小さな画像サイズを使用します。この段階では、画像サイズを 512x10 に設定します。

パフォーマンスを測定するには、プロファイル サマリおよびタイムライン トレース レポートを使用します。これらのレポートの詳細は、「アクセラレーションされたアプリケーションをビルドして実行するための基本的な概念」チュートリアルの[プロファイルおよびトレース レポートの生成](../Pathway3/ProfileAndTraceReports.md)演習を参照してください。

## カーネル コードの変更

次の手順で C コードをカーネル コードに変換し、`convolve_cpu` 関数をハードウェア アクセラレーションされたカーネルに変換して、FPGA で実行します。

> **ヒント:** カーネル ソース ファイルは `reference-files/baseline` フォルダーに含まれています。必要に応じて、リファレンスとしてご利用ください。

1. `cpu_src` フォルダーの `convolve_kernel.cpp` ファイルを選択して、`convolve_fpga.cpp` という名前のコピーを作成します。

2. `convolve_fpga.cpp` ファイルを開いて、`convolve_cpu` 関数を `convolve_fpga` という名前に変更します。

3. 次の HLS INTERFACE プラグマを波かっこで囲んで関数の最初に追加します。

        #pragma HLS INTERFACE s_axilite port=return bundle=control
        #pragma HLS INTERFACE s_axilite port=inFrame bundle=control
        #pragma HLS INTERFACE s_axilite port=outFrame bundle=control
        #pragma HLS INTERFACE s_axilite port=coefficient bundle=control
        #pragma HLS INTERFACE s_axilite port=coefficient_size bundle=control
        #pragma HLS INTERFACE s_axilite port=img_width bundle=control
        #pragma HLS INTERFACE s_axilite port=img_height bundle=control

   FPGA カーネルには、ホスト アプリケーションが設定されてカーネルと通信できるようにする AXI4-Lite スレーブ制御インターフェイス (`s_axilite`) が 1 つ必要です。return も含めたすべての関数引数は、専用のプラグマを使用してこの `control` インターフェイスにマップされる必要があります。

4. また、各ポインター引数も専用のプラグマを使用して AXI メモリ マップド マスター ポート (`m_axi`) にマップする必要があります。次のプラグマを関数に追加します。

        #pragma HLS INTERFACE m_axi port=inFrame offset=slave bundle=gmem1
        #pragma HLS INTERFACE m_axi port=outFrame offset=slave bundle=gmem1
        #pragma HLS INTERFACE m_axi port=coefficient offset=slave bundle=gmem2

   AXI マスター ポートはグローバル メモリのデータにアクセスするためにカーネルに使用されます。データ (`offset`) のベース アドレスは前に定義した制御インターフェイス (`slave`) を使用してカーネルに提供されます。プラグマの bundle プロパティを使用すると、インターフェイス ポートに名前を付けることができます。この例の場合、関数に 3 つのポインター引数が含まれますが、カーネルには `gmem1` と `gmem2` の 2 つのポートが含まれるようになります。`gmem1` ポートは、`inFrame` を読み出し、`outFrame` を書き込むのに使用され、`gmem2` ポートは `coefficient` を読み出すのに使用されます。

5. 最後に、`inFrame` および `outFrame` 引数に `data_pack` プラグマを追加します。このプラグマは、コンパイラに struct のエレメントすべてを 1 つの幅の広いベクターに集めるように伝えて、struct のすべてのメンバーが同時にアクセスできるようにします。

        #pragma HLS data_pack variable=inFrame
        #pragma HLS data_pack variable=outFrame

   > **注記:** プラグマの意味については、[Vitis 環境リファレンス マニュアル](https://japan.xilinx.com/html_docs/xilinx2019_2/vitis_doc/yxl1556143111967.html)を参照してください。

6. `convolve_fpga.cpp` ファイルを保存します。

## ハードウェア エミュレーションの実行

1. `makefile` ディレクトリに移動して、次のコマンドでハードウェア エミュレーションを実行します。

   ```
   cd design/makefile
   make run TARGET=hw_emu STEP=baseline SOLUTION=1 NUM_FRAMES=1
   ```

   > **ヒント:** 各演習に基づいた変更は、`src` ディレクトリで実行できます。この演習のすべての最適化に対応するすべてのコードも含まれます。結果を実行するだけの場合は、コードを変更する必要はありません。これは `SOLUTION=1` に設定するとイネーブルにできます。

   アプリケーションの実行中と実行後の両方でコンソールに次のテキストが表示されるはずです。

   ```
   Processed 0.02 MB in 3241.206s (0.00 MBps)

   INFO: [Vitis-EM 22] [Wall clock time: 23:56, Emulation time: 9.17236 ms] Data transfer between kernel(s) and global memory(s)
   convolve_fpga_1:m_axi_gmem1-DDR          RD = 167.781 KB             WR = 20.000 KB
   convolve_fpga_1:m_axi_gmem2-DDR          RD = 167.781 KB             WR = 0.000 KB
   ```

   最初の行は、前の実行のクロック時間とエミュレーション時間を表示します。クロック時間は現在のシステム時間で、エミュレーション時間はこの実行に対してエミュレーターがシミュレーションした時間です。次の行は、FPGA とオンボード FPGA RAM 間で転送されたデータ サイズを表示します。

## ハードウェア エミュレーションのプロファイル サマリの表示

1. 次のコマンドを使用して、プロファイル サマリ レポートを表示します。

   ```
   make view_run_summary TARGET=hw_emu STEP=baseline
   ```

2. では、プロファイル サマリ レポートを見てみます。「Kernel Execution」セクションからは、カーネル実行時間が 3.903 ms であることがわかります。この数値は、それぞれの最適化段階の後にパフォーマンスの改善を測定するために使用されます。

3. 「Kernels to Global Memory」セクションには、各転送の平均バイトが 4 バイトだけで、パフォーマンスに影響していることが示されます。

4. また、転送数は 91024 なので、必要なデータ転送回数よりも多すぎるようです。次のセクションでは、これらのエリアで最適化をします。

5. プロファイル サマリ レポートからのパフォーマンス結果を記録して、次の表を埋めてください。数値は、場合によって異なることもあります。

   この演習では、次の情報を記録します。

* **Time**: ハードウェア エミュレーション時間。「Duration」の下の「Top Kernel Execution」セクションにリストされるプロファイル サマリ レポートに記述されています。
* **Kernel**: カーネルの名前。
* **Image Size**: イメージのサイズおよび形。
* **Reads**: オンボード RAM から読み出されたバイト数。コンソール ウィンドウに表示され、プロファイル サマリ レポートにリストされます。
* **Writes**: オンボード RAM に書き込まれたバイト数。コンソール ウィンドウに表示され、プロファイル サマリ レポートにリストされます。
* **Avg. Read**: 各メモリ トランザクションの平均読み出しサイズ。「Data Transfer: Kernels and Global Memory」セクションの \[Data Transfer] タブのプロファイル サマリ レポートに記述されています。
* **Avg. Write**: 各メモリ トランザクションの平均書き込みサイズ。「Data Transfer: Kernels and Global Memory」セクションの \[Data Transfer] タブのプロファイル サマリ レポートに記述されています。
* **BW**: 帯域幅は (512x10x4)/Time で計算されます。

| Step          | Image Size | Time (HW-EM)(ms) | Reads (KB) | Writes (KB) | Avg. Read (KB) | Avg. Write (KB) | BW (MBps) |
| :------------ | :--------- | ---------------: | ---------: | ----------: | -------------: | --------------: | --------: |
| baseline      | 512x10     | 3.903            | 344        | 20.0        | 0.004          | 0.004           |   5.2     |
---------------------------------------

[baseline_hwemu_profilesummary]: ./images/191_baseline_hwemu_pfsummary_new_2.JPG "ベースライン バージョンのハードウェア エミュレーションのプロファイル サマリ"

## 次のステップ

ここまでで、ハードウェア カーネルを作成して、ハードウェア エミュレーション モードでアプリケーションを実行しました。次は、ホスト アプリケーションとカーネル間のデータ転送の検証を開始し、[メモリ転送を最適化](./localbuf.md)するためのストラテジを決定します。</br>

<hr/>
<p align="center"><b><a href="../vitis-getting-started/README.md">入門コースの初めに戻る</a> &mdash; <a href="README.md">チュートリアルの初めに戻る</a></b></p>
<p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>

この資料は表記のバージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。
