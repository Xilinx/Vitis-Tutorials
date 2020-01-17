<table>
 <tr>
   <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2019.2 Vitis™ アプリケーション アクセラレーション開発フローのチュートリアル</h1><a href="https://github.com/Xilinx/SDAccel-Tutorials/branches/all">SDAccel™ 開発環境 2019.1 チュートリアルを参照</a></td>
 </tr>
 <tr>
 <td align="center"><h1>アクセラレーションした FPGA アプリケーションの最適化: たたみ込みの例</td>
 </tr>
</table>

# 5\. データフローを使用して最適化

前の演習では、固定小数点型を使用してカーネルの効率を改善しました。この演習では、コードの構造を見て、タスク レベルと命令レベルの並列処理を増加します。パイプライン処理は、命令レベルで並列化をするためにほとんどのプロセッサで使用される共通の手法です。パイプライン処理すると、前の命令を終了したハードウェアで命令を実行することで、プロセッサが複数の命令を同時に処理できるようになります。

Vitis™ コンパイラは自動的にほとんどの演算をパイプライン処理しますので、コード内でパイプラインをイネーブルにするために何かする必要はありません。各ループがどのようにパイプライン処理されたかは、HLS レポートで確認できます。

パイプラインは通常命令レベルで機能しますが、Vitis コンパイラでは dataflow という変換方法を使用して、関数レベルでパイプライン処理することもできます。dataflow を使用すると複数の関数をパイプライン処理できるので、別の反復セットでそれらの命令を同時に実行できます。次の図に例を示します。

![][dataflow]

dataflow を使用しない場合、`func_A`、`func_B`、`func_C` は順次実行されます。dataflow をイネーブルにすると、この 3 つの関数が重複されるので、実行時間合計が減ります。

> **ヒント:** この演習は、[Vitis 統合ソフトウェア プラットフォームを使用してアプリケーションをアクセラレーションする設計手法](https://japan.xilinx.com/html_docs/xilinx2019_2/vitis_doc/Chunk1821279816.html#wgb1568690490380)の[手順 1: コードの load-compute-store パターンへの分割](https://japan.xilinx.com/html_docs/xilinx2019_2/vitis_doc/Chunk1821279816.html#vhk1551127460562)に関連しています。

この演習では、関数を dataflow でインプリメントするために、まず元のたたみ込み関数を次の 3 つの別々の関数に分割します。

* `read_dataflow`: データを読み出します。
* `compute_dataflow`: 特定行のたたみ込みを計算します。
* `write_dataflow`: 出力をメイン メモリに書き込みます。

これら 3 つの関数は、互いにデータを渡すのにストリームを使用します。ストリームについては、『Vivado Design Suite ユーザー ガイド: 高位合成』 ([UG902](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=latest;d=ug902-vivado-high-level-synthesis.pdf)) の「HLS ストリーム ライブラリ」を参照してください。ストリームでは、データをデータのエレメント レベルで処理できます。ストリームを使用する場合、次のモジュールがストリーム変数に挿入されると、即座にエレメントが処理され始めます。

## カーネル コードの変更

> **ヒント:** カーネル ソース ファイルは `reference-files/dataflow` フォルダーに含まれています。必要に応じて、リファレンスとしてご利用ください。

1. `src/dataflow` から `convolve_fpga.cpp` ファイルを開きます。

2. まず、カーネルの最上位関数である `convolve_fpga` 関数に移動します。次のように、この関数の構造を 3 つのサブ関数を呼び出すように変更します。

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

   3 つのサブ関数が呼び出され、ストリーム オブジェクト型の `read_stream` および `write_stream` によりデータが渡されます。これらのストリームは FPGA 上の FIFO を使用してインプリメントされます。DATAFLOW プラグマが使用されているので、3 つの関数が並列に実行されます。dataflow プラグマの詳細は、『Vivado Design Suite ユーザー ガイド: 高位合成』 ([UG902](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=latest;d=ug902-vivado-high-level-synthesis.pdf)) を参照してください。

3. 次は、3 つのサブ関数を 1 つずつ作成します。まず、`read_dataflow` 関数から始めます。

   この関数は、グローバル メモリからデータを読み出し、そのデータをストリームを使用して次の関数に渡します。範囲外からデータを読み出さないように、ストリームに 0 を挿入して、画像の一番下にパディング ピクセルを追加します。行番号が image\_height より小さい場合、入力画像から行を読み出して、それを read\_stream ストリームに格納します。

   すべての関数は、次のようになります。

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

   hls::stream オブジェクトは `hls_stream.h` ヘッダーでｄ定義されます。値をストリームに挿入するには、`<<` 演算子を使用します。

4. では、次に `compute_dataflow` 関数を作成します。これは、`read_flow` 関数からストリーミング データを読み込んで計算するので、最も複雑な関数です。出力データは `write_stream` に書き込まれます。

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

   ローカル バッファー `window_mem` は、計算に必要なデータの行を格納するように定義されます。ストリームからエレメントを抽出するには、`>>` 演算子を使用します。`window_mem` 配列の冒頭の数行は画像のパディングを示すために 0 にする必要があります。これは、最初の反復でのみ実行されます。

   計算がメイン ループにインプリメントされ、COEFFICIENT\_SIZE 変数がコンパイル時に前もって定義されるので、ループがパイプライン処理されます。ループの最適化の詳細は、[Vitis 統合ソフトウェア プラットフォームでのアプリケーション アクセラレーション手法](https://japan.xilinx.com/html_docs/xilinx2019_2/vitis_doc/Chunk1821279816.html#wgb1568690490380)の[手順 4: ループ レイテンシの改善](https://japan.xilinx.com/html_docs/xilinx2019_2/vitis_doc/Chunk1821279816.html#wnz1555544738414)を参照してください。

   計算が終了したら、`out` 値がダウンストリーム関数の `write_stream` にプッシュされて処理されます。

5. では、次に `write_dataflow` 関数を作成します。

   ```
   void write_dataflow(RGBPixel* outFrame, hls::stream<RGBPixel>& write_stream,
                       int elements) {
     int pixel = 0;
     while(elements--) {
         write_stream >> outFrame[pixel++];
     }
   }
   ```

   この関数はかなり単純で、ストリーミング結果をグローバル メモリに出力するだけのものです。

## ハードウェア エミュレーションの実行 (データフローを使用した場合)

`makefile` ディレクトリに移動して、次のコマンドでハードウェア エミュレーションを実行します。

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

カーネル実行時間が 0.059 ms に減っています。

アップデートされた表は、次のようになります。

| 演習               | Image Size | Time (HW-EM)(ms) | Reads(KB)       | Writes(KB) | Avg. Read (KB) | Avg. Write (KB) | BW (MBps)  |
| :---------------   | :--------- | ---------------: | --------------: | ---------: | -------------: | --------------: | ---------: |
| baseline           |     512x10 | 3.903            | 344             |       20.0 |          0.004 |           0.004 |    5.2     |
| localbuf           |     512x10 | 1.574 (2.48x)    | 21  (0.12x)     |       20.0 |          0.064 |           0.064 |    13      |
| fixed-point data   |     512x10 | 0.46 (3.4x)      | 21              |       20.0 |          0.064 |           0.064 |    44      |
| dataflow           |     512x10 | 0.059 (7.8x)     | 21              |       20.0 |          0.064 |           0.064 |    347     |
---------------------------------------

## 次のステップ

ここまでで、ハードウェア カーネルで最適化をいくつか実行して、パフォーマンスを改善しました。次は、[順不同キューと複数の計算ユニットを使用](./multi-CU.md)したりする別のホスト コード最適化を実行します。

</br>

[function_pipeline]: ./images/4_function_pipelining.png "関数パイプライン"
[dataflow]: ./images/dataflow.png "データフロー"
[dataflow_hwemu_profilesummary]: ./images/191_dataflow_hwemu_pfsummary_new_2.jpg "データフロー バージョンのハードウェア エミュレーションのプロファイル サマリ"
<hr/>
<p align="center"><b><a href="../../docs/vitis-getting-started/README.md">入門コースの初めに戻る</a> &mdash; <a href="./README.md">チュートリアルの初めに戻る</a></b></p>
<p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>
