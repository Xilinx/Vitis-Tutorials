<table>
 <tr>
   <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2019.2 Vitis™ アプリケーション アクセラレーション開発フローのチュートリアル</h1><a href="https://github.com/Xilinx/SDAccel-Tutorials/branches/all">SDAccel™ 開発環境 2019.1 チュートリアルを参照</a></td>
 </tr>
 <tr>
 <td align="center"><h1>アクセラレーションした FPGA アプリケーションの最適化: たたみ込みの例</td>
 </tr>
</table>

# 3\. メモリ転送の最適化

前の演習では、1 つのフレームを使用してアプリケーションのベースライン パフォーマンスを測定しました。この演習では、余分なメモリ アクセスをなくして、カーネルとグローバル メモリ間のデータ移動を最適化します。

ハードウェア エミュレーション実行では、カーネルがサイズ 512x10 の画像を処理します。各ピクセルは、RGBA 値を表す 4 バイトで構成されます。512x10 の画像の場合、毎フレーム 20 KB を処理することになります。処理されたレポートには、アプリケーションがカーネルに (167.781+167.781=335.562) KB のデータを読み込んだことも示されているので、20 KB 入力フレームよりもかなり多くなっています。これは、1 ピクセルを処理するたびにフィルター係数すべてと 9 ピクセルを読み込んで計算を実行するからです。安定した状態では、処理のために 9 ピクセル中 8 ピクセルが既に読み込まれています。

ほとんどのプロセッサにはハードウェア制御のキャッシュがあり、かなり高速なローカル キャッシュにデータが自動的に格納されるようになるので、これは近代的な CPU では問題にならないこともありますが、FPGA では、データを作成してハードウェア カーネルのローカル配列にコピーして明示的に実行する必要があります。

同じ方法を使用すると、ローカル配列に画像の一部を読み込み、グローバル メモリではなく、ローカル データでたたみ込みを実行できます。フィルター幅と同じ行分読み込んでから、これらの行でたたみ込みを実行し、その行の結果をグローバル メモリに書き込みます。この後、次の行だけをローカル メモリの次のスロットに読み込んで、次の動作を実行します。次の図は、このプロセスを示しています。

![][convolve_fpga_local]

最初の動作では、3 行をローカル メモリに読み込んで、その 3 行の動作を実行します。結果をグローバル メモリに格納し戻し、ローカル メモリ配列の 4 行目で次の行を読み出します。バッファーの終わりに到達したら、ローカル メモリ配列の一番上に戻って、そこに書き込みます。

## カーネル コードの変更

> **ヒント:** カーネル ソース ファイルは `reference-files/localbuf` フォルダーに含まれています。必要に応じて、リファレンスとしてご利用ください。

`src/localbuf` フォルダーの `convolve_fpga.cpp` ファイルを開いて、次のように変更します。

1. ファイルの一番上に `cstring` ヘッダーを追加し、`memcpy` 関数の宣言を追加します。

        #include <cstring>

2. 関数本体の最後のプラグマの後に次を追加します。

        float coef[MAX_FILTER * MAX_FILTER];
        memcpy(coef, coefficient, coefficient_size * coefficient_size * sizeof(float));

   これらの行はフィルター係数を格納する `coef` 配列を作成してから、関数に渡される `coefficient` 配列からのデータをコピーします。`coef` 配列は、ブロック RAM リソースを使用して FPGA で合成されます。ブロック RAM へのアクセスのレイテンシは、オンボード DDR メモリよりもかなり短くなります。

3. これで `coef` というキャッシュ配列を作成できたので、その配列を使用するように残りのコードを変更する必要があります。内部ループの次の行 (48 ～ 50 行目) を見つけます。

        sum_r += inFrame[(ii * img_width) + jj].r * coefficient[(m * coefficient_size) + n];
        sum_g += inFrame[(ii * img_width) + jj].g * coefficient[(m * coefficient_size) + n];
        sum_b += inFrame[(ii * img_width) + jj].b * coefficient[(m * coefficient_size) + n];

   これを次のように変更します。

        sum_r += inFrame[(ii * img_width) + jj].r * coef[(m * coefficient_size) + n];
        sum_g += inFrame[(ii * img_width) + jj].g * coef[(m * coefficient_size) + n];
        sum_b += inFrame[(ii * img_width) + jj].b * coef[(m * coefficient_size) + n];

4. `MAX_FILTER` は、カーネルが処理する最大フィルターを指定します。`MAX_WIDTH` は、このカーネルが処理する画像の最大幅です。これらは `constants.h` ヘッダーで設定され、大きなサイズを処理するように変更できます。この例の場合、`MAX_FILTER` は 19 で、`MAX_WIDTH` は 1920 です。入力ピクセルと出力ピクセルのローカル配列を作成し、`MAX_FILTER` 行のみを格納します。これは、各反復でたたみ込みを実行するのに少なくともその行数分が必要になるからです。`for` ループ (34 行目) の前に次の行を含めます。

        RGBPixel window_mem[MAX_FILTER][MAX_WIDTH];
        RGBPixel out_line[MAX_WIDTH];
        #pragma HLS data_pack variable=window_mem
        #pragma HLS data_pack variable=out_line

   `window_mem` 変数は入力ピクセルを格納します。`out_line` 変数は出力ピクセルを格納します。

5. 次に、配列の最初の半分を 0 で埋めて、境界が考慮されるようにします。前の手順で追加した行の下に次の行を含めます。

        static const RGBPixel zero = {0, 0, 0, 0};
        int center = coefficient_size / 2;
        for(int line = 0; line < center; line++) {
            for(int pixel = 0; pixel < MAX_WIDTH; pixel++) {
                window_mem[line][pixel] = zero;
            }
        }

   この場合、`center` がフィルターの中央ピクセルの位置で、カーネルの範囲と同じになります。

6. 配列の後半の半分を画像の最初の数行で埋めます。次の `for` ループを最後の行に含めます。手順 5 の行の下に次のコードを含めます。

        for(int line = center; line < coefficient_size-1; line++) {
            memcpy(window_mem[line], inFrame + ((line-1) * img_width), img_width * sizeof(RGBPixel));
        }

7. 画像の高さを反復する `for` ループの場合、まず画像の次の行をローカル配列に読み込みます。また、画像の終わりにパディングを追加することも考慮する必要があるので、画像の一番下まで到達したかどうかを確認するチェックを追加して、配列に 0 を挿入する必要があります。外部ループを次のように変更します (38 行目)。

        for(int line = 0; line < img_height; ++line) {
            int next_line = (line + coefficient_size - 1) % MAX_FILTER;
            if(line + 1 < img_height) {
                memcpy(window_mem[next_line], inFrame + ((line+center) * img_width), img_width * sizeof(RGBPixel));
            }
            else {
                for(int pixel = 0; pixel < MAX_WIDTH; pixel++) {
                    window_mem[next_line][pixel] = zero;
                }
            }
            ...

   `next_line` 変数は、ウィンドウのどこに入力画像の次の行を格納するかを追跡します。

8. `window_mem` のどの行をたたみ込みに読み込むのかも追跡する必要もあります。ここでの計算では `window_line_idx` 変数を使用して、入力画像の行をローカル配列の `window_mem` 行にマップします。`top_idx` 変数は、`window_mem` 配列の最初の行を指定するために使用されます。手順 7 で追加した行の下に次のコードを含めます。

        int window_line_idx = next_line - center - 1;
        if(window_line_idx < 0) window_line_idx += MAX_FILTER;
        int top_idx = window_line_idx;

   ![][convolve_variables]

9. 次に、`inFrame` パラメーターではなく、新しい `window_mem` 配列を使用するようにたたみ込み演算を変更します。次の内部ループ (47 行目以降) の計算を見つけます。

        int ii = line + m - center;
        int jj = pixel + n - center;

        if(ii >= 0 && ii < img_height && jj >= 0 && jj < img_width)
        {
            sum_r += inFrame[(ii * img_width) + jj].r * coef[(m * coefficient_size) + n];
            sum_g += inFrame[(ii * img_width) + jj].g * coef[(m * coefficient_size) + n];
            sum_b += inFrame[(ii * img_width) + jj].b * coef[(m * coefficient_size) + n];
        }

   これを次のように変更します。

        int jj = pixel + n - center;
        if(jj >= 0 && jj < img_width)
        {
            sum_r += window_mem[window_line_idx][jj].r * coef[m * coefficient_size + n];
            sum_g += window_mem[window_line_idx][jj].g * coef[m * coefficient_size + n];
            sum_b += window_mem[window_line_idx][jj].b * coef[m * coefficient_size + n];
        }

   `window_line_idx` 変数を使用して `ii` ではなく `window_mem` 変数にインデックスを付けています。

10. `window_line_idx` 変数は `m` 変数をアップデートするたびにインクリメントされるようにする必要があります。ローカル配列の端を読み込んだ後、最初の行に戻るようにする必要もあります。これは、次のコードで実行されます。

        window_line_idx=(window_line_idx + 1) == MAX_FILTER ? 0 : window_line_idx + 1;

    これは、`window_line_idx = (window_line_idx + 1) % MAX_FILTER;` と同じですが、計算負荷の高い剰余演算を回避しています。

    これは、`m` 変数をアップデートするループの終わりに含める必要があります。

11. 指定のピクセルのたたみ込みを計算したら、`window_line_idx` 変数を `top_idx` にリセットし、結果を `out_line` 配列に書き込みます。`outFrame` への書き込みを `out_line` への書き込みに置換します。次の 58 ～ 60 行目を見つけます。

        outFrame[line * img_width + pixel].r = fabsf(sum_r);
        outFrame[line * img_width + pixel].g = fabsf(sum_g);
        outFrame[line * img_width + pixel].b = fabsf(sum_b);

    これを次と置換します。

        window_line_idx = top_idx;
        out_line[pixel].r =  fabsf(sum_r);
        out_line[pixel].g =  fabsf(sum_g);
        out_line[pixel].b =  fabsf(sum_b);

    画像が処理されたら、`out_line` が `outFrame` に書き込まれます。

        memcpy(outFrame+(line * img_width), out_line, img_width * sizeof(RGBPixel));

    これは、`image_width` を反復する for ループの後に配置されます。

これですべての変更が終了したので、最適化したコードを使用してハードウェア エミュレーションを実行できます。

## ハードウェア エミュレーションの実行

1. `Makefile` ディレクトリに移動して、次のコマンドでハードウェア エミュレーションを実行します。

   ```
   make run TARGET=hw_emu STEP=localbuf SOLUTION=1 NUM_FRAMES=1
   ```

次のように表示されます。

```
Processed 0.02 MB in 461.702s (0.00 MBps)

INFO: [Vitis-EM 22] [Wall clock time: 20:41, Emulation time: 1.61452 ms] Data transfer between kernel(s) and global memory(s)
convolve_fpga_1:m_axi_gmem1-DDR[0]          RD = 20.000 KB              WR = 20.000 KB       
convolve_fpga_1:m_axi_gmem2-DDR[0]          RD = 0.035 KB               WR = 0.000 KB
```

## ハードウェア エミュレーションのプロファイル サマリの表示

1. 次のコマンドを使用して、プロファイル サマリ レポートを表示します。

   ```
   make view_run_summary TARGET=hw_emu STEP=localbuf
   ```

   カーネル実行時間は、ベースライン パフォーマンスの 3.903 ms から 1.574 ms まで改善されました。これは、計算のためのすべてのグローバル メモリ アクセスを削除して、画像データと係数をローカル メモリに格納したからです。

2. プロファイル サマリ レポートからのパフォーマンスを次の表に追加します。

アップデートされた表は、次のようになります。カーネル実行時間が 2.48 倍改善されています。

| 演習               | Image Size | Time (HW-EM)(ms) | Reads (KB)      | Writes (KB) | Avg. Read (KB) | Avg. Write (KB) |  BW (MBps) |
| :----------------- | :--------- | ---------------: | --------------: | ----------: | -------------: | --------------: | ---------: |
| baseline           |     512x10 | 3.903            | 344             |        20.0 |          0.004 |           0.004 |    5.2     |
| localbuf           |     512x10 | 1.574 (2.48x)    | 21  (0.12x)     |        20.0 |          0.064 |           0.064 |    13      |
--------------------------------------------------------------------------------------------

[convolve_fpga_local]: ./images/convolve_fpga_local.png "ローカル メモリを使用した FPGA カーネル"
[convolve_variables]: ./images/convolve_variables.png "内部ループで使用される変数"
[localbuf_hwemu_profilesummary]: ./images/191_localbuf_hwemu_pfsummary_new_2.JPG "ローカル バッファー バージョンのハードウェア エミュレーションのプロファイル サマリ"
## 次のステップ

次は、[固定小数点データ型を使用して最適化](./fixedtype.md)して、FPGA の浮動小数点計算を最適化します。</br>

<hr/>
<p align="center"><b><a href="../../docs/vitis-getting-started/README.md">入門コースの初めに戻る</a> &mdash; <a href="./README.md">チュートリアルの初めに戻る</a></b></p>
<p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>
