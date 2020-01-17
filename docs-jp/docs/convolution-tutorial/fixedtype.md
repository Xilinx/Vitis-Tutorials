<table>
 <tr>
   <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2019.2 Vitis™ アプリケーション アクセラレーション開発フローのチュートリアル</h1><a href="https://github.com/Xilinx/SDAccel-Tutorials/branches/all">SDAccel™ 開発環境 2019.1 チュートリアルを参照</a></td>
 </tr>
 <tr>
 <td align="center"><h1>アクセラレーションした FPGA アプリケーションの最適化: たたみ込みの例</td>
 </tr>
</table>

# 4\. 固定小数点データ型を使用した最適化

前の演習では、カーネルとグローバル メモリ間のメモリ アクセスを最適化しました。この演習では、浮動小数点型を固定小数点型に変換して、カーネルの効率を改善します。

元のコードでは、合計と係数に浮動小数点が使用されています。ここでは、ap\_fixed\<16,9> 型 (9 ビットの符号付き整数に 7 ビットの小数) を使用します。このデータ型を選択したのは、アプリケーションに必要な精度を維持しつつ、パフォーマンスとリソース使用量を改善できるからです。

次のたたみ込みカーネルの内部ループを見てみます。

    for(int pixel = 0; pixel < img_width; ++pixel)
    {
        float sum_r = 0, sum_g=0, sum_b=0;
        for(int m = 0; m < coefficient_size; ++m)
        {
            for(int n = 0; n < coefficient_size; ++n)
            {
                int jj = pixel + n - center;
                if(jj >= 0 && jj < img_width)
                {
                    sum_r += window_mem[window_line_idx][jj].r * coef[m * coefficient_size + n];
                    sum_g += window_mem[window_line_idx][jj].g * coef[m * coefficient_size + n];
                    sum_b += window_mem[window_line_idx][jj].b * coef[m * coefficient_size + n];
                }
            }
            window_line_idx=(window_line_idx + 1) == MAX_FILTER ? 0 : window_line_idx + 1;
        }
        window_line_idx = top_idx;
        out_line[pixel].r =  fabsf(sum_r);
        out_line[pixel].g =  fabsf(sum_g);
        out_line[pixel].b =  fabsf(sum_b);
    }

この内部ループは `RGBPixel` オブジェクト (符号なしの char に浮動小数点の `coef` 配列) の個別メンバーを乗算します。この演算結果は、浮動小数点の変数 `sum_r`、`sum_g`、`sum_b` に格納し戻され、最後に `RGBPixel` に書き込まれます。これらの計算に基づくと、sum 引数で記述される最大数は、符号なしの char の最大値である 256 になります。この結果、16 ビット幅で整数側に 8 ビット使用される固定小数点データ型を使用できるようになります。

## カーネル コードの変更

> **ヒント:** カーネル ソース ファイルは `reference-files/fixedpoint` フォルダーに含まれています。必要に応じて、リファレンスとしてご利用ください。

`src/fixedpoint` から `convolve_fpga.cpp` ファイルを開いて、次のように変更します。

1. `ap_fixed.h` ヘッダーをファイルの冒頭に含めます。

        #include "ap_fixed.h"

2. `ap_fixed<16,9>` にマップされる固定小数点型用に typedef を作成します。

        typedef ap_fixed<16,9> fixed;

3. 次の行 (39 行目) を見つけます。

        float coef[MAX_FILTER * MAX_FILTER];

   これを次に置換します。

        fixed coef[MAX_FILTER * MAX_FILTER];

   これにより、`coef` 配列のデータ型が固定小数点の配列に変更されます。

4. `coef` のデータ型は `coefficient` とは異なるので、`memcpy` コマンドは Vivado® 高位合成 (HLS) ツールではサポートされません。代わりに、`for` ループ インプリメンテーションに変換します。次の 40 行目を見つけます。

        memcpy(coef, coefficient, coefficient_size * sizeof(float));

   これを次に置換します。

        int num_coefficients = coefficient_size * coefficient_size;
        for(int i = 0; i < num_coefficients; i++) {
            coef[i] = coefficient[i];
        }

   同じ演算が実行されますが、`coefficient` 配列の浮動小数点エレメントが `coef` 配列で固定小数点エレメントに変換されます。

5. 次に、`sum_r`、`sum_g`、`sum_b` 変数のデータ型を固定小数点型に変換します。次の 70 行目を見つけます。

        float sum_r = 0, sum_g=0, sum_b=0;

   これを次に置換します。

        fixed sum_r = 0, sum_g=0, sum_b=0;

## ハードウェア エミュレーションの実行

1. `design/makefile` ディレクトリに移動します。

2.  次のコマンドを使用してハードウェア エミュレーションを実行します。

   ```
   make run TARGET=hw_emu STEP=fixedpoint SOLUTION=1 NUM_FRAMES=1
   ```

   次のような結果になるはずです。

   ```
   Processed 0.02 MB in 108.788s (0.00 MBps)

   INFO: [Vitis-EM 22] [Wall clock time: 21:17, Emulation time: 0.510047 ms] Data transfer between kernel(s) and global memory(s)
   convolve_fpga_1:m_axi_gmem1-DDR[0]          RD = 20.000 KB              WR = 20.000 KB
   convolve_fpga_1:m_axi_gmem2-DDR[0]          RD = 0.035 KB               WR = 0.000 KB  
   ```

## ハードウェア エミュレーションのプロファイル サマリ レポートの表示

次のコマンドを使用して、プロファイル サマリ レポートを表示します。

```
make view_run_summary TARGET=hw_emu STEP=fixedpoint
```

カーネル実行時間が 0.46 ms に減っています。このようにかなり時間が削減されたのは、固定小数点演算を使用すると、ループの計算がパイプライン処理されるからです。このため、レイテンシ合計がかなり改善されています。

アップデートされた表は、次のようになります。カーネル実行時間が 3.4 倍改善されています。

| 演習             | Image Size | Time (HW-EM)(ms) | Reads (KB)      | Writes (KB) | Avg. Read (KB) | Avg. Write (KB) | BW (MBps)  |
| :---------------- | :--------- | ---------------: | --------------: | ----------: | -------------: | --------------: | ---------: |
| baseline          |     512x10 | 3.903            | 344             |        20.0 |          0.004 |           0.004 |    5.2     |
| localbuf          |     512x10 | 1.574 (2.48x)    | 21  (0.12x)     |        20.0 |          0.064 |           0.064 |    13      |
| fixedpoint data   |     512x10 | 0.46 (3.4x)      | 21              |        20.0 |          0.064 |           0.064 |    44      |
-----------------------------------------------------------------------------------

[fixedtype_hwemu_profilesummary]: ./images/191_fixedtype_hwemu_pfsummary_new_2.jpg "固定小数点型バージョンのハードウェア エミュレーションのプロファイル サマリ"
## 次のステップ

次のセクションでは、1 つの関数をサブ関数に分割することで、異なる関数間のタスクレベルの並列処理を達成する方法について説明します。この場合、[データフローを最適化](./dataflow.md)します。</br>

<hr/>
<p align="center"><b><a href="../../docs/vitis-getting-started/README.md">入門コースの初めに戻る</a> &mdash; <a href="./README.md">チュートリアルの初めに戻る</a></b></p>
<p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>
