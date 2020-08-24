<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ アプリケーション アクセラレーション開発フロー チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">2019.2 Vitis アプリケーション アクセラレーション開発フロー チュートリアル</a></td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# 5\. データフローを使用した最適化

前の演習では、固定小数点型を使用してカーネルの効率を改善しました。この演習では、コードの構造を調べ、タスク レベルと命令レベルの並列処理を増加します。パイプライン処理は、命令レベルで並列処理のためにほとんどのプロセッサで使用される一般的な手法です。パイプライン処理を使用すると、前の命令を終了したハードウェアで命令を実行することにより、プロセッサで複数の命令を同時に処理できるようになります。

Vitis™ コンパイラではほとんどの演算が自動的にパイプライン処理されるので、コードでパイプライン処理をイネーブルにする必要はありません。各ループがどのようにパイプライン処理されたかは、HLS レポートで確認できます。

パイプライン処理は通常命令レベルで適用されますが、Vitis コンパイラではデータフローという変換方法を使用して、関数レベルでパイプライン処理を適用できます。データフローを使用すると、複数の関数をパイプライン処理することにより、それらの命令を異なる反復セットで同時に実行できます。次の図に例を示します。

![][dataflow]

データフローを使用しない場合、`func_A`、`func_B`、`func_C` は順次実行されます。データフローをイネーブルにすると、この 3 つの関数がオーバーラップするので、合計実行時間が短縮されます。

> **ヒント:** この演習は、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの [Vitis ソフトウェア プラットフォームでのアプリケーションのアクセラレーション手法](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=methodologyacceleratingapplications.html)に含まれる[手順 1: コードをロード/計算/ストア パターンに分割](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=methodologyacceleratingapplications.html;a=vhk1551127460562)に関連しています。

この演習では、関数をデータフローでインプリメントするため、まず元のたたみ込み関数を次の 3 つの関数に分割します。

* `read_dataflow`: データを読み出します。
* `compute_dataflow`: 特定の行のたたみ込みを計算します。
* `write_dataflow`: 出力をメイン メモリに書き込みます。

これら 3 つの関数間のデータ転送には、ストリームを使用します。ストリームの詳細は、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの [HLS ストリーム ライブラリ](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=vitishlslibrariesreference.html;a=mes1539734221433)を参照してください。ストリームでは、データを要素レベルで処理できます。ストリームを使用すると、次のモジュールがストリーム変数に挿入されるとすぐに要素の処理が開始します。

## カーネル コードの変更

> **ヒント:** カーネル ソース ファイルは `reference-files/dataflow` フォルダーに含まれています。必要に応じて、リファレンスとしてご利用ください。

1. `src/dataflow` ディレクトリにある `convolve_fpga.cpp` ファイルを開きます。

2. カーネルの最上位関数である `convolve_fpga` 関数に移動します。この関数の構造を、次に示すように 3 つのサブ関数を呼び出すよう変更します。

   ```
   void convolve_fpga(const RGBPixel* inFrame, RGBPixel* outFrame,
                      const float* coefficient, int coefficient_size,
                      int img_width, int img_height)
   {
     int half = COEFFICIENT_SIZE / 2;

     hls::stream<RGBPixel> read_stream("read");
     hls::stream<RGBPixel> write_stream("write");
     int elements = img_width * img_height;

   #pragma HLS dataflow
     read_dataflow(read_stream, inFrame, img_width, elements, half);
     compute_dataflow(write_stream, read_stream, coefficient, img_width, elements, half);
     write_dataflow(outFrame, write_stream, elements);  
   }  
   ```

   3 つのサブ関数が呼び出され、ストリーム型オブジェクト `read_stream` および `write_stream` によりデータが渡されます。これらのストリームは FPGA 上の FIFO を使用してインプリメントされます。dataflow プラグマが使用されているので、3 つの関数が並列に実行されます。dataflow プラグマの詳細は、『Vivado Design Suite ユーザー ガイド: 高位合成』 ([UG902](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;d=ug902-vivado-high-level-synthesis.pdf)) を参照してください。

3. 3 つのサブ関数を 1 つずつ作成します。まず、`read_dataflow` 関数から始めます。

   この関数は、グローバル メモリからデータを読み出し、そのデータをストリームを使用して次の関数に渡します。範囲外からデータを読み出さないように、画像の下部にパディング ピクセルを追加するためストリームに 0 を挿入します。行インデックスが image_height より小さい場合、入力画像から行を読み出し、read_stream ストリームに格納します。

   完成した関数は、次のようになります。

   ```
   void read_dataflow(hls::stream<RGBPixel>& read_stream, const RGBPixel *in,
                      int img_width, int elements, int half) {
   int pixel = 0;
   while(elements--) {
         read_stream <<  in[pixel++];
   }
   int padding = img_width * half + COEFFICIENT_SIZE;
   while(padding--) {
         read_stream << zero;
   }
   }
   ```

   hls::stream オブジェクトは `hls_stream.h` ヘッダーで定義されます。値をストリームに挿入するには、`<<` 演算子を使用します。

4. `compute_dataflow` 関数を作成します。これは、`read_flow` 関数からストリーミング データを読み込んで計算するので、最も複雑な関数です。出力データは `write_stream` に書き込まれます。

   `compute_dataflow` のコード部分は、次のとおりです。

   ```
   void compute_dataflow(hls::stream<RGBPixel>& write_stream, hls::stream<RGBPixel>& read_stream,
                         const float* coefficient, int img_width, int elements, int center) {
   static RGBPixel window_mem[COEFFICIENT_SIZE][MAX_WIDTH];
   #pragma HLS data_pack variable=window_mem
   #pragma HLS array_partition variable=window_mem complete dim=1
   static fixed coef[COEFFICIENT_SIZE * COEFFICIENT_SIZE];
   #pragma HLS array_partition variable=coef complete

       for(int i  = 0; i < COEFFICIENT_SIZE*COEFFICIENT_SIZE; i++) {
           coef[i] = coefficient[i];
       }

       int line_idx = 0;
       while(line_idx < center) {
           for(int i = 0; i < img_width; i++) {
               window_mem[line_idx][i] = zero;
           }
           line_idx++;
       }

       while(line_idx < COEFFICIENT_SIZE - 1) {
           for(int ii = 0; ii < img_width; ii++) {
               read_stream >> window_mem[line_idx][ii];
           }
           line_idx++;
       }

       for(int ii = 0; ii < COEFFICIENT_SIZE; ii++) {
           read_stream >> window_mem[line_idx][ii];
       }

       int top_idx = 0;
       int insert_idx = line_idx;
       int window_line_idx = top_idx;
       int j = 0;
       int insert_column_idx = COEFFICIENT_SIZE;
       while(elements--) {
           fixed sum_r = 0, sum_g=0, sum_b=0;
           for(int m = 0; m < COEFFICIENT_SIZE; ++m) {
               for(int n = 0; n < COEFFICIENT_SIZE; ++n) {
                   int jj = j + n - center;
                   RGBPixel tmp = (jj >= 0 && jj < img_width) ? window_mem[window_line_idx][jj] : zero;
                   fixed coef_tmp = coef[m * COEFFICIENT_SIZE + n] * (jj >= 0 && jj < img_width);
                   sum_r += tmp.r * coef_tmp;
                   sum_g += tmp.g * coef_tmp;
                   sum_b += tmp.b * coef_tmp;
               }
               window_line_idx = ((window_line_idx + 1) == COEFFICIENT_SIZE) ? 0 : window_line_idx + 1;
           }
           window_line_idx = top_idx;
           RGBPixel out = {sum_r.to_int(), sum_g.to_int(), sum_b.to_int(), 0};
           write_stream << out;
           j++;
           if(j >= img_width) {
               j = 0;
               top_idx = ((top_idx + 1) == COEFFICIENT_SIZE) ? 0 : top_idx + 1;
               window_line_idx = top_idx;
           }
           read_stream >> window_mem[insert_idx][insert_column_idx++];
           if (insert_column_idx >= img_width) {
               insert_column_idx = 0;
               insert_idx = ((insert_idx + 1) == COEFFICIENT_SIZE) ? 0 : insert_idx + 1;
           }
       }
   }
   ```

   ローカル バッファー `window_mem` は、計算に必要なデータの行を格納します。ストリームから要素を抽出するには、`>>` 演算子を使用します。`window_mem` 配列の冒頭の数行は画像のパディングを示すために 0 にする必要があります。これは、最初の反復でのみ実行されます。

   計算はメイン ループにインプリメントされ、COEFFICIENT\_SIZE 変数がコンパイル時に定義されるので、ループがパイプライン処理されます。ループ最適化の詳細は、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの [Vitis ソフトウェア プラットフォームでのアプリケーションのアクセラレーション手法](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=methodologyacceleratingapplications.html)に含まれる[手順 4: ループ レイテンシの改善](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=methodologyacceleratingapplications.html;a=wnz1555544738414)を参照してください。

   計算が終了すると、`out` 値がダウンストリーム関数の `write_stream` にプッシュされて処理されます。

5. `write_dataflow` 関数を作成します。

   ```
   void write_dataflow(RGBPixel* outFrame, hls::stream<RGBPixel>& write_stream,
                       int elements) {
     int pixel = 0;
     while(elements--) {
         write_stream >> outFrame[pixel++];
     }
   }
   ```

   この関数は単純で、ストリーミング結果をグローバル メモリに出力するだけです。

## データフローを使用した場合のハードウェア エミュレーションの実行

`makefile` ディレクトリに移動し、次のコマンドを使用してハードウェア エミュレーションを実行します。

```
make run TARGET=hw_emu STEP=dataflow SOLUTION=1 NUM_FRAMES=1
```

次のような結果になるはずです。

```
Processed 0.02 MB in 29.728s (0.00 MBps)

INFO: [Vitis-EM 22] [Wall clock time: 23:03, Emulation time: 0.108257 ms] Data transfer between kernel(s) and global memory(s)
convolve_fpga_1:m_axi_gmem1-DDR[0]          RD = 20.000 KB              WR = 0.000 KB
convolve_fpga_1:m_axi_gmem2-DDR[0]          RD = 0.000 KB               WR = 20.000 KB
convolve_fpga_1:m_axi_gmem3-DDR[0]          RD = 0.035 KB               WR = 0.000 KB
```

## ハードウェア エミュレーションのプロファイル サマリ レポートの表示

次のコマンドを使用して、プロファイル サマリ レポートを表示します。

```
make view_run_summary TARGET=hw_emu STEP=dataflow
```

カーネル実行時間が 0.059 ms に短縮されています。

アップデートされた表は、次のようになります。

| 手順| 画像サイズ| 時間 (HW-EM) (ms)| 読み出し (KB)| 書き込み (KB)| 読み出し平均 (KB)| 書き込み平均 (KB)| 帯域幅 (Mbps)
|:----------|:----------|----------:|----------:|----------:|----------:|----------:|----------:
| baseline| 512x10| 3.903| 344| 20.0| 0.004| 0.004| 5.2
| localbuf| 512x10| 1.574 (2.48x)| 21 (0.12x)| 20.0| 0.064| 0.064| 13
| fixedpoint| 512x10| 0.46 (3.4x)| 21| 20.0| 0.064| 0.064| 44
| dataflow| 512x10| 0.059 (7.8x)| 21| 20.0| 0.064| 0.064| 347

## 次の手順

この演習では、ハードウェア カーネルに対していくつかの最適化を実行し、パフォーマンスを改善しました。次のセクションでは、[順不同キューと複数の計算ユニットを使用](./multi-CU.md)するなどのほかのホスト コード最適化を実行します。

</br>

[function_pipeline]: ./images/4_function_pipelining.png "関数パイプライン"
[dataflow]: ./images/dataflow.png "データフロー"
[dataflow_hwemu_profilesummary]: ./images/191_dataflow_hwemu_pfsummary_new_2.jpg "データフロー バージョンのハードウェア エミュレーションのプロファイル サマリ"
<hr/>
<p align="center" class="sphinxhide"><b><a href="/docs/vitis-getting-started/README.md">入門コースの初めに戻る</a> &mdash; <a href="/docs/convolution-tutorial/README.md">チュートリアルの初めに戻る</a></b></p>
<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
